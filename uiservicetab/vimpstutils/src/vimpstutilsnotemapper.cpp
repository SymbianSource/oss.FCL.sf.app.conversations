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
#include "vimpstutilsnotemapper.h"
#include "vimpstutilsdialog.h"
#include "vimpstallerrors.h"
#include <vimpstuires.rsg>
#include <StringLoader.h>
#include <eikenv.h>
#include "uiservicetabtracer.h"



// ---------------------------------------------------------
// VIMPSTUtilsNoteMapper::ShowNoteL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void VIMPSTUtilsNoteMapper::ShowNoteL( const TInt aErrorCode, const TDesC& aAdditionalData )
	{
	TRACER_AUTO;
	TBool ignoreAdditionalData( EFalse ); // Some logical strings are without parameters...
	TBool errorNote( ETrue );
	TBool defaultNote( EFalse );
	TInt resourceID( 0 );
	TBool confirmationNote( EFalse );
	TBool softkeyOk( EFalse );

  	switch( aErrorCode )
		{	
		case EVIMPSTWrongUserIdPassword:// Invalid password or userid
			{
			defaultNote = ETrue;
			errorNote = ETrue;
			resourceID = R_IM_LOGIN_PASSER_NOTE;
            ignoreAdditionalData = ETrue;
			break;
			}
		case EVIMPSTSessionExpired:
		case EVIMPSTNotSupported:
			{
			defaultNote = ETrue;
			errorNote = ETrue;
            ignoreAdditionalData = ETrue;
			resourceID = R_IM_ERROR_SERVICE_ERR;
			break;
			}
        case EVIMPSTMultipleErrors:   // flowthrough
	   	default: // Unknown error -> show general note
			{
			defaultNote = ETrue;
			errorNote = ETrue;
            ignoreAdditionalData = ETrue;
			resourceID = R_IM_ERROR_GEN_ERROR;
			break;
			}
		}
 
	if( resourceID != 0 )
		{
		if( confirmationNote )
		    {
		    VIMPSTUtilsDialog::DisplayQueryDialogL( resourceID );
		    }
		else
		    {
    		HBufC* prompt = NULL;

    		if( aAdditionalData == KNullDesC || defaultNote || ignoreAdditionalData )
    			{
    			prompt = CEikonEnv::Static()->AllocReadResourceLC( resourceID );
    			}
    		else
    			{
    			prompt = StringLoader::LoadLC( resourceID, aAdditionalData );
    			}

    		if( errorNote )
    			{
    			VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt, softkeyOk );
    			}
    		else
    			{
    			VIMPSTUtilsDialog::DisplayNoteDialogL( *prompt );
    			}

    		CleanupStack::PopAndDestroy( prompt );
		    }
		}
	}

//  End of File
