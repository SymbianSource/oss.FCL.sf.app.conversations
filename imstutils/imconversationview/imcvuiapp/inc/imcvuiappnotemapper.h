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
* Description:  note mapper class
*
*/


#ifndef IMCVUIAPPNOTEMAPPER_H
#define IMCVUIAPPNOTEMAPPER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  Class is used for mapping engine errors to correct error notes
*
*  imcvuiapp.exe
*  @since 5.0
*/
class IMCVUiAppNoteMapper
    {
    public: // New functions
		/**
		* Shows specified error note for aErrorCode (UI specification)
		* @param aErrorCode Error code
		* @param aAdditionalData, any aditional info to show
		* @return Did class have information about given error or not
		*/
		static void ShowNoteL( const TInt aErrorCode, const TDesC& aAdditionalData );
	
	
		/**
         * Displays query dialog with given data
         * @param aDialogResourceId Dialogs resource id
         * @param aPrompt Prompt text
         * @return Dialog dismiss key
         */
        static TInt DisplayQueryDialogL( TInt aDialogResourceId, 
                const TDesC& aPrompt = KNullDesC );
	

    };

#endif      // IMCVUIAPPNOTEMAPPER_H

// End of File
