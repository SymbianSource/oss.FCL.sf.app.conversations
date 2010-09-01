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
* Description:  vimpst settings store definitions and repository transactions.
*
*/

//  INCLUDES
#include <e32base.h>
#include "s_vimpstsettingsstoreimp.h"

/**
 *  vimpst settings API class
 *
 *  Main class of managing vimpst settings
 *
 *  @code example code of how to get/set a value to the setting item in cenrep 
 *  MVIMPSTSettingsStore* obj = CVIMPSTUiSettingsStoreFactory::NewL();
 *  TInt value = 1;
 *  TInt err = obj->Set(serviceId, ETOUDlgOff, value );
 *  similarly for getting the value
 *  obj->Get(serviceId, ETOUDlgOff, value);
 *  @lib vimpstsettingsstore.lib
 *  @since S60 v5.0
 */

// -----------------------------------------------------------------------------
// CVIMPSTSettingsStoreFactory::NewL
// Create an instance of the class CVIMPSTSettingsStoreImp 
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTSettingsStore* CVIMPSTSettingsStoreFactory::NewL()
    {    
    MVIMPSTSettingsStore* self = CVIMPSTSettingsStoreFactory::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::Destructor
// Release the singelton
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsStoreFactory::~CVIMPSTSettingsStoreFactory()
    {
    CVIMPSTSettingsStoreImp::Release();
    }
// -----------------------------------------------------------------------------
// CVIMPSTSettingsStoreFactory::NewLC
// Create an instance of the class CVIMPSTSettingsStoreImp 
// -----------------------------------------------------------------------------
//

EXPORT_C MVIMPSTSettingsStore* CVIMPSTSettingsStoreFactory::NewLC()
    {    
    MVIMPSTSettingsStore* self = CVIMPSTSettingsStoreImp::InitialiseLibraryL();
    CleanupDeletePushL ( self );
    return  self;
    }


// End of file
