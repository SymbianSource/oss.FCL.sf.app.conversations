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
* Description: Image handler for VImpstengine handler
*
*/

#ifndef __CVIMPSTENGINEIMAGEHANDLER_H
#define __CVIMPSTENGINEIMAGEHANDLER_H

//  INCLUDES
#include <e32base.h>

#include "mvimpstimageprocessobserver.h"
#include <mdaimageconverter.h>
// FORWARD DECLARATIONS           
class CVimpstImageProcessor;
class CFbsBitmap;

// CLASS DECLARATION
/**
 * This Class is wrapper over the CVimpstImageProcessor implements the processing of image
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineImageHandler ) : public CBase,							   
                                                 public MCAImageProcessObserver,
                                                 public MMdaImageUtilObserver

    {


    public:  // Two-phased constructors and destructor

        /**
         * Two-phased constructor.
         */

        IMPORT_C static CVIMPSTEngineImageHandler* NewL();
        /**
         * Two-phased constructor.
         */										
        IMPORT_C static CVIMPSTEngineImageHandler* NewLC();
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        /**
         * C++ Destructor.
         */
        virtual ~CVIMPSTEngineImageHandler();


    private:

        /**
         * default constructor
         */
        CVIMPSTEngineImageHandler();


public: //from MMdaImageUtilObserver
    /**
     * Defines required client behaviour when a create operation has completed.
     *
     * @param aError:A value identifying the status of the create operation. KErrNone or one of the system wide error codes.
     * 
     */
    void MiuoCreateComplete(TInt aError);
    /**
     * Defines required client behaviour when an open operation has completed.
     *
     * @param aError: A value identifying the status of the open operation. KErrNone or one of the system wide error codes
     */
    void MiuoOpenComplete(TInt aError);
    /**
     * Defines required client behaviour when a conversion, image rotation or image re-scaling operation has completed.
     *
     * @param aError: A value identifying the status of the operation. KErrNone or one of the system wide error codes.
     */
    void MiuoConvertComplete(TInt aError);

    public: 
        /**
         * process the image from file name
         * @return processed image content	     
         */
        IMPORT_C HBufC8* ProcessImageFromFileL(const TDesC& aFilename ,
                const  TDesC8& aMimetype);
        /**
         * process the image from image data
         * @return processed image content      
         */
        IMPORT_C HBufC8* ProcessImageFromDataL( const TDesC8& aImageData , const TDesC8& aMimeType);
        /**
         * process the image from image bitmap
         * @return processed image content      
         */
        IMPORT_C HBufC8* ProcessFromBitmapL( CFbsBitmap& aBitMap );
			 /**
        * Get Bitmap
        * @return bitmap
        */   
        IMPORT_C CFbsBitmap& Bitmap();
    public:  // from MCAImageProcessObserver

        /**
         * See MCAImageProcessObserver	     
         */
        void HandleProcessingCompleteL( TInt aStatus );	


    private:   	


        //Result code from waited request
        TInt    iReqResult;

        // Owns content
        HBufC8* iContent;

        // Owns imageporcessor
        CVimpstImageProcessor* iProcessor;
        
        // own active sheduler 
        CActiveSchedulerWait    iWait;
        /*
         * bitmap to descriptor pointer
         * Owns
         */
        CMdaImageBitmapToDescUtility* iToDesc;
        /*
         * image format
         */
        TMdaJfifClipFormat iFormat;
        /*
         * stores converted descriptor from bitmap
         * Doesn't Own: ownership is transferred to caller
         */
        HBufC8* iBuf;
        /*
         * stores error code
         */
        TInt iError;

    };

#endif      //__CVIMPSTENGINEIMAGEHANDLER_H

// End of File
