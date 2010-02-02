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
* Description: s_vimpstengineaimagehandler.h
*
*/

#ifndef __CVIMPSTENGINEIMAGEHANDLER_H
#define __CVIMPSTENGINEIMAGEHANDLER_H

//  INCLUDES
#include <e32base.h>

#include "mvimpstimageprocessobserver.h"

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
                                                 public MCAImageProcessObserver

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

    };

#endif      //__CVIMPSTENGINEIMAGEHANDLER_H

// End of File
