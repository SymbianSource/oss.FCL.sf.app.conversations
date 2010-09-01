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
* Description: STIF testclass declaration
*
*/

#ifndef TESTINSTANTMSGALERT_H
#define TESTINSTANTMSGALERT_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include "mimalertmanagerobserver.h"
////* Source code include files
#include "cimalertmanager.h"
#include <coreapplicationuisdomainpskeys.h>
#include    <ProfileEngineSDKCRKeys.h>



//*/

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KtestinstantmsgalertLogPath, "//logs//testframework//testinstantmsgalert//" ); 
// Log file
_LIT( KtestinstantmsgalertLogFile, "testinstantmsgalert.txt" ); 
_LIT( KtestinstantmsgalertLogFileWithTitle, "testinstantmsgalert_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Ctestinstantmsgalert;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ctestinstantmsgalert test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Ctestinstantmsgalert) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestinstantmsgalert* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestinstantmsgalert();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        Ctestinstantmsgalert( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_NewL( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_PlayL( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_SetVolume( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_SetRingingType( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_SetVibra( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_DoSetRingingType( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_ConvertVolume( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_Stop( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_MapcInitComplete( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_MapcPlayComplete( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_HandleKeyChange( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_HandleNotifyInt( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_MatoPrepareComplete( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_MatoPlayComplete( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_AddObserverL( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_RemoveObserver( CStifItemParser& aItem );
        virtual TInt CIMAlertManager_HandleError( CStifItemParser& aItem );  
 
        

        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // TESTINSTANTMSGALERT_H

// End of File
