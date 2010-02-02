/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class subscribes and listens to the rproperty
*
*/


// INCLUDE FILES
#include <e32property.h>
#include "imstatuspaneindicator.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::CIMStatusPaneIndicator
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CIMStatusPaneIndicator::CIMStatusPaneIndicator()
 :CActive(CActive::EPriorityStandard )
    {
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CIMStatusPaneIndicator::ConstructL()
    {
    iSenderId = HBufC::NewL(KMAXLENGTH);
    // attach the properties.
    User::LeaveIfError( iProperty.Attach(KIMStatusProperty,EIMStatusPropertyMessageCount  ) );
    User::LeaveIfError( iProperty.Attach(KIMStatusProperty,EIMStatusPropertyServiceId  ) );
    User::LeaveIfError( iProperty.Attach(KIMStatusProperty,EIMMultipleSender  ) );
    User::LeaveIfError( iProperty.Attach(KIMStatusProperty,EIMStatusPropertySenderId  ) );
    //User::LeaveIfError( iProperty.Attach(KIMStatusProperty,EIMStatusPropertyMessageCount  ) );
    CActiveScheduler::Add(this);
    // initial subscription and process current property value
    RunL();
    }

// -----------------------------------------------------------------------------
// CIMStatusPaneIndicator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMStatusPaneIndicator* CIMStatusPaneIndicator::NewL()
    {
    CIMStatusPaneIndicator* self = 
        new( ELeave ) CIMStatusPaneIndicator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CIMStatusPaneIndicator::~CIMStatusPaneIndicator()
    {
    delete iSenderId;
    iProperty.Close();
    Cancel();
    }
// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::DoCancel()
// ---------------------------------------------------------------------------
//
void CIMStatusPaneIndicator::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::RunL()
// ---------------------------------------------------------------------------
//
void CIMStatusPaneIndicator::RunL()
  {
     // resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe( iStatus );
    SetActive();
    if( iProperty.Get(KIMStatusProperty, EIMStatusPropertyMessageCount,iMsgCount ) == KErrNotFound )
        {
        //do necessary action
        }
    if( iProperty.Get(KIMStatusProperty, EIMStatusPropertyServiceId,iServiceId ) == KErrNotFound )
       {
       //do necessary action
       }
    if( iProperty.Get(KIMStatusProperty, EIMMultipleSender,iMultipleSenders ) == KErrNotFound )
          {
          //do necessary action
          }
    TBuf<KMAXLENGTH> senderId;
    if( iProperty.Get(KIMStatusProperty, EIMStatusPropertySenderId,senderId) == KErrNotFound )
       {
       //do necessary action
       }
    if(iSenderId)
       {
       delete iSenderId;
       iSenderId = NULL;
       }
    iSenderId = senderId.AllocL();
  }
// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::MsgCount()
// ---------------------------------------------------------------------------
//
TInt CIMStatusPaneIndicator::MsgCount()
    {
    return iMsgCount;
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::ServiceId()
// ---------------------------------------------------------------------------
//
TInt CIMStatusPaneIndicator::ServiceId()
    {
    return iServiceId;
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::SenderId()
// ---------------------------------------------------------------------------
//
const TDesC& CIMStatusPaneIndicator::SenderId()
    {
    if(iSenderId)
        {
        return *iSenderId;
        }
    return KNullDesC() ;
    }

// ---------------------------------------------------------------------------
// CIMStatusPaneIndicator::IsMultipleSenders()
// ---------------------------------------------------------------------------
//
TInt CIMStatusPaneIndicator::IsMultipleSenders()
    {
    return iMultipleSenders;
    }
//  End of File
