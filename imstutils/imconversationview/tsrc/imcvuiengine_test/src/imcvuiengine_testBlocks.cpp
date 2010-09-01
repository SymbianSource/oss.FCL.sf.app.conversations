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
* Description: imcvuiengine_testBlocks.cpp
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "imcvuiengine_test.h"
#include "imcvuiengine_testsendmessage.h"
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
// Cimcvuiengine_test::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cimcvuiengine_test::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "StartNewConvesationL", Cimcvuiengine_test::StartNewConvesationL ),
        ENTRY( "CloseConvesationL", Cimcvuiengine_test::CloseConvesationL ),
		ENTRY( "SendNullMessageL", Cimcvuiengine_test::SendNullMessageL ),
        ENTRY( "SendTextMessageL", Cimcvuiengine_test::SendTextMessageL ),
        ENTRY( "SendSplCharMessageL", Cimcvuiengine_test::SendSplCharMessageL ),
        ENTRY( "SendCosMessageL", Cimcvuiengine_test::SendCosMessageL ),
        ENTRY( "DeactivateConvesationL", Cimcvuiengine_test::DeactivateConvesationL ),
        ENTRY( "TestMultipleSendMessageL", Cimcvuiengine_test::TestMultipleSendMessageL ),
        ENTRY( "TestSendRecieveMsgL", Cimcvuiengine_test::TestSendRecieveMsgL ),
        ENTRY( "TestSendFetchSendMsgL", Cimcvuiengine_test::TestSendFetchSendMsgL ),
        // newly added test cases
        ENTRY( "TestFailedMsgL", Cimcvuiengine_test::TestFailedMsgL ),
        ENTRY( "TestConnectionTerminationL", Cimcvuiengine_test::TestConnectionTerminationL ),
        ENTRY( "TestLoginStateL", Cimcvuiengine_test::TestLoginStateL ),
        ENTRY( "TestLoginUserIdL", Cimcvuiengine_test::TestLoginUserIdL ), 
        ENTRY( "TestChatInterfaceL", Cimcvuiengine_test::TestChatInterfaceL ), 
        
        ENTRY( "TestMessageCoundL", Cimcvuiengine_test::TestMessageCoundL ), 
        ENTRY( "TestMessageUnreadCoundL", Cimcvuiengine_test::TestMessageUnreadCoundL ), 
        
        ENTRY( "TestDeleteContextL", Cimcvuiengine_test::TestDeleteContextL ), 
        ENTRY( "TestCreateContextL", Cimcvuiengine_test::TestCreateContextL ), 
        
        ENTRY( "TestSubscribePresenceL", Cimcvuiengine_test::TestSubscribePresenceL ),     
        ENTRY( "TestAppendInfoMessageL", Cimcvuiengine_test::TestAppendInfoMessageL ), 
        ENTRY( "TestGetPresenceStatusL", Cimcvuiengine_test::TestGetPresenceStatusL ),         
        ENTRY( "TestGetBrandIdL", Cimcvuiengine_test::TestGetBrandIdL ), 
        ENTRY( "TestGetLanguageIdL", Cimcvuiengine_test::TestGetLanguageIdL ),  
        
        // =======================================================================
        ENTRY( "TestCCHHandlerL", Cimcvuiengine_test::TestCCHHandlerL ),        
       	ENTRY( "ReadyForShutdownTestL", Cimcvuiengine_test::ReadyForShutdownTestL ),     
        ENTRY( "ServiceNameTestL", Cimcvuiengine_test::ServiceNameTestL ), 
        ENTRY( "OpenChatsTestL", Cimcvuiengine_test::OpenChatsTestL ),         
        ENTRY( "GetServiceIdTestL", Cimcvuiengine_test::GetServiceIdTestL ), 
        ENTRY( "TestClosAllContainerL", Cimcvuiengine_test::TestClosAllContainerL ),         
        ENTRY( "TestDeleteChatL", Cimcvuiengine_test::TestDeleteChatL ), 
        ENTRY( "TestEditorMessageL", Cimcvuiengine_test::TestEditorMessageL ),
        ENTRY( "TestConvertPresCacheEnum", Cimcvuiengine_test::TestConvertPresCacheEnum ),
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::StartNewConvesationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::StartNewConvesationL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
  	TInt err = iSender->StartNewConvesationL();
  	iSender->ReleaseConnectionL();
	return err;
	}

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::CloseConvesationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::CloseConvesationL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->CloseConvesationL();
 	iSender->ReleaseConnectionL();
    return err ;
    }


// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendTextMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::SendTextMessageL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
	TInt err = iSender->SendTextMessageL();
	iSender->ReleaseConnectionL();
    return err;
    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendNullMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::SendNullMessageL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->SendNullMessageL();
 	iSender->ReleaseConnectionL();
    return err ;
    }


// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendSplCharMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::SendSplCharMessageL( CStifItemParser& /*aItem*/ )
    {
    
    iSender->CreateConnectionL();
  	TInt err = iSender->SendSplCharMessageL();
  	iSender->ReleaseConnectionL();
    return err;
    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::SendCosMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::SendCosMessageL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->SendCosMessageL();
 	iSender->ReleaseConnectionL();
    return err ;
    }


