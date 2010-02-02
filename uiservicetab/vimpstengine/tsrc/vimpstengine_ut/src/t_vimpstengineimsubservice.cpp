/*
* t_vimpstengineimsubservice.cpp Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstengineimsubservice.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstengineimsubservice.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>

#include "cvimpstengineimsubservice.h"
#include "cvimpstenginecchhandler.h"
#include "cvimpstengineservicetablefetcher.h"

#include "s_cch.h"

extern MyCchService* myService;
extern TBool createService ;
extern TBool tryConnParamWithSuccess ;
extern TBool gConversationExist;
extern TBool gConversationUnreadCount;


const TInt KTestServiceId = 5;

_LIT( KTestBuddyId,"TEST_BUDDY_ID");
//_LIT( KTestRequesterId,"TEST_REQUESTER_ID");

T_VimpstEngineImSubService* T_VimpstEngineImSubService::NewLC()
    {
    T_VimpstEngineImSubService* self = new( ELeave ) T_VimpstEngineImSubService;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEngineImSubService::~T_VimpstEngineImSubService()
    {
    }

void T_VimpstEngineImSubService::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


//  FROM OBSERVER INTERFACES
void T_VimpstEngineImSubService::HandleChatMessageEventL( TVIMPSTEnums::TIMEventType aEventType ,
    const TDesC& /*aSender*/)
    {
    iChatMsgReceived = ETrue;
    }

void T_VimpstEngineImSubService::HandleServceConnectionEventL()
    {
    }


void T_VimpstEngineImSubService::SetupL()
    {    
    createService = ETrue;
    tryConnParamWithSuccess = ETrue;
    iTestServiceId = CreateSpEntryL( KTestServiceId );
    
    iCchHandler = CVIMPSTEngineCchHandler::NewL( iTestServiceId ,*iCchUiEventObserver );
    
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL(); 
    
    
    iImSubService = CVIMPSTEngineIMSubService::NewL( 
        iTestServiceId,
        *iCchHandler,
        *iServiceTableFetcher,
        *this );
    }

void T_VimpstEngineImSubService::Teardown()
    {
    if ( iCchHandler )
        {
        delete iCchHandler;
        iCchHandler = NULL;
        }
    if ( iServiceTableFetcher )
        {
        delete iServiceTableFetcher;
        iServiceTableFetcher = NULL;
        }
    
    if ( iImSubService )
        {
        delete iImSubService;
        iImSubService = NULL;
        }
    
    if ( myService )
        {
        delete myService;
        myService = NULL;
        }  
    gConversationExist = EFalse;
    gConversationUnreadCount = EFalse;
    
    }

TInt T_VimpstEngineImSubService::CreateSpEntryL( TInt aServiceId )
    {
    CSPSettings* settings = CSPSettings::NewLC();
    
    CSPEntry* entry = CSPEntry::NewLC();
    entry->SetServiceId( aServiceId );
    
    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = settings->FindEntryL( entry->GetServiceId(), *testEntry );
    
    if( result == KErrNone )
        {
        settings->UpdateEntryL( *entry );
        }
    else
        {
        settings->AddEntryL( *entry );
        }
    
    TInt createdServiceId = entry->GetServiceId();
    
    CleanupStack::PopAndDestroy( testEntry );
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( settings );
    
    return createdServiceId;
    }   

void T_VimpstEngineImSubService::T_NewLL()
    {
    CVIMPSTEngineIMSubService* imSubService = NULL;
    imSubService = CVIMPSTEngineIMSubService::NewLC(
        iTestServiceId,
        *iCchHandler,
        *iServiceTableFetcher,
        *this);
       
    EUNIT_ASSERT( imSubService != NULL );      
    CleanupStack::PopAndDestroy( imSubService );
    }

void T_VimpstEngineImSubService::T_RegisterChatObserverL()
    {
    iImSubService->RegisterChatObserver( this );
    EUNIT_ASSERT( 1 == iImSubService->iChatObserver.Count() );
    }

void T_VimpstEngineImSubService::T_UnRegisterChatObserverL()
    {
    iImSubService->RegisterChatObserver( this );
    iImSubService->UnRegisterChatObserver( this );
    EUNIT_ASSERT( 0 == iImSubService->iChatObserver.Count() );
    }

