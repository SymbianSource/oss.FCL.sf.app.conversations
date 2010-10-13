/*
* t_vimpstenginerequest.cpp :: Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstenginerequest.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginerequest.h"
#include "cvimpstenginerequest.h"


TXIMPRequestId gTXIMPRequestId;

T_VIMPSTEngineRequest* T_VIMPSTEngineRequest::NewLC()
    {
    T_VIMPSTEngineRequest* self = new( ELeave ) T_VIMPSTEngineRequest;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VIMPSTEngineRequest::~T_VIMPSTEngineRequest()
    {
    }

void T_VIMPSTEngineRequest::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

        
void T_VIMPSTEngineRequest::SetupL()
    {
    gTXIMPRequestId.Init(100);   
    iRequest = CVIMPSTEngineRequest::NewL(gTXIMPRequestId,
        					EVIMPSTXimpOperationBind) ;
    }    

void T_VIMPSTEngineRequest::Teardown()
    {
    delete iRequest;
    }


void T_VIMPSTEngineRequest::TestAllFunctionsL()
    {
    
    //Do all the functions here..   
    EUNIT_ASSERT( EVIMPSTXimpOperationBind == iRequest->RequestType() );
    
    EUNIT_ASSERT( gTXIMPRequestId == iRequest->GetRequestId() );
    
    
    iRequest->SetRequestType(EVIMPSTXimpOperationUnBind);
    EUNIT_ASSERT( EVIMPSTXimpOperationUnBind == iRequest->RequestType() );
    
       
    iRequest->StopWait();
    TBool temp = iRequest->IsWaitStarted();
    EUNIT_ASSERT( EFalse == temp );
    iRequest->StopWait();
    
    iRequest->StartWait();
     temp = iRequest->IsWaitStarted();
    EUNIT_ASSERT( EFalse == temp );

      
    
    }


EUNIT_BEGIN_TEST_TABLE( T_VIMPSTEngineRequest, "T_VIMPSTEngineRequest", "" )

    EUNIT_TEST( "", "", "", "", SetupL, TestAllFunctionsL, Teardown )
    
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
