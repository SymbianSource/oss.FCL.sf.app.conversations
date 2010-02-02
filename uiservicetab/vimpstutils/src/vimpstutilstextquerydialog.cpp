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
* Description:  Text query dialog that accepts empty input
*
*/



// INCLUDE FILES
#include "vimpstutilstextquerydialog.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTUtilsTextQueryDialog::CVIMPSTUtilsTextQueryDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsTextQueryDialog::CVIMPSTUtilsTextQueryDialog( TDes& aDataText, 
                                        const TTone& aTone /*= ENoTone*/,
                                        TBool aLeftSoftkeyAlwaysVisible /*= ETrue*/,
                                        TBool aDomainSelectionQuery /*= EFalse*/ )
    : CAknTextQueryDialog( aDataText, aTone ),
      iLeftSoftkeyAlwaysVisible( aLeftSoftkeyAlwaysVisible ),
      iDomainSelectionQuery( aDomainSelectionQuery )
    {
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsTextQueryDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsTextQueryDialog* CVIMPSTUtilsTextQueryDialog::NewL( TDes& aDataText, 
                                              const TTone& aTone /*= ENoTone*/,
                                              TBool aLeftSoftkeyAlwaysVisible /*= ETrue*/,
                                              TBool aDomainSelectionQuery /*= EFalse*/ )
    {
    CVIMPSTUtilsTextQueryDialog* self = new( ELeave ) CVIMPSTUtilsTextQueryDialog( 
                                                            aDataText, aTone,
                                                            aLeftSoftkeyAlwaysVisible,
                                                            aDomainSelectionQuery );
    return self;
    }

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
//    

EXPORT_C CVIMPSTUtilsTextQueryDialog::~CVIMPSTUtilsTextQueryDialog()
    {    
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsTextQueryDialog::UpdateLeftSoftKeyL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTUtilsTextQueryDialog::UpdateLeftSoftKeyL()
    {
    if ( iLeftSoftkeyAlwaysVisible )
        {
        // do nothing so the left softkey is always visible
        return;
        }
    else
        {
        CAknTextQueryDialog::UpdateLeftSoftKeyL();
        }
	}

// -----------------------------------------------------------------------------
// CVIMPSTUtilsTextQueryDialog::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TKeyResponse CVIMPSTUtilsTextQueryDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                         TEventCode aType )
    {
    TInt oldLen = 0;
    CEikEdwin* edwin = NULL;
    // If this is launched after domain selection query
    // check editor state
    if ( iDomainSelectionQuery )
        {
        CCoeControl* ctrl = ControlOrNull( EGeneralQuery );
        if ( ctrl )
            {
            CAknQueryControl* query = static_cast<CAknQueryControl*>( ctrl );
            ctrl = query->ControlByLayoutOrNull( EDataLayout );
            if ( ctrl )
                {
                edwin = static_cast<CEikEdwin*>( ctrl );
                oldLen = edwin->TextLength();
                }
            }
        }
        
    TKeyResponse response = 
        CAknTextQueryDialog::OfferKeyEventL( aKeyEvent, aType );
    
    if ( iDomainSelectionQuery && edwin
        && aKeyEvent.iCode != EKeyOK )
        {
        if ( oldLen > 0
            && response == EKeyWasConsumed
            && edwin->TextLength() == 0 )
            {
            // User has cleared editor -> exit dialog
            TryExitL( EAknSoftkeyExit );
            }
        }
        
    return response;
	}
	
// -----------------------------------------------------------------------------
// CVIMPSTUtilsTextQueryDialog::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CVIMPSTUtilsTextQueryDialog::OkToExitL( TInt aButtonId )
    {
    if ( aButtonId == EAknSoftkeyExit && iDomainSelectionQuery )
        {
        return ETrue;
        }
    else
        {
        return CAknTextQueryDialog::OkToExitL( aButtonId );
        }
    }
//  End of File
