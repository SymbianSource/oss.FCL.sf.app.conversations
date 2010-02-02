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
* Description:  Implementation of detailsview contact brand data
 *
*/



#ifndef CVIMPSTDETAILSVIEWBRANDHANDLER_H
#define CVIMPSTDETAILSVIEWBRANDHANDLER_H

//  INCLUDES
#include    <e32base.h>
#include    <AknIconArray.h>
#include    <gulicon.h>
#include 	"spdefinitions.h"
//Forward Declarations
class CBSFactory;
class MBSAccess; 
class CSPSettings;
// CLASS DECLARATION
		
/**
 *  Brand handler implemenation class
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since 5.0
 */
class CVIMPSTDetailsViewBrandHandler : public CBase
    {
    public:  // Constructors and destructor
		
	
        /**
         * Two-phased constructor.
		 * @param aServiceId , service Id
		 * @return a pointer to this class
		 * @since s60 v5.0
		 **/
        static CVIMPSTDetailsViewBrandHandler* NewL(TInt aServiceId);
           

        /**
         * Destructor.
         * @since s60 v5.0
         **/
        virtual ~CVIMPSTDetailsViewBrandHandler();
	public: // New Functions

   		/**
         * Loads necessary icons for this decorator
         * @return Icon array. Ownership is transferred.
         * @since s60 v5.0
         **/
	     CAknIconArray* LoadIconsLC( );
	     
	     /**
         * Loads necessary icons for this decorator
         * @param aBitmap ,bitmap
         * @param aMask , mask
         * @param aBitmapId  bitmap Id
         * @param aMaskId : mask Id
         * @since s60 v5.0
         **/
	   	 void LoadBitMapLC(CFbsBitmap*& aBitmap,
			             CFbsBitmap*& aMask,
			             TInt aBitmapId,
			             TInt aMaskId  );
		
		/**
		* read the service name from settings
		* @param aServiceName , reference to service name
		* @since s60 v5.0
		**/	             
		void GetServiceNameL( TDes& aServiceName );	
			
		/**
		* read the service store name from settings
		* @return aServiceStoreUri , reference to service store uri with cnt path
		* @since s60 v5.0
		**/	  
		const TDesC& ServiceStoreUriL();	
	
		/**
        * read if service supported from cch 
        * @return ETrue if service supported
        * @since s60 v5.0
        **/   
		TBool ServiceSupportedL( );
		
		/**
		 * constructs branded icons for this decorator
		 * @param aId id of the bitmap to be fetchec from the Branding server.
		 * @return Icon. 
		 * @since s60 v5.0
		 **/
		CGulIcon* LoadBrandBitmapL( const TDesC8& aId );
		
	private:
	
	
		/**
         * Loads brand icons for this decorator
         * @return Icon array. Ownership is transferred.
         * @since s60 v5.0
         **/
	     CAknIconArray* LoadBrandIconsLC( );
	

		/**
         * Loads default icons for this decorator
         * @param aMbmFile Bitmap file
         * @return Icon array. Ownership is transferred.
         * @since s60 v5.0
         **/
	     CAknIconArray* LoadDefaultIconsLC(const TDesC& aMbmFile );
	     
	     
	  
		/**
         * constructs necessary icons for this decorator
         * @param aBitmapId bitmapId
         * @param aMaskId maskId
         * @param aFullPath Bitmap path
         * @return Icon. 
         * @since s60 v5.0
         **/
		 CGulIcon* LoadDefaultBitmapL( TInt aBitmapId,
		 							 TInt aMaskId, 
                                      const TDesC& aFullPath );	 

	
        /**
         * C++ default constructor.
		 * @param aContacts Pointer to contacts-API
		 * @since s60 v5.0
         */
        CVIMPSTDetailsViewBrandHandler(TInt aServiceId);

        /**
         * By default Symbian OS constructor is private.
         * @since s60 v5.0
         */
        void ConstructL();
        
        /**
         * Prepares branding access
         * @return KErrNone if sucessfull, else the Error.
         * @since S60 5.0
         */   
        TInt PrepareBrandingAccess();
        
        /**
         * get the brand info from settings
         * @return KErrNone if sucessfull, else the Error.
         * @since S60 5.0
         */  
        TInt GetBrandInfoL(TLanguage& aBrandLanguage, 
				TInt &aBrandVersion, TDes8& aBrandId);
		/**
         * get the brand version from settings
         * @param aServiceId service id 
         * @param aBrand , Brand to get 
         * @param aPropertyName , property name
         * @return KErrNone if sucessfull, else the Error.
         * @since S60 5.0
         */  		
		void PropertyReadBrandL( TUint32 aServiceId ,TInt &aBrand, 
							TServicePropertyName aPropertyName );
    
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
	    
	    // owns : service id 
	    TInt iServiceId;
	    
	    // owns : settings 
	    CSPSettings* iSettings ;
	    
	     // owns : service store uri 
	    HBufC* iServiceStoreUri;
	  
	                  
	};

#endif      // CVIMPSTDETAILSVIEWBRANDHANDLER_H

// End of File
