/*
* t_vimpstenginepresencesubservice.cpp ::Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstenginepresencesubservice.cpp
*
*/
#include <platform/digia/eunit/eunitmacros.h>
#include <platform/digia/eunit/teunitassertioninfo.h>
#include <platform/digia/eunit/CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginepresencesubservice.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>
#include <avabilitytext.h>
//#include "vimpstcustomcleanupapi.h"
#include <cntdb.h>

#include "s_ximpclient.h"
#include "s_ximpcontext.h"
#include "s_presencefeatures.h"
#include "s_presentitygroups.h"
#include "s_ximprequestcompleteevent.h"
#include "s_ximppresensitypresenceevent.h"
#include "s_ximppresencegrantrequestlistevent.h"
#include "s_ximppresensitygroupcontentevent.h"
#include "s_ximpcontextstateevent.h"
#include "s_ximpidentity.h"
#include "s_presenceinfo.h"
#include "s_presencebuddyinfo2.h"
#include "s_presenceblockinfo.h"

//#include "s_presencebuddyinfolist.h"

#include "s_ximppresensitypresenceevent.h"
#include "s_presencesubserviceeventobserver.h"
#include "cvimpstenginepresencesubservice.h"
#include "cvimpstenginecchhandler.h"
#include "cvimpstengineservicetablefetcher.h"
#include "cvimpstenginesessioncntxtobserver.h"
#include "cvimpstblockedlistmanager.h"

//extern MyCchService* myService;
extern TBool createService;
extern TBool tryPresenceSubServiceAvailable;
extern TBool tryAllSubserviceAvailable;

extern TInt32 gMXIMPRequestCompleteEventStubInterfaceId;
extern TInt32 gMXIMPContextStateEventStubInterfaceId;
extern TInt32 gMXIMPPresentityPresenceEventStubInterfaceId;
extern TInt32 gMXIMPPresenceGrantRequestListEventStubInterfaceId;
extern TInt32 gMXIMPPresenceBlockInfoStubInterfaceId;
extern TInt32 gMXIMPPresentityGroupEventStubInterfaceId;
extern TBool gRequestCompleteEventResultCodeKerrNotSupported;
extern TXIMPRequestId gMXIMPRequestCompleteEventStubRequestId;
extern TBool gCreateXIMPIdentity;
extern TInt gNewRequestsCountValue;
extern TInt gCurrentRequestsCountValue;
extern TBool gPersonPresenceInfoAvailable;
extern TBool tryConnParamWithSuccess;
extern TBool tryParamTomatchOwnId;
//extern MPresenceInfoStub* gMPresenceInfoStub;
// cache error 
extern TInt gAllBuddiesPresenceInServiceError;
const TInt KTestServiceId = 5;

//_LIT( KTestDb, "contacts.cdb" );

//_LIT( KTestContactList, "TEST_CONTACT_LIST" );
//_LIT( KTestUserId, "TEST_USER_ID" );
//_LIT( KTestNickName, "TEST_NICKNAME" );
_LIT( KTestContactId, "TEST_CONTACT_ID" );
_LIT( KTestStatusText, "TEST_STATUS" );
//_LIT( KTestPresenceUri, "test@test.com" );

//availability status text
_LIT(KBlockedState,"BlockedState");
//_LIT(KInvisibleState,"InvisibleState");
//_LIT(KAwayState,"AwayState");
//_LIT(KOnPhoneState,"OnPhoneState");
//_LIT(KDndState,"DoNotDisturb");
//_LIT(KPendingRequestState,"PendingRequestState");

//extern RPointerArray<MPresenceBuddyInfo> gPresenceArry;


