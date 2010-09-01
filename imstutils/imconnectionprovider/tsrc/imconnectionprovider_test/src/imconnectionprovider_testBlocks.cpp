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
* Description:  imconnectionprovider_testBlocks.cpp
*
*/



// [INCLUDE FILES] - do not remove

#include "imconnectionprovider_test.h"
#include <e32std.h>
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <e32def.h>
#include <spdefinitions.h>
#include <ximperrors.hrh>
#include "cchserviceobserver.h"

extern TBool setpassword ;
// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cimconnectionprovider_test::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cimconnectionprovider_test::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "T_EnableServiceL", 					Cimconnectionprovider_test::T_EnableServiceL ),
        ENTRY( "T_DisableServiceL", 				Cimconnectionprovider_test::T_DisableServiceL ),
        ENTRY( "T_SetIMDisabledL", 					Cimconnectionprovider_test::T_SetIMDisabledL),
        ENTRY( "T_FindRequestId",                   Cimconnectionprovider_test::T_FindRequestId),
        ENTRY( "T_RemoveRequestId", 				Cimconnectionprovider_test::T_RemoveRequestId),
		ENTRY( "T_UnBindL", 						Cimconnectionprovider_test::T_UnBindL),
		ENTRY( "T_BindL", 							Cimconnectionprovider_test::T_BindL),
		ENTRY( "T_IsReserved", 						Cimconnectionprovider_test::T_IsReserved),
		ENTRY( "T_SetConnectionParameter", 			Cimconnectionprovider_test::T_SetConnectionParameter),
		ENTRY( "T_GetConnectionParameter", 		    Cimconnectionprovider_test::T_GetConnectionParameter),
		ENTRY( "T_ValidateServiceL",               Cimconnectionprovider_test::T_ValidateServiceL),
		ENTRY( "T_ValidateSubService",             Cimconnectionprovider_test::T_ValidateSubService),
		ENTRY( "T_GetServiceNameL", Cimconnectionprovider_test::T_GetServiceNameL),
		ENTRY( "T_ConvertXIMPErrToCChErr", Cimconnectionprovider_test::T_ConvertXIMPErrToCChErr),
		ENTRY( "T_GetSPSettingsIntPropertyL", Cimconnectionprovider_test::T_GetSPSettingsIntPropertyL),
		ENTRY( "T_GetServiceNetworkInfo_passwordExistL", Cimconnectionprovider_test::T_GetServiceNetworkInfo_passwordExistL),
		ENTRY( "T_GetServiceNetworkInfo_emptypasswordL", Cimconnectionprovider_test::T_GetServiceNetworkInfo_emptypasswordL),
        };


    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::NewL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CIcpServiceManager* Cimconnectionprovider_test::NewL( MCchServiceObserver* aServiceObserver )
	{
	//MCchServiceObserver& aServiceObserver = NULL;
    CIcpServiceManager* self = CIcpServiceManager::NewL( *this );
    return self;
	}


void Cimconnectionprovider_test::ServiceStateChanged(
                    const TServiceSelection aServiceSelection,
                    TCCHSubserviceState aState, 
                    TInt aError )
{
	
}

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_EnableServiceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimconnectionprovider_test::T_EnableServiceL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
  	CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
   	if(icpManager->iPresenceCtx)
	   	{
		delete icpManager->iPresenceCtx;
	   	}
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();
	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
	CleanupStack::Pop( icpManager->iPresenceCtx );
    icpManager->EnableServiceL(1,ECCHIMSub);
    delete icpManager;
    return KErrNone;
    
    }



// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_DisableServiceL
// T_DisableServiceL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimconnectionprovider_test::T_DisableServiceL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
 //   TCCHSubserviceType type = TCCHSubserviceType::ECCHIMSub;
    icpManager->DisableServiceL(1,ECCHIMSub);
       	if(icpManager->iPresenceCtx)
	   	{
		delete icpManager->iPresenceCtx;
	   	}
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();
	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
	CleanupStack::Pop( icpManager->iPresenceCtx );
    delete icpManager;
    return KErrNone;
   
    }


	

	

	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_SetIMDisabledL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_SetIMDisabledL( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	icpManager->SetIMDisabledL(0);
	delete icpManager;
	return KErrNone;
	}



	
	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_RemoveRequestId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_RemoveRequestId( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();


   	if(icpManager->iPresenceCtx)
   	{
	delete icpManager->iPresenceCtx;
   	}	
   	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
   	CleanupStack::Pop( icpManager->iPresenceCtx );
   
    TXIMPRequestId reqId ;
	
	
	icpManager->RemoveRequestId(reqId);
	
	delete icpManager;
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_FindRequestId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_FindRequestId( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();


   	if(icpManager->iPresenceCtx)
   	{
	delete icpManager->iPresenceCtx;
   	}	
   	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
   	CleanupStack::Pop( icpManager->iPresenceCtx );
   	
	
    TXIMPRequestId reqId; 
	icpManager->FindRequestId(reqId);
	
	delete icpManager;
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_UnBindL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_UnBindL( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();


   	if(icpManager->iPresenceCtx)
   	{
	delete icpManager->iPresenceCtx;
   	}	
   	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
   	CleanupStack::Pop( icpManager->iPresenceCtx );
   
   	
   	icpManager->UnBindL();
	delete icpManager;
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_BindL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_BindL( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	
	if(icpManager->iPresClient)
		{
		delete icpManager->iPresClient;
		}
	icpManager->iPresClient = MXIMPClientStub::NewClientL();


   	if(icpManager->iPresenceCtx)
   	{
	delete icpManager->iPresenceCtx;
   	}	
   	icpManager->iPresenceCtx = (icpManager->iPresClient)->NewPresenceContextLC();
   	CleanupStack::Pop( icpManager->iPresenceCtx );
   	
	
   	CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
   	icpManager->BindL(1);
	delete icpManager;
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_IsReserved
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_IsReserved( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
	icpManager->IsReserved(1,ECCHIMSub);
	delete icpManager;
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_SetConnectionParameter
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_SetConnectionParameter( CStifItemParser& aItem )
	{
	CIcpServiceManager *icpManager = NewL(NULL);
	CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
	icpManager->SetConnectionParameter(1,ECCHIMSub,ECchUsername,1/*KServreName*/);
	icpManager->SetConnectionParameter(1,ECCHIMSub,ECchPassword,1/*KServreName*/);
	icpManager->SetConnectionParameter(1,ECCHIMSub,ECchIapId,0);
	icpManager->SetConnectionParameter(1,ECCHIMSub,ECchSnapId,0);
	delete icpManager;
	return KErrNone;
	}

	

// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_HandlePresenceNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_GetConnectionParameter( CStifItemParser& aItem )
	{
	TInt dunny;
	CIcpServiceManager *icpManager = NewL(NULL);
	CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
	icpManager->GetConnectionParameter(1,ECCHIMSub,ECchIapId,dunny);
	icpManager->GetConnectionParameter(1,ECCHIMSub,ECchSnapId,dunny);
	icpManager->GetConnectionParameter(1,ECCHIMSub,ECchSnapLocked,dunny);
	icpManager->GetConnectionParameter(1,ECCHIMSub,ECchPasswordSet,dunny);
	delete icpManager;
	return KErrNone;
	}
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_ValidateServiceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_ValidateServiceL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    icpManager->ValidateServiceL(1);
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::T_GetSPSettingsIntPropertyL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt Cimconnectionprovider_test::T_GetSPSettingsIntPropertyL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    icpManager->GetSPSettingsIntPropertyL(1,EServiceName);
    return KErrNone;
    }


TInt Cimconnectionprovider_test::T_ValidateSubService( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    icpManager->ValidateSubService(ECCHUnknown);
    icpManager->ValidateSubService(ECCHPresenceSub);
    icpManager->ValidateSubService(ECCHIMSub);
    return KErrNone;
    }

TInt Cimconnectionprovider_test::T_GetServiceNameL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    HBufC* serviceName = HBufC::NewLC( 100 );
    TPtr serviceNamePtr( serviceName->Des() );
    icpManager->GetServiceNameL(1,serviceNamePtr );
    //icpManager->GetServiceNameL()
    TInt ret = KErrNotFound;
    if( serviceNamePtr.Length() )
        {
        ret = KErrNone;
        }
    CleanupStack::PopAndDestroy(); // serviceName
    return ret;
    }

TInt Cimconnectionprovider_test::T_GetServiceNetworkInfo_passwordExistL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    TCCHSubserviceType aSubServiceType;
    TUint32 aSnapId = 0; 
    TUint32 aIapId =0;
    TBool aSnapLocked = EFalse;
    TBool aPasswordSet; 
    setpassword = ETrue;   //password is set
    
    icpManager->GetServiceNetworkInfoL(1,ECCHUnknown,aSnapId,aIapId,aSnapLocked,aPasswordSet);
    if(aPasswordSet)
        {
        return KErrNone;
        }
    else
        {
        return KErrNotFound;
        }
    }

TInt Cimconnectionprovider_test::T_GetServiceNetworkInfo_emptypasswordL( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    TCCHSubserviceType aSubServiceType;
    TUint32 aSnapId = 0; 
    TUint32 aIapId =0;
    TBool aSnapLocked = EFalse;
    TBool aPasswordSet; 
    setpassword = EFalse;   //password is empty
    
    icpManager->GetServiceNetworkInfoL(1,ECCHUnknown,aSnapId,aIapId,aSnapLocked,aPasswordSet);
    if(aPasswordSet)
        {
        return KErrNotFound; //test case failed
        }
    else
        {
        return KErrNone;   //test case passed
         
        }
    }
TInt Cimconnectionprovider_test::T_ConvertXIMPErrToCChErr( CStifItemParser& aItem )
    {
    CIcpServiceManager *icpManager = NewL(NULL);
    CreateSpEntryL( 1, EPropertyPCSPluginId, 10 );
    icpManager->ConvertXIMPErrToCChErr(KXIMPErrServiceGeneralError);
    icpManager->ConvertXIMPErrToCChErr(KXIMPErrServiceShutdown);
    icpManager->ConvertXIMPErrToCChErr(KXIMPErrServiceRequestTypeNotSupported);
    icpManager->ConvertXIMPErrToCChErr(KXIMPErrServiceAuthenticationFailed);
    return KErrNone;
    }
	
	
void Cimconnectionprovider_test::CreateSpEntryL( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TInt aValue )
    {
    CSPSettings* settings = CSPSettings::NewL();
    
    CSPProperty* property = CSPProperty::NewL();
    CleanupStack::PushL( property );
    property->SetName( aName );
    property->SetValue( aValue );

    CSPEntry* entry = CSPEntry::NewL();
    CleanupStack::PushL( entry );
    entry->SetServiceId( aServiceId );
    entry->SetServiceName(KServiceName() );

    entry->AddPropertyL( *property );

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
    CleanupStack::PopAndDestroy( 3 );
    }    
    
// -----------------------------------------------------------------------------
// Cimconnectionprovider_test::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Cimconnectionprovider_test::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
