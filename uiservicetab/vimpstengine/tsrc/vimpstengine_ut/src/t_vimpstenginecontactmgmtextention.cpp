/*
* t_vimpstenginecontactmgmtextention.cpp  :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: t_vimpstenginecontactmgmtextention.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginecontactmgmtextention.h"
#include "cvimpstenginecontactmgmtextention.h"

#include "s_ximpclient.h"
#include "s_ximpcontext.h"
#include "s_presencefeatures.h"
#include "s_presentitygroups.h"
//#include <ximpidentity.h>

#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"

#include "s_ximprequestcompleteevent.h"

_LIT(KTestId, "TestId");
_LIT(KTestIdentity, "TEST_IDENTITY");


extern TXIMPRequestId gTXIMPRequestId;
extern TBool gCreateXIMPIdentity;

extern TInt32 gMXIMPRequestCompleteEventStubInterfaceId;
extern TXIMPRequestId gMXIMPRequestCompleteEventStubRequestId;
extern TBool gRequestCompleteEventResultCodeKerrNotSupported;

T_VIMPSTEngineContactMgmtExtention* T_VIMPSTEngineContactMgmtExtention::NewLC()
    {
    T_VIMPSTEngineContactMgmtExtention* self = new( ELeave ) T_VIMPSTEngineContactMgmtExtention;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VIMPSTEngineContactMgmtExtention::~T_VIMPSTEngineContactMgmtExtention()
    {
    }

void T_VIMPSTEngineContactMgmtExtention::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

        
void T_VIMPSTEngineContactMgmtExtention::SetupL()
    {   
    iClient = MXIMPClientStub::NewClientL();               
    
    //Create new sink to receive ximp context events
	iPresenceCtx = iClient->NewPresenceContextLC();    
    // ignore code scanner warning, it gives panic
    CleanupStack::Pop();// because of LC method    
	
	iFeatures = MyMPresenceFeatureStub::NewStubL(iPresenceCtx);	
	//
	iGroups = &(iFeatures->PresentityGroups());
	
	iRequestMapper = CVIMPSTEngineRequestMapper::NewL() ;
	
	iContactManager = CVIMPSTEngineContactMgmtExtention::NewL( *iPresenceCtx,
														*iGroups,
														*iRequestMapper );
	
    }    

void T_VIMPSTEngineContactMgmtExtention::Teardown()
    {
    
    delete iContactManager;
    delete iRequestMapper;    
    delete iFeatures;
	delete iPresenceCtx;
	delete iClient;
	
	//gCreateXIMPIdentity = EFalse;
	//gRequestCompleteEventResultCodeKerrNotSupported = EFalse;
    }


void T_VIMPSTEngineContactMgmtExtention::TestConstructionL()
    {
    EUNIT_ASSERT( iContactManager );
    }

void T_VIMPSTEngineContactMgmtExtention::TestCreateIdentityNormalL()
	{
	gCreateXIMPIdentity = ETrue;
	MXIMPIdentity* identity = iContactManager->CreateIdentityLC(KTestId);
	EUNIT_ASSERT( identity );
	CleanupStack::PopAndDestroy();
	}

void T_VIMPSTEngineContactMgmtExtention::TestAddServerContactSuccessValidDataL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNone;
	iContactManager->iSupported = TVIMPSTEnums::ESupported;
	
	TInt error = iContactManager->AddServerContactL(KTestId(), KTestId(), KTestId());
	
	EUNIT_ASSERT(TVIMPSTEnums::ESupported == iContactManager->IsSupported())
	EUNIT_ASSERT( KErrNone == error );	
	}
	
void T_VIMPSTEngineContactMgmtExtention::TestAddServerContactSuccessWithDataLengthZeroL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNone;
	iContactManager->iSupported = TVIMPSTEnums::ESupported;
	TInt error =  iContactManager->AddServerContactL(KNullDesC(), KNullDesC(), KNullDesC());
	EUNIT_ASSERT(TVIMPSTEnums::ESupported == iContactManager->IsSupported())	
	EUNIT_ASSERT( KErrNone == error );	
	}
	


void T_VIMPSTEngineContactMgmtExtention::TestAddServerContactErrorValidDataL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNotSupported;
	iContactManager->iSupported = TVIMPSTEnums::ENotSupported;
	TInt error = iContactManager->AddServerContactL(KTestId(), KTestId(), KTestId());
	
	EUNIT_ASSERT(TVIMPSTEnums::ENotSupported == iContactManager->IsSupported())
	EUNIT_ASSERT( KErrNone == error );	
	}		


void T_VIMPSTEngineContactMgmtExtention::TestType()
	{	
	EUNIT_ASSERT(TVIMPSTEnums::EContactManagement == iContactManager->Type())	
	}
	

void T_VIMPSTEngineContactMgmtExtention::TestIsSupported()
	{	
	EUNIT_ASSERT(TVIMPSTEnums::ESupportUnKnown == iContactManager->IsSupported())	
	}
		

void T_VIMPSTEngineContactMgmtExtention::TestDeleteServerContactSuccessValidDataL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNone;
	iContactManager->iSupported = TVIMPSTEnums::ESupported;
	
	TInt error = iContactManager->DeleteServerContactL(KTestId(), KTestId());
	
	EUNIT_ASSERT(TVIMPSTEnums::ESupported == iContactManager->IsSupported())
	EUNIT_ASSERT( KErrNone == error );	
	}

void T_VIMPSTEngineContactMgmtExtention::TestDeleteServerContactSuccessWithDataLengthZeroL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNone;
	iContactManager->iSupported = TVIMPSTEnums::ESupported;
	
	TInt error = iContactManager->DeleteServerContactL(KNullDesC(), KNullDesC());
	
	EUNIT_ASSERT(TVIMPSTEnums::ESupported == iContactManager->IsSupported())	
	EUNIT_ASSERT( KErrNone == error );	
	}


	


void T_VIMPSTEngineContactMgmtExtention::TestDeleteServerContactErrorValidDataL()
	{
	gCreateXIMPIdentity = ETrue;
	iContactManager->iReqResult = KErrNotSupported;
	iContactManager->iSupported = TVIMPSTEnums::ENotSupported;
	TInt error = iContactManager->DeleteServerContactL(KTestId(), KTestId());
	
	EUNIT_ASSERT(TVIMPSTEnums::ENotSupported == iContactManager->IsSupported())
	EUNIT_ASSERT( KErrNone == error );	
	}
	


void T_VIMPSTEngineContactMgmtExtention::TestHandleSessionContextEventOperationDeleteErrNoneL()
    {
    TXIMPRequestId id;
    id.Init( 10 );

	iRequestMapper->CreateRequestL(id, ETrue, EVIMPSTXimpOperationDeleteContact);// waite here
	
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;

    ::gMXIMPRequestCompleteEventStubRequestId = id;
    
    iContactManager->HandleSessionContextEventL
    				(*iPresenceCtx, requestCompleteEvent, EVIMPSTXimpOperationDeleteContact);
    
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(id);     
    EUNIT_ASSERT( NULL == req1 );
    

    EUNIT_ASSERT(KErrNone == iContactManager->iReqResult)
                                             
    }	


void T_VIMPSTEngineContactMgmtExtention::TestHandleSessionContextEventOperationAddErrNoneL()
    {
    TXIMPRequestId id;
    id.Init( 20 );

    gRequestCompleteEventResultCodeKerrNotSupported = EFalse;
	iRequestMapper->CreateRequestL(id, ETrue, EVIMPSTXimpOperationAddContact);// waite here
	
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;

    ::gMXIMPRequestCompleteEventStubRequestId = id;
    
    iContactManager->HandleSessionContextEventL
    				(*iPresenceCtx, requestCompleteEvent, EVIMPSTXimpOperationAddContact);
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(id);     
    EUNIT_ASSERT( NULL == req1 );
    
    TInt res = iContactManager->iReqResult;
    EUNIT_ASSERT(KErrNone == iContactManager->iReqResult)
                                             
    }	
 
 
 void T_VIMPSTEngineContactMgmtExtention::TestHandleSessionContextEventOperationDeleteNotSupportedL()
    {
    TXIMPRequestId id;
    id.Init( 10 );

	gRequestCompleteEventResultCodeKerrNotSupported = ETrue;
	
	iRequestMapper->CreateRequestL(id, ETrue, EVIMPSTXimpOperationDeleteContact);// waite here
	
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;

    ::gMXIMPRequestCompleteEventStubRequestId = id;
    
    iContactManager->HandleSessionContextEventL
    				(*iPresenceCtx, requestCompleteEvent, EVIMPSTXimpOperationDeleteContact);
    
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(id);     
    EUNIT_ASSERT( NULL == req1 );
    
    EUNIT_ASSERT(KErrNotSupported == iContactManager->iReqResult)
                                             
    }	


void T_VIMPSTEngineContactMgmtExtention::TestHandleSessionContextEventOperationAddNotSupportedL()
    {
    TXIMPRequestId id;
    id.Init( 20 );

	gRequestCompleteEventResultCodeKerrNotSupported = ETrue;
	
	iRequestMapper->CreateRequestL(id, ETrue, EVIMPSTXimpOperationAddContact);// waite here
	
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;

    ::gMXIMPRequestCompleteEventStubRequestId = id;
    
    iContactManager->HandleSessionContextEventL
    				(*iPresenceCtx, requestCompleteEvent, EVIMPSTXimpOperationAddContact);
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(id);     
    EUNIT_ASSERT( NULL == req1 );
    
    EUNIT_ASSERT(KErrNotSupported == iContactManager->iReqResult)
                                             
    }	
 

void T_VIMPSTEngineContactMgmtExtention::TestHandleSessionContextEventOperationNotValidOperationL()
    {
    TXIMPRequestId id;
    id.Init( 20 );

	iRequestMapper->CreateRequestL(id, ETrue, EVIMPSTXimpOperationAddContact);// waite here
	
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;

    ::gMXIMPRequestCompleteEventStubRequestId = id;
    
    iContactManager->iReqResult = KErrNone;
    
    iContactManager->HandleSessionContextEventL
    				(*iPresenceCtx, requestCompleteEvent, EVIMPSTXimpOperationNoOperation);
    
    CVIMPSTEngineRequest* req1 = iRequestMapper->FindRequestId(id);     
    EUNIT_ASSERT( req1 );
    
    EUNIT_ASSERT(KErrNone == iContactManager->iReqResult)
                                             
    }
  
    
	
EUNIT_BEGIN_TEST_TABLE( T_VIMPSTEngineContactMgmtExtention, "T_VIMPSTEngineContactMgmtExtention", "" )

    EUNIT_TEST( "TestConstructionL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "ConstructionL",
            "FUNCTIONALITY", SetupL, TestConstructionL, Teardown )
    EUNIT_TEST( "TestCreateIdentityNormalL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "CreateIdentityLC",
            "FUNCTIONALITY", SetupL, TestCreateIdentityNormalL, Teardown )
    
    EUNIT_TEST( "TestAddServerContactSuccessValidDataL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "AddServerContactL",
            "FUNCTIONALITY", SetupL, TestAddServerContactSuccessValidDataL, Teardown )
    EUNIT_TEST( "TestAddServerContactSuccessWithDataLengthZeroL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "AddServerContactL",
            "FUNCTIONALITY", SetupL, TestAddServerContactSuccessWithDataLengthZeroL, Teardown )
    EUNIT_TEST( "TestAddServerContactErrorValidDataL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "AddServerContactL",
            "FUNCTIONALITY", SetupL, TestAddServerContactErrorValidDataL, Teardown )	 
    EUNIT_TEST( "TestIsSupported ",
            "T_VIMPSTEngineContactMgmtExtention",
            "IsSupported",
            "FUNCTIONALITY", SetupL, TestIsSupported, Teardown )	  
    EUNIT_TEST( "TestType ",
            "T_VIMPSTEngineContactMgmtExtention",
            "Type",
            "FUNCTIONALITY", SetupL, TestType, Teardown )
    EUNIT_TEST( "TestDeleteServerContactSuccessValidDataL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "DeleteServerContactL",
            "FUNCTIONALITY", SetupL, TestDeleteServerContactSuccessValidDataL, Teardown )
    EUNIT_TEST( "TestDeleteServerContactSuccessWithDataLengthZeroL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "DeleteServerContactL",
            "FUNCTIONALITY", SetupL, TestDeleteServerContactSuccessWithDataLengthZeroL, Teardown )
    EUNIT_TEST( "TestDeleteServerContactErrorValidDataL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "DeleteServerContactL",
            "FUNCTIONALITY", SetupL, TestDeleteServerContactErrorValidDataL, Teardown )
    
    EUNIT_TEST( "TestHandleSessionContextEventOperationDeleteErrNoneL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "HandleSessionContextEventL",
            "FUNCTIONALITY", SetupL, TestHandleSessionContextEventOperationDeleteErrNoneL, Teardown )
    EUNIT_TEST( "TestHandleSessionContextEventOperationDeleteNotSupportedL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "HandleSessionContextEventL",
            "FUNCTIONALITY", SetupL, TestHandleSessionContextEventOperationDeleteNotSupportedL, Teardown )
    EUNIT_TEST("TestHandleSessionContextEventOperationAddErrNoneL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "HandleSessionContextEventL",
            "FUNCTIONALITY", SetupL, TestHandleSessionContextEventOperationAddErrNoneL, Teardown )
    EUNIT_TEST( "TestHandleSessionContextEventOperationAddNotSupportedL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "HandleSessionContextEventL",
            "FUNCTIONALITY",SetupL, TestHandleSessionContextEventOperationAddNotSupportedL, Teardown )	  
    EUNIT_TEST( "TestHandleSessionContextEventOperationNotValidOperationL ",
            "T_VIMPSTEngineContactMgmtExtention",
            "HandleSessionContextEventL",
            "FUNCTIONALITY", SetupL, TestHandleSessionContextEventOperationNotValidOperationL, Teardown )
    
    
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
