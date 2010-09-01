/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for Open ConversationList 
*
*/


// INCLUDE FILES

#include "cimcvengineopenchats.h"
#include "cimcvenginemessagehandler.h"
#include "mimcvenginechatinterface.h"
#include "mimcvenginemessagecontainerinfo.h"

#include <escapeutils.h>
#include <collate.h>
#include "imcvuiliterals.h"
#include "imcvlogger.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats : Ctor
//
// -----------------------------------------------------------------------------
//
CIMCVEngineOpenChats::CIMCVEngineOpenChats(CIMCVEngineMessageHandler& aMessageHandler, 
								MIMCVEngineChatInterface& aChatInterface) :    
     iActiveItem(-1),
     iMessageHandler(aMessageHandler),
     iChatInterface(aChatInterface)
    {
    }


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats : NewL
//
// -----------------------------------------------------------------------------
//
CIMCVEngineOpenChats* CIMCVEngineOpenChats::NewL(CIMCVEngineMessageHandler& aMessageHandler,
								MIMCVEngineChatInterface& aChatInterface,TInt aServiceId )
    {
    CIMCVEngineOpenChats* self = new ( ELeave ) CIMCVEngineOpenChats(aMessageHandler, aChatInterface);     
    self->ConstructL(aServiceId);       
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats : Destructor
//
// -----------------------------------------------------------------------------
//
CIMCVEngineOpenChats::~CIMCVEngineOpenChats()
    {
    iChatInterface.UnregisterNewMsgObserver();  
    iChatInterface.UnregisterChatListObserver(this);  
    iOpenChatsArray.Reset();
    }


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: GetDisplayNameL
// -----------------------------------------------------------------------------
//
TPtrC CIMCVEngineOpenChats::GetDisplayNameL(TInt aIndex) const
	{
	if ( (aIndex >= iOpenChatsArray.Count()) || (aIndex <0) )
	    {	      
	    User::LeaveIfError(KErrArgument);
	    }
    else 
		{		
		
		MIMCVEngineMessageReadInterface* arrayItem = 
						(MIMCVEngineMessageReadInterface*) iOpenChatsArray[aIndex];
		
		if (arrayItem)
			{
			return arrayItem->Name();				
			}			
		}
		
		return TPtrC(KNullDesC);
	}
	

// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: GetTargetIdL
// -----------------------------------------------------------------------------
//
const TDesC& CIMCVEngineOpenChats::GetTargetIdL(TInt aIndex) const
	{
	if ( ( aIndex < iOpenChatsArray.Count() ) && (aIndex >= 0 ) )
	    {	      
			MIMCVEngineMessageReadInterface* arrayItem = 
						(MIMCVEngineMessageReadInterface *)iOpenChatsArray[aIndex];
			if (arrayItem)
				{
				return arrayItem->TargetId().Target();	
				}     
			}
	return (KNullDesC);		
	}



// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: FindIndexL
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineOpenChats::FindIndexL(const TDesC& aItem)
	{
	
	HBufC* decodedItemId = EscapeUtils::EscapeDecodeL( aItem );	
	
	// Create custom collation method to ignore punctuations
    // index 0 gets the default method
    TCollationMethod collation =
        *Mem::CollationMethodByIndex( 0 );
        
    collation.iFlags |= TCollationMethod::EIgnoreNone;
	
	for (TInt index=0; index<iOpenChatsArray.Count(); index++)
		{
		TPtrC ptrId1( GetTargetIdL(index) );
		TPtrC ptrId2( decodedItemId->Des() );

	    // domains are compared only when it is really needed
	    // check if userid part is the same in both ids   
	    TInt idResult = ptrId1.CompareC( ptrId2, KCollationLevel, &collation );

		if( !idResult )
			{
			delete decodedItemId;
			return index;
			}
		}
	
	delete decodedItemId;
	return KErrNotFound;
	}
	
// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: Count
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineOpenChats::Count() const
	{
	return iOpenChatsArray.Count();
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: SetActiveItemL
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::SetActiveItemL( const TInt aIndex )
	{
	
	if ( (aIndex >= iOpenChatsArray.Count()) || (aIndex <0) )
	    {	      
	    User::LeaveIfError(KErrArgument);
	    }
	    
	iActiveItem = aIndex;

	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: GetActiveItem
// -----------------------------------------------------------------------------
//
const TDesC& CIMCVEngineOpenChats::GetActiveItem() const
	{
	
	if ( (iActiveItem < iOpenChatsArray.Count()) && (iActiveItem >= 0) )
	    {	      
	    MIMCVEngineMessageReadInterface *arrayItem = 
	    (MIMCVEngineMessageReadInterface *)iOpenChatsArray[iActiveItem];
	    if (arrayItem)
	        {
	        return arrayItem->TargetId().Target();	
	        }
	    }
	return (KNullDesC); 
	
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: IsMsgPendingL
// -----------------------------------------------------------------------------
//
TBool CIMCVEngineOpenChats::IsMsgPendingL( const TInt aIndex ) const
	{
	if(aIndex >= (iOpenChatsArray.Count()) || aIndex <0)
	    {	      
	    User::LeaveIfError(KErrArgument);
	    }
    else 
		{
		MIMCVEngineMessageReadInterface *arrayItem = 
								(MIMCVEngineMessageReadInterface *)iOpenChatsArray[aIndex];
		if (arrayItem)
			{
			return arrayItem->UnreadCount();        	
			}
		}
	return EFalse;			
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: ConstructL
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::ConstructL( TInt aServiceId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineOpenChats ::CONSTRUCTL () ") );
	iChatInterface.RegisterNewMsgObserver( this ); 
	iChatInterface.RegisterChatListObserver( this );	

	RArray<SIMCacheChatItem> chatItem = iMessageHandler.GetChatListL( aServiceId ); 
	IM_CV_LOGS(TXT("CIMCVEngineOpenChats :: CHAT ITEMS COUNT %d"), chatItem.Count () );
	
	for (int index=0; index<chatItem.Count(); index++)
		{
		
		iChatInterface.MessageReadInterfaceL(chatItem[index].iServiceId,
		                                           *chatItem[index].iBuddyId );
		
		delete chatItem[index].iBuddyId;	    
    			
		}
	chatItem.Reset();
	chatItem.Close();
	}
	

// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: Insert
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::Insert( MIMCVEngineMessageReadInterface* aContainerInfo )
	{	
    iOpenChatsArray.Append( aContainerInfo );
    
    if (iChatObserver)
	    {    
	    TRAP_IGNORE( iChatObserver->HandleChangeL(iMessageHandler.ServiceId(), iOpenChatsArray.Count()-1, ENewChat) );	  
	    }
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: RemoveL
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::RemoveL( MIMCVEngineMessageReadInterface& aContainerInfo )
	{
	TInt index = FindIndexL(aContainerInfo.TargetId().Target());
	
	if (KErrNotFound != index)
		{
		iOpenChatsArray.Remove(index);
		iOpenChatsArray.Compress();
		if (iChatObserver)
			{
			iChatObserver->HandleChangeL(iMessageHandler.ServiceId(), index, EChatClosed);		
			}
		}
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: RegisterObserver
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::RegisterObserver(MIMCVEngineOpenChatsObserver* aChatObserver)
	{
	iChatObserver = aChatObserver;
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: UnRegisterObserver
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::UnRegisterObserver()
	{
	iChatObserver = NULL;
 	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: HandleChatEventL
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::HandleChatEventL( TChatEventType /*aEvent*/, 
											MIMCVEngineMessage* aMessage /*= NULL*/ ) 
	{
	if (aMessage)
		{
		TInt index = KErrNotFound;
		
		 index = FindIndexL(aMessage->Recipient()) ;
		
		if (KErrNotFound != index)
			{
			MIMCVEngineMessageReadInterface *arrayItem = 
								(MIMCVEngineMessageReadInterface *)iOpenChatsArray[index];
			if (arrayItem && iChatObserver)
				{
				TInt serviceId = iMessageHandler.ServiceId();
				iChatObserver->SetStatusMsgL(serviceId, index,aMessage->Text());
				iChatObserver->HandleChangeL(serviceId, index, EUnReadCountChanged);			
				}	
			}		
		}	
	}


// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: HandleChatListEvent
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::HandleChatListEvent( TInt aServiceId ,TChatListEventType aEvent, 
									MIMCVEngineMessageReadInterface* aContainerInfo
									) 
	{
		
	switch (aEvent)
		{
		case EChatItemAdded:
			{				
			Insert(aContainerInfo);
			break;
			}

		case EChatItemDeleted:
			{				
			TRAP_IGNORE(RemoveL(*aContainerInfo));
			break;
			}
		case EChatAllItemDeleted:
		    {
		    // not owns
		    iOpenChatsArray.Reset();
		    if (iChatObserver)
		        {
		        TRAP_IGNORE( iChatObserver->HandleChangeL( aServiceId, KErrNotFound, EChatAllClosed ) );      
		        }
		    }
		default:
			break;
		}
	
	}
	
// -----------------------------------------------------------------------------
// CIMCVEngineOpenChats: ResetL
// -----------------------------------------------------------------------------
//
void CIMCVEngineOpenChats::ResetL()
	{
	iOpenChatsArray.Reset();
	iActiveItem = -1;		
	}
		
// End of File

