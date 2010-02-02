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
* Description:  message handler class implementation
*
*/

#include "cimcvenginemessagehandler.h"

#include "mimcvenginechatinterface.h"
#include "mimcvenginemessageswriteinterface.h"
#include "imcvlogger.h"
#include "mimcvenginenewmessagehandler.h"
#include "imcvenginemessageutils.h"
#include "imcvuiliterals.h"

// presence cache
#include <presencecachereader2.h>


#include <imfeatures.h>
#include <imconversation.h>
#include <imobjectfactory.h>
#include <ximpobjectfactory.h>
#include <imconversationinfo.h>
#include <servicepresenceinfo.h>
#include <avabilitytext.h>

#include <apmstd.h> // KMaxDataTypeLength
#include <utf.h>    // CnvUtfConverter

// imcache  related headers
#include    <cimcachefactory.h>
#include	<mimcacheupdater.h>
#include    <mimcacheeventhandler.h>

#include <spsettings.h>
#include <spentry.h>

#include "cimcvengineopenchats.h"
#include "mimcvenginebuffermemoryhandler.h"
#include "cimcvenginestoragelistener.h"

const TInt KMemorySafeValue = 1024; // One kbyte for safe memory allocation.

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::CIMCVEngineMessageHandler
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageHandler::CIMCVEngineMessageHandler( MIMCVEngineChatInterface& aChatInterface,
                                                      MIMCVEngineMessageCreator& aMessageCreator,
				                                      MXIMPContext* aContext, 
				                                      TInt aServiceId )
	: iMessageCreator( aMessageCreator ),
 	 iChatInterface( aChatInterface ),
 	 iContext(aContext),
 	iServiceId( aServiceId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CIMCVEngineMessageHandler() start") );
	iPresenceStatus =EUnknown; // default status
	iServiceName = NULL;
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CIMCVEngineMessageHandler() end") );
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::~CIMCVEngineMessageHandler
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageHandler::~CIMCVEngineMessageHandler()
    {
  	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::~CIMCVEngineMessageHandler() start") );
  	delete iStorageListener;
  	
  	iObserver = NULL;
  	delete iServiceName;
  	delete iPresenceCacheReader ; 
  	if( iIMCacheUpdater )
	  	{
	  	iIMCacheUpdater->UnRegisterObserver(*this);	
	  	}
	if( iIMCacheFactory )
		{
		CIMCacheFactory::Release();
		iIMCacheUpdater = NULL;
		iIMCacheFactory = NULL;
		}
    if(iImFeatures )
	    {
	    delete iImFeatures;
	    iImFeatures = NULL;	
	    }
    iHoldingMessages.Close();
    delete iOpenChats;
  
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::~CIMCVEngineMessageHandler() end") );
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageHandler* CIMCVEngineMessageHandler::NewL( 
					        MIMCVEngineChatInterface& aChatInterface, 
					        MIMCVEngineMessageCreator& aMessageCreator,
					        MXIMPContext* aContext,
					        TInt aServiceId,
						    const TDesC& aOwnId)
    {
  	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::NewL() start") );
    CIMCVEngineMessageHandler* self = CIMCVEngineMessageHandler::NewLC( 
            aChatInterface, 
            aMessageCreator,
            aContext,
		    aServiceId,
		    aOwnId );
    CleanupStack::Pop( self );
   
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::NewL() end") );
    return self;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::NewLC
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageHandler* CIMCVEngineMessageHandler::NewLC( 
					        MIMCVEngineChatInterface& aChatInterface, 
					        MIMCVEngineMessageCreator& aMessageCreator,
					        MXIMPContext* aContext,
					        TInt aServiceId,
							const TDesC& aOwnId)
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::NewLC() start") );
    CIMCVEngineMessageHandler* self = new ( ELeave ) CIMCVEngineMessageHandler( 
            aChatInterface, 
            aMessageCreator, 
             aContext,
             aServiceId );
    CleanupStack::PushL( self );
    self->ConstructL(aOwnId);
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::NewLC() end") );
    return self;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageHandler::ConstructL( const TDesC& aOwnId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ConstructL() start") );
	 iStorageListener = CIMCVEngineStorageListener::NewL(*this);
	// read the service name	
	ReadServiceNameL( iServiceId );
	// ximp im plugin
	if (iContext)
		{
	    iImFeatures = MImFeatures::NewL(iContext); 
		}
	//initialize the presence cache.
	iPresenceCacheReader = MPresenceCacheReader2::CreateReaderL();													 
    iPresenceCacheReader->SetObserverForSubscribedNotifications(this);
    
    // im cache factory
    iIMCacheFactory = CIMCacheFactory::InstanceL(); 
    // im cache update interface
    iIMCacheUpdater = iIMCacheFactory->CreateUpdaterL(iServiceId, aOwnId, ETrue );//registration required (ETrue).
    iIMCacheUpdater->RegisterObserverL(*this);
    
    iOpenChats = CIMCVEngineOpenChats::NewL(*this, iChatInterface,iServiceId );
    
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ConstructL() end") );
	}	

// ---------------------------------------------------------
// CIMCVEngineMessageHandler::ReadServiceNameL()
// ---------------------------------------------------------
//
void CIMCVEngineMessageHandler::ReadServiceNameL(TInt aServiceId )
	{
	CSPSettings* settings = CSPSettings::NewLC();
	// read service name 	
	CSPEntry* entry = CSPEntry::NewLC();
    settings->FindEntryL( aServiceId, *entry ) ;

	iServiceName = NULL;
	iServiceName = entry->GetServiceName().AllocL();
	CleanupStack::PopAndDestroy(2); // entry, settings	
	}
		
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandleDeliveryReportL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageHandler::HandleDeliveryReportL(TXIMPRequestId aOperatioCode, TInt aResult)  
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandleDeliveryReportL() start") );
	
	TInt holdingMessages = iHoldingMessages.Count();
    MIMCVEngineMessage* message = NULL;
    // Check if holding messages hold one ready message
    for(TInt index= 0 ; index < holdingMessages && !message; ++index )
        {
        MIMCVEngineMessage* msg = iHoldingMessages[ index ];
        if(  msg->OperationCode() == aOperatioCode  )
            {
            IM_CV_LOGS(TXT("CIMCVEngineMessageHandler:: pending message index = %d "), index );
            message = msg;
            if( aResult != KErrNone )
				{
				IM_CV_LOGS(TXT("CIMCVEngineMessageHandler:: message failed to deliver") );
				HandleMessageSentFailedL( *iHoldingMessages[ index ] );
				}
			iHoldingMessages.Remove(index);
			NotifyOperationCompletdL(message->MessagerType(), aResult );
            }
        }
   
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandleDeliveryReportL() end") );	
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandleIMCacheMessageEventL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
 void CIMCVEngineMessageHandler::HandleConnectionEventL( TIMCVConnectionState aState, TInt aReason )
      {
      if( iObserver )
          {
          iObserver->HandleConnectionEventL( iServiceId, aState, aReason );
          }
      }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandleIMCacheMessageEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineMessageHandler::HandleIMCacheEventL(TIMCacheEventType aEvent, TAny* aChatMessage )
	{
	switch (aEvent)
		{
		case EIMCacheNewMessage:
			{
			if (!aChatMessage)
				{
				return;	
				}
			SIMCacheMessageData* chatMesssge =  static_cast<SIMCacheMessageData*>( aChatMessage ) ;
			
			MIMCVEngineMessage::TMessageType messageType =  GetEngineMessageType( chatMesssge->iMessageType ) ;
			MIMCVEngineMessage::TMessagerType messagerType = GetEngineMessagerType( chatMesssge->iMessagerType );
			MIMCVEngineMessage::TContentType contentType = GetEngineContentType( chatMesssge->iContentType ) ;
				
			IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandleIMCacheMessageEventL() Msg ") );
				
			TTime time = chatMesssge->iTime;
		
			TPtrC buddyId = chatMesssge->iBuddyId->Des();
		
			if ( buddyId != KNullDesC )
		        {
		        
		        if( !iChatInterface.MemoryHandler().FreeMemoryIfNeededL( KMemorySafeValue + chatMesssge->iText->Size() ) )
		            {
		            //NotifyMessageError( KErrNoMemory, NULL );
		            User::Leave( KErrNoMemory );
		            }
		        }
				
		
			IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandleIMCacheMessageEventL() Msg %S"), &buddyId );
			

			MIMCVEngineMessageCreator::SMessageData data = 
														{
														KMessageDataVersion,
														KNullDesC,
														buddyId,
														chatMesssge->iText->Des(),
														contentType,
														messagerType
														};	
			MIMCVEngineMessageWriteInterface& messageContainer = 
		                	    iChatInterface.MessageWriteInterfaceL(iServiceId, buddyId );
			
			MIMCVEngineMessage* message	= iMessageCreator.CreateMessageL( data );	
			message->SetTimeStamp(time);
			   
			// Add message
			IMCVEngineMessageUtils::AppendMessageWithDateStampL( 
			                                 *message, 
			                                 messageContainer,
			                                 iMessageCreator );
			
			
			break;	
			}
		case EIMCacheNewChat:
			{
			if (!aChatMessage)
				{
				return;	
				}
			
			SIMCacheChatItem* chatItem =  static_cast<SIMCacheChatItem*>( aChatMessage ) ;
			
            iStorageListener->GetContactLinkInfoL( chatItem->iServiceId,*(chatItem->iBuddyId));
             
                  
			MIMCVEngineMessageReadInterface& messageContainer = 
		                	    iChatInterface.MessageReadInterfaceL( chatItem->iServiceId,
		                                                              chatItem->iBuddyId->Des() );
			break;	
			}
		case EIMCacheChatClosed:
			{
			if (!aChatMessage)
				{
				return;	
				}
			SIMCacheChatItem* chatItem =  static_cast<SIMCacheChatItem*>( aChatMessage ) ;
			iChatInterface.DeleteChatL( chatItem->iServiceId, chatItem->iBuddyId->Des() );						
			break;	
			}
		case EIMCacheAllChatClosed:
		    {
			IM_CV_LOGS  (TXT("CIMCVEngineMessageHandler :: HandleChatList Event AL CHAT DELETED"));
			iChatInterface.CloseAllContainers();                     
		    break;  
		    }
		case EIMCacheRequestCompleted :
			{
			NotifyOperationCompletdL(MIMCVEngineMessage::EMessageStarted,  KErrNone ) ;
			break;	
			}	
		default:
			break;
						
		}
	                                
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::GetEngineMessageType
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
MIMCVEngineMessage::TMessageType  CIMCVEngineMessageHandler::GetEngineMessageType(TIMCacheMessageType aType )
	{
	MIMCVEngineMessage::TMessageType messageType = MIMCVEngineMessage::EMessageInvalid;
	switch( aType )
		{
		case EIMCMessagePTOP:
			{
			messageType = MIMCVEngineMessage::EMessagePTOP;
			break;	
			}
		case EIMCMessageSystem:
			{
			messageType = MIMCVEngineMessage::EMessageSystem;
			break;	
			}
		default :
			{
			break;	
			}
		}
	return messageType;
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::GetEngineMessagerType
// ( Other items commented in header )
//-----------------------------------------------------------------------------    	
MIMCVEngineMessage::TMessagerType CIMCVEngineMessageHandler::GetEngineMessagerType(TIMCacheMessagerType aType )
	{
	MIMCVEngineMessage::TMessagerType messagerType = MIMCVEngineMessage::EMessageOther;
	switch( aType )
		{
		case EIMCMessageSent:
			{
			messagerType = MIMCVEngineMessage::EMessageSent;
			break;	
			}
		case EIMCMessageReceived:
			{
			messagerType = MIMCVEngineMessage::EMessageReceived;
			break;	
			}
		default :
			{
			break;	
			}
		}
	return messagerType;
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::GetEngineContentType
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
MIMCVEngineMessage::TContentType  CIMCVEngineMessageHandler::GetEngineContentType(TIMCacheContentType aType )
	{
	MIMCVEngineMessage::TContentType contentType = MIMCVEngineMessage::EContentInvalid;
	switch( aType )
		{
		case EIMCContentText:
			{
			contentType = MIMCVEngineMessage::EContentText;
			break;	
			}
		case EIMCContentOther:
			{
			contentType = MIMCVEngineMessage::EContentOther;
			break;	
			}
		default :
			{
			break;	
			}
		}
	return contentType;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::MessageCreator
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
MIMCVEngineMessageCreator& CIMCVEngineMessageHandler::MessageCreator() const
    {
    return iMessageCreator;
    }
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::StartNewConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineMessageHandler::StartNewConversationL(const TDesC& aRecipientId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::StartNewConversationL() start") );
	if(iIMCacheUpdater)
		{
		iIMCacheUpdater->StartNewConversationL(aRecipientId);
		}

	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::StartNewConversationL() end") );
	}
  
 //-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::CloseConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TInt CIMCVEngineMessageHandler::CloseConversationL(const TDesC& aBuddyId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseConversationL() start ") );
	if(iIMCacheUpdater)
		{
		iIMCacheUpdater->CloseConversationL( aBuddyId );;
		}
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseConversation () %S ************"), &aBuddyId);
	UnSubscribePresenceL (aBuddyId);
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseConversationL() end ") );
	return KErrNone;
	} 
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::CloseAllConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineMessageHandler::CloseAllConversationL()
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseAllConversationL() start") );
    iPresenceStatus = EOffline;
    if(iIMCacheUpdater  )
        {
        iIMCacheUpdater->CloseAllConversationL();
        }
	TInt count = iOpenChats->Count ();

	for (TInt i = 0; i < count; i++)
		{
			IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseAllConversationL() CLOSING %d, CHAT ************"), i );
			UnSubscribePresenceL (iOpenChats->GetTargetIdL (i));
		}
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::CloseAllConversationL() end") );
    } 
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::DeactivateConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TInt CIMCVEngineMessageHandler::DeactivateConversationL()
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::DeactivateConversation()") );
	if(iIMCacheUpdater)
		{
		return iIMCacheUpdater->DeactivateConversationL();
		}
	return KErrNone;
	} 
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::SendMessage
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineMessageHandler::SendMessageL(MIMCVEngineMessage& message )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SendMessageL() start") );
	if(iContext == NULL || iImFeatures == NULL)
		{
		return;	
		}
		
	MXIMPObjectFactory& objFactory = iContext->ObjectFactory();
	MImConversation& imconversation = iImFeatures->ImConversation();    

	MImConversationInfo* imMessage = iImFeatures->ImObjectFactory().NewImConversationInfoLC();

	MXIMPIdentity* identity = objFactory.NewIdentityLC();  
	identity->SetIdentityL( KMsgId );

	imMessage->SetMessageIdL(identity);

	imMessage->SetTextMessageL( message.Text() );

	CDesCArrayFlat* recipients = new ( ELeave )CDesCArrayFlat( 2 );
	CleanupStack::PushL( recipients);  //userid's
	recipients->AppendL( message.Recipient() );

	imMessage->SetRecipientL(recipients);
	
    TXIMPRequestId reqId = imconversation.SendMessageL(*imMessage);
    // must set operation code
    message.SetOperationCode(reqId);
    // must append message to holding until it get delivered
    iHoldingMessages.Append( &message );
    
	CleanupStack::PopAndDestroy();// recipients
	// ownership is transfer to ximpfw
	CleanupStack::Pop(); //identity
	CleanupStack::PopAndDestroy();//imMessage  
	
	if(iIMCacheUpdater)
		{
		iIMCacheUpdater->AppendSendMessageL( message.Text() );
		}
	
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SendMessageL() end") );
	}    
  
//-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::AppendToCacheL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CIMCVEngineMessageHandler::AppendToCacheL(const TDesC& aBuddyId, const TDesC& aTextMessage )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::AppendToCacheL() start") );
	if(iIMCacheUpdater)
		{
		iIMCacheUpdater->AppendMessageL(aBuddyId, aTextMessage );
		}
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::AppendToCacheL() End") );
	} 	
 //-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandleMessageSentFailedL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageHandler::HandleMessageSentFailedL( MIMCVEngineMessage& aMessage )
    {
   	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::HandleMessageSentFailedL() end") );
    MIMCVEngineMessage* failMessage = 
    iMessageCreator.CreateFailedMessageL( &aMessage );
    CleanupDeletePushL( failMessage );
    MIMCVEngineMessageWriteInterface& messageContainer = 
        iChatInterface.MessageWriteInterfaceL(
                0, 
                aMessage.Recipient() );
    CleanupStack::Pop( failMessage );
    messageContainer.AppendL( failMessage );  
  
  	// text will be Failed:text 
  	if(iIMCacheUpdater)
		{
		iIMCacheUpdater->AppendSendMessageL( failMessage->Text() );
		} 
    	  
    IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::HandleMessageSentFailedL() end") );
    }
 //-----------------------------------------------------------------------------
// CIMCVEngineMessageHandler::NotifyOperationCompletdL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
 void CIMCVEngineMessageHandler::NotifyOperationCompletdL(MIMCVEngineMessage::TMessagerType aType , TInt aResult  ) 
	 {
	 IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::NotifyOperationCompletdL() start") );
	 if( iObserver )
        {
        iObserver->HandleOperationCompletdL(iServiceId, aType, aResult );
        }
     IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::NotifyOperationCompletdL() end") );	
	 }
 //-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::ResetContextL
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageHandler::ResetContextL(MXIMPContext* aContext)
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ResetContextL() start") );
    iContext = NULL;
    iContext = aContext;
    if(iContext)
		{
		delete iImFeatures;
		iImFeatures = NULL;	
		iImFeatures = MImFeatures::NewL(iContext);	
		}
    else
		{
		if( iIMCacheUpdater )
			{
			iIMCacheUpdater->UnRegisterObserver(*this);	
			}
		if( iIMCacheFactory )
			{
			CIMCacheFactory::Release();
			iIMCacheUpdater = NULL;
			iIMCacheFactory = NULL;
			}	
		}
		
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ResetContextL() end") );
    }


void CIMCVEngineMessageHandler::UnSubscribePresenceL(const TDesC& aRecipientId)
{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::UnSubscribePresenceL() start %S"), &aRecipientId);
	if( !iServiceName )
		{
		return;	
		}
	iPresenceStatus = EUnknown; // default status
		
	HBufC* name = HBufC::NewLC( aRecipientId.Length() + iServiceName->Length() + KCVColon().Length() );
	TPtr namePtr( name->Des() );
	namePtr.Zero();

	// append the service name followed by user id ,seperated by colon
	namePtr.Append(*iServiceName);
	namePtr.Append(KCVColon);
	namePtr.Append(aRecipientId);
	
	
	IM_CV_LOGS(TXT("***************CIMCVEngineMessageHandler::UnSubscribePresenceL() %S ***************************"), &(name->Des ()));
	// subscribe for presence change notification 
	iPresenceCacheReader->UnSubscribePresenceBuddyChangeL(*name);

    CleanupStack::PopAndDestroy(); //  name  
    	
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::UnSubscribePresenceL() End") );	                                            
}

// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::SubscribePresenceL
// ---------------------------------------------------------------------------
//  
void CIMCVEngineMessageHandler::SubscribePresenceL(const TDesC& aRecipientId)
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SubscribePresenceL() start %S"), &aRecipientId );
	if( !iServiceName )
		{
		return;	
		}
	iPresenceStatus = EUnknown; // default status
		
	HBufC* name = HBufC::NewLC( aRecipientId.Length() + iServiceName->Length() + KCVColon().Length() );
	TPtr namePtr( name->Des() );
	namePtr.Zero();

	// append the service name followed by user id ,seperated by colon
	namePtr.Append(*iServiceName);
	namePtr.Append(KCVColon);
	namePtr.Append(aRecipientId);
	
	// read the cache status
	ReadPresenceStatusL( *name );

	IM_CV_LOGS(TXT("***************CIMCVEngineMessageHandler::SubscribePresenceL() %S ***************************"), &(name->Des ()));
	// subscribe for presence change notification 
	iPresenceCacheReader->SubscribePresenceBuddyChangeL(*name);

    CleanupStack::PopAndDestroy(); //  name  
    	
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SubscribePresenceL() End") );	                                            
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::SetPresenceObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageHandler::SetObserver( MIMCVEngineMessageHandlerObserver* aObserver )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SetPresenceObserver() start") );
	iObserver = NULL;
	iObserver = aObserver;
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::SetPresenceObserver() end") );
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageContainer::RemovePresenceObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageHandler::RemoveObserver()
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::RemovePresenceObserver() start") );
    iObserver = NULL;
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::RemovePresenceObserver() end") );
    }
// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::ReadPresenceStatusL
// ---------------------------------------------------------------------------
//  
void CIMCVEngineMessageHandler::ReadPresenceStatusL(const TDesC& aIdentity )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ReadPresenceStatusL() start") );
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ReadPresenceStatusL() : buddy Id = %s"), &aIdentity );
	MPresenceBuddyInfo2* presenceInfo = iPresenceCacheReader->PresenceInfoLC( aIdentity ); //pushed into cleanup stack
	if( presenceInfo )
	    {
	    MPresenceBuddyInfo2::TAvailabilityValues  availabilityEnum = presenceInfo->Availability();
        TPtrC availabilityText = presenceInfo->AvailabilityText();
	    iPresenceStatus = ConvertPresenceCacheEnums( availabilityEnum ,availabilityText );
	    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ReadPresenceStatusL():ConvertPresenceCacheEnums: %d"),iPresenceStatus);
	    if(EUnknown ==iPresenceStatus)
	        {
	        GetKeyFiledsAndValuesL(*presenceInfo);
	        IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ReadPresenceStatusL()::GetKeyFiledsAndValuesL: %d"),iPresenceStatus);
	        }
	    CleanupStack::PopAndDestroy(); //presenceInfo 
	    }
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ReadPresenceStatusL() End") );	                                            
	}

// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandlePresenceReadL
// ---------------------------------------------------------------------------
//  
void CIMCVEngineMessageHandler::HandlePresenceReadL(TInt /*aErrorCode*/,
                    RPointerArray<MPresenceBuddyInfo2>& /*aPresenceBuddyInfoList*/)
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceReadL() start") );
	// this is notification for all budy presence 
	// not interested for all 
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceReadL() End") );
    }	
// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::HandlePresenceNotificationL
// ---------------------------------------------------------------------------
//  
void CIMCVEngineMessageHandler::HandlePresenceNotificationL(TInt /*aErrorCode*/,
                            MPresenceBuddyInfo2* aPresenceBuddyInfo )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceNotificationL() start") );	
    
	if ( aPresenceBuddyInfo )
	    {
	    CleanupDeletePushL( aPresenceBuddyInfo );
	    TPtrC xspId = aPresenceBuddyInfo->BuddyId();
	    // extract user id from xsp id
	    TPtrC userId = xspId.Right(xspId.Length() - iServiceName->Length() - KCVColon().Length() );
	    
	    // read the availability /presence state enum value 
	    MPresenceBuddyInfo2::TAvailabilityValues  availabilityEnum = aPresenceBuddyInfo->Availability();
        TPtrC availabilityText = aPresenceBuddyInfo->AvailabilityText();
        iPresenceStatus = ConvertPresenceCacheEnums( availabilityEnum ,availabilityText);
        IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceNotificationL() : status = %d "), iPresenceStatus);   
        if(EUnknown == iPresenceStatus)
	        {
	        GetKeyFiledsAndValuesL(*aPresenceBuddyInfo);
	        IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceNotificationL():GetKeyFiledsAndValuesL : status = %d "), iPresenceStatus); 
	        }
	    // check if the display is already there,
	    // else get it from the servicetab
	    MIMCVEngineMessageReadInterface& readContainer = 
	                                       iChatInterface.MessageReadInterfaceL(iServiceId, userId );
	    IM_CV_LOGS(TXT(" CIMCVEngineMessageHandler::HandlePresenceNotificationL() before if") );
	    if(readContainer.DisplayName().Length() == 0)
	        {
	        
	        // before informing ui about the hnages,
            // create the chat if it is already not created,
            // hence we can always have the first name and the contact link before ui gets notified.
	        IM_CV_LOGS(TXT("1 CIMCVEngineMessageHandler::HandlePresenceNotificationL() before call GetContactLinkInfoL") );
	        iStorageListener->GetContactLinkInfoL( iServiceId,userId );
            IM_CV_LOGS(TXT(" CIMCVEngineMessageHandler::HandlePresenceNotificationL() after call GetContactLinkInfoL uid = %S"),&userId );
	       
	        }
	    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceNotificationL::GetContactLinkInfoL() End ") );
	   if( iObserver )
	        {
	        iObserver->HandlePresenceChangedL( iServiceId, userId );
	        }
	    CleanupStack::PopAndDestroy(); // aPresenceBuddyInfo
	    }
        
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::HandlePresenceNotificationL() End ") );	
    }

// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::ConvertStringToStatus
// ---------------------------------------------------------------------------
//  
TIMCVOnlineStatus CIMCVEngineMessageHandler::ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues  aAvailabilityEnum ,TPtrC aAvailabilityText)
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ConvertStringToStatus() Start ") );	
   
    // convert the presence cache enums to conversation view enumvalues
    // by default if the enum doesnot match then its  TVIMPSTEnums::UnKnown
    TIMCVOnlineStatus status;
    switch( aAvailabilityEnum )
        {
        case MPresenceBuddyInfo2::EBusy:
            {
			status  = EBusy;
			if(0==aAvailabilityText.Compare(KAwayState))
			    {
			    status  = EAway;
			    }
			if(0==aAvailabilityText.Compare(KOnPhoneState))
			    {
			    status  = EOnPhone;
			    }
			if(0==aAvailabilityText.Compare(KDndState))
			    {
			    status  = EDoNotDisturb;
			    }
            break;
            }
        case MPresenceBuddyInfo2::EAvailable:
            {
            status  = EOnline;
            break;
            }
        case MPresenceBuddyInfo2::ENotAvailable:
            {
            status  = EOffline;
            break;
            }
        case MPresenceBuddyInfo2::EUnknownAvailability:
            {
            status  = EUnknown;
						if(0==aAvailabilityText.Compare(KInvisibleState))
						    {
						    status  = EInvisible;
						    }
            break;
            }  
        default:
            {
            status  = EUnknown;
            break;
            }    
        }

    IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::ConvertStringToStatus() End ") );	
    return status;
    }  

