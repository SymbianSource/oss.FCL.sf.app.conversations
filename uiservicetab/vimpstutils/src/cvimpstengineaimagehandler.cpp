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

#include "vimpstdebugprint.h"
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
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::NewL");
    CVIMPSTEngineImageHandler* self = 
    CVIMPSTEngineImageHandler::NewLC( );
    CleanupStack::Pop( self );
    CHAT_DP_FUNC_DONE("CVIMPSTEngineImageHandler::NewL");
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
EXPORT_C CVIMPSTEngineImageHandler* CVIMPSTEngineImageHandler::NewLC( )
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::NewLC");
    CVIMPSTEngineImageHandler* self = new (ELeave) 
    CVIMPSTEngineImageHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CHAT_DP_FUNC_DONE("CVIMPSTEngineImageHandler::NewLC");
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
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ConstructL start");
    iProcessor = CVimpstImageProcessor::NewL(this);
    iFormat.iSettings.iQualityFactor = KQUALITYFACTOR;
    iFormat.iSettings.iSampleScheme = TMdaJpgSettings::TColorSampling(TMdaJpgSettings::EColor420);
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ConstructL end");
    }
// ---------------------------------------------------------
// CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler
// ---------------------------------------------------------
CVIMPSTEngineImageHandler::~CVIMPSTEngineImageHandler()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::~CVIMPSTEngineAvatarExtention");
    delete iProcessor;
    if(iToDesc)
        {
        delete iToDesc;
        }
    CHAT_DP_FUNC_DONE("CVIMPSTEngineImageHandler::~CVIMPSTEngineAvatarExtention");
    }

// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::HandleProcessingCompleteL
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::HandleProcessingCompleteL( TInt aStatus )  
    {
	CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::HandleProcessingCompleteL start");
    if (!aStatus)
        {
        iContent = iProcessor->GetContentData();
        }

    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::HandleProcessingCompleteL end");
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromFileL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromFileL(const TDesC& aFilename ,
        const  TDesC8& aMimetype)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromFileL start");
    if(aFilename.Length())
        {
        CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessFromFileL called");
        iProcessor->ProcessFromFileL( aFilename , aMimetype);
        if( ! iWait.IsStarted() )
            {
            // codescanner warning can be ignored, we are not starting an active object but 
            // CActiveSchedulerWait.
            iWait.Start();
            }
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromFileL end");
    return iContent;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessImageFromDataL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessImageFromDataL( const TDesC8& aImageData , const TDesC8& aMimeType)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromDataL start");
    if(aImageData.Length())
        {
        CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromDataL called");
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
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromDataL end");
    return iContent;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::ProcessFromBitmapL
// 
// ------------------------------------------------------------------------
EXPORT_C HBufC8* CVIMPSTEngineImageHandler::ProcessFromBitmapL( CFbsBitmap& aBitMap )
    {

    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessFromBitmapL start");

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

    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::ProcessImageFromDataL end");
    return iBuf;
    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoCreateComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoCreateComplete(TInt aError)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoCreateComplete start");
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoCreateComplete end");

    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoOpenComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoOpenComplete(TInt aError)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoOpenComplete start");
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoOpenComplete end");

    }
// ------------------------------------------------------------------------
// CVIMPSTEngineImageHandler::MiuoConvertComplete
// 
// ------------------------------------------------------------------------
void CVIMPSTEngineImageHandler::MiuoConvertComplete(TInt aError)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoConvertComplete start");
    iError = aError;
    if(iWait.IsStarted() )   
        {
        iWait.AsyncStop();  
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineImageHandler::MiuoConvertComplete end");

    }
// CVIMPSTEngineImageHandler::Bitmap
// 
// ------------------------------------------------------------------------    
EXPORT_C CFbsBitmap& CVIMPSTEngineImageHandler::Bitmap()
    {
    return iProcessor->Bitmap();
    }    

// end of file
