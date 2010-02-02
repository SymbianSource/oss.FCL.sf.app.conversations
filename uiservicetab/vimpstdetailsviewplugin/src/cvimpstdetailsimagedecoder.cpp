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
* Description:  Utility class for asynchronously decoding the header thumbnail image
 *
*/

 
#include "cvimpstdetailsimagedecoder.h"

#include "cvimpstdetailsheadercontrol.h"

#include <cbsbitmap.h>
#include <imageconversion.h> 

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::NewL
// Two-phase construction
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsImageDecoder* CVIMPSTDetailsImageDecoder::NewL(CVIMPSTDetailsHeaderControl& aHeader, const TDesC8& aBitmapData)
    {
    CVIMPSTDetailsImageDecoder* self = new (ELeave) CVIMPSTDetailsImageDecoder(aHeader);
    CleanupStack::PushL(self);
    self->ConstructL(aBitmapData);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::CVIMPSTDetailsImageDecoder
// First phase (C++) constructor
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsImageDecoder::CVIMPSTDetailsImageDecoder(CVIMPSTDetailsHeaderControl& aHeader) : 
	CActive( CActive::EPriorityStandard ), iHeader(aHeader)
    { 
    CActiveScheduler::Add(this); 
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::ConstructL
// ConstructL, second phase constructor
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::ConstructL(const TDesC8& aBitmapData)
    {
    iBitmapData = aBitmapData.AllocL();
    User::LeaveIfError( iFs.Connect() );
    iImgDecoder = CImageDecoder::DataNewL( iFs, *iBitmapData );
    TFrameInfo info = iImgDecoder->FrameInfo();
    iBitmap = new ( ELeave ) CFbsBitmap;
    User::LeaveIfError( iBitmap->Create( info.iOverallSizeInPixels, info.iFrameDisplayMode ));
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::~CVIMPSTDetailsImageDecoder
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsImageDecoder::~CVIMPSTDetailsImageDecoder()
    {
    DoCancel();
    if (iImgDecoder)
        {
        delete iImgDecoder;
        iImgDecoder = NULL;
        }
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if (iBitmapData)
        {
        delete iBitmapData;
        iBitmapData = NULL;
        }
    iFs.Close();
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::Start
// Starts the decoding process
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::Start()
    {
    iStatus = KRequestPending;
    iImgDecoder->Convert( &iStatus, *iBitmap );
    SetActive();
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::RunL
// Called by the active object framework when the decoding (request) is
// completed.
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::RunL() 
	{
	User::LeaveIfError( iStatus.Int() );
	// Ownership of the bitmap is transferred
	iHeader.SetBitmap(iBitmap, NULL );
	iBitmap = NULL;
   }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::DoCancel
// Called when the decoding (request) is cancelled for some reason.
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::DoCancel()
    { 
    iImgDecoder->Cancel(); 
    }

// end of file
