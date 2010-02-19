/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  application main view
*
*/



#ifndef _CIMCVAPPVIEW_H
#define _CIMCVAPPVIEW_H

// INCLUDES
#include "cimcvappviewcontainer.h" 
#include <coecntrl.h>
#include <aknview.h>
#include "mimcvenginemessagehandlerobserver.h"
#include "mimcvrecipientobserver.h"
#include "mimcvtabobserver.h"
#include "mimcvnewmsgfadecontrolobserver.h"
#include "mimcvenginechatlistobserver.h"
#include <akntoolbarobserver.h>
//For KMaxPackedContactLinkLength
#include "imcvuiparams.h"
#include "mimcvngineservicechangeobserver.h"
#include <AiwCommon.h>
// FORWARD DECLARATIONS
class CIMCVAppUi;
class MIMCVEngineChatInterface;
class CFindItemMenu;
class CItemFinder;
class CIMCVAppUi;
class CIMCVEngine;
class CIMCVAppStatusPaneHandler;
class MCCAConnection;
class MCCAParameter;
class CIMCVMenuExtensionManager;
class MIMCVEngineFactory;
class CAiwServiceHandler;


// CLASS DECLARATION

/**
 *  This is view class for conversation view
 */
 
class CIMCVAppView : public CAknView,
					 public MIMCVRecipientObserver,
					 public MIMCVTabObserver,
					 public MIMCVNewMsgFadeControlObserver,
					 public MAknToolbarObserver,
					 public MIMCVEngineServiceChangeObserver
    
    {

    public:  // Constructors and destructor

        /**
         * C++ default constructor.
         */
        CIMCVAppView();

        /**
         * Symbian 2nd phase constructor.
         * @param aAvkonViewResourceId Resource id for view
         * @param aViewId Id of the view
         */
        void ConstructL( TInt aAvkonViewResourceId, TUid aViewId, 
                            CIMCVAppMessageExtensionsHandler& aMessageHandler);

        /**
         * Destructor.
         */
        virtual ~CIMCVAppView();
	
	
	public : // MIMCVEngineServiceChangeObserver
	
	    /*
	     * new service has been installed
	     */
	    void HandleNewServiceL( TInt aServiceId )  ;
	    /*
	     *  service has been deleted
	     */
	    void HandleServiceDeletedL( TInt aServiceId ) ;
	    /**
	     * handle presence change notification
	     * call back from imcvuiengine
	     */
	    void HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId ) ;

	    /**
	     * Handle message send operation completion.
	     * @param aResult result  to handle.
	     */
	    void HandleOperationCompletdL(TInt aServiceId , MIMCVEngineMessage::TMessagerType aType , TInt aResult);

	    /**
	     * @see HandleConnectionTerminatedL
	     * to handle the connection lost
	     * @param aReason, reason report
	     */
	    void HandleConnectionEventL(TInt aServiceId, TIMCVConnectionState aState, TInt aReason )  ;

	    /*
	     * See MIMCVEngineChatListObserver
	     */
	    void HandleChatListEvent(TInt aServiceId,  TChatListEventType aEvent, 
	            MIMCVEngineMessageReadInterface* aContainerInfo  ) ;

	           
    public: // New methods

        /**
         * Handles changes in CBA states
		 * @since 5.0
         * @param aResourceId Resource that changes
         */		
        void HandleCbaChangeL( TInt aResourceId );
        
        /**
         * SetStatusPaneIconsL set the icons based on presence status
		 * @since 5.0
         */	
        void SetStatusPaneIconsL();
        
         /**
         * From MIMCVRecipientObserver
         * Shows customized menu
         */
        void ShowPopUpMenuL( );

        /**
         * Function to launch help
         */
        void LaunchHelpL( const TDesC& aContext);          

        void ShowMeL( const TDesC8& aParamData ) ;
        
        void UpdateToolbarButtonsStateL();
        
        /**
         * Launches the contact details view 
         * 
         */
        void LaunchCcaL();
        
        /*
         * return active engine reference
         */
        CIMCVEngine& ActiveEngine() const ;
   
    private: // Functions from base classes

        /**
         * From MEikMenuObserver, Called by framework before 
         * constructing menupane
         * @param aResourceId Resource to be constructed
         * @param aMenuPane Pointer to menupane being constructed
		 * @see MEikMenuObserver
         */
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
        
        /**
	     * Resolve currently selected item type and set it to CItemFinder
	     */
        void UpdateItemTypeL();    
   
    private:

        /**
         * From CAknView, Gets called from framework when activating this view
         * @param aPrevViewId Previous view id
         * @param aCustomMessageId Custom message's id
         * @param aCustomMessage Custom message
		 * @see CAknView
         */
        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
                const TDesC8& aCustomMessage);

        /**
         * From AknView, deactivates view
		 * @see CAknView
         */
        void DoDeactivate();

       

        /**
         * From CAknView 
         * @param aCommand Command that was received
		 * @see CAknView
         */
        void HandleCommandL(TInt aCommand);

        /**
         * Handles foreground event
         * @param aForeground ETrue if foreground else EFalse
		 * @see CAknView
		 */
        void HandleForegroundEventL( TBool aForeground );
        
		/**
		* Show query dialog
		* @param aDialogResourceId resource id
		* @param aPrompt text to be shown
		*/
		TInt DisplayQueryDialogL( TInt aDialogResourceId, 
            					  const TDesC& aPrompt );
		
	



	private:
		/** Helper Function
		 * This function is to be called when switching between tabs
		 * @param aChatId - Switch to this chat item
		 * @param aTabIndex - Index of the tab
		 */
		
		void SwitchTabL(TInt aActiveServiceId , const TDesC& aChatId, const TInt aTabIndex);
		
	
		
	private:
		
		/**
         * From MIMCVNewMsgFadeControlObserver          
		 * @see MIMCVNewMsgFadeControlObserver
         */		
		void NotifyWhenCompleted() ;
		
		/**
         * From MIMCVNewMsgFadeControlObserver          
		 * @see MIMCVNewMsgFadeControlObserver
         */
		void NotifyWhenStarted();
	    
	   public:
	   		/** 
		 		* This function is to be called when left/right Navigation key is pressed
		 		* @param aKeyEvent 
		 		* @param aType
		 		*/
	       TKeyResponse OfferTabEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	public: // MAknToolbarObserver
	    /**
	     * Called when toolbar events occur
	     *
	     * @since 5.0
	     * @return void
	     */
	    void OfferToolbarEventL( TInt aCommand ) { HandleCommandL(aCommand); }
		
	private:
		/**
		 * id from base clas.
		 */
	           
	    TUid Id() const;
	    
	   /**
         * constructs necessary icons for this decorator
         * @return Icon. 
         **/
		 CGulIcon* LoadBitmapL(  );
	   
	        
         /**
         * refreshes the view.
         */
         void ViewRefreshL();
         
         /*
		 *  switch the valid service references
		 */
         void SwitchToValidServiceL( TInt aServiceId );
         
         /*
          * Deltete the CIMCVAppRichTextContainerCache when xsp id deleted from detail view..
          */
         void DeleteContainerCache(const TDesC& aPrimaryKey, TInt aServiceId );

    private: // Data

        // View's UID
        TUid iViewId;

        // Owns. Container
        CIMCVAppViewContainer* iContainer;

        //Owns. Contact id
        HBufC* iRecipientUserId;
      		
	    //Doesn't own. Message handler
        CIMCVAppMessageExtensionsHandler* iExtensionHandler;
       
              
        // not owned, from CCoeEnv
        CIMCVAppUi* iAppUi;
        
         
	    //stores the default MBM path
	    TFileName iDefaultMbmPath;
              
          // Owns. Popup-menu for find item UI
        CEikMenuBar* iFindContextMenu;   
        
        // owns. menu pane for find item functionality
        CFindItemMenu* iFindMenu;
        
        // owns. 
        CItemFinder* iItemFinder;    
		
		//Service Id
		TInt iServiceId;
		
		//Doesnt Own
		CIMCVAppStatusPaneHandler* iStatusPane;

		
		 MCCAConnection* iConnection;
		 
		 MCCAParameter* iParameter;
		
       	TBool iViewDeactivated;
       	
       	//Owns Cache to store CIMCVAppRichTextContainer objects
       	CIMCVAppRichTextContainerCache* iRTContainerCache;
       	
        //Owns. "From me" icon
        CGulIcon* iFromMe;
        
        //Owns. "To me" icon
        CGulIcon* iToMe;
        
        //Owns Menu Extention manager
        CIMCVMenuExtensionManager* iMenuExtManager;
        
        // not owns : reference to imcvuiengine component
        MIMCVEngineFactory* iEngineFactory ;
        
        // not owns : active change engine pointer
        CIMCVEngine* iActiveEngine;
        
        // not owned , chat interface
         MIMCVEngineChatInterface* iActiveChatInterface;
              //owns. object of aiwservicehandler.
       CAiwServiceHandler *iAiwServiceHandler;
    
         //Keep track if Detail view is opened.
         TBool iDetailViewOpen;
         
         //Keep track, if conv. view need to be closed when xsp id is deleted
         //when doing back operation conv. view need to be closed if xsp id deleted from detail view.
         TBool iConvViewDel ; 
     

    };

#endif // _CIMCVAPPVIEW_H
