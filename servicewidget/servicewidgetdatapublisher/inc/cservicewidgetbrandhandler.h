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
* Description:  brand status icons handler 
 *                
 *
*/

 

#ifndef CSERVICEWIDGETBRANDHANDLER_H
#define CSERVICEWIDGETBRANDHANDLER_H

#include <e32base.h>
#include <gulicon.h>
#include "servicewidgetenums.hrh"

// farword declarations
//Forward Declarations
class CBSFactory;
class MBSAccess; 
class CSPSettings;
/**
 * TODO : Temp solution until homescreenfw support the bitmap and mask publishing 
 * @since 5.0
 */
    
// class defnitions
class CServiceWidgetBrandHandler  : public CBase
	{
	
	friend class Cservicewidgetdatapublisher_ut;
	
	public:

		/**
		* Two-phased constructor.
		*/
		static CServiceWidgetBrandHandler* NewL(TInt aServiceId );

		/**
		* Two-phased constructor.
		*/
		static CServiceWidgetBrandHandler* NewLC(TInt aServiceId );

		/**
		* Standard C++ destructor
		*/    
		~CServiceWidgetBrandHandler();

	public:


		/**
		* @return bitmap path for status icons
		*/
		void GetStatusBitmapL(TWSPresenceStatus aIndex, TInt& aBitmapHandle, TInt& aMaskHandle);


	private: // Implementation

		/**
		* Standard C++ constructor
		*/    
		CServiceWidgetBrandHandler(TInt aServiceId );
	  	
		/**
         * Load the bitmaps from branding server
         * @since S60 5.0
         */
		void LoadBrandingBitmapsL() ;

        /**
         * Load the bitmaps from branding server
         * @return CGulIcon if sucessfull, else the Null.
         * @param aIndex - index of the TWSPresenceStatus enum
         * @since S60 5.0
         */
		CGulIcon* LoadBitmapL(TInt aIndex) ;
		
		/**
		* Performs the 2nd phase of construction.
		* @param aServiceName, service name
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
	

	private: // Data
	
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
	    
	    // owns : service id 
	    TInt iServiceId;
	    
	    RPointerArray <CGulIcon> iBrandIcons;
};

#endif /*CSERVICEWIDGETBRANDHANDLER_H*/
