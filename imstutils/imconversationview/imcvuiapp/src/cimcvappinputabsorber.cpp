/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Instance of this class catches key inputs when it is activated
*
*/



// INCLUDE FILES
#include    "cimcvappinputabsorber.h"
#include	<eikenv.h>
#include	<eikappui.h>
#include	<avkon.rsg>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppInputAbsorber::CIMCVAppInputAbsorber
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppInputAbsorber::CIMCVAppInputAbsorber()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppInputAbsorber::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppInputAbsorber::ConstructL()
    {
	CreateWindowL();
	SetExtent( TPoint( 0, 0 ), TSize( 0, 0 ) );
	ActivateL();
	ClaimPointerGrab( ETrue );	
	iAppUi = iEikonEnv->EikAppUi();	
    }

// -----------------------------------------------------------------------------
// CIMCVAppInputAbsorber::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppInputAbsorber* CIMCVAppInputAbsorber::NewL()
    {
    CIMCVAppInputAbsorber* self = new( ELeave ) CIMCVAppInputAbsorber;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CIMCVAppInputAbsorber::~CIMCVAppInputAbsorber()
    {
    if ( iIsCaptured ) 
        {
        if ( iAppUi ) 
            {
            iAppUi->RemoveFromStack( this );
            }
        }
    }

// ---------------------------------------------------------
// CIMCVAppInputAbsorber::CaptureEventsL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppInputAbsorber::CaptureEventsL()
	{
	if ( !iIsCaptured ) 
		{
		iAppUi->AddToStackL( this );
        SetPointerCapture();
		}
    
	iIsCaptured = ETrue;	
	}

// ---------------------------------------------------------
// CIMCVAppInputAbsorber::Release()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppInputAbsorber::Release()
	{
	if ( iIsCaptured ) 
		{
		iAppUi->RemoveFromStack( this );
		SetPointerCapture( EFalse );
		}	

	iIsCaptured = EFalse;
	}


// ---------------------------------------------------------
// CIMCVAppInputAbsorber::IsCaptured()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppInputAbsorber::IsCaptured() const
	{
	return iIsCaptured;
	}


// -----------------------------------------------------------------------------
// CIMCVAppInputAbsorber::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CIMCVAppInputAbsorber::OfferKeyEventL( const TKeyEvent& /*aKeyEvent*/, 
											  TEventCode /*aType*/ )
	{
    // consume all keys
	return EKeyWasConsumed;
	}

// -----------------------------------------------------------------------------
// CIMCVAppInputAbsorber::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppInputAbsorber::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // For debugging purposes. 
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }

//  End of File  
