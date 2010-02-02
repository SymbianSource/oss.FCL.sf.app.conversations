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



#ifndef VIMPSTUTILSDIALOG_H
#define VIMPSTUTILSDIALOG_H

//  INCLUDES
#include <avkon.rsg> // for R_AVKON_SOFTKEYS_EMPTY
#include <aknnotedialog.h>
#include <eiklbm.h>

// CLASS DECLARATION
/**
 *  This class provides some useful methods to launch different dialogs.
 *
 *  @lib vimpstutils.lib
 *  @since 5.0
 */
class VIMPSTUtilsDialog
    {
    
    public: // Enumerations
    
        enum TLeftSoftkey
            {
            EDefault = 0,
            ESend,
            EOk
            };

        enum TInitialSelectionMode
            {
            ESelectAll = 0, // Default mode
            ESelectNone
            };
    
    public: // New functions

        /**
         * This method displays note dialogs e.g. confirmation dialog
         * Note dialog's type is defined in resource file
         * @param aText Prompt text of dialog
         * @param aButtonsResourceId tells buttons that are shown with note, default none
         * @param aTimeout timeout when note showing stops
         * @return KeyCode (e.g. EAknSoftKeyOk)
         */
         IMPORT_C static TInt DisplayNoteDialogL( const TDesC& aText, 
                const TInt aButtonsResourceId = R_AVKON_SOFTKEYS_EMPTY,
                const CAknNoteDialog::TTimeout& aTimeout = CAknNoteDialog::EShortTimeout );

        /**
         * This method displays error notes for corresponding resource Id
         * @param aText Prompt text of dialog
         * @param aSoftkeyOk Should there be OK softkey
         * @return KeyCode
         */
        IMPORT_C static TInt DisplayErrorNoteL( const TDesC& aText, 
        										TBool aSoftkeyOk = EFalse );
        

        /**
         * Displays query dialog with given data
         * @param aDialogResourceId Dialogs resource id
         * @param aPrompt Prompt text
         * @return Dialog dismiss key
         */
        IMPORT_C static TInt DisplayQueryDialogL( TInt aDialogResourceId, 
                const TDesC& aPrompt = KNullDesC );

        /**
		 * This method displays Text Query Dialog
		 * @param aDataText Descriptor where written data is stored
		 * @param aTitleResourceId Resource ID of title text
		 * @param aDialogResource Resource of dialog
         * @param aPredictiveInput Enable T9 predictive input (when ETrue)
         * @param aLeftSoftkeyAlwaysVisible Is LSK always visible, or visible
         *                                  only when there is text in the editor.
         * @param aLeftSoftkey Left soft key of dialog.
         * @param aSelectionMode Initial selection mode in dialog editor.
         * @param aDomainSelectionQuery Is domain selection variated on or not,
         *                              ETrue causes dialog to exit when editor field
         *                              is cleared by user.
		 * @return KeyCode (e.g. EAknSoftKeyOk)
		 */
		IMPORT_C static TInt DisplayTextQueryDialogL( TDes& aDataText, 
											 const TInt aTitleResourceId, 
											 const TInt aDialogResourceId,
                                             TBool aPredictiveInput = EFalse,
                                             TBool aLeftSoftkeyAlwaysVisible = EFalse,
                                             TLeftSoftkey aLeftSoftkey = EDefault,
                                             TInitialSelectionMode aSelectionMode = ESelectAll,
                                             TBool aDomainSelectionQuery = EFalse );
        /**
         * This method displays singleselection List Query Dialog
         * NOTE that it is the responsibility of the caller to use
         * cleanupstack for aItemList (ownership is NOT transferred to the
         * list box).
         * @param aSelectedIndex Index of selected item
         * @param aItemList List of Items (Must be formatted)
         * @param aDialogResourceId Resource ID of dialog
         * @param aAlternativeTitle Alternative title text
         * @param aListEmptyText Text to be shown if the list is empty
         * @param aIsForwardQuery The dialog is for forwarding the message (ETrue)
         * @return KeyCode (e.g. EAknSoftKeyOk)
         */
        IMPORT_C static TInt DisplayListQueryDialogL( TInt* aSelectedIndex,
                MDesCArray* aItemList,
                const TInt aDialogResourceId,
                const TDesC& aAlternativeTitle = KNullDesC,
                const TDesC& aListEmptyText = KNullDesC,
                const TBool aIsForwardQuery = EFalse,
                const TBool aIsOnlySelect = EFalse  );
                                             
    };

#endif      // VIMPSTUTILSDIALOG_H   

// End of File