// ---------------------------------------------------------
// CIMCVEngineMessageHandler::GetPresenceStatus
// 
// ---------------------------------------------------------	
TIMCVOnlineStatus CIMCVEngineMessageHandler::GetPresenceStatus()
	{
	// current presence status
	return iPresenceStatus;
	}
	
// ---------------------------------------------------------
// CIMCVEngineMessageHandler::GetChatListL
// 
// ---------------------------------------------------------	
RArray<SIMCacheChatItem> CIMCVEngineMessageHandler::GetChatListL( TInt aServiceId )
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::StartNewConversationL() start") );
	
	IM_CV_LOGS(TXT("CIMCVEngineMessageHandler::StartNewConversationL() end") );
	
	return iIMCacheUpdater->GetChatListL(aServiceId);

	}

// ---------------------------------------------------------
// CIMCVEngineMessageHandler::OpenChats
// 
// ---------------------------------------------------------
CIMCVEngineOpenChats& CIMCVEngineMessageHandler::OpenChats() 
	{
	return 	*iOpenChats;
	}


// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::GetKeyFiledsAndValuesL
// ---------------------------------------------------------------------------
//
void CIMCVEngineMessageHandler::GetKeyFiledsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo)
    {
    TPtrC8 value = aPresenceBuddyInfo.GetAnyField( KExtensionKey());
    // At any point of time fro remote and blocked contact only one of the keys
    // KPendingRequestExtensionValue/KBlockedExtensionValue will be assigned, and not both the keys.
    if(value.Compare( KPendingRequestExtensionValue) == 0 )
        {
        iPresenceStatus = EPending;
        }
    else if(value.Compare( KBlockedExtensionValue ) == 0 )
        {
        iPresenceStatus = EBlock;
        }
    else if(value.Compare( KServiceExtensionValue ) == 0)
       {
       iPresenceStatus  = EServiceOut;
       }
    else if(value.Compare( KCallForwardExtensionValue ) == 0)
       {
       iPresenceStatus  = ECallForward;
       }
    }
