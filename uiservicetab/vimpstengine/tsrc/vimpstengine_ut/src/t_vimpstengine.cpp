/*
* t_vimpstengine.cpp  Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:t_vimpstengine.cpp
*
*/
#include <platform/digia/eunit/eunitmacros.h>
#include <platform/digia/eunit/teunitassertioninfo.h>
#include <platform/digia/eunit/CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstengine.h"

#include <spentry.h>
#include <spsettings.h>
#include <spproperty.h>

#include "cvimpstengine.h"
#include "cvimpstengineservicetablefetcher.h"
#include "mvimpstenginecontactmgmtextention.h"
#include "mvimpstenginesearchmgrextention.h"
#include "mvimpstengineextentionfeatures.h"
#include "cvimpstenginesessioncntxtobserver.h"
//#include "cvimpstenginesearchmgrextention.h"
#include "cvimpstenginecontactmgmtextention.h"
#include "mvimpstenginesubservice.h"

#include "s_cch.h"

extern MyCchService* myService;
extern TBool createService ;
extern CSPSettings* myCSPSettings;

extern TBool getUserName;
extern TBool getPassword;

const TInt KTestServiceId = 4;
_LIT( KTestServiceName, "TEST_SERVICE" );
//_LIT8( KTestBrandId, "TEST_BRAND_ID" );

_LIT( KTestInternetTelephoneField, "Internet telephone" );

T_VimpstEngine* T_VimpstEngine::NewLC()
    {
    T_VimpstEngine* self = new( ELeave ) T_VimpstEngine;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VimpstEngine::~T_VimpstEngine()
    {
    }

void T_VimpstEngine::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void T_VimpstEngine::SetupL()
    {
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();
    
    createService = ETrue;
    myCSPSettings = CSPSettings::NewL();
    iTestServiceId = CreateSpEntryL( KTestServiceId );
    
    iEngine = CVIMPSTEngine::NewL( 
        iTestServiceId, *iServiceTableFetcher );
    }    

void T_VimpstEngine::Teardown()
    {    
    if ( iServiceTableFetcher )
        {
        delete iServiceTableFetcher;
        iServiceTableFetcher = NULL;
        }   
    if ( iEngine )
        {
        delete iEngine;
        iEngine = NULL;
        }
    
    if ( myService )
        {
        delete myService;
        myService = NULL;  
        }
  
    if ( myCSPSettings )
        {
        delete myCSPSettings;
        myCSPSettings = NULL;
        }
    
    createService = EFalse;
    getUserName = EFalse;
    getPassword = EFalse;
    }


// FROM OBSERVERS

void T_VimpstEngine::HandleServiceEventL( 
    TVIMPSTEnums::TVIMPSTRegistrationState /*aState*/, 
    TInt /*aServiceError*/ )
    {
    
    }



// HELP METHODS

TInt T_VimpstEngine::CreateSpEntryL( TInt aServiceId )
    {    
    CSPEntry* entry = CSPEntry::NewLC();
    entry->SetServiceId( aServiceId );
    User::LeaveIfError( entry->SetServiceName( KTestServiceName ) );
    
    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = myCSPSettings->FindEntryL( entry->GetServiceId(), *testEntry );
    
    if( result == KErrNone )
        {
        myCSPSettings->UpdateEntryL( *entry );
        }
    else
        {
        myCSPSettings->AddEntryL( *entry );
        }
    
    CSPProperty* property = NULL;
   
    // set voip enabled property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyVoIPEnabled );
    property->SetValue( EOn );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );
    
    // set presence settings id property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyPresenceSettingsId );
    property->SetValue( 1 );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );
    
    // set im settings id property
    property = CSPProperty::NewLC();
    property->SetName( ESubPropertyIMSettingsId );
    property->SetValue( 1 );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );
    
    // set Presence FW PSC plugin UID
    property = CSPProperty::NewLC();
    property->SetName( EPropertyPCSPluginId );
    property->SetValue( 1 );
    entry->AddPropertyL( *property );
    CleanupStack::PopAndDestroy( property );
    
    myCSPSettings->UpdateEntryL( *entry ); 
    TInt createdServiceId = entry->GetServiceId();
    
    CleanupStack::PopAndDestroy( testEntry );
    CleanupStack::PopAndDestroy( entry );
    
    return createdServiceId;
    } 



