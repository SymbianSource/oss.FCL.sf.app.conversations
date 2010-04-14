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
* Description:  Class that provides centralized access for UI classes 
*                to logic handling
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <coeaui.h>

#include "cvimpstuiviewmanager.h"
#include "cvimpstuiextensionservice.h"
#include "cvimpstuiextensionviewinfo.h"
#include "cvimpstuiextensionfactory.h"
#include "cvimpstuimenuextension.h"
#include "uiservicetabtracer.h"



// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::CVIMPSTUIViewManager
// ---------------------------------------------------------------------------
// 
CVIMPSTUIViewManager::CVIMPSTUIViewManager(
    const TDesC& aServiceName,
    CVIMPSTUiExtensionFactory& aExtensionFactory ):
    iServiceName( aServiceName ),
    iExtensionFactory( aExtensionFactory )
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIViewManager::ConstructL()
    {
    
    iEikEnv = CEikonEnv::Static();        
        
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUIViewManager* CVIMPSTUIViewManager::NewL(
    const TDesC& aSpsHandler,
    CVIMPSTUiExtensionFactory& aExtensionFactory )
    {    
    CVIMPSTUIViewManager* self = NewLC( aSpsHandler,
        aExtensionFactory );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTUIViewManager* CVIMPSTUIViewManager::NewLC(
    const TDesC& aSpsHandler,
    CVIMPSTUiExtensionFactory& aExtensionFactory )
    {
    CVIMPSTUIViewManager* self =
        new (ELeave) CVIMPSTUIViewManager( aSpsHandler,aExtensionFactory );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::~CVIMPSTUIViewManager
// ---------------------------------------------------------------------------
// 
CVIMPSTUIViewManager::~CVIMPSTUIViewManager()
    {   
    
    iViewInfos.Reset();
    iViewInfos.Close();
    
   
    iServices.ResetAndDestroy();
    iServices.Close();       
    
    }
    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::AddNewClientL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIViewManager::AddNewClientL( TUint aServiceId, MVIMPSTEngine& aEngine,
    TInt aTabbedViewId,TInt aSearchViewId,TInt aBlockedViewId ,CVIMPSTUIMenuExtension& aMenuExtension )
    {    
    
	TRACER_AUTO;
    CVIMPSTUIExtensionService* service =
        CVIMPSTUIExtensionService::NewL( 
            aServiceId,
            aTabbedViewId,
            aSearchViewId,
            aBlockedViewId,
            aEngine.ServiceName(),   
            aMenuExtension,
            aEngine
             );
    iServices.Append( service );

    iViewInfos.Append( service->TabbedViewInfoL() );         
    iViewInfos.Append( service->SearchViewInfo() );   
    iViewInfos.Append( service->BlockedViewInfo() ); 
    }    
    
    
  
// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::EikEnv()
// ---------------------------------------------------------------------------
//  
CEikonEnv& CVIMPSTUIViewManager::EikEnv()
    {
    return *iEikEnv;
    }    


// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::ViewInfoCount()
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTUIViewManager::ViewInfoCount() const 
    {
    return iViewInfos.Count();
    } 
    
// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::ViewInfo()
// ---------------------------------------------------------------------------
//  
const MxSPViewInfo* CVIMPSTUIViewManager::ViewInfo( 
    TInt aIndex ) const 
    {
    return static_cast<MxSPViewInfo*>( iViewInfos[ aIndex ] );    
    }    
    
// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::ServiceCount()
// ---------------------------------------------------------------------------

TInt CVIMPSTUIViewManager::ServiceCount() const 
    {
    return iServices.Count();    
    }    

// ---------------------------------------------------------------------------
// CVIMPSTUIViewManager::ServiceCount()
// ---------------------------------------------------------------------------
//  
CVIMPSTUIExtensionService& CVIMPSTUIViewManager::Service( 
    TInt aIndex ) const 
    {
    return *iServices[ aIndex ];    
    }    



// End of file