// ---------------------------------------------------------------------------
// CIMCVEngineMessageHandler::ServiceId
// ---------------------------------------------------------------------------
//
TInt CIMCVEngineMessageHandler::ServiceId() const 
    {
    return iServiceId;
    }
 
// -----------------------------------------------------------------------------
// CIMCVEngineMessageHandler: HandleServiceBuddyDetailsFetchedL
// -----------------------------------------------------------------------------
//
void CIMCVEngineMessageHandler::HandleServiceBuddyDetailsFetchedL(TInt aServiceId, 
        const TDesC& aXspId,
        const TDesC8 & aContactLink,
        const TDesC & aDisplayName ) 
    {
    IM_CV_LOGS(TXT("HandleServiceBuddyDetailsFetchedL Start") );  
    IM_CV_LOGS(TXT("HandleServiceBuddyDetailsFetchedL aName= %S"),&aDisplayName );
    IM_CV_LOGS(TXT("HandleServiceBuddyDetailsFetchedL aLink= %S"),&aContactLink);  
    MIMCVEngineMessageWriteInterface& messageContainer = 
    iChatInterface.MessageWriteInterfaceL(aServiceId, aXspId, aDisplayName, aContactLink );
    messageContainer.SetNameL(aDisplayName);
    messageContainer.SetLinkL(aContactLink);
    IM_CV_LOGS(TXT("HandleServiceBuddyDetailsFetchedL End") );  
    }
 // end of file
 
 