T_VimpstEnginePresenceSubService* T_VimpstEnginePresenceSubService::NewLC()
    {
    T_VimpstEnginePresenceSubService* self = new( ELeave ) T_VimpstEnginePresenceSubService;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEnginePresenceSubService::~T_VimpstEnginePresenceSubService()
    {
    }

void T_VimpstEnginePresenceSubService::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


//  FROM OBSERVER INTERFACES
void T_VimpstEnginePresenceSubService::HandleServceConnectionEventL()
    {
    }


void T_VimpstEnginePresenceSubService::SetupL()
    {
 	createService = ETrue;
    tryPresenceSubServiceAvailable = ETrue;
    
    iTestServiceId = CreateSpEntryL( KTestServiceId );
   
    iCchHandler = CVIMPSTEngineCchHandler::NewL( iTestServiceId, *iCchUiEventObserver);
   
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();
    
  
    iSessionCntxtObserver = CVIMPSTEngineSessionCntxtObserver::NewL( iTestServiceId );
    
    delete iSessionCntxtObserver->iFeatures;
    iSessionCntxtObserver->iFeatures = NULL;
    delete iSessionCntxtObserver->iPresenceCtx;
    iSessionCntxtObserver->iPresenceCtx = NULL;
    delete iSessionCntxtObserver->iClient;
    iSessionCntxtObserver->iClient = NULL;
  
    iSessionCntxtObserver->iClient = MXIMPClientStub::NewClientL();
    iSessionCntxtObserver->iPresenceCtx = 
        iSessionCntxtObserver->iClient->NewPresenceContextLC();    
    CleanupStack::Pop();// because of LC method
    iSessionCntxtObserver->iFeatures =  MyMPresenceFeatureStub::NewStubL( iSessionCntxtObserver->iPresenceCtx);
    
       
    iPresenceSubService = CVIMPSTEnginePresenceSubService::NewL( 
												        iTestServiceId,
												        *iCchHandler,
												        *iServiceTableFetcher,
												        *iSessionCntxtObserver,
												        *this );
    }

void T_VimpstEnginePresenceSubService::Teardown()
    {
    tryPresenceSubServiceAvailable = EFalse;
   // tryAllSubserviceAvailable = EFalse;
    createService = EFalse;
    
   // gCreateXIMPIdentity = EFalse;
   // gPersonPresenceInfoAvailable = EFalse;
    
    if ( iSessionCntxtObserver )
        {
        delete iSessionCntxtObserver->iFeatures;
	    iSessionCntxtObserver->iFeatures = NULL;
	    
	    delete iSessionCntxtObserver->iPresenceCtx;
	    iSessionCntxtObserver->iPresenceCtx = NULL;
	    
	    delete iSessionCntxtObserver->iClient;
	    iSessionCntxtObserver->iClient = NULL;
	    
	    delete iSessionCntxtObserver;
        iSessionCntxtObserver = NULL;
        }
   
    if ( iPresenceSubService )
        {
        delete iPresenceSubService;
        iPresenceSubService = NULL;
        } 
    if ( iCchHandler )
        {
        delete iCchHandler;
        iCchHandler = NULL;
        }
    if ( iServiceTableFetcher )
        {
        delete iServiceTableFetcher;
        iServiceTableFetcher = NULL;
        }
    
   /* if ( gMPresenceInfoStub )
        {
        delete gMPresenceInfoStub;
        gMPresenceInfoStub = NULL;
        }*/
    }

TInt T_VimpstEnginePresenceSubService::CreateSpEntryL( TInt aServiceId )
    {
    CSPSettings* settings = CSPSettings::NewLC();
    CSPEntry* entry = CSPEntry::NewLC();
    entry->SetServiceId( aServiceId );
    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = settings->FindEntryL( entry->GetServiceId(), *testEntry );
    if( result == KErrNone )
        {
        settings->UpdateEntryL( *entry );
        }
    else
        {
        settings->AddEntryL( *entry );
        }
    TInt createdServiceId = entry->GetServiceId();
    CleanupStack::PopAndDestroy( testEntry );
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( settings );
    return createdServiceId;
    }   

void T_VimpstEnginePresenceSubService::T_NewLL()
    {
    CVIMPSTEnginePresenceSubService* presenceSubService = NULL;
    presenceSubService = CVIMPSTEnginePresenceSubService::NewL(
        iTestServiceId,
        *iCchHandler,
        *iServiceTableFetcher,
        *iSessionCntxtObserver,
        *this);
       
    EUNIT_ASSERT( presenceSubService != NULL );
    delete presenceSubService;
    presenceSubService = NULL;
    }

void T_VimpstEnginePresenceSubService::T_SubServiceStateL()
    {
    RArray<TVIMPSTEnums::TVIMPSTRegistrationState> states;
    CleanupClosePushL( states );
    states.AppendL( TVIMPSTEnums::ESVCENotRegistered );
    states.AppendL( TVIMPSTEnums::ESVCERegistered );
    states.AppendL( TVIMPSTEnums::ESVCENetworkConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEWaitingForNetwork );
    states.AppendL( TVIMPSTEnums::ESVCENetworkDisConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEUpdatingContacts );
    for ( TInt i( 0 ) ; i < states.Count() ; i++ )
        {
        iPresenceSubService->iServiceState = states[ i ];
        EUNIT_ASSERT( states[ i ] == iPresenceSubService->SubServiceState() );
        }
       
    CleanupStack::PopAndDestroy( &states );
    }

void T_VimpstEnginePresenceSubService::T_TypeL()
    {    
    EUNIT_ASSERT( TVIMPSTEnums::EPresence == iPresenceSubService->Type() );
    }

void T_VimpstEnginePresenceSubService::T_Enabled()
    {
    // failed
    iPresenceSubService->iServiceState = TVIMPSTEnums::ESVCERegistered;
    TBool enabled = iPresenceSubService->Enabled();
    EUNIT_ASSERT( enabled == ETrue );
    
    iPresenceSubService->iServiceState = TVIMPSTEnums::ESVCENotRegistered;
    enabled = iPresenceSubService->Enabled();
    EUNIT_ASSERT( enabled == EFalse );
    }

void T_VimpstEnginePresenceSubService::T_RetrieveSubscribedListLL()
    {
    TInt error( KErrNone );
    gCreateXIMPIdentity = EFalse;
    TRAP( error, iPresenceSubService->RetrieveSubscribedListL( ) );
    EUNIT_ASSERT( KErrNoMemory == error );
    
    gCreateXIMPIdentity = ETrue;
    TRAP( error, iPresenceSubService->RetrieveSubscribedListL( ) );
    EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_SubscribePresenceOfSingleContactL()
    {
    gCreateXIMPIdentity = ETrue;
    TRAPD( err, iPresenceSubService->SubscribePresenceOfSingleContactL( KNullDesC ) );
    EUNIT_ASSERT( err == KErrArgument );
   
   	gCreateXIMPIdentity = ETrue;
    TRAP( err, iPresenceSubService->SubscribePresenceOfSingleContactL(KTestContactId ) );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_UnSubscribePresenceOfSingleContactLL()
    {
    gCreateXIMPIdentity = ETrue;
    TRAPD( err, iPresenceSubService->UnSubscribePresenceOfSingleContactL( KTestContactId ) );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_DoHandlePresentityGroupContentEventLL()
	{
	TInt error( KErrNone );
	MXIMPContext* context = NULL;
	MPresentityGroupContentEventStub presentityGroupContentEvent;
	TRAP( error, iPresenceSubService->DoHandlePresentityGroupContentEventL( 
	 								*context, presentityGroupContentEvent ) );
	EUNIT_ASSERT( KErrNone == error );

	iPresenceSubService->iServiceState = TVIMPSTEnums::ESVCEUpdatingContacts;
	TRAP( error, iPresenceSubService->DoHandlePresentityGroupContentEventL( 
	 								*context, presentityGroupContentEvent ) );
	EUNIT_ASSERT( KErrNone == error );
	}

void T_VimpstEnginePresenceSubService::T_DoHandlePresenceBlockListEventL()
    {
    TInt error( KErrNone );
    MXIMPContext* context = NULL;
    MPresenceBlockInfoStub presenceblockinfo;
  
    TRAP( error, iPresenceSubService->DoHandlePresenceBlockListEventL( 
                                    *context, presenceblockinfo ) );
    
    
    EUNIT_ASSERT( KErrNone == error );

    iPresenceSubService->iServiceState = TVIMPSTEnums::ESVCEUpdatingContacts;
    TRAP( error, iPresenceSubService->DoHandlePresenceBlockListEventL( 
                                    *context, presenceblockinfo ) );
    
 
    EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_DoHandlePresenceGrantRequestListEventLL()
    {
    TInt error( KErrNone );
    MXIMPContext* context = NULL;
    MPresenceGrantRequestListEventStub presenceGrantRequestListEventStub;
    ::gNewRequestsCountValue = 1;
    ::gCurrentRequestsCountValue = 1;
    
    TRAP( error, iPresenceSubService->DoHandlePresenceGrantRequestListEventL( 
        *context, presenceGrantRequestListEventStub ) );
    ::gNewRequestsCountValue = KErrNone;
    ::gCurrentRequestsCountValue = KErrNone;
    EUNIT_ASSERT( KErrNone == error );
    }


void T_VimpstEnginePresenceSubService::T_ResolveServiceStateLL()
    {
    TInt error( KErrNotFound );
    RArray<TCCHSubserviceState> subServiceStates;   
    CleanupClosePushL( subServiceStates );
    subServiceStates.AppendL( ECCHUninitialized );
    subServiceStates.AppendL( ECCHDisabled );
    subServiceStates.AppendL( ECCHConnecting );
    subServiceStates.AppendL( ECCHEnabled );
    subServiceStates.AppendL( ECCHDisconnecting );
    // Test with service error: KErrNone and all subservice states.
    TInt serviceError( KErrNone );
    for ( TInt i( 0 ) ; i < subServiceStates.Count() ; i++ )
        {
        TRAP( error, iPresenceSubService->ResolveServiceStateL( 
            	subServiceStates[ i ], serviceError ) );
        EUNIT_ASSERT( KErrNone == error );
        }
              
    // Test with service error: KErrNotFound and all subservice states.
    serviceError = KErrNotFound;
    TRAP( error, iPresenceSubService->ResolveServiceStateL( 
            	ECCHDisconnecting, serviceError ) );
    EUNIT_ASSERT( KErrNone == error );
           
    CleanupStack::PopAndDestroy( &subServiceStates );
    }

void T_VimpstEnginePresenceSubService::T_CchEventOccuredLL()
    {
    TInt error( KErrNotFound );
    RArray<TCCHSubserviceState> subServiceStates;   
    CleanupClosePushL( subServiceStates );
    subServiceStates.AppendL( ECCHUninitialized );
    subServiceStates.AppendL( ECCHDisabled );
    subServiceStates.AppendL( ECCHConnecting );
    subServiceStates.AppendL( ECCHEnabled );
    subServiceStates.AppendL( ECCHDisconnecting );
   // Test with service error: KErrNone and all subservice states.
    TInt serviceError( KErrNone );
    for ( TInt i( 0 ) ; i < subServiceStates.Count() ; i++ )
        {
        TRAP( error, iPresenceSubService->CchEventOccuredL( 
            iTestServiceId, subServiceStates[ i ], serviceError ) );
        EUNIT_ASSERT( KErrNone == error );
        }
    // Test with service error: KErrNotFound and all subservice states.
    serviceError = KErrNotFound;
    for ( TInt j( 0 ) ; j < subServiceStates.Count() ; j++ )
        {
        TRAP( error, iPresenceSubService->CchEventOccuredL( 
            iTestServiceId, subServiceStates[ j ], serviceError ) );
        EUNIT_ASSERT( KErrNone == error );
        }
    CleanupStack::PopAndDestroy( &subServiceStates );
    EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_DoHandleCchErrorLL()
    {
    RArray<TInt> serviceErrors;
    CleanupClosePushL( serviceErrors );
    serviceErrors.AppendL( KErrGeneral ); // Tests default case
    serviceErrors.AppendL( KCCHErrorBandwidthInsufficient );
    serviceErrors.AppendL( KCCHErrorInvalidIap );
    serviceErrors.AppendL( KCCHErrorAuthenticationFailed );
    serviceErrors.AppendL( KCCHErrorNetworkLost );
    serviceErrors.AppendL( KCCHErrorLoginFailed );
    serviceErrors.AppendL( KCCHErrorServiceNotResponding );
    serviceErrors.AppendL( KCCHErrorInvalidSettings );
    serviceErrors.AppendL( KCCHErrorAccessPointNotDefined );
    for ( TInt i( 0 ) ; i < serviceErrors.Count() ; i++ )
        {
        TRAPD( err, iPresenceSubService->DoHandleCchErrorL( 
            serviceErrors[ i ] ) );
        EUNIT_ASSERT( KErrNone == err );
        }
    CleanupStack::PopAndDestroy( &serviceErrors );
    }


void T_VimpstEnginePresenceSubService::T_PublishOwnPresenceLL()
    {
    RArray<TVIMPSTEnums::TOnlineStatus> onlineStatusArray;
    CleanupClosePushL( onlineStatusArray );
    onlineStatusArray.AppendL( TVIMPSTEnums::EUnknown );
 //   onlineStatusArray.AppendL( TVIMPSTEnums::EOffline );
 //   onlineStatusArray.AppendL( TVIMPSTEnums::EOnline );
 //   onlineStatusArray.AppendL( TVIMPSTEnums::EInvisible );
 //   onlineStatusArray.AppendL( TVIMPSTEnums::EAway );
 //   onlineStatusArray.AppendL( TVIMPSTEnums::EBusy );
    for ( TInt i( 0 ); i < onlineStatusArray.Count() ; i++ )
        {
        TRAPD( err, iPresenceSubService->PublishOwnPresenceL( 
            onlineStatusArray[ i ], KTestStatusText ) );
        EUNIT_ASSERT( KErrNone == err );
        }
    CleanupStack::PopAndDestroy( &onlineStatusArray );
    }

void T_VimpstEnginePresenceSubService::T_ConvertPresenceStatus()
    {
    EUNIT_ASSERT( NPresenceInfo::ENotAvailable == 
        iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EOffline ) );
    
    EUNIT_ASSERT( NPresenceInfo::EAvailable == 
            iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EOnline ) );
    
    EUNIT_ASSERT( NPresenceInfo::EHidden == 
            iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EInvisible ) );
    
    EUNIT_ASSERT( NPresenceInfo::EAway == 
            iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EAway ) );
    
    EUNIT_ASSERT( NPresenceInfo::EBusy == 
            iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EBusy ) );
    
    EUNIT_ASSERT( NPresenceInfo::ENotAvailable == 
                iPresenceSubService->ConvertPresenceStatus( TVIMPSTEnums::EUnknown ) );
    }
/*
void T_VimpstEnginePresenceSubService::T_FetchPresenceFormCacheL()
    {
    TInt error = iPresenceSubService->FetchPresenceFormCache();
    EUNIT_ASSERT( KErrNone == error );
    
    gAllBuddiesPresenceInServiceError = KErrBadHandle;
    error = iPresenceSubService->FetchPresenceFormCache();
    EUNIT_ASSERT( KErrBadHandle == error );
    
    delete iPresenceSubService->iServiceName;
    iPresenceSubService->iServiceName = NULL;
    error = iPresenceSubService->FetchPresenceFormCache();
    EUNIT_ASSERT( KErrArgument == error );
   
    }
*/
void T_VimpstEnginePresenceSubService::T_HandlePresenceReadLL()
    {
    TInt error( KErrNone );
    TInt testError( KErrGeneral );
    
    tryConnParamWithSuccess = ETrue;
    
    RPointerArray< MPresenceBuddyInfo2> buddyInfoList;
    TRAP( error, iPresenceSubService->HandlePresenceReadL( 
        testError, buddyInfoList ) );
    EUNIT_ASSERT( KErrNone == error );
    
    CleanupClosePushL( buddyInfoList );
    MPresenceBuddyInfo2Stub* buddyInfo1 = MPresenceBuddyInfo2Stub::NewStubL();
    CleanupDeletePushL( buddyInfo1 );
    MPresenceBuddyInfo2Stub* buddyInfo2 = MPresenceBuddyInfo2Stub::NewStubL();
    CleanupDeletePushL( buddyInfo2 );
    MPresenceBuddyInfo2Stub* buddyInfo3 = MPresenceBuddyInfo2Stub::NewStubL();
    CleanupDeletePushL( buddyInfo3 );
    
    buddyInfoList.AppendL( buddyInfo1 );
    buddyInfoList.AppendL( buddyInfo2 );
    buddyInfoList.AppendL( buddyInfo3 );
    testError = KErrNone;
    TRAP( error, iPresenceSubService->HandlePresenceReadL( 
        testError, buddyInfoList ) );
    EUNIT_ASSERT( KErrNone == error );
  
    CleanupStack::Pop(3); //buddyInfo1,2,3 just pop since ownership is tranferred
    
    tryConnParamWithSuccess = EFalse;
    tryParamTomatchOwnId = ETrue;

    testError = KErrNone;
    buddyInfo1 = MPresenceBuddyInfo2Stub::NewStubL();
    CleanupDeletePushL( buddyInfo1 );
    buddyInfoList.AppendL( buddyInfo1 );
   
    TRAP( error, iPresenceSubService->HandlePresenceReadL( 
            testError, buddyInfoList ) ); // buddyInfo : ownership transferred 
    EUNIT_ASSERT( KErrNone == error );
    CleanupStack::Pop(2); //buddyInfoList,buddyInfo1
    }
/*
void T_VimpstEnginePresenceSubService::T_SubscribeToPresenceCacheLL()
    {
    TRAPD( err, iPresenceSubService->SubscribeToPresenceCacheL( 
        KTestContactId ) );
    EUNIT_ASSERT( KErrNone == err );
    
    TRAP( err, iPresenceSubService->SubscribeToPresenceCacheL( 
        KNullDesC ) );
    EUNIT_ASSERT( KErrArgument == err );
   
    }*/


void T_VimpstEnginePresenceSubService::T_HandlePresenceNotificationLL()
    {
    tryConnParamWithSuccess = ETrue;
    TInt error( KErrNone );
    MPresenceBuddyInfo2Stub* buddyInfo = NULL;
    TInt testError( KErrGeneral );
    TRAP( error, iPresenceSubService->HandlePresenceNotificationL( 
        testError, buddyInfo ) );
    EUNIT_ASSERT( KErrNone == error );
     
    testError = KErrNone;
    buddyInfo = MPresenceBuddyInfo2Stub::NewStubL();
    TRAP( error, iPresenceSubService->HandlePresenceNotificationL( 
        testError, buddyInfo ) ); // buddyInfo : ownership transferred 
    EUNIT_ASSERT( KErrNone == error );
    tryConnParamWithSuccess = EFalse;
    tryParamTomatchOwnId = ETrue;
    
    testError = KErrNone;
    buddyInfo = MPresenceBuddyInfo2Stub::NewStubL();
    TRAP( error, iPresenceSubService->HandlePresenceNotificationL( 
            testError, buddyInfo ) ); // buddyInfo : ownership transferred 
    EUNIT_ASSERT( KErrNone == error );
   
    }

void T_VimpstEnginePresenceSubService::T_UnSubscribeForAuthorizationLL()
    {
    TRAPD( err, iPresenceSubService->UnsubscribeListsL() );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_SubscribeForAuthorizationLL()
    {
    TRAPD( err, iPresenceSubService->SubscribeForAuthorizationL() );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_SendPresenceGrantPresentityLL()
    {
    gCreateXIMPIdentity = EFalse;
    TInt error( KErrNone );
    TRAP( error, iPresenceSubService->SendPresenceGrantPresentityL( 
        KTestContactId, EFalse ) );
    EUNIT_ASSERT( KErrNoMemory == error );
    
    gCreateXIMPIdentity = ETrue;
    TRAP( error, iPresenceSubService->SendPresenceGrantPresentityL( 
        KNullDesC, EFalse ) );
    EUNIT_ASSERT( KErrArgument == error );
    
	TRAP( error, iPresenceSubService->SendPresenceGrantPresentityL( 
        KTestContactId, EFalse ) );
    EUNIT_ASSERT( KErrNone == error );
    
    TRAP( error, iPresenceSubService->SendPresenceGrantPresentityL( 
        KTestContactId, ETrue ) );
    EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_HandleSessionContextEventLL()
    {
    // failed
    gCreateXIMPIdentity = ETrue;
    MXIMPContext* context = NULL;
    //Test with MXIMPRequestCompleteEvent: HandleXimpRequestCompleteL
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;
    TInt error( KErrNone );
  
    RArray<TXimpOperation> ximpOperation;   
    CleanupClosePushL( ximpOperation );
    ximpOperation.AppendL( EVIMPSTXimpOperationNoOperation );
    ximpOperation.AppendL( EVIMPSTXimpOperationBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetSubscribedList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetBlockList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetPresenceGrantList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationWithdrawPresenceGrant );
    ximpOperation.AppendL( EVIMPSTXimpOperationGrantPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationCancelPresenceBlockFromPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationBlockPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationPublisOwnPresence );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationAddContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationDeleteContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSearchContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribeSearchKeys );
    
    // Test with error code: KErrNone and all operations.
    ::gRequestCompleteEventResultCodeKerrNotSupported = ETrue;
     for ( TInt i( 0 ) ; i < ximpOperation.Count() ; i++ )
        {
        TRAP( error, iPresenceSubService->HandleSessionContextEventL(
                *context, requestCompleteEvent, ximpOperation[ i ] ) );
        EUNIT_ASSERT( KErrNone == error );
        }
              
    ::gRequestCompleteEventResultCodeKerrNotSupported = EFalse;
    for ( TInt j( 0 ) ; j < ximpOperation.Count() ; j++ )
        {
        TRAP( error, iPresenceSubService->HandleSessionContextEventL( 
                *context, requestCompleteEvent, ximpOperation[ j ] ) );
        EUNIT_ASSERT( KErrNone == error );
        }
    //Test with MXIMPContextStateEvent
    MXIMPContextStateEventStub contextStateEvent;
    ::gMXIMPContextStateEventStubInterfaceId = MXIMPContextStateEvent::KInterfaceId;
    TRAP( error, iPresenceSubService->HandleSessionContextEventL( 
            *context, contextStateEvent, ximpOperation[ 0 ] ) );
     EUNIT_ASSERT( KErrNone == error );  
       
    //Test with MPresentityGroupContentEvent: DoHandlePresentityGroupContentEventL( aContext, aEvent )
    MPresentityGroupContentEventStub presentityGroupContentEvent;
    ::gMXIMPPresentityGroupEventStubInterfaceId = 
            MPresentityGroupContentEvent::KInterfaceId;
    TRAP( error, iPresenceSubService->HandleSessionContextEventL( 
            *context, presentityGroupContentEvent, ximpOperation[ 0 ] ) );
    EUNIT_ASSERT( KErrNone == error );
    
    //Test with MPresenceGrantRequestListEvent: DoHandlePresenceGrantRequestListEventL( aContext, aEvent )
    MPresenceGrantRequestListEventStub presenceGrantRequestListEventStub;
    ::gMXIMPPresenceGrantRequestListEventStubInterfaceId = 
            MPresenceGrantRequestListEvent::KInterfaceId;
    TRAP( error, iPresenceSubService->HandleSessionContextEventL( 
            *context, presenceGrantRequestListEventStub, ximpOperation[ 0 ] ) );
    EUNIT_ASSERT( KErrNone == error );
    
    CleanupStack::PopAndDestroy( &ximpOperation );
    }

void T_VimpstEnginePresenceSubService::T_HandleListEventCompleteLL()
    {
    // failed
    gCreateXIMPIdentity = ETrue;
    TInt error( KErrNone );
    RArray<TXimpOperation> ximpOperation;   
    CleanupClosePushL( ximpOperation );
    
    ximpOperation.AppendL( EVIMPSTXimpOperationNoOperation );
    ximpOperation.AppendL( EVIMPSTXimpOperationBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetSubscribedList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetBlockList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetPresenceGrantList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationWithdrawPresenceGrant );
    ximpOperation.AppendL( EVIMPSTXimpOperationGrantPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationCancelPresenceBlockFromPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationBlockPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationPublisOwnPresence );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationAddContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationDeleteContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSearchContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribeSearchKeys );
    
    MXIMPRequestCompleteEventStub requestCompleteEvent;
     // Test with error code: KErrNone and all operations.
    TInt errorCode( KErrNone );
    for ( TInt i( 0 ) ; i < ximpOperation.Count() ; i++ )
        {
        TRAP( error, iPresenceSubService->HandleListEventCompleteL( 
                ximpOperation[ i ], errorCode, requestCompleteEvent ) );
        EUNIT_ASSERT( KErrNone == error );
        }
     // Test with random error code
    errorCode = KErrGeneral;
    for ( TInt j( 0 ) ; j < ximpOperation.Count() ; j++ )
        {
        TRAP( error, iPresenceSubService->HandleListEventCompleteL( 
                ximpOperation[ j ], errorCode, requestCompleteEvent ) );
        EUNIT_ASSERT( KErrNone == error );
        }
    CleanupStack::PopAndDestroy( &ximpOperation );
    EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_HandleXimpRequestCompleteLL()
    {
    // failed
    gCreateXIMPIdentity = ETrue;
     //Test with MXIMPRequestCompleteEvent: HandleXimpRequestCompleteL
    MXIMPRequestCompleteEventStub requestCompleteEvent;
    ::gMXIMPRequestCompleteEventStubInterfaceId = MXIMPRequestCompleteEvent::KInterfaceId;
    TInt error( KErrNone );
    RArray<TXimpOperation> ximpOperation;   
    CleanupClosePushL( ximpOperation );
    
    ximpOperation.AppendL( EVIMPSTXimpOperationNoOperation );
    ximpOperation.AppendL( EVIMPSTXimpOperationBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnBind );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetSubscribedList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetBlockList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetPresenceGrantList );
    ximpOperation.AppendL( EVIMPSTXimpOperationGetWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeWatcherList );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubcribeGrantRequestList );
    ximpOperation.AppendL( EVIMPSTXimpOperationWithdrawPresenceGrant );
    ximpOperation.AppendL( EVIMPSTXimpOperationGrantPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationCancelPresenceBlockFromPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationBlockPresenceForPresentity );
    ximpOperation.AppendL( EVIMPSTXimpOperationPublisOwnPresence );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationUnsubscribe );
    ximpOperation.AppendL( EVIMPSTXimpOperationAddContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationDeleteContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSearchContact );
    ximpOperation.AppendL( EVIMPSTXimpOperationSubscribeSearchKeys );
      
    // Test with error code: KErrNone and all operations.
    ::gRequestCompleteEventResultCodeKerrNotSupported = ETrue;
     // Test with KErrNone
    TInt testError( KErrNone ); 
    for ( TInt i( 0 ) ; i < ximpOperation.Count() ; i++ )
          {
          TRAP( error, iPresenceSubService->HandleXimpRequestCompleteL(
                  ximpOperation[ i ], testError, requestCompleteEvent ) );
          EUNIT_ASSERT( KErrNone == error );
          }
                
    // Test with random error code
    testError = KErrGeneral;
    for ( TInt j( 0 ) ; j < ximpOperation.Count() ; j++ )
        {
        TRAP( error, iPresenceSubService->HandleXimpRequestCompleteL(
            ximpOperation[ j ], testError, requestCompleteEvent ) );
        EUNIT_ASSERT( KErrNone == error );
        }
     CleanupStack::PopAndDestroy( &ximpOperation );
     EUNIT_ASSERT( KErrNone == error );
    }

