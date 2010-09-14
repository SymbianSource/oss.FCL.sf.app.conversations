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




#ifndef VIMPSTSTORAGE_TEST_H
#define VIMPSTSTORAGE_TEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <cntdef.h>

#include "mvimpststoragecontactsobserver.h"
#include "MVIMPSTStorageContactManagementObserver.h"
#include "mvimpststoragecontact.h"
#include <MVPbkContactObserver.h>

//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kvimpststorage_testLogPath, "\\logs\\testframework\\vimpststorage_test\\" ); 
// Log file
_LIT( Kvimpststorage_testLogFile, "vimpststorage_test.txt" ); 
_LIT( Kvimpststorage_testLogFileWithTitle, "vimpststorage_test_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cvimpststorage_test;
class MVIMPSTStorageContacts;
class MVIMPSTStorageContactList;
class MVIMPSTStorageItemModel;
// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION
class CContactDatabase;
class CContactIdArray;
/**
*  Cvimpststorage_test test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cvimpststorage_test) : public CScriptBase,
										public MVIMPSTStorageContactsObserver,
										public MVIMPSTStorageContactManagementObserver,
										public MVPbkContactObserver,
										public MVIMPSTContactOperationCompleteObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cvimpststorage_test* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cvimpststorage_test();

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
        Cvimpststorage_test( CTestModuleIf& aTestModuleIf );

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
        
        TInt InitialiseLibraryTestL( CStifItemParser& aItem  );
             
        TInt InitialiseViewTestL( CStifItemParser& aItem  );
        
        TInt CVIMPSTStorageContactTestCase1L();
        
        TInt CVIMPSTStorageContactTestCase2L();
        
        TInt CVIMPSTStorageContactTestCase3L();
		
		TInt CVIMPSTStorageContactTestCase4L();			
		
		TInt CVIMPSTStorageContactTestCase6L();
		
		TInt CVIMPSTStorageContactTestCase7L();
		
		TInt CVIMPSTStorageContactTestCase8L();
		
		TInt CVIMPSTStorageContactTestCase9L();
		
		TInt CVIMPSTStorageContactTestCase10L();
		
		TInt CVIMPSTStorageContactTestCase11L();
		TInt CVIMPSTStorageContactTestCase12L();
		TInt CVIMPSTStorageContactTestCase13L();
		TInt CVIMPSTStorageContactTestCase14L();
		TInt CVIMPSTStorageContactTestCase15L();
		TInt CVIMPSTStorageContactListTestCase16L();
		TInt CVIMPSTStorageContactListTestCase17L();
		TInt CVIMPSTStorageContactListTestCase18L();
		TInt CVIMPSTStorageContactListTestCase19L();
		TInt CVIMPSTStorageContactListTestCase20L();
		
		TInt CVIMPSTStorageContactListTestCase21L();
		
		TInt CVIMPSTStorageXSPStoreHandlerTestCase01L();
        //[TestMethods] - Do not remove

    public: // New functions

        /**
         * Called when a contact is deleted.
         * @param aId The ID of the deleted contact
         * @param aFromPEC, ETrue, if event is coming from PEC
         */
         void HandleContactDeleteL( const TDesC& aContactId );

        /**
         * Called when a contact is added.
         * @param aList. List for contact that was added.
         * @param aContact The contact which was added
         * @param aFromPEC, ETrue, if event is coming from PEC
         */
         void HandleAdditionL( MVIMPSTStorageContactList& aList, MVIMPSTStorageContact& aContact );

        /**
         * Called when a contact or list is about to change, or has been changed.
         * @param aList List that is changed. Can be NULL if contact is changed.
         * @param aContact Contact that is changed. Can be NULL if list is changed
         * @param aEventType The type of event: EStorageEventPreChange before
         * the change (contact still has the old information), or
         * EStorageEventPostChange after the change took place 
         * (contact has new information)
         */
         void HandleChangeL( MVIMPSTStorageContactList* aList, MVIMPSTStorageContact* aContact,
                TVIMPSTEnums::TCAObserverEventType aEventType,
                TBool aUserIdChanged );

	public:
		/**
         * Called when all the contacts are fetched
         */
        void HandleContactAddedL(RPointerArray<HBufC>& aServiceDetails);  
        
        /**
         * Called when all the contacts are fetched
         */
        void HandleContactDeletedL(RPointerArray<HBufC>& aServiceDetails);  
        
        void HandleContactOperationCompleteL( MVIMPSTStorageContact& aContact, TInt aError = KErrNone );        
        				
        /**
         * From  MVPbkContactObserver
         */
        void ContactOperationCompleted(TContactOpResult aResult);

        /**
         * From MVPbkContactObserver
         */
        void ContactOperationFailed
            (TContactOp aOpCode, TInt aErrorCode, TBool aErrorNotified);


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

        CActiveScheduler*    		iActive;  
        
        TBool iObserverReceivedNotification;  
    };

#endif      // VIMPSTSTORAGE_TEST_H

// End of File
