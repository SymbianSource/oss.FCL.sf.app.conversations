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
* Description: t_vimpstenginerequest.h
*
*/
#ifndef _T_VIMPSTEngineRequest_H_
#define _T_VIMPSTEngineRequest_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>


#include "cvimpstenginerequest.h"

//Forward declarations
class CVIMPSTEngineRequest;

class T_VIMPSTEngineRequest : public CEUnitTestSuiteClass
    {
    public:

        static T_VIMPSTEngineRequest* NewLC();
        ~T_VIMPSTEngineRequest();

    private:

        void ConstructL();
        void SetupL();        
        void Teardown();

        void TestAllFunctionsL();
		
    private:

        T_VIMPSTEngineRequest() {};

    
    private:
    	CVIMPSTEngineRequest* iRequest;
    	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
