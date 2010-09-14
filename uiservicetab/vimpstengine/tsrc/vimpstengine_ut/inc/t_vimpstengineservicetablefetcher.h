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
* Description: t_vimpstengineservicetablefetcher.h
*
*/
#ifndef _T_VIMPSTEngineServiceTableFetcher_H_
#define _T_VIMPSTEngineServiceTableFetcher_H_

//  EXTERNAL INCLUDES
#include <platform/digia/eunit/ceunittestsuiteclass.h>

#include <spdefinitions.h>

//Forward declarations
class CVIMPSTEngineServiceTableFetcher;
class CSPProperty;
class CSPEntry;
class CSPSettings;

class T_VIMPSTEngineServiceTableFetcher : public CEUnitTestSuiteClass
    {
    public:

        static T_VIMPSTEngineServiceTableFetcher* NewLC();
        ~T_VIMPSTEngineServiceTableFetcher();

    private:

        void ConstructL();
        void SetupL();     
        void Setup1L();   
        void Teardown();

        void TestConstructionL();
        void TestConstructionWithValidServicesL();
        void TestConstructionWithInValidServicesL();
        
        void GetMasterServiceIdsNoServicesL();
        
        void CreateSpEntryL( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TInt aValue );
                                            
		void CreateSpEntry1L( TInt aServiceId,
                                            TServicePropertyName aName,
                                            const TDesC& aValue );   
        
        void CreateSpEntry1L( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TOnOff aValue );                        
		
		void GetMasterServiceIdsServicesMatchContactViewUidValidServiceL();
		void GetMasterServiceIdsServicesMatchContactViewUidInValidServiceL();
    	void GetMasterServiceIdsServicesNoMatchContactViewUidL();
    	
    	void GetContactStoreIdPropertyNotFoundL();
    	void GetContactStoreIdPropertyFoundL();
    	void GetContactStoreIdPropertyFoundWithContactUriDomainL();
    	
    	
    	void GetBrandIdLPropertyNotFoundL();
    	void GetBrandIdLPropertyFoundL();
    	
    	void FindPropertyNotFoundL();
    	void FindPropertyFoundL();
    	
    	
		void PropertyBrandVersionNotFoundL();
		void PropertyBrandVersionFoundL();
		void PropertyBrandLanguageNotFoundL();
		void PropertyBrandLanguageFoundL();
		void GetServiceNameNotFoundL();
    	void GetServiceNameFoundL();   	
    	   
   		void XimpAdapterUidNotFoundL();
   		void XimpAdapterUidFoundL();
   		
   		void PresenceRequestStatusNotFoundL();
		void PresenceRequestStatusFoundWithEOffL();
		void PresenceRequestStatusFoundWithEOnL();    
    	
    	void ShutDownPhonebookL();
    	
    	void DoHandleNotifyChangeL_withOutNewServiceL();
    	void DoHandleNotifyChangeL_withNewServiceL();
    	
    private:

        T_VIMPSTEngineServiceTableFetcher() {};

    private:
    	CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
    	
    	// Own: service provider settings
        CSPSettings* iSpSettings;
    	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
