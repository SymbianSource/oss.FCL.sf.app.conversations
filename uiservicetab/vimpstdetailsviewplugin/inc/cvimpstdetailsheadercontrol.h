/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the header part control(s) of the details view layout
*
*/


#ifndef CVIMPSTDETAILSHEADERCONTROL_H
#define CVIMPSTDETAILSHEADERCONTROL_H

// INCLUDES
#include <e32base.h>
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CEikImage;
class CEikLabel;
class CVIMPSTDetailsViewBrandHandler;
class MVPbkStoreContact;
class MVPbkStoreContactField;
class CVIMPSTDetailsImageDecoder;
class MVIMPSTDetailsPresenceHandler;

// CLASS DECLARATION
/**
 *  Class implementing the header part control of vimpstdetailsviewplugin
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
 
class CVIMPSTDetailsHeaderControl : public CCoeControl
    {
public:
    // Construction & destruction
    /**
     * Two-phased constructor
     * @param aBrandHandler, Reference to brand handler class
     * @param aPresenceHandler , reference presence handler
     * @return a instance of this class
     * @since S60 v5.0
     */
    static CVIMPSTDetailsHeaderControl* NewL(CVIMPSTDetailsViewBrandHandler& aBrandHandler,
    										 MVIMPSTDetailsPresenceHandler& aPresenceHandler );
    
    /**
     * Destructor.
     * @since S60 v5.0
     */
    ~CVIMPSTDetailsHeaderControl();

public: // FROM CCoeControl    
    // Functions from base classes 
    /**
     * From CCoeControl
     * Component control handling (compound control)
     * @since S60 v5.0
     */
    TInt CountComponentControls() const;
    
    /**
     * From CCoeControl
     * Component control handling (compound control)
     * @since S60 v5.0
     */
    CCoeControl* ComponentControl(TInt aIndex) const;
    
    /**
     * From CoeControl
     * Called by framework when the view size is changed.
     * @since S60 v5.0
     */
    void SizeChanged();
    
    /**
     * From CoeControl
     * Overridden to set the container window for child controls
     * @since S60 v5.0
    */
    void SetContainerWindowL(const CCoeControl& aContainer);
    
     /**
     * Sets the bitmap shown in the header image 
     * Ownership is transferred
     * 
     * @param aBmp The bitmap
     * @param aMask The bitmap mask
     * @since S60 v5.0
     */
    void SetBitmap(CFbsBitmap* aBmp, CFbsBitmap* aMask );
    
    /**
     * Clears the texts & the image
     * @since S60 v5.0
     */
    void ClearL();

public:
    
   
    /**
     * create the presentation for given contact 
     * @param aContactStore the contact store
     * @since S60 v5.0
     */
     void CreateHeaderPresentationL( MVPbkStoreContact& aContact );
     
  	/**
     * Helper function for the leaving part of changing labels size
     * @since S60 v5.0
     */
    void LabelsSizeChangedL();
    /**
     * Helper function for changing avatar icon in the header 
     * @since S60 v5.0
     */
    void HeaderSizeChangedL();
    
private:

    /**
     * Constructor for performing 1st stage construction
     * @param aBrandHandler , reference brand handlere
     * @param aPresenceHandler , reference presence handler
     * @since S60 v5.0
     */
    CVIMPSTDetailsHeaderControl(CVIMPSTDetailsViewBrandHandler& aBrandHandler, 
    						 MVIMPSTDetailsPresenceHandler& aPresenceHandler );

    /**
     * Constructor for performing 2nd stage construction
     * @since S60 v5.0
     */
    void ConstructL();
  
	    
private:  // data member

 	/**
     * not Owned : The label controls
     */  
   	CVIMPSTDetailsViewBrandHandler& iBrandHandler;
   	
	/**
     * Not owned : reference to presence handler
     */  
	MVIMPSTDetailsPresenceHandler& iPresenceHandler;  
      
      
    /**
     * Owns : The image control
     */
    CEikImage* iImage;
    
    /**
     * Owns : The bitmap
     */
    CFbsBitmap* iBitmap;
    
    /**
     * Owns : The mask
     */
    CFbsBitmap* iMask;
    
    /**
     * Owns : The label controls
     */    
    RPointerArray<CEikLabel> iLabels;
       
  
   
	/**
     * Owns : virtual pbk field
     */  
	MVPbkStoreContactField* iHeaderField;

	/**
     * Owns : image decoder
     */  
	CVIMPSTDetailsImageDecoder* iImageDecoder;
	

    };

#endif // CVIMPSTDETAILSHEADERCONTROL_H
