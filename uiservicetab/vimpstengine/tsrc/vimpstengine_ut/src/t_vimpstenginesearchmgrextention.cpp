/*
* t_vimpstenginesearchmgrextention.cpp :: Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstenginesearchmgrextention.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>

#include "T_vimpstenginesearchmgrextention.h"

#include "s_presencefeatures.h"
#include "s_presentitygroups.h"
#include "s_ximpclient.h"
#include "s_ximpcontext.h"
#include "s_ximprequestcompleteevent.h"
#include "s_ximpobjectfactory.h"

#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginesearchmgrextention.h"
#include "cvimpstenginerequest.h"


/*
 * To be tested: 
 * - SearchContactsL
 * - SubscribeForSearchKeysL
 * - IsSupported
 * - Type
 * - HandleSessionContextEventL
  */


extern TXIMPRequestId gTXIMPRequestId;

T_VIMPSTEngineSearchMgrExtention* T_VIMPSTEngineSearchMgrExtention::NewLC()
    {
    T_VIMPSTEngineSearchMgrExtention* self = new( ELeave ) T_VIMPSTEngineSearchMgrExtention;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VIMPSTEngineSearchMgrExtention::~T_VIMPSTEngineSearchMgrExtention()
    {
    }

void T_VIMPSTEngineSearchMgrExtention::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

        
void T_VIMPSTEngineSearchMgrExtention::SetupL()
    {
    iClient = MXIMPClientStub::NewClientL();               
        
    //Create new sink to receive ximp context events
    iPresenceCtx = iClient->NewPresenceContextLC();    
    // ignore code scanner warning, it gives panic
    CleanupStack::Pop();// because of LC method    
        
    iFeatures = MyMPresenceFeatureStub::NewStubL(iPresenceCtx); 
        
    iRequestMapper = CVIMPSTEngineRequestMapper::NewL() ;
          
    iSearcMgrExtention = CVIMPSTEngineSearchMgrExtention::NewL( *iPresenceCtx,
            *iRequestMapper );
    }    

void T_VIMPSTEngineSearchMgrExtention::Teardown()
    {
    delete iSearcMgrExtention;
    delete iRequestMapper;    
    delete iFeatures;
    delete iPresenceCtx;
    delete iClient;
    }

// From MVIMPSTEngineSearchExtentionEventObserver
void HandleSearchResultL(RArray<TVIMPSTSearchData> aSerachData)
    {
    
    }
void HandleSearchFinishedL(TInt /*aError*/)
    {
    
    }
void HandleSearchKeysEventL( RArray<TInt>& /*aEnumKeysArray*/, RPointerArray<HBufC>& /*aLebelKeysArray*/ )
    {
    
    }

// T_VIMPSTEngineSearchMgrExtention::TestSearchContactsL
// TODO: All test cases...
void T_VIMPSTEngineSearchMgrExtention::TestSearchContactsL()
    {
    TInt error = KErrNone;
    iSearcMgrExtention->RegisterObserver( this );
   
    
    RArray<TVIMPSTSearchKeyData> aKeyDataArray;
    TVIMPSTSearchKeyData data =
                        {
                        TVIMPSTEnums::EVIMPSTFirstName,
                        KNullDesC,
                        KNullDesC  
                        };
    aKeyDataArray.Append(data);
    
    error = iSearcMgrExtention->SearchContactsL( aKeyDataArray );
    EUNIT_ASSERT( error == KErrNone );
    
    iSearcMgrExtention->UnRegisterObserver( this );
    }

// T_VIMPSTEngineSearchMgrExtention::TestSubscribeForSearchKeysL
// TODO: All test cases...
void T_VIMPSTEngineSearchMgrExtention::TestSubscribeForSearchKeysL()
    {
    //MXIMPBase* event = NULL;
    }

// T_VIMPSTEngineSearchMgrExtention::TestIsSupported
// TODO: All test cases...
void T_VIMPSTEngineSearchMgrExtention::TestIsSupported()
    {
    
    }

// T_VIMPSTEngineSearchMgrExtention::TestType
// TODO: All test cases...
void T_VIMPSTEngineSearchMgrExtention::TestType()
    {
    
    }

// T_VIMPSTEngineSearchMgrExtention::TestHandleSessionContextEventL
// TODO: All test cases...
void T_VIMPSTEngineSearchMgrExtention::TestHandleSessionContextEventL()
    {
    
    }

// T_VIMPSTEngineSearchMgrExtention::TestAllFunctionsL
// TODO: Remove this functio...
void T_VIMPSTEngineSearchMgrExtention::TestAllFunctionsL()
    {
    
    //Do all the functions here..   
    /*EUNIT_ASSERT( EVIMPSTXimpOperationBind == iRequest->RequestType() );
    
    EUNIT_ASSERT( gTXIMPRequestId == iRequest->GetRequestId() );
    
    
    iRequest->SetRequestType(EVIMPSTXimpOperationUnBind);
    EUNIT_ASSERT( EVIMPSTXimpOperationUnBind == iRequest->RequestType() );
    
    iRequest->StartWait();
    EUNIT_ASSERT( ETrue == iRequest->IsWaitStarted() );    
    iRequest->StartWait();
    
    iRequest->StopWait();
    EUNIT_ASSERT( EFalse == iRequest->IsWaitStarted() );
    iRequest->StopWait();*/
      
    
    }


EUNIT_BEGIN_TEST_TABLE( T_VIMPSTEngineSearchMgrExtention, "T_VIMPSTEngineSearchMgrExtention", "" )

    EUNIT_TEST( "", "", "", "", SetupL, TestSearchContactsL, Teardown )
    EUNIT_TEST( "", "", "", "", SetupL, TestSubscribeForSearchKeysL, Teardown )
    EUNIT_TEST( "", "", "", "", SetupL, TestIsSupported, Teardown )
    EUNIT_TEST( "", "", "", "", SetupL, TestType, Teardown )
    EUNIT_TEST( "", "", "", "", SetupL, TestHandleSessionContextEventL, Teardown )
    
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
