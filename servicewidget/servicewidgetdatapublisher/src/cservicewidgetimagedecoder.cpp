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
* Description:  presene avatar decoder handler 
 *                
 *
*/

 
#include "cservicewidgetimagedecoder.h"
#include "servicewidgetcpglobals.h"
#include <cbsbitmap.h>
#include <ImageConversion.h> 

const TInt KMimeLength = 16;
// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::NewL
// Two-phase construction
// ---------------------------------------------------------------------------
//
CServiceWidgetImageDecoder* CServiceWidgetImageDecoder::NewL(MServiceWidgetImagerDecodeObserver& aObserver )
    {
    CServiceWidgetImageDecoder* self = new (ELeave) CServiceWidgetImageDecoder(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::CServiceWidgetImageDecoder
// First phase (C++) constructor
// ---------------------------------------------------------------------------
//
CServiceWidgetImageDecoder::CServiceWidgetImageDecoder(MServiceWidgetImagerDecodeObserver& aObserver ) 
	: 
	CActive( CActive::EPriorityStandard ), 
	iObserver(aObserver)
    { 
    CActiveScheduler::Add(this); 
    }

// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::ConstructL
// ConstructL, second phase constructor
// ---------------------------------------------------------------------------
//
void CServiceWidgetImageDecoder::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::~CServiceWidgetImageDecoder
// ---------------------------------------------------------------------------
//
CServiceWidgetImageDecoder::~CServiceWidgetImageDecoder()
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
// CServiceWidgetImageDecoder::StartDecodingL
// Starts the decoding process
// ---------------------------------------------------------------------------
//
void CServiceWidgetImageDecoder::StartDecodingL(const TDesC8& aBitmapData )
    {
    iBitmapData = aBitmapData.AllocL();
    
    iMimeType = HBufC8::NewL( KMimeLength);
    TPtr8 mimePtr = iMimeType->Des();
    CImageDecoder::GetMimeTypeDataL( *iBitmapData , mimePtr );
     
    iImgDecoder = CImageDecoder::DataNewL( iFs, *iBitmapData );
    TFrameInfo info = iImgDecoder->FrameInfo();
    iBitmap = new ( ELeave ) CFbsBitmap;
    User::LeaveIfError( iBitmap->Create( info.iOverallSizeInPixels, info.iFrameDisplayMode ));
    
    iNextOperationId = EOpEncodeBitmap;
    iStatus = KRequestPending;
    iImgDecoder->Convert( &iStatus, *iBitmap );
    SetActive();
    }

// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::RunL
// Called by the active object framework when the decoding (request) is
// completed.
// ---------------------------------------------------------------------------
//
void CServiceWidgetImageDecoder::RunL() 
	{
	User::LeaveIfError( iStatus.Int() );
	
	switch( iNextOperationId )
		{
		case EOpEncodeBitmap: 
			{
			StartEncodingL();
			break;	
			}
		case EOpComplete: 
			{
			iObserver.HandlerDecodeCompleteL();
			break;	
			}
		}
   }

// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::DoCancel
// Called when the decoding (request) is cancelled for some reason.
// ---------------------------------------------------------------------------
//
void CServiceWidgetImageDecoder::DoCancel()
    { 
    iImgDecoder->Cancel(); 
    }
// ---------------------------------------------------------------------------
// CServiceWidgetImageDecoder::StartEncodingL
// Called when the decoding (request) is cancelled for some reason.
// ---------------------------------------------------------------------------
//
void CServiceWidgetImageDecoder::StartEncodingL()
    { 
 	iImageEncoder = CImageEncoder::FileNewL(iFs, 
 											KSeTAvatarPath, 
 											*iMimeType, 
 											CImageEncoder::EOptionExtUseWithSetThumbnail );
    iImageEncoder->SetThumbnail(ETrue);
    
    iNextOperationId = EOpComplete;
    iStatus = KRequestPending;
    iImageEncoder->Convert( &iStatus, *iBitmap );
    SetActive();
    }

// end of file
