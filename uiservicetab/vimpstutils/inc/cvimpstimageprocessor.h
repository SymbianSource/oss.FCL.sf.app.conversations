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
* Description:  Image processor
*
*/


#ifndef CVIMPSTIMAGEPROCESSOR_H
#define CVIMPSTIMAGEPROCESSOR_H

#include <f32file.h>
#include "mvimpstimageprocessobserver.h"
class CImageDecoder;
class CBitmapScaler;
class CFbsBitmap;
class CImageEncoder;
// CLASS DECLARATION

/**
 *  Image processor
 *
 *  @lib vimpstutils.dll
 */
class CVimpstImageProcessor : public CActive
	{
   public: 
   		/**
	     * Content processing states
	     */
	    enum TContentProcessState
	        {
	        EContentNotProcessed = 0,
	        EConvertImageDatatoBitmap,
	        EScaleBitmap, 
	        EConentFromScaledBitmap,
	        EProcessingComplete
	        };
    public: // Construction
		
		/**
		 * Construction
		 */
		static CVimpstImageProcessor* NewL( MCAImageProcessObserver* aObserver);
		
		/**
		 * Destruction
		 */
		~CVimpstImageProcessor();
		
	private: // Construction
	
		/**
		 * Constructor
		 */
		CVimpstImageProcessor(MCAImageProcessObserver* aObserver);
		
		/**
		 * Constructor
		 */
		void ConstructL();

    private: // From CActive
    
        /**
         * @see CActive
         */
	    void RunL();
	    	    
        /**
         * @see CActive
         */
	    void DoCancel();
	    
        /**
         * @see CActive.
         */
		TInt RunError( TInt aError );	    
		
   public: 
       /**
        * This is used when the image data is ready
        * image data is proceesed and the new content is of the image is given
        * by reducing the image data size
        * 
        * @param  aImageData : Content of the image file
        * @param  aMimeType : Mime type of the file from which the content is passed
        */
       TInt ProcessFromDataL( const TDesC8& aImageData , const TDesC8& aMimeType);
       /**
        * This is used when the image data is ready
        * image data is proceesed and the new content is of the image is given
        * by reducing the image data size
        * 
        * @param  aFileName : image file name
        * @param  aMimeType : Mime type of the file from which the content is passed
        */
       void ProcessFromFileL( const TDesC& aFileName , const TDesC8& aMimeType);
       /**
        * Cancel the image processing operation 
        */ 
       void CancelProcessing();
       /**
        * Register the observer to give callback
        * once the processing is done
        * @param  aObserver : obeserver
        */   
       void RegisterObserver( MCAImageProcessObserver* aObserver );
       /**
        * Retrieve the processed image
        * Ownership of iContent is transferred to caller
        * @return  processed image
        */  
       HBufC8* GetContentData();
       /**
        * Get Bitmap
        * @return bitmap
        */   
       CFbsBitmap& Bitmap();
           
   private: // New helper methods
       
       /**
        * Convert image to bitmap
        */
       void  ConvertImageToBitmapL();
       /**
        * Scale Bit map
        */
       void  ScaleBitmapL();
       /**
        * store scaled bitmap content into iimageencoder
        */
       void  ContentFromScaledBitmapL();
       /**
         * Get new size after decoding.
         * Scale to nearest (larger or equal) size with ratio 1:1, 1:2, 1:4 or 1:8
         * directly when decoding.
         *
         * @param aSize Size of original image
         * @param aTargetSize Size of target image
         * @param aAnyRatio Set this to ETrue if the decoder is cabable of scaling to any          *  ratio
         * @return New size of decoded image
         */
        TSize DecodeSize( const TSize& aSize, const TSize& aTargetSize,
                          TBool aAnyRatio );
        
        /**
         * Get new size after scaling. If scale not needed return EFalse
         * Scaled size will be 40x30 if width > height or 30x40 if 
         * height > width.
         *
         * @param aSize Original size of image
         * @param aNewSize New size of scaled image
         * @return ETrue if scale needed.
         */
        TBool ScaleSize( const TSize& aSize, TSize& aNewSize );
        
        /**
         * Notify observer for completion and remove message from array         
         */
        void CompleteRequestL();
        /**
         * Reset current operation and remove all pending messages from stack.
         * @param aReason reson for reseting
         */
        void ResetProcessingL( TInt aReason );
        
        /**
         * Deletes and cancels all processing units ( decoder, encoder etc ).
         */
        void ClearState();

    
    private: // Data
    
        /// File system access
        RFs iFs;
        
        // Decoder for image.
        CImageDecoder* iImageDecoder;
        CBitmapScaler* iScaler;
        CImageEncoder* iImageEncoder;
        /**
         *  Bitmap. Owns.
         */
        CFbsBitmap* iBitmap;
        /**
         *  Target size for bitmap
         */
        TSize iTargetSize;

        /**
         *  Thumbnail size for bitmap
         */
        TSize iThumbSize;
        /**
         * Doesn't Own
         */ 
        HBufC8* iContent;

        /// Owns.
        HBufC8* iMimeType;
        TContentProcessState iProcessingState;
        HBufC* iFilename;
        
        MCAImageProcessObserver* iObserver;
        TInt iScaleSize;    
        TBool isBitmapReturned;
        };

#endif      // CVIMPSTIMAGEPROCESSOR_H

// End of File
