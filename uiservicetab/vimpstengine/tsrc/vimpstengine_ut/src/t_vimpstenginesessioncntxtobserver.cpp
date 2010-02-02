/*
* t_vimpstenginesessioncntxtobserver.cpp ::Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstenginesessioncntxtobserver.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstenginesessioncntxtobserver.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>

#include "cvimpstenginesessioncntxtobserver.h"

#include "s_ximpclient.h"
#include "s_ximpcontext.h"
#include "s_ximpidentity.h"
#include "s_presencefeatures.h"
#include "s_presentitygroups.h"
#include "s_ximprequestcompleteevent.h"
#include "s_ximppresensitypresenceevent.h"
#include "s_ximppresensitygroupcontentevent.h"
#include "s_ximpcontextstateevent.h"
#include "s_ximppresencegrantrequestlistevent.h"
#include "s_ximpsearchevent.h"
#include "s_ximpsearchkeysevent.h"
#include "s_presenceblockinfo.h"


#include "mvimpstenginesessioncntxtobserver.h"

extern TInt32 gMXIMPRequestCompleteEventStubInterfaceId;
extern TXIMPRequestId gMXIMPRequestCompleteEventStubRequestId;

extern TInt gUnBindingLeaves;

const TInt KTestServiceId = 5;


class dummy_observer: public MVIMPSTEngineSessionCntxtObserver
    {
    void HandleSessionContextEventL( const MXIMPContext& /*aContext*/, 
                                            const MXIMPBase& /*aEvent*/,
                                            TXimpOperation /*aXimpOperation = EVIMPSTXimpOperationNoOperation*/
                                            )
        {
        
        }
    };