void T_VimpstEnginePresenceSubService::T_StoreToVirtualStoreLL()
    {
    MPresentityGroupContentEventStub presentityGroupContentEvent;
    TRAPD( err, iPresenceSubService->StoreToVirtualStoreL( presentityGroupContentEvent ) );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_IsChangeOwnStatusSupportedL()
    {
    iPresenceSubService->iChangeStatusSupported = EFalse;
    TBool supported =
        iPresenceSubService->IsChangeOwnStatusSupported();
    EUNIT_ASSERT( EFalse == supported );
    
    iPresenceSubService->iChangeStatusSupported = ETrue;
    supported =
        iPresenceSubService->IsChangeOwnStatusSupported();
    EUNIT_ASSERT( ETrue == supported );
    }

void T_VimpstEnginePresenceSubService::T_IsStatusMsgSupportedL()
    {
    iPresenceSubService->iChangeStatusMsgSupported = EFalse;
    TBool supported =
        iPresenceSubService->IsStatusMsgSupported();
    EUNIT_ASSERT( EFalse == supported );
    
    iPresenceSubService->iChangeStatusMsgSupported = ETrue;
    supported =
        iPresenceSubService->IsStatusMsgSupported();
    EUNIT_ASSERT( ETrue == supported );
    }

void T_VimpstEnginePresenceSubService::T_ConvertXimpToClientPresenceStatusL()
    {
    RArray<MPresenceBuddyInfo2::TAvailabilityValues> presenceInfo;
   // CleanupStack::PushL(&presenceInfo);
    CleanupClosePushL( presenceInfo );
    presenceInfo.AppendL( MPresenceBuddyInfo2::EUnknownAvailability );
    presenceInfo.AppendL( MPresenceBuddyInfo2::ENotAvailable );
    presenceInfo.AppendL( MPresenceBuddyInfo2::EBusy );
    presenceInfo.AppendL( MPresenceBuddyInfo2::EAvailable );
    
    RPointerArray<HBufC>    availabilityTextArray; 
   // CleanupStack::PushL(&availabilityTextArray);
    CleanupClosePushL( availabilityTextArray );

    availabilityTextArray.Append(KBlockedState().AllocL());
    availabilityTextArray.Append(KInvisibleState().AllocL());
    availabilityTextArray.Append(KAwayState().AllocL());
    availabilityTextArray.Append(KOnPhoneState().AllocL());
    availabilityTextArray.Append(KDndState().AllocL());
    
    TInt error( KErrNone );
    for ( TInt i( 0 ) ; i < presenceInfo.Count() ; i++ )
        {
        if(MPresenceBuddyInfo2::EBusy == presenceInfo[i])
            {
            for(TInt j = 0; j < availabilityTextArray.Count(); j++)
                {
                TRAP( error, iPresenceSubService->ConvertPresenceCacheEnums( 
                               presenceInfo[ i ], availabilityTextArray[i]->Des()));
                }
            }
        else
            {
            TRAP( error, iPresenceSubService->ConvertPresenceCacheEnums( 
                            presenceInfo[ i ], KNullDesC()));
            }
        
        EUNIT_ASSERT( KErrNone == error );
        }
    //CleanupStack::PopAndDestroy(&availabilityTextArray);
    //CleanupStack::PopAndDestroy( &presenceInfo );
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    EUNIT_ASSERT( KErrNone == error );
    }
/*
void T_VimpstEnginePresenceSubService::T_RegisterPresenceEventObserverL()
    {
    MyPresenceSubServiceEventObserverStub* observerNull = NULL ;
    TRAPD( error, iPresenceSubService->RegisterPresenceEventObserverL( observerNull ) );    
    EUNIT_ASSERT( KErrArgument == error );
   
   	MyPresenceSubServiceEventObserverStub* observer = MyPresenceSubServiceEventObserverStub::NewL() ;
    TRAP( error, iPresenceSubService->RegisterPresenceEventObserverL( observer ) );    
    EUNIT_ASSERT( KErrNone == error );
    EUNIT_ASSERT( iPresenceSubService->iObserverArray.Count() == 1 );
    // Try to register same observer again
    iPresenceSubService->RegisterPresenceEventObserverL( observer );    
    EUNIT_ASSERT( iPresenceSubService->iObserverArray.Count() == 1 );
    delete observer;
    }
*/
/*
void T_VimpstEnginePresenceSubService::T_UnRegisterPresenceEventObserverL()
    {
    MyPresenceSubServiceEventObserverStub* observerNull = NULL ;
    TRAPD( error, iPresenceSubService->RegisterPresenceEventObserverL( observerNull ) );
    EUNIT_ASSERT( KErrArgument == error );
    
    MyPresenceSubServiceEventObserverStub* observer  = MyPresenceSubServiceEventObserverStub::NewL() ;
    TRAP( error, iPresenceSubService->RegisterPresenceEventObserverL( observer ) );    
    EUNIT_ASSERT( KErrNone == error );
    TRAP(error, iPresenceSubService->RegisterPresenceEventObserverL( observer ) );
    EUNIT_ASSERT( KErrNone == error );
  
    iPresenceSubService->UnRegisterPresenceEventObserver( observer );
    EUNIT_ASSERT( iPresenceSubService->iObserverArray.Count() == 0 ); 
    
    // Try to unregister same observer again
    iPresenceSubService->UnRegisterPresenceEventObserver ( observer );
    EUNIT_ASSERT( iPresenceSubService->iObserverArray.Count() == 0 );
    delete observer; 
    }
*/
void T_VimpstEnginePresenceSubService::T_AddToBlockListL()
    {
    _LIT(KDummyUserID,"dummy@domian.com");
    TRAPD( err, iPresenceSubService->AddToBlockListL(KDummyUserID) );
           EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_AddToBlockListL_NoUserid()
    {
       TRAPD( err, iPresenceSubService->AddToBlockListL(KNullDesC()) );
              EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_AddToBlockListL_NoDomainNameL()
    {
    _LIT(KDummyUserID,"dummy");
    TRAPD( err, iPresenceSubService->AddToBlockListL(KDummyUserID) );
                  EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_RemoveFromBlockListL()
    {
    _LIT(KDummyUserID,"dummy@domian.com");
      TRAPD( err, iPresenceSubService->RemoveFromBlockListL(KDummyUserID) );
             EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_RemoveFromBlockListMgr()
    {
    _LIT(KID1,"id1@xyz.com");
    _LIT(KID2,"id2@xyz.com");
    _LIT(KID3,"id3@xyz.com");
    _LIT(KID4,"id4@xyz.com");
    iPresenceSubService->ResetBlockedListManagerL();
    if(iPresenceSubService->iBlockedListMgr)
	    {
	    iPresenceSubService->iBlockedListMgr->AddToBlockedListL(KID1);
	    iPresenceSubService->iBlockedListMgr->AddToBlockedListL(KID2);
	    iPresenceSubService->iBlockedListMgr->AddToBlockedListL(KID3);
	    iPresenceSubService->iBlockedListMgr->AddToBlockedListL(KID4);
		EUNIT_ASSERT( 4 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
		iPresenceSubService->iBlockedListMgr->RemoveFromBlockListL(KID2);
		EUNIT_ASSERT( 3 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
		iPresenceSubService->iBlockedListMgr->RemoveFromBlockListL(KID1);
		EUNIT_ASSERT( 2 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
		iPresenceSubService->iBlockedListMgr->RemoveFromBlockListL(KID3);
		EUNIT_ASSERT( 1 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
		iPresenceSubService->iBlockedListMgr->RemoveFromBlockListL(KID4);
		EUNIT_ASSERT( 0 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
		iPresenceSubService->iBlockedListMgr->RemoveFromBlockListL(KID4);
		EUNIT_ASSERT( 0 == iPresenceSubService->iBlockedListMgr->iBlockedList.Count() );
	    }
    }
    
void T_VimpstEnginePresenceSubService::T_FetchBlockedListFromServer()
	{
	iPresenceSubService->FetchBlockedListFromServerL(NULL);
	EUNIT_ASSERT(iPresenceSubService->iBlockListFetchReqPending == ETrue );
	EUNIT_ASSERT(iPresenceSubService->iBlockedListObserver == NULL)
	}
	
void T_VimpstEnginePresenceSubService::T_RemoveFromBlockList_NoUseridL()
    {
      TRAPD( err, iPresenceSubService->RemoveFromBlockListL(KNullDesC()) );
             EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_RemoveFromBlockList_NoDomainNameL()
    {
    _LIT(KDummyUserID,"dummy");
      TRAPD( err, iPresenceSubService->RemoveFromBlockListL(KDummyUserID) );
             EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEnginePresenceSubService::T_GetKeyFiledsAndValuesStatusPendingL()
    {
    TVIMPSTEnums::TOnlineStatus status = TVIMPSTEnums::EUnknown;
    MPresenceBuddyInfo2Stub* buddyInfo1 = MPresenceBuddyInfo2Stub::NewStubL();
    buddyInfo1->SetAnyFieldL(KExtensionKey,KPendingRequestExtensionValue);
    //set the setanyfield to pending and get that status 
    TRAPD( error, iPresenceSubService->GetKeyFieldsAndValuesL(*buddyInfo1,status) ); 
    EUNIT_ASSERT(TVIMPSTEnums::EPending  == status  ) ;
    
    }

void T_VimpstEnginePresenceSubService::T_GetKeyFiledsAndValuesStatusBlockedL()
    {
    TVIMPSTEnums::TOnlineStatus status = TVIMPSTEnums::EUnknown;
    MPresenceBuddyInfo2Stub* buddyInfo1 = MPresenceBuddyInfo2Stub::NewStubL();
    buddyInfo1->SetAnyFieldL(KExtensionKey,KBlockedExtensionValue);
    //set the setanyfield to pending and get that status 
    TRAPD( error, iPresenceSubService->GetKeyFieldsAndValuesL(*buddyInfo1,status) ); 
    EUNIT_ASSERT(TVIMPSTEnums::EBlocked  == status  ) ;
    
    }

void T_VimpstEnginePresenceSubService::T_GetKeyFiledsAndValuesStatusUnknownL()
    {
    TVIMPSTEnums::TOnlineStatus status = TVIMPSTEnums::EUnknown;
    MPresenceBuddyInfo2Stub* buddyInfo1 = MPresenceBuddyInfo2Stub::NewStubL();
    //set the setanyfield to pending and get that status 
    TRAPD( error, iPresenceSubService->GetKeyFieldsAndValuesL(*buddyInfo1,status ) ); 
    EUNIT_ASSERT(TVIMPSTEnums::EUnknown == status  ) ;
    
    }
EUNIT_BEGIN_TEST_TABLE( T_VimpstEnginePresenceSubService, "T_VimpstEnginePresenceSubService", "" )

EUNIT_TEST(
    "NewL - test ",
    "T_VimpstEnginePresenceSubService",
    "NewL",
    "FUNCTIONALITY",
    SetupL, T_NewLL, Teardown)

EUNIT_TEST(
    "SubServiceStateL - test ",
    "T_VimpstEnginePresenceSubService",
    "SubServiceStateL",
    "FUNCTIONALITY",
    SetupL, T_SubServiceStateL, Teardown)

EUNIT_TEST(
    "Type - test ",
    "T_VimpstEnginePresenceSubService",
    "Type",
    "FUNCTIONALITY",
    SetupL, T_TypeL, Teardown)

EUNIT_TEST(
    "Enabled - test ",
    "T_VimpstEnginePresenceSubService",
    "Enabled",
    "FUNCTIONALITY",
    SetupL, T_Enabled, Teardown)

EUNIT_TEST(
    "RetrieveSubscribedListL - test ",
    "T_VimpstEnginePresenceSubService",
    "RetrieveSubscribedListL",
    "FUNCTIONALITY",
    SetupL, T_RetrieveSubscribedListLL, Teardown)
    
EUNIT_TEST(
    "SubscribePresenceOfSingleContact - test ",
    "T_VimpstEnginePresenceSubService",
    "SubscribePresenceOfSingleContact",
    "FUNCTIONALITY",
    SetupL, T_SubscribePresenceOfSingleContactL, Teardown)
    
EUNIT_TEST(
    "UnSubscribePresenceOfSingleContactL - test ",
    "T_VimpstEnginePresenceSubService",
    "UnSubscribePresenceOfSingleContactL",
    "FUNCTIONALITY",
    SetupL, T_UnSubscribePresenceOfSingleContactLL, Teardown)


EUNIT_TEST(
    "DoHandlePresentityGroupContentEventL - test ",
    "T_VimpstEnginePresenceSubService",
    "DoHandlePresentityGroupContentEventL",
    "FUNCTIONALITY",
    SetupL, T_DoHandlePresentityGroupContentEventLL, Teardown)

EUNIT_TEST(
    "DoHandlePresenceGrantRequestListEventL - test ",
    "T_VimpstEnginePresenceSubService",
    "DoHandlePresenceGrantRequestListEventL",
    "FUNCTIONALITY",
    SetupL, T_DoHandlePresenceGrantRequestListEventLL, Teardown)
 
EUNIT_TEST(
    "T_DoHandlePresenceBlockListEventL - test ",
    "T_VimpstEnginePresenceSubService",
    "T_DoHandlePresenceBlockListEventL",
    "FUNCTIONALITY",
    SetupL, T_DoHandlePresenceBlockListEventL, Teardown)


EUNIT_TEST(
    "ResolveServiceStateL - test ",
    "T_VimpstEnginePresenceSubService",
    "ResolveServiceStateL",
    "FUNCTIONALITY",
    SetupL, T_ResolveServiceStateLL, Teardown)
    
EUNIT_TEST(
    "CchEventOccuredL - test ",
    "T_VimpstEnginePresenceSubService",
    "CchEventOccuredL",
    "FUNCTIONALITY",
    SetupL, T_CchEventOccuredLL, Teardown)

EUNIT_TEST(
    "DoHandleCchErrorL - test ",
    "T_VimpstEnginePresenceSubService",
    "DoHandleCchErrorL",
    "FUNCTIONALITY",
    SetupL, T_DoHandleCchErrorLL, Teardown)
    
EUNIT_TEST(
    "PublishOwnPresenceL - test ",
    "T_VimpstEnginePresenceSubService",
    "PublishOwnPresenceL",
    "FUNCTIONALITY",
    SetupL, T_PublishOwnPresenceLL, Teardown)
    
EUNIT_TEST(
    "ConvertPresenceStatus - test ",
    "T_VimpstEnginePresenceSubService",
    "ConvertPresenceStatus",
    "FUNCTIONALITY",
    SetupL, T_ConvertPresenceStatus, Teardown)
/*
EUNIT_TEST(
    "FetchPresenceFormCache - test ",
    "T_VimpstEnginePresenceSubService",
    "FetchPresenceFormCache",
    "FUNCTIONALITY",
    SetupL, T_FetchPresenceFormCacheL, Teardown)
 */   
EUNIT_TEST(
    "HandlePresenceReadL - test ",
    "T_VimpstEnginePresenceSubService",
    "HandlePresenceReadL",
    "FUNCTIONALITY",
    SetupL, T_HandlePresenceReadLL, Teardown)
/*    
EUNIT_TEST(
    "SubscribeToPresenceCacheL - test ",
    "T_VimpstEnginePresenceSubService",
    "SubscribeToPresenceCacheL",
    "FUNCTIONALITY",
    SetupL, T_SubscribeToPresenceCacheLL, Teardown)
 */   
EUNIT_TEST(
    "HandlePresenceNotificationL - test ",
    "T_VimpstEnginePresenceSubService",
    "HandlePresenceNotificationL",
    "FUNCTIONALITY",
    SetupL, T_HandlePresenceNotificationLL, Teardown)
    
EUNIT_TEST(
    "UnSubscribeForAuthorizationL - test ",
    "T_VimpstEnginePresenceSubService",
    "UnSubscribeForAuthorizationL",
    "FUNCTIONALITY",
    SetupL, T_UnSubscribeForAuthorizationLL, Teardown)
    
EUNIT_TEST(
    "SubscribeForAuthorizationL - test ",
    "T_VimpstEnginePresenceSubService",
    "SubscribeForAuthorizationL",
    "FUNCTIONALITY",
    SetupL, T_SubscribeForAuthorizationLL, Teardown)
    
EUNIT_TEST(
    "SendPresenceGrantPresentityL - test ",
    "T_VimpstEnginePresenceSubService",
    "SendPresenceGrantPresentityL",
    "FUNCTIONALITY",
    SetupL, T_SendPresenceGrantPresentityLL, Teardown)
    
EUNIT_TEST(
    "HandleSessionContextEventL - test ",
    "T_VimpstEnginePresenceSubService",
    "HandleSessionContextEventL",
    "FUNCTIONALITY",
    SetupL, T_HandleSessionContextEventLL, Teardown)
    
EUNIT_TEST(
    "HandleListEventCompleteL - test ",
    "T_VimpstEnginePresenceSubService",
    "HandleListEventCompleteL",
    "FUNCTIONALITY",
    SetupL, T_HandleListEventCompleteLL, Teardown)
    
EUNIT_TEST(
    "HandleXimpRequestCompleteL - test ",
    "T_VimpstEnginePresenceSubService",
    "HandleXimpRequestCompleteL",
    "FUNCTIONALITY",
    SetupL, T_HandleXimpRequestCompleteLL, Teardown)
    
EUNIT_TEST(
    "StoreToVirtualStoreL - test ",
    "T_VimpstEnginePresenceSubService",
    "StoreToVirtualStoreL",
    "FUNCTIONALITY",
    SetupL, T_StoreToVirtualStoreLL, Teardown)
    
    
EUNIT_TEST(
    "IsChangeOwnStatusSupported - test ",
    "T_VimpstEnginePresenceSubService",
    "IsChangeOwnStatusSupported",
    "FUNCTIONALITY",
    SetupL, T_IsChangeOwnStatusSupportedL, Teardown)
    
EUNIT_TEST(
    "IsStatusMsgSupported - test ",
    "T_VimpstEnginePresenceSubService",
    "IsStatusMsgSupported",
    "FUNCTIONALITY",
    SetupL, T_IsStatusMsgSupportedL, Teardown)
    

    
EUNIT_TEST(
    "ConvertXimpToClientPresenceStatus - test ",
    "T_VimpstEnginePresenceSubService",
    "ConvertXimpToClientPresenceStatus",
    "FUNCTIONALITY",
    SetupL, T_ConvertXimpToClientPresenceStatusL, Teardown)
/*    
EUNIT_TEST(
    "RegisterPresenceEventObserver - test ",
    "T_VimpstEnginePresenceSubService",
    "RegisterPresenceEventObserver",
    "FUNCTIONALITY",
    SetupL, T_RegisterPresenceEventObserverL, Teardown)
    
EUNIT_TEST(
    "UnRegisterPresenceEventObserver - test ",
    "T_VimpstEnginePresenceSubService",
    "UnRegisterPresenceEventObserver",
    "FUNCTIONALITY",
    SetupL, T_UnRegisterPresenceEventObserverL, Teardown)
*/    
EUNIT_TEST(
    "AddToBlockListL - test ",
    "T_VimpstEnginePresenceSubService",
    "AddToBlockListL",
    "FUNCTIONALITY",
    SetupL, T_AddToBlockListL, Teardown)
    
EUNIT_TEST(
    "AddToBlockListL_nouserid - test ",
    "T_VimpstEnginePresenceSubService",
    "AddToBlockListL_nouserid",
    "FUNCTIONALITY",
    SetupL, T_AddToBlockListL_NoUserid, Teardown)
EUNIT_TEST(
    "AddToBlockListL_nodomainname - test ",
    "T_VimpstEnginePresenceSubService",
    "AddToBlockListL_nodomainname",
    "FUNCTIONALITY",
    SetupL, T_AddToBlockListL_NoDomainNameL, Teardown)
EUNIT_TEST(
    "RemoveFromBlockListL - test ",
    "T_VimpstEnginePresenceSubService",
    "RemoveFromBlockListL",
    "FUNCTIONALITY",
    SetupL, T_RemoveFromBlockListL, Teardown)
    
EUNIT_TEST(
    "RemoveFromBlockListL_nouserid - test ",
    "T_VimpstEnginePresenceSubService",
    "RemoveFromBlockListL_nouserid",
    "FUNCTIONALITY",
    SetupL, T_RemoveFromBlockList_NoUseridL, Teardown)

EUNIT_TEST(
    "T_RemoveFromBlockListMgr - test ",
    "T_RemoveFromBlockListMgr",
    "T_RemoveFromBlockListMgr",
    "FUNCTIONALITY",
    SetupL, T_RemoveFromBlockListMgr, Teardown)
    
EUNIT_TEST(
    "T_GetKeyFiledsAndValuesStatusPendingL - test ",
    "T_GetKeyFiledsAndValuesStatusPendingL",
    "T_GetKeyFiledsAndValuesStatusPendingL",
    "FUNCTIONALITY",
    SetupL, T_GetKeyFiledsAndValuesStatusPendingL, Teardown)
    
EUNIT_TEST(
    "T_GetKeyFiledsAndValuesStatusBlockedL - test ",
    "T_GetKeyFiledsAndValuesStatusBlockedL",
    "T_GetKeyFiledsAndValuesStatusBlockedL",
    "FUNCTIONALITY",
    SetupL, T_GetKeyFiledsAndValuesStatusBlockedL, Teardown)
    
EUNIT_TEST(
    "T_GetKeyFiledsAndValuesStatusUnknownL - test ",
    "T_GetKeyFiledsAndValuesStatusUnknownL",
    "T_GetKeyFiledsAndValuesStatusUnknownL",
    "FUNCTIONALITY",
    SetupL, T_GetKeyFiledsAndValuesStatusUnknownL, Teardown)
    
EUNIT_TEST(
    "T_FetchBlockedListFromServer - test ",
    "T_FetchBlockedListFromServer",
    "T_FetchBlockedListFromServer",
    "FUNCTIONALITY",
    SetupL, T_FetchBlockedListFromServer, Teardown)
    

EUNIT_END_TESTCASE_TABLE

//  END OF FILE