void T_VimpstEngineImSubService::T_HandleIMCacheEventLL()
    {
    TInt error( KErrNone );
        
    RArray<TIMCacheEventType> cacheEventTypes;
    CleanupClosePushL( cacheEventTypes );
        
    // Add all cache events to array
    cacheEventTypes.AppendL( EIMCacheRequestCompleted );
    cacheEventTypes.AppendL( EIMCacheNewMessage );
    cacheEventTypes.AppendL( EIMCacheUnreadMessage );
    cacheEventTypes.AppendL( EIMCacheUnreadChange );
    cacheEventTypes.AppendL( EIMCacheChatStarted );
    cacheEventTypes.AppendL( EIMCacheAllChatClosed );
    cacheEventTypes.AppendL( EIMCacheNewChat );
    cacheEventTypes.AppendL( EIMCacheChatClosed );
    

    // go through all cache events with NULL pointer
    for ( TInt i( 0 ) ; i < cacheEventTypes.Count() ; i++ )
        {
        TRAP( error, iImSubService->HandleIMCacheEventL( 
            cacheEventTypes[ i ], NULL ) );
            
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
        EUNIT_ASSERT( KErrNone == error );
        }
        
    // go through all cache events with chatData
    SIMCacheMessageData chatData = 
        {
        TIMCacheMessageType(0),
        };
          
    chatData.iBuddyId = HBufC::NewLC( KTestBuddyId().Length() );
    chatData.iBuddyId->Des().Copy( KTestBuddyId );

	//for add request
	/*SIMCacheContactData contactData = {0};
	
	contactData.iServiceId = iTestServiceId;
	contactData.iRequesterId = HBufC::NewLC( KTestBuddyId().Length() );
    contactData.iRequesterId->Des().Copy( KTestBuddyId );    
	*/
    for ( TInt i( 0 ) ; i < cacheEventTypes.Count() ; i++ )
        {
        
        if (cacheEventTypes[ i ] == EIMCacheChatStarted || 
        			cacheEventTypes[ i ] == EIMCacheAllChatClosed)
	        {
	        TRAP( error, iImSubService->HandleIMCacheEventL( 
	            cacheEventTypes[ i ]) );	
	        }
        else
	        {	        	
	        TRAP( error, iImSubService->HandleIMCacheEventL( 
	            cacheEventTypes[ i ], &chatData ) );
	        }
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
        EUNIT_ASSERT( KErrNone == error );
        }
    
  //  CleanupStack::PopAndDestroy(contactData.iRequesterId);
    CleanupStack::PopAndDestroy(chatData.iBuddyId);
    
    EUNIT_ASSERT( EFalse == iChatMsgReceived );
    
    CleanupStack::PopAndDestroy( &cacheEventTypes );
    }

void T_VimpstEngineImSubService::T_SubServiceStateL()
    {
    RArray<TVIMPSTEnums::TVIMPSTRegistrationState> states;
    CleanupClosePushL( states );
    states.AppendL( TVIMPSTEnums::ESVCENotRegistered );
    states.AppendL( TVIMPSTEnums::ESVCERegistered );
    states.AppendL( TVIMPSTEnums::ESVCENetworkConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEWaitingForNetwork );
    states.AppendL( TVIMPSTEnums::ESVCENetworkDisConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEUpdatingContacts );
       
    for ( TInt i( 0 ) ; i < states.Count() ; i++ )
        {
        iImSubService->iServiceState = states[ i ];
        EUNIT_ASSERT( states[ i ] == iImSubService->SubServiceState() );
        }
       
    CleanupStack::PopAndDestroy( &states );
    }

void T_VimpstEngineImSubService::T_IsConversationExistLL()
    {
    gConversationExist = ETrue;
    iImSubService->CreateIMCacheAccessorL();
    EUNIT_ASSERT( ETrue == iImSubService->IsConversationExistL(KTestBuddyId()) );
    iImSubService->ReleaseIMCacheAccessor();    
    }


void T_VimpstEngineImSubService::T_IsConversationDoesntExistLL()
    {
    iImSubService->CreateIMCacheAccessorL();    
    EUNIT_ASSERT( EFalse == iImSubService->IsConversationExistL(KTestBuddyId()) );
    iImSubService->ReleaseIMCacheAccessor();
    }


    
void T_VimpstEngineImSubService::T_CloseConversationExisitingConversationLL()
    {
    gConversationExist = ETrue;
    iImSubService->CreateIMCacheAccessorL();
    
    TRAPD( err, iImSubService->CloseConversationL( KTestBuddyId() ) );
        
    EUNIT_ASSERT( KErrNone == err );
    iImSubService->ReleaseIMCacheAccessor();
    }
    
