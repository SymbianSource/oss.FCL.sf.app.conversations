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
* Description:  Search view declaration
*
*/



#ifndef CVIMPSTUISEARCHVIEW_H
#define CVIMPSTUISEARCHVIEW_H


//  INCLUDES
#include <e32base.h>            // CBase
#include <badesca.h>
#include <AknIconArray.h>

#include "CxSPBaseView.h"
#include "tvimpstenums.h"
#include "mvimpstcmdobserver.h"
#include "mvimpstenginesearchextentioneventobserver.h"

// FORWARD DECLARATIONS
class CVIMPSTUiSearchViewControl;
class CVIMPSTUIExtensionService;
class MVIMPSTEngine;
class MVIMPSTCmdHandler;
class CAknNavigationDecorator;
class CVIMPSTUtilsWaitNote;
class CAsyncCallBack;


// CLASS DECLARATION
/**
 * Search view implementation 
 *  @lib vimpstui.lib
 *  @since S60 v5.0 
 */
 // codescanner warnings can be ignored as  CxSPBaseView derived from CBsse
 
class CVIMPSTUiSearchView :public CxSPBaseView,
						   public MVIMPSTCmdObserver,
						   public MVIMPSTEngineSearchExtentionEventObserver
    {
    public:  // Constructors and destructor
    
         
    	 /**
         * Creates a new CVIMPSTUiSearchView.
         *
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aSearchViewId  search view id
         * @param aServiceId service id
         * @param aView Reference to Phonebook2-created view instance
         * @param aId native view id
         * @param aServiceData Reference to ui extension service
         * @param aEngine Reference to engine object
         * @param aCommandHandler Reference to commnad handler object
         * @return New instance of this class         
         */
        static CVIMPSTUiSearchView* NewL( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aSearchViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid aId,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler );
     
        
	
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
         * Function to launch help
         */
        void LaunchHelpL( const TDesC& aContext);   
        
        /**
         *  Search CallBack function
         *  This is used to call the search functionality when its called form the 
         *   Tabbed view
         * 
         *  @param aInstance : Search view pointer is passed in our case
         *  @return Error to  call back function
         */
        static TInt  SearchCallbackL(TAny* aInstance);
	
	private: // From CxSPBaseView
   	    
	    /**
         * @see CxSPBaseView
         */
   		void DoActivateViewL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
      
    
    private : //From MImCommandObserver
    	
        /**
         * @see MImCommandObserver
         */
    	void CommandFinishedL(
                const MVIMPSTCmd& aCommand );
        
    	 /**
          * @see MImCommandObserver
          */
    	void HandleCommandEventL(TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError) ;  
    	  
    private: 
    
    	/**
	     * Used to search for contacts matched against entered string 
	     */ 
        void SearchContactsL();	
        
        
        /**
		 * This function updates text to navi pane
         */
		void UpdateNaviPaneTextL( );
  
    		
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
		
		
    private: 

        /**
         * Standard C++ constructor
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aSearchViewId  search view id
         * @param aServiceId service id
         * @param aView Reference to Phonebook2-created view instance
         * @param aId native view id
         * @param aServiceData Reference to ui extension service
         * @param aEngine Reference to engine object
         * @param aCommandHandler Reference to commnad handler object
         */
        CVIMPSTUiSearchView( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aSearchViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid aId,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler );

        /**
         * Performs the 2nd phase of construction.
         */
        void ConstructL();

        /**
         * Standard C++ destructor.
         */
        ~CVIMPSTUiSearchView();
        
    public:
    
        /**
         * returns ETrue if the contact is already in the contact list
         * else  returns EFalse 
         * Based on this option "add to contacts " is shown
         *
         * @return whether the contact is already in the contact list or not
         */
        TBool HideAddToContactsL() const;
        
        
	private:    // Data

	    // Own: This view's control container
        CVIMPSTUiSearchViewControl* iContainer;             
               
        // Stores view id
        TUint iSearchViewId;     
        
         //service id
        TUint32 iServiceId;                  
        
        //Doesnt own - reference to engine for this service
        MVIMPSTEngine& iEngine;
                      
         // Own: Used for navigation pane text
        CAknNavigationDecorator* iNaviDecorator;    
        
        //doesnt own, refernec to extension service
        CVIMPSTUIExtensionService& iExtensionService;  
        
        //Owns command Handler for this view
        MVIMPSTCmdHandler& iCommandHandler;  
        
        // owned
        CVIMPSTUtilsWaitNote* iWaitNote;
        
        // this flag is to check  whether the search id initiated 
        // from the search view or not
        // At the first time its EFalse since search is always initiated from tabview
        TBool iSearchInitiated;
        
        //flag to check if End Key was pressed
        TBool iEndKeyPressed;
        /**
         * Active object for async calls.
         */
        CAsyncCallBack* iCallBack;
       
	};

#endif      // CVIMPSTUISEARCHVIEW_H   
            
// End of File
