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
* Description:  imconnectionprovider_test.h
*
*/



#ifndef IMCONNECTIONPROVIDER_TEST_H
#define IMCONNECTIONPROVIDER_TEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <cchservice.h>
#include <cchtypes.h>
#include <e32property.h>
#include <spentry.h>
#include <spproperty.h>
#include <spsettings.h>
#include <spdefinitions.h>
#include <presenceauthorization.h>

#include "s_ximpobjectfactory.h"
#include "s_ximpcontext.h"
#include "s_xmppsettingsapi.h"
#include "s_ximpclient.h"

#include "s_ximprequestcompleteevent.h"
#include "cicpservicemanager.h"

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( Kimconnectionprovider_testLogPath, "\\logs\\testframework\\imconnectionprovider_test\\" ); 
// Log file
_LIT( Kimconnectionprovider_testLogFile, "imconnectionprovider_test.txt" ); 
_LIT( Kimconnectionprovider_testLogFileWithTitle, "imconnectionprovider_test_[%S].txt" );

_LIT( KServiceName, "TestService");

_LIT( KServicePropName, "DummyService");

_LIT( KServreName, "test@testserver.com");

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;


class Cimconnectionprovider_test;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cimconnectionprovider_test test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cimconnectionprovider_test) : public CScriptBase,
												public MCchServiceObserver
    {
    public:  // Constructors and destructor

		 friend class CIcpServiceManager;
        /**
        * Two-phased constructor.
        */
        static Cimconnectionprovider_test* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cimconnectionprovider_test();
        
        CIcpServiceManager* NewL( MCchServiceObserver* aServiceObserver = NULL);

    private:
    	void ServiceStateChanged(
                    const TServiceSelection aServiceSelection,
                    TCCHSubserviceState aState, 
                    TInt aError );
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
        Cimconnectionprovider_test( CTestModuleIf& aTestModuleIf );

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
        virtual TInt T_EnableServiceL( CStifItemParser& aItem );
        
        virtual TInt T_DisableServiceL( CStifItemParser& aItem );
    
        virtual TInt T_SetIMDisabledL( CStifItemParser& aItem );
  
        virtual TInt T_RemoveRequestId( CStifItemParser& aItem );
   
        virtual TInt T_BindL( CStifItemParser& aItem );
        
        virtual TInt T_UnBindL( CStifItemParser& aItem );
        
        virtual TInt T_IsReserved( CStifItemParser& aItem );
        
        virtual TInt T_SetConnectionParameter( CStifItemParser& aItem );
        
        virtual TInt T_GetConnectionParameter( CStifItemParser& aItem );
  
        virtual TInt T_ValidateServiceL( CStifItemParser& aItem );
        
        virtual TInt T_GetSPSettingsIntPropertyL( CStifItemParser& aItem );
        
        virtual TInt T_ValidateSubService( CStifItemParser& aItem );
        
        virtual TInt T_GetServiceNameL( CStifItemParser& aItem );
        
        virtual TInt T_ConvertXIMPErrToCChErr( CStifItemParser& aItem );
        
        virtual TInt T_FindRequestId( CStifItemParser& aItem );
        
        virtual TInt T_GetServiceNetworkInfo_emptypasswordL( CStifItemParser& aItem );
        
        virtual TInt T_GetServiceNetworkInfo_passwordExistL( CStifItemParser& aItem );
        
       // virtual TInt T_UnsubscribePresenceGrantRequestListL( CStifItemParser& aItem );
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();
        
        void Cimconnectionprovider_test::CreateSpEntryL( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TInt aValue );

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        CSPSettings* iSpSettings;
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

#endif      // IMCONNECTIONPROVIDER_TEST_H

// End of File
