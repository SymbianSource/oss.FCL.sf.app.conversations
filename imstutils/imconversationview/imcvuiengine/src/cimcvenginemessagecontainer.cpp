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
* Description:  message container class implementation
*
*/

#include "cimcvenginemessagecontainer.h"
#include "mimcvenginemessageobserver.h"
#include "mimcvenginechatinterface.h"           // KSendBufferId
#include "cimcvenginemessagebase.h"
#include <escapeutils.h>
#include "imcvuiliterals.h"
#include "imcvlogger.h"
#include <collate.h>
#include "mimcvenginebuffermemoryhandler.h"
#include "mimcvenginenewmsgobserver.h"

// needed to skip domains in domain-neutral comparison (TUint version)
const TUint KAtUInt( '@' );

// general colon needed in various places
_LIT( KColon, ":" );

// general slash needed in various places
_LIT( KSlash, "/" );

// needed to skip domains in domain-neutral comparison
_LIT( KAt, "@" );

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::CIMCVEngineMessageContainer
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer::CIMCVEngineMessageContainer( 
                                MIMCVEngineBufferMemoryHandler& aMemoryHandler )
: iMemoryHandler( aMemoryHandler )
	{
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::~CIMCVEngineMessageContainer
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer::~CIMCVEngineMessageContainer()
	{
	

	for( TInt a( 0 ); a < iMessageContainer.Count(); ++a )
	    {
	    if( iMessageContainer[ a ]->DecreaseOwnerCount() )
	        {
	        delete iMessageContainer[ a ];
	        }
	    }	
    iMessageContainer.Reset();

	
	delete iTargetId;
	delete iBuddyDisplayName;
	delete iDecodedTargetId;
	delete iEditerMessge;
	delete iContactLink;
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer* CIMCVEngineMessageContainer::NewL(
                                TInt aServiceId, 
                                const TDesC& aTargetId,
                                const TDesC& aDisplayName, 
                                const TDesC8& aContactLink,
                                MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
                                TContainerType aType )
	{
	CIMCVEngineMessageContainer* self = CIMCVEngineMessageContainer::NewLC(  
	                                                        aServiceId, 
	                                                        aTargetId,
	                                                        aDisplayName, 
	                                                        aContactLink,
	                                                        aMemoryHandler, 
	                                                        aType );
	CleanupStack::Pop( self );
	return self;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::NewLC
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageContainer* CIMCVEngineMessageContainer::NewLC(                                  
                                 TInt aServiceId, 
                                 const TDesC& aTargetId,
                                 const TDesC& aDisplayName,
                                 const TDesC8& aContactLink,
                                 MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
                                 TContainerType aType )
	{
	CIMCVEngineMessageContainer* self = 
	                       new ( ELeave ) CIMCVEngineMessageContainer( aMemoryHandler );
	CleanupStack::PushL( self );
	self->ConstructL( aServiceId, aTargetId,aDisplayName,aContactLink, aType );
	return self;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageContainer::ConstructL( 
                                      TInt /*aServiceId*/, 
                                      const TDesC& aTargetId,
                                      const TDesC& aDisplayName,
                                      const TDesC8& aContactLink,
                                      TContainerType aType )
	{
 	iTargetId = aTargetId.AllocL();
 	iBuddyDisplayName = aDisplayName.AllocL();
	iContactLink = aContactLink.Alloc();
	iDecodedTargetId = EscapeUtils::EscapeDecodeL( *iTargetId );
    ResolveType( aType );   

	}	

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::UnreadCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineMessageContainer::UnreadCount( 
                    MIMCVEngineMessageReadInterface::TUnreadFilter aUnreadFilter ) const
	{
	TInt countOfAll = iMessageContainer.Count();
	TInt unreadCount = countOfAll - iUnreadOffset;
	if( aUnreadFilter != EUnreadAll )
	    {
        for( TInt a = countOfAll - 1; a >= iUnreadOffset; --a )	
            {
            if( iMessageContainer[ a ]->MessagerType() == MIMCVEngineMessage::EMessageSent )
                {
                unreadCount--;
                }
            }
	    }
    return unreadCount;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::StoreEditerMessageL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageContainer::StoreEditerMessageL(const TDesC& aEditorMessage )
	{
	if( iEditerMessge)
	    {
	    delete iEditerMessge;
	    iEditerMessge = NULL;
	    }
	iEditerMessge = aEditorMessage.AllocL();  
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::GetEditerMessage
// ( Other items commented in header )
//-----------------------------------------------------------------------------

const TDesC& CIMCVEngineMessageContainer::GetEditerMessage() 
    {
    if( iEditerMessge)
        {
        return *iEditerMessge;
        }
    return KNullDesC;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::MessageCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineMessageContainer::MessageCount() const
    {
    // Offset is index to first unread message. It is exatly same as count of
    // read messages.
    return iUnreadOffset; 
    }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::Read
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageContainer::Read( TReadType aReadType, TInt aAmount /* = 0 */ )
	{
	TInt messageCount( iMessageContainer.Count() );
	TInt originalUnreadCount = UnreadCount( );
	
	switch( aReadType )
		{
		case EReadAll:
			iUnreadOffset = messageCount;
			break;
		case EReadAmount:
			iUnreadOffset+= aAmount;
			break;
		case EReadOne:
			++iUnreadOffset;
			break;
		default:		    
			return;
		}

	// Cannot be more than messageCount
	if( iUnreadOffset > messageCount )
		{
		iUnreadOffset = messageCount; 
		}
	
	// Inform observers only if unreadcount is changed.
	if( originalUnreadCount != UnreadCount() )
	    {
        if (( iChatObserver ) && (!iActiveChat))
            {
            TRAP_IGNORE( iChatObserver->HandleChatEventL( 
                                MIMCVEngineNewMsgObserver::EUnreadCountChanged ) );
            }	    
	    }
	
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::Message
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage& CIMCVEngineMessageContainer::Message( TInt aIndex ) const
	{
	//__CHAT_ASSERT_DEBUG( aIndex >= 0 && aIndex < MessageCount() );
	return *iMessageContainer[ aIndex ];
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ReadNextUnread
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage& CIMCVEngineMessageContainer::ReadNextUnread()
	{
	//__CHAT_ASSERT_DEBUG( UnreadCount() != 0 );
    Read( EReadOne );
	return *iMessageContainer[ MessageCount() - 1 ];
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ReadUnreadFromIndex
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage& CIMCVEngineMessageContainer::ReadUnreadFromIndex( TInt aIndex )
	{
	//__CHAT_ASSERT_DEBUG( UnreadCount() != 0 );
	//__CHAT_ASSERT_DEBUG( aIndex >= 0 && aIndex < iMessageContainer.Count() );
    Read( EReadOne );
	return *iMessageContainer[aIndex];
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageContainer::SetObserver( MIMCVEngineMessageObserver* aObserver )
	{
	iObserver = aObserver;
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TPtrC CIMCVEngineMessageContainer::Name() const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageContainer::Name Start"));
    if( iBuddyDisplayName && iBuddyDisplayName->Length())
        {
        IM_CV_LOGS(TXT("to_del CIMCVEngineMessageContainer::Name iBuddyDisplayName= %S"),&(*iBuddyDisplayName) );
        //IM_CV_LOGS(TXT("to_del CIMCVEngineMessageContainer::Name iBuddyDisplayName length = %d"),iBuddyDisplayName->Length() );
        return *iBuddyDisplayName;
        }
     if( iTargetId )
         {
         IM_CV_LOGS(TXT("to_del  CIMCVEngineMessageContainer::target iTargetId= %S"), &(*iTargetId));
         return DisplayId( *iTargetId ); 
         }
     return KNullDesC();
     }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::DisplayName
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
TDesC CIMCVEngineMessageContainer::DisplayName() const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageContainer::DisplayName Start" ));
    if( iBuddyDisplayName && iBuddyDisplayName->Length())
        {
        IM_CV_LOGS(TXT("CIMCVEngineMessageContainer::DisplayName EChatContainer: %s"), &(*iBuddyDisplayName));
        return *iBuddyDisplayName;
        }
    return KNullDesC();
    }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetName
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
void  CIMCVEngineMessageContainer::SetNameL(const TDesC& aDisplayName)
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageContainer::SetNameL Start"));
    //if the iBuddydisplayName is same as aDisplayName 
    // then no need to set it again.
    if(iBuddyDisplayName && (aDisplayName.Compare(*iBuddyDisplayName)!=0))
        {
        delete iBuddyDisplayName;
        iBuddyDisplayName = NULL;
        IM_CV_LOGS(TXT("CIMCVEngineMessageContainer::SetName=%S"),&aDisplayName);
        iBuddyDisplayName = aDisplayName.AllocL();
        }
    }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetName
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
void  CIMCVEngineMessageContainer::SetLinkL(const TDesC8& aContactLink)
    {
    IM_CV_LOGS(TXT(" CIMCVEngineMessageContainer::SetLinkL Start"));
    //if the iContactLink is same as aContactLink 
    // then no need to set it again.
    if(iContactLink && (aContactLink.Compare(*iContactLink)!=0))
        {
        delete iContactLink;
        iContactLink = NULL;
        IM_CV_LOGS(TXT(" CIMCVEngineMessageContainer::SetLinkL=%S"),&aContactLink);
        iContactLink = aContactLink.AllocL();
        }
   
    }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ContactLink
// ( Other items commented in header )
//----------------------------------------------------------------------------- 

TPtrC8 CIMCVEngineMessageContainer::ContactLink() const
    {
    return *iContactLink;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::DeleteMessage
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageContainer::DeleteMessage( TInt aIndex )
    {
    MIMCVEngineMessage* target = &Message( aIndex );
    iMessageContainer.Remove( aIndex );
    iContainerSizeInBytes -= target->SizeInBytes();
    --iUnreadOffset;
    
    if( iObserver )
        {
        iObserver->HandleMessageEvent( MIMCVEngineMessageObserver::EMessageDeleted, 
                                       aIndex );
        }
        
    if( target->DecreaseOwnerCount() )
        {
        delete target;
        }
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::AppendL
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageContainer::AppendL( MIMCVEngineMessage* aMessage, TBool aSharedOwnership )
	{
	if( !aMessage )
		{
		User::Leave( KErrArgument );
		}
		
    if( !aSharedOwnership )
        {
	    CleanupDeletePushL( aMessage );
        }
	
	// Check memory status
	// if failes, leave with KErrNoMemory
	if( !iMemoryHandler.FreeMemoryIfNeededL( aMessage->SizeInBytes() ) )
		{
		User::Leave( KErrNoMemory );
		}	    
		
	iMessageContainer.AppendL( aMessage );
	iContainerSizeInBytes += aMessage->SizeInBytes();
	
    if( !aSharedOwnership )
        {
	    CleanupStack::Pop( aMessage );
        }

	aMessage->IncreaseOwnerCount();
	
	aMessage->SetContainerInfo( this );

    IM_CV_LOGS(TXT( "Sender %s, Recipient %s" ),                           
             &aMessage->Sender(),
             &aMessage->Recipient() );
    	                                                            
    
	if( iObserver )
		{
		iObserver->HandleMessageEvent( MIMCVEngineMessageObserver::ENewMessage );
		}
		
    if ( ( iChatObserver ) && (!iActiveChat) )
        {
        iChatObserver->HandleChatEventL( MIMCVEngineNewMsgObserver::EUnreadCountChanged, 
                                        aMessage );
        }
   }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ServerAddress
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//
TTime CIMCVEngineMessageContainer::Time()
    {
    TTime time = Time::NullTTime();
    TInt count = iMessageContainer.Count();
    if( count == 0 )
        {
        // no messages, return null
        return time;
        }
    return iMessageContainer[count - 1]->TimeStamp();
    }


	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::Target
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
const TDesC& CIMCVEngineMessageContainer::Target() const
	{
	if( !iTargetId )
	    {
	    return KNullDesC;
	    }
	return *iTargetId;
	}
	

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::OrderUid
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TInt CIMCVEngineMessageContainer::OrderUid( const CIMCVEngineMessageContainer& aSourceA, 
									const CIMCVEngineMessageContainer& aSourceB )
	{
    
    // points to user part of id
    TPtrC ptrId1( aSourceA.ComparableTarget() );
    TPtrC ptrId2( aSourceB.ComparableTarget() );	

    // find out if we have domains in the ids
    TInt domainPos1( ptrId1.Locate( KAtUInt ) );
    TInt domainPos2( ptrId2.Locate( KAtUInt ) );

    TBool domainIn1( KErrNotFound != domainPos1 );
    TBool domainIn2( KErrNotFound != domainPos2 );

    // points to domains in the neutral id
    TPtrC ptrDom1( KNullDesC );
    TPtrC ptrDom2( KNullDesC );

    // points to user parts in the neutral id
    TPtrC ptrUid1( ptrId1 );
    TPtrC ptrUid2( ptrId2 );

    // separate user id parts and domain parts
    if ( domainIn1 )
        {
        ptrDom1.Set( ptrId1.Mid( domainPos1 + 1) );
        ptrUid1.Set( ptrId1.Mid( 0, domainPos1 ) );
        }

    if ( domainIn2 )
        {
        ptrDom2.Set( ptrId2.Mid( domainPos2 + 1) );
        ptrUid2.Set( ptrId2.Mid( 0, domainPos2 ) );
        }

    // Create custom collation method to ignore punctuations
    // index 0 gets the default method
    TCollationMethod collation =
        *Mem::CollationMethodByIndex( 0 );
    collation.iFlags |= TCollationMethod::EIgnoreNone;

    // domains are compared only when it is really needed
    // check if userid part is the same in both ids   
    TInt idResult = ptrUid1.CompareC( ptrUid2, KCollationLevel, &collation );

	if( idResult != 0 )
		{
		return idResult;
		}

	// id part is same, we have to compare domain

    // If domain comparison is neutral and one id is without domain
    // -> Domains are same. Other situation domainResult stays valid.
    if ( domainIn1 ^ domainIn2 )
        {
        return 0;
        }
    else
    	{
    	return ptrDom1.CompareC( ptrDom2, KCollationLevel, &collation );
    	}
    }
                                        
		
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ContainerType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessageReadInterface::TContainerType CIMCVEngineMessageContainer::ContainerType() const
    {
    return iContainerType;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ResolveType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageContainer::SetChatObserver( MIMCVEngineNewMsgObserver* aObserver )
    {
    iChatObserver = aObserver;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ResolveType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageContainer::ResolveType( TContainerType aType )
    {
    if( aType != ENoneContainer )
        {
        iContainerType = aType;
        }
    else if( iTargetId->Compare( KSendBufferId ) == 0 )
        {
        iContainerType = ESendContainer;
        }    
    else
        {
        iContainerType = EChatContainer;
        }
    }
    
    
        
// -----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::TargetId
// ( Other items commented in header )
// -----------------------------------------------------------------------------
//
MIMCVEngineMessageContainerInfo& CIMCVEngineMessageContainer::TargetId()
    {
    return *this;
    }
    
// -----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::Lock
// locks messagecontainer if needed.
// -----------------------------------------------------------------------------
//
void CIMCVEngineMessageContainer::Lock( TBool aLocked )
    {
    iLocked = aLocked;
    }
 
// -----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ComparableTarget
// ( Other items commented in header )
// -----------------------------------------------------------------------------
//    
const TDesC& CIMCVEngineMessageContainer::ComparableTarget() const
    {
    if ( !iDecodedTargetId )
        {
        return KNullDesC;
        }
    return *iDecodedTargetId;
    }
    
// -----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::AllMessagesCount
// ( Other items commented in header )
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineMessageContainer::AllMessagesCount() const
    {
    return iMessageContainer.Count();
    }
    
// -----------------------------------------------------------------------------
// CCABufferManager::NotifyContactForPending
// ( Other items commented in header )
// -----------------------------------------------------------------------------
//    
MIMCVEngineMessage& CIMCVEngineMessageContainer::MessageFromAll( TInt aIndex ) const
    {
	//__CHAT_ASSERT_DEBUG( aIndex >= 0 && aIndex < AllMessagesCount() );
	return *iMessageContainer[ aIndex ];
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::DeleteMessageFromAll
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//
void CIMCVEngineMessageContainer::DeleteMessageFromAll( TInt aIndex )
    {        
    MIMCVEngineMessage* target = &MessageFromAll( aIndex );
    iMessageContainer.Remove( aIndex );
    iContainerSizeInBytes -= target->SizeInBytes();
    if( iUnreadOffset > aIndex )
        {
        --iUnreadOffset;        
        }
    
    if( iObserver )
        {
        iObserver->HandleMessageEvent( MIMCVEngineMessageObserver::EMessageDeleted, 
                                       aIndex );
        }
        
    if( target->DecreaseOwnerCount() )
        {
        delete target;
        }
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::IsLocked
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//    
TBool CIMCVEngineMessageContainer::IsLocked() const
    {
    return iLocked ;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ContainerSizeInBytes
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//
TInt CIMCVEngineMessageContainer::ContainerSizeInBytes() const
    {
    return iContainerSizeInBytes;
    }


//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetActive
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//
void CIMCVEngineMessageContainer::SetActive( TBool aActiveChat )    
	{
	iActiveChat = aActiveChat;		
	}    


//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::IsActive
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
//
TBool CIMCVEngineMessageContainer::IsActive()   
	{
	return iActiveChat;		
	}    



// -----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::DisplayId
// -----------------------------------------------------------------------------
//
TPtrC CIMCVEngineMessageContainer::DisplayId( const TDesC& aId, TBool aDomainHiding )
	{
	TPtrC ret( aId );

	// locate ":" for userid, groupid.
	// locate "/" for list id.
	TInt pos = aId.FindC( aDomainHiding ? KSlash : KColon );

	if ( ( pos != KErrNotFound) && ( pos != aId.Length()-1) )
		{
		// contains the special character, and it is not the last char
		// remove everything before the special char (including the char)
		ret.Set( aId.Mid( pos + 1 ) );
		}
	if( aDomainHiding)
		{
		// remove also the domain part
		TInt domainPos = ret.FindC( KAt );
		if ( ( domainPos != KErrNotFound ) && ( domainPos != 0 ) )
			 {
			 ret.Set( ret.Mid( 0, domainPos ) );
			 }

		} 

	return ret;
	}

		   
// end of file