void T_VimpstEngineImSubService::T_CloseConversationNonExisitingConversationLL()
    {
    iImSubService->CreateIMCacheAccessorL();
    TRAPD( err, iImSubService->CloseConversationL( KTestBuddyId() ) );
        
    EUNIT_ASSERT( KErrNone == err );
    iImSubService->ReleaseIMCacheAccessor();
    }  
    
void T_VimpstEngineImSubService::T_IsConversationExistNoIMCacheAccessorL()
    {
    
    iImSubService->ReleaseIMCacheAccessor();
    EUNIT_ASSERT( EFalse == iImSubService->IsConversationExistL(KTestBuddyId()) );
    
    }
         

void T_VimpstEngineImSubService::T_CloseConversationNoIMCacheAccessorL()
    {
    iImSubService->ReleaseIMCacheAccessor();
    
    TRAPD( err, iImSubService->CloseConversationL( KTestBuddyId() ) );
        
    EUNIT_ASSERT( KErrNone == err );
    }


void T_VimpstEngineImSubService::T_GetUnreadCountL()
    {
    gConversationUnreadCount = ETrue;
    iImSubService->CreateIMCacheAccessorL();
    TInt count = -1;
    
    TRAPD( err, count = iImSubService->GetUnreadCountL( KTestBuddyId() ) );
    
    EUNIT_ASSERT( 1 == count ); 
	        
    EUNIT_ASSERT( KErrNone == err );
    iImSubService->ReleaseIMCacheAccessor();
    }
    
    
void T_VimpstEngineImSubService::T_GetUnreadCountNoIMCacheAccessorL()
    {    
    TInt count = -1;
    
    TRAPD( err, count = iImSubService->GetUnreadCountL( KTestBuddyId() ) );
    
    EUNIT_ASSERT( 0 == count ); 
        
    EUNIT_ASSERT( KErrNone == err );
    }
   
    
void T_VimpstEngineImSubService::T_ResolveServiceStateLL()
    {
    TInt error( KErrNone );
        
    RArray<TCCHSubserviceState> subServiceStates;   
    CleanupClosePushL( subServiceStates );
    subServiceStates.AppendL( ECCHUninitialized );
    subServiceStates.AppendL( ECCHDisabled );
    subServiceStates.AppendL( ECCHConnecting );
    subServiceStates.AppendL( ECCHEnabled );
    subServiceStates.AppendL( ECCHDisconnecting );
    
    // Test with service error: KErrNone and all subservice states.
    TInt serviceError( KErrNone );
            
    for ( TInt i( 0 ) ; i < subServiceStates.Count() ; i++ )
        {
        TRAP( error, iImSubService->ResolveServiceStateL( 
            subServiceStates[ i ], serviceError ) );
                
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
                
        EUNIT_ASSERT( KErrNone == error );
        }
              
    // Test with service error: KErrNotFound and all subservice states.
    serviceError = KErrNotFound;
    for ( TInt j( 0 ) ; j < subServiceStates.Count() ; j++ )
        {
        TRAP( error, iImSubService->ResolveServiceStateL( 
            subServiceStates[ j ], serviceError ) );
                
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
                       
        EUNIT_ASSERT( KErrNone == error );
        }
        
    CleanupStack::PopAndDestroy( &subServiceStates );
    }

void T_VimpstEngineImSubService::T_TypeL()
    {
    EUNIT_ASSERT( TVIMPSTEnums::EIM == iImSubService->Type() );
    }

void T_VimpstEngineImSubService::T_CchEventOccuredLL()
    {
    TInt error( KErrNone );
        
    RArray<TCCHSubserviceState> subServiceStates;   
    CleanupClosePushL( subServiceStates );
    subServiceStates.AppendL( ECCHUninitialized );
    subServiceStates.AppendL( ECCHDisabled );
    subServiceStates.AppendL( ECCHConnecting );
    subServiceStates.AppendL( ECCHEnabled );
    subServiceStates.AppendL( ECCHDisconnecting );

    // Test with service error: KErrNone and all subservice states.
    TInt serviceError( KErrNone );
        
    for ( TInt i( 0 ) ; i < subServiceStates.Count() ; i++ )
        {
        TRAP( error, iImSubService->CchEventOccuredL( 
            iTestServiceId, subServiceStates[ i ], serviceError ) );
            
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
            
        EUNIT_ASSERT( KErrNone == error );
        }
          
    // Test with service error: KErrNotFound and all subservice states.
    serviceError = KErrNotFound;
    for ( TInt j( 0 ) ; j < subServiceStates.Count() ; j++ )
        {
        TRAP( error, iImSubService->CchEventOccuredL( 
            iTestServiceId, subServiceStates[ j ], serviceError ) );
            
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
                   
        EUNIT_ASSERT( KErrNone == error );
        }
        
    CleanupStack::PopAndDestroy( &subServiceStates );
    }

