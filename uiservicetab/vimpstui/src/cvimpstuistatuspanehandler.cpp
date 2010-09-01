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
* Description:  Tabbed view declaration
*
*/

#include "cvimpstuistatuspanehandler.h"

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::CCVIMPSTUiStatusPaneHandler
// --------------------------------------------------------------------------
CCVIMPSTUiStatusPaneHandler::CCVIMPSTUiStatusPaneHandler()
    {
    // No implementation required
    iNaviPush = EFalse;
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::~CCVIMPSTUiStatusPaneHandler
// --------------------------------------------------------------------------
CCVIMPSTUiStatusPaneHandler::~CCVIMPSTUiStatusPaneHandler()
    {
    if( iNaviPush && iNaviPane )
        {
        iNaviPane->Pop( iNaviDeco );
        }
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::NewLC()
// --------------------------------------------------------------------------
CCVIMPSTUiStatusPaneHandler* CCVIMPSTUiStatusPaneHandler::NewLC()
    {
    CCVIMPSTUiStatusPaneHandler* self = new (ELeave)CCVIMPSTUiStatusPaneHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::NewL()
// --------------------------------------------------------------------------
CCVIMPSTUiStatusPaneHandler* CCVIMPSTUiStatusPaneHandler::NewL()
    {
    CCVIMPSTUiStatusPaneHandler* self=CCVIMPSTUiStatusPaneHandler::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::ConstructL()
// --------------------------------------------------------------------------
void CCVIMPSTUiStatusPaneHandler::ConstructL()
    {
    iEnv = CEikonEnv::Static();
    iStatusPane = iEnv->AppUiFactory()->StatusPane();

    if( iStatusPane->PaneCapabilities( TUid::Uid(EEikStatusPaneUidTitle) ).IsPresent() )
        iTitlePane = static_cast<CAknTitlePane*>( iStatusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        
    if( iStatusPane->PaneCapabilities( TUid::Uid(EEikStatusPaneUidContext) ).IsPresent()  )
        iContextPane = static_cast<CAknContextPane*> ( iStatusPane->ControlL( TUid::Uid( EEikStatusPaneUidContext ) ) );
        
    if( iStatusPane->PaneCapabilities( TUid::Uid(EEikStatusPaneUidNavi)).IsPresent()  )
        iNaviPane = static_cast<CAknNavigationControlContainer*>( iStatusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::SetPictureL()
// --------------------------------------------------------------------------
void CCVIMPSTUiStatusPaneHandler::SetPictureL( CFbsBitmap* aBitmap, CFbsBitmap* aMask, TBool aNaviPush )
    {
    if( iContextPane->IsVisible() )
        {
        iContextPane->SetPicture( aBitmap, aMask );
        }
    else if( aNaviPush )
        {
        iNaviDeco = iNaviPane->CreateNavigationImageL( aBitmap, aMask );
        iNaviPane->PushL( *iNaviDeco );
        iNaviPush = aNaviPush;
        }
    else 
        {
        delete aBitmap;
        aBitmap = NULL;
        delete aMask;
        aMask = NULL;
        }
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::SetTextL()
// --------------------------------------------------------------------------
void CCVIMPSTUiStatusPaneHandler::SetTextL( const TDesC& aText )
    {
    iTitlePane->SetTextL( aText );
    }

// --------------------------------------------------------------------------
// CCVIMPSTUiStatusPaneHandler::ClearPaneL()
// --------------------------------------------------------------------------
void CCVIMPSTUiStatusPaneHandler::ClearPaneL( )
    {
    if( iContextPane->IsVisible() )
        {
        iContextPane->SetPicture( NULL, NULL);
        }
    if( iNaviPush )
        {
        iNaviPane->Pop( iNaviDeco );
        iNaviPush = EFalse;
        }
    }

//END OF FILE

