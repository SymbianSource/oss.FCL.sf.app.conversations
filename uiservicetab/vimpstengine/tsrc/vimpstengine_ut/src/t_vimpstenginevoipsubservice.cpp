/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstenginevoipsubservice.cpp
*
*/
#include <platform/digia/eunit/eunitmacros.h>
#include <platform/digia/eunit/teunitassertioninfo.h>
#include <platform/digia/eunit/CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginevoipsubservice.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>

#include "cvimpstenginevoipsubservice.h"
#include "cvimpstenginecchhandler.h"
#include "cvimpstengineservicetablefetcher.h"

const TInt KTestServiceId = 5;

T_VimpstEngineVoipSubService* T_VimpstEngineVoipSubService::NewLC()
    {
    T_VimpstEngineVoipSubService* self = new( ELeave ) T_VimpstEngineVoipSubService;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEngineVoipSubService::~T_VimpstEngineVoipSubService()
    {
    }

void T_VimpstEngineVoipSubService::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


//  FROM OBSERVER INTERFACES
void T_VimpstEngineVoipSubService::HandleServceConnectionEventL()
    {
    }


void T_VimpstEngineVoipSubService::SetupL()
    {
    iTestServiceId = CreateSpEntryL( KTestServiceId );
   // MVIMPSTEngineCchUiEventObserver *aCchUiEventObserver;
    
    iCchHandler = CVIMPSTEngineCchHandler::NewL( iTestServiceId, *iCchUiEventObserver);
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL(); 
    
    iVoIPSubService = CVIMPSTEngineVOIPSubService::NewL( 
        iTestServiceId,
        *iCchHandler,
        *iServiceTableFetcher,
        *this );
    }

void T_VimpstEngineVoipSubService::Teardown()
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
   
    if ( iVoIPSubService )
        {
        delete iVoIPSubService;
        iVoIPSubService = NULL;
        }
    }

TInt T_VimpstEngineVoipSubService::CreateSpEntryL( TInt aServiceId )
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

void T_VimpstEngineVoipSubService::T_NewLL()
    {
    CVIMPSTEngineVOIPSubService* voipSubService = NULL;
    voipSubService = CVIMPSTEngineVOIPSubService::NewL(
        iTestServiceId,
        *iCchHandler,
        *iServiceTableFetcher,
        *this);
       
    EUNIT_ASSERT( voipSubService != NULL );
       
    delete voipSubService;
    voipSubService = NULL;
    }

void T_VimpstEngineVoipSubService::T_CchEventOccuredLL()
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
        TRAP( error, iVoIPSubService->CchEventOccuredL( 
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
        TRAP( error, iVoIPSubService->CchEventOccuredL( 
            iTestServiceId, subServiceStates[ j ], serviceError ) );
            
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
                   
        EUNIT_ASSERT( KErrNone == error );
        }
        
    CleanupStack::PopAndDestroy( &subServiceStates );
    }

void T_VimpstEngineVoipSubService::T_DoHandleCchErrorLL()
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
        TRAPD( err, iVoIPSubService->DoHandleCchErrorL( 
            serviceErrors[ i ] ) );
        
        if ( KErrNoMemory == err )
            {
            User::Leave( err );
            }
                      
        EUNIT_ASSERT( KErrNone == err );
        }
    
    CleanupStack::PopAndDestroy( &serviceErrors );
    }

void T_VimpstEngineVoipSubService::T_ResolveServiceStateL()
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
        TRAP( error, iVoIPSubService->ResolveServiceState( 
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
        TRAP( error, iVoIPSubService->ResolveServiceState( 
            subServiceStates[ j ], serviceError ) );
                
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
                       
        EUNIT_ASSERT( KErrNone == error );
        }
        
    CleanupStack::PopAndDestroy( &subServiceStates );
    }

void T_VimpstEngineVoipSubService::T_SubServiceStateL()
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
        iVoIPSubService->iServiceState = states[ i ];
        EUNIT_ASSERT( states[ i ] == iVoIPSubService->SubServiceState() );
        }
       
    CleanupStack::PopAndDestroy( &states );
    }

void T_VimpstEngineVoipSubService::T_TypeL()
    {    
    EUNIT_ASSERT( TVIMPSTEnums::EVoip == iVoIPSubService->Type() );
    }

EUNIT_BEGIN_TEST_TABLE( T_VimpstEngineVoipSubService, "T_VimpstEngineVoipSubService", "" )

EUNIT_TEST(
    "NewL - test ",
    "T_VimpstEngineVoipSubService",
    "NewL",
    "FUNCTIONALITY",
    SetupL, T_NewLL, Teardown)
    
EUNIT_TEST(
    "CchEventOccuredL - test ",
    "T_VimpstEngineVoipSubService",
    "CchEventOccuredL",
    "FUNCTIONALITY",
    SetupL, T_CchEventOccuredLL, Teardown)
    
EUNIT_TEST(
    "DoHandleCchErrorL - test ",
    "T_VimpstEngineVoipSubService",
    "DoHandleCchErrorL",
    "FUNCTIONALITY",
    SetupL, T_DoHandleCchErrorLL, Teardown)
    
EUNIT_TEST(
    "ResolveServiceState - test ",
    "T_VimpstEngineVoipSubService",
    "ResolveServiceState",
    "FUNCTIONALITY",
    SetupL, T_ResolveServiceStateL, Teardown)
    
EUNIT_TEST(
    "SubServiceState - test ",
    "T_VimpstEngineVoipSubService",
    "SubServiceState",
    "FUNCTIONALITY",
    SetupL, T_SubServiceStateL, Teardown)
    
EUNIT_TEST(
    "Type - test ",
    "T_VimpstEngineVoipSubService",
    "Type",
    "FUNCTIONALITY",
    SetupL, T_TypeL, Teardown)

EUNIT_END_TESTCASE_TABLE

//  END OF FILE
