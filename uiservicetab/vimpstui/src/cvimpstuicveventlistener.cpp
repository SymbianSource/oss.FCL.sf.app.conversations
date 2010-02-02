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
* Description:   Implementation for RProperty where conversation view event handled.
*
*/


// INCLUDE FILES

#include "cvimpstuicveventlistener.h"
#include "cvimpstuitabbedview.h"
#include "imcvuiliterals.h"
#include "vimpstui.hrh"

//system includes
#include <avkon.hrh>

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::CVIMPSTUiSearchView
// --------------------------------------------------------------------------
//
CVIMPSTUICvEventListener::CVIMPSTUICvEventListener( CVIMPSTUiTabbedView& aTabbedView )
    : CActive( CActive::EPriorityStandard ),
    iTabbedView( aTabbedView )
    {
    CActiveScheduler::Add( this );
    }
    
// --------------------------------------------------------------------------
// CVIMPSTUICvEventListener::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUICvEventListener::ConstructL()
    {
    RProperty::Define(KIMCVPropertyUid, KIMCVConnectedKey, RProperty::EInt );
    RProperty::Set(KIMCVPropertyUid, KIMCVConnectedKey, EIMCVUiEventNone ); //0 reset property
    User::LeaveIfError( iProperty.Attach(KIMCVPropertyUid, KIMCVConnectedKey) );
    iProperty.Subscribe(iStatus);
    SetActive();
    }


// --------------------------------------------------------------------------
// CVIMPSTUICvEventListener::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUICvEventListener* CVIMPSTUICvEventListener::NewL(CVIMPSTUiTabbedView& aTabbedView )
    {
    CVIMPSTUICvEventListener* self = new(ELeave) CVIMPSTUICvEventListener( aTabbedView );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------
// CVIMPSTUICvEventListener::~CVIMPSTUICvEventListener()
// C++ Destructor 
// ---------------------------------------------------------
//
CVIMPSTUICvEventListener::~CVIMPSTUICvEventListener()
    {    
    Cancel (); // Cancel any request, if outstanding
    iProperty.Close (); // Destroy the property object
    }
// ---------------------------------------------------------
// CVIMPSTUICvEventListener::RunL()
// ---------------------------------------------------------
//
void CVIMPSTUICvEventListener::RunL()
    {
        
    TInt eventValue = KErrNone;
    
    TInt error = RProperty::Get(KIMCVPropertyUid,KIMCVConnectedKey, eventValue  );
 
    if( eventValue == static_cast<TInt>( EIMCVUiEventExitOnly ) )
        {
        // do service tab exit and keep connection open
        iTabbedView.HandleCommandL( EAknCmdExit );
        }
    else if( eventValue == static_cast<TInt>( EIMCVUiEventLogoutExit ) ) 
        {
        // do logout and exit
        iTabbedView.HandleCommandL( EIMCmdLogoutExit );
        }
    // reset property to zero
    RProperty::Set(KIMCVPropertyUid, KIMCVConnectedKey, EIMCVUiEventNone ); 
    }
// ---------------------------------------------------------
// CVIMPSTUICvEventListener::RunL()
// ---------------------------------------------------------
//
void CVIMPSTUICvEventListener::DoCancel()
    {    
    iProperty.Cancel();
    }
// ---------------------------------------------------------
// CVIMPSTUICvEventListener::StartListening()
// ---------------------------------------------------------
//
void CVIMPSTUICvEventListener::StartListening()
    {
    if(!IsActive() )
        {
        iProperty.Subscribe(iStatus);
        SetActive(); // Tell scheduler a request is active  
        }
    }

//  End of File  

