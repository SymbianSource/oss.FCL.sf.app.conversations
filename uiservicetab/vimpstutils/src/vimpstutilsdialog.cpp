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
* Description:  Utils for IMUiServiceTab modules.
*
*/



// INCLUDE FILES
#include    "vimpstutilsdialog.h"
#include    "vimpstutilstextquerydialog.h"

#include    <StringLoader.h>
#include 	<AknQueryDialog.h> 
#include    <vimpstuires.rsg>
#include 	<AknQueryDialog.h>




// ---------------------------------------------------------
// VIMPSTUtilsDialog::DisplayNoteDialogL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt VIMPSTUtilsDialog::DisplayNoteDialogL( const TDesC& aText,
        const TInt aButtonsResourceId, /*= R_AVKON_SOFTKEYS_EMPTY*/
        const CAknNoteDialog::TTimeout& aTimeout /*= CAknNoteDialog::EShortTimeout*/ )
	{
	// CodeScanner warning ignored because CS does not
    // notice that PrepareLC puts the dialog to cleanupstack
    CAknNoteDialog* dlg = new ( ELeave ) CAknNoteDialog( // CSI: 35 # See comment above
		CAknNoteDialog::EConfirmationTone , aTimeout );

	//TODO : Modify the function body when required later
	
	dlg->PrepareLC( R_IM_INFO_NOTE_TEMPLATE );
	dlg->SetTextL( aText );
	
    CEikButtonGroupContainer& cba = dlg->ButtonGroupContainer();
	cba.SetCommandSetL( aButtonsResourceId );

	return dlg->RunLD();
	}


// ---------------------------------------------------------
// VIMPSTUtilsDialog::DisplayErrorNoteL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt VIMPSTUtilsDialog::DisplayErrorNoteL( const TDesC& aText, 
													TBool aSoftkeyOk /*= EFalse*/ )
	{
	//TODO : Modify the function body when required later
	if( aSoftkeyOk )
	    {
	    return VIMPSTUtilsDialog::DisplayQueryDialogL( R_IM_ERROR_NOTE_TEMPLATE_WITH_OK, aText );
	    }
    else
        {
        // CodeScanner warning ignored because CS does not
        // notice that PrepareLC puts the dialog to cleanupstack
        CAknNoteDialog* dlg =   // CSI: 35 # See comment above
            new ( ELeave ) CAknNoteDialog( CAknNoteDialog::EErrorTone ,
                                           CAknNoteDialog::ELongTimeout );
	    dlg->PrepareLC( R_IM_ERROR_NOTE_TEMPLATE );
	    dlg->SetTextL( aText );
        return dlg->RunLD();
        }
	}


// ---------------------------------------------------------
// VIMPSTUtilsDialog::DisplayQueryDialogL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt VIMPSTUtilsDialog::DisplayQueryDialogL( TInt aDialogResourceId, 
            											const TDesC& aPrompt )
	{
	CAknQueryDialog* dlg = new (ELeave) CAknQueryDialog(
											CAknQueryDialog::ENoTone );
	if( aPrompt != KNullDesC )
		{
		CleanupStack::PushL( dlg );
		dlg->SetPromptL( aPrompt );
		CleanupStack::Pop( dlg );
		}

	return dlg->ExecuteLD( aDialogResourceId );
	}

// ---------------------------------------------------------
// VIMPSTUtilsDialog::DisplayTextQueryDialogL()
// Displays text query dialog
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt VIMPSTUtilsDialog::DisplayTextQueryDialogL( 
        TDes& aDataText, 
        const TInt aTitleResourceId, 
        const TInt aDialogResourceId,
        TBool aPredictiveInput /* = EFalse */,
        TBool aLeftSoftkeyAlwaysVisible /* = EFalse */,
        TLeftSoftkey /*aLeftSoftkey  = EDefault */,
        TInitialSelectionMode aSelectionMode /* = ESelectAll */,
        TBool aDomainSelectionQuery /* = EFalse */ )
    {
    // CodeScanner warning ignored because CS does not
    // notice that PrepareLC puts the dialog to cleanupstack
    CAknTextQueryDialog* dlg = CVIMPSTUtilsTextQueryDialog::NewL( aDataText, // CSI: 35 # See comment above
            CAknQueryDialog::ENoTone,
            aLeftSoftkeyAlwaysVisible,
            aDomainSelectionQuery );

    dlg->PrepareLC( aDialogResourceId );
    HBufC* text = CEikonEnv::Static()->AllocReadResourceLC( aTitleResourceId );
    dlg->SetPromptL( *text );
    dlg->SetPredictiveTextInputPermitted( aPredictiveInput );

    CleanupStack::PopAndDestroy( text );

    // Set correct selection mode
    switch ( aSelectionMode )
        {
        case ESelectNone:
            {
            CCoeControl* ctrl = dlg->ControlOrNull( EGeneralQuery );
            if ( ctrl )
                {
                CAknQueryControl* query = static_cast<CAknQueryControl*>( ctrl );
                ctrl = query->ControlByLayoutOrNull( EDataLayout );
                if ( ctrl )
                    {
                    CEikEdwin* edwin = static_cast<CEikEdwin*>( ctrl );
                    edwin->AddFlagToUserFlags( CEikEdwin::ENoAutoSelection );
                    }                
                }            
            break;
            }
        default:
            {
            // Use default flags in edwin
            break;
            }
        }
    return ( dlg->RunLD() );	
    }

// ---------------------------------------------------------
// VIMPSTUtilsDialog::DisplayListQueryDialogL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt VIMPSTUtilsDialog::DisplayListQueryDialogL( TInt* aSelectedIndex, 
        MDesCArray* aItemList, 
        const TInt aDialogResourceId,
        const TDesC& aAlternativeTitle /*KNullDesC*/,
        const TDesC& aListEmptyText /*= KNullDesC*/,
        const TBool aIsForwardQuery /* EFalse */,
        const TBool /*aIsOnlySelect  = EFalse */ )

    {
    CAknListQueryDialog* dlg = new (ELeave) CAknListQueryDialog( 
                                                    aSelectedIndex );
    dlg->PrepareLC( aDialogResourceId );
    dlg->SetItemTextArray( aItemList );
    dlg->SetOwnershipType( ELbmDoesNotOwnItemArray );

	if ( aAlternativeTitle != KNullDesC ) 
		{
		dlg->SetHeaderTextL( aAlternativeTitle );
		}

	if ( aItemList->MdcaCount() == 0 ) 
		{
		CEikButtonGroupContainer& cba = dlg->ButtonGroupContainer();
		cba.SetCommandSetL( R_AVKON_SOFTKEYS_CANCEL );		
		}
	else
		{
		if ( aIsForwardQuery )
			{
			CEikButtonGroupContainer& cba = dlg->ButtonGroupContainer();
			cba.SetCommandSetL( R_AVKON_SOFTKEYS_SELECT_CANCEL__SELECT );
			}
		}
    	
    dlg->ListBox()->SetCurrentItemIndex( *aSelectedIndex );

	if( aListEmptyText != KNullDesC )
		{
		dlg->ListBox()->View()->SetListEmptyTextL( aListEmptyText );
		}

	return ( dlg->RunLD() );
	}


//  End of File
