/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   tstxmppadapter.h
*
*/




#ifndef TSTXMPPADAPTER_H
#define TSTXMPPADAPTER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KtstXmppadapterLogPath, "\\logs\\testframework\\tstxmppadapter\\" ); 
// Log file
_LIT( KtstxmppadapterLogFile, "tstxmppadapter.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CtstXmppAdapter;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Ctstlm test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CtstXmppAdapter) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CtstXmppAdapter* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CtstXmppAdapter();

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
        CtstXmppAdapter( CTestModuleIf& aTestModuleIf );

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
        
        virtual TInt xmppadapter_newL( CStifItemParser& aItem );
        
        virtual TInt xmppadapter_visit_EWPParameterAppIDL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_kwpapplicationL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_KWPResourceL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterNonAppIDL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterAAuthNameL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterAAuthSecretL( CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterProviderIDL( CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterName_ProtocolL( CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPParameterName_ServerL( CStifItemParser& aItem );
        virtual TInt xmppadapter_visitLinkL( CStifItemParser& aItem );
        virtual TInt xmppadapter_SummaryTextL( CStifItemParser& aItem );
        virtual TInt xmppadapter_SummaryTitleL( CStifItemParser& aItem );
        virtual TInt xmppadapter_SaveL( CStifItemParser& aItem );
        virtual TInt xmppadapter_ItemCountL( CStifItemParser& aItem );
        virtual TInt xmppadapter_ItemZeroCountL( CStifItemParser& aItem );
        virtual TInt xmppadapter_DetailsL( CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_KWPAppAddrL(CStifItemParser& aItem );
        virtual TInt xmppadapter_visit_EWPNamedParameter_KSslL(CStifItemParser& aItem);
        virtual TInt xmppadapter_visit_EWPNamedParameter_KPortNbrL(CStifItemParser& aItem);
        virtual TInt xmppadapter_visit_EWPNamedParameter_KBrandIdL(CStifItemParser& aItem);
        virtual TInt xmppadapter_visit_EWPParameterDomainL(CStifItemParser& aItem );
        //virtual TInt xmppadapter_visit_withoutAppId_KWPResourceL( CStifItemParser& aItem );
        
        


  
    private:    // Data
        TUid iEcomDtorID;
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

      };

#endif      // TSTXMPPADAPTER_H

// End of File
