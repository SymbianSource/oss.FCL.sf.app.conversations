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
* Description: t_vimpstenginesessioncntxtobserver.h
*
*/
#ifndef _T_VimpstEngineSessionCntxtObserver_H_
#define _T_VimpstEngineSessionCntxtObserver_H_

//  EXTERNAL INCLUDES
#include <platform/digia/eunit/ceunittestsuiteclass.h>


//Forward declarations
class CVIMPSTEngineSessionCntxtObserver;

class T_VimpstEngineSessionCntxtObserver : public CEUnitTestSuiteClass
    {
    public:

        static T_VimpstEngineSessionCntxtObserver* NewLC();
        ~T_VimpstEngineSessionCntxtObserver();

    private:

        void Setup1L();
    	void ConstructL();
		void Setup2L();
		void Teardown();

		TInt CreateSpEntryL( TInt aServiceId );
		void T_NewLL();
		void T_HandlePresenceContextEventL();
		    

		void T_GetCompletedReqResultL();
		    

		void T_GetRequestMapperL();
		    

		void T_ServerBindL();
		    

		void T_ServerUnBindL();
		    

		void T_UriFromXimpOperationLCL();
		    
		void T_DoHandlePresenceContextEventLL();
		    
		void T_XimpAuthorizationLL();
		    
		void T_XimpPresenceWatchingLL();
		    
		void T_XimpPresentityGroupsLL();
		    
		void T_XimpPresencePublishingLL();
		    
		void T_XimpPresenceContextL();
		    
		void T_PresenceObjectFactoryL();
		    

		void T_RegisterObserverL();
		    

		void T_UnRegisterObserverL();
		void T_IdentifySupportedFeaturesL();
		void T_GetSupportedFeaturesL();
		
    

	    
    	
    private:

        T_VimpstEngineSessionCntxtObserver() {};

    
    private:
  
        CVIMPSTEngineSessionCntxtObserver* iSessionCntxtObserver;
        
        TInt iTestServiceId;
    
	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
