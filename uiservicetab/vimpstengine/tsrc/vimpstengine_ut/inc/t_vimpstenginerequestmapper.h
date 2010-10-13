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
* Description: t_vimpstenginerequestmapper.h
*
*/
#ifndef _T_VIMPSTEngineRequestMapper_H_
#define _T_VIMPSTEngineRequestMapper_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>


#include "cvimpstenginerequestmapper.h"

//Forward declarations
class CVIMPSTEngineRequestMapper;

class T_VIMPSTEngineRequestMapper : public CEUnitTestSuiteClass
    {
    public:

        static T_VIMPSTEngineRequestMapper* NewLC();
        ~T_VIMPSTEngineRequestMapper();

    private:

        void ConstructL();
        void SetupL();        
        void Teardown();

        void TestAllFunctionsL();
		
    private:

        T_VIMPSTEngineRequestMapper() {};

    
    private:
    	CVIMPSTEngineRequestMapper* iRequestMapper;
    	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
