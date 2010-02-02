/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  imcvuiengine_testsendmessage.cpp
*
*/


// [INCLUDE FILES] - do not remove
#include 	<e32math.h>
#include 	"imcvuiengine_testsendmessage.h"
#include 	"cimcvengine.h"
#include 	"mimcvenginemessagecontainerinfo.h"
#include 	"mimcvenginemessagecreator.h"
#include 	"cimcvenginemessagehandler.h"
#include 	<ximpbase.h>
#include    <e32property.h>
#include 	"mimcvenginechatinterface.h" 
#include 	"mimcvenginemessageswriteinterface.h"
#include    "mimcvengineopenchats.h"

#include 	"cimcvenginemessagehandler.h"
#include 	<avabilitytext.h>

#include 	<spentry.h>
#include 	<spsettings.h> 
#include 	<spproperty.h>
#include    "s_cch.h"
#include    "cimcvenginefactory.h"
#include    "mimcvenginefactory.h"
#include    "cimcvenginemessagecreator.h"
#include    "mimcvenginemessagecreator.h"

_LIT(KOwnUser,"testlm123@gmail.com");
_LIT8(KBrandId,"YAHOO");
                                  
const TInt KProtocolUid = 0x20007B6D;
const TUid KIMPropertyUid = { 0x0D334E82 };

const TUint KIMConnectedKey    	  	 = 0x00000001;
const TUint KIMServiceIdKey    	  	 = 0x00000002;
const TUint KIMServiceProtocolUidKey = 0x00000003;
const TUint KIMOwnUserIdKey    	     = 0x00000004;

_LIT(KRecipientID,"testui123@gmail.com");
_LIT(KRecipientID2,"testlm123@gmail.com");

_LIT(KInvalidRecipientID,"abcsddd@gmail.com");

// meco service uid
#define KMECOIMPLEMENTATIONUID 0x20012423


//const TInt ProcessInterval = 2000000; //2 seconds

// --------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::CIMCVUiEngine_TestSendMessage
// --------------------------------------------------------------------------
//
CIMCVUiEngine_TestSendMessage::CIMCVUiEngine_TestSendMessage()
	{
		
	}
    
    
// CIMCommands_TestLogin::~CIMCommands_TestLogin
// --------------------------------------------------------------------------
//
CIMCVUiEngine_TestSendMessage::~CIMCVUiEngine_TestSendMessage()
	{
	delete iSettings;
	CIMCVEngineFactory::Release();
	}
    
    
