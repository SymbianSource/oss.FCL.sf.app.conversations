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
* Description:  utils helper class
*
*/


#include "imcvuiapputils.h"
#include  <centralrepository.h>
#include <AknUtils.h>


// ---------------------------------------------------------
// IMCVUiAppUtils::GetCRKeyL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUint32 IMCVUiAppUtils::CRKeyL( TUid aUid, TUint32 aKey )
    {
    TInt val( 0 );

    TRAPD( err,
            CRepository* rep = CRepository::NewL( aUid );
            rep->Get( aKey, val );
            delete rep;
         );

    if( err != KErrNotFound && err != KErrPathNotFound )
        {
        // "Not found" errors are ignored, because it's ok to 
        // return zero if key is not found
        User::LeaveIfError( err );
        }

    return val;    
    }
    	

// ---------------------------------------------------------------------------
// IMCVUiAppUtils::PenEnabled
// ---------------------------------------------------------------------------
//
TBool IMCVUiAppUtils::PenEnabled()
    {
#ifndef RD_30_DISABLE_TOUCH
    return AknLayoutUtils::PenEnabled();

#else
    // In 3.0 mode we don't have pen support
    return EFalse;

#endif //RD_30_DISABLE_TOUCH  
    }
    	
// End of file

