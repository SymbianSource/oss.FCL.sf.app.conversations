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
* Description:  Publish & Subscribe property change observer implementation
*
*/ 

// INCLUDE FILES
#include "cimalertpropertyobserver.h"
#include "mimalertpropertynotificationobserver.h"

#include <bautils.h>
#include <barsc.h>
#include <e32property.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::CIMAlertPropertyObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMAlertPropertyObserver::CIMAlertPropertyObserver( 
        MIMAlertPropertyNotificationObserver& aObserver )
    : CActive( CActive::EPriorityLow ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMAlertPropertyObserver* CIMAlertPropertyObserver::NewL( 
        MIMAlertPropertyNotificationObserver& aObserver )
    {
    CIMAlertPropertyObserver* self = new( ELeave ) CIMAlertPropertyObserver( aObserver );
    return self;
    }


// Destructor
CIMAlertPropertyObserver::~CIMAlertPropertyObserver()
    {
    if( IsActive() )
        {
        Cancel();
        }
    }


// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::ObservePropertyChangeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMAlertPropertyObserver::ObservePropertyChangeL( TUid aCategory, TUint aKey )
    {
	if( IsActive() )
		{
		return;
		}
    User::LeaveIfError( iProperty.Attach( aCategory, aKey ) );
	iKey = aKey;
	iCategory = aCategory;
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::CancelObserve
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMAlertPropertyObserver::CancelObserve()
    {
    if( IsActive() )
        {
        Cancel();
        }
	}

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMAlertPropertyObserver::RunL()
    {
	iProperty.Subscribe( iStatus );
	SetActive();
    iObserver.HandlePropertyNotificationEventL( iCategory, iKey );      
    }

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMAlertPropertyObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::Category
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUid CIMAlertPropertyObserver::Category()
	{
	return iCategory;
	}

// -----------------------------------------------------------------------------
// CIMAlertPropertyObserver::Key
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CIMAlertPropertyObserver::Key()
	{
	return iKey;
	}

//  End of File
