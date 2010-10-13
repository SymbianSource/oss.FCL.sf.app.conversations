/*
* t_vimpstenginecchhandler.cpp :  Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: t_vimpstenginecchhandler.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include <cch.h>
#include "t_vimpstenginecchhandler.h"
#include "cvimpstenginecchhandler.h"


#include "s_cch.h"
#include "s_mcchuiobserver.h"

extern MyCchService* myService;
extern TBool createService ;
extern TBool tryAllSubserviceAvailable ;
extern TBool tryVoipSubServiceAvailable ;
extern TBool tryPresenceSubServiceAvailable ;
extern TBool tryIMSubServiceAvailable ;
extern TBool tryConnParamLeaveWithNotSupported ;
extern TBool tryConnParamWithSuccess ;
extern TBool sendObserverEvent;
extern TBool tryConnParamWithFailure ;


T_VimpstEngineCchHandler* T_VimpstEngineCchHandler::NewLC()
    {
    T_VimpstEngineCchHandler* self = new( ELeave ) T_VimpstEngineCchHandler;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEngineCchHandler::~T_VimpstEngineCchHandler()
    {
    }

void T_VimpstEngineCchHandler::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void T_VimpstEngineCchHandler::SetupL()
    {
    iCChHandler = CVIMPSTEngineCchHandler::NewL( 3,*iCchUiEventObserver );
    }
    
void T_VimpstEngineCchHandler::SetupL1()
    {
    createService = ETrue;
    iCChHandler = CVIMPSTEngineCchHandler::NewL( 3 ,*iCchUiEventObserver);
    }    

void T_VimpstEngineCchHandler::Teardown()
    {
    delete myService;
    myService = NULL;    
    createService = EFalse;
    
    tryAllSubserviceAvailable = EFalse;
	tryVoipSubServiceAvailable = EFalse;
	tryPresenceSubServiceAvailable = EFalse;
	tryIMSubServiceAvailable = EFalse;
	
	tryConnParamLeaveWithNotSupported = EFalse;
	tryConnParamWithSuccess = EFalse;
    
    delete iCChHandler;
    iCChHandler = NULL;    
    }


void T_VimpstEngineCchHandler::TestConstructionL()
    {
    
    EUNIT_ASSERT( iCChHandler );
    
    }
    
void T_VimpstEngineCchHandler::TestGetServiceStateServiceNotFoundL()
    {
    
    TCCHSubserviceState state;
    EUNIT_ASSERT_EQUALS( KErrNotFound, iCChHandler->GetServiceState( 99, ECCHVoIPSub, state ) ); 
    
    
    }
  
    
void T_VimpstEngineCchHandler::TestGetServiceStateServiceFoundL()
    {
    
    TCCHSubserviceState state;
    
    createService = ETrue;
    tryVoipSubServiceAvailable = ETrue;
    
    EUNIT_ASSERT_EQUALS( KErrNone, iCChHandler->GetServiceState( 5, ECCHVoIPSub, state ) );    
    
    }


void T_VimpstEngineCchHandler::TestEnableServiceServiceFoundL() 
	{
	
    createService = ETrue;
    EUNIT_ASSERT_EQUALS( KErrNone, iCChHandler->EnableService() ); 
    	
	}
	
	
void T_VimpstEngineCchHandler::TestEnableServiceServiceNotFoundL() 
	{
	
    EUNIT_ASSERT_EQUALS( KErrNotFound, iCChHandler->EnableService() ); 
    
	}
	
void T_VimpstEngineCchHandler::TestDisableServiceServiceFoundL() 
	{
	
    createService = ETrue;
    EUNIT_ASSERT_EQUALS( KErrNone, iCChHandler->DisableService() ); 
    	
	}
	
	
void T_VimpstEngineCchHandler::TestDisableServiceServiceNotFoundL() 
	{
	
    EUNIT_ASSERT_EQUALS( KErrNotFound, iCChHandler->DisableService() ); 
    
	}	
/*	
void T_VimpstEngineCchHandler::ValidateServiceServiceNotFoundL()
	{
	EUNIT_ASSERT_EQUALS( EFalse, iCChHandler->ValidateService(99) ); 		
	}*/
/*	
void T_VimpstEngineCchHandler::ValidateServiceAllSubserviceAvailableL()
	{
	createService = ETrue;
	tryAllSubserviceAvailable = ETrue;
	
	EUNIT_ASSERT_EQUALS( ETrue, iCChHandler->ValidateService(99) ); 		
	}
*/
/*
void T_VimpstEngineCchHandler::ValidateServiceAnyOneSubserviceAvailableL()
	{
	createService = ETrue;
	tryPresenceSubServiceAvailable = ETrue;
	
	EUNIT_ASSERT_EQUALS( ETrue, iCChHandler->ValidateService(99) ); 		
	}		
*/