// TEST CASES

void T_VimpstEngine::T_NewLL()
    {  
    EUNIT_ASSERT( iEngine != NULL );
    }

void T_VimpstEngine::T_LoginL()
    {  
   // iEngine->iCchHandler = mycch_stub::N
    TInt err = iEngine->Login();   
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngine::T_LogoutL()
    {  
    iEngine->LogoutL();   
    }

void T_VimpstEngine::T_ServiceIdL()
    {  
    TUint32 serviceId( KErrNone );
    serviceId = iEngine->ServiceId();
    EUNIT_ASSERT( iTestServiceId == serviceId );
    }

void T_VimpstEngine::T_ServiceNameL()
    {  
    RBuf serviceName;
    CleanupClosePushL( serviceName );
    serviceName.CreateL( 15 );
    serviceName.Copy( iEngine->ServiceName() );   
    EUNIT_ASSERT( serviceName.Compare( KTestServiceName ) == 0 );
    CleanupStack::PopAndDestroy( &serviceName );
    }

void T_VimpstEngine::T_ServiceStateL()
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
        iEngine->iState = states[ i ];
        TVIMPSTEnums::TVIMPSTRegistrationState state = 
            iEngine->ServiceState();
        EUNIT_ASSERT( state == states[ i ] );
        }
    
    CleanupStack::PopAndDestroy( &states );
    }

void T_VimpstEngine::T_GetBrandInfoLL()
    {  
    TLanguage language;
    TInt version( KErrNone );
    RBuf8 brandId;
    CleanupClosePushL( brandId );
    brandId.CreateL( 20 );
    
    TRAPD( err, iEngine->GetBrandInfoL( 
        language, version, brandId ) );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    
    CleanupStack::PopAndDestroy( &brandId );
    }

void T_VimpstEngine::T_IsSubServiceSupportedLL()
    {  
    RArray<TVIMPSTEnums::SubServiceType> types;
    CleanupClosePushL( types );
    
    types.AppendL( TVIMPSTEnums::EVoip );
    types.AppendL( TVIMPSTEnums::EPresence );
    types.AppendL( TVIMPSTEnums::EIM );
    types.AppendL( TVIMPSTEnums::ELast );

    for ( TInt i( 0 ) ; i < types.Count() ; i++ )
        {
        TBool supported( EFalse );
        TRAPD( err, supported =  
            iEngine->IsSubServiceSupportedL( types[ i ] ) );
        
        if ( KErrNoMemory == err )
            {
            User::Leave( err );
            }
        
        EUNIT_ASSERT( KErrNone == err );
        EUNIT_ASSERT( ETrue == supported || EFalse == supported );
        }
    
    CleanupStack::PopAndDestroy( &types );
    }

void T_VimpstEngine::T_IsSubServiceEnabledL()
    {  
    RArray<TVIMPSTEnums::SubServiceType> types;
    CleanupClosePushL( types );
       
    types.AppendL( TVIMPSTEnums::EVoip );
    types.AppendL( TVIMPSTEnums::EPresence );
    types.AppendL( TVIMPSTEnums::EIM );
    types.AppendL( TVIMPSTEnums::ELast );

    for ( TInt i( 0 ) ; i < types.Count() ; i++ )
        {
        TBool enabled( EFalse );
        TRAPD( err, enabled =  
            iEngine->IsSubServiceEnabled( types[ i ] ) );
           
        if ( KErrNoMemory == err )
            {
            User::Leave( err );
            }
           
        EUNIT_ASSERT( KErrNone == err );
        EUNIT_ASSERT( ETrue == enabled || EFalse == enabled );
        }
       
    CleanupStack::PopAndDestroy( &types );
    }

void T_VimpstEngine::T_ContactStoreIdLL()
    {  
    RBuf contactStoreId;
    CleanupClosePushL( contactStoreId );
    contactStoreId.CreateL( 50 );
    
    TRAPD( err, iEngine->ContactStoreIdL( contactStoreId ) );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    
    CleanupStack::PopAndDestroy( &contactStoreId );
    }

