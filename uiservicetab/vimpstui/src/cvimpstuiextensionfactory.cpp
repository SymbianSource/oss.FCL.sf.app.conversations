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
* Description: Extension factory for VIMPSTUi
*
*/


// INCLUDE FILES
#include "cvimpstuiextensionfactory.h"

// System includes
#include <coemain.h>
#include <coeutils.h>
#include <bautils.h>
#include <aknViewAppUi.h>
#include <StringLoader.h>

// Pbk2
#include <MPbk2UIExtensionView.h>
#include <CPbk2UIExtensionView.h>


// xSP extension manager
#include <MxSPViewMapper.h>
#include <MxSPContactManager.h>


#include "vimpstextentionuiuid.h"
#include <vimpstuires.rsg>


#include "cvimpstuiviewmanager.h"
#include "cvimpstuiextensionservice.h"

#include "cvimpstenginefactory.h"
#include "cvimpstuitabbedview.h"
#include "cvimpstuisearchview.h"
#include "tvimpstconsts.h"
#include "cvimpstuiblockview.h"

#include "uiservicetabtracer.h"
#include "cvimpstuimenuextension.h"

// ================= MEMBER FUNCTIONS =======================

inline CVIMPSTUiExtensionFactory::CVIMPSTUiExtensionFactory() :
											iCoeEnv( *CCoeEnv::Static() )
    {
    }


// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiExtensionFactory::ConstructL()
    {
    
    // Load the Resource File
	TRACER_AUTO;
	TFileName resPathWithoutDrive;
    resPathWithoutDrive.Zero();
    resPathWithoutDrive.Append( KResourceDir() );
    resPathWithoutDrive.Append( KResourceFileName() );
    
    TFileName path;    
    Dll::FileName(path); //get the location of the dll
    
    TDriveName drive( TParsePtrC( path ).Drive( ) ); // solve drive

    // leave to stack, LC method
    HBufC* resourceFilePath = HBufC::NewLC( KMaxResourcePathLength );
    TPtr ptr( resourceFilePath->Des() );
    ptr.Append( resPathWithoutDrive ); // without drive
    ptr.Insert( 0, drive ); // insert drive

    // NearestLanguageFile takes only TFileName
    path.Zero();
    path.Append( *resourceFilePath );

    if ( path.Length() != 0 )
        {
        BaflUtils::NearestLanguageFile( iCoeEnv.FsSession(), path );
        }
	
	CleanupStack::PopAndDestroy();

	iResourceOffset = iCoeEnv.AddResourceFileL(path);
    
	iEngineFactoryInstance = CVIMPSTEngineFactory::InstanceL();
	
    iEngine = CVIMPSTUIViewManager::NewL( 
        			KNullDesC(), *this );  
    iMenuExtension = CVIMPSTUIMenuExtension::NewL();  
    // Get's all sevices
    CreateServicesL();
     
    }


// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiExtensionFactory* CVIMPSTUiExtensionFactory::NewL()
    {
	TRACER_AUTO;
    CVIMPSTUiExtensionFactory* self = new (ELeave) CVIMPSTUiExtensionFactory;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::~CVIMPSTUiExtensionFactory
// --------------------------------------------------------------------------
//
CVIMPSTUiExtensionFactory::~CVIMPSTUiExtensionFactory()
    {
	TRACER_AUTO;
    delete iEngine;
    
    iCoeEnv.DeleteResourceFile( iResourceOffset );
    
    //for deleting iEngineFactoryInstance
    CVIMPSTEngineFactory::Release();
    
    delete iMenuExtension;
    
    }



// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::CreateViewL
// --------------------------------------------------------------------------
//
CxSPBaseView* CVIMPSTUiExtensionFactory::CreateViewL( TUid aViewId, CPbk2UIExtensionView& aView )
    {
	TRACER_AUTO;
    CxSPBaseView* view = NULL;
    
    TInt oldView;
	TInt err = iMapper->GetOldView( KIMCONTACTSEXTNIMPLEMENTATIONUID, aViewId.iUid, oldView );
	
    if( !err )
	    {
	    
        for ( TInt index = 0 ; 
              index < iEngine->ServiceCount() && !view ; 
              index++ )
            {
            if ( iEngine->Service( index ).HasViewId( oldView ) )
                {
				if ( IsTabbedId( oldView ) )
						{
	                		view = iEngine->Service( index ).TabbedViewL(
	                        *iMapper, 
	                        *iContactManager, 
	                        aView,
	                        oldView, 
	                        aViewId,
	                        iEngine->Service(index).ServiceEngine() );                
                		}
				else if ( IsSearchId( oldView ) )
						{
						view = iEngine->Service( index ).SearchViewL(
						*iMapper, 
						*iContactManager, 
						aView,
						oldView, 
						aViewId,
						iEngine->Service(index).ServiceEngine() );               
						}
				else if(IsBlockeId( oldView ))
					{
						view = iEngine->Service( index ).BlockViewL(
						*iMapper, 
						*iContactManager, 
						aView,
						oldView, 
						iEngine->Service(index).ServiceEngine() );               
					}
				}

			}
	    }
   
    return view;
    }
  
  
// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::RegisterViewMapper
// --------------------------------------------------------------------------
//    
void CVIMPSTUiExtensionFactory::RegisterViewMapper( MxSPViewMapper& aMapper )
	{
	iMapper = &aMapper;
	}
  
  
// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::ViewInfoResource
// --------------------------------------------------------------------------
//                               
TInt CVIMPSTUiExtensionFactory::ViewInfoResource() const
	{
	TRACER_AUTO;
	// Indicate KErrNotFound. This way static resources are not used but
    // xSP Extension Manager queries data. Exception: If IM is not
    // supported, indicate it via resource for simplicity
    TInt ret = KErrNotFound;
    
    // add the FeatureManager checks for IM here
    
    // Check for any service 
    if ( !iEngine->ServiceCount()  )
        {
        ret = R_IM_CONTACTS_VIEW_INFO;
        }
    
    return ret;
	}



// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::Id
// --------------------------------------------------------------------------
//    	
TUint32 CVIMPSTUiExtensionFactory::Id() const
	{
	return KIMCONTACTSEXTNIMPLEMENTATIONUID;
	}		


// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::Name()
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTUiExtensionFactory::Name() const
	{
	_LIT(KNull, " ");
	return KNull;	
	}


// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::RegisterContactManager
// --------------------------------------------------------------------------
//	
void CVIMPSTUiExtensionFactory::RegisterContactManager( MxSPContactManager& aManager )
	{
	iContactManager = &aManager;
	}



// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::SetVPbkContactManager
// --------------------------------------------------------------------------
//	
void CVIMPSTUiExtensionFactory::SetVPbkContactManager( CVPbkContactManager& /*aVPbkContactManager*/ )
	{	
	}

// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::UpdateStorePropertiesL
// --------------------------------------------------------------------------
//	
void CVIMPSTUiExtensionFactory::UpdateStorePropertiesL( CPbk2StorePropertyArray& /*aPropertyArray*/ )
	{	
	}
// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::ViewInfoCount
// --------------------------------------------------------------------------
//	
TInt CVIMPSTUiExtensionFactory::ViewInfoCount() const 
	{	
	return iEngine->ViewInfoCount();	
	}
// --------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::ViewInfo
// --------------------------------------------------------------------------
//	
const MxSPViewInfo* CVIMPSTUiExtensionFactory::ViewInfo( TInt aIndex ) const 
	{	
	return iEngine->ViewInfo( aIndex );	
	}
	

// ---------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::CreateServicesL()
// ---------------------------------------------------------------------------
//  
void CVIMPSTUiExtensionFactory::CreateServicesL()
    {
	TRACER_AUTO;
    // Get all available services   
    RPointerArray<MVIMPSTEngine> engines;
    
    if (iEngineFactoryInstance)
	    {
	    iEngineFactoryInstance->GetServiceEnginePtr(engines);      
	    TRACE("CreateServicesL() engines.Count(): %d", engines.Count());
    // iterate the service array
    for ( TInt index = 0; index < engines.Count() ; index++ )        
        {
        //ServiceId is used to create unique view id for each service, to avoid the reuse of view id of existing service to newly installed service
        TInt ServiceId = engines[index]->ServiceId();
        iEngine->AddNewClientL( ServiceId, *engines[index], 
                ( KVIMPSTTabbedViewIdBase + ServiceId ), ( KVIMPSTSearchViewIdBase + ServiceId), (KVIMPSTBlockedViewIdBase +ServiceId ), *iMenuExtension);
        }
    
    engines.Reset();
    engines.Close();
	    }
  
    }	
	
// ---------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::IsSearchId()
// ---------------------------------------------------------------------------
//  
TBool CVIMPSTUiExtensionFactory::IsSearchId( TUint aId )
    {
    return ( aId >= KVIMPSTSearchViewIdBase && aId<KVIMPSTBlockedViewIdBase);
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::IsSearchId()
// ---------------------------------------------------------------------------
//  
TBool CVIMPSTUiExtensionFactory::IsBlockeId( TUint aId )
    {
    return ( aId >= KVIMPSTBlockedViewIdBase );
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTUiExtensionFactory::IsTabbedId()
// ---------------------------------------------------------------------------
//  
TBool CVIMPSTUiExtensionFactory::IsTabbedId( TUint aId )
    {
    return ( aId >= KVIMPSTTabbedViewIdBase && aId < KVIMPSTSearchViewIdBase );
    }
	
//  End of File
