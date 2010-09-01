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
* Description:  Implementation of detailsview plugin control container
 *
*/


#ifndef CVIMPSTDETAILSVIEWCONTAINER_H
#define CVIMPSTDETAILSVIEWCONTAINER_H

// INCLUDES
#include <e32base.h>
#include <aknlists.h>
#include <ccappviewpluginakncontainer.h>
#include "cvimpstdetailscontacthandler.h"

// forward declaration
class CVIMPSTDetailsViewListBoxModel;
class CVIMPSTDetailsViewBrandHandler;
class CVIMPSTDetailsHeaderControl;
class MEikListBoxObserver;
class CAknsBasicBackgroundControlContext;
class RResourceFile;
class MVIMPSTDetailsPresenceHandler;
class MVPbkStoreContact;

// CLASS DECLARATION

/**
 *  Class implementing CCCAppViewPluginAknContainer interface from cca. This is
 *  this class is responsible for creating the control to show
 *  this creates the two control , one is header and another is list box
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
 
class CVIMPSTDetailsViewContainer : public CCCAppViewPluginAknContainer,
									public MEikListBoxObserver
{
public:

    /**
     * Constructor. This is called from plugin class NewContainerL.
     * @param aPluginUid, plugin uid
     * @param aBrandHandler, reference to brand handler 
     * @param aLaunchParameter, reference to presence handler
     * @param aFocusedListIndex, field index to focus
     */
    CVIMPSTDetailsViewContainer(const TUid& aPluginUid,
    							CVIMPSTDetailsViewBrandHandler& aBrandHandler ,
    							MVIMPSTDetailsPresenceHandler& aPresenceHandler,
    							TInt aFocusedListIndex );

    /**
     * Destructor.
     */
    virtual ~CVIMPSTDetailsViewContainer();

public:  // from base class CCCAppViewPluginAknContainer
	
	/**
     * From CCoeControl
     * (see details from baseclass )
     * @param aRect, a rect layout 
     */
    void Draw( const TRect& aRect ) const;

    /**
     * From CCoeControl
     * (see details from baseclass )
     */
    virtual void SizeChanged();
    
       
    /**
     * From CCoeControl
     * (see details from baseclass )
     * @param aId, object type uid
     */
    TTypeUid::Ptr MopSupplyObject( TTypeUid aId );
    
    /**
     * From CCoeControl
     * (see details from baseclass )
     * @param aDrawNow, reference to draNow class
     */
    void FocusChanged( TDrawNow aDrawNow );
    
    
      /**
     * From CCCAppViewPluginAknContainer.
     * (see baseclass for details)
     * @return  control count
     * @since S60 v5.0
     */
    TInt CountComponentControls() const;

    /**
     * From CCCAppViewPluginAknContainer.
     * (see baseclass for details)
     * @param aIndex, index of control
     * @return selected control
     * @since S60 v5.0
     */
    CCoeControl* ComponentControl(TInt aIndex) const;

    /**
     * From CCCAppViewPluginAknContainer.
     * (see baseclass for details)
     * @param aKeyEvent, a event
     * @param aType, a event type
     * @return key response 
     * @since S60 v5.0
     */
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    /**
     * From CCoeControl
     * (see details from baseclass )
	 * @param aContext, a reference to help context
     */
    void GetHelpContext(TCoeHelpContext& aContext) const;
    
     /**
     * From MEikListBoxObserver
     * (see details from baseclass )
     * @param aListBox, a pointer to list box 
	 * @param aEventType, alist box event type
     */
    void HandleListBoxEventL( CEikListBox *aListBox, TListBoxEvent aEventType ); 

public:
    
    /**
     * Helper function to get listbox.
     *
     * @see CAknDoubleStyleListBox
     * @since S60 5.0
     * @return reference to listbox object.
     */
    CAknFormDoubleGraphicStyleListBox& ListBox() const;

    /**
     * Helper function to get (temporary) listbox model.
     *
     * @see CVIMPSTDetailsViewListBoxModel
     * @since S60 5.0
     * @return reference to listbox model object.
     */
    CVIMPSTDetailsViewListBoxModel& ListBoxModel() const;
      
	/** 
	 * called from plugin
     * notification call back from presence handler about presence change
     * @since s60 v5.0
     */
    void PresenceChangedL() ;

	/**
	* contact retrived
	* @param aContact, a contact fetched from store
	*/	    
	void CreatePresentationL( MVPbkStoreContact& aContact,TDesC& aServiceName,TStoreType aStoreType  )  ;
	
	/**
	* show no data in view
	*/	    
	void CreateEmptyPresentationL()  ;
	
	/**
	* create the title text based on service store typoe
	* @param aStoreType, store type
	* object is pushed on clean up stack if valid
	* @return HBufC pointer , Ownership is given to caller
	*/	
	HBufC* GetTitleTextLC( TStoreType aStoreType );
	    
private:


    // Called by ccapplication framework
    void ConstructL();

    /**
     * See more details from CCoeControl.
     * @param aType, resource type
     */
    void HandleResourceChange(TInt aType);
 
  
private:  // data
   
    /**
     * not owns : Reference to plugin uid.
     */
    const TUid& iPluginUid;
    
     /**
     * not owns : Reference to barnd handler
     */
    CVIMPSTDetailsViewBrandHandler& iBrandHandler;
    
	/**
	* not Owns : reference  to presence
	*/
	MVIMPSTDetailsPresenceHandler& iPresenceHandler;
    
    /**
     *  owns : pointer to the list box
     */
    CAknFormDoubleGraphicStyleListBox* iListBox;
    
    /**
     *  owns : pointer to the list box model
     */
    CVIMPSTDetailsViewListBoxModel* iListBoxModel;
    
    /**
     * owns : Reference to plugin uid.
     */
    CVIMPSTDetailsHeaderControl* iHeaderCtrl; 
    
    /**
     * owns : pointer to background context
     */ 
    CAknsBasicBackgroundControlContext* iBackground;
    
	/**
     * Owns : focus field index in the display list
     */
    TInt iFocusedListIndex;
    
     /**
    * owns : resource files
    */
    RResourceFile iResourceFile;
     
    };

#endif // CVIMPSTDETAILSVIEWCONTAINER_H

// End of File
