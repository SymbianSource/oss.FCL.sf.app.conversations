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
* Description:  Tabbed view declaration
*
*/


#ifndef __CVIMPSTUITABBEDVIEW_H__
#define __CVIMPSTUITABBEDVIEW_H__

//  INCLUDES
#include <e32base.h>            // CBase
#include <badesca.h> 

#include <AknIconArray.h>
#include <akntoolbarobserver.h> // Toolbar

#include <downloadedcontenthandler.h>
#include <AknServerApp.h>
#include <apparc.h>
// CBase
#include <CxSPBaseView.h>
#include <mpbk2uiextensionview2.h>
#include "tvimpstenums.h"

//command observer
#include "mvimpstcmdobserver.h"
//auth event observer.
#include "mvimpstenginesearchextentioneventobserver.h"

#include <AiwCommon.h>
//  FORWARD DECLARATIONS
class CEikMenuPane;
class MVIMPSTUiTabbedViewControl;
class CPbk2UIExtensionView;
class MVIMPSTCmdHandler;
class CVIMPSTUIExtensionService;
class MVIMPSTEngine;
class MCCAParameter;
class MCCAConnection;
class MVIMPSTCmd;
class CVIMPSTUiBrandData;
class CVIMPSTUtilsWaitNote;
class CAiwServiceHandler;
class CAknToolbar; 
class MVPbkStoreContact;
class CBrowserLauncher;
class MVPbkStoreContact;
class MVIMPSTSettingsStore;
class CCVIMPSTUiStatusPaneHandler;
class CVIMPSTUIMenuExtension;
class CVIMPSTUiContactSelection;
class TAiwSingleEntrySelectionDataV2;
class TAiwMultipleEntrySelectionDataV2;

class CCoeAppUi;
// CLASS DECLARATION
/**
 * Tabbed view implementation 
 *  @lib vimpstui.lib
 *  @since S60 v5.0 
 */
 // codescanner warnings can be ignored as  CxSPBaseView derived from CBsse
 