void T_VimpstEngineImSubService::T_DoHandleCchErrorLL()
    {
    RArray<TInt> serviceErrors;
    CleanupClosePushL( serviceErrors );
    
    serviceErrors.AppendL( KErrGeneral ); // Tests default case
    serviceErrors.AppendL( KCCHErrorBandwidthInsufficient );
    serviceErrors.AppendL( KCCHErrorInvalidIap );
    serviceErrors.AppendL( KCCHErrorAuthenticationFailed );
    serviceErrors.AppendL( KCCHErrorNetworkLost );
    serviceErrors.AppendL( KCCHErrorLoginFailed );
    serviceErrors.AppendL( KCCHErrorServiceNotResponding );
    serviceErrors.AppendL( KCCHErrorInvalidSettings );
    serviceErrors.AppendL( KCCHErrorAccessPointNotDefined );
    
    for ( TInt i( 0 ) ; i < serviceErrors.Count() ; i++ )
        {
        TRAPD( err, iImSubService->DoHandleCchErrorL( 
            serviceErrors[ i ] ) );
        
        if ( KErrNoMemory == err )
            {
            User::Leave( err );
            }
                      
        EUNIT_ASSERT( KErrNone == err );
        }
    
    CleanupStack::PopAndDestroy( &serviceErrors );
    }

void T_VimpstEngineImSubService::T_CreateIMCacheAccessorLL()
    {
    
    iImSubService->CreateIMCacheAccessorL();
    EUNIT_ASSERT(iImSubService->iIMCacheFactory);
    EUNIT_ASSERT(iImSubService->iIMCacheAccessor)
    
    }

void T_VimpstEngineImSubService::T_ReleaseIMCacheAccessorL()
    {
    iImSubService->CreateIMCacheAccessorL();    
    iImSubService->ReleaseIMCacheAccessor();
    
    EUNIT_ASSERT(NULL == iImSubService->iIMCacheFactory);
    EUNIT_ASSERT(NULL == iImSubService->iIMCacheAccessor);
    
    }


void T_VimpstEngineImSubService::T_RegisterNULLChatObserverL()
    {
    iImSubService->RegisterChatObserver( NULL );
    EUNIT_ASSERT( 0 == iImSubService->iChatObserver.Count() );
    }

void T_VimpstEngineImSubService::T_UnRegisterNULLChatObserverL()
    {
    iImSubService->RegisterChatObserver( NULL );
    iImSubService->UnRegisterChatObserver( NULL );
    EUNIT_ASSERT( 0 == iImSubService->iChatObserver.Count() );
    }

void T_VimpstEngineImSubService::T_RegisterChatObserverTwiceL()
    {
    iImSubService->RegisterChatObserver( this );
    iImSubService->RegisterChatObserver( this );
    EUNIT_ASSERT( 1 == iImSubService->iChatObserver.Count() );
    }

void T_VimpstEngineImSubService::T_UnRegisterChatObserverTwiceL()
    {
    iImSubService->RegisterChatObserver( this );
    iImSubService->UnRegisterChatObserver( this );
    iImSubService->UnRegisterChatObserver( this );
    EUNIT_ASSERT( 0 == iImSubService->iChatObserver.Count() );
    }    


