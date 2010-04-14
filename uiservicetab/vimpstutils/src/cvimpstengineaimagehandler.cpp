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
* Description: Image handler for VImpst engine handler
*
*/

// INCLUDE FILES
#include "cvimpstengineimagehandler.h"

#include "uiservicetabtracer.h"
#include "tvimpstenums.h"


#include "vimpstallerrors.h"
#include "tvimpstconsts.h"

#include "cvimpstimageprocessor.h" // image processor

const TInt KOFFSETSIZE = 10000;
const TInt KSIZEMULTIPLE = 3;
const TInt KQUALITYFACTOR = 55;
// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------
EXPORT_C CVIMPSTEngineImageHandler* CVIMPSTEngineImageHandler::NewL()
    {
	TRACER_AUTO;
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
	TRACER_AUTO;
    CVIMPSTEngineImageHandler* self = new (ELeave) 
    CVIMPSTEngineImageHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::CVIMPSTEngineImageHandler
// ---------------------------------------------------------	
CVIMPSTEngineImageHandler::CVIMPSTEngineImageHandler():iError(0)
    {

    }
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::ConstructL
// ---------------------------------------------------------
void CVIMPSTEngineImageHandler::ConstructL()
    {
	TRACER_AUTO;
    iProcessor = CVimpstImageProcessor::NewL(this);
    iFormat.iSettings.iQualityFactor = KQUALITYFACTOR;
    iFormat.iSettings.iSampleScheme = TMdaJpgSettings::TColorSampling(TMdaJpgSettings::EColor420);
    
    }
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler
// ---------------------------------------------------------
CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler()
    {
	TRACER_AUTO;
    delete iProcessor;
    if(iToDesc)
        {
        delete iToDesc;
        }
   
    }

// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::HandleProcessingCompleteL
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::HandleProcessingCompleteL( TInt aStatus )  
    {
	TRACER_AUTO;
    if (!aStatus)
        {
        iContent = iProcessor->GetContentData();
        }

    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromFileL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromFileL(const TDesC& aFilename ,
        const  TDesC8& aMimetype)
    {
	TRACER_AUTO;
    if(aFilename.Length())
        {
        TRACE(" called");
        iProcessor->ProcessFromFileL( aFilename , aMimetype);
        if( ! iWait.IsStarted() )
            {
            // codescanner warning can be ignored, we are not starting an active object but 
            // CActiveSchedulerWait.
            iWait.Start();
            }
        }
    
    return iContent;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromDataL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromDataL( const TDesC8& aImageData , const TDesC8& aMimeType)
    {
	TRACER_AUTO;
    if(aImageData.Length())
        {
        TRACE("ProcessImageFromDataL called");
        TInt err =  iProcessor->ProcessFromDataL( aImageData , aMimeType);
        if(KErrNone != err)
           {
           iContent = KNullDesC8().AllocL();
           return iContent;
           }
        if( ! iWait.IsStarted() )
            {
            // codescanner warning can be ignored, we are not starting an active object but 
            // CActiveSchedulerWait.
            iWait.Start();
            }
        }
    
    return iContent;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessFromBitmapL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessFromBitmapL( CFbsBitmap& aBitMap )
    {

	TRACER_AUTO;

    //bitmap to desc
    const TSize size = aBitMap.SizeInPixels();
    const TInt descSize = (size.iWidth * size.iHeight * KSIZEMULTIPLE) + KOFFSETSIZE;
    iBuf = HBufC8::NewL(descSize);
    TPtr8 imageBuffer(iBuf->Des());
    if(iToDesc)
        {
        delete iToDesc;//delete the old one
        iToDesc = NULL;
        }
    iToDesc = CMdaImageBitmapToDescUtility::NewL(*this);
    iToDesc->CreateL(imageBuffer, &iFormat, 0, 0);
    if( ! iWait.IsStarted() )
        {
        // codescanner warning can be ignored, we are not starting an active object but 
        // CActiveSchedulerWait.
        iWait.Start();
        }
    User::LeaveIfError(iError);//CreateL is a asynch call.leave if error in creating     
    iToDesc->ConvertL(aBitMap);
    if( ! iWait.IsStarted() )
        {
        // codescanner warning can be ignored, we are not starting an active object but 
        // CActiveSchedulerWait.
        iWait.Start();
        }
    User::LeaveIfError(iError);//CreateL is a asynch call.leave if error in creating 

    
    return iBuf;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoCreateComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoCreateComplete(TInt aError)
    {
	TRACER_AUTO;
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    

    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoOpenComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoOpenComplete(TInt aError)
    {
	TRACER_AUTO;
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    

    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoConvertComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoConvertComplete(TInt aError)
    {
	TRACER_AUTO;
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    

    }
// CVIMPSTEngineImageHandler::Bitmap
// 
// ------------------------------------------------------------------------    
EXPORT_C CFbsBitmap& CVIMPSTEngineImageHandler::Bitmap()
    {
	TRACER_AUTO;
    return iProcessor->Bitmap();
    }    

// end of file