T_VimpstEngineSessionCntxtObserver* T_VimpstEngineSessionCntxtObserver::NewLC()
    {
    T_VimpstEngineSessionCntxtObserver* self = 
        new( ELeave ) T_VimpstEngineSessionCntxtObserver;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEngineSessionCntxtObserver::~T_VimpstEngineSessionCntxtObserver()
    {
    }

void T_VimpstEngineSessionCntxtObserver::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void T_VimpstEngineSessionCntxtObserver::Setup1L()
    {
    iTestServiceId = CreateSpEntryL( KTestServiceId );
    }

void T_VimpstEngineSessionCntxtObserver::Setup2L()
    {
    iTestServiceId = CreateSpEntryL( KTestServiceId );
    iSessionCntxtObserver = 
        CVIMPSTEngineSessionCntxtObserver::NewL( iTestServiceId );
    
    
    delete iSessionCntxtObserver->iFeatures;
    iSessionCntxtObserver->iFeatures = NULL;
    delete iSessionCntxtObserver->iPresenceCtx;
    iSessionCntxtObserver->iPresenceCtx = NULL;
    delete iSessionCntxtObserver->iClient;
    iSessionCntxtObserver->iClient = NULL;
    
    iSessionCntxtObserver->iClient = MXIMPClientStub::NewClientL();;
    
    //Create new sink to receive ximp context events
    iSessionCntxtObserver->iPresenceCtx = 
        iSessionCntxtObserver->iClient->NewPresenceContextLC();    
    CleanupStack::Pop();// because of LC method         
    
    iSessionCntxtObserver->iFeatures = 
        MyMPresenceFeatureStub::NewStubL(
            iSessionCntxtObserver->iPresenceCtx); 
    }

void T_VimpstEngineSessionCntxtObserver::Teardown()
    {
    if ( iSessionCntxtObserver )
        {
        delete iSessionCntxtObserver;
        iSessionCntxtObserver = NULL;
        }
    }

TInt T_VimpstEngineSessionCntxtObserver::CreateSpEntryL( TInt aServiceId )
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

void T_VimpstEngineSessionCntxtObserver::T_NewLL()
    {
    iSessionCntxtObserver = 
        CVIMPSTEngineSessionCntxtObserver::NewL( iTestServiceId );
    
    EUNIT_ASSERT( iSessionCntxtObserver != NULL );
    }

void T_VimpstEngineSessionCntxtObserver::T_HandlePresenceContextEventL()
    {
    MXIMPContext* context = NULL;
    MXIMPRequestCompleteEventStub requestCompleteEvent;    
    iSessionCntxtObserver->HandlePresenceContextEvent( 
        *context, requestCompleteEvent );
    }

void T_VimpstEngineSessionCntxtObserver::T_GetCompletedReqResultL()
    {
    TInt result( KErrNotFound );  
    iSessionCntxtObserver->iReqResult = KErrNone;
    result = iSessionCntxtObserver->GetCompletedReqResult();
    EUNIT_ASSERT( KErrNone == result );
    }

void T_VimpstEngineSessionCntxtObserver::T_GetRequestMapperL()
    {
    CVIMPSTEngineRequestMapper* requestMapper =
        iSessionCntxtObserver->GetRequestMapper();
    EUNIT_ASSERT( requestMapper != NULL );
    }

void T_VimpstEngineSessionCntxtObserver::T_ServerBindL()
    {  
    TInt err( KErrNone );   
    TUid protocolUid( KNullUid );
    
    // Test with bind status EVIMPSTBindDone
    // -> error code shoud be KErrNotFound
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindDone;
    err = iSessionCntxtObserver->ServerBindL( protocolUid );
    EUNIT_ASSERT( KErrNotFound == err );
    
    // Test with other bind status that EVIMPSTBindDone
    // -> error code should be KErrNone
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindNotDone;
    err = iSessionCntxtObserver->ServerBindL( protocolUid );
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngineSessionCntxtObserver::T_ServerUnBindL()
    {
    TInt err( KErrNone );
    TBool doUnsubscribe( EFalse );
    
    // Test different combinations with bind status EVIMPSTBindDone and
    // binding leaving with KErrGeneral
    gUnBindingLeaves = KErrGeneral;
    
    doUnsubscribe = EFalse;
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindDone;  
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrGeneral == err );
    
    doUnsubscribe = ETrue;
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindDone;
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrGeneral == err );
    
    // Test different combinations with bind status EVIMPSTBindDone and
    // binding not leaving
    gUnBindingLeaves = KErrNone;
    
    doUnsubscribe = EFalse;
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindDone;
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrNone == err );
    
    doUnsubscribe = ETrue;
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindDone;
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrNone == err );
    
    // Test with bind status EVIMPSTBindFailureMode
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindFailureMode;
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrNotFound == err );
    
    // Test with some other bind status that EVIMPSTBindDone or EVIMPSTBindFailureMode
    iSessionCntxtObserver->iBindStatus = TVIMPSTEnums::EVIMPSTBindNotDone;
    err = iSessionCntxtObserver->ServerUnBindL( doUnsubscribe );
    EUNIT_ASSERT( KErrNotFound == err );
    }

void T_VimpstEngineSessionCntxtObserver::T_UriFromXimpOperationLCL()
    {
    MPresentityPresenceEventStub event;   
    HBufC* uri = iSessionCntxtObserver->UriFromXimpOperationLC( event );   
    EUNIT_ASSERT( uri != NULL )   
    //EUNIT_ASSERT( uri->Des().Compare( KTestIdentity ) == 0 );
    CleanupStack::PopAndDestroy( uri );
    }
