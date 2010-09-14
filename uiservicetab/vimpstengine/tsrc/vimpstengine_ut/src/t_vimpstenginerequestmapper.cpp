/*
* t_vimpstenginerequestmapper.cpp :: Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: t_vimpstenginerequestmapper.cpp
*
*/
#include <platform/digia/eunit/eunitmacros.h>
#include <platform/digia/eunit/teunitassertioninfo.h>
#include <platform/digia/eunit/CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginerequestmapper.h"
#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"

extern TXIMPRequestId gTXIMPRequestId;

T_VIMPSTEngineRequestMapper* T_VIMPSTEngineRequestMapper::NewLC()
    {
    T_VIMPSTEngineRequestMapper* self = new( ELeave ) T_VIMPSTEngineRequestMapper;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VIMPSTEngineRequestMapper::~T_VIMPSTEngineRequestMapper()
    {
    }

void T_VIMPSTEngineRequestMapper::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

        
void T_VIMPSTEngineRequestMapper::SetupL()
    {   
    iRequestMapper = CVIMPSTEngineRequestMapper::NewL() ;
    }    

void T_VIMPSTEngineRequestMapper::Teardown()
    {
    delete iRequestMapper;
    }


void T_VIMPSTEngineRequestMapper::TestAllFunctionsL()
    {
    
    //Do all the functions here.. 
    gTXIMPRequestId.Init(1000);  
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(gTXIMPRequestId);    
    EUNIT_ASSERT(NULL == req1);
    req1 = NULL;
    
    // remove the request mapper
    iRequestMapper->RemoveRequestId(gTXIMPRequestId);
   
    
    CVIMPSTEngineRequest* req3 = iRequestMapper->CreateRequestL(gTXIMPRequestId, 
										ETrue,
										EVIMPSTXimpOperationBind) ;
	EUNIT_ASSERT(req3);									
	EUNIT_ASSERT(EVIMPSTXimpOperationBind == req3->RequestType());
	req3 = NULL;
											
	iRequestMapper->RemoveRequestId(gTXIMPRequestId) ;	
	
	gTXIMPRequestId.Init(2000);  
	CVIMPSTEngineRequest* req5 = iRequestMapper->CreateRequestL(gTXIMPRequestId, 
										EFalse,
										EVIMPSTXimpOperationUnBind) ;
	EUNIT_ASSERT(req5);									
	EUNIT_ASSERT(EVIMPSTXimpOperationUnBind == req5->RequestType());
	req5 = NULL;
	
	CVIMPSTEngineRequest* req7 = iRequestMapper->FindRequestId(gTXIMPRequestId);    
    EUNIT_ASSERT(req7);
    EUNIT_ASSERT(EVIMPSTXimpOperationUnBind == req7->RequestType());
    req7 = NULL;
    										
	iRequestMapper->RemoveRequestId(gTXIMPRequestId) ;	
    }


EUNIT_BEGIN_TEST_TABLE( T_VIMPSTEngineRequestMapper, "T_VIMPSTEngineRequestMapper", "" )

    EUNIT_TEST( "TestAllFunctionsL ",
            "T_VIMPSTEngineRequestMapper",
            "TestAllFunctionsL",
            "FUNCTIONALITY", SetupL, TestAllFunctionsL, Teardown )
    
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
