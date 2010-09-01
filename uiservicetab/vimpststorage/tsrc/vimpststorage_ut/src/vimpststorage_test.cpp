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
* Description:  
:                
*
*/




// INCLUDE FILES
#include <Stiftestinterface.h>
#include "vimpststorage_test.h"
#include <SettingServerClient.h>
#include <cntdb.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <cntfield.h>

// CONSTANTS



// ============================= LOCAL FUNCTIONS ===============================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cvimpststorage_test::Cvimpststorage_test
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Cvimpststorage_test::Cvimpststorage_test( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// Cvimpststorage_test::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Cvimpststorage_test::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(Kvimpststorage_testLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(Kvimpststorage_testLogFile);
        }

    iLog = CStifLogger::NewL( Kvimpststorage_testLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    
    CActiveScheduler* activeexist =CActiveScheduler::Current();
    if( !activeexist )
		{
		iActive = new (ELeave) CActiveScheduler();
		CActiveScheduler::Install(iActive);	
		}
		
    }

// -----------------------------------------------------------------------------
// Cvimpststorage_test::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Cvimpststorage_test* Cvimpststorage_test::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    Cvimpststorage_test* self = new (ELeave) Cvimpststorage_test( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
Cvimpststorage_test::~Cvimpststorage_test()
    { 
    
    
	
    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
    
    if(iActive)
	    {
	    delete iActive;	
	    }

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

    return ( CScriptBase* ) Cvimpststorage_test::NewL( aTestModuleIf );

    }


//  End of File
