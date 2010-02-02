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
* Description:  Image processor  and scaler
 *
*/


#include "cvimpstimageprocessor.h"
#include "tvimpstconsts.h"
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include <s32file.h>
#include <e32cmn.h>
#include "vimpstdebugprint.h"
// CONSTANTS
const TInt KChatCustomBitmapWidth =  65;
const TInt KChatCustomBitmapHeight = 65;
const TInt KMaxImageSize = 1800; //2k
const TInt KMimeLength = 16;
const TInt KScaleSize = 10;

//-----------------------------------------------------------------------------
// CCAImageLoader::CCAImageLoader
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CVimpstImageProcessor::CVimpstImageProcessor( MCAImageProcessObserver* aObserver )
: CActive( CActive::EPriorityLow ),iObserver(aObserver),iScaleSize(KScaleSize)
	    {
	    CActiveScheduler::Add( this );
	    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::CVimpstImageProcessor
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CVimpstImageProcessor::~CVimpstImageProcessor()
    {
    iFs.Close();

    delete iImageDecoder;
    delete iScaler;
    delete iImageEncoder;
    if(!isBitmapReturned)
        {
        delete iBitmap;
        }
    delete iMimeType;
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CVimpstImageProcessor* CVimpstImageProcessor::NewL( MCAImageProcessObserver* aObserver)
    {
    CVimpstImageProcessor* self =
    new ( ELeave ) CVimpstImageProcessor(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    }


//-----------------------------------------------------------------------------
// CVimpstImageProcessor::RunL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::RunL()
    {
    TInt status = iStatus.Int();    

    if(!status)
        {
        switch(iProcessingState)
            {
            case EConvertImageDatatoBitmap:
                {
                ConvertImageToBitmapL();
                break;
                }
            case EScaleBitmap:
                {
                ScaleBitmapL();
                break;
                }
            case EConentFromScaledBitmap:
                {
                ContentFromScaledBitmapL();
                break;
                }
            case EProcessingComplete: 
                {
                if( iContent->Length() > KMaxImageSize)
                    {
                    delete iScaler;
                    iScaler = NULL;
                    delete iImageEncoder;
                    iImageEncoder = NULL;
                    delete iContent;
                    iTargetSize.SetSize( KChatCustomBitmapWidth - iScaleSize, KChatCustomBitmapHeight - iScaleSize );
                    iScaleSize += KScaleSize;
                    ScaleBitmapL();
                    }
                else
                    {
                    CHAT_DP_FUNC_ENTER("CVimpstImageProcessor::RunL completed ");
                    CompleteRequestL();
                    }
                break;
                }  
            default:
                {
                
                }
            }

        }
    else
        {
        CHAT_DP_FUNC_ENTER("CVimpstImageProcessor::RunL completed ");
        CompleteRequestL();
        }

    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ConvertImageToBitmapL()
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ConvertImageToBitmapL()
    {
    TSize origSize( iImageDecoder->FrameInfo().iOverallSizeInPixels );
    ScaleSize( origSize, iTargetSize );
    TBool fullyScaleable( iImageDecoder->FrameInfo().iFlags & TFrameInfo::EFullyScaleable );
    TSize decodeSize( DecodeSize( origSize, iTargetSize, fullyScaleable ) );

    iBitmap = new (ELeave) CFbsBitmap();
    User::LeaveIfError( iBitmap->Create( decodeSize, 
            iImageDecoder->FrameInfo().iFrameDisplayMode ) );
    iImageDecoder->Convert( &iStatus, *iBitmap );
    iProcessingState = EScaleBitmap;  //  set state to next step
    if( !IsActive() )
        {
        SetActive();
        }
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ScaleBitmapL()
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ScaleBitmapL()
    {
    TSize currentSize( iBitmap ? iBitmap->SizeInPixels() : iTargetSize );

    if( currentSize != iTargetSize )
        {
        // bitmap exists and the size is wrong,
        // we have to scale the bitmap
        iScaler = CBitmapScaler::NewL();
        iScaler->Scale( &iStatus, *iBitmap, iTargetSize );
        iProcessingState = EConentFromScaledBitmap; //  set state to next step
        if( !IsActive() )
            {
            SetActive();
            }
        }
    else 
        { // get content from scaled bitmap
        ContentFromScaledBitmapL();
        }

    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ContentFromScaledBitmapL()
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ContentFromScaledBitmapL()
    {
    iContent = NULL;
    iImageEncoder = CImageEncoder::DataNewL( iContent,*iMimeType);
    iImageEncoder->Convert( &iStatus, *iBitmap );
    iProcessingState = EProcessingComplete ; //  set state to next step
    if( !IsActive() )
        {
        SetActive();
        }
    }


//-----------------------------------------------------------------------------
// CVimpstImageProcessor::DoCancel
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::DoCancel()
    {
    ClearState();        
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::RunError
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CVimpstImageProcessor::RunError( TInt aError )
    {
    if( IsActive() )
        {
        iStatus = aError;
        Cancel();
        }

    if( aError == KErrNoMemory )
        {
        CActiveScheduler::Current()->Error( KErrNoMemory );
        }

    // Clean up and reset all pending messages
    ClearState();
    TRAP_IGNORE( ResetProcessingL( aError ) );

    return KErrNone;    
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::StartProcessingFromDataL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CVimpstImageProcessor::ProcessFromDataL( const TDesC8& aImageData , const TDesC8& aMimeType)
    {
    TInt err = KErrNone;
    if ( aMimeType.Length())
        {
        // if the mimetype is passed then allocate the memory
        iMimeType = aMimeType.AllocL();
        }
    else 
        {
        // if no mimetype find the mimetype
        iMimeType = HBufC8::NewL( KMimeLength);
        TPtr8 mimePtr = iMimeType->Des();
        TRAP(err,CImageDecoder::GetMimeTypeDataL( aImageData , mimePtr ));
        if(KErrNone != err)
            {
            return err;
            }
        }  
    iImageDecoder = NULL;
    // need to check for the mimetype length, as sometimes CImageDecoder::GetMimeTypeDataL
    // is alos not able to give the mimetype
    iImageDecoder = CImageDecoder::DataNewL(iFs, aImageData , *iMimeType);
    iProcessingState = EConvertImageDatatoBitmap;
    RunL();
    return err;  
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ProcessFromFileL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ProcessFromFileL( const TDesC& aFileName , const TDesC8& aMimeType)
    {
    CHAT_DP_FUNC_ENTER("CVimpstImageProcessor::ProcessFromFileL start");
    iMimeType = aMimeType.AllocL();
    iImageDecoder = NULL;
    iImageDecoder = CImageDecoder::FileNewL(iFs, aFileName, *iMimeType );
    iProcessingState = EConvertImageDatatoBitmap;
    RunL();
    CHAT_DP_FUNC_ENTER("CVimpstImageProcessor::ProcessFromFileL end ");
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::CancelProcessing
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::CancelProcessing()
    {
    if( IsActive() )
        {
        Cancel();
        }        
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::RegisterObserver
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::RegisterObserver( MCAImageProcessObserver* aObserver )
    {
    iObserver = aObserver;
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::GetContentData
// ( Other items commented in header )
//-----------------------------------------------------------------------------

HBufC8* CVimpstImageProcessor::GetContentData()
    {
    return iContent;
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::Bitmap
// ( Other items commented in header )
//-----------------------------------------------------------------------------

CFbsBitmap& CVimpstImageProcessor::Bitmap()
    {
    isBitmapReturned = ETrue;
    return *iBitmap;
    }
//-----------------------------------------------------------------------------
// CVimpstImageProcessor::DecodeSize
// Scale to nearest (larger or equal) size with ratio 1:1, 1:2, 1:4 or 1:8
//-----------------------------------------------------------------------------    
TSize CVimpstImageProcessor::DecodeSize( const TSize& aSize, const TSize& aTargetSize,
        TBool aAnyRatio )
    {
    if( aAnyRatio || aSize == aTargetSize )
        {
        // decoder can scale to any ratio or the size is already correct
        return aTargetSize;
        }

    // 1:1 is always valid ratio for decode scaling
    TInt lastValidRatio( 1 );
    for( TInt ratio( KDecodeScaleRatioMin ); ratio <= KDecodeScaleRatioMax; ratio <<= 1 )
        {
        if( aSize.iWidth % ratio + aSize.iHeight % ratio == 0 )
            {
            // this ratio is valid
            if( aSize.iWidth / ratio < aTargetSize.iWidth || 
                    aSize.iHeight / ratio < aTargetSize.iHeight )
                {
                // the decoded size was smaller in some dimension, 
                // the last valid ratio should be used
                break;
                }

            // this scale ratio results to greater or equal size
            lastValidRatio = ratio;
            }
        }

    // return the size scaled with correct ratio
    return TSize( aSize.iWidth / lastValidRatio, 
            aSize.iHeight / lastValidRatio );
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ScaleSize
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TBool CVimpstImageProcessor::ScaleSize( const TSize& aSize, TSize& aNewSize )
    {
    if( Max( aSize.iHeight, aSize.iWidth ) > KChatCustomBitmapWidth )
        {  
        aNewSize.SetSize( KChatCustomBitmapWidth, KChatCustomBitmapHeight );
        return ETrue;
        }

    // old size is correct
    aNewSize = aSize;
    return EFalse;
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::CompleteRequestL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::CompleteRequestL()
    {    
    iObserver->HandleProcessingCompleteL( iStatus.Int() );
    }


//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ResetProcessingL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ResetProcessingL( TInt aReason )
    {
    // cancel operation
    if( IsActive() )
        {
        iStatus = aReason;
        Cancel();
        }  
    }

//-----------------------------------------------------------------------------
// CVimpstImageProcessor::ClearState
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVimpstImageProcessor::ClearState()
    {
    if( iScaler )
        {
        iScaler->Cancel();
        delete iScaler;
        iScaler = NULL;
        }
    if( iImageDecoder )
        {
        iImageDecoder->Cancel();
        delete iImageDecoder;
        iImageDecoder = NULL;
        }
    if( iImageEncoder )
        {
        iImageEncoder->Cancel();
        delete iImageEncoder;
        iImageEncoder = NULL;
        }
    if( iBitmap )
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if (iMimeType)
        {
        delete iMimeType;
        iMimeType = NULL;
        }

    }


// End of file
