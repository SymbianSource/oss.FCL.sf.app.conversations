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

 
#ifndef CSERVICEWIDGETIMAGEDECODER_H
#define CSERVICEWIDGETIMAGEDECODER_H

#include <e32base.h>
#include <f32file.h>

class CImageDecoder;
class CBitmapScaler;
class CFbsBitmap;
class CImageEncoder;

/**
 * decode notification class
 *
 *  @lib servicewidgetdatapublisher.dll
 *  @since S60 v5.0
 */
 
class MServiceWidgetImagerDecodeObserver
	{
	public :
			/**
			* HandlerDecodeComplete notification once decode complete
			* @param aBitmap decoded bitmap
			* @since S60 v5.0 
			* takes the ownership of aBitmap
			*/
			virtual void HandlerDecodeCompleteL() = 0 ;
			
			/**
			* virtaul Destructor
			* @since S60 v5.0 
			*/
			virtual ~MServiceWidgetImagerDecodeObserver()
				{
					
				}
	};

/**
 * Utility class for asynchronously decoding the header thumbnail image
 *
 *  @lib servicewidgetdatapublisher.dll
 *  @since S60 v5.0
 */

class CServiceWidgetImageDecoder : public CActive
    {
    enum TSWDEOperationId
	    {
	    EOpUnknown = -1,
	    EOpEncodeBitmap,
	    EOpComplete	
	    };
public:
    /**
     * Two-phased constructor
     * 
     * @param aHeader The header control to which the bitmap is to be inserted after loading
     * @param aBitmapData Descriptor containing the bitmap data stream
     * @return a instance of same class
     * @since S60 v5.0 
     */
    static CServiceWidgetImageDecoder* NewL(MServiceWidgetImagerDecodeObserver& aObserver );
    
    /**
     * Destructor
	 * @since S60 v5.0 
     */
    ~CServiceWidgetImageDecoder();
    
    /**
     * Starts the decoding process
     * @since S60 v5.0 
     * @param aBitmapData Descriptor containing the bitmap data stream
     *
     */
    void StartDecodingL( const TDesC8& aBitmapData );
    
    /**
     * From CActive
     * (see details from baseclass )
     * @since S60 v5.0 
     */
    void RunL();
    
    /**
     * From CActive
     * (see details from baseclass )
     * @since S60 v5.0 
     */
    void DoCancel();
private:
    /**
     * First-phase (C++) constructor
     * @param aHeader The header control to which the bitmap is to be inserted after loading
     * @since S60 v5.0  
     */
    CServiceWidgetImageDecoder(MServiceWidgetImagerDecodeObserver& aObserver);
    
    /**
     * Second phase constructor
     * @since S60 v5.0 
     */
    void ConstructL();
    
    /**
     * helper to do encoding
     * @since S60 v5.0 
     */
    void StartEncodingL();
  
 private : // data  
    
  
    /**
     * Not Owns : notify once complete
     */
    MServiceWidgetImagerDecodeObserver& iObserver;


    /**
     * Owns : The image decoder whose service is called to decode the image
     */
    CImageDecoder* iImgDecoder;
    
    /**
     * Owns : The image encoder whose service is called to decode the image
     */
    CImageEncoder* iImageEncoder;
      
    /**
     * Owns : Handle to the file server session for loading the bitmap
     */ 
    RFs iFs;
    
    /**
     * The bitmap.
     * Owned until decoding is complete, then ownersip is transferred to header control.
     */
    CFbsBitmap* iBitmap; // owned until completion
    
    /**
     * Owns : Copy of the bitmap data stream
     */
    HBufC8* iBitmapData;
    
     /**
     * Owns : Copy of mimeType
     */
    HBufC8* iMimeType;
    
    // next operation to perform
    TSWDEOperationId iNextOperationId;
    };

#endif /*CSERVICEWIDGETIMAGEDECODER_H*/
