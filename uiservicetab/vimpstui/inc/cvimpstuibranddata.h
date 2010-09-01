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
* Description:  Brand handler
*
*/



#ifndef _CVIMPSTUIBRANDDATA_H
#define _CVIMPSTUIBRANDDATA_H

//  INCLUDES
#include    <e32base.h>
#include    <bamdesca.h> // MDesCArray
#include    <AknIconArray.h>
#include    <gulicon.h>
#include    <f32file.h>
#include    <fbs.h>

//Forward Declarations
class CBSFactory;
class MBSAccess; 
class MVIMPSTEngine;


// CLASS DECLARATION

/**
 *  Brand handler
 *
 *  @lib vimpstui.dll
 *  @since 5.0
 */
class CVIMPSTUiBrandData : public CBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
		 * @param aAppId : application Id
		 * @param aBrandId : Brand Id
		 * @param aLanguageId : Language Id
         **/
        static CVIMPSTUiBrandData* NewL(MVIMPSTEngine& aEngine);
           

        /**
         * Destructor.
         **/
        virtual ~CVIMPSTUiBrandData();
	public: // New Functions

   		/**
         * Loads necessary icons for this decorator
         * @return Icon array. Ownership is transferred.
         **/
	     CAknIconArray* LoadIconsLC( );
	     
	     
	   	/**
         * Loads necessary icons for Own status decorator
         * @return Icon array. Ownership is transferred.
         **/
	     CAknIconArray* LoadOwnStatusIconsLC( );
         /**
          * Gets you bitmaps to construct branded icons 
          * @param aBitmap bitmap id to be fetched from Branding server
          * @param aMask bitmap id to be fetched from Branding server
          * @return error code.
          **/
        TInt GetBrandBitmapsL(CFbsBitmap **aBitmap, CFbsBitmap **aMask);
        
          /**
           * helper function to check branding is supported or not
           **/  
          TBool IsBrandingSupported();
          /**
          * Get the string, To be shown as terms of use befor login.
          * ownership is transfered
          * @return ToU String.If does not find then return NULL.
          **/        
          HBufC* GetToUStringL();	     
	     
          
	private:
	
	    /**
	     * Loads necessary icons for this decorator
	     * @return Icon array. 
	     **/
	    CAknIconArray* LoadIconsL( );
	
		/**
         * Loads brand icons for this decorator
         * @return Icon array. Ownership is transferred.
         **/
	     CAknIconArray* LoadBrandIconsL( );
	
		     
	     
	     /**
         * Loads brand icons for ownstatus
         * @return Icon array. Ownership is transferred.
         **/
	     CAknIconArray* LoadBrandOwnStatusIconsLC( );
	
	
	
		/**
         * Loads default icons for ownstatus
         * @param aMbmFile Bitmap file
         * @return Icon array. Ownership is transferred.
         **/
	     CAknIconArray* LoadDefaultOwnStatusIconsLC(const TDesC& aMbmFile );
			
		/**
         * constructs necessary icons for this decorator
         * @param aBitmapId bitmapId
         * @param aMaskId maskId
         * @param aFullPath Bitmap path
         * @return Icon. 
         **/
		 CGulIcon* LoadDefaultBitmapL( TInt aBitmapId,
		 							 TInt aMaskId, 
                                      const TDesC& aFullPath );

		 /**
          * constructs branded icons for this decorator
          * @param aId id of the bitmap to be fetchec from the Branding server.
          * @return Icon. 
          **/
		 CGulIcon* LoadBrandBitmapL( const TDesC8& aId );
		 
		 
	private:
	
        /**
         * C++ default constructor.
		 * @param aContacts Pointer to contacts-API
         */
        CVIMPSTUiBrandData();

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL(MVIMPSTEngine& aEngine);
        
        /**
         * Prepares branding access
         * @return KErrNone if sucessfull, else the Error.
         * @since S60 5.0
         */   
        TInt PrepareBrandingAccess();
        
         /**
         * get branded string.
         * @return Branded string
         * ownership is transfered
         * @since S60 5.0
         */   
        HBufC* GetToUBrandedStringL();

        /*
         * Concatenates the two descriptors. 
         */
        HBufC* ConcatLC( TDesC& aDesc1, TDesC& aDesc2 );

    private:    // Data
        
        // Not own. Settings id of the service..
		const TDesC8& iAppId;
		
		// Not own. Settings id of the service..
		HBufC8* iBrandId;

		// Not own. Settings id of the service..
		TLanguage iLanguageId; 
        
        //Owns, handle to brnadingfactory.
	    CBSFactory* iBrandingFactory; 
	    
	    //Owns, handle to brandaccessor
	    MBSAccess* iBrandingAccess;
	    
	    //stores the default MBM path
	    TFileName iDefaultMbmPath;
	    // owns : list of availabe icons
	    CAknIconArray* iIconsArray;
	                  
	};

#endif      // _CVIMPSTUIBRANDDATA_H

// End of File
