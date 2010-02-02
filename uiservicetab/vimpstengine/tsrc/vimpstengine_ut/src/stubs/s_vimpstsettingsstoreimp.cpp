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
#include <e32std.h>
#include "s_vimpstsettingsstoreimp.h"

//dummy serviceId, assuming the service id not more than 256. for the first time
//and it is assumed that this service id is for initializing the cenrep keys only.
//not updated in the service table and hence will not get a TAB in phone book
//const TInt KServiceIDOffset = 16;

// opaque data length, see cvimpstuisapsettings.h SetOpaqueDesC16 method description.
//const TInt KSapSettingsDataBufferLength = 5;
// Collation level 1 to ignore case
//const TInt KCollationLevel = 1;

/**  Key to CIMPSSAPSettings opaque value */
//_LIT( KIMToUAccepted, "KIMToUAccepted" );
/**  User id separator in KIMToUAccepted list */
//_LIT( KIMDot, "," );

//================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTSettingsStoreImp::NewL
// 
// ---------------------------------------------------------
MVIMPSTSettingsStore* CVIMPSTSettingsStoreImp::InitialiseLibraryL( )
    {
    // no existing instance, create a new one
    MVIMPSTSettingsStore *settingsStoreImp = CVIMPSTSettingsStoreImp::NewL();
    return (MVIMPSTSettingsStore*)settingsStoreImp;
    }

void CVIMPSTSettingsStoreImp::Release( )
    {
    
    }
// ---------------------------------------------------------
// CVIMPSTSettingsStoreImp::NewLC
// 
// ---------------------------------------------------------
MVIMPSTSettingsStore* CVIMPSTSettingsStoreImp::NewL( )
    {
    CVIMPSTSettingsStoreImp* self = new (ELeave) CVIMPSTSettingsStoreImp( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStoreImp::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTSettingsStoreImp::ConstructL( )
    {
   
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStoreImp::Destructor
// 
// ---------------------------------------------------------
CVIMPSTSettingsStoreImp::~CVIMPSTSettingsStoreImp()
    {
   
    }

// --------------------------------------------------------------------------------------------------
// CVIMPSTSettingsStoreImp::SetL
// 
// ---------------------------------------------------------------------------------------------------

TInt CVIMPSTSettingsStoreImp::SetL(TUint32 /*aServiceId*/, TSettingItemName /*aSettingItemName*/, const TDesC8& /*aBuffer*/  )
    {
    return KErrNone;  // return the value : error code  
    }

// --------------------------------------------------------------------------------------------------
// CVIMPSTSettingsStoreImp::SetL
// 
// ---------------------------------------------------------------------------------------------------

TInt CVIMPSTSettingsStoreImp::GetL(TUint32 /*aServiceId*/, TSettingItemName /*aSettingItemName*/, const TDesC8& /*aBuffer*/  )
    {
    return KErrNone;  // return the value : error code  
    }

//End of file
