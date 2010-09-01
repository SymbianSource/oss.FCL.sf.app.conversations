/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message handler notification
 *                
 *
*/

 
#include "cservicewidgetmessagehandler.h"
#include "mservicewidgetobservers.h"
#include "swpdebugtrace.h"
//im cache
#include  <cimcachefactory.h>
#include  <mimcacheaccessor.h>
#include <apgtask.h> 
#include <coemain.h>

// ---------------------------------------------------------------------------
// CServiceWidgetMessageHandler::CServiceWidgetMessageHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetMessageHandler::CServiceWidgetMessageHandler(
						MServiceWidgetNewMessageObserver& aMessageObserver )
	:iMessageObserver( aMessageObserver )
   {
   TRACE_SWP(TXT("CServiceWidgetMessageHandler::CServiceWidgetMessageHandler") );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetMessageHandler::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetMessageHandler* CServiceWidgetMessageHandler::NewL( 
						MServiceWidgetNewMessageObserver& aMessageObserver,
						TInt aServiceId, 
     				    const TDesC& aOwnUserId )
    {
    TRACE_SWP(TXT("CServiceWidgetMessageHandler::NewL() start") );
    CServiceWidgetMessageHandler* self = NewLC(aMessageObserver,
    									aServiceId,
    									aOwnUserId);
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetMessageHandler::NewL() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetMessageHandler::NewLC
// ---------------------------------------------------------------------------
// 
CServiceWidgetMessageHandler* CServiceWidgetMessageHandler::NewLC( 
					   MServiceWidgetNewMessageObserver& aMessageObserver,
						TInt aServiceId, 
				 		const TDesC& aOwnUserId )
    {
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::NewLC() start") );
    CServiceWidgetMessageHandler* self =
        new (ELeave) CServiceWidgetMessageHandler(aMessageObserver );
    CleanupStack::PushL(self);
    self->ConstructL( aOwnUserId ,aServiceId );
    TRACE_SWP(TXT("CServiceWidgetMessageHandler::NewLC() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetMessageHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetMessageHandler::ConstructL( const TDesC& aOwnUserId, TInt aServiceId )
    {
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::ConstructL() start") );
   	iIMCacheFactory = CIMCacheFactory::InstanceL(); 
	iAccessor = iIMCacheFactory->CreateAccessorL( aServiceId, aOwnUserId );
	iAccessor->RegisterObserverL(*this);
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::ConstructL() end") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetMessageHandler::~CServiceWidgetMessageHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetMessageHandler::~CServiceWidgetMessageHandler()
    {
    TRACE_SWP(TXT("CServiceWidgetMessageHandler::~CServiceWidgetMessageHandler() start") );
  	if( iIMCacheFactory )
		{
		if( iAccessor )
		    {
	    	iAccessor->UnRegisterObserver(*this);	
	    	}
		CIMCacheFactory::Release();  	
		}
	iSendersArray.ResetAndDestroy();
	
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::~CServiceWidgetMessageHandler() end") );
    }

// -----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::GetUnreadMessageCountL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CServiceWidgetMessageHandler::GetUnreadMessageCountL()
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::GetUnreadMessageCountL()") );
	return iAccessor->GetAllUnreadMessageCountL() ;
	}
//-----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::HandleIMCacheEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------  

void CServiceWidgetMessageHandler::HandleIMCacheEventL(TIMCacheEventType aEventType, TAny* aChatMessage  )
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::HandleIMCacheEventL() start") );
	switch( aEventType )
		{
		case EIMCacheUnreadMessage :
			{
			TRACE_SWP(TXT("CServiceWidgetMessageHandler::HandleIMCacheEventL() EIMCacheUnreadMessage ") );
			if (!aChatMessage)
				{
				return;	
				}
			SIMCacheMessageData* chatData = static_cast<SIMCacheMessageData*>( aChatMessage ) ;
			// check that buddy id exists
			if ( chatData->iBuddyId )
			    {
			    TPtrC buddyId = chatData->iBuddyId->Des();
				TPtrC messageText = chatData->iText->Des();
				if( !IsExistInSendersArrayL( buddyId ) ) // append only if not exist 
					{
					HBufC* sender = buddyId.AllocLC();
					if( sender )
						{
						iSendersArray.InsertL( sender ,0 ); // ownership to array
						CleanupStack::Pop(sender); //	
						}
					}
				iMessageObserver.HandleNewMessageL( buddyId, messageText );
			    }
			break;	
			}
		case EIMCacheUnreadChange :
			{
			TRACE_SWP(TXT("CServiceWidgetMessageHandler::HandleIMCacheEventL() EIMCacheUnreadChange ") );
			SynchronizeSendersArrayL();
		    iMessageObserver.HandleCountChangedL();
			break;	
			}
		default:
			{
			break;
			}
		}
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::HandleIMCacheEventL() end ") );
	}

// -----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::GetUnreadSenderId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CServiceWidgetMessageHandler::GetUnreadSenderId() 
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::GetUnreadSenderId() start") );
	if( iSendersArray.Count() )
		{
		TRACE_SWP(TXT("CServiceWidgetMessageHandler::GetUnreadSenderId() end") );
		return *( iSendersArray[ 0 ] ); //  latest sender always on top
		}
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::GetUnreadSenderId() end") );
	return KNullDesC;
	}

// -----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::IsSingleMessageSender
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CServiceWidgetMessageHandler::IsSingleMessageSender() 
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::IsSingleMessageSender()") );
	return ( iSendersArray.Count() == 1 );
	}
	
// -----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::IsExistInSendersArrayL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CServiceWidgetMessageHandler::IsExistInSendersArrayL( const TDesC& aNewSenderId ) 
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::IsExistInSendersArrayL() start") );
	TInt count = iSendersArray.Count();
	for( TInt i=0; i<count; i++ )
		{
		HBufC* senderId = iSendersArray[i];
		if( senderId && aNewSenderId.Compare( *senderId ) == 0 )	
			{
			TRACE_SWP(TXT("CServiceWidgetMessageHandler::IsExistInSendersArrayL() end") );
			return  ETrue;	
			}
		}
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::IsExistInSendersArrayL() end") );
	return EFalse;
	}
// -----------------------------------------------------------------------------
// CServiceWidgetMessageHandler::SynchronizeSendersArrayL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CServiceWidgetMessageHandler::SynchronizeSendersArrayL() 
	{
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::SynchronizeSendersArrayL() start") );
	TInt count = iSendersArray.Count();
	for( TInt i=0; i < count; i++ )
		{
		HBufC* senderId = iSendersArray[i];
		if( senderId )	
			{
			TInt unreadCount = iAccessor->GetUnreadMessageCountL(*senderId  ) ;
			if( !unreadCount )
				{
				iSendersArray.Remove( i );
				delete 	senderId;
				senderId = NULL;
				iSendersArray.Compress();
				break;
				}
			}
		}
	TRACE_SWP(TXT("CServiceWidgetMessageHandler::SynchronizeSendersArrayL() end") );
	}		
	
// end of file