class CVIMPSTUiTabbedView : public CxSPBaseView, // CSI: 51 #
							public MVIMPSTCmdObserver,
							public MAknToolbarObserver, // Toolbar    
						    public MVIMPSTEngineSearchExtentionEventObserver,
						    public MDownloadedContentHandler,
						    public MAknServerAppExitObserver,
						    public MAiwNotifyCallback,
						    public MPbk2UIExtensionView2

							
							  
							  
    {
    public: // Constructor
        
        enum TXmppMenuValues
                {
                EXmppMenuValuesUseExisting = 0,
                EXmppMenuValuesCreateNew = 1
                };        
        
        enum TMenuSelection
            {
            EInvitAcceptCreateNew = 0,
            EInvitAcceptUpdateExisting = 1,
            EContactCreateNew,
            EContactAddFromContacts,
            ECreateXSPCall
            };   

        /**
         * Creates a new CVIMPSTUiTabbedView.
         *
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aTabbedViewId, view i.
         * @param aServiceId, service id.
         * @param aView, reference to the view.
         * @param aId, uid
         * @param aServiceData, service info.
         * @param aEngine, reference to engine.
         * @param aCommandHandler, reference to commandhandler.
         * @return New instance of this class         
         */
        static CVIMPSTUiTabbedView* NewL( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aTabbedViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid aId,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler,
			    CVIMPSTUIMenuExtension& aMenuExtension);
  
   public: // From MDownloadedContentHandler

            /**
            * Handle downloaded content that has been saved to a file, 
            * and return ETrue if it was handled successfully.
            * @param aFileName File name with path.
            * @param aParamList Generic parameter list.
            * @param aContinue Output parameter: It must be ETrue if the caller wants 
            *                  Browser not to terminate after the content is handled.
            * @return ETrue if the implementer has processed the content. If it is 
            *                  EFalse, BrowserLauncher opens that content in an 
            *                  appropriate viewer application.
            */
            TBool HandleContentL( const TDesC& aFileName,
                                                    const CAiwGenericParamList& aParamList,
                                                    TBool& aContinue );

            /**
            * Reserved for future use.
            */
            TAny* DownloadedContentHandlerReserved1( TAny* aAnyParam );
   public: // from MApaServerAppExitObserver
       /**
       * Handle the exit of a connected server app.
       * This implementation provides Series 60 default behavior
       * for handling of the EAknCmdExit exit code. Derived classes
       * should base-call this implementation if they override this
       * function.
       * @param aReason The reason that the server application exited.
       * This will either be an error code, or the command id that caused
       * the server app to exit.
       */
       void HandleServerAppExit(TInt aReason);
       
     
    public: // From MPbk2UIExtensionView through CxSPBaseView
        
        /**
         * @see MPbk2UIExtensionView
         */
        TBool HandleCommandKeyL(
                const TKeyEvent& aKeyEvent,
                TEventCode aType );
        
        /**
         * @see MPbk2UIExtensionView
         */
		void DoDeactivate();  
		
		/**
         * @see MPbk2UIExtensionView
         */
        void HandleCommandL( TInt aCommand );
        
        /**
         * @see MPbk2UIExtensionView
         */
        void DynInitMenuPaneL( TInt aResourceId,
                               CEikMenuPane* aMenuPane );
        
        /**
         * @see MPbk2UIExtensionView
         */
        void HandleStatusPaneSizeChange();
        
        /**
         * @see MPbk2UIExtensionView
         */
        TAny* UIExtensionViewExtension( TUid aExtensionUid );
        
        /**
         * @see MPbk2UIExtensionView2
         */
         void HandleForegroundEventL(TBool aForeground);
        
        
    public: // new functions
        
         /**
         * Changes current status message
         * @param aStatus Status which message is needed.
         * @param aUpdateToNetwork ETrue update, EFalse do not update.
         * @return KErrCancel if canceled.
         */
        TInt AskStatusMessageL( TVIMPSTEnums::TOnlineStatus aStatus, 
                TBool aUpdateToNetwork );
        
        /**
         * Get status old status messages from file.
         * @param aArray. Array of messages. Don't clear before filling.
         * @param aStatus Status which message is needed.
         */
        void ReadStatusMessagesL( CDesCArray& aArray, TVIMPSTEnums::TOnlineStatus aStatus  );

        /**
         * Write status messages to file.
         * @param aArray. Array of messages to write. Newest in top. Do not write duplicates.
         * @param aStatus Status which message is needed.               
         */
        void WriteStatusMessagesL( MDesCArray& aArray, TVIMPSTEnums::TOnlineStatus aStatus );
        
        /** 
         * Loads status icons for own status.
         * @returns iconarray,ownership is transfered.
         **/
        CAknIconArray* LoadOwnStatusIconsL();
        
        /** 
         * Changes ownstatus
         **/
        void ChangeStatusProcessL();

        /** 
         * Adds a contact.
         * @param aUserId id of contact to be added.
         * @param aResourceId   resource for query.
         **/
    	void AddContactL( const TInt aResouceId, const TDesC& aUserId = KNullDesC );
    	
     	 /** 
	     * Deletes a contact.
	     **/	
    	void DeleteContactL();
    	
    	/** 
         * @retunrs currentitemindex.
         **/
        TInt CurrentItemIndex();
        
       	/** 
         * @retunrs current state.
         **/
        TVIMPSTEnums::TVIMPSTRegistrationState GetServiceState();
        
        
        
        /** 
         * Makes a internet call form send key and toolbar butotn
         * if applicable.
         **/
        void MakeVoipCallL();
        
        
        /**
         * close conversation from service tab. 
         **/
         void CloseConversationL();

          /*
         *handles the completion of create new/update existing selection dialog
         */
         void HandleContactSelectionCompleteL();
         

         
              
    public: //Toolbar
    	/**
         * From MAknToolbarObserver
         */
        void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );
        
        /**
         * From MAknToolbarObserver
         */
        void OfferToolbarEventL( TInt aCommand );
        
        /**
        * Create Fixed Toolbar
        * Since 5.0
        */
        void CreateToolBarL();
        
        /**
        * Handle Fixed Toolbar command updating
        * Since 5.0
        */
        void UpdateToolbarL();
               
       
           
        /**
         * Show or hide the toolbar
         * Since 5.0
         * @aShow if ETrue, show toolbar, otherwise, Don't show the toolbar.
         */
        void ShowHideFixedToolbarL(TBool aShow);
        
        /**
         * Function to launch help
         */
        void LaunchHelpL( const TDesC& aContext);

       	/** 
         * returns the MVIMPSTSettingsStore pointer.
         * Its created at the ConstructL of the view, so it stays till view is destructed
         * 
         * @return  MVIMPSTSettingsStore pointer : no ownership is transferred
         **/
        MVIMPSTSettingsStore* SettingsStore();
        
        /**
         * returns whether the cancel login is supported or not
         *
         * @return TBool,  ETrue if cancel login is supported else EFasle
         */
        TBool CancelLoginSupportedL();

       
	private: // Functions from MVIMPSTEngineSearchExtentionEventObserver

	   /**
        * @see MVIMPSTEngineSearchExtentionEventObserver
        */
		void HandleSearchResultL(RArray<TVIMPSTSearchData> aSerachData) ;
		
	   /**
        * @see MVIMPSTEngineSearchExtentionEventObserver
        */
		void HandleSearchFinishedL(TInt aError);
		
		
		/**
        * @see MVIMPSTEngineSearchExtentionEventObserver
        */
		void HandleSearchKeysEventL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray )  ;
		

   	private: // From CxSPBaseView
   	    
   	    /** 
         * @see CxSPBaseView
         **/ 
   		void DoActivateViewL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        
                

    private : //From MImCommandObserver
    	
        /** 
         * @see MImCommandObserver
         **/
    	void CommandFinishedL(
                const MVIMPSTCmd& aCommand );
        
    	/** 
         * @see MImCommandObserver
         **/
    	void HandleCommandEventL(TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError) ;   
    	
        
        /**
         * BlockContactL
         * @param Block if ETrue then call block else call unblock
         */
        void  BlockContactL(TBool aBlock = EFalse );
        
        /**
        * Show error note 
        * @param aError, error type
        */
        void ShowErrorNoteL(TInt aError );	

    private: // Implementation
        /**
         * Standard C++ constructor
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aTabbedViewId, view i.
         * @param aServiceId, service id.
         * @param aView, reference to the view.
         * @param aId, uid
         * @param aServiceData, service info.
         * @param aEngine, reference to engine.
         * @param aCommandHandler, reference to commandhandler.
         */
        CVIMPSTUiTabbedView( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aTabbedViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid aId,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler,
			    CVIMPSTUIMenuExtension& aMenuExtension);

        /**
         * Performs the 2nd phase of construction.
         */
        void ConstructL();

        /**
         * Standard C++ destructor.
         */
        ~CVIMPSTUiTabbedView();
        
        
        /**
         * Gets the reference to commandhandler.
         * @returns MVIMPSTCmdHandler&, reference to CommandHandlerL.
         */
        MVIMPSTCmdHandler& CommandHandlerL();  
        
        /**
	    * change own status message
	     */ 
        void ChangeOwnStatusMessageL(); 
        
		/**
		* change own avatar 
		* @param aFileName - a file name
		* @param aMimeType - a mime type
		*/ 
        void ChangeOwnAvatarL(const TDesC& aFileName, const TDesC8& aMimeType );
        
        /**
	     * search a given string
	     */ 
        void SearchContactsL() ;
          
        /**
	     * Used to launch search view
	     * @param aViewId - id of view to be launched
	     */ 
        void LaunchViewL( TInt aViewId ) ;    
   	           
        /**
         * processes friend request,when user opens friend request item
         */
        void ProcessFriendRequestL();
        /**
         * do login realted operations, tou, url 
         */
        void LoginL();
        /**
         * shows URL to register for the First time users.
         */
        void ShowURLQueryL();
        
        /**
         * opens broswer
         * @param TDesC& aUrl - url to be opened
         */
        void OpenWebBrowserL( const TDesC& aUrl );
        
        /**
         * shows poplist with Create New account, login with the existing userid.
         *
         * @since S60 5.0
         * @param aMenuChoise, will return the selected poplist item
         */
        void ShowUseExistingOrCreateNewMenuL( TInt& aMenuChoise );
        
        /**
                * shows poplist with Create New account, login with the existing userid.
                *
                * @since S60 5.0
                * @return, will return the selected poplist item
                */
        TInt ShowNewUpdateQueryL();
        
        /**
         * shows user name and password query dialog
         *
         * @since S60 5.0
         * @param aUserName, user name
         * @param aPassword, password
         */
         void ShowLoginDialogL( TDes& aUserName, TDes& aPassword );

        /** 
         * Adds a phonebook contact.
         **/
        void LaunchPhoneBookAddContactL();
        
        /** 
         * launches selection dialog.
         **/
        void LaunchPbkSelectionDlgL();
        
        /** 
         * Adds all selected contact in aContacts.
         **/
        void AddContactL(RPointerArray<HBufC> aContacts);
        
        
        /** 
         * copies selected contact to  phone book.
         **/
        void CopyToPhoneBookL(); 

         /**
          * shows terms of use and logs in  if userid and password is provided.
          *
          * @since S60 5.0
          * @return TInt, error if any.
          */
         TInt ShowTermsofuseL();
         
                 
         
         /**
          * returns whether the Change Connection is supported or not
          *
          * @return TBool,  ETrue if Change Connection is supported else EFasle
          */
         TBool ChangeConnectionSupportedL();
         
         /*
         *Shows the list of blocked contacts.
         */
         void ShowBlockedContactListL();
         
         /*
          *Launches the setting view
         */
         void LaunchSettingsViewL();
         
          /*
          *helper method for add or link to contacts
         */
         void CreateOrLinkToPhoneBookL(TInt aCommand );
         
          /*
          *Launches the setting view
         */
         void EditContactL();
         /*
          *return true if URL query to be shown at login time,
          * other wise false.
          */
         TBool IsURLQueryToBeShownL();
         
         
          /*
        * helper method for executing the call
        */
        void ExecuteAiwCallL( const TDesC& aSelectedAddress );
        
         /*
        * helper method for checking and setting if user id is changed 
        */
        void ReadAndSetOwnUserIdL();
        /*
        * launch the phonebook dialog for add from contacts depending on 
        * whether it is localstore or not 
        */
       void LaunchFetchDialogL();
       
				/*
				* Launch single contact selection dialog in case of local contacts for 
				* add from phonebook
				*/       
        void ExecuteSingleItemFetchL( TAiwSingleEntrySelectionDataV2 aData );
        
       /*
				* Launch multiple contact selection dialog in case of server contacts for 
				* add from phonebook
				*/ 
        void ExecuteMultipleItemFetchL( TAiwMultipleEntrySelectionDataV2 aData );
        
        /*
        * Check and Create the servicehandler if it is NULL
        */      
         void ReInitializeServiceHandlerL();
         
    public://from MAiwNotifyCallback.
        TInt HandleNotifyL(
                    TInt aCmdId,
                    TInt aEventId,
                    CAiwGenericParamList& aEventParamList,
                    const CAiwGenericParamList& aInParamList);
         
    private:
        enum TAssignmentState
            {
            EInvalid = 0,
            EXSPAssign,
            EDefaultAssign
            };

	private: // Data   
                     
        // Stores view id
        TUint iViewId;  
        
        //service id
        TInt iServiceId;     
        
        //Doesnt own - reference to engine for this service
        MVIMPSTEngine& iEngine;
        
        //doesnt own
		CVIMPSTUiBrandData* iBrandHandler;
       
        // Own: This view's control container
        MVIMPSTUiTabbedViewControl* iContainer;             

        // previous state.
        TVIMPSTEnums::TVIMPSTRegistrationState iPreviousState;
                        
        //CCA Param - Owns
        MCCAParameter* iParameter;
       
		// Own: to connect CCA 
        //it is member variable because it is needed during the HideInBackground event
		MCCAConnection* iConnection;
		 
        //Owns. status message.
        HBufC* iStatusMessage;
        
		//owns. object of aiwservicehandler.
		CAiwServiceHandler *iServiceHandler;
		
       	// not owned, reference to extension service
       	CVIMPSTUIExtensionService& iExtensionService;
       	
       	//Owns command Handler for this view
        MVIMPSTCmdHandler& iCommandHandler;    
        
        //doew not own, reference to menu extension
        CVIMPSTUIMenuExtension& iMenuExtension;
              
       	// owned, wait note.
        CVIMPSTUtilsWaitNote* iWaitNote;
        
        //owned. Pointer to toolbar object.
        CAknToolbar*       	 		iFixedToolbar;
            
        
        //owns. object of MVPbkStoreContact
        MVPbkStoreContact* iContact;
        //Owns: Stores the focused contact ID from iContainer before it gets destroyed
        HBufC* iFocussedIDHistory;
        //handle to customize status pane
        CCVIMPSTUiStatusPaneHandler* iStatusPaneHandler;

        //Owns Browser launcher
        CBrowserLauncher* iBrLauncher;
        
        //Owns, username
        HBufC* iUserId;
     
		// owns, retrive settings from cenrep store
        MVIMPSTSettingsStore* iSettingsStore;
        //in param list for aiw generic options, owns
        CAiwGenericParamList* iAiwParamList;
        // selected data
        TVPbkSelectedData iSelectedData;
        // Friend request accept,method index
        TInt iSelectedAction;
       // Owns : selected friendrequest id 
        HBufC* iFriendRequestId;
        // owns : contact selection 
        CVIMPSTUiContactSelection* iContactSelection;
        //to know whether to hide the add contact button on the toolbar
        TBool iAddContact;
        // owns : selected result from aiw call query
        HBufC* iQueryResult;
        
        //iIsToolBarHidden, if ETrue: tool bar is already hidded 
        // no need to call again hidetoolbar.
        TBool iIsToolBarHidden;
        // NOT OWNS
        CCoeAppUi* iAppUi;
        
        //Keep track, if iServiceHandler has been reset or not.
        TBool iIsServiceHandleReSet;
        
        TBool iContainerCreationStarted;
        
    };

#endif // __CVIMPSTUITABBEDVIEW_H__

// End of File
