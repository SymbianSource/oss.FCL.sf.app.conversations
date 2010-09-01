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
* Description:  Publish and subscribe implementation.
*
*/

// INCLUDE FILES
#include    "cimalertnotifierimpl.h"
#include    "mimalertnotifierkeyobserver.h"
#include    "cimalertpropertyobserver.h"
#include    <e32property.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::CIMAlertNotifierImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMAlertNotifierImpl::CIMAlertNotifierImpl( MIMAlertNotifierKeyObserver* aObserver )
:iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMAlertNotifierImpl* CIMAlertNotifierImpl::NewL( MIMAlertNotifierKeyObserver* aObserver )
    {
    CIMAlertNotifierImpl* self = new( ELeave ) CIMAlertNotifierImpl( aObserver );
    return self;
    }

    
// Destructor
CIMAlertNotifierImpl::~CIMAlertNotifierImpl()
    {
    delete iPropWatcher;
    }

// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::SetInt
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMAlertNotifierImpl::SetInt( TNotifyKey aKey, TInt aValue )
	{
	TInt err = KErrNone;
	switch ( aKey ) 
		{
		case EMSGToneQuitKey:	
		    err = RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit, aValue );
			break;
		default:
			{
			return KErrNotFound;
			}
		}	
	return err;
	}    

// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::GetInt
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMAlertNotifierImpl::GetInt( TNotifyKey aKey, TInt& aValue )
	{
	TInt err = KErrNone;
	switch ( aKey ) 
		{
		case EMSGToneQuitKey:
			{
			err = RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit, aValue );		
			break;
			}
		default:
			{
			err = KErrNotFound;	
			break;
			}
		}
	return err;
	}
	
// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::ListenKeyChanges
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMAlertNotifierImpl::ListenKeyChanges( TNotifyKey aKey )
	{
	TInt err = KErrNone;
	delete iPropWatcher;
	iPropWatcher = NULL;
	switch ( aKey ) 
		{
		case EMSGToneQuitKey:
			{
			TRAP( err, iPropWatcher = CIMAlertPropertyObserver::NewL( *this ) );
			if( !err )
			    {
			    TRAP( err, iPropWatcher->ObservePropertyChangeL( 
			                KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit ) );
			    }
			break;
			}
		default:
			{
			err = KErrNotFound;
			break;
			}
		}
	return err;
	}

// -----------------------------------------------------------------------------
// CIMAlertNotifierImpl::HandlePropertyNotificationEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMAlertNotifierImpl::HandlePropertyNotificationEventL( TUid /* aCategory */, TUint aKey )
    {
    TInt value( 0 );
    User::LeaveIfError( 
          RProperty::Get( KPSUidCoreApplicationUIs, aKey, value ) );
    TUpdatedKey key;
    key.iUid = KPSUidCoreApplicationUIs;
    key.iKey = aKey;
    key.iValue = value;
    iObserver->HandleKeyChange( key );
    }

//  End of File  