void T_VimpstEngineSessionCntxtObserver::T_DoHandlePresenceContextEventLL()
    {
    TInt error( KErrNone );
    
    MXIMPContext* context = NULL;
    
    // Test with MXIMPRequestCompleteEventStub
    MXIMPRequestCompleteEventStub requestCompleteEvent;      
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, requestCompleteEvent ) );   
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }  
    EUNIT_ASSERT( KErrNone == error );
    
    // Test with MXIMPContextStateEventStub
    MXIMPContextStateEventStub contextStateEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, contextStateEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );
    
    // Test with MPresentityPresenceEventStub
    MPresentityPresenceEventStub presentityPresenceEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, presentityPresenceEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );
    
    // Test with MPresentityGroupContentEventStub
    MPresentityGroupContentEventStub presentityGroupContentEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, presentityGroupContentEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );
    
    // Test with MPresenceGrantRequestListEventStub
    MPresenceGrantRequestListEventStub presenceGrantRequestListEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, presentityGroupContentEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );
    
    // Test with MPresenceblockinfostub
    MPresenceBlockInfoStub presenceblockinfo;
      TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
          *context, presenceblockinfo ) );  
      if ( KErrNoMemory == error )
          {
          User::Leave( error );
          }   
      EUNIT_ASSERT( KErrNone == error );
    
    // Test with MSearchEventStub
    MSearchEventStub searchEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, searchEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );

    //Test with MSearchKeysEventStub
    MSearchKeysEventStub searchKeysEvent;
    TRAP( error, iSessionCntxtObserver->DoHandlePresenceContextEventL( 
        *context, searchKeysEvent ) );  
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }   
    EUNIT_ASSERT( KErrNone == error );
    }
void T_VimpstEngineSessionCntxtObserver::T_XimpAuthorizationLL()
    {
    MPresenceAuthorization& presenceAuthorization = 
        iSessionCntxtObserver->XimpAuthorizationL();
    }
void T_VimpstEngineSessionCntxtObserver::T_XimpPresenceWatchingLL()
    {
    TRAPD( err,
           MPresenceWatching& presenceWatching = 
               iSessionCntxtObserver->XimpPresenceWatchingL()
         );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err || KErrNotSupported == err );
    }
void T_VimpstEngineSessionCntxtObserver::T_XimpPresentityGroupsLL()
    {   
    TRAPD( err,
           MPresentityGroups& presentityGroups = 
               iSessionCntxtObserver->XimpPresentityGroupsL()
         );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err || KErrNotSupported == err );
    }
void T_VimpstEngineSessionCntxtObserver::T_XimpPresencePublishingLL()
    {
    TRAPD( err,
           MPresencePublishing& presencePublishing = 
               iSessionCntxtObserver->XimpPresencePublishingL()
          );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err || KErrNotSupported == err );
    }
void T_VimpstEngineSessionCntxtObserver::T_XimpPresenceContextL()
    {
    MXIMPContext& ximpContext = 
        iSessionCntxtObserver->XimpPresenceContextL();
    }
void T_VimpstEngineSessionCntxtObserver::T_PresenceObjectFactoryL()
    {
    MPresenceObjectFactory& presenceObjectFactory = 
        iSessionCntxtObserver->PresenceObjectFactoryL();
    }

void T_VimpstEngineSessionCntxtObserver::T_RegisterObserverL()
    {
    MVIMPSTEngineSessionCntxtObserver* observer = new dummy_observer; // chauhan
    iSessionCntxtObserver->RegisterObserver( observer );    
    EUNIT_ASSERT( iSessionCntxtObserver->iObserverArray.Count() == 1 );
    }

void T_VimpstEngineSessionCntxtObserver::T_UnRegisterObserverL()
    {
    MVIMPSTEngineSessionCntxtObserver* observer = new dummy_observer; // chauhan
    iSessionCntxtObserver->RegisterObserver( observer );    
    EUNIT_ASSERT( iSessionCntxtObserver->iObserverArray.Count() == 1 );
    
    iSessionCntxtObserver->UnRegisterObserver( observer );
    EUNIT_ASSERT( iSessionCntxtObserver->iObserverArray.Count() == 0 );
    }

void T_VimpstEngineSessionCntxtObserver::T_IdentifySupportedFeaturesL()
    {
    iSessionCntxtObserver->IdentifySupportedFeaturesL();
    }

void T_VimpstEngineSessionCntxtObserver::T_GetSupportedFeaturesL()
    {
    TInt supportedFeatures = 
        iSessionCntxtObserver->GetSupportedFeatures();
    }






