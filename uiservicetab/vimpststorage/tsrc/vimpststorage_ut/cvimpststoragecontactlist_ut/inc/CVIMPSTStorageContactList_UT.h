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

#ifndef CVIMPSTSTORAGECONTACTLIST_UT_H
#define CVIMPSTSTORAGECONTACTLIST_UT_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include "vimpstutils.h"
#include "cvimpststoragecontact.h"
#include "cvimpststoragecontactsorter.h"
#include "cvimpststoragecontactlist.h"
#include "tvimpstenums.h"

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KCVIMPSTStorageContactList_UTLogPath, "\\logs\\testframework\\CVIMPSTStorageContactList_UT\\" ); 
// Log file
_LIT( KCVIMPSTStorageContactList_UTLogFile, "CVIMPSTStorageContactList_UT.txt" ); 
_LIT( KCVIMPSTStorageContactList_UTLogFileWithTitle, "CVIMPSTStorageContactList_UT_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCVIMPSTStorageContactList_UT;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CCVIMPSTStorageContactList_UT test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCVIMPSTStorageContactList_UT) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCVIMPSTStorageContactList_UT* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCVIMPSTStorageContactList_UT();

    public: // New functions

        /*
         * This is a test function.
         * This tests the NewL function of the CVIMPSTStorageContactList
         * class.
         * @param: aItem,stif parser item to fetch parameters passed in the cfg file
         * @return: integer, indicates the result of the test. If KErrNone
         * is returned then it indicates that test case is passed. Any other
         * return value indicates failure of the test. 
         */
        TInt Test_NewL( CStifItemParser& aItem );
        
       /*
        * This is a test function.
        * This tests the ListId function of the CVIMPSTStorageContactList
        * class.
        * @param: aItem,stif parser item to fetch parameters passed in the cfg file
        * @return: integer, indicates the result of the test. If KErrNone
        * is returned then it indicates that test case is passed. Any other
        * return value indicates failure of the test. 
        */
        TInt Test_ListId( CStifItemParser& aItem );
        
        /*
         * This is a test function.
         * This tests the ListId function of the CVIMPSTStorageContactList
         * class.
         * @param: aItem,stif parser item to fetch parameters passed in the cfg file
         * @return: integer, indicates the result of the test. If KErrNone
         * is returned then it indicates that test case is passed. Any other
         * return value indicates failure of the test. 
         */
         TInt Test_DisplayName( CStifItemParser& aItem );
        
         /*
          * This is a test function.
          * This tests the AddContactToList function of the CVIMPSTStorageContactList
          * class.
          * @param: aItem,stif parser item to fetch parameters passed in the cfg file
          * @return: integer, indicates the result of the test. If KErrNone
          * is returned then it indicates that test case is passed. Any other
          * return value indicates failure of the test. 
          */
          TInt Test_AddContactToList( CStifItemParser& aItem );

          /*
           * This is a test function.
           * This tests the RemoveContactFromList function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_RemoveContactFromList( CStifItemParser& aItem );

          /*
           * This is a test function.
           * This tests the Count function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_Count( CStifItemParser& aItem );

          /*
           * This is a test function.
           * This tests the FilteredContact function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_FilteredContact( CStifItemParser& aItem );

          /*
           * This is a test function.
           * This tests the FilteredCount function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_FilteredCount( CStifItemParser& aItem );
          
          /*
           * This is a test function.
           * This tests the FilterAllowsContact function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_FilterAllowsContact( CStifItemParser& aItem );
          
          /*
           * This is a test function.
           * This tests the FindIndexOfContact function of the CVIMPSTStorageContactList
           * class.
           * @param: aItem,stif parser item to fetch parameters passed in the cfg file
           * @return: integer, indicates the result of the test. If KErrNone
           * is returned then it indicates that test case is passed. Any other
           * return value indicates failure of the test. 
           */
          TInt Test_FindIndexOfContact( CStifItemParser& aItem );
          
          /*
           * This is a helper function.
           * This function returns the desired filtered based on the
           * integer passed into it. 
           * @param: a integer, indicating the filter. these integers are 
           * defined in the cfg file. 
           * @return: filtertype.  
           */
          TVIMPSTEnums::TFilterType GetFilter( TInt aFilter );
          
          /*
           * This is a helper function.
           * This function returns the desired presence status
           *  based on the integer passed into it. 
           * @param: a integer, indicating the status. these integers are 
           * defined in the cfg file. 
           * @return: online status.  
           */
          TVIMPSTEnums::TOnlineStatus GetStatus( TInt aStatus );

          /*
           * This is a helper function.
           * This function returns the a contact list with 
           * predefined contacts in it. Tester can change
           * the contacts at will in this function for test 
           * purposes. 
           * The function does not take any params, because
           * to create a long list, the param list will be 
           * long. 
           * @return: pointer to the contact list.  
           */
          MVIMPSTStorageContactList* GetList( );
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
        CCVIMPSTStorageContactList_UT( CTestModuleIf& aTestModuleIf );

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

#endif      // CVIMPSTSTORAGECONTACTLIST_UT_H

// End of File
