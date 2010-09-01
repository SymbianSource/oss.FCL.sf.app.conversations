/*
* t_vimpstengineservicetablefetcher.cpp Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: t_vimpstengineservicetablefetcher.cpp
*
*/
#include <EUnitMacros.h>
#include <TEUnitAssertionInfo.h>
#include <CEUnitAllocTestCaseDecorator.h>
#include "t_vimpstengineservicetablefetcher.h"

#include "cvimpstengineservicetablefetcher.h"
#include "vimpstextentionuiuid.h"

#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>

#include "s_cch.h"

#include "tvimpstenums.h"

extern TBool createService ;
extern TBool tryPresenceSubServiceAvailable ;
extern MyCchService* myService;

_LIT( KDbName, "contacts.cdb");
_LIT( KDbNameURI, "cntdb://c:contacts.cdb");
_LIT( KServiceName, "TestService");


T_VIMPSTEngineServiceTableFetcher* T_VIMPSTEngineServiceTableFetcher::NewLC()
    {
    T_VIMPSTEngineServiceTableFetcher* self = new( ELeave ) T_VIMPSTEngineServiceTableFetcher;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_VIMPSTEngineServiceTableFetcher::~T_VIMPSTEngineServiceTableFetcher()
    {
    }

void T_VIMPSTEngineServiceTableFetcher::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void T_VIMPSTEngineServiceTableFetcher::SetupL()
    {    
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();
    iSpSettings = CSPSettings::NewL();
    }    

void T_VIMPSTEngineServiceTableFetcher::Setup1L()
    {        
    }
    
void T_VIMPSTEngineServiceTableFetcher::Teardown()
    {
    delete myService;
    myService = NULL;    
    createService = EFalse;    
    tryPresenceSubServiceAvailable = EFalse;
	
    delete iServiceTableFetcher;
    //delete iSpSettings; only 1 instance of SPSettings is created this will be deleted by iServiceTableFetcher
    }


void T_VIMPSTEngineServiceTableFetcher::TestConstructionL()
    {
    
    EUNIT_ASSERT( iServiceTableFetcher );
    
    }


void T_VIMPSTEngineServiceTableFetcher::TestConstructionWithValidServicesL()
    {
    
    iSpSettings = CSPSettings::NewL();
    
    createService = ETrue;
	tryPresenceSubServiceAvailable = ETrue; //atleast 1 subservice is valid
	
	CreateSpEntryL( 1, EPropertyContactViewPluginId, KIMCONTACTSEXTNIMPLEMENTATIONUID );
	
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();    
    
    EUNIT_ASSERT( iServiceTableFetcher );
    
    }


void T_VIMPSTEngineServiceTableFetcher::TestConstructionWithInValidServicesL()
    {
    
    iSpSettings = CSPSettings::NewL();
    
	CreateSpEntryL( 1, EPropertyContactViewPluginId, KIMCONTACTSEXTNIMPLEMENTATIONUID );
	
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();    
    
    EUNIT_ASSERT( iServiceTableFetcher );
    
    }



void T_VIMPSTEngineServiceTableFetcher::GetMasterServiceIdsNoServicesL()
	{
	RArray<TUint32> services;
	
	iServiceTableFetcher->GetMasterServiceIdsL(services);
	
	EUNIT_ASSERT_EQUALS( 0, services.Count() ); 
	
	services.Close();			
	}
	

void T_VIMPSTEngineServiceTableFetcher::GetMasterServiceIdsServicesMatchContactViewUidInValidServiceL()
	{                                    
	RArray<TUint32> services;
	
	CreateSpEntryL( 1, EPropertyContactViewPluginId, KIMCONTACTSEXTNIMPLEMENTATIONUID );
	
	iServiceTableFetcher->GetMasterServiceIdsL(services);
		
	EUNIT_ASSERT_EQUALS( 1, services.Count() ); 
	
	services.Close();			
	}	

void T_VIMPSTEngineServiceTableFetcher::GetMasterServiceIdsServicesMatchContactViewUidValidServiceL()
	{                                    
	RArray<TUint32> services;
	
	createService = ETrue;
	tryPresenceSubServiceAvailable = ETrue; //atleast 1 subservice is valid
	
	CreateSpEntryL( 1, EPropertyContactViewPluginId, KIMCONTACTSEXTNIMPLEMENTATIONUID );
	
	iServiceTableFetcher->GetMasterServiceIdsL(services);
		
	EUNIT_ASSERT_EQUALS( 1, services.Count() ); 
	
	services.Close();			
	}	
	

void T_VIMPSTEngineServiceTableFetcher::GetMasterServiceIdsServicesNoMatchContactViewUidL()
	{
	RArray<TUint32> services;
	
	CreateSpEntryL( 1, EPropertyContactViewPluginId, 0x20010000 );
	
	iServiceTableFetcher->GetMasterServiceIdsL(services);
		
	EUNIT_ASSERT_EQUALS( 0, services.Count() ); 
	
	services.Close();			
	}	
	
void T_VIMPSTEngineServiceTableFetcher::CreateSpEntryL( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TInt aValue )
    {
    CSPProperty* property = CSPProperty::NewL();
    CleanupStack::PushL( property );
    property->SetName( aName );
    property->SetValue( aValue );

    CSPEntry* entry = CSPEntry::NewL();
    CleanupStack::PushL( entry );
    entry->SetServiceId( aServiceId );
    entry->SetServiceName(KServiceName() );

    entry->AddPropertyL( *property );

    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = iSpSettings->FindEntryL( entry->GetServiceId(), *testEntry );
    if( result == KErrNone )
        {
        iSpSettings->UpdateEntryL( *entry );
        }
    else
        {
        iSpSettings->AddEntryL( *entry );
        }
    CleanupStack::PopAndDestroy( 3 );
    }    
    
void T_VIMPSTEngineServiceTableFetcher::CreateSpEntry1L( TInt aServiceId,
                                            TServicePropertyName aName,
                                            const TDesC& aValue )
    {
    CSPProperty* property = CSPProperty::NewL();
    CleanupStack::PushL( property );
    property->SetName( aName );
    property->SetValue( aValue );

    CSPEntry* entry = CSPEntry::NewL();
    CleanupStack::PushL( entry );
    entry->SetServiceId( aServiceId );
    entry->SetServiceName(KServiceName() );

    entry->AddPropertyL( *property );

    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = iSpSettings->FindEntryL( entry->GetServiceId(), *testEntry );
    if( result == KErrNone )
        {
        iSpSettings->UpdateEntryL( *entry );
        }
    else
        {
        iSpSettings->AddEntryL( *entry );
        }
    CleanupStack::PopAndDestroy( 3 );
    
    }    
    

void T_VIMPSTEngineServiceTableFetcher::CreateSpEntry1L( TInt aServiceId,
                                            TServicePropertyName aName,
                                            TOnOff aValue )
    {
    CSPProperty* property = CSPProperty::NewL();
    CleanupStack::PushL( property );
    property->SetName( aName );
    property->SetValue( aValue );

    CSPEntry* entry = CSPEntry::NewL();
    CleanupStack::PushL( entry );
    entry->SetServiceId( aServiceId );
    entry->SetServiceName(KServiceName() );

    entry->AddPropertyL( *property );

    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt result = iSpSettings->FindEntryL( entry->GetServiceId(), *testEntry );
    if( result == KErrNone )
        {
        iSpSettings->UpdateEntryL( *entry );
        }
    else
        {
        iSpSettings->AddEntryL( *entry );
        }
    CleanupStack::PopAndDestroy( 3 );
    
    }         


void T_VIMPSTEngineServiceTableFetcher::GetContactStoreIdPropertyNotFoundL()
	{
	
	HBufC* contactStoreId = HBufC::NewL(500);
	CleanupStack::PushL(contactStoreId);
	
	TPtr storeNamePtr = contactStoreId->Des();

	iServiceTableFetcher->GetContactStoreIdL(1, storeNamePtr);	
	
	EUNIT_ASSERT_EQUALS( 0, storeNamePtr.Length() ); 		
	
	CleanupStack::PopAndDestroy( contactStoreId );	
			
	}


void T_VIMPSTEngineServiceTableFetcher::GetContactStoreIdPropertyFoundL()
	{
	
	HBufC* contactStoreId = HBufC::NewL(500);
	CleanupStack::PushL(contactStoreId);
	//contactStoreId->Des().Copy(_L("1_contacts.cdb"));
	
	TPtr storeNamePtr = contactStoreId->Des();
	
	CreateSpEntry1L( 1, EPropertyContactStoreId, KDbName() );
	
	iServiceTableFetcher->GetContactStoreIdL(1, storeNamePtr);	
	
	if (storeNamePtr.Compare(KDbName))
		{
		User::Leave( KErrArgument ); 	    		
		}
		
	CleanupStack::PopAndDestroy( contactStoreId );	
			
	}
	
void T_VIMPSTEngineServiceTableFetcher::GetContactStoreIdPropertyFoundWithContactUriDomainL()
	{
	
	HBufC* contactStoreId = HBufC::NewL(500);
	CleanupStack::PushL(contactStoreId);
	
	TPtr storeNamePtr = contactStoreId->Des();
	
	CreateSpEntry1L( 1, EPropertyContactStoreId, KDbNameURI() );
	
	iServiceTableFetcher->GetContactStoreIdL(1, storeNamePtr);	
	
	if (storeNamePtr.Compare(KDbNameURI))
		{
		User::Leave( KErrArgument ); 	    		
		}
		
	CleanupStack::PopAndDestroy( contactStoreId );	
			
	}	


void T_VIMPSTEngineServiceTableFetcher::GetBrandIdLPropertyNotFoundL()
	{
	
	HBufC8* brandId = HBufC8::NewL(500);
	CleanupStack::PushL(brandId);
	
	TPtr8 brandIdPtr = brandId->Des();

	iServiceTableFetcher->GetBrandIdL(1, brandIdPtr);	
	
	EUNIT_ASSERT_EQUALS( 0, brandIdPtr.Length() ); 		
	
	CleanupStack::PopAndDestroy( brandId );	
			
	}


void T_VIMPSTEngineServiceTableFetcher::GetBrandIdLPropertyFoundL()
	{
	
	HBufC8* brandId = HBufC8::NewL(500);
	CleanupStack::PushL(brandId);
	
	TPtr8 brandIdPtr = brandId->Des();
	
	CreateSpEntry1L( 1, EPropertyBrandId, _L("MyBrand") );
	
	iServiceTableFetcher->GetBrandIdL(1, brandIdPtr);	
	
	if (brandIdPtr.Compare(_L8("MyBrand")))
		{
		User::Leave( KErrArgument ); 	    		
		}
		
	CleanupStack::PopAndDestroy( brandId );	
			
	}
	


void T_VIMPSTEngineServiceTableFetcher::FindPropertyNotFoundL()
	{                                    
	TInt err = iServiceTableFetcher->FindPropertyL(1, EPropertyContactViewPluginId);
		
	EUNIT_ASSERT_EQUALS( KErrNotFound, err ); 
				
	}

void T_VIMPSTEngineServiceTableFetcher::FindPropertyFoundL()
	{  
	CreateSpEntryL( 1, EPropertyContactViewPluginId, 12234 );
	                                  
	TInt err = iServiceTableFetcher->FindPropertyL(1, EPropertyContactViewPluginId);
		
	EUNIT_ASSERT_EQUALS( KErrNone, err ); 							
	}


void T_VIMPSTEngineServiceTableFetcher::PropertyBrandVersionNotFoundL()
	{                                    
	TInt version = iServiceTableFetcher->PropertyBrandVersionL(1);
		
	EUNIT_ASSERT_EQUALS( KErrNotFound, version ); 
				
	}

void T_VIMPSTEngineServiceTableFetcher::PropertyBrandVersionFoundL()
	{  
	CreateSpEntryL( 1, EPropertyBrandVersion, 1 );
	                                  
	TInt version = iServiceTableFetcher->PropertyBrandVersionL(1);
		
	EUNIT_ASSERT_EQUALS( 1, version ); 							
	}


void T_VIMPSTEngineServiceTableFetcher::PropertyBrandLanguageNotFoundL()
	{                                    
	TLanguage lang = iServiceTableFetcher->PropertyBrandLanguageL(1);
		
	EUNIT_ASSERT_EQUALS( ELangInternationalEnglish, lang ); //if not found default is set to english
				
	}

void T_VIMPSTEngineServiceTableFetcher::PropertyBrandLanguageFoundL()
	{  
	CreateSpEntryL( 1, EPropertyBrandLanguage, ELangFrench );
	                                  
	TLanguage lang = iServiceTableFetcher->PropertyBrandLanguageL(1);
		
	EUNIT_ASSERT_EQUALS( ELangFrench, lang ); 							
	}
		

void T_VIMPSTEngineServiceTableFetcher::GetServiceNameNotFoundL()
	{
	
	HBufC* name = HBufC::NewL(500);
	CleanupStack::PushL(name);
	
	TPtr namePtr = name->Des();

	iServiceTableFetcher->GetServiceNameL(1, namePtr);	
	
	EUNIT_ASSERT_EQUALS( 0, namePtr.Length() ); 		
	
	CleanupStack::PopAndDestroy( name );	
			
	}


void T_VIMPSTEngineServiceTableFetcher::GetServiceNameFoundL()
	{
	
	HBufC* name = HBufC::NewL(500);
	CleanupStack::PushL(name);
	
	TPtr namePtr = name->Des();
	
	CreateSpEntry1L( 1, EPropertyBrandId, _L("MyBrand") );
	
	iServiceTableFetcher->GetServiceNameL(1, namePtr);	
	
	if (namePtr.Compare(KServiceName()))
		{
		User::Leave( KErrArgument ); 	    		
		}
		
	CleanupStack::PopAndDestroy( name );
			
	}


void T_VIMPSTEngineServiceTableFetcher::XimpAdapterUidNotFoundL()
	{                                    
	TInt ximpAdapter = iServiceTableFetcher->XimpAdapterUidL(1);
		
	EUNIT_ASSERT_EQUALS( KErrNotFound, ximpAdapter ); 
				
	}

void T_VIMPSTEngineServiceTableFetcher::XimpAdapterUidFoundL()
	{  
	CreateSpEntryL( 1, EPropertyPCSPluginId, 0x2000 );
	                                  
	TInt ximpAdapter = iServiceTableFetcher->XimpAdapterUidL(1);
		
	EUNIT_ASSERT_EQUALS( ximpAdapter, 0x2000 ); 							
	}


void T_VIMPSTEngineServiceTableFetcher::PresenceRequestStatusNotFoundL()
	{                                    
	TVIMPSTEnums::TVIMPSTPresenceRequestStatus ret = iServiceTableFetcher->PresenceRequestStatusL(1);
		
	EUNIT_ASSERT_EQUALS( TVIMPSTEnums::ESVCEPresenceRequestStatusUnKnown, ret ); 
				
	}

void T_VIMPSTEngineServiceTableFetcher::PresenceRequestStatusFoundWithEOffL()
	{  
	CreateSpEntry1L( 1, ESubPropertyPresenceRequestPreference, EOff );
	                                  
	TVIMPSTEnums::TVIMPSTPresenceRequestStatus ret = iServiceTableFetcher->PresenceRequestStatusL(1);
		
	EUNIT_ASSERT_EQUALS( ret, TVIMPSTEnums::ESVCEPresenceRequestStatusAlwaysAsk ); 							
	}

void T_VIMPSTEngineServiceTableFetcher::PresenceRequestStatusFoundWithEOnL()
	{  
	CreateSpEntry1L( 1, ESubPropertyPresenceRequestPreference, EOn );
	                                  
	TVIMPSTEnums::TVIMPSTPresenceRequestStatus ret = iServiceTableFetcher->PresenceRequestStatusL(1);
		
	EUNIT_ASSERT_EQUALS( ret, TVIMPSTEnums::ESVCEPresenceRequestStatusAutoAccept ); 							
	}	

	
void T_VIMPSTEngineServiceTableFetcher::ShutDownPhonebookL()
	{  
	//nothing to worry over here
	iServiceTableFetcher->ShutDownPhonebookL();							
	}

void T_VIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL_withNewServiceL()
    {  
    //nothing to worry over here
    MVIMPSTEngine * engine = NULL;
    iServiceTableFetcher->DoHandleNotifyChangeL(3, ETrue, engine);                         
    }	
void T_VIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL_withOutNewServiceL()
    {  
    //nothing to worry over here
    MVIMPSTEngine * engine = NULL;
    iServiceTableFetcher->DoHandleNotifyChangeL(3, EFalse, engine);                         
    }  


EUNIT_BEGIN_TEST_TABLE( T_VIMPSTEngineServiceTableFetcher, "T_VIMPSTEngineServiceTableFetcher", "" )

    EUNIT_TEST( "TestConstructionL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "ConstructionL",
            "FUNCTIONALITY", SetupL, TestConstructionL, Teardown )
    EUNIT_TEST( "TestConstructionWithValidServicesL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "ConstructionL",
            "FUNCTIONALITY", Setup1L, TestConstructionWithValidServicesL, Teardown )
    EUNIT_TEST( "TestConstructionWithInValidServicesL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "ConstructionL",
            "FUNCTIONALITY", Setup1L, TestConstructionWithInValidServicesL, Teardown )
    EUNIT_TEST("GetMasterServiceIdsNoServicesL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetMasterServiceIds",
            "FUNCTIONALITY", SetupL, GetMasterServiceIdsNoServicesL, Teardown )  
    EUNIT_TEST( "GetMasterServiceIdsServicesMatchContactViewUidValidServiceL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetMasterServiceIds",
            "FUNCTIONALITY", SetupL, GetMasterServiceIdsServicesMatchContactViewUidValidServiceL, Teardown )  
    EUNIT_TEST( "GetMasterServiceIdsServicesMatchContactViewUidInValidServiceL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetMasterServiceIds",
            "FUNCTIONALITY", SetupL, GetMasterServiceIdsServicesMatchContactViewUidInValidServiceL, Teardown )     
    EUNIT_TEST( "GetMasterServiceIdsServicesNoMatchContactViewUidL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetMasterServiceIds",
            "FUNCTIONALITY", SetupL, GetMasterServiceIdsServicesNoMatchContactViewUidL, Teardown ) 
    EUNIT_TEST( "GetContactStoreIdPropertyNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetMasterServiceIds",
            "FUNCTIONALITY", SetupL, GetContactStoreIdPropertyNotFoundL, Teardown ) 
    EUNIT_TEST( "GetContactStoreIdPropertyFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetContactStoreId",
            "FUNCTIONALITY", SetupL, GetContactStoreIdPropertyFoundL, Teardown ) 
    EUNIT_TEST("GetContactStoreIdPropertyFoundWithContactUriDomainL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "TestAllFunctionsL",
            "GetContactStoreId", SetupL, GetContactStoreIdPropertyFoundWithContactUriDomainL, Teardown )    
    EUNIT_TEST( "GetBrandIdLPropertyNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetBrandIdL",
            "FUNCTIONALITY", SetupL, GetBrandIdLPropertyNotFoundL, Teardown ) 
    EUNIT_TEST( "GetBrandIdLPropertyFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetBrandIdL",
            "FUNCTIONALITY",SetupL, GetBrandIdLPropertyFoundL, Teardown )    
    EUNIT_TEST("FindPropertyNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "FindProperty",
            "FUNCTIONALITY", SetupL, FindPropertyNotFoundL, Teardown ) 
    EUNIT_TEST( "FindPropertyFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "FindProperty",
            "FUNCTIONALITY", SetupL, FindPropertyFoundL, Teardown )  
    EUNIT_TEST( "PropertyBrandVersionNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PropertyBrandVersion",
            "FUNCTIONALITY", SetupL, PropertyBrandVersionNotFoundL, Teardown ) 
    EUNIT_TEST( "PropertyBrandVersionFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PropertyBrandVersion",
            "FUNCTIONALITY", SetupL, PropertyBrandVersionFoundL, Teardown )    
    EUNIT_TEST( "PropertyBrandLanguageNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PropertyBrandVersion",
            "FUNCTIONALITY", SetupL, PropertyBrandLanguageNotFoundL, Teardown ) 
    EUNIT_TEST( "PropertyBrandLanguageFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PropertyBrandVersion",
            "FUNCTIONALITY", SetupL, PropertyBrandLanguageFoundL, Teardown )  
    EUNIT_TEST( "GetServiceNameNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetServiceName",
            "FUNCTIONALITY", SetupL, GetServiceNameNotFoundL, Teardown ) 
    EUNIT_TEST( "GetServiceNameFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "GetServiceName",
            "FUNCTIONALITY", SetupL, GetServiceNameFoundL, Teardown )  
    EUNIT_TEST( "XimpAdapterUidNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "XimpAdapterUid",
            "FUNCTIONALITY", SetupL, XimpAdapterUidNotFoundL, Teardown ) 
    EUNIT_TEST( "XimpAdapterUidFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "XimpAdapterUid",
            "FUNCTIONALITY",SetupL, XimpAdapterUidFoundL, Teardown ) 
    EUNIT_TEST( "PresenceRequestStatusNotFoundL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PresenceRequestStatus",
            "FUNCTIONALITY", SetupL, PresenceRequestStatusNotFoundL, Teardown )  
    EUNIT_TEST( "PresenceRequestStatusFoundWithEOffL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PresenceRequestStatus",
            "FUNCTIONALITY", SetupL, PresenceRequestStatusFoundWithEOffL, Teardown ) 
    EUNIT_TEST("PresenceRequestStatusFoundWithEOnL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "PresenceRequestStatus",
            "FUNCTIONALITY", SetupL, PresenceRequestStatusFoundWithEOnL, Teardown )     
    EUNIT_TEST("ShutDownPhonebookL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "ShutDownPhonebookL",
            "FUNCTIONALITY", SetupL, ShutDownPhonebookL, Teardown ) 
   EUNIT_TEST("DoHandleNotifyChangeL_withOutNewServiceL ",
            "T_VIMPSTEngineServiceTableFetcher",
            "ShutDownPhonebookL",
            "FUNCTIONALITY", SetupL, DoHandleNotifyChangeL_withOutNewServiceL, Teardown ) 
   EUNIT_TEST("DoHandleNotifyChangeL_withNewServiceL ",
           "T_VIMPSTEngineServiceTableFetcher",
           "ShutDownPhonebookL",
           "FUNCTIONALITY", SetupL, DoHandleNotifyChangeL_withNewServiceL, Teardown ) 

EUNIT_END_TESTCASE_TABLE

//  END OF FILE
