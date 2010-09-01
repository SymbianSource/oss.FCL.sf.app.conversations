/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  container class for im messages
*
*/


#include "cimcvenginechatcontainer.h"

#include "cimcvenginemessagecontainer.h"
#include "mimcvenginenewmsgobserver.h"
#include "mimcvenginemessage.h"
#include <hal.h>
#include "imcvlogger.h"
#include "mimcvenginechatlistobserver.h"


#include <hal.h>

const TInt KMinAvailableMemory = 0x32000; // 200kb

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::CIMCVEngineChatContainer
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineChatContainer::CIMCVEngineChatContainer(TInt aServiceId):
    iServiceId( aServiceId )
	{	
	}

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::CIMCVEngineChatContainer
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineChatContainer::~CIMCVEngineChatContainer()
	{
	iChatContainer.ResetAndDestroy();	
	iSendContainer.ResetAndDestroy();
	iNewMsgObserver = NULL;
	iChatListObservers.Reset();
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineChatContainer* CIMCVEngineChatContainer::NewL(TInt aServiceId)
	{
	CIMCVEngineChatContainer* self = 
	        new ( ELeave ) CIMCVEngineChatContainer(aServiceId);

	return self;
	}	
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::MessageReadInterfaceL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessageReadInterface& CIMCVEngineChatContainer::MessageReadInterfaceL(                  
                 TInt aServiceId, 
                 const TDesC& aTargetId,
                 const TDesC& aDisplayName,
                 const TDesC8& aContactLink,
                 MIMCVEngineMessageReadInterface::TContainerType aType )
	{
	return *ChatL( ChatL( aServiceId, aTargetId,aDisplayName,aContactLink,aType ) );
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::MessageWriteInterfaceL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessageWriteInterface& CIMCVEngineChatContainer::MessageWriteInterfaceL(                 
                TInt aServiceId, 
                const TDesC& aTargetId,
                const TDesC& aDisplayName,
                const TDesC8& aContactLink,
                MIMCVEngineMessageReadInterface::TContainerType aType )
	{
	return *ChatL( ChatL( aServiceId, aTargetId, aDisplayName, aContactLink , aType ) );
	}

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::DeleteChatL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineChatContainer::DeleteChatL(  
		                            TInt aServiceId,
		                            const TDesC& aTargetId )
    {
    // Find out if chat exist or not. ChatExists() not used, 
    // because we need index.
    CIMCVEngineMessageContainer* tempChat = CreateContainerLC(                         
                        aServiceId, 
                        aTargetId,
                        KNullDesC,
                        KNullDesC8,
                        *this, 
                        MIMCVEngineMessageReadInterface::ENoneContainer );
	TInt index( FindChat( *tempChat ) );
	CleanupStack::PopAndDestroy( tempChat );

    // If index is positive  value, Chat exists and we can remove it.	
	if( index >= 0 )
	    {
        CIMCVEngineMessageContainer* target = ChatL( index, EFalse );

        // Get count of pending messages before removing chat
        TInt ignore;
        TInt pendignMsgs = MessagesPendingCount( ignore );

        RemoveChat( index );
        
        // Check if removed chat had unread messages
        if( pendignMsgs != MessagesPendingCount( ignore ) )
            {
            // Notify about changed unread count
            HandleChatEventL( EUnreadCountChanged );
            }
            
        // Observers are notified, delete target.
        delete target;
	    }    
	else if( index != KErrNotFound ) 
	    {
	    // some other error than not found occurred. Inform caller.
	    User::Leave( index );
	    }	   
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::CloseAllContainers
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineChatContainer::CloseAllContainers()
    {
    IM_CV_LOGS(TXT("CIMCVEngineChatContainer::CloseAllContainers() START") );
    TInt count( iChatListObservers.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
    	IM_CV_LOGS(TXT("CIMCVEngineChatContainer::CloseAllContainers() closing container ") );    
        iChatListObservers[ a ]->HandleChatListEvent( iServiceId, EChatAllItemDeleted, NULL  );
        }   
	//iChatContainer.ResetAndDestroy();
	IM_CV_LOGS(TXT("CIMCVEngineChatContainer::CloseAllContainers() End") );
	}
  
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::ChatCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::ChatCount() const
    {
    return iChatContainer.Count();
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::ChatAt
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessageReadInterface& CIMCVEngineChatContainer::ChatAt( TInt aIndex ) const
    {
    //__CHAT_ASSERT_DEBUG( aIndex >= 0 && aIndex < ChatCount() )
    return *iChatContainer[ aIndex ];    
    }
    

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::RegisterNewMsgObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineChatContainer::RegisterNewMsgObserver( MIMCVEngineNewMsgObserver* aObserver )
    {
    iNewMsgObserver = aObserver;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::UnregisterNewMsgObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineChatContainer::UnregisterNewMsgObserver()
    {
    iNewMsgObserver = NULL;
    }
  
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::RegisterChatListObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::RegisterChatListObserver( MIMCVEngineChatListObserver* aObserver )
    {
    TInt index = iChatListObservers.Find( aObserver );
    if( index == KErrNotFound )
        {
        return iChatListObservers.Append( aObserver );
        }
    return KErrAlreadyExists;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::UnregisterChatListObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::UnregisterChatListObserver( MIMCVEngineChatListObserver* aObserver )
    {
    TInt index = iChatListObservers.Find( aObserver );
    if( index >= 0 )
        {
        iChatListObservers.Remove( index );
        return KErrNone;
        }
    return index;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::PendingMessagesCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::MessagesPendingCount( TInt& aCountOfChats,
    MIMCVEngineMessageReadInterface::TUnreadFilter aUnreadFilter
    /*= MIMCVEngineMessageReadInterface::EUnreadAll*/ ) const
    {
    TInt count( ChatCount() );
    TInt pendingCount( 0 );
    aCountOfChats = 0;
    for( TInt a( 0 ); a < count; ++a )
        {
        TInt unreadCount = ChatAt( a ).UnreadCount( aUnreadFilter );
        if( unreadCount > 0 )
            {
            pendingCount += unreadCount;
            ++aCountOfChats;
            }        
        }
    return pendingCount;
    }

    
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::MessageInterface
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TBool CIMCVEngineChatContainer::FreeMemoryIfNeededL( TInt aSize )
	{
	//CHAT_DP_FUNC_ENTER( "CIMCVEngineChatContainer::FreeMemoryIfNeededL" );
	
    TInt halSize;
    TInt biggestBlock;

    HAL::Get( HALData::EMemoryRAMFree, halSize );	
    TInt maxHeap = Min( User::Heap().MaxLength(), halSize );	
	TInt available = maxHeap - ( User::Heap().Size() - User::Heap().Available( biggestBlock ) );
	TInt memoryNeed = KMinAvailableMemory + aSize;
		
    TTime oldestUnread;
    TInt unreadIndex;
    
    IM_CV_LOGS(TXT( "CIMCVEngineChatContainer::FreeMemoryIfNeededL, heapSize = %d"), maxHeap );
    IM_CV_LOGS(TXT( "****heapSize, available, need = %d, %d, %d"), 
                              maxHeap, available, memoryNeed );
        
    TBool ready = EFalse;
    TBool memLowNotified = EFalse;
    while( !ready )
        {
        TBool freeMemoryNeeded( EFalse );
        if( iExternalMemoryHandler )
            {
            freeMemoryNeeded = !iExternalMemoryHandler->FreeMemoryIfNeededL( aSize );
            }
        if( freeMemoryNeeded || memoryNeed > available )
            {
            // we have to react.
            TInt count = ChatCount();
            
            // Check how much memory can be made free
            TInt freeableBytes = 0;
            for( TInt i = 0; i < count; ++i )
                {
                TBool locked = iChatContainer[i]->IsLocked();
                if ( iChatContainer[i]->AllMessagesCount() && !locked )
                    {
                    freeableBytes += iChatContainer[i]->ContainerSizeInBytes();
                    }
                }
            
            if ( ( ( available + freeableBytes ) < memoryNeed )
                && !memLowNotified )
                {
                // Can't free enough memory
                return EFalse;
                }
            
            oldestUnread.HomeTime();
            unreadIndex = KErrNotFound;            
            
            for( TInt a( 0 ); a < count; ++a )
                {
                MIMCVEngineMessageReadInterface& chat = ChatAt( a );
                TBool locked = iChatContainer[ a ]->IsLocked();
                if( iChatContainer[ a ]->AllMessagesCount() && !locked )
                    {
                    MIMCVEngineMessage& message = iChatContainer[ a ]->MessageFromAll( 0 );
                    if( oldestUnread > message.TimeStamp() )
                        {
                        oldestUnread = message.TimeStamp();
                        unreadIndex = a;                        
                        }
                    }
                }
                
            if( unreadIndex == KErrNotFound )
                {
                return EFalse;
                }
            else
                {
                // Inform observers about memory handling
                if ( !memLowNotified )
                    {
                    HandleChatEventL( EMemoryLow );
                    memLowNotified = ETrue;
                    }                
                iChatContainer[ unreadIndex ]->DeleteMessageFromAll( 0 );
                }
	        available = maxHeap - ( User::Heap().Size() - User::Heap().Available( biggestBlock ) );
            IM_CV_LOGS(TXT( "****heapSize, available, need = %d, %d, %d"), 
                                      maxHeap, available, memoryNeed );
            }
        else
            {
            ready = ETrue;
            }                    
        }
	//CHAT_DP_FUNC_DONE( "CIMCVEngineChatContainer::FreeMemoryIfNeededL" );           
	return ETrue;
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::HandleChatEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineChatContainer::HandleChatEventL( TChatEventType aEvent, 
                                        MIMCVEngineMessage* aMessage )
    {
    if( iNewMsgObserver )
        {
        iNewMsgObserver->HandleChatEventL( aEvent, aMessage );
        }
    }


//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::ChatL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::ChatL(                 
                 TInt aServiceId, 
                 const TDesC& aTargetId,
                 const TDesC& aDisplayName,
                 const TDesC8& aContactLink,
                 MIMCVEngineMessageReadInterface::TContainerType aType 
                               /*= MIMCVEngineMessageReadInterface::ENoneContainer*/ )
	{
    IM_CV_LOGS(TXT("CIMCVEngineChatContainer::ChatL aName= %S"),&aDisplayName );

    IM_CV_LOGS(TXT("CIMCVEngineChatContainer::ChatL aLink= %S"),&aContactLink);  

	CIMCVEngineMessageContainer* tempChat = 
	     CreateContainerLC( aServiceId, aTargetId, aDisplayName, aContactLink, *this, aType );
		
	TInt index( FindChat( *tempChat ) );
	if( index == KErrNotFound )
		{
        IM_CV_LOGS(TXT("CIMCVEngineChatContainer::ChatL,Chat not found creating with aName %S"),&aDisplayName );  
        IM_CV_LOGS(TXT("CIMCVEngineChatContainer::ChatL,Chat not found creating with xspId %S"),&aTargetId );  
		
		TLinearOrder< CIMCVEngineMessageContainer > order( 
		                                    CIMCVEngineMessageContainer::OrderUid );
		// User::LeaveIfError returns value if value is positive.
		iLatestType = tempChat->ContainerType();
		switch( iLatestType )
		    {
		    case MIMCVEngineMessageReadInterface::EChatContainer:
		        {
                User::LeaveIfError( iChatContainer.InsertInOrder( tempChat, 
                                                                  order ) );			                                                  
		        index = FindChat( *tempChat );
    	        iChatContainer[ index ]->SetChatObserver( this );	
    	        
    	        HandleChatListEvent( EChatItemAdded, *tempChat );
                break;
		        }
		    case MIMCVEngineMessageReadInterface::ESendContainer:
		        {
                User::LeaveIfError( iSendContainer.InsertInOrder( tempChat, 
                                                                  order ) );
		        
		        index = FindChat( *tempChat );
                break;
		        }
		    default:
		        {
		        User::Leave( KErrArgument );
		        break;
		        }
		    }		
		CleanupStack::Pop( tempChat );
		}
	else
	    {
	    // if contact link is null
	    if(!(iChatContainer[ index ]->ContactLink().Length()))
	        {
	        if(	(aContactLink.Length() > 0) && (iLatestType == MIMCVEngineMessageReadInterface::EChatContainer))
	            {		
	            iChatContainer[ index ]->SetLinkL(aContactLink);
	            }
	        }
	    CleanupStack::PopAndDestroy( tempChat );
	    }
		
	return User::LeaveIfError( index );
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::ChatL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer* CIMCVEngineChatContainer::ChatL( TInt aIndex, 
                                              TBool aResetLatest /* = ETrue*/ )
    {
    MIMCVEngineMessageReadInterface::TContainerType type = iLatestType;
    
    if( aResetLatest )
        {
        iLatestType = MIMCVEngineMessageReadInterface::ENoneContainer;
        }
    
    switch( type )
        {
        
        case MIMCVEngineMessageReadInterface::EChatContainer:
            {
            return iChatContainer[ aIndex ];
            }
        case MIMCVEngineMessageReadInterface::ESendContainer:
            {
            return iSendContainer[ aIndex ];
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    // Can never get this far
    //__CHAT_ASSERT_DEBUG( EFalse );
    return NULL;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::RemoveChatL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineChatContainer::RemoveChat( TInt aIndex, 
                                   TBool aResetLatest /* = ETrue */ )
    {
    MIMCVEngineMessageReadInterface::TContainerType type = iLatestType;
    
    if( aResetLatest )
        {
        iLatestType = MIMCVEngineMessageReadInterface::ENoneContainer;
        }
    
    switch( type )
        {
        case MIMCVEngineMessageReadInterface::EChatContainer:
            {
            HandleChatListEvent( EChatItemDeleted, *iChatContainer[aIndex] );
            iChatContainer.Remove( aIndex );            
            break;
            }
        case MIMCVEngineMessageReadInterface::ESendContainer:
            {
            
            iSendContainer.Remove( aIndex );
            break;
            }
        default:
            {
            break;
            }
        }    
    }
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::FindChat
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineChatContainer::FindChat( const CIMCVEngineMessageContainer& aChat )
	{
	
	TInt index( 0 );
    TLinearOrder< CIMCVEngineMessageContainer > order( CIMCVEngineMessageContainer::OrderUid );
    TInt status = iChatContainer.FindInOrder( &aChat, index, order );
    if( status == KErrNone )
        {
        iLatestType = MIMCVEngineMessageReadInterface::EChatContainer;
        return index;
        }
                   
    iLatestType = MIMCVEngineMessageReadInterface::ESendContainer;
    status = iSendContainer.FindInOrder( &aChat, index, order );
    return ( status == KErrNone ? index : status );
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineChatContainer::CreateContainerLC
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer* CIMCVEngineChatContainer::CreateContainerLC(                               
                              TInt aServiceId, 
                              const TDesC& aTargetId,
                              const TDesC& aDisplayName,  
                              const TDesC8& aContactLink,
                              MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
                              MIMCVEngineMessageReadInterface::TContainerType aType )
    {
    
    CIMCVEngineMessageContainer* messageContainer = CIMCVEngineMessageContainer::NewL(                                                     
                                                    aServiceId, 
                                                    aTargetId,
                                                    aDisplayName,
                                                    aContactLink,
                                                    aMemoryHandler, 
                                                    aType );
    CleanupStack::PushL( messageContainer );
    return messageContainer;
    }


// ---------------------------------------------------------
// CIMCVEngineChatContainer::MemoryHandler
// ---------------------------------------------------------
//
MIMCVEngineBufferMemoryHandler& CIMCVEngineChatContainer::MemoryHandler() 
    {
    return *this;   
    }
        

// ---------------------------------------------------------
// CIMCVEngineChatContainer::HandleChatListEvent
// ---------------------------------------------------------
//
void CIMCVEngineChatContainer::HandleChatListEvent( TChatListEventType aEvent, 
								MIMCVEngineMessageReadInterface& aContainerInfo ) 
	{
 	IM_CV_LOGS (TXT("CIMCVENGINECHATCONTAINER :: HANDLECHATLISTEVENT"));
	// Inform all observer about chat event.
    //TInt count( iChatListObservers.Count() );  //kept for reference
    for( TInt a( 0 ); a <  iChatListObservers.Count() ; ++a )
        {
        iChatListObservers[ a ]->HandleChatListEvent( iServiceId, aEvent, &aContainerInfo );
        }	
	}
// end of file
