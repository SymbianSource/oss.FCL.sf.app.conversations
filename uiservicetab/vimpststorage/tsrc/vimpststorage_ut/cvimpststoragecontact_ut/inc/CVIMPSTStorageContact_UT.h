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
* Description:  
:                
*
*/



#ifndef CVIMPSTSTORAGECONTACT_UT_H
#define CVIMPSTSTORAGECONTACT_UT_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KCVIMPSTStorageContact_UTLogPath, "//logs//testframework//CVIMPSTStorageContact_UT//" ); 
// Log file
_LIT( KCVIMPSTStorageContact_UTLogFile, "CVIMPSTStorageContact_UT.txt" ); 
_LIT( KCVIMPSTStorageContact_UTLogFileWithTitle, "CVIMPSTStorageContact_UT_[%S].txt" );

class CCVIMPSTStorageContact_UT;


/**
*  CCVIMPSTStorageContact_UT test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCVIMPSTStorageContact_UT) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCVIMPSTStorageContact_UT* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCVIMPSTStorageContact_UT();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
        /*
         * Test case to test NewL function
         */
        TInt Test_NewL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting userid for a contact.
         */
        TInt Test_SetUserId_UserIdL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting avatar content.
         */
        TInt Test_AvatarContentL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting displayName for a contact.
         */
        TInt Test_DisplayNameL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting the presence status for a contact.
         */
        TInt Test_PresenceStatusL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting the status text for a contact.
         */
        TInt Test_StatusTextL( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * It tests getting and setting the avatar index for a contact.
         */
        TInt Test_AvatarIndexL( CStifItemParser& aItem );
        
        /*
         * This is test function.
         * It tests the getting ContactLink functionality for a contact.
         */
        TInt Test_ContactLinkL( CStifItemParser& aItem );
        

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

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */

    private:

        /**
        * C++ default constructor.
        */
        CCVIMPSTStorageContact_UT( CTestModuleIf& aTestModuleIf );

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

#endif      // CVIMPSTSTORAGECONTACT_UT_H

// End of File