void T_VimpstEngine::T_RegisterServiceSessionObserverLL()
    {  
    TInt err( KErrNone );
    
    // Register observer
    TRAP( err, iEngine->RegisterServiceSessionObserverL( this ) );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
        
    EUNIT_ASSERT( KErrNone == err );
    
    // Try to register same observer again
    TRAP( err, iEngine->RegisterServiceSessionObserverL( this ) );
       
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngine::T_UnRegisterServiceSessionObserverLL()
    {
    TInt err( KErrNone );
    
    // Register observer to be unregistered
    TRAP( err, iEngine->RegisterServiceSessionObserverL( this ) );
       
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    
    // Unregister observer
    TRAP( err, iEngine->UnRegisterServiceSessionObserver( this ) );
        
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
            
    EUNIT_ASSERT( KErrNone == err );
    
    // Try to unregister observer again
    TRAP( err, iEngine->UnRegisterServiceSessionObserver( this ) );
        
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
            
    EUNIT_ASSERT( KErrNone == err )
    }

void T_VimpstEngine::T_IntializeStorageLL()
    {  
    TRAPD( err, iEngine->IntializeStorageL() );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
            
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngine::T_UnIntializeStorageLL()
    {  
    iEngine->UnIntializeStorage();
    }

/*void T_VimpstEngine::T_SetUserNameL()
    {  
    TInt err( KErrNone );
    //err = iEngine->SetUserName( KTestUserName );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngine::T_SetPasswordL()
    {  
    TInt err( KErrNone );
    //rr = iEngine->SetPassword( KTestPassword );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( KErrNone == err );
    }
*/
/*
void T_VimpstEngine::T_UserNameLL()
    {  
    getUserName = ETrue;
    
    HBufC* userName = NULL;
    TRAPD( err, userName = iEngine->UserNameL() );
    CleanupStack::PushL( userName );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( userName->Des().Compare( KTestUserName ) == 0 );
    
    CleanupStack::PopAndDestroy( userName );
    }*/
/*
void T_VimpstEngine::T_PasswordLL()
    {  
    getPassword = ETrue;
    
    HBufC* passWord = NULL;      
    TRAPD( err, passWord = iEngine->PasswordL() );
    CleanupStack::PushL( passWord );
       
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( passWord->Des().Compare( KTestPassword ) == 0 );
       
    CleanupStack::PopAndDestroy( passWord );
    }*/

void T_VimpstEngine::T_SubServiceL()
    {  
    RArray<TVIMPSTEnums::SubServiceType> types;
    CleanupClosePushL( types );
          
    types.AppendL( TVIMPSTEnums::EVoip );
    types.AppendL( TVIMPSTEnums::EPresence );
    types.AppendL( TVIMPSTEnums::EIM );
    types.AppendL( TVIMPSTEnums::ELast );
    
    for ( TInt i( 0 ) ; i < types.Count() ; i++ )
        {
        MVIMPSTEngineSubService* subservice = 
            iEngine->SubService( types[ i ] );
        }
    
    CleanupStack::PopAndDestroy( &types );
    }

void T_VimpstEngine::T_ExtentionFeaturesL()
    { 
    // Try when no extension features exist
    MVIMPSTEngineSearchMgrExtention* searchMgr = 
        dynamic_cast<MVIMPSTEngineSearchMgrExtention*> 
        (iEngine->ExtentionFeatures(TVIMPSTEnums::ESearch));
        
    MVIMPSTEngineContactMgmtExtention* contactMgr = 
        dynamic_cast<MVIMPSTEngineContactMgmtExtention*> 
        (iEngine->ExtentionFeatures(
            TVIMPSTEnums::EContactManagement));
        
    iEngine->ExtentionFeatures(
        TVIMPSTEnums::EBlocking);
    
    iEngine->ExtentionFeatures(
        TVIMPSTEnums::EExtentionLast);
    
    // Test again after creating extension features
    TRAPD( err, iEngine->CreateExtentionFeaturesL() );
       
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
              
    EUNIT_ASSERT( KErrNone == err );
    
    searchMgr = NULL;
    contactMgr = NULL;
    
    searchMgr = 
        dynamic_cast<MVIMPSTEngineSearchMgrExtention*> 
        (iEngine->ExtentionFeatures(TVIMPSTEnums::ESearch));
            
    contactMgr = 
        dynamic_cast<MVIMPSTEngineContactMgmtExtention*> 
        (iEngine->ExtentionFeatures(
            TVIMPSTEnums::EContactManagement));
            
    iEngine->ExtentionFeatures(
        TVIMPSTEnums::EBlocking);
        
    iEngine->ExtentionFeatures(
        TVIMPSTEnums::EExtentionLast);
    }

void T_VimpstEngine::T_AddExtentionFeaturesLL()
    {  
    /*CVIMPSTEngineSessionCntxtObserver* observer = 
        CVIMPSTEngineSessionCntxtObserver::NewLC( 
            iTestServiceId );
    
    CVIMPSTEngineSearchMgrExtention* searchitem = 
        CVIMPSTEngineSearchMgrExtention::NewLC(
             observer->XimpPresenceContextL(),
            *observer->GetRequestMapper());
    
    TRAPD( err, iEngine->AddExtentionFeaturesL( searchitem ) );    
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    
    CleanupStack::PopAndDestroy( searchitem );
    CleanupStack::PopAndDestroy( observer );*/
    }

void T_VimpstEngine::T_RemoveExtentionFeaturesL()
    {  
    RArray<TVIMPSTEnums::ExtentionType> types;
    CleanupClosePushL( types );
                
    // Add feature that can be removed
    CVIMPSTEngineSessionCntxtObserver* observer = 
       CVIMPSTEngineSessionCntxtObserver::NewLC( 
           iTestServiceId );
       
   /* CVIMPSTEngineSearchMgrExtention* searchitem = 
       CVIMPSTEngineSearchMgrExtention::NewLC(
            observer->XimpPresenceContextL(),
            *observer->GetRequestMapper());
       
    TRAPD( err, iEngine->AddExtentionFeaturesL( searchitem ) );    
       
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
       
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( iEngine->iExtentionFeatures.Count() == 1 );   
   */  
    
    // Test with all types
   // types.AppendL( TVIMPSTEnums::ESearch );
    //types.AppendL( TVIMPSTEnums::EBlocking );
    types.AppendL( TVIMPSTEnums::EContactManagement );
    types.AppendL( TVIMPSTEnums::EExtentionLast );
    
    for ( TInt i( 0 ) ; i < types.Count() ; i++ )
        {
        iEngine->RemoveExtentionFeatures( types[ i ] );
        }
    
    // Created feature should be removed --> count = 0
    EUNIT_ASSERT( iEngine->iExtentionFeatures.Count() == 0 ); 
    
    //CleanupStack::PopAndDestroy( searchitem );
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( &types );
    }

void T_VimpstEngine::T_HandleServceConnectionEventLL()
    {  
    TRAPD( err, iEngine->HandleServceConnectionEventL() );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    }

void T_VimpstEngine::T_ParseGetServiceStateL()
    {  
    RArray<TVIMPSTEnums::TVIMPSTRegistrationState> states;
    CleanupClosePushL( states ); 
    
    states.AppendL( TVIMPSTEnums::ESVCENotRegistered );
    states.AppendL( TVIMPSTEnums::ESVCERegistered );
    states.AppendL( TVIMPSTEnums::ESVCENetworkConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEWaitingForNetwork );
    states.AppendL( TVIMPSTEnums::ESVCENetworkDisConnecting );
    states.AppendL( TVIMPSTEnums::ESVCEUpdatingContacts );
    
    TVIMPSTEnums::TVIMPSTRegistrationState state;
    state = iEngine->ParseGetServiceState();   
    EUNIT_ASSERT( state == TVIMPSTEnums::ESVCENotRegistered );
    
    // TBD: test with different states
    
    CleanupStack::PopAndDestroy( &states );
    }

void T_VimpstEngine::T_HandleContactFetchedLL()
    {  
    TRAPD( err, iEngine->HandleContactFetchedL() );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    }
/*
void T_VimpstEngine::T_HandleContactAddedLL()
    {  
    RPointerArray<HBufC> serviceDetails;    
    CDesCArray* array = new (ELeave) CDesCArrayFlat( 1 );
    CleanupStack::PushL( array );
    
    array->AppendL( KTestInternetTelephoneField );    
    const CDesCArray& constArray = *array;
    
    for ( TInt i( 0 ) ; i < constArray.MdcaCount() ; i++ )
        {
        serviceDetails.InsertInOrder( constArray[ i ].Alloc(),0);
        }
    
    TRAPD( err, iEngine->HandleContactAddedL( serviceDetails ) );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    
    EUNIT_ASSERT( KErrNone == err );
    
    CleanupStack::PopAndDestroy( array );
    serviceDetails.ResetAndDestroy();
    }*/
/*
void T_VimpstEngine::T_HandleContactDeletedLL()
    {  
  //  RPointerArray<HBufC> serviceDetails;
    MVIMPSTStorageContact* aStorageContact;
    
    //TRAPD( err, iEngine->HandleContactDeletedL( serviceDetails ) );
    
    TRAPD( err, iEngine->HandleContactDeletedL( *aStorageContact ) );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
        
    EUNIT_ASSERT( KErrNone == err );
    
    //serviceDetails.ResetAndDestroy();
    
    }
*/
void T_VimpstEngine::T_CreateExtentionFeaturesLL()
    {  
    TRAPD( err, iEngine->CreateExtentionFeaturesL() );
    
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }
/*
void T_VimpstEngine::T_EmptySetUserNameL()
    {  
    TRAPD( err, iEngine->SetUserNameL(KNullDesC()) );
    
    if ( KErrNone != err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }
*/
/*
void T_VimpstEngine::T_SetUserNameL()
    {  
    TRAPD( err, iEngine->SetUserNameL(_L("testui")) );
    
    if ( KErrNone != err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }
*/
/*
void T_VimpstEngine::T_WithDomainNameSetUserNameL()
    {  
    TRAPD( err, iEngine->SetUserNameL(_L("testui123@gmail.com")) );
    
    if ( KErrNone != err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }
*/
/*
void T_VimpstEngine::T_EmptySetPassword()
    {  
    TRAPD( err, iEngine->SetPassword(KNullDesC()) );
    
    if (KErrNone != err )
        {
        User::Leave( err );
        }
           
    EUNIT_ASSERT( KErrNone == err );
    }
*/
/*
void T_VimpstEngine::T_SetPassword()
    {  
    TRAPD( err, iEngine->SetPassword(_L("test_ui123")) );
    
    if (KErrNone != err )
            {
            User::Leave( err );
            }
               
    EUNIT_ASSERT( KErrNone == err );
    }
*/
void T_VimpstEngine::T_BlockNotSupported()
    {
    TBool isBlockSupported = iEngine->IsBlockSupportedL();
    EUNIT_ASSERT( EFalse == isBlockSupported );
    }
    
void T_VimpstEngine::T_FetchBlockedListL()
    {
    TInt err = KErrNone;
    TRAP(err,iEngine->FetchBlockedListL())
    EUNIT_ASSERT( KErrNone == err);
    }

void T_VimpstEngine::T_RegisterBlockedListObserver()
    {
    TInt err = KErrNone;
    iEngine->RegisterBlockedListObserver(NULL);
    EUNIT_ASSERT( NULL == iEngine->iBlockListObs );
    }
    

void T_VimpstEngine::T_BlockSupported()
    {
    
    iEngine->iSessionCntxtObserver =  CVIMPSTEngineSessionCntxtObserver::NewL(3);
    TBool isBlockSupported = iEngine->IsBlockSupportedL();
    EUNIT_ASSERT( EFalse == isBlockSupported );
    }

EUNIT_BEGIN_TEST_TABLE( T_VimpstEngine, "T_VimpstEngine", "" )

EUNIT_TEST(
    "NewL - test ",
    "T_VimpstEngine",
    "NewL",
    "FUNCTIONALITY",
    SetupL, T_NewLL, Teardown)
    
EUNIT_TEST(
    "Login - test ",
    "T_VimpstEngine",
    "Login",
    "FUNCTIONALITY",
    SetupL, T_LoginL, Teardown)
    
EUNIT_TEST(
    "Logout - test ",
    "T_VimpstEngine",
    "Logout",
    "FUNCTIONALITY",
    SetupL, T_LogoutL, Teardown)
    
EUNIT_TEST(
    "ServiceId - test ",
    "T_VimpstEngine",
    "ServiceId",
    "FUNCTIONALITY",
    SetupL, T_ServiceIdL, Teardown)
        
EUNIT_TEST(
    "ServiceName - test ",
    "T_VimpstEngine",
    "ServiceName",
    "FUNCTIONALITY",
    SetupL, T_ServiceNameL, Teardown)
    
EUNIT_TEST(
    "ServiceState - test ",
    "T_VimpstEngine",
    "ServiceState",
    "FUNCTIONALITY",
    SetupL, T_ServiceStateL, Teardown)
    
EUNIT_TEST(
    "GetBrandInfoL - test ",
    "T_VimpstEngine",
    "GetBrandInfoL",
    "FUNCTIONALITY",
    SetupL, T_GetBrandInfoLL, Teardown)
    
EUNIT_TEST(
    "IsSubServiceSupportedL - test ",
    "T_VimpstEngine",
    "IsSubServiceSupportedL",
    "FUNCTIONALITY",
    SetupL, T_IsSubServiceSupportedLL, Teardown)
    
EUNIT_TEST(
    "IsSubServiceEnabled - test ",
    "T_VimpstEngine",
    "IsSubServiceEnabled",
    "FUNCTIONALITY",
    SetupL, T_IsSubServiceEnabledL, Teardown)
    
EUNIT_TEST(
    "ContactStoreIdL - test ",
    "T_VimpstEngine",
    "ContactStoreIdL",
    "FUNCTIONALITY",
    SetupL, T_ContactStoreIdLL, Teardown)
    
EUNIT_TEST(
    "RegisterServiceSessionObserverL",
    "T_VimpstEngine",
    "RegisterServiceSessionObserverL",
    "FUNCTIONALITY",
    SetupL, T_RegisterServiceSessionObserverLL, Teardown)
    
EUNIT_TEST(
    "UnRegisterServiceSessionObserverL - test ",
    "T_VimpstEngine",
    "UnRegisterServiceSessionObserverL",
    "FUNCTIONALITY",
    SetupL, T_UnRegisterServiceSessionObserverLL, Teardown)
    
EUNIT_TEST(
    "IntializeStorageL - test ",
    "T_VimpstEngine",
    "IntializeStorageL",
    "FUNCTIONALITY",
    SetupL, T_IntializeStorageLL, Teardown)
    
EUNIT_TEST(
    "UnIntializeStorageL - test ",
    "T_VimpstEngine",
    "UnIntializeStorageL",
    "FUNCTIONALITY",
    SetupL, T_UnIntializeStorageLL, Teardown)
    
/*EUNIT_TEST(
    "SetUserName - test ",
    "T_VimpstEngine",
    "SetUserName",
    "FUNCTIONALITY",
    SetupL, T_SetUserNameL, Teardown)
    
EUNIT_TEST(
    "SetPassword - test ",
    "T_VimpstEngine",
    "SetPassword",
    "FUNCTIONALITY",
    SetupL, T_SetPasswordL, Teardown)
  */ 
 /*   
EUNIT_TEST(
    "UserNameL - test ",
    "T_VimpstEngine",
    "UserNameL",
    "FUNCTIONALITY",
    SetupL, T_UserNameLL, Teardown)
    
EUNIT_TEST(
    "PasswordL - test ",
    "T_VimpstEngine",
    "PasswordL",
    "FUNCTIONALITY",
    SetupL, T_PasswordLL, Teardown)
  */  
EUNIT_TEST(
    "SubService - test ",
    "T_VimpstEngine",
    "SubService",
    "FUNCTIONALITY",
    SetupL, T_SubServiceL, Teardown)
    
EUNIT_TEST(
    "ExtentionFeatures - test ",
    "T_VimpstEngine",
    "ExtentionFeatures",
    "FUNCTIONALITY",
    SetupL, T_ExtentionFeaturesL, Teardown)
    
EUNIT_TEST(
    "AddExtentionFeaturesL - test ",
    "T_VimpstEngine",
    "AddExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_AddExtentionFeaturesLL, Teardown)
    
EUNIT_TEST(
    "RemoveExtentionFeatures - test ",
    "T_VimpstEngine",
    "RemoveExtentionFeatures",
    "FUNCTIONALITY",
    SetupL, T_RemoveExtentionFeaturesL, Teardown)
    
EUNIT_TEST(
    "HandleServceConnectionEventL - test ",
    "T_VimpstEngine",
    "HandleServceConnectionEventL",
    "FUNCTIONALITY",
    SetupL, T_HandleServceConnectionEventLL, Teardown)
    
EUNIT_TEST(
    "ParseGetServiceState - test ",
    "T_VimpstEngine",
    "ParseGetServiceState",
    "FUNCTIONALITY",
    SetupL, T_ParseGetServiceStateL, Teardown)
    
EUNIT_TEST(
    "HandleContactFetchedL - test ",
    "T_VimpstEngine",
    "HandleContactFetchedL",
    "FUNCTIONALITY",
    SetupL, T_HandleContactFetchedLL, Teardown)
/*    
EUNIT_TEST(
    "HandleContactAddedL - test ",
    "T_VimpstEngine",
    "HandleContactAddedL",
    "FUNCTIONALITY",
    SetupL, T_HandleContactAddedLL, Teardown)
    
EUNIT_TEST(
    "HandleContactDeletedL - test ",
    "T_VimpstEngine",
    "HandleContactDeletedL",
    "FUNCTIONALITY",
    SetupL, T_HandleContactDeletedLL, Teardown)
 */   
EUNIT_TEST(
    "CreateExtentionFeaturesL - test ",
    "T_VimpstEngine",
    "CreateExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_CreateExtentionFeaturesLL, Teardown)

/*
EUNIT_TEST(
    "EmptySetPassword - test ",
    "T_VimpstEngine",
    "CreateExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_EmptySetPassword, Teardown)*/
/*
EUNIT_TEST(
    "EmptySetUserNameL - test ",
    "T_VimpstEngine",
    "CreateExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_EmptySetUserNameL, Teardown)

EUNIT_TEST(
    "SetUserNameL - test ",
    "T_VimpstEngine",
    "CreateExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_SetUserNameL, Teardown)

EUNIT_TEST(
    "WithDomainNameSetUserNameL - test ",
    "T_VimpstEngine",
    "CreateExtentionFeaturesL",
    "FUNCTIONALITY",
    SetupL, T_WithDomainNameSetUserNameL, Teardown)
  */  
EUNIT_TEST(
    "BlockSupported - test ",
    "T_VimpstEngine",
    "BlockSupported",
    "FUNCTIONALITY",
    SetupL, T_BlockSupported, Teardown)

EUNIT_TEST(
    "BlockNotSupported - test ",
    "T_VimpstEngine",
    "BlockNotSupported",
    "FUNCTIONALITY",
    SetupL, T_BlockNotSupported, Teardown)

EUNIT_TEST(
    "FetchBlockedList - test ",
    "T_FetchBlockedListL",
    "FetchBlockedList",
    "FUNCTIONALITY",
    SetupL, T_FetchBlockedListL, Teardown)

EUNIT_TEST(
    "RegisterBlockedListObserver - test ",
    "T_RegisterBlockedListObserver",
    "RegisterBlockedListObserver",
    "FUNCTIONALITY",
    SetupL, T_RegisterBlockedListObserver , Teardown)
   
EUNIT_END_TESTCASE_TABLE

//  END OF FILE
