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
#include "uiservicetabtracer.h"



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
	TRACER_AUTO;
    iSettings = CSPSettings::NewL();
    CompressViewIdsL();
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher* 
    CVIMPSTEngineServiceTableFetcher::NewL( )
    {
	TRACER_AUTO;
    
    CVIMPSTEngineServiceTableFetcher* self = NewLC();
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher* 
    CVIMPSTEngineServiceTableFetcher::NewLC()
    {
	TRACER_AUTO;
    
    CVIMPSTEngineServiceTableFetcher* self =
        new (ELeave) CVIMPSTEngineServiceTableFetcher( );
    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::
//      ~CVIMPSTEngineServiceTableFetcher()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineServiceTableFetcher::~CVIMPSTEngineServiceTableFetcher()
    {
	TRACER_AUTO;
    
    
    delete iSettings;	
    iSettings = NULL;
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetMasterServiceIdsL( 
    RArray<TUint32>& aServiceIds )
    {
	TRACER_AUTO;
    
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
            TRACE( " found serviceid: (%d)", services[ i ] );

            aServiceIds.Append( services[ i ] );                

            TRACE("service append ok" ); 

            }
        }
    CleanupStack::PopAndDestroy( &services );
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetContactStoreIdL( 
    TUint32 aServiceId, TDes& aContactStoreId )
    {
	TRACER_AUTO;
    
    TRACE( "ServiceId: %d", aServiceId );
        
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                EPropertyContactStoreId,      
                                *property );    
	if (KErrNone == err)
		{	
	    err = property->GetValue( aContactStoreId );
	    
		}

    CleanupStack::PopAndDestroy( property );
    
    TRACE( "StoreId: %S", &aContactStoreId );     
    
 	
    }   

    

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetBrandIdL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetBrandIdL( 
    TUint32 aServiceId, TDes8& aBrandId )
    {
	TRACER_AUTO;
    
    TRACE( "ServiceId: %d", aServiceId );
       
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
    
    TRACE( "BrandId: %S", &aBrandId ); 
    
    }    


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::FindPropertyL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::FindPropertyL( TUint32 aServiceId,
	                                TServicePropertyName aPropertyName )
	{
	TRACER_AUTO;
    
	 TRACE( "ServiceId: %d Property: %d", aServiceId, aPropertyName );
       
	CSPProperty* property = CSPProperty::NewLC();
    TInt result = iSettings->FindPropertyL( aServiceId,
                                aPropertyName,
                                *property );		
	CleanupStack::PopAndDestroy( property );
	
	TRACE(" Result: %d", result );
    								
       
	return result;
	}




// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::PropertyBrandVersionL( 
    TUint32 aServiceId ) 
    {
	TRACER_AUTO;
    
	TRACE( "ServiceId: %d", aServiceId );
       
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
    
    TRACE( "version: %d", brandVersion );
    
    return brandVersion;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL()
// ---------------------------------------------------------------------------
//
TLanguage CVIMPSTEngineServiceTableFetcher::PropertyBrandLanguageL( 
    TUint32 aServiceId ) 
    {
	TRACER_AUTO;
    
	 TRACE( " ServiceId: %d", aServiceId );
    
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

    TRACE( "brandLanguage: (%d) ", brandLanguage );
	
       
    return ((TLanguage) (brandLanguage) );
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::ContactViewUidL()
// ---------------------------------------------------------------------------
//
TUint32 CVIMPSTEngineServiceTableFetcher::ContactViewUidL( 
    TUint32 aServiceId )
    {
	TRACER_AUTO;
    
	TRACE( "ServiceId: %d", aServiceId );
        
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
          
    TRACE( "ContactViewUid: %d", contactViewUid ); 
    
       
    return contactViewUid;
    }    

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetServiceNameL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetServiceNameL( 
    TUint32 aServiceId, TDes& aServiceName )
    {
	TRACER_AUTO;
    
	TRACE( "ServiceId: %d", aServiceId );
    
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( aServiceId, *entry ) ;
    if ( !err )
        {
        aServiceName = entry->GetServiceName();
        }
    
    CleanupStack::PopAndDestroy( entry );    
    
    TRACE("ServiceName: %S", &aServiceName );
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::DoHandleNotifyChangeL(
    		TServiceId aServiceId, TBool aNewService, MVIMPSTEngine* aEngine )
    {
	TRACER_AUTO;
	TRACE( "ServiceId:%d", aServiceId );
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
            TRACE( "Service (%d) found", aServiceId );
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
            TRACE( "removing service" );    //or while deleting service from settings
            CompressViewIdsL();                                             //check whether engine exists,which is true for only MecoServicetabs.
            if(aEngine)
                {
                aEngine->DeleteDataBaseL();
                }
            ShutDownPhonebookL();
            }
        TRACE( "service removed" );
        }

    }
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::CompressViewIdsL()
// ---------------------------------------------------------------------------
//
TBool CVIMPSTEngineServiceTableFetcher::CompressViewIdsL( 
    TServiceId aServiceId )
    {
	TRACER_AUTO;
	TRACE(" ServiceId: %d", aServiceId );
    
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

        TRACE( "  -> compressing view ids, service: %d", serviceId );
        TRACE( "  -> compressing view ids, view id: (%d)", ( KVIMPSTTabbedViewIdBase+ serviceId ) );

            WriteTabViewIdToServiceTableL( services[ i ], ( KVIMPSTTabbedViewIdBase + serviceId ) );
        
            ret = ETrue;
            }
        }
    CleanupStack::PopAndDestroy( &services );
    
    TRACE( "CompressViewIdsL returns: (%d)", ret );
    
    return ret;
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::WriteTabViewIdToServiceTableL( 
    TUint32 aServiceId, TUint aViewId ) 
    {
	TRACER_AUTO;
    
    TRACE( "aServiceId: %d, aViewId: %d ",aServiceId, aViewId );
    
    // Write specific view id to service table
    // No need to check for error beacuse 
    // we need to add/update the property

    
    CSPProperty* property = CSPProperty::NewLC();
    iSettings->FindPropertyL( aServiceId,
                                EPropertyContactViewId,
                                *property );    
    
    TRACE( " OK");
    
    TInt err = property->SetName( EPropertyContactViewId );
    
    TRACE( " ERR1=%d", err );
    
    TInt err2 = property->SetValue( aViewId );
    
    if ( !err && !err2 )
        {
        iSettings->AddOrUpdatePropertyL( aServiceId, *property );
        }
    
    TRACE(" ERR2=%d", err2 );
        
    
    CleanupStack::PopAndDestroy( property );  
                   
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::ShutDownPhonebookL()
    {
	TRACER_AUTO;
    
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
    
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineServiceTableFetcher::XimpAdapterUidL( 
    TUint32 aServiceId )
    {
	TRACER_AUTO;
    
	TRACE( "ServiceId: %d", aServiceId );
    										
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
    
    
    TRACE( "XimpAdapterUid: %d ",ret );
    										
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
	TRACER_AUTO;
    
	 TRACE( " ServiceId: %d", aServiceId );
    
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
    
    TRACE("PresenceRequestStatus: %d ", ret );
    										
    return ret;        
    }
  
// ---------------------------------------------------------------------------
// CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineServiceTableFetcher::GetDefaultDomainNameL( 
    TUint32 aServiceId, TDes& aDefaultDomainName )
    {
	TRACER_AUTO;
    
	
	 TRACE( "ServiceId: %d", aServiceId );
        
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( aServiceId,
                                ESubPropertyPresenceAddrScheme,      
                                *property );    
	if (KErrNone == err)
		{	
	    property->GetValue( aDefaultDomainName );	    
		}

    CleanupStack::PopAndDestroy( property );
    
    TRACE( " StoreId: %S", &aDefaultDomainName );
    
 	
    } 

   
// End of file
