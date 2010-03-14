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
#include <bitmaptransforms.h>

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
    if (iBitmapScaler)
        {
        delete iBitmapScaler;
        iBitmapScaler = NULL;
        }

    iFs.Close();
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::Start
// Starts the decoding process
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::StartL(TSize aBitmapSize)
    {
    iDecoderState = ECcaConvertThumbnailImage;
    iBitmapSize = aBitmapSize;
    CreateBitmapL();
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
	switch ( iDecoderState )
	    {
	    case ECcaConvertThumbnailImage:
	        {
	        iDecoderState = ECcaScaleThumbnail;
	        ScaleBitmapL();
	        break;
	        }
	    case ECcaScaleThumbnail:
	        {
	        // don't delete
	        // Ownership of the bitmap is transferred   
	        iHeader.SetBitmap(iBitmap, NULL );
	        iBitmap = NULL;
	        }
	    default:
	        break;
	    }	
	
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

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::ScaleBitmapL
// scaling it to fit to the screen
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsImageDecoder::ScaleBitmapL()
    {   
    iBitmapScaler = CBitmapScaler::NewL();
    iBitmapScaler->Scale( &iStatus, *iBitmap, iBitmapSize );
    SetActive();
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsImageDecoder::CreateBitmapL
// creates the bitmap
// ---------------------------------------------------------------------------
//

void CVIMPSTDetailsImageDecoder::CreateBitmapL()
    {
    if ( iDecoderState == ECcaConvertThumbnailImage )
        {
        iImgDecoder = CImageDecoder::DataNewL( iFs, *iBitmapData, CImageDecoder::EOptionAlwaysThread );
        }   
   if ( !iBitmap )
       {
       TFrameInfo info = iImgDecoder->FrameInfo();
       iBitmap = new ( ELeave ) CFbsBitmap;
       User::LeaveIfError( iBitmap->Create( info.iOverallSizeInPixels, info.iFrameDisplayMode ));
       } 
    iStatus = KRequestPending;
    iImgDecoder->Convert( &iStatus, *iBitmap );
    SetActive();
    }
// end of file
