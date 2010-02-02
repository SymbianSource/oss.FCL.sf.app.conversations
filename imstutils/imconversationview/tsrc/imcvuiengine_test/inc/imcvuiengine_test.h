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
* Description: imcvuiengine_test.h
*
*/



#ifndef IMCVUIENGINE_TEST_H
#define IMCVUIENGINE_TEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kimcvuiengine_testLogPath, "\\logs\\testframework\\imcvuiengine_test\\" ); 
// Log file
_LIT( Kimcvuiengine_testLogFile, "imcvuiengine_test.txt" ); 
_LIT( Kimcvuiengine_testLogFileWithTitle, "imcvuiengine_test_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cimcvuiengine_test;
class CIMCVUiEngine_TestSendMessage;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cimcvuiengine_test test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cimcvuiengine_test) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cimcvuiengine_test* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cimcvuiengine_test();

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
        Cimcvuiengine_test( CTestModuleIf& aTestModuleIf );

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
        * StartNewConvesationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt StartNewConvesationL( CStifItemParser& aItem );
        
         /**
        * CloseConvesationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt CloseConvesationL( CStifItemParser& aItem );
        
        
        /**
        * SendNullMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SendNullMessageL( CStifItemParser& aItem );
        
         /**
        * SendTextMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SendTextMessageL( CStifItemParser& aItem );

        /**
        * SendSplCharMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SendSplCharMessageL( CStifItemParser& aItem );
        
         /**
        * SendCosMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SendCosMessageL( CStifItemParser& aItem );
        
        /**
        * DeactivateConvesationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt DeactivateConvesationL( CStifItemParser& aItem );
        
               
        /**
        * TestRecievedMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestMultipleSendMessageL( CStifItemParser& aItem );
        
         /**
        * TestSendRecieveMsgL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestSendRecieveMsgL( CStifItemParser& aItem );

        /**
        * TestSendFetchSendMsgL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestSendFetchSendMsgL( CStifItemParser& aItem );
        
        
          /**
        * TestFailedMsgL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestFailedMsgL( CStifItemParser& aItem );

  
          /**
        * TestConnectionTerminationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestConnectionTerminationL( CStifItemParser& aItem );
        
         /**
        * TestLoginStateL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestLoginStateL( CStifItemParser& aItem );
        
          /**
        * TestLoginUserIdL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestLoginUserIdL( CStifItemParser& aItem );
        /**
         * TestLoginUserIdL test method.
         * @since ?Series60_version
         * @param aItem Script line containing parameters.
         * @return Symbian OS error code.
         */
         virtual TInt TestGetBrandIdL( CStifItemParser& aItem );
         /**
          * TestLoginUserIdL test method.
          * @since ?Series60_version
          * @param aItem Script line containing parameters.
          * @return Symbian OS error code.
          */
          virtual TInt TestGetLanguageIdL( CStifItemParser& aItem );

           /**
        * TestChatInterfaceL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestChatInterfaceL( CStifItemParser& aItem );
        
        
           /**
        * TestMessageCoundL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestMessageCoundL( CStifItemParser& aItem );
        
           /**
        * TestMessageUnreadCoundL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestMessageUnreadCoundL( CStifItemParser& aItem );
        
       
           /**
        * TestDeleteContextL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestDeleteContextL( CStifItemParser& aItem );
        
           /**
        * TestCreateContextL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestCreateContextL( CStifItemParser& aItem );
        
         
        
        /**
        * TestSubscribePresenceL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestSubscribePresenceL( CStifItemParser& aItem );
        
        
        /**
        * TestAppendInfoMessageL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestAppendInfoMessageL( CStifItemParser& aItem );
        
        /**
        * TestGetPresenceStatusL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt TestGetPresenceStatusL( CStifItemParser& aItem );
        
        
		/**
		* TestCCHHandlerL test method.
		* @since ?Series60_version
		* @param aItem Script line containing parameters.
		* @return Symbian OS error code.
		*/
		virtual TInt TestCCHHandlerL( CStifItemParser& aItem );
		
		
		/**
		* TestCCHHandlerL test method.
		* @since ?Series60_version
		* @param aItem Script line containing parameters.
		* @return Symbian OS error code.
		*/
		virtual TInt ReadyForShutdownTestL( CStifItemParser& aItem );
		
		
		/**
		* TestCCHHandlerL test method.
		* @since ?Series60_version
		* @param aItem Script line containing parameters.
		* @return Symbian OS error code.
		*/
		virtual TInt ServiceNameTestL( CStifItemParser& aItem );
		
		
		/**
		* TestCCHHandlerL test method.
		* @since ?Series60_version
		* @param aItem Script line containing parameters.
		* @return Symbian OS error code.
		*/
		virtual TInt OpenChatsTestL( CStifItemParser& aItem );
		
		
		/**
		* TestCCHHandlerL test method.
		* @since ?Series60_version
		* @param aItem Script line containing parameters.
		* @return Symbian OS error code.
		*/
		virtual TInt GetServiceIdTestL( CStifItemParser& aItem );
	
		virtual TInt TestClosAllContainerL( CStifItemParser& aItem );
		
		virtual TInt TestDeleteChatL( CStifItemParser& aItem );
		
		virtual TInt TestEditorMessageL( CStifItemParser& aItem );
		virtual TInt TestConvertPresCacheEnum( CStifItemParser& aItem );
                          
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
		CIMCVUiEngine_TestSendMessage* iSender;
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // IMCVUIENGINE_TEST_H

// End of File
