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
* Description:  Dialogue handler for TOU
*
*/


#include "cvimpstuitermsofusedialoghandler.h"
#include "cvimpstuitermsofusedlg.h"

#include "cvimpstsettingsstore.h"
#include <vimpstuires.rsg>
#include <avkon.hrh>
#include "uiservicetabtracer.h"




    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTUiTermsOfUseDialogHandler::HandleTermsOfUseDialogLD
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiTermsOfUseDialogHandler::HandleTermsOfUseDialogLD(    
        MVIMPSTSettingsStore& aSettingsStore,
        const TDesC& /*aUserName*/,
        TInt& aServiceId,
        HBufC* aToUMsg/* = NULL*/)
    {   
	TRACER_AUTO;
    // Show Terms of Use, return KErrCancel if user does not accept them,
    // otherwise continue
    if ( !DisplayTermsOfUseAndQueryLD(aToUMsg) )
        {
        // User canceled ToU query, login cancelled
        // note is already shown inside DisplayTermsOfUseAndQueryL
        // method, just return KErrCancel
        //CleanupStack::PopAndDestroy(tmpSap);
        return KErrCancel;
        }
    else
        {
        TInt touOff =1;
        aSettingsStore.SetL(aServiceId, ETOUDlgOff, touOff ); 
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTermsOfUseDialogHandler::DisplayTermsOfUseAndQueryLD()
// -----------------------------------------------------------------------------
//
TBool CVIMPSTUiTermsOfUseDialogHandler::DisplayTermsOfUseAndQueryLD(HBufC* aToUMsg/* = NULL*/)
    {
	TRACER_AUTO;
    TInt retVal = EAknSoftkeyCancel;    
    CVIMPSTUiTermsOfUseDlg* dlg = CVIMPSTUiTermsOfUseDlg::NewLC(  );
    dlg->SetToUMsgL(aToUMsg);
    retVal = dlg->RunDialogLD( R_CNUI_TERMS_OF_USE_DIALOG );
    CleanupStack::Pop(dlg);
    if ( retVal == EAknSoftkeyOk ||  retVal == EAknSoftkeyYes)
        {                
        return ETrue;
        }
    else 
        {    
        return EFalse;
        }
    }

//End of File
