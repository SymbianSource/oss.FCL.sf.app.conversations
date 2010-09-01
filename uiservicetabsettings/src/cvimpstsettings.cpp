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
* Description:  settings container.
*
*/

// INCLUDE FILES
#include    <e32std.h>
#include	<centralrepository.h>
#include    <s32mem.h>

#include    "cvimpstsettings.h"
#include    "cvimpstsettingskeyvaluepairs.h"
#include    "cvimpstsettingskeyvaluepair.h"




// ================= MEMBER FUNCTIONS =======================
// -----------------------------------------------------------------------------
// CVIMPSTSettings::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTSettings* CVIMPSTSettings::NewL()
    {
    CVIMPSTSettings* self = CVIMPSTSettings::NewLC();
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettings::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTSettings* CVIMPSTSettings::NewLC()
    {
    CVIMPSTSettings* self = new (ELeave) CVIMPSTSettings();
    CleanupStack::PushL( self );
    self->iKeyValuePairs = CVIMPSTSettingsKeyValuePairs::NewL();   
    return self;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettings::~CVIMPSTSettings()
// Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTSettings::~CVIMPSTSettings()
    {    
    delete iKeyValuePairs;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettings::CVIMPSTSettings()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTSettings::CVIMPSTSettings()
    {
    }
// -----------------------------------------------------------------------------
// CVIMPSTSettings::SetOpaqueDesC16()
// Key-value pairs.
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettings::SetOpaqueDesC16( const TDesC& aKey,
        const TDesC16& aValue )
    {
    TInt err = iKeyValuePairs->SetValueDesC16( aKey, aValue );    
    return err;
    }
// -----------------------------------------------------------------------------
// CVIMPSTSettings::GetOpaqueDesC16()
// Key-value pairs.
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettings::GetOpaqueDesC16( const TDesC& aKey,
                                                 TPtrC16& aValue ) const
    {
    return iKeyValuePairs->GetValueDesC16( aKey, aValue );
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettings::DeleteOpaque()
// Key-value pairs.
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettings::DeleteOpaque( const TDesC& aKey )
    {
    return iKeyValuePairs->DeletePair( aKey );
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettings::KeyValuePairs()
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePairs& CVIMPSTSettings::KeyValuePairs()
    {
    return *iKeyValuePairs;
    }
//  End of File

