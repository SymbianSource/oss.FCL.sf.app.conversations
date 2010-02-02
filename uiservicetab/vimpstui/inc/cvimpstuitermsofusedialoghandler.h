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
* Description: helper class for handling terms of use dialog.
*
*/


#ifndef _CVIMPSTUITERMSOFUSEDIALOGHANDLER_H
#define _CVIMPSTUITERMSOFUSEDIALOGHANDLER_H

#include <e32std.h>

class MVIMPSTSettingsStore;


/**
 *  Static helper class for handling terms of use dialog.
 *
 *  @lib vimpstui.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CVIMPSTUiTermsOfUseDialogHandler )
    {

public:

    /**
     * Method used to handle terms of use (ToU) dialog.
     * Checks if current user id in aSap has
     * already accepted ToU or not. If not shows
     * ToU. Shows also "Login cancelled" note if user does not
     * accept ToU.
     * ownership is transfered.
     *    
     * @param aSapStore Current SAP settings.     
     * @param aUserName current user name to login 
     * @return KErrCancel if user cancels ToU dialog, KErrNone if
     *         user accepts or has previously accepted ToU.
     */
    TInt static HandleTermsOfUseDialogLD( MVIMPSTSettingsStore& aSettingsStore,
                                         const TDesC& aUserName,
                                         TInt& aServiceId,
                                         HBufC* aToUMsg = NULL);
                                         
    TBool static DisplayTermsOfUseAndQueryLD(HBufC* aToUMsg = NULL);                                         

protected:

    // Prohibited constructor / destructor
    CVIMPSTUiTermsOfUseDialogHandler();
    ~CVIMPSTUiTermsOfUseDialogHandler();
    
    };

#endif // _CVIMPSTUITERMSOFUSEDIALOGHANDLER_H
