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
* Description:  This is a plugin implementation
 *                
 *
*/

 
#include "cservicewidgetplugin.h"

#include <implementationproxy.h>
#include "servicewidgetutils.h"
#include "servicewidgetpluginuids.hrh"
#include "servicewidgetcpglobals.h"
#include "cservicewidgetservicemanager.h"
#include "cservicewidgetcontentpublisher.h"
#include "swpdebugtrace.h"
#include <spsettings.h> 
#include <spproperty.h>
#include <cvimpstsettingsstore.h>
#include <servicewidgetpluginres.rsg>
#include <bautils.h>
#include <eikenv.h>
#include <spnotifychange.h>

// Provide a key pair value table for ECOM.
// Used to identify the correct construction function for the requested interface

const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( AI_UID_DLL_SERVICEWIDGETPUBLISHER_PLUGIN,
                                CServiceWidgetPlugin::NewL )
    };

// ======== GLOBAL FUNCTIONS ========
// Return an instance of the proxy table.

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }
    
// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetPlugin* CServiceWidgetPlugin::NewL()
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::NewL() start") );
    CServiceWidgetPlugin* self = new( ELeave )CServiceWidgetPlugin;
    CleanupStack::PushL(self);
   	self->ConstructL();
   	CleanupStack::Pop(self); 
   	TRACE_SWP(TXT("CServiceWidgetPlugin::NewL() End") );
   	return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetPlugin::CServiceWidgetPlugin()
   {
   TRACE_SWP(TXT("CServiceWidgetPlugin::CServiceWidgetPlugin() Start") );
   iEnv = CCoeEnv::Static();
   TRACE_SWP(TXT("CServiceWidgetPlugin::CServiceWidgetPlugin() End") );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetPlugin::ConstructL()
	{
	TRACE_SWP(TXT("CServiceWidgetPlugin::ConstructL() Start") );
	iContentPublisher = CServiceWidgetContentPublisher::NewL( *this );
	TRACE_SWP(TXT("CServiceWidgetContentPublisher created succesfully") );
	iSettingsStore = CVIMPSTSettingsStore::NewL();
	TRACE_SWP(TXT("CServiceWidgetPlugin iSettingsStore created succesfully") );
	// read service ids 
	ReadServiceIdsL();
	TRACE_SWP(TXT(" CServiceWidgetPlugin ReadServiceIdsL excuted") );
	LoadResourceFileL();
	TRACE_SWP(TXT(" CServiceWidgetPlugin LoadResourceFileL excuted") );
	TRACE_SWP(TXT("CServiceWidgetPlugin::ConstructL() End") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetPlugin::~CServiceWidgetPlugin()
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::~CServiceWidgetPlugin() Start") );
    delete iSpNotifyChange;
    iServiceManagers.ResetAndDestroy();
    delete iSettingsStore;
    delete iContentPublisher;
    iServiceIds.Reset();
    
    if( iEnv )
		{
		iEnv->DeleteResourceFile( iResourceOffset );	
		}
	TRACE_SWP(TXT("CServiceWidgetPlugin::~CServiceWidgetPlugin() End") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::ReadServiceIdsL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetPlugin::ReadServiceIdsL()
	{
	TRACE_SWP(TXT("CServiceWidgetPlugin::ReadServiceIdsL() Start") );
	CSPSettings* settings = CSPSettings::NewLC(); 
	///////////////////////Get Service Id////////////////    
	RArray<TServiceId> services;
	CleanupClosePushL( services );
	//Get the all configured service ID in services array
	settings->FindServiceIdsL( services );
	const TInt serviceCount = services.Count();
	iServiceIds.Reset();
    //Append all the service ID in which service tab is interested in iServiceIds array		
	TRACE_SWP(TXT("CServiceWidgetPlugin::ReadServiceIdsL() serviceCount %d"), serviceCount );
	for ( TInt i( 0 ); i < serviceCount; i++ )
		{
		TInt refContactUid = KErrNotFound;
		CSPProperty* property = CSPProperty::NewLC();
		settings->FindPropertyL( services[i],EPropertyContactViewPluginId,*property );    
		if ( property )
		    {
		    property->GetValue( refContactUid );
		    }
		CleanupStack::PopAndDestroy( property );             
		if ( (TUid::Uid( refContactUid ) == TUid::Uid( KMECOIMPLEMENTATIONUID ))
		        || (TUid::Uid( refContactUid ) ==  TUid::Uid( KIMServicetabPluginUid ) )
		        || (TUid::Uid( refContactUid ) ==  TUid::Uid( 0x2002B334 ) ))  
		    {
		    TRACE_SWP(TXT("CServiceWidgetPlugin::ReadServiceIdsL() valid service id %d"), services[i] );   
		    iServiceIds.Append( services[i] );
		    }
		}
	CleanupStack::PopAndDestroy( &services); //services  
	CleanupStack::PopAndDestroy( settings ); //settings 
	TRACE_SWP(TXT("CServiceWidgetPlugin::ReadServiceIdsL() End ") ); 
	}

 // ---------------------------------------------------------------------------
// CServiceWidgetPlugin::LoadResourceFileL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetPlugin::LoadResourceFileL()
	{
	TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() Start") );
	if( iEnv )
		{
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() iEnv is valid") );
		TFileName path;    
		Dll::FileName(path); //get the location of the dll
		TDriveName drive( TParsePtrC( path ).Drive( ) ); // solve drive
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() TParsePtrC called") );
		// leave to stack, LC method
		HBufC* resourceFilePath = HBufC::NewLC( KSW_RESOURCE_MAX_LENGTH );
		TPtr ptr( resourceFilePath->Des() );
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() resourceFilePath called") );
		ptr.Zero();
		ptr.Append( KResourceDrive() );
		ptr.Append( KResourceFileName() );
		   // NearestLanguageFile takes only TFileName
		path.Zero();
		path.Append( *resourceFilePath );
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() path.Append called") );
		if ( path.Length() != 0 )
		    {
		    TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() path length not zero") );
		    BaflUtils::NearestLanguageFile( iEnv->FsSession(), path );
		    }
		CleanupStack::PopAndDestroy(resourceFilePath); // resourceFilePath
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() resourceFilePath deleted") );
		iResourceOffset = iEnv->AddResourceFileL(path);
		TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() AddResourceFileL called") );
		}
	TRACE_SWP(TXT("CServiceWidgetPlugin::LoadResourceFileL() End") );
	}
// ----------------------------------------------------------------------------
// This function updates information in CPS storage
// ----------------------------------------------------------------------------
//	
void CServiceWidgetPlugin::UpdateL() 
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::UpdateL() Start") );
    //register for service table notifications
    RArray<TUint> serviceIds;
    CleanupClosePushL( serviceIds );
    iSpNotifyChange = CSPNotifyChange::NewL( *this );    
    iSpNotifyChange->NotifyChangeL( serviceIds );
    CleanupStack::PopAndDestroy( &serviceIds ) ; 

    TInt serviceCount = iServiceIds.Count();
    TRACE_SWP(TXT("CServiceWidgetPlugin::UpdateL() serviceCount %d"), serviceCount );
    iServiceManagers.ResetAndDestroy();
    if( serviceCount )
        {
        for( TInt i=0; i < serviceCount; i++ )
            {
            TRACE_SWP(TXT("CServiceWidgetPlugin::UpdateL() service id  %d"), iServiceIds[i] );

            CServiceWidgetServiceManager* servicemanager = CServiceWidgetServiceManager::NewL( iServiceIds[i],
                    *iContentPublisher,*iSettingsStore );
            CleanupStack::PushL( servicemanager );
            servicemanager->RegisterWidgetL();
            iServiceManagers.Append(servicemanager);
            CleanupStack::Pop(servicemanager );
            TRACE_SWP(TXT("CServiceWidgetPlugin::UpdateL() service manager created for %d" ) , iServiceIds[i] );
            }//End for
        }
    TRACE_SWP(TXT("CServiceWidgetPlugin::UpdateL() End") );
    }    


// ----------------------------------------------------------------------------
// This function Validates all serviceManagers and add or delete as per service 
// CServiceWidgetPlugin::ValidateAllServicesL
// ----------------------------------------------------------------------------
//  
void CServiceWidgetPlugin::ValidateAllServicesL()
{
TRACE_SWP(TXT("CServiceWidgetPlugin::ValidateAllServicesL() Start") );
TInt count = iServiceIds.Count();
//If all services are deleted then unregister all widget first and then delete
//ServiceManager
if( count == 0 )
    {
    TInt serviceCount = iServiceManagers.Count();
    for( TInt i=0; i<serviceCount; i++ )
        {
        iServiceManagers[ i ]->UnregisterWidgetL();
        }
    iServiceManagers.ResetAndDestroy();
    }
else
    {
    // check first all deleted services and remove their manager.
    TBool servicemanagerFound = EFalse;
    TInt serviceCount = iServiceManagers.Count();
    for( TInt i = 0; i<serviceCount; i++)
        {
        servicemanagerFound = EFalse;
        for( TInt j=0; j<count; j++ )
            {
            if( iServiceIds[ j ] == iServiceManagers[i]->ServiceId() )
                {
                servicemanagerFound = ETrue;
                break;
                }
            }//End for J
        if( !servicemanagerFound )
            {
            // Delete service manager
            CServiceWidgetServiceManager* servicemanager = iServiceManagers[ i ];
            //Unregister widget
            servicemanager->UnregisterWidgetL();
            TRACE_SWP(TXT("CServiceWidgetContentPublisher::RegisterWidgetPublishers() UnRegistered widget servID %d"),iServiceManagers[i]->ServiceId()  );
            iServiceManagers.Remove( i );
            delete servicemanager;
            servicemanager = NULL;
            iServiceManagers.Compress();
            serviceCount = serviceCount-1;
            }
        }//End For i
        // Check for new services and create their manager if not exist
        for( TInt i=0; i<count; i++)
            {
            servicemanagerFound = EFalse;
            serviceCount = iServiceManagers.Count();
            for( TInt j=0; j<serviceCount; j++ )
                {
                if( iServiceIds[ i ] == iServiceManagers[j]->ServiceId() )
                    {
                    servicemanagerFound = ETrue;
                    break;
                    }
                }//End For J
            if( !servicemanagerFound )
                {
                // create new service manager
                CServiceWidgetServiceManager* servicemanager = CServiceWidgetServiceManager::NewL( iServiceIds[ i ],
                        *iContentPublisher,*iSettingsStore );
                CleanupStack::PushL( servicemanager );
                //Register widget to HS
                servicemanager->RegisterWidgetL();
                iServiceManagers.Append(servicemanager);
                CleanupStack::Pop(servicemanager );
                }
            }//End for i
    }//End Else
TRACE_SWP(TXT("CServiceWidgetPlugin::ValidateAllServicesL() End") );
}//End ValidateAllServicesL

// -----------------------------------------------------------------------------------
// CServiceWidgetPlugin::HandleServiceEventL
// This function handles all homescreen callbacks (publisher received callback events 
// from homescreen and directed them to plugin to take appropriate action.) 
// -----------------------------------------------------------------------------------
//  
void CServiceWidgetPlugin::HandleServiceEventL(const TDesC& aPublisherId, const TDesC8& aEvent,TDesC& aInstId )
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Start") );
    CServiceWidgetServiceManager* servicemanager = NULL;
    TInt serviceCount = iServiceManagers.Count();
    TInt index = KErrNotFound;
    //First identify which manager is supposed to handle event
    for( index=0; index<serviceCount; index++ )
        {
        if( iServiceManagers[ index]->ServiceName().Compare( aPublisherId ) == 0 )
            {
            servicemanager = iServiceManagers[ index ];
            break;
            }
        }
    //Take action based on Event
    if( servicemanager && (index < serviceCount) && (index >=0) )
        {
        if( aEvent.Compare( KActive )  == 0 )
            {
            TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Active Event") );
            // this publisher widget is added into homescreen.
            // publish content to widget ,widget is ready to use
            servicemanager->ActivateWidgetL(aInstId);
            }
        else if( aEvent.Compare( KResume )  == 0  )
            {
            TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Resume Event") );
            servicemanager->WidgetSuspended( EFalse );
            // Home screen is in foreground or back light is on now. 
            // publishing / removing data now will be immediately updated in your widget. 
            // this publisher widget is added into homescreen.
            // publich content to widget ,widget is ready to use
            servicemanager->UpdateContentsL();
            }
        else if( aEvent.Compare( KSuspend )  == 0 || aEvent.Compare( KInactive )  == 0 )
            {
            TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Suspend Event") );
            // Home screen is in back ground or back light is off. 
            // don't publish new data now it won't be updated in your widget.
            // widget is suspended
            // Home screen page is switched 
            //             // don't publish new data now it won't be updated in your widget.
            //             // widget is suspended and in Inactive state
            servicemanager->WidgetSuspended( ETrue ); 
            }
        else if( aEvent.Compare( KDeActive )  == 0   )
            {
             TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Deactive Event") );
             // this publisher widget is removed from homescreen.
             // remove the last data published to homescreen. 
             // So that when the user add widget again it won't show the old data.
             servicemanager->DeactivateWidgetL(aInstId);
            }
        else if( aEvent.Compare( KSelected )  == 0   )
            {
             TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() Selected Event") );
            // this publisher widget is clicked from homescreen.
            servicemanager->ExecuteActionL();
            }
        }
    TRACE_SWP(TXT("CServiceWidgetPlugin::HandleServiceEventL() End") );
    }//End HandleServiceEventL


//////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::HandleNotifyChange()
// ---------------------------------------------------------------------------
//
void CServiceWidgetPlugin::HandleNotifyChange(
    TServiceId /*aServiceId*/ )
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::HandleNotifyChange() start") );
    //This function will be triggred if any change in any service
    TRAP_IGNORE( // Read the service id if any installed 
                 ReadServiceIdsL();
                 ValidateAllServicesL(); 
                 );
    TRACE_SWP(TXT("CServiceWidgetPlugin::HandleNotifyChange() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPlugin::HandleError()
// ---------------------------------------------------------------------------
//
void CServiceWidgetPlugin::HandleError( TInt /*aError*/ )
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::HandleError() start/end") );
    }  

// ---------------------------------------------------------------------------
// From class MAiPropertyExtension
// Read property of publisher plug-in.
// ---------------------------------------------------------------------------
//
/*TAny* CServiceWidgetPlugin::GetPropertyL( TInt aProperty )
    {
    TRACE_SWP(TXT("CServiceWidgetPlugin::GetPropertyL() start") );
    TAny* property = NULL;
    
    switch ( aProperty )
        {
        case EAiPublisherInfo:
            {
             property = static_cast<TAiPublisherInfo*>( &iInfo );
            break;  
            }       
        }
    TRACE_SWP(TXT("CServiceWidgetPlugin::GetPropertyL() end") );
    return property;
    }

// ---------------------------------------------------------------------------
// From class MAiPropertyExtension
// Write property value to optimize the content model.
// ---------------------------------------------------------------------------
//
void CServiceWidgetPlugin::SetPropertyL( TInt aProperty, TAny* aValue )
    { 
    TRACE_SWP(TXT("CServiceWidgetPlugin::SetPropertyL() start") );
    switch ( aProperty )
        {
        case EAiPublisherInfo:
            {
            if( aValue )
                {
                const TAiPublisherInfo* info = static_cast<const TAiPublisherInfo*>( aValue );
                iInfo.iName.Copy(info->iName);
                iInfo.iNamespace.Copy(info->iNamespace);
                }
            break;
            }
        default:
            break;         
        }
    TRACE_SWP(TXT("CServiceWidgetPlugin::SetPropertyL() end") );
    }*/
// End of File