// -----------------------------------------------------------------------------
// Cimcvuiengine_test::DeactivateConvesationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::DeactivateConvesationL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
  	TInt err = iSender->DeactivateConvesationL();
  	iSender->ReleaseConnectionL();
    return err;
    }



// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestMultipleSendMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestMultipleSendMessageL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
  	TInt err = iSender->TestMultipleSendMessageL();
  	iSender->ReleaseConnectionL();
    return err;
    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestSendRecieveMsgL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestSendRecieveMsgL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestSendRecieveMsgL();
 	iSender->ReleaseConnectionL();
    return err ;
    }

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestSendFetchSendMsgL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestSendFetchSendMsgL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestSendFetchSendMsgL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestFailedMsgL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestFailedMsgL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->SendToInvaliedRecipientL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestConnectionTerminationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestConnectionTerminationL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestConnectionTerminationL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestLoginStateL
// Example test method function.TestLoginStateL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestLoginStateL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestLoginStateL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestLoginUserIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestLoginUserIdL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestLoginUserIdL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestGetBrandIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestGetBrandIdL( CStifItemParser& /*aItem*/ )
    {
    
    TInt err = iSender->TestGetBrandIdL();    
    return err ;
    }
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestGetLanguageIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestGetLanguageIdL( CStifItemParser& /*aItem*/ )
    {
    TInt err = iSender->TestGetLanguageIdL();    
    return err ;
    }

 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestChatInterfaceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestChatInterfaceL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestChatInterfaceL();
 	iSender->ReleaseConnectionL();
    return err ;
    } 
  // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestLoginUserIdL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestMessageCoundL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestMessageCoundL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestChatInterfaceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestMessageUnreadCoundL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestMessageUnreadCoundL();
 	iSender->ReleaseConnectionL();
    return err ;
    }  
  
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestDeleteContextL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestDeleteContextL( CStifItemParser& /*aItem*/ )
    {
   	TInt err = iSender->DeleteContextL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestCreateContextL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestCreateContextL( CStifItemParser& /*aItem*/ )
    {
    TInt err = iSender->CreateContextL();
 	iSender->ReleaseConnectionL();
    return err ;
    }  
    
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestSubscribePresenceL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestSubscribePresenceL( CStifItemParser& /*aItem*/ )
    {
 	iSender->CreateConnectionL();
 	TInt err = iSender->TestSubscribePresenceL();
 	iSender->ReleaseConnectionL();
    return err ;
    } 
  // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestAppendInfoMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestAppendInfoMessageL( CStifItemParser& /*aItem*/ )
    {
     iSender->CreateConnectionL();
 	TInt err = iSender->TestAppendInfoMessageL();
 	iSender->ReleaseConnectionL();
    return err ;
    }
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestGetPresenceStatusL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestGetPresenceStatusL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
 	TInt err = iSender->TestGetPresenceStatusL();
 	iSender->ReleaseConnectionL();
    return err ;
    }  
    

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestCCHHandlerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestCCHHandlerL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->TestCCHHandlerL();
    iSender->ReleaseConnectionL();
    return err ;
    }    
  
  
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::ReadyForShutdownTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::ReadyForShutdownTestL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->ReadyForShutdownTestL();
    iSender->ReleaseConnectionL();
    return err;
    }
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::ServiceNameTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::ServiceNameTestL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->ServiceNameTestL();
    iSender->ReleaseConnectionL();
    return err ;
    } 

// -----------------------------------------------------------------------------
// Cimcvuiengine_test::OpenChatsTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::OpenChatsTestL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->OpenChatsTestL();
    iSender->ReleaseConnectionL();
    return err ;
    }    
 
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::GetServiceIdTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::GetServiceIdTestL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->GetServiceIdTestL();
    iSender->ReleaseConnectionL();
    return err ;
    }         
 // -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestClosAllContainerL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestClosAllContainerL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->TestClosAllContainerL();
    iSender->ReleaseConnectionL();
    return err ;
    }    
 
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestDeleteChatL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcvuiengine_test::TestDeleteChatL( CStifItemParser& /*aItem*/ )
    {
    iSender->CreateConnectionL();
    TInt err = iSender->TestDeleteChatL();
    iSender->ReleaseConnectionL();
    return err ;
    }         
    
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestEditorMessageL()
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------

TInt Cimcvuiengine_test::TestEditorMessageL(CStifItemParser& /*aItem*/)
    {
    iSender->CreateConnectionL();
    TInt err = iSender->TestEditorMessageL();
    iSender->ReleaseConnectionL();
    return err ;
    }
    
// -----------------------------------------------------------------------------
// Cimcvuiengine_test::TestConvertPresCacheEnum()
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------

TInt Cimcvuiengine_test::TestConvertPresCacheEnum(CStifItemParser& /*aItem*/)
    {
    TInt err = iSender->TestConvertPresCacheEnum();
    iSender->ReleaseConnectionL();
    return err ;
    }
//  [End of File] - Do not remove
