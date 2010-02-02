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
* Description:  View plugin base class of ECE's CCA-application
 *
*/

 
#ifndef CVIMPSTDETAILSVIEWPLUGIN_H
#define CVIMPSTDETAILSVIEWPLUGIN_H

#include <e32base.h>

#include <ccappviewpluginbase.h>
#include "mvimpstdetailsviewcontactobserver.h"
#include "mvimpstdetailspresencehandler.h"

// Forward declaration
class CVIMPSTDetailsViewMenuHandler;
class CVIMPSTDetailsViewBrandHandler;
class MVIMPSTDetailsPresenceObserver;
class MVIMPSTDetailsViewContactObserver;
class MVIMPSTDetailsPresenceHandler;
class CVIMPSTDetailsContactHandler;

class MVPbkStoreContact;
class MCCAParameter;
class CAknIcon;


/**
 *  Class implementing CCCAppViewPluginBase interface. This is
 *  the main class and controlling the other classes of the plugin.
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */

class CVIMPSTDetailsViewPlugin : public CCCAppViewPluginBase,
								 public MVIMPSTDetailsPresenceObserver,
								 public MVIMPSTDetailsViewContactObserver
	{
	public:

	    /**
	     * Two-phased constructor.
	     */
	    static CVIMPSTDetailsViewPlugin* NewL(TInt aServiceId);

	    /**
	     * Destructor.
	     */
	    ~CVIMPSTDetailsViewPlugin();

	private:

	    // from base class CCCAppViewPluginBase

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aPluginParameter, reference to plugin parameter
	     * @since S60 v5.0
	     */
	    void PreparePluginViewL(MCCAppPluginParameter& aPluginParameter);

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     *
	     * Note: This will be removed.
	     * @since S60 v5.0
	     */
	    TUid ImplementationUid();

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aIconType, reference to icon type
	     * @param aBitmap, reference to bitmap pointer
	     * @param aBitmapMask, reference to mask pointer
	     * @since S60 v5.0
	     */
	    void ProvideBitmapLC(TCCAppIconType aIconType, CFbsBitmap*& aBitmap,
	        				CFbsBitmap*& aBitmapMask);

	    /**
	     * From CCCAppViewPluginBase
	     * NEW API
	     * @param aIconType, reference to icon type
	     * @param aIcon, reference to icon
	     */
	    void ProvideBitmapL(TCCAppIconType aIconType, CAknIcon& aIcon);

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * create the container class
	     * @since S60 v5.0
	     */
	    void NewContainerL();

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aResourceId, a resource id 
	     * @param aMenuPane, reference to menu pane
	     * @since S60 v5.0
	     */
	    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aCommand, a command id to handle 
	     * @since S60 v5.0
	     */
	    void HandleCommandL(TInt aCommand);

	    /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @return the plugin uid
	     * @since S60 v5.0
	     */
	    TUid Id() const;

		/**
		* From CCCAppViewPluginBase
		* @param aPrevViewId, previous view id
		* @param aCustomMessageId, a custom message id  
		* @param aCustomMessage, a custom message 
		*/
	    void DoActivateL( const TVwsViewId& aPrevViewId,
				          TUid aCustomMessageId,
				           const TDesC8& aCustomMessage );

	    /**
	     * From CCCAppViewPluginBase
	     */
	    void DoDeactivate();

	public:   // new

	    /**
	     * Helper function for get the reference
	     * to container data.
	     *
	     * @since S60 v5.0
	     * return container
	     */
	    inline const CCCAppViewPluginAknContainer& GetContainer()
		    {
		    return *iContainer;
		    }
	    /**
	     * Helper function for get contact handler         
	     *
	     * @since S60 v5.0
	     * return iContactHandler
	     */
	    CVIMPSTDetailsContactHandler* GetContactHandler();

	public :  // from MVIMPSTDetailsPresenceObserver
		
		/** from MVIMPSTDetailsPresenceObserver
	     * notification call back from presence handler about presence change
	     * @since s60 v5.0
	     */
	    void HandlePresenceChangedL() ;

	public : // MVIMPSTDetailsViewContactObserver

		/**
		* From MVIMPSTDetailsViewContactObserver
		* call back from contact manager
		* @param aContact, a contact fetched from store
		*/	    
		void HandleContactReadyL( MVPbkStoreContact& aContact )  ;
		
		/**
		* From MVIMPSTDetailsViewContactObserver
		* call back from contact manager
		* @param aContact, a contact fetched from store
		*/	    
		void HandleContactUnavailableL()  ;
	
	private:   // new

	    /**
	     * Helper function for reading and
	     * loading resources needed by plugin.
	     *
	     * @since S60 v5.0
	     */
	    void PrepareViewResourcesL();
		
		/**
	     * default constructor
	     * @since S60 v5.0
	     */
	    CVIMPSTDetailsViewPlugin(TInt aServiceId);

		/**
	     * providing the two phase  construction
	     * @since S60 v5.0
	     */
	    void ConstructL();

		/**
	     * Helper function for reading 
	     * contact data from passed link .
	     * @since S60 v5.0
	     */
	    void GetContactDataL();
	    
	   
	    /**
	     * set title text
	     * @since S60 v5.0
	     */
	    void SetTitleL();


	private: // data

	    /**
	     * owns : pointer to commlauncher's menuhandler
	     */
	    CVIMPSTDetailsViewMenuHandler* iMenuHandler;
	 
	     /**
	     * not Owns : Pointer to launch parameter
	     */
	    MCCAParameter* iLaunchParameter;

	    /**
	     * Owns : focus field index in the display list
	     */
	    TInt iFocusedListIndex;
	  
	    /**
	     * Owns : service id to use
	     */
	    TInt iServiceId;
	    /**
	     * Owns : pointer to brand data handler
	     */
	    CVIMPSTDetailsViewBrandHandler* iBrandHandler;
	    
		/**
		*  owned : pointer to presence handler
		*/  
		MVIMPSTDetailsPresenceHandler* iPresenceHandler; 

		/**
		* Owns : pointer to contact handling from virtual phonebook
		*/
		CVIMPSTDetailsContactHandler* iContactHandler;
	
		};

#endif // CVIMPSTDETAILSVIEWPLUGIN_H

// End of File