void T_VimpstEngineImSubService::T_HandleIMCacheEventWithRegisteredObserverL()
    {
    TInt error( KErrNone );
        
    RArray<TIMCacheEventType> cacheEventTypes;
    CleanupClosePushL( cacheEventTypes );
	
	iImSubService->RegisterChatObserver( this );    
            
    // Add all cache events to array
	   cacheEventTypes.AppendL( EIMCacheRequestCompleted );
	    cacheEventTypes.AppendL( EIMCacheNewMessage );
	    cacheEventTypes.AppendL( EIMCacheUnreadMessage );
	    cacheEventTypes.AppendL( EIMCacheUnreadChange );
	    cacheEventTypes.AppendL( EIMCacheChatStarted );
	    cacheEventTypes.AppendL( EIMCacheAllChatClosed );
	    cacheEventTypes.AppendL( EIMCacheNewChat );
	    cacheEventTypes.AppendL( EIMCacheChatClosed );
        
    // go through all cache events with NULL pointer
    for ( TInt i( 0 ) ; i < cacheEventTypes.Count() ; i++ )
        {
        TRAP( error, iImSubService->HandleIMCacheEventL( 
            cacheEventTypes[ i ], NULL ) );
            
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
        EUNIT_ASSERT( KErrNone == error );
        }
        
    // go through all cache events with chatData
    SIMCacheMessageData chatData = 
        {
        TIMCacheMessageType(0),
        };
          
    chatData.iBuddyId = HBufC::NewLC( KTestBuddyId().Length() );
    chatData.iBuddyId->Des().Copy( KTestBuddyId );

	//for add request
	/*SIMCacheContactData contactData = {0,};
	
	contactData.iServiceId = iTestServiceId;
	contactData.iRequesterId = HBufC::NewLC( KTestBuddyId().Length() );
    contactData.iRequesterId->Des().Copy( KTestBuddyId );   
    */ 
	
    for ( TInt i( 0 ) ; i < cacheEventTypes.Count() ; i++ )
        {
        if (cacheEventTypes[ i ] == EIMCacheChatStarted || 
        			cacheEventTypes[ i ] == EIMCacheAllChatClosed)
	        {
	        TRAP( error, iImSubService->HandleIMCacheEventL( 
	            cacheEventTypes[ i ] ) );	
	        }
        else
	        {	        	
	        TRAP( error, iImSubService->HandleIMCacheEventL( 
	            cacheEventTypes[ i ], &chatData ) );
	        }
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
        EUNIT_ASSERT( KErrNone == error );
        }
    
    //CleanupStack::PopAndDestroy(contactData.iRequesterId);
    CleanupStack::PopAndDestroy(chatData.iBuddyId);
    
    iImSubService->UnRegisterChatObserver( this );
    
    EUNIT_ASSERT( ETrue == iChatMsgReceived );
    
    CleanupStack::PopAndDestroy( &cacheEventTypes );
    }
        

EUNIT_BEGIN_TEST_TABLE( T_VimpstEngineImSubService, "T_VimpstEngineImSubService", "" )

EUNIT_TEST(
    "NewL - test ",
    "T_VimpstEngineImSubService",
    "NewL",
    "FUNCTIONALITY",
    SetupL, T_NewLL, Teardown)
    
EUNIT_TEST(
    "RegisterChatObserver - test ",
    "T_VimpstEngineImSubService",
    "RegisterChatObserver",
    "FUNCTIONALITY",
    SetupL, T_RegisterChatObserverL, Teardown)
    
EUNIT_TEST(
    "UnRegisterChatObserver - test ",
    "T_VimpstEngineImSubService",
    "UnRegisterChatObserver",
    "FUNCTIONALITY",
    SetupL, T_UnRegisterChatObserverL, Teardown)
    
EUNIT_TEST(
    "HandleIMCacheEventL - without RegisteredObservers ",
    "T_VimpstEngineImSubService",
    "HandleIMCacheEventL without RegisteredObservers",
    "FUNCTIONALITY",
    SetupL, T_HandleIMCacheEventLL, Teardown)
    
EUNIT_TEST(
    "GetUnreadCountL - test ",
    "T_VimpstEngineImSubService",
    "GetUnreadCountL",
    "FUNCTIONALITY",
    SetupL, T_GetUnreadCountL, Teardown)

EUNIT_TEST(
    "GetUnreadCountL - IMCacheAccessor Not Created ",
    "T_VimpstEngineImSubService",
    "GetUnreadCountL - IMCacheAccessor Not Created",
    "FUNCTIONALITY",
    SetupL, T_GetUnreadCountNoIMCacheAccessorL, Teardown)
    

    
EUNIT_TEST(
    "SubServiceState - test ",
    "T_VimpstEngineImSubService",
    "SubServiceState",
    "FUNCTIONALITY",
    SetupL, T_SubServiceStateL, Teardown)
    
EUNIT_TEST(
    "IsConversationExistL - test ",
    "T_VimpstEngineImSubService",
    "IsConversationExistL",
    "FUNCTIONALITY",
    SetupL, T_IsConversationExistLL, Teardown)