void T_VimpstEngineCchHandler::GetConParametersServiceNotFoundL()
	{
	HBufC* temp = NULL;
	
	createService = ETrue;  
	tryConnParamWithFailure = ETrue;
	
	TRAPD(err, temp = iCChHandler->GetConParametersL(ECchUsername));
	
	EUNIT_ASSERT_EQUALS( KErrNotFound, err ); 
	
	if (temp)
		{
		User::Leave( KErrArgument ); // For alloc failure testing	  	
		}	
			
	}
	
void T_VimpstEngineCchHandler::GetConParametersServiceFoundL()
	{
	HBufC* temp = NULL;
	
	createService = ETrue;	
	tryConnParamWithSuccess = ETrue;
	tryConnParamWithFailure = EFalse;
	
	TRAPD(err, temp = iCChHandler->GetConParametersL(ECchUsername));	
	
	EUNIT_ASSERT_EQUALS( KErrNone, err ); 	
	
	CleanupStack::PushL(temp);
	
	if (temp->Des().Compare(KTestUserName()))
		{
		User::Leave( KErrArgument ); // For alloc failure testing	    		
		}
	
	CleanupStack::PopAndDestroy(temp);
	}
	

void T_VimpstEngineCchHandler::GetConParametersLeavesServiceFoundL()
	{
	HBufC* temp = NULL;
	
	createService = ETrue;
	tryConnParamLeaveWithNotSupported = ETrue;	
	
	TRAPD(err, temp = iCChHandler->GetConParametersL(ECchUsername));
	
	EUNIT_ASSERT_EQUALS( KErrNotSupported, err ); 	
	
	if (temp)
		{
		User::Leave( KErrArgument ); // For alloc failure testing	  	
		}	
	}		
		
		
		

void T_VimpstEngineCchHandler::SetConParametersServiceNotFoundL()
	{
    
	TInt err = iCChHandler->SetConnectionParameter(ECchUsername, KTestUserName);
	
	EUNIT_ASSERT_EQUALS( KErrNotFound, err ); 	
			
	}
	
void T_VimpstEngineCchHandler::SetConParametersServiceFoundL()
	{
	createService = ETrue;	
	tryConnParamWithSuccess = ETrue;
	
	TInt err = iCChHandler->SetConnectionParameter(ECchUsername, KTestUserName);	
	
	EUNIT_ASSERT_EQUALS( KErrNone, err ); 	
	
	}
	

void T_VimpstEngineCchHandler::SetConParametersLeavesServiceFoundL()
	{
	createService = ETrue;
	tryConnParamLeaveWithNotSupported = ETrue;	
	
	TInt err = iCChHandler->SetConnectionParameter(ECchUsername, KTestUserName);
	
	EUNIT_ASSERT_EQUALS( KErrNotSupported, err ); 			
	}				
		    		

void T_VimpstEngineCchHandler::ShutdownCch()
	{
	iCChHandler->ShutdownCch();
	}			    		

void T_VimpstEngineCchHandler::CheckObservingForPresenceSubServiceL()
	{
	sendObserverEvent = ETrue;
	tryPresenceSubServiceAvailable = ETrue;
	
	iCallingEnable = ETrue;
	iGotObserverEvent = EFalse;
	
	iCChHandler->RegisterCchObserverL(this, ECCHPresenceSub);
	iCChHandler->EnableService();
	iCChHandler->UnRegisterCchObserver(ECCHPresenceSub);	
	
	EUNIT_ASSERT_EQUALS( ETrue, iGotObserverEvent ); 	
	}
	
void T_VimpstEngineCchHandler::CheckNoObservingForPrsenceSubServiceL()
	{
	sendObserverEvent = ETrue;
	tryPresenceSubServiceAvailable = ETrue;
	
	iCallingEnable = ETrue;
	iGotObserverEvent = EFalse;
	
	iCChHandler->RegisterCchObserverL(this, ECCHIMSub);
	iCChHandler->EnableService();
	iCChHandler->UnRegisterCchObserver(ECCHIMSub);
	
	
	EUNIT_ASSERT_EQUALS( EFalse, iGotObserverEvent ); 	
	}	

void T_VimpstEngineCchHandler::RegisterObserverWithNull()
	{
	sendObserverEvent = ETrue;
	tryPresenceSubServiceAvailable = ETrue;
	
	iCallingEnable = ETrue;
	iGotObserverEvent = EFalse;
	
	iCChHandler->RegisterCchObserverL(NULL, ECCHUnknown);
	iCChHandler->EnableService();
	iCChHandler->UnRegisterCchObserver(ECCHUnknown);
	
	
	EUNIT_ASSERT_EQUALS( EFalse, iGotObserverEvent ); 	
	}


void T_VimpstEngineCchHandler::CchEventOccuredL( 
            TUint /*aServiceId*/, 
            TCCHSubserviceState aState, 
            TInt /*aServiceError*/ )
	{
	
	if (iCallingEnable)
		{
		if (ECCHEnabled == aState)
			{
			iGotObserverEvent = ETrue;				
			}
		}
	}
	
