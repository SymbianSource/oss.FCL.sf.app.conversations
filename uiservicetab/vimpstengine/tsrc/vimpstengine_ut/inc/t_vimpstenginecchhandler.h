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
* Description: t_vimpstenginecchhandler.h
*
*/
#ifndef _T_VimpstEngineCchHandler_H_
#define _T_VimpstEngineCchHandler_H_

//  EXTERNAL INCLUDES
#include <platform/digia/eunit/ceunittestsuiteclass.h>

#include "mvimpstenginecchhandlerobserver.h"
#include "mvimpstenginecchuieventobserver.h"
//Forward declarations
class CVIMPSTEngineCchHandler;

class T_VimpstEngineCchHandler : public CEUnitTestSuiteClass,
							public MVIMPSTEngineCchHandlerObserver
    {
    public:

        static T_VimpstEngineCchHandler* NewLC();
        ~T_VimpstEngineCchHandler();

    private:

        void ConstructL();
        void SetupL();
        void SetupL1();        
        void Teardown();

        void TestConstructionL();
		void TestGetServiceStateServiceNotFoundL();
		void TestGetServiceStateServiceFoundL();
		
		
		void TestEnableServiceServiceFoundL() ;	
		void TestEnableServiceServiceNotFoundL();
		
	//	void ValidateServiceServiceNotFoundL();
	
	//	void ValidateServiceAllSubserviceAvailableL();
		
	//	void ValidateServiceAnyOneSubserviceAvailableL();
		
		void GetConParametersServiceNotFoundL();
		void GetConParametersServiceFoundL();
		void GetConParametersLeavesServiceFoundL();
		
		void SetConParametersServiceNotFoundL();
		void SetConParametersServiceFoundL();
		void SetConParametersLeavesServiceFoundL();
		
		void TestDisableServiceServiceFoundL() ;	
		void TestDisableServiceServiceNotFoundL();
		
		void ShutdownCch();
		
		void CheckObservingForPresenceSubServiceL();
		void CheckObservingWithoutRegisteringL();
		void RegisterObserverWithNull();
		void CheckNoObservingForPrsenceSubServiceL();
			
	private: //From MVIMPSTEngineCchHandlerObserver
		void CchEventOccuredL( 
            TUint aServiceId, 
            TCCHSubserviceState aState, 
            TInt aServiceError ) ;	
    private:

        T_VimpstEngineCchHandler() {};

    private:
    	CVIMPSTEngineCchHandler* iCChHandler;
    	MVIMPSTEngineCchUiEventObserver* iCchUiEventObserver;
    	
    	TBool iCallingEnable;
    	
    	TBool iGotObserverEvent;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
