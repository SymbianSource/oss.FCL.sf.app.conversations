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
* Description: s_vimpstengineaimagehandler.cpp
*
*/

// INCLUDE FILES
#include "s_vimpstengineaimagehandler.h"


// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------
EXPORT_C CVIMPSTEngineImageHandler* CVIMPSTEngineImageHandler::NewL()
    {
    CVIMPSTEngineImageHandler* self = 
    CVIMPSTEngineImageHandler::NewLC( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
EXPORT_C CVIMPSTEngineImageHandler* CVIMPSTEngineImageHandler::NewLC( )
    {
    CVIMPSTEngineImageHandler* self = new (ELeave) 
    CVIMPSTEngineImageHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::CVIMPSTEngineImageHandler
// ---------------------------------------------------------	
CVIMPSTEngineImageHandler::CVIMPSTEngineImageHandler()
    {

    }
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::ConstructL
// ---------------------------------------------------------
void CVIMPSTEngineImageHandler::ConstructL()
    {
    }
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler
// ---------------------------------------------------------
CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler()
    {
    }

// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::HandleProcessingCompleteL
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::HandleProcessingCompleteL( TInt /*aStatus*/ )  
    {
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromFileL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromFileL(const TDesC& /*aFilename*/ ,
        const  TDesC8& /*aMimetype*/)
    {
    HBufC8* temp = NULL;
    return temp;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromDataL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromDataL( const TDesC8& /*aImageData */, const TDesC8& /*aMimeType*/)
    {
    HBufC8* temp = NULL;
    return temp;
    }

// end of file
