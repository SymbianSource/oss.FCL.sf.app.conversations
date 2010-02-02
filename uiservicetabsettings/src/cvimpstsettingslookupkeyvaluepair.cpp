/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Lookup key container.
*
*/

// INCLUDE FILES
#include    "e32std.h"
#include    "cvimpstsettingslookupkeyvaluepair.h"



// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::NewL()
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsLookupKeyValuePair* CVIMPSTSettingsLookupKeyValuePair::NewL()
    {
    CVIMPSTSettingsLookupKeyValuePair* self = new (ELeave) CVIMPSTSettingsLookupKeyValuePair;
    return self;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::~CVIMPSTSettingsLookupKeyValuePair()
// Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsLookupKeyValuePair::~CVIMPSTSettingsLookupKeyValuePair()
    {
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::CVIMPSTSettingsLookupKeyValuePair()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsLookupKeyValuePair::CVIMPSTSettingsLookupKeyValuePair()
    {
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::Key() const
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTSettingsLookupKeyValuePair::Key() const
    {
    return iLookupKey;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::SetLookupKey()
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsLookupKeyValuePair::SetLookupKey( const TDesC& aLookUpKey )
    {
    iLookupKey.Set( aLookUpKey );
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsLookupKeyValuePair::ResetLookupKey()
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsLookupKeyValuePair::ResetLookupKey()
    {
    iLookupKey.Set( NULL, 0 );
    }

//  End of File

