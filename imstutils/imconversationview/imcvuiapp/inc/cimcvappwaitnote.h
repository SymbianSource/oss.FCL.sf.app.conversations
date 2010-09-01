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



#ifndef CIMCVAPPWAITNOTE_H
#define CIMCVAPPWAITNOTE_H

//  INCLUDES
#include <AknWaitDialog.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION


// Observer class to notice dismissing note by cancel.
class MIMCVWaitNoteObserver
    {
    public: // Interface methods
    
        /**
         * Wait note is canceled.
         */
        virtual void NoteCanceled( TInt aButtonId ) = 0;    
    };

/**
 *  Helper class for wait notes
 *
 *  @lib imcvuiapp.exe
 *  @since 
 */
class CIMCVAppWaitNote : public CBase, public MProgressDialogCallback
    {
    public:  // Constructors and destructor

        /**
         * Shows waitnote
         *
         * @param aText 				Text to be shown in note.
         * @param aShowImmediately 		If ETrue waitnote is shown immediately
         *								(not after 1 sec delay)
         * @param aCanBeCanceledByUser  Do we show wait note with cancel soft key or not
         */
        static CIMCVAppWaitNote* ShowWaitNoteL( const TDesC& aText,
        									TBool aShowImmediately = EFalse,
        									TBool aCanBeCanceledByUser = EFalse, 
        									MIMCVWaitNoteObserver* aObserver = NULL );

		/**
         * Shows waitnote and pushes the item to cleanupstack
         *
         * @param aTextResource 		Resource id of text to be shown in note.
         * @param aShowImmediately 		If ETrue waitnote is shown immediately
		 *								(not after 1 sec delay)
         * @param aCanBeCanceledByUser  Do we show wait note with cancel soft key or not
         */
        static CIMCVAppWaitNote* ShowWaitNoteLC( TInt aTextResource,
        									TBool aShowImmediately = EFalse,
        								    TBool aCanBeCanceledByUser = EFalse, 
        								    MIMCVWaitNoteObserver* aObserver = NULL );
		
		/**
         * Shows waitnote, does not leave anything to cleanupstack.
         *
         * @param aTextResource 		Resource id of text to be shown in note.
         * @param aShowImmediately 		If ETrue waitnote is shown immediately
		 *								(not after 1 sec delay)
         * @param aCanBeCanceledByUser  Do we show wait note with cancel soft key or not
         */
        static CIMCVAppWaitNote* ShowWaitNoteL( TInt aTextResource,
        								   TBool aShowImmediately = EFalse,
        								   TBool aCanBeCanceledByUser = EFalse, 
        								   MIMCVWaitNoteObserver* aObserver = NULL );
		/**
		 * Destructor.
		 */
		virtual ~CIMCVAppWaitNote();

    protected: // From MProgressDialogCallback

        /**
         * From MProgressDialogCallback Get's called when a dialog is dismissed
         * @param aButtonId Button that was used to dismiss the dialog
         */
        void DialogDismissedL( TInt aButtonId );


	public: // New functions

		/**
         * Dismisses this waitnote
		 */
		void DismissDialog();


    private:

        /**
         * C++ default constructor.
         */
        CIMCVAppWaitNote();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL( const TDesC& aText,
        		  		 TBool aShowImmediately,
        				 TBool aCanBeCanceledByUser,
        				 MIMCVWaitNoteObserver* aObserver );

    public: // New methods
   
        /**
         * Set wait note observer. NULL will unset this one.
         */
        void SetObserver( MIMCVWaitNoteObserver* aObserver );

    private: // Data

    	// Doesn't own (destroys itself)
		CAknWaitDialog* iWaitDialog;
       
        /// Wait note observer. This will be notified when wait note is dismissed.
        MIMCVWaitNoteObserver* iObserver;
    };

#endif      // CIMCVAPPWAITNOTE_H

// End of File
