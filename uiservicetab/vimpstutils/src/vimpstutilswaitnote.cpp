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
* Description:  Helper class for wait notes
*
*/



// INCLUDE FILES
#include    "vimpstutilswaitnote.h"

#include    <vimpstuires.rsg>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::CVIMPSTUtilsWaitNote
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTUtilsWaitNote::CVIMPSTUtilsWaitNote()
    {
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTUtilsWaitNote::ConstructL( const TDesC& aText,
							  TBool aShowImmediately,
        					  TBool aCanBeCanceledByUser,
        					  MVIMPSTUtilsWaitNoteObserver* aObserver )
    {
	// don't give dialog pointer as a parameter because
	// it gets invalid after deletion of this instance
	iWaitDialog = new( ELeave )CAknWaitDialog( NULL, aShowImmediately );
	if( aCanBeCanceledByUser )
	    {
        iWaitDialog->PrepareLC( R_IM_WAIT_NOTE_TEMPLATE_WITH_CANCEL );	    
	    }
	else
	    {
	    iWaitDialog->PrepareLC( R_IM_WAIT_NOTE_TEMPLATE );
	    }    
    iWaitDialog->SetTone( CAknNoteDialog::ENoTone );
    iWaitDialog->SetTextL( aText );
    iWaitDialog->SetCallback( this );

    SetObserver( aObserver );
    iWaitDialog->RunLD();   // CSI: 50 # iWaitDialog is not owned by us
    }


// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::ShowWaitNoteL
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsWaitNote* CVIMPSTUtilsWaitNote::ShowWaitNoteL( const TDesC& aText,
		        			 			  TBool aShowImmediately /*= EFalse*/,
        								  TBool aCanBeCanceledByUser /*= EFalse*/,
        								  MVIMPSTUtilsWaitNoteObserver* aObserver /*= NULL*/ )
    {
    CVIMPSTUtilsWaitNote* self = ShowWaitNoteLC(aText,aShowImmediately,aCanBeCanceledByUser,aObserver);
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::ShowWaitNoteLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsWaitNote* CVIMPSTUtilsWaitNote::ShowWaitNoteLC( const TDesC& aText,
		        			 			  TBool aShowImmediately /*= EFalse*/,
        								  TBool aCanBeCanceledByUser /*= EFalse*/,
        								  MVIMPSTUtilsWaitNoteObserver* aObserver /*= NULL*/ )
    {
    CVIMPSTUtilsWaitNote* self = new( ELeave ) CVIMPSTUtilsWaitNote();
    CleanupStack::PushL( self );
    self->ConstructL( aText, aShowImmediately, aCanBeCanceledByUser, aObserver );
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::ShowWaitNoteLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsWaitNote* CVIMPSTUtilsWaitNote::ShowWaitNoteLC( TInt aTextResource,
		        			 			  TBool aShowImmediately /*= EFalse*/,
        								  TBool aCanBeCanceledByUser /*= EFalse*/,
        								  MVIMPSTUtilsWaitNoteObserver* aObserver /*= NULL*/ )
    {
	CVIMPSTUtilsWaitNote* self = new( ELeave ) CVIMPSTUtilsWaitNote();
    CleanupStack::PushL( self );

    HBufC* text = CCoeEnv::Static()->AllocReadResourceLC( aTextResource );
    self->ConstructL( *text, aShowImmediately, aCanBeCanceledByUser, aObserver );
    CleanupStack::PopAndDestroy( text );

    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::ShowWaitNoteL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CVIMPSTUtilsWaitNote* CVIMPSTUtilsWaitNote::ShowWaitNoteL( TInt aTextResource,
		        			 			 TBool aShowImmediately /*= EFalse*/,
        								 TBool aCanBeCanceledByUser /*= EFalse*/,
        								 MVIMPSTUtilsWaitNoteObserver* aObserver /*= NULL*/ )
    {
	CVIMPSTUtilsWaitNote* self = ShowWaitNoteLC( aTextResource,
	                                    aShowImmediately,
	                                    aCanBeCanceledByUser,
	                                    aObserver );
    
    CleanupStack::Pop( self );
    return self;
    }




// Destructor
CVIMPSTUtilsWaitNote::~CVIMPSTUtilsWaitNote()
    {
	DismissDialog();
    }


// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::DialogDismissedL
// Called when dialog is dismissed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTUtilsWaitNote::DialogDismissedL( TInt aButtonId )
    {
    iWaitDialog = NULL;
    if( iObserver )
        {
        iObserver->NoteCanceled( aButtonId );
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::DismissDialog
// Dismisses the dialog
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTUtilsWaitNote::DismissDialog()
	{
	if( iWaitDialog )
        {
        iWaitDialog->SetCallback( NULL );
        TRAPD( err, iWaitDialog->ProcessFinishedL() );
        
        if( err != KErrNone )
            {
            // don't know for sure that ProcessFinishedL deletes the dialog
            // in all cases, so let's delete it
            delete iWaitDialog;
            iWaitDialog = NULL;
            CActiveScheduler::Current()->Error( err );
            }

        TRAP_IGNORE( DialogDismissedL( 0 ) );
        }
	}

// -----------------------------------------------------------------------------
// CVIMPSTUtilsWaitNote::SetObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTUtilsWaitNote::SetObserver( MVIMPSTUtilsWaitNoteObserver* aObserver )
    {
    iObserver = aObserver;
    }

//  End of File
