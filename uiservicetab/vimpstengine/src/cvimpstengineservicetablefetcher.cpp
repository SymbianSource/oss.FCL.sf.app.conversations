/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles the use of Service Provider Settings API
*
*/


// INCLUDE FILES
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>


// Phonebook shutter
#include <apgtask.h>
#include <w32std.h>
#include <Pbk2UID.h>
#include <apgcli.h>
#include <apaid.h>

#include "cvimpstengineservicetablefetcher.h"
#include "cvimpstenginecchhandler.h"

#include "vimpstextentionuiuid.h"
#include "tvimpstconsts.h"
#include "mvimpstenginefactory.h"
#include "cvimpstenginefactory.h"
#include "mvimpststorageserviceview.h"
#include "cvimpststoragemanagerfactory.h"
#include "vimpstdebugtrace.h"



// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::CVIMPSTEngineServiceTableFetcher()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher::CVIMPSTEngineServiceTableFetcher()
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::ConstructL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::ConstructL()
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::ConstructL start"));
    iSettings = CSPSettings::NewL();
    CompressViewIdsL();
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::ConstructL end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher* 
    CVIMPSTEngineServiceTableFetcher::NewL( )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::NewL start"));
    
    CVIMPSTEngineServiceTableFetcher* self = NewLC();
    CleanupStack::Pop(self);
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::NewL end"));
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher* 
    CVIMPSTEngineServiceTableFetcher::NewLC()
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::NewLC start"));
    
    CVIMPSTEngineServiceTableFetcher* self =
        new (ELeave) CVIMPSTEngineServiceTableFetcher( );
    CleanupStack::PushL(self);
    self->ConstructL();
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::NewLC end"));
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::
//      ~CVIMPSTEngineServiceTableFetcher()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher::~CVIMPSTEngineServiceTableFetcher()
    {
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::~CVIMPSTEngineServiceTableFetcher start"));   
    
    delete iSettings;	
    iSettings = NULL;
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::~CVIMPSTEngineServiceTableFetcher end"));    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL( 
    RArray<TUint32>& aServiceIds )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL start"));
    
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    iSettings->FindServiceIdsL( services );
	
	const TInt serviceCount = services.Count();
    for ( TInt i( 0 ); i < serviceCount; i++ )
        {
        TUint32 refContactUid = ContactViewUidL( services[i] );
        
        if ( TUid::Uid( refContactUid ) == 
             TUid::Uid( KIMCONTACTSEXTNIMPLEMENTATIONUID ) 
             || TUid::Uid( refContactUid ) == 
             TUid::Uid( 0x20007B6D ) )
            {
            TRACE( T_LIT("GetMasterServiceIdsL() found serviceid: (%d)"), services[ i ] );

            aServiceIds.Append( services[ i ] );                

            TRACE( T_LIT("GetMasterServiceIdsL() - service append ok") );                

            }
        }
    CleanupStack::PopAndDestroy( &services );
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL end"));  
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL( 
    TUint32 aServiceId, TDes& aContactStoreId )
    {
    TRACE( T_LIT( 
       "CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL start")); 
    
    TRACE( T_LIT("GetContactStoreIdL() ServiceId: %d"), aServiceId );
        
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyContactStoreId,      
                                *property );    
	if (KErrNone == err)
		{	
	    err = property->GetValue( aContactStoreId );
	    
		}

    CleanupStack::PopAndDestroy( property );
    
    TRACE( T_LIT("GetContactStoreIdL() - StoreId: %S"), &aContactStoreId );                
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL end"));  
 	
    }   

    

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetBrandIdL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetBrandIdL( 
    TUint32 aServiceId, TDes8& aBrandId )
    {
    TRACE( T_LIT( "CVIMPSTEngineServiceTableFetcher::GetBrandIdL start"));
    
    TRACE( T_LIT("GetBrandIdL() ServiceId: %d"), aServiceId );
       
    HBufC* ret = HBufC::NewLC( KVIMPSTUISPSMaxPropertyLength );    
    TPtr retPtr( ret->Des() );
    
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyBrandId,
                                *property );    
    if ( KErrNone == err )
        {
        err = property->GetValue( retPtr );
        if ( KErrNone == err )
	        {
	        aBrandId.Copy( retPtr );
	        }
        }
    CleanupStack::PopAndDestroy( property );        
    CleanupStack::PopAndDestroy( ret );        
    
    TRACE( T_LIT("GetBrandIdL() - BrandId: %S"), &aBrandId ); 
    
    TRACE( T_LIT( "CVIMPSTEngineServiceTableFetcher::GetBrandIdL end"));
    }    


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::FindPropertyL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::FindPropertyL( TUint32 aServiceId,
	                                TServicePropertyName aPropertyName )
	{
	TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::FindPropertyL start"));
    
    TRACE( T_LIT("FindPropertyL- ServiceId: %d Property: %d"), 
    								aServiceId, aPropertyName ); 
       
	CSPProperty* property = CSPProperty::NewLC();
    TInt result = iSettings->FindPropertyL( aServiceId,
                                aPropertyName,
                                *property );		
	CleanupStack::PopAndDestroy( property );
	
	TRACE( T_LIT("FindPropertyL- Result: %d"), result );
    								
	TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::FindPropertyL end"));
       
	return result;
	}




// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL( 
    TUint32 aServiceId ) 
    {
    TRACE( T_LIT( "CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL start"));
    
    TRACE( T_LIT("PropertyBrandVersionL() ServiceId: %d"), aServiceId );
       
    TInt brandVersion = KErrNotFound; // default value
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyBrandVersion,
                                *property );    
    if ( KErrNone == err )
        {
        err = property->GetValue( brandVersion );
        }
        
    CleanupStack::PopAndDestroy( property );
    
    TRACE( T_LIT("PropertyBrandVersionL - version: %d"), brandVersion );  
    
    TRACE( T_LIT( "CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL end"));
    return brandVersion;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL()
// ---------------------------------------------------------------------------
//
TLanguage CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL( 
    TUint32 aServiceId ) 
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL start"));
    
    TRACE( T_LIT("PropertyBrandLanguageL() ServiceId: %d"), aServiceId );
    
    // ELangInternationalEnglish is selected as Default Brand Language Id.
    TInt brandLanguage = ELangInternationalEnglish;    
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyBrandLanguage,
                                *property );    
    if ( KErrNone == err )
        {
        err = property->GetValue( brandLanguage );
        }
    CleanupStack::PopAndDestroy( property );

    TRACE( T_LIT("PropertyBrandVersionL - brandLanguage: (%d) "), brandLanguage );  
	
	TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL end"));
       
    return ((TLanguage) (brandLanguage) );
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::ContactViewUidL()
// ---------------------------------------------------------------------------
//
TUint32 CVIMPSTEngineServiceTableFetcher::ContactViewUidL( 
    TUint32 aServiceId )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::ContactViewUidL start"));
    
    TRACE( T_LIT("ContactViewUidL() ServiceId: %d"), aServiceId );
        
    TInt contactViewUid = KErrNotFound;
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyContactViewPluginId,
                                *property );    
    if ( KErrNone == err )
        {
        property->GetValue( contactViewUid );
        }
    CleanupStack::PopAndDestroy( property );      
          
    TRACE( T_LIT("ContactViewUid: %d"), contactViewUid );    
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL end"));
       
    return contactViewUid;
    }    

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetServiceNameL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetServiceNameL( 
    TUint32 aServiceId, TDes& aServiceName )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetServiceNameL start"));    
    
    TRACE( T_LIT("GetServiceNameL() ServiceId: %d"), aServiceId );
    
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( aServiceId, *entry ) ;
    if ( !err )
        {
        aServiceName = entry->GetServiceName();
        }
    
    CleanupStack::PopAndDestroy( entry );    
    
    TRACE( T_LIT("GetServiceNameL() ServiceName: %S"), &aServiceName );
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::GetServiceNameL end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL(
    		TServiceId aServiceId, TBool aNewService, MVIMPSTEngine* aEngine )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL start"));  
    TRACE( T_LIT("DoHandleNotifyChange() ServiceId:%d"), aServiceId );
    TBool serviceFound( EFalse ); 
    if(!aNewService)
        {
        RArray<TUint32> serviceIdArray;
        CleanupClosePushL( serviceIdArray ); 
        RArray<TServiceId> services;
        CleanupClosePushL( services );    
        GetMasterServiceIdsL(serviceIdArray);
        iSettings->FindServiceIdsL(services );
        const TInt serviceCount = services.Count();
        for ( TInt index = 0 ; index < serviceCount ; index++ )
            {
            if ( services[index] == aServiceId )
                {
                TRACE( T_LIT("DoHandleNotifyChange() - Service (%d) found"), aServiceId );
                serviceFound = ETrue;
                }
            }
        CleanupStack::PopAndDestroy( &services );
        CleanupStack::PopAndDestroy( &serviceIdArray ); 
        }
    if ( aNewService || !serviceFound )  // new service or MECO service not found
        {
        // check if any other services other then MECO exists before
        // shutting down the phonebook. there can be services which does't use
        // MECO UI ( example OVI - does't use MECO UI it uses own UI )
        // check with the property EPropertyContactViewPluginId in the service 
        // this will the meco UID in case of MECO Service.
        //this is a temperory hack as phonebook doesnot refresh once we delete or add any service
        //once phonebook gives a permanant fix ,remove the below code
        if(( TUid::Uid(ContactViewUidL( aServiceId )) == TUid::Uid( KIMCONTACTSEXTNIMPLEMENTATIONUID )) || aEngine ) 
            {                                                               // only MecoServicetabs shutdown phonebook.
            TRACE( T_LIT("DoHandleNotifyChange() - removing service") );    //or while deleting service from settings
            CompressViewIdsL();                                             //check whether engine exists,which is true for only MecoServicetabs.
            if(aEngine)
                {
                aEngine->DeleteDataBaseL();
                }
            ShutDownPhonebookL();
            }
        TRACE( T_LIT("DoHandleNotifyChange() - service removed") );
        }
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL end"));    

    }
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::CompressViewIdsL()
// ---------------------------------------------------------------------------
//
TBool CVIMPSTEngineServiceTableFetcher::CompressViewIdsL( 
    TServiceId aServiceId )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::CompressViewIdsL start"));       
    TRACE( T_LIT("CompressViewIdsL() ServiceId: %d"), aServiceId );
    
    TBool ret = EFalse;
    // Compress view id's in service table
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    iSettings->FindServiceIdsL( services );
    TUint32 serviceId = 0;
    const TInt serviceCount = services.Count();
    for ( TInt i( 0 ); i < serviceCount; i++ )
        {
        serviceId = services[i];
        TUint32 refContactUid = ContactViewUidL( serviceId );
        if ( TUid::Uid( refContactUid ) == 
        TUid::Uid( KIMCONTACTSEXTNIMPLEMENTATIONUID ) )
            {

            TRACE( T_LIT("  -> compressing view ids, service: %d"), serviceId );
            TRACE( T_LIT("  -> compressing view ids, view id: (%d)"), ( KVIMPSTTabbedViewIdBase+ serviceId ) );

            WriteTabViewIdToServiceTableL( services[ i ], ( KVIMPSTTabbedViewIdBase + serviceId ) );
        
            ret = ETrue;
            }
        }
    CleanupStack::PopAndDestroy( &services );
    
    TRACE( T_LIT("CompressViewIdsL returns: (%d)"), ret );     
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::CompressViewIdsL end"));    
    
    return ret;
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL( 
    TUint32 aServiceId, TUint aViewId ) 
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL start"));   
    TRACE( T_LIT("WriteTabViewIdToServiceTableL() aServiceId: %d, aViewId: %d "), 
    										aServiceId, aViewId );
    
    // Write specific view id to service table
    // No need to check for error beacuse 
    // we need to add/update the property

    
    CSPProperty* property = CSPProperty::NewLC();
    iSettings->FindPropertyL( aServiceId,
                                EPropertyContactViewId,
                                *property );    
    
    TRACE( T_LIT( 
       "WriteTabViewIdToServiceTableL() - OK"));
    
    TInt err = property->SetName( EPropertyContactViewId );
    
     TRACE( T_LIT( 
       "WriteTabViewIdToServiceTableL ERR1=%d"), err );
    
    TInt err2 = property->SetValue( aViewId );
    
    if ( !err && !err2 )
        {
        iSettings->AddOrUpdatePropertyL( aServiceId, *property );
        }
    
    TRACE( T_LIT( 
       "WriteTabViewIdToServiceTableL ERR2=%d"), err2 );
        
    
    CleanupStack::PopAndDestroy( property );  
                   
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL end") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL()
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL start"));   
    
    // Kills phonebook if running
    RWsSession ws;
    User::LeaveIfError( ws.Connect() );
    CleanupClosePushL( ws );
    TApaTaskList list( ws );
    TUid uid = { KPbk2UID3 };
    TApaTask task( list.FindApp( uid ) );
    if ( task.Exists() )
        {
        task.EndTask();
        }
    CleanupStack::PopAndDestroy( &ws ); // ws
    
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL end"));
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL( 
    TUint32 aServiceId )
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL start"));   
    
    TRACE( T_LIT("XimpAdapterUidL() ServiceId: %d"), aServiceId );
    										
    TInt ret = KErrNotFound;
    CSPProperty* property = CSPProperty::NewLC();
    
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyPCSPluginId,
                                *property );    
    if (KErrNone == err)
	    {
	    property->GetValue( ret );
	    }
    
    CleanupStack::PopAndDestroy( property );
    
    TRACE( T_LIT("XimpAdapterUidL() XimpAdapterUid: %d "), 
    										ret );
    										
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL end"));   
    return ret;
    }        
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::PresenceRequestStatusL()
// ---------------------------------------------------------------------------
//
TVIMPSTEnums::TVIMPSTPresenceRequestStatus 
CVIMPSTEngineServiceTableFetcher::PresenceRequestStatusL( 
        TUint32 aServiceId )    
    {
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::PresenceRequestStatusL  start")); 
    
    TRACE( T_LIT("PresenceRequestStatusL() ServiceId: %d"), aServiceId );
    
    TVIMPSTEnums::TVIMPSTPresenceRequestStatus ret = 
    			TVIMPSTEnums::ESVCEPresenceRequestStatusUnKnown;
            
    CSPProperty* property = CSPProperty::NewLC();
    
    TInt err = iSettings->FindPropertyL( aServiceId,
           ESubPropertyPresenceRequestPreference,
                                *property );    
    if ( KErrNone == err )
        {
        TOnOff value = EOff;        
        err = property->GetValue( value );
        if ( !err )
            {
            if ( EOn == value )
                {
                ret = TVIMPSTEnums::ESVCEPresenceRequestStatusAutoAccept;
                }
			else
				{
				ret = TVIMPSTEnums::ESVCEPresenceRequestStatusAlwaysAsk;
				}                
            }
        }
    
    CleanupStack::PopAndDestroy( property );
    
    TRACE( T_LIT("PresenceRequestStatusL() PresenceRequestStatus: %d "), 
    										ret );
    										
    TRACE( T_LIT("CVIMPSTEngineServiceTableFetcher::PresenceRequestStatusL end"));   
    return ret;        
    }
  
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL( 
    TUint32 aServiceId, TDes& aDefaultDomainName )
    {
    TRACE( T_LIT( 
       "CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL start")); 
    
    TRACE( T_LIT("GetContactStoreIdL() ServiceId: %d"), aServiceId );
        
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                ESubPropertyPresenceAddrScheme,      
                                *property );    
	if (KErrNone == err)
		{	
	    property->GetValue( aDefaultDomainName );	    
		}

    CleanupStack::PopAndDestroy( property );
    
    TRACE( T_LIT("GetContactStoreIdL() - StoreId: %S"), &aDefaultDomainName );                
    
    TRACE( T_LIT( 
       "CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL end"));  
 	
    } 

   
// End of file