// --------------------------------------------------------------------------
// CIMCommands_TestLogin::NewL
// --------------------------------------------------------------------------
//
CIMCVUiEngine_TestSendMessage* CIMCVUiEngine_TestSendMessage::NewL()
    {
    CIMCVUiEngine_TestSendMessage* self = new(ELeave) CIMCVUiEngine_TestSendMessage();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::ConstructL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
void CIMCVUiEngine_TestSendMessage::ConstructL()
	{
		// define the property for upper layer to use serivceid , 
	// servicesetting id and loggedin user id
	// servicesetting id and loggedin user id
	
	CreateSpEntryL(KTestServiceId);
	ReadServiceDetalsL();
	
			  
	TInt err = RProperty::Define(KIMPropertyUid, KIMServiceIdKey, RProperty::EInt,
					  ECapabilityReadUserData ,ECapabilityWriteUserData);					  
    //User::LeaveIfError(err);  
    
			  
	err = RProperty::Define(KIMPropertyUid, KIMServiceProtocolUidKey, RProperty::EInt, 
					  ECapabilityReadUserData ,ECapabilityWriteUserData );					  
	//User::LeaveIfError(err);
	
	err = RProperty::Define(KIMPropertyUid, KIMConnectedKey, RProperty::EInt, 
					  ECapabilityReadUserData ,ECapabilityWriteUserData );					  
	//User::LeaveIfError(err);

	// write into defined property for upper layer to use serivceid , 

	RProperty::Set( KIMPropertyUid, KIMServiceIdKey, KTestServiceId );
	RProperty::Set( KIMPropertyUid, KIMServiceProtocolUidKey, KProtocolUid );
    RProperty::Set( KIMPropertyUid, KIMConnectedKey, ETrue );
               
    iEngineFactory = CIMCVEngineFactory::InstanceL();
    
	iEngine = &iEngineFactory->GetServiceEngineL( KTestServiceId );	
	
	// for send operation
	iEngine->MessageHandler().SetObserver(this);
	
	//iEngine->CCHHandler().EnableService();
	}
// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::ReadServiceDetalsL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
void CIMCVUiEngine_TestSendMessage::ReadServiceDetalsL()
	{
	iSettings = CSPSettings::NewL(); // CCH Login
    iServiceId = KErrNotFound;
    ///////////////////////Get Service Id////////////////    
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    iSettings->FindServiceIdsL( services );
    const TInt serviceCount = services.Count();
    for ( TInt i( 0 ); i < serviceCount; i++ )
		{
		TInt refContactUid = KErrNotFound;
		CSPProperty* property = CSPProperty::NewLC();
		iSettings->FindPropertyL( services[i],EPropertyContactViewPluginId,*property );    
		if ( property )
		    {
		    property->GetValue( refContactUid );
		    }
		CleanupStack::PopAndDestroy( property );             
		if ( (TUid::Uid( refContactUid ) == TUid::Uid( KMECOIMPLEMENTATIONUID ))
		        || (TUid::Uid( refContactUid ) ==  TUid::Uid( 0x20007B6D ) ))
		    {
	        iServiceId = services[i];
	        break;
		    }
		}
    CleanupStack::PopAndDestroy( &services );
   
	}


// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::CreateConnectionL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
void CIMCVUiEngine_TestSendMessage::CreateConnectionL()
	{
	// TODO : allready loggedin in local mode enable when login to n/w
	//if(!iEngine->IsLoggedIn() )
	//		{
	//		iEngine->CreateContextL();	
	//		}	
	}

// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::ReleaseConnectionL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
void CIMCVUiEngine_TestSendMessage::ReleaseConnectionL()
	{
	if(iEngine->IsLoggedIn() )
		{
		iEngine->ReleaseConnectionL();	
		}
//	iEngine->DeleteContextL();	
	}

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendTextMessageL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::SendTextMessageL()
	{
	_LIT(KSomeStr,"hello");	//String to be sent
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    write.AppendL(message );
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	return iResult;
	}

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendNullMessageL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::SendNullMessageL()
	{
	_LIT(KSomeStr,"");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID);
		
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
			
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    write.AppendL(message );
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	return iResult;
	}	

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::StartNewConvesationL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::StartNewConvesationL()
	{
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	return iResult;
	}	

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::CloseConvesationL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::CloseConvesationL()
	{
		
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	iEngine->MessageHandler().CloseConversationL( KRecipientID );
	
	return iResult;
	}	
	// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendSplCharMessageL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::SendSplCharMessageL()
	{
	_LIT(KSomeSplStr,"@&!`$?#");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
			
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    
    write.AppendL(message );
    
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	return iResult;
	}	
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendCosMessageL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::SendCosMessageL()
	{
	_LIT(KSomeSplStr,"asdscmdsdsvndv dsvndsfnv dfkdsfkd vkdsfvkdsj dsfdsjfkdsjf dsfjkdsjfkds dskfjdskfjdsk dfjkdsjfds dsfkdsjfkds dsfkdmfkds safdsajhf asfsajfnha adsfjndajsnhfd dsafnajdsfnh adsfjdajf ajfajfnhj fajadsjfa  asasjfjafja afnajf asjfjdsaf");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
			
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    
    write.AppendL(message );
    
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	return iResult;
	}	
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::DeactivateConvesationL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::DeactivateConvesationL()
	{
	
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	iEngine->MessageHandler().DeactivateConversationL();
	
	return iResult;
	}
		
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendToInvaliedRecipientL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::SendToInvaliedRecipientL()
	{
	_LIT(KSomeSplStr,"Invalied Recipient");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	iEngine->MessageHandler().StartNewConversationL( KInvalidRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
			
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KInvalidRecipientID,
												KSomeSplStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    
    write.AppendL(message );
    
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	if(iResult == KErrArgument )
		{
		iResult = KErrNone;	
		}
	return iResult;
	}	
	// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestRecievedMessageL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestMultipleSendMessageL()
	{
	_LIT(KSomeSplStr1,"this is recieved message 1");	//String to be sent
	_LIT(KSomeSplStr2,"this is recieved message 2");	//String to be sent
	_LIT(KSomeSplStr3,"this is recieved message 3");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	MIMCVEngineMessageCreator::SMessageData data1 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr1,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message1 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data1 );
    write.AppendL(message1);
	iEngine->MessageHandler().SendMessageL( *message1 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data2 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr2,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message2 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data2 );
    write.AppendL(message2);
	iEngine->MessageHandler().SendMessageL( *message2 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data3 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KInvalidRecipientID,
												KSomeSplStr3,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message3 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data3 );
    write.AppendL(message3);
	iEngine->MessageHandler().SendMessageL( *message3 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
									
	return iResult;
	}	
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestSendRecieveMsgL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestSendRecieveMsgL()
	{
	
	_LIT(KSomeSplStr1,"this is send recieved message 1");	//String to be sent
	_LIT(KSomeSplStr2,"this is send recieved message 2");	//String to be sent
	

	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	MIMCVEngineMessageCreator::SMessageData data1 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr1,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message1 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data1 );
    write.AppendL(message1 );
	iEngine->MessageHandler().SendMessageL( *message1 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data2 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr2,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message2 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data2 );
    
    write.AppendL(message2 );
	iEngine->MessageHandler().SendMessageL( *message2 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	iEngine->MessageHandler().DeactivateConversationL();
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
			
    return iResult;
	}
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestSendFetchSendMsgL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestSendFetchSendMsgL()
	{
	_LIT(KSomeSplStr1,"this is send recieved message 1");	//String to be sent
	_LIT(KSomeSplStr2,"this is send recieved message 2");	//String to be sent
	_LIT(KSomeSplStr3,"this is send recieved message 3 after fetched im from cache");	//String to be sent
	
	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );


	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	MIMCVEngineMessageCreator::SMessageData data1 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr1,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message1 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data1 );
    write.AppendL(message1);
	iEngine->MessageHandler().SendMessageL( *message1 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data2 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr2,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message2 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data2 );
    write.AppendL(message2);
	iEngine->MessageHandler().SendMessageL( *message2 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	iEngine->MessageHandler().DeactivateConversationL();
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	
	MIMCVEngineMessageCreator::SMessageData data3 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
												KSomeSplStr3,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message3 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data3 );
    write.AppendL(message3);
	iEngine->MessageHandler().SendMessageL( *message3 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
	return iResult;		
	}	

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::PrintTest
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::GetResult() const
	{
	return iResult;
	}

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::HandleOperationCompletdL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//	
void CIMCVUiEngine_TestSendMessage::HandleOperationCompletdL( TInt aserviceId, MIMCVEngineMessage::TMessagerType aType , TInt aResult )
	{
	iResult = aResult;	
	if ( iWait.IsStarted() )
		{
		iWait.AsyncStop();	
		}
	}
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestConnectionTerminationL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::TestConnectionTerminationL() 
	{
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::HandleConnectionTerminatedL
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//	
void CIMCVUiEngine_TestSendMessage::HandleConnectionEventL(TInt aServiceId, TIMCVConnectionState aState, TInt aReason )
	{
	iResult = aReason;	
	if ( iWait.IsStarted() )
		{
		iWait.AsyncStop();	
		}
	}

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestLoginStateL
// Example test method function.TestLoginStateL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestLoginStateL()
    {
   	
    if(iEngine->IsLoggedIn() )
	    {
	    return KErrNone;	
	    }
    return KErrServerTerminated ;
    }
 
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestLoginUserIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestLoginUserIdL()
    {
   	
    TPtrC userId = iEngine->GetLoggedInUserId();
     
 	if(! userId.Compare(KOwnUser ) )
	    {
	    return KErrNone;	
	    }
    return KErrArgument ;
    }
// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestGetBrandIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestGetBrandIdL()
    {
    TPtrC8 BrandId = iEngine->GetBrandId();
     
    if( BrandId.Length() )
        {
        return KErrNone;    
        }
    return KErrArgument ;
    }
// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestGetLanguageIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestGetLanguageIdL()
    {
    TInt languageId = iEngine->GetLanguageId();
     
    if(KErrNone != languageId )
        {
        return KErrNone;    
        }
    return KErrArgument ;
    }
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestChatInterfaceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestChatInterfaceL()
    {
    
   	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

	MIMCVEngineMessageReadInterface& read = chat.MessageReadInterfaceL(1, KRecipientID) ;
	read.Lock(ETrue); // to make sure ,valid ref

	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
 	write.Time();// to make sure ,valid ref
 	
 	return KErrNone;
 	
    } 

// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestMessageCoundL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestMessageCoundL()
    {
    _LIT(KSomeStr,"hello");	//String to be sent
    
   		
    MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
    iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	

	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageSent
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
    
	write.AppendL(message );
	
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageReadInterface& read = chat.MessageReadInterfaceL(1, KRecipientID) ;

	// in case of send this must be zero , except unread
	// this is the ReadOffset not the actaul message count
	TInt count = read.MessageCount(); 
	
	if(count == 0 )
		{
		return KErrNone;	
		}
 
 	return KErrNotFound;
    }   
   
  // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestMessageUnreadCoundL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestMessageUnreadCoundL()
    {
    _LIT(KSomeStr,"hello");	//String to be sent

    	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

    iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageReceived
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
   
	write.AppendL(message );
	
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageReadInterface& read = chat.MessageReadInterfaceL(1, KRecipientID) ;

	TInt count = read.UnreadCount(MIMCVEngineMessageReadInterface::EUnreadAll);
	
	if(count == 1)
		{
		return KErrNone;	
		}
 
 	return KErrNotFound;
 	
    }       
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::DeleteContextL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::DeleteContextL()
    {
    //by default engine's context needs to be binded
	iEngine->CreateContextL() ;	
    
    TRAPD(err, iEngine->DeleteContextL() );
    
 	return err;
    }    
  
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::CreateContextL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::CreateContextL()
    {
    // by default presence must be EOffline
    TRAPD(err, iEngine->CreateContextL() );   
    
 	return err;
    }    
     
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestSubscribePresenceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestSubscribePresenceL()
    {
    TRAPD(err, iEngine->MessageHandler().SubscribePresenceL(KRecipientID) );
  	return err;
    }  
      
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestGetPresenceStatusL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestGetPresenceStatusL()
    {
   
    // by default presence must be EOffline
    TIMCVOnlineStatus status = iEngine->MessageHandler().GetPresenceStatus();
    if(status == EOffline )
	    {
	    return KErrNone;	
	    }
    
  	return KErrArgument;
 	
    } 
    
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestAppendInfoMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestAppendInfoMessageL()
    {
   _LIT(KSomeStr,"hello this is information message ");	//String to be sent
    
    	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

    iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageOther
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
   
	write.AppendL(message );
	
	TRAPD(err, iEngine->MessageHandler().AppendToCacheL(KRecipientID, message->Text() ) );
		
	return err;	
    }                  
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::HandlePresenceChangedL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiEngine_TestSendMessage::HandlePresenceChangedL(TInt aServiceId, const TDesC& /*aBuddyId*/ )
	{
	// do nothings	
	}
	

 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestCCHHandlerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::TestCCHHandlerL()	
	{
	TInt error = KErrNone;
	
	MIMCVEngineCchHandler& cchHandler = iEngine->CCHHandler();
	
	if ( cchHandler.IsServiceLoggedIn() )
		{
		error = KErrArgument ;	
		}
	HBufC* user = cchHandler.GetUserIdL();
	
	if( user )
		{
		delete 	user;
		error = KErrNone ;
		}
	else
		{
		error = KErrArgument ;		
		}
	return error;			
	}

 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::ReadyForShutdownTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::ReadyForShutdownTestL()	
	{
	if( !iEngine->ReadyForShutdown() )
		{
		// loggedin
		return KErrNone;	
		}
	return KErrArgument;			
	}
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::ServiceNameTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::ServiceNameTestL()	
	{
	if (iEngine->ServiceName().Length() )
		{
		return KErrNone ;	
		}
	return KErrArgument;			
	}
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::OpenChatsTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::OpenChatsTestL()	
	{
	if ( &iEngine->OpenChats()  )
		{
		MIMCVEngineOpenChats& openchat = iEngine->OpenChats() ;
		if( !openchat.Count())
			{
			return KErrNone ;	
			}
		}
	return KErrArgument;			
	}
 // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::GetServiceIdTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt CIMCVUiEngine_TestSendMessage::GetServiceIdTestL()	
	{
	if ( iEngine->GetServiceId() > 0 )
		{
		return KErrNone ;	
		}
	return KErrArgument;			
	}
	

  
  // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestClosAllContainerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestClosAllContainerL()
    {
    _LIT(KSomeStr,"hello");	//String to be sent

    	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

    iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageReceived
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
   
	write.AppendL(message );
	
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	if( chat.ChatCount() )
		{
		chat.CloseAllContainers();
	
		}
	if( !chat.ChatCount() )
		{
		return KErrNone;
	
		}
	
	return KErrArgument;	
	
 	
    }    
    
  
  // -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestDeleteChatL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestDeleteChatL()
    {
    _LIT(KSomeStr,"hello");	//String to be sent

    	
	MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

    iEngine->MessageHandler().StartNewConversationL( KRecipientID );
	
	MIMCVEngineMessageWriteInterface& write = chat.MessageWriteInterfaceL(1, KRecipientID );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageReceived
												};										
    MIMCVEngineMessage* message = iEngine->MessageHandler().MessageCreator().CreateMessageL( data );
   
	write.AppendL(message );
	
	iEngine->MessageHandler().SendMessageL( *message );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}

	MIMCVEngineMessageWriteInterface& write2 = chat.MessageWriteInterfaceL(1, KRecipientID2 );
	
	iEngine->MessageHandler().StartNewConversationL( KRecipientID2 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	MIMCVEngineMessageCreator::SMessageData data1 = 
												{
												KMessageDataVersion,
												KOwnUser,
												KRecipientID2,
											    KSomeStr,
												MIMCVEngineMessage::EContentText,
												MIMCVEngineMessage::EMessageReceived
												};										
    MIMCVEngineMessage* message1 = iEngine->MessageHandler().MessageCreator().CreateMessageL( data1 );
   
	write2.AppendL(message1 );
	
	iEngine->MessageHandler().SendMessageL( *message1 );
	
	if ( !iWait.IsStarted() )
		{
		iWait.Start();	
		}
		
	if( chat.ChatCount() == 2 )
		{
		chat.DeleteChatL( 1, KRecipientID2 );
		}
	if( chat.ChatCount() == 1)
		{
		return KErrNone;
	
		}
	
	return KErrArgument;	
	
 	
    }  
// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestEditorMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestEditorMessageL()
    {
    
    MIMCVEngineChatInterface& chat = iEngine->ChatInterface();

    MIMCVEngineMessageReadInterface& read = chat.MessageReadInterfaceL(1, KRecipientID) ;
    read.Lock(ETrue); // to make sure ,valid ref
    read.StoreEditerMessageL(_L("Test Message"));
    TBuf<15> receiveData;
    receiveData = read.GetEditerMessage();
    
    if(0 == receiveData.Compare(_L("Test Message")))
        return KErrNone;
    
    return KErrArgument;
    } 
    
// -----------------------------------------------------------------------------
// CIMCVUiEngine_TestSendMessage::TestEditorMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVUiEngine_TestSendMessage::TestConvertPresCacheEnum()
    {
    TInt err;
    MIMCVEngineMessageUtils* aMessageUtils = NULL;
    MIMCVEngineChatInterface& chat = iEngine->ChatInterface();
    MIMCVEngineMessageCreator* messageCreator  = CIMCVEngineMessageCreator::NewL();
    CIMCVEngineMessageHandler* handler = CIMCVEngineMessageHandler::NewL( chat, *messageCreator, NULL, 1 ,KNullDesC);
    MPresenceBuddyInfo2::TAvailabilityValues value = MPresenceBuddyInfo2::EUnknownAvailability;
    TIMCVOnlineStatus status;
    
    // the ConvertPresenceCacheEnums does not have the support for block and pending, 
    // hence should be added in the new test cases.
    
    /*TBuf<50> buf1(KBlockedState);
    TPtrC     Ptr1 (buf1);
    TIMCVOnlineStatus status = handler->ConvertPresenceCacheEnums(value, Ptr1);
    if(EUnknown != status)
	    {
	    	return KErrNotFound;
	    }
	  */  
	value = MPresenceBuddyInfo2::EUnknownAvailability;    
	TBuf<50> buf2(KInvisibleState);
	TPtrC     Ptr2 (buf2);
	status = handler->ConvertPresenceCacheEnums(value, Ptr2);
	if(EInvisible != status)
	    {
	    	return KErrNotFound;
	    }

	value = MPresenceBuddyInfo2::EBusy;    
	TBuf<50> buf3(KAwayState);
	TPtrC     Ptr3 (buf3);
	status = handler->ConvertPresenceCacheEnums(value, Ptr3);
	if(EAway != status)
	    {
	    	return KErrNotFound;
	    }
	    
	value = MPresenceBuddyInfo2::EBusy;    
	TBuf<50> buf4(KOnPhoneState);
	TPtrC     Ptr4 (buf4);
	status = handler->ConvertPresenceCacheEnums(value, Ptr4);
	if(EOnPhone != status)
	    {
	    	return KErrNotFound;
	    }

	value = MPresenceBuddyInfo2::EBusy;    
	TBuf<50> buf5(KDndState);
	TPtrC     Ptr5 (buf5);
	status = handler->ConvertPresenceCacheEnums(value, Ptr5);
	if(EDoNotDisturb != status)
	    {
	    	return KErrNotFound;
	    }

	/*value = MPresenceBuddyInfo2::EUnknownAvailability;    
	TBuf<50> buf6(KPendingRequestState);
	TPtrC     Ptr6 (buf6);
	status = handler->ConvertPresenceCacheEnums(value, Ptr6);
	if(EUnknown != status)
	    {
	    	return KErrNotFound;
	    }
	   */ 
	value = MPresenceBuddyInfo2::EAvailable;
	_LIT(KAnyMsg,"Any msg");
	TBuf<50> buf7(KAnyMsg);
	TPtrC     Ptr7 (buf7);
	status = handler->ConvertPresenceCacheEnums(value, Ptr7);
	if(EOnline != status)
	    {
	    	return KErrNotFound;
	    }
	    
	value = MPresenceBuddyInfo2::ENotAvailable;   
	status = handler->ConvertPresenceCacheEnums(value, Ptr7);
	if(EOffline != status)
	    {
	    	return KErrNotFound;
	    }
	    
	value = MPresenceBuddyInfo2::EUnknownAvailability;
	status = handler->ConvertPresenceCacheEnums(value, Ptr7);
	if(EUnknown != status)
	    {
	    	return KErrNotFound;
	    }

	return KErrNone;        
    }
    
       
// end of file