EUNIT_BEGIN_TEST_TABLE( T_VimpstEngineSessionCntxtObserver, "T_VimpstEngineSessionCntxtObserver", "" )

EUNIT_TEST(
    "NewL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "NewL",
    "FUNCTIONALITY",
    Setup1L, T_NewLL, Teardown)
    
EUNIT_TEST(
    "HandlePresenceContextEvent - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "HandlePresenceContextEvent",
    "FUNCTIONALITY",
    Setup2L, T_HandlePresenceContextEventL, Teardown)
    
EUNIT_TEST(
    "GetCompletedReqResult - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "GetCompletedReqResult",
    "FUNCTIONALITY",
    Setup2L, T_GetCompletedReqResultL, Teardown)
    
EUNIT_TEST(
    "GetRequestMapper - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "GetRequestMapper",
    "FUNCTIONALITY",
    Setup2L, T_GetRequestMapperL, Teardown)
    
EUNIT_TEST(
    "ServerBind - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "ServerBind",
    "FUNCTIONALITY",
    Setup2L, T_ServerBindL, Teardown)
    
EUNIT_TEST(
    "ServerUnBind - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "ServerUnBind",
    "FUNCTIONALITY",
    Setup2L, T_ServerUnBindL, Teardown)
    
EUNIT_TEST(
    "UriFromXimpOperationLC - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "UriFromXimpOperationLC",
    "FUNCTIONALITY",
    Setup2L, T_UriFromXimpOperationLCL, Teardown)
    
EUNIT_TEST(
    "DoHandlePresenceContextEventL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "DoHandlePresenceContextEventL",
    "FUNCTIONALITY",
    Setup2L, T_DoHandlePresenceContextEventLL, Teardown)
    
EUNIT_TEST(
    "XimpAuthorizationL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "XimpAuthorizationL",
    "FUNCTIONALITY",
    Setup2L, T_XimpAuthorizationLL, Teardown)
    
EUNIT_TEST(
    "XimpPresenceWatchingL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "XimpPresenceWatchingL",
    "FUNCTIONALITY",
    Setup2L, T_XimpPresenceWatchingLL, Teardown)
    
EUNIT_TEST(
    "XimpPresentityGroupsL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "XimpPresentityGroupsL",
    "FUNCTIONALITY",
    Setup2L, T_XimpPresentityGroupsLL, Teardown)
    
EUNIT_TEST(
    "XimpPresencePublishingL - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "XimpPresencePublishingL",
    "FUNCTIONALITY",
    Setup2L, T_XimpPresencePublishingLL, Teardown)
    
EUNIT_TEST(
    "XimpPresenceContext - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "XimpPresenceContext",
    "FUNCTIONALITY",
    Setup2L, T_XimpPresenceContextL, Teardown)
    
EUNIT_TEST(
    "PresenceObjectFactory - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "PresenceObjectFactory",
    "FUNCTIONALITY",
    Setup2L, T_PresenceObjectFactoryL, Teardown)
    
EUNIT_TEST(
    "RegisterObserver - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "RegisterObserver",
    "FUNCTIONALITY",
    Setup2L, T_RegisterObserverL, Teardown)
    
EUNIT_TEST(
    "UnRegisterObserver - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "UnRegisterObserver",
    "FUNCTIONALITY",
    Setup2L, T_UnRegisterObserverL, Teardown)
    
EUNIT_TEST(
    "IdentifySupportedFeatures - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "IdentifySupportedFeatures",
    "FUNCTIONALITY",
    Setup2L, T_IdentifySupportedFeaturesL, Teardown)
    
EUNIT_TEST(
    "GetSupportedFeatures - test ",
    "T_VimpstEngineSessionCntxtObserver",
    "GetSupportedFeatures",
    "FUNCTIONALITY",
    Setup2L, T_GetSupportedFeaturesL, Teardown)


EUNIT_END_TESTCASE_TABLE

//  END OF FILE