EUNIT_TEST(
    "IsConversationDoesntExistL - test ",
    "T_VimpstEngineImSubService",
    "T_IsConversationDoesntExistLL",
    "FUNCTIONALITY",
    SetupL, T_IsConversationDoesntExistLL, Teardown)


EUNIT_TEST(
    "CloseExisitingConversationL - test ",
    "T_VimpstEngineImSubService",
    "CloseExisitingConversationL",
    "FUNCTIONALITY",
    SetupL, T_CloseConversationExisitingConversationLL, Teardown)
        
EUNIT_TEST(
    "CloseNonExisitingConversationL - test ",
    "T_VimpstEngineImSubService",
    "CloseNonExisitingConversationL",
    "FUNCTIONALITY",
    SetupL, T_CloseConversationNonExisitingConversationLL, Teardown)

EUNIT_TEST(
    "IsConversationExistL - IMCacheAccessor Not Created",
    "T_VimpstEngineImSubService",
    "IsConversationExistL - IMCacheAccessor Not Created",
    "FUNCTIONALITY",
    SetupL, T_IsConversationExistNoIMCacheAccessorL, Teardown)


EUNIT_TEST(
    "CloseConversationL - IMCacheAccessor Not Created ",
    "T_VimpstEngineImSubService",
    "CloseConversationL - - IMCacheAccessor Not Created",
    "FUNCTIONALITY",
    SetupL, T_CloseConversationNoIMCacheAccessorL, Teardown)


    
EUNIT_TEST(
    "ResolveServiceStateL - test ",
    "T_VimpstEngineImSubService",
    "ResolveServiceStateL",
    "FUNCTIONALITY",
    SetupL, T_ResolveServiceStateLL, Teardown)
    
EUNIT_TEST(
    "Type - test ",
    "T_VimpstEngineImSubService",
    "Type",
    "FUNCTIONALITY",
    SetupL, T_TypeL, Teardown)
    
EUNIT_TEST(
    "CchEventOccuredL - test ",
    "T_VimpstEngineImSubService",
    "CchEventOccuredL",
    "FUNCTIONALITY",
    SetupL, T_CchEventOccuredLL, Teardown)
    
EUNIT_TEST(
    "DoHandleCchErrorL - test ",
    "T_VimpstEngineImSubService",
    "DoHandleCchErrorL",
    "FUNCTIONALITY",
    SetupL, T_DoHandleCchErrorLL, Teardown)
    
EUNIT_TEST(
    "CreateIMCacheAccessorL - test ",
    "T_VimpstEngineImSubService",
    "CreateIMCacheAccessorL",
    "FUNCTIONALITY",
    SetupL, T_CreateIMCacheAccessorLL, Teardown)
    
EUNIT_TEST(
    "ReleaseIMCacheAccessor - test ",
    "T_VimpstEngineImSubService",
    "ReleaseIMCacheAccessor",
    "FUNCTIONALITY",
    SetupL, T_ReleaseIMCacheAccessorL, Teardown)
    

EUNIT_TEST(
    "RegisterChatObserver - NULL Observer ",
    "T_VimpstEngineImSubService",
    "RegisterChatObserver - NULL Observer",
    "FUNCTIONALITY",
    SetupL, T_RegisterNULLChatObserverL, Teardown)
    
EUNIT_TEST(
    "UnRegisterChatObserver - NULL Observer",
    "T_VimpstEngineImSubService",
    "UnRegisterChatObserver - NULL Observer",
    "FUNCTIONALITY",
    SetupL, T_UnRegisterNULLChatObserverL, Teardown)  

EUNIT_TEST(
    "RegisterChatObserver - Twice Same Observer",
    "T_VimpstEngineImSubService",
    "RegisterChatObserver - Twice Same Observer",
    "FUNCTIONALITY",
    SetupL, T_RegisterChatObserverTwiceL, Teardown)  


EUNIT_TEST(
    "UnRegisterChatObserver - Twice Same Observer",
    "T_VimpstEngineImSubService",
    "UnRegisterChatObserver - Twice Same Observer",
    "FUNCTIONALITY",
    SetupL, T_UnRegisterChatObserverTwiceL, Teardown)  


EUNIT_TEST(
    "HandleIMCacheEventL - with RegisteredObserver ",
    "T_VimpstEngineImSubService",
    "HandleIMCacheEventL with RegisteredObserver",
    "FUNCTIONALITY",
    SetupL, T_HandleIMCacheEventWithRegisteredObserverL, Teardown)
        
    
    
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
