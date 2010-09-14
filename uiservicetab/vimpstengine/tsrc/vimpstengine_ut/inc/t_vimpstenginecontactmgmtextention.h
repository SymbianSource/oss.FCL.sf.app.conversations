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
* Description: t_vimpstenginecontactmgmtextention.h
*
*/
#ifndef _T_VIMPSTEngineContactMgmtExtention_H_
#define _T_VIMPSTEngineContactMgmtExtention_H_

//  EXTERNAL INCLUDES
#include <platform/digia/eunit/ceunittestsuiteclass.h>


#include "cvimpstenginecontactmgmtextention.h"

//Forward declarations
class CVIMPSTEngineContactMgmtExtention;
class MXIMPClient;
class MXIMPContext;
class MPresenceFeatures;
class CVIMPSTEngineRequestMapper;

class T_VIMPSTEngineContactMgmtExtention : public CEUnitTestSuiteClass
    {
    public:

        static T_VIMPSTEngineContactMgmtExtention* NewLC();
        ~T_VIMPSTEngineContactMgmtExtention();

    private:

        void ConstructL();
        void SetupL();        
        void Teardown();

        void TestConstructionL();
		void TestCreateIdentityNormalL();

    	
    	void TestAddServerContactSuccessValidDataL();
		void TestAddServerContactSuccessWithDataLengthZeroL();

	    void TestAddServerContactErrorValidDataL();
    
    	void TestIsSupported();
    	void TestType();
    	
    	void TestDeleteServerContactSuccessValidDataL();
		void TestDeleteServerContactSuccessWithDataLengthZeroL();

	    void TestDeleteServerContactErrorValidDataL();
	    
	    void TestHandleSessionContextEventOperationDeleteL();
 		void TestHandleSessionContextEventOperationAddL();
 		
 		void TestHandleSessionContextEventOperationDeleteErrNoneL();
    	void TestHandleSessionContextEventOperationAddErrNoneL();
    	void TestHandleSessionContextEventOperationDeleteNotSupportedL();    
    	void TestHandleSessionContextEventOperationAddNotSupportedL();
    	void TestHandleSessionContextEventOperationNotValidOperationL();
	    
    	
    private:

        T_VIMPSTEngineContactMgmtExtention() {};

    
    private:
    	CVIMPSTEngineContactMgmtExtention* iContactManager;
    	
    	MXIMPClient* iClient;               
	    //Create new sink to receive ximp context events
		MXIMPContext* iPresenceCtx;
	    
		MPresenceFeatures* iFeatures;
		
		MPresentityGroups* iGroups;
		
		CVIMPSTEngineRequestMapper* iRequestMapper;
	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
