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
* Description:  Icon file provider.
 *
*/


// INCLUDE FILES
#include    "cimcvuiappiconfileprovider.h"
#include	<aknappui.h>
#include    <AknIconUtils.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVUiAppIconFileProvider::CIMCVUiAppIconFileProvider
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVUiAppIconFileProvider::CIMCVUiAppIconFileProvider()
    {
    }
// -----------------------------------------------------------------------------
// CIMCVUiAppIconFileProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVUiAppIconFileProvider::ConstructL(RFile& aFile)
    {
    User::LeaveIfError( iFile.Duplicate( aFile ) );
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppIconFileProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVUiAppIconFileProvider* CIMCVUiAppIconFileProvider::NewL(
        RFile& aFile )
    {
    CIMCVUiAppIconFileProvider* self = new( ELeave ) CIMCVUiAppIconFileProvider();

    CleanupStack::PushL( self );
    self->ConstructL(aFile);
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CIMCVUiAppIconFileProvider::~CIMCVUiAppIconFileProvider()
    {
    iFile.Close();
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppIconFileProvider::RetrieveIconFileHandleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiAppIconFileProvider::RetrieveIconFileHandleL(
            RFile& aFile, const TIconFileType /*aType*/ )
    {
    aFile.Duplicate( iFile );
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppIconFileProvider::Finished
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiAppIconFileProvider::Finished()
    {
    // commit suicide because Avkon Icon Server said so
    delete this;
    }


//  End of File
