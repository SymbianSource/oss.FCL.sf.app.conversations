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



#ifndef CVIMPSTSTORAGECONTACTSORTER_UT_H
#define CVIMPSTSTORAGECONTACTSORTER_UT_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <flogger.h>
#include "vimpststorageutils.h"
#include "cvimpststoragecontactsorter.h"

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KCVIMPSTStorageContactSorter_UTLogPath, "//logs//testframework//CVIMPSTStorageContactSorter_UT//" ); 
// Log file
_LIT( KCVIMPSTStorageContactSorter_UTLogFile, "CVIMPSTStorageContactSorter_UT.txt" ); 
_LIT( KCVIMPSTStorageContactSorter_UTLogFileWithTitle, "CVIMPSTStorageContactSorter_UT_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCVIMPSTStorageContactSorter_UT;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CCVIMPSTStorageContactSorter_UT test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCVIMPSTStorageContactSorter_UT) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCVIMPSTStorageContactSorter_UT* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCVIMPSTStorageContactSorter_UT();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
        /*
         * Test Function: Test construction of sorter object.
         */
        TInt Test_NewL( CStifItemParser& aItem );
        
        /*
         * Test Function: Tests sorting functionality. 
         */
        TInt Test_SortL( CStifItemParser& aItem );
        
        /*
         * Test Function: Tests Comparing functionality.
         */
        TInt Test_CompareL( CStifItemParser& aItem );
        
        /*
         *  Test Function: Tests setting algorithm.
         */
        TInt SetSortAlgorithm( CStifItemParser& aItem );
        

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
        CCVIMPSTStorageContactSorter_UT( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

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
        
        /*
         * Helper Function: Creates an array of contacts.
         */
        void CreateContactArray();
        
        /*
         * Helper Function: Creates an array of contacts.
         * In alphabetical order.
         * The array created by this function is used to
         * cross check the array given as a result by sort function.
         */
        void CreateAlphabeticalSortArray();
        
        /*
         * Helper Function: Creates an array of contacts.
         * In order of presence.
         * The array created by this function is used to cross 
         * check the array given as a result by sort function.
         */
        void CreatePresenceSortArray();

        /*
         * Helper Function: Compares two arrays.
         * It compares the userids of the corresponding elements
         * of the arrays and returns 
         * : zero, if arrays are equal.
         * : index of difference, if difference is found.
         */
        TInt CompareArray(RPointerArray< MVIMPSTStorageContact > & aFirstArray, RPointerArray< MVIMPSTStorageContact >& aSecondArray );
        
        /*
         * Helper Function: Resets the arrays after use.
         */
        void ResetArray( RPointerArray< MVIMPSTStorageContact > & aArray );

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        RPointerArray< MVIMPSTStorageContact > iContactArray;
        
        RPointerArray< MVIMPSTStorageContact > iAlphabeticalSortArray;
        
        RPointerArray< MVIMPSTStorageContact > iPresenceSortArray;
        
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // CVIMPSTSTORAGECONTACTSORTER_UT_H

// End of File
