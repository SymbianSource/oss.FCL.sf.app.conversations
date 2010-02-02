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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "testinstantmsgalert.h"



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
// Ctestinstantmsgalert::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestinstantmsgalert::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Ctestinstantmsgalert::ExampleL ),
        ENTRY( "CIMAlertManager_NewL", Ctestinstantmsgalert::CIMAlertManager_NewL ),
        ENTRY( "CIMAlertManager_PlayL", Ctestinstantmsgalert::CIMAlertManager_PlayL ),
        ENTRY( "CIMAlertManager_Stop", Ctestinstantmsgalert::CIMAlertManager_Stop ),
        ENTRY( "CIMAlertManager_SetVolume", Ctestinstantmsgalert::CIMAlertManager_SetVolume ),
        ENTRY( "CIMAlertManager_SetRingingType", Ctestinstantmsgalert::CIMAlertManager_SetRingingType ),
        ENTRY( "CIMAlertManager_SetVibra", Ctestinstantmsgalert::CIMAlertManager_SetVibra ),
        ENTRY( "CIMAlertManager_DoSetRingingType", Ctestinstantmsgalert::CIMAlertManager_DoSetRingingType ),
        ENTRY( "CIMAlertManager_ConvertVolume", Ctestinstantmsgalert::CIMAlertManager_ConvertVolume ),
        ENTRY( "CIMAlertManager_MapcInitComplete", Ctestinstantmsgalert::CIMAlertManager_MapcInitComplete ), 
        ENTRY( "CIMAlertManager_MapcPlayComplete", Ctestinstantmsgalert::CIMAlertManager_MapcPlayComplete ), 
        ENTRY( "CIMAlertManager_HandleKeyChange", Ctestinstantmsgalert::CIMAlertManager_HandleKeyChange ), 
        ENTRY( "CIMAlertManager_HandleNotifyInt", Ctestinstantmsgalert::CIMAlertManager_HandleNotifyInt ), 
        ENTRY( "CIMAlertManager_MatoPrepareComplete", Ctestinstantmsgalert::CIMAlertManager_MatoPrepareComplete ), 
        ENTRY( "CIMAlertManager_MatoPlayComplete", Ctestinstantmsgalert::CIMAlertManager_MatoPlayComplete ), 
        ENTRY( "CIMAlertManager_AddObserverL", Ctestinstantmsgalert::CIMAlertManager_AddObserverL ), 
        ENTRY( "CIMAlertManager_RemoveObserver", Ctestinstantmsgalert::CIMAlertManager_RemoveObserver ), 
        ENTRY( "CIMAlertManager_HandleError", Ctestinstantmsgalert::CIMAlertManager_HandleError ), 

        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Ktestinstantmsgalert, "testinstantmsgalert" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Ktestinstantmsgalert, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Ktestinstantmsgalert, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_NewL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_NewL( CStifItemParser& aItem )
    {
    
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksRequests ); 
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );  
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    
    if(object)
        {
        delete object;
        return KErrNone;
        }
    else
        return KErrGeneral;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_PlayL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_PlayL( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->PlayL(5);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_SetVolume
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_SetVolume( CStifItemParser& aItem )
    {

    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->SetVolume(5);
    delete object;
    return KErrNone;


    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_SetRingingType
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_SetRingingType( CStifItemParser& aItem )
    {
    
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->SetRingingType(EProfileRingingTypeRingingOnce);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_SetVibra
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_SetVibra( CStifItemParser& aItem )
    {
    
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->SetVibra(ETrue);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_DoSetRingingType
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_DoSetRingingType( CStifItemParser& aItem )
    {
    
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->DoSetRingingType(EProfileRingingTypeRingingOnce);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_ConvertVolume
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_ConvertVolume( CStifItemParser& aItem )
    {
    
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->ConvertVolume(5);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_Stop
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_Stop( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->Stop();
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_MapcInitComplete
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_MapcInitComplete( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->MapcInitComplete(KErrNone,5);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_MapcPlayComplete
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_MapcPlayComplete( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->MapcPlayComplete(KErrNone);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_HandleKeyChange
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_HandleKeyChange( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    TUpdatedKey temp;
    temp.iValue =  ECoreAppUIsStopTonePlaying;
    object->HandleKeyChange(temp);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_HandleNotifyInt
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_HandleNotifyInt( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->HandleNotifyInt(KProEngActiveRingingVolume,5);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_MatoPrepareComplete
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_MatoPrepareComplete( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->MatoPrepareComplete(KErrNone);
    delete object;
    return KErrNone;

    }
// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_MatoPlayComplete
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_MatoPlayComplete( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    object->MatoPlayComplete(KErrNone);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_AddObserverL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_AddObserverL( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL(); 
    MIMAlertManagerObserver* observer = NULL;
    object->AddObserverL(observer);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_RemoveObserver
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_RemoveObserver( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    MIMAlertManagerObserver* observer = NULL;
    object->RemoveObserver(observer);
    delete object;
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Ctestinstantmsgalert::CIMAlertManager_HandleError
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Ctestinstantmsgalert::CIMAlertManager_HandleError( CStifItemParser& aItem )
    {
    CIMAlertManager* object  = CIMAlertManager::NewL();   
    MIMAlertManagerObserver* observer = NULL;
    object->HandleError(KErrNone);
    delete object;
    return KErrNone;

    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
