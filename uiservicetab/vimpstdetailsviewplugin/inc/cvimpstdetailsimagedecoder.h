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

 
#ifndef CVIMPSTDETAILSIMAGEDECODER_H_
#define CVIMPSTDETAILSIMAGEDECODER_H_

#include <e32base.h>
#include <f32file.h>

class CVIMPSTDetailsHeaderControl;
class CImageDecoder;
class CFbsBitmap;

/**
 * Utility class for asynchronously decoding the header thumbnail image
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
class CVIMPSTDetailsImageDecoder : public CActive
    {
public:
    /**
     * Two-phased constructor
     * 
     * @param aHeader The header control to which the bitmap is to be inserted after loading
     * @param aBitmapData Descriptor containing the bitmap data stream
     * @return a instance of same class
     * @since S60 v5.0 
     */
    static CVIMPSTDetailsImageDecoder* NewL(CVIMPSTDetailsHeaderControl& aHeader, const TDesC8& aBitmapData);
    
    /**
     * Destructor
	 * @since S60 v5.0 
     */
    ~CVIMPSTDetailsImageDecoder();
    
    /**
     * Starts the decoding process
     * @since S60 v5.0 
     *
     */
    void Start();
    
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
    CVIMPSTDetailsImageDecoder(CVIMPSTDetailsHeaderControl& aHeader);
    
    /**
     * Second phase constructor
     * @since S60 v5.0 
     * @param aBitmapData Descriptor containing the bitmap data stream
     */
    void ConstructL(const TDesC8& aBitmapData);
  
 private : // data  
    
  
    /**
     * Not Owns : The header control to which the bitmap is to be inserted after loading
     */
    CVIMPSTDetailsHeaderControl& iHeader;


    /**
     * Owns : The image decoder whose service is called to decode the image
     */
    CImageDecoder* iImgDecoder;
      
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
    };

#endif /*CVIMPSTDETAILSIMAGEDECODER_H_*/
