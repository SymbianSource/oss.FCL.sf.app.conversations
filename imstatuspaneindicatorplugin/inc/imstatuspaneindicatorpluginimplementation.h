/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is the imstatuspaneindicator plugin implementation
*/


#ifndef __CIMSTATUSPANEINDICATORPLUGINIMPLEMENTATION_H
#define __CIMSTATUSPANEINDICATORPLUGINIMPLEMENTATION_H

// Until aknstylusactivatedpopupcontent.h is fixed
class TAknWindowComponentLayout; 

//  INCLUDES
#include <AknIndicatorPlugin.h>
#include "imstatuspaneindicatorpluginuids.h"


//system includes
#include <aknenv.h>
#include <e32base.h>
#include <barsc.h>
#include <xSPViewServices.h>

//Forward Declarations
class CIMStatusPaneIndicator;
class RxSPViewServices;

_LIT( KResourceDir, "\\resource\\" );
_LIT( KIndicatorResourceFile, "imstatuspaneindicatorpluginrss.rsc" );
const TInt KMaxMessageNumberLength = 4; // 9999 = 4 numbers
const TInt KMaxResourcePathLength  = 512;
#define KMECOIMPLEMENTATIONUID  0x20012423
/**
*  Instant msg indicator Plug-in implementation
*
*  @lib ImIndicatorPlugin
*  @since 5.0
*/
class CIMStatusPaneIndicatorPluginImplementation : public CAknIndicatorPlugin
                                                
                                                 
    {
        
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CIMStatusPaneIndicatorPluginImplementation* NewL();

        /**
        * Destructor.
        */
        ~CIMStatusPaneIndicatorPluginImplementation();

    public:// From CAknIndicatorPlugin
    
	    /**
	    * Returns the description used as link text, displayed with
	    * the indicator icon in the indicator popup.
	    *
	    * @param       aUid       UID of the indicator.
	    * @param[out]  aTextType  Type of the text,
	    *                         @see TAknIndicatorPluginTextTypes.
	    *
	    * @return  Indicator description or link text.
	    *          Ownership is transferred.
	    */
    	HBufC* TextL( const TInt aUid, TInt& aTextType );
    	
    	 /**
          * @see CAknIndicatorPlugin
          */
        void HandleIndicatorTapL( const TInt aUid );
   
    private:

        /**
        * C++ default constructor.
        */
        CIMStatusPaneIndicatorPluginImplementation();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private: // new functions

        /**
         * Create singular or plural content message for the popup
         * @return constructed text
         * 
         * NOTE! Ownership of the text is transferred to caller.
         */
        HBufC* CreatePopupContentMessageL(); 
        
        /**
         * Shows IM message(s)
         */
       void ShowMessageL();
      
    private: // data
    
    // environment
    CCoeEnv& iCoeEnv;
    
    // offset index for resource file
    TInt iResourceOffset;
    // Owns. Popup content singular (template)
    HBufC* iPopupTextOne;

   // Owns. Popup content plural (template)
    HBufC* iPopupTextMultiple;
    
    CIMStatusPaneIndicator* iIMStatusPaneIndicator;
    /*
     * Handle to xSPViewServices.
     */ 
    RxSPViewServices iViewServices;
    };

#endif      // __CIMSTATUSPANEINDICATORPLUGINIMPLEMENTATION_H

// End of File
