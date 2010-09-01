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
* Description:  note mapper class implementation
*
*/


// INCLUDE FILES
#include "imcvuiappnotemapper.h"

#include <StringLoader.h>
#include <imcvuiapp.rsg>
#include <aknnotedialog.h>
#include <AknQueryDialog.h> 
#include <aknnotewrappers.h> 
// global errors
#include<imerrors.hrh>
#include "imcvlogger.h"
// ---------------------------------------------------------
// IMCVUiAppNoteMapper::ShowNoteL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void IMCVUiAppNoteMapper::ShowNoteL( const TInt aErrorCode , const TDesC& aAdditionalData )
	{
	IM_CV_LOGS(TXT("IMCVUiAppNoteMapper::ShowNoteL start") );
	TInt resourceId( 0 );
	TBool aditionalInfo = EFalse;
	
	switch( aErrorCode )
		{	
		case KErrNoMemory:
			{
			resourceId = R_CHAT_CEDITOR_MEMLO;
         	break;
			}
		case KErrDiskFull:
			{
			resourceId = R_CHAT_CEDITOR_MEMFULL;
       		break;
			}
             // flowthrough
        case KImApiErrSenderBlocked :
		case KImErrServiceRequestTypeNotSupported: // Service not supported
			{
			resourceId = R_CHAT_ERROR_SERVICE_ERR;
           	break;
			}
		// flowthrough
		case KImApiErrRecipientNotLogged: // Message recipient not logged in
			{
			aditionalInfo = ETrue;
			resourceId = R_CHAT_ERROR_NOT_REACH;
			break;
			}
			// flowthrough
		case KImErrServiceForceClosedSession: // force logged out
			{
			resourceId = R_CHAT_ERROR_DISCONNECT_ERROR;
			break;
			}		
		
		case KErrNone:
			{
			resourceId = 0;
			break;	
			}
        case KImErrServiceGeneralError:   // flowthrough
		default: // Unknown error -> show general note
			{
		   	resourceId = R_CHAT_ERROR_GEN_ERROR;
			break;
			}
		}

	if( resourceId != 0 )
		{
		HBufC* prompt = NULL;
		
		if( aditionalInfo && aAdditionalData != KNullDesC )
			{
			prompt = StringLoader::LoadLC( resourceId, aAdditionalData );	
			}
		else
			{
			prompt = StringLoader::LoadLC( resourceId );	
			}
		
		CAknErrorNote* errorNote = new (ELeave) CAknErrorNote;
		errorNote->ExecuteLD(*prompt);
		 
		CleanupStack::PopAndDestroy( prompt );   

		}
	IM_CV_LOGS(TXT("IMCVUiAppNoteMapper::ShowNoteL End") );	
	}

// ---------------------------------------------------------
// IMCVUiAppNoteMapper::DisplayQueryDialogL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt IMCVUiAppNoteMapper::DisplayQueryDialogL( TInt aDialogResourceId, 
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
	
//  End of File