void T_VimpstEngineCchHandler::CheckObservingWithoutRegisteringL()
	{
	sendObserverEvent = ETrue;
	
	iCallingEnable = ETrue;
	iGotObserverEvent = EFalse;
	
	iCChHandler->EnableService();	
	
	EUNIT_ASSERT_EQUALS( EFalse, iGotObserverEvent ); 	
	}
	
            		    		

EUNIT_BEGIN_TEST_TABLE( T_VimpstEngineCchHandler, "T_VimpstEngineCchHandler", "" )

    EUNIT_TEST("TestConstructionL ",
            "T_VimpstEngineCchHandler",
            "ConstructL",
            "FUNCTIONALITY",
            SetupL, TestConstructionL, Teardown )
    EUNIT_TEST("TestGetServiceStateServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "GetServiceState",
            "FUNCTIONALITY", 
              SetupL, TestGetServiceStateServiceNotFoundL, Teardown )
    EUNIT_TEST( "TestGetServiceStateServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "GetServiceState",
            "FUNCTIONALITY",
            SetupL, TestGetServiceStateServiceFoundL, Teardown )
    EUNIT_TEST( "TestEnableServiceServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "EnableService",
            "FUNCTIONALITY",
            SetupL, TestEnableServiceServiceFoundL, Teardown )
    EUNIT_TEST( "TestEnableServiceServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "EnableService",
            "FUNCTIONALITY",
            SetupL, TestEnableServiceServiceNotFoundL, Teardown )
   /* EUNIT_TEST( "ValidateServiceServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "ValidateService",
            "FUNCTIONALITY",
            SetupL, ValidateServiceServiceNotFoundL, Teardown )
    EUNIT_TEST("ValidateServiceAllSubserviceAvailableL ",
            "T_VimpstEngineCchHandler",
            "ValidateService",
            "FUNCTIONALITY",
            SetupL, ValidateServiceAllSubserviceAvailableL, Teardown )
    EUNIT_TEST("ValidateServiceAnyOneSubserviceAvailableL ",
            "T_VimpstEngineCchHandler",
            "ValidateService",
            "FUNCTIONALITY",
            SetupL, ValidateServiceAnyOneSubserviceAvailableL, Teardown )  
            */  
    EUNIT_TEST( "GetConParametersServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "GetConParametersL",
            "FUNCTIONALITY",
            SetupL, GetConParametersServiceNotFoundL, Teardown )
    EUNIT_TEST( "GetConParametersServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "GetConParametersL",
            "FUNCTIONALITY",
            SetupL, GetConParametersServiceFoundL, Teardown )
    EUNIT_TEST("GetConParametersLeavesServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "GetConParametersL",
            "FUNCTIONALITY",
            SetupL, GetConParametersLeavesServiceFoundL, Teardown )
    EUNIT_TEST( "SetConParametersServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "SetConParametersL",
            "FUNCTIONALITY",
            SetupL, SetConParametersServiceNotFoundL, Teardown )
    EUNIT_TEST( "SetConParametersServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "SetConParametersL",
            "FUNCTIONALITY",
            SetupL, SetConParametersServiceFoundL, Teardown )
    EUNIT_TEST("SetConParametersLeavesServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "SetConParametersL",
            "FUNCTIONALITY",
            SetupL, SetConParametersLeavesServiceFoundL, Teardown )
    EUNIT_TEST( "TestDisableServiceServiceFoundL ",
            "T_VimpstEngineCchHandler",
            "DisableService",
            "FUNCTIONALITY",
            SetupL, TestDisableServiceServiceFoundL, Teardown )
    EUNIT_TEST( "TestDisableServiceServiceNotFoundL ",
            "T_VimpstEngineCchHandler",
            "DisableService",
            "FUNCTIONALITY",
            SetupL, TestDisableServiceServiceNotFoundL, Teardown )
    EUNIT_TEST( "ShutdownCch ",
            "T_VimpstEngineCchHandler",
            "ShutdownCch",
            "FUNCTIONALITY",
            SetupL, ShutdownCch, Teardown )
    EUNIT_TEST( "CheckObservingForPresenceSubServiceL ",
            "T_VimpstEngineCchHandler",
            "CheckObservingForPresenceSubServiceL",
            "FUNCTIONALITY",
            SetupL1, CheckObservingForPresenceSubServiceL, Teardown )
    EUNIT_TEST( "CheckObservingWithoutRegisteringL ",
            "T_VimpstEngineCchHandler",
            "CheckObservingWithoutRegisteringL",
            "FUNCTIONALITY",
            SetupL1, CheckObservingWithoutRegisteringL, Teardown )
    EUNIT_TEST( "RegisterObserverWithNull ",
            "T_VimpstEngineCchHandler",
            "RegisterObserverWithNull",
            "FUNCTIONALITY",
            SetupL1, RegisterObserverWithNull, Teardown )  
    EUNIT_TEST( "CheckNoObservingForPrsenceSubServiceL ",
            "T_VimpstEngineCchHandler",
            "CheckNoObservingForPrsenceSubServiceL",
            "FUNCTIONALITY",
            SetupL1, CheckNoObservingForPrsenceSubServiceL, Teardown )  
     
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
