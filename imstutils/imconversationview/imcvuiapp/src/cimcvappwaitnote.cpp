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
#include    "cimcvappwaitnote.h"

#include    <imcvuiapp.rsg>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::CIMCVAppWaitNote
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppWaitNote::CIMCVAppWaitNote()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppWaitNote::ConstructL( const TDesC& aText,
							  TBool aShowImmediately,
        					  TBool /*aCanBeCanceledByUser*/,
        					  MIMCVWaitNoteObserver* aObserver )
    {
	// don't give dialog pointer as a parameter because
	// it gets invalid after deletion of this instance
	iWaitDialog = new( ELeave )CAknWaitDialog( NULL, aShowImmediately );

    iWaitDialog->PrepareLC( R_SERVTAB_WAIT_NOTE_TEMPLATE );

    iWaitDialog->SetTone( CAknNoteDialog::ENoTone );
    iWaitDialog->SetTextL( aText );
    iWaitDialog->SetCallback( this );

    SetObserver( aObserver );
    iWaitDialog->RunLD();   // CSI: 50 # iWaitDialog is not owned by us
    }

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::ShowWaitNoteL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppWaitNote* CIMCVAppWaitNote::ShowWaitNoteL( const TDesC& aText,
		        			 			  TBool aShowImmediately /*= EFalse*/,
        								  TBool aCanBeCanceledByUser /*= EFalse*/,
        								  MIMCVWaitNoteObserver* aObserver /*= NULL*/ )
    {
    CIMCVAppWaitNote* self = new( ELeave ) CIMCVAppWaitNote();
    CleanupStack::PushL( self );
    self->ConstructL( aText, aShowImmediately, aCanBeCanceledByUser, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::ShowWaitNoteLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppWaitNote* CIMCVAppWaitNote::ShowWaitNoteLC( TInt aTextResource,
		        			 			  TBool aShowImmediately /*= EFalse*/,
        								  TBool aCanBeCanceledByUser /*= EFalse*/,
        								  MIMCVWaitNoteObserver* aObserver /*= NULL*/ )
    {
	CIMCVAppWaitNote* self = new( ELeave ) CIMCVAppWaitNote();
    CleanupStack::PushL( self );

    HBufC* text = CCoeEnv::Static()->AllocReadResourceLC( aTextResource );
    self->ConstructL( *text, aShowImmediately, aCanBeCanceledByUser, aObserver );
    CleanupStack::PopAndDestroy( text );

    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::ShowWaitNoteL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppWaitNote* CIMCVAppWaitNote::ShowWaitNoteL( TInt aTextResource,
		        			 			 TBool aShowImmediately /*= EFalse*/,
        								 TBool aCanBeCanceledByUser /*= EFalse*/,
        								 MIMCVWaitNoteObserver* aObserver /*= NULL*/ )
    {
	CIMCVAppWaitNote* self = ShowWaitNoteLC( aTextResource,
	                                    aShowImmediately,
	                                    aCanBeCanceledByUser,
	                                    aObserver );
    
    CleanupStack::Pop( self );
    return self;
    }




// Destructor
CIMCVAppWaitNote::~CIMCVAppWaitNote()
    {
	DismissDialog();
    }


// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::DialogDismissedL
// Called when dialog is dismissed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppWaitNote::DialogDismissedL( TInt aButtonId )
    {
    
    iWaitDialog = NULL;
    if( iObserver )
        {
        iObserver->NoteCanceled( aButtonId );
        }
    
    }

// -----------------------------------------------------------------------------
// CIMCVAppWaitNote::DismissDialog
// Dismisses the dialog
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppWaitNote::DismissDialog()
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
// CIMCVAppWaitNote::SetObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppWaitNote::SetObserver( MIMCVWaitNoteObserver* aObserver )
    {
    iObserver = aObserver;
    }

//  End of File
