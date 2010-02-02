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
* Description:   tstxmppadapter.cpp
*
*/




// INCLUDE FILES
#include <StifTestInterface.h>
#include "tstxmppadapter.h"
#include <e32cons.h>

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
// CtstXmppAdapter::CtstXmppAdapter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CtstXmppAdapter::CtstXmppAdapter( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    
    }

// -----------------------------------------------------------------------------
// CtstXmppAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CtstXmppAdapter::ConstructL()
    {
    iLog = CStifLogger::NewL( KtstXmppadapterLogPath, 
                          KtstxmppadapterLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksMem );
   	TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksRequests  );
	TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles   );


    }

// -----------------------------------------------------------------------------
// CtstXmppAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CtstXmppAdapter* CtstXmppAdapter::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CtstXmppAdapter* self = new (ELeave) CtstXmppAdapter( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CtstXmppAdapter::~CtstXmppAdapter()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CtstXmppAdapter::NewL( aTestModuleIf );

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function.
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason code
    {
    return(KErrNone);

    }
#endif // EKA2

//  End of File
