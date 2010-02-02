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
*  Description: Block view declaration
 *
*/


#ifndef CVIMPSTUIBLOCKVIEW_H
#define CVIMPSTUIBLOCKVIEW_H


//  INCLUDES
#include <e32base.h>            // CBase

#include "CxSPBaseView.h"
#include "mvimpstcmdobserver.h"
#include "mvimpstengineblockedlistfetcheventobserver.h"

// FORWARD DECLARATIONS
class CVIMPSTUiBlockViewControl;
class CVIMPSTUIExtensionService;
class MVIMPSTEngine;
class CAknNavigationDecorator;
class MVIMPSTEnginePresenceSubService;
class MVIMPSTCmdHandler;

// CLASS DECLARATION
/**
 * Search view implementation 
 *  @lib vimpstui.lib
 *  @since S60 v5.0 
 */
 // codescanner warnings can be ignored as  CxSPBaseView derived from CBsse
 
class CVIMPSTUiBlockView :public CxSPBaseView,
						  public MVIMPSTCmdObserver,
						  public MVIMPSTEngineBlockedListFetchEventObserver
    {
    public:  // Constructors and destructor
    
         
    	 /**
         * Creates a new CVIMPSTUiSearchView.
         *
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aBlockViewId  Block view id
         * @param aView Reference to Phonebook2-created view instance
         * @param aServiceData Reference to ui extension service
         * @param aEngine Reference to engine object
         * @return New instance of this class         
         */
        static CVIMPSTUiBlockView* NewL( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aBlockViewId,
			    CPbk2UIExtensionView& aView,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler );
     
        
	 
    public: // From MPbk2UIExtensionView through CxSPBaseView
        
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
        
        //From MVIMPSTCmdObserver
        
        /**
        *@see MVIMPSTCmdObserver
        */
        void CommandFinishedL( const MVIMPSTCmd& aCommand );

        /**
        *@see MVIMPSTCmdObserver
        */
        void HandleCommandEventL( TVIMPSTEnums::TVIMPSTRegistrationState aState, 
        					TInt aServiceError) ;
        					
        //from MVIMPSTEngineBlockedListFetchEventObserver.
        /**
        *@see MVIMPSTEngineBlockedListFetchEventObserver
        */
		void HandleBlockedListFetchCompleteL() ;
	private: // From CxSPBaseView
   	    
	    /**
         * @see CxSPBaseView
         */
   		void DoActivateViewL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
      
    
        /**
		 * This function updates text to navi pane
         */
		void UpdateNaviPaneTextL( );
		
		void FetchBlockedListFromServerL();
  
    private: 

        /**
         * Standard C++ constructor
         * @param aMapper Reference to view mapper object
         * @param aContactManager Reference to contact manager object
         * @param aBlockViewId  Block view id
         * @param aView Reference to Phonebook2-created view instance
         * @param aServiceData Reference to ui extension service
         * @param aEngine Reference to engine object
         */
        CVIMPSTUiBlockView( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aBlockViewId,
			    CPbk2UIExtensionView& aView,
			    CVIMPSTUIExtensionService& aServiceData ,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler );

        /**
         * Performs the 2nd phase of construction.
         */
        void ConstructL();

        /**
         * Standard C++ destructor.
         */
        ~CVIMPSTUiBlockView();
        
	private:    // Data

	    // Own: This view's control container
        CVIMPSTUiBlockViewControl* iContainer;             
               
        // Stores view id
        TUint iBlockViewId;     
        
        //Doesnt own - reference to engine for this service
        MVIMPSTEngine& iEngine;
                      
         // Own: Used for navigation pane text
        CAknNavigationDecorator* iNaviDecorator;    
        
        //doesnt own, refernec to extension service
        CVIMPSTUIExtensionService& iExtensionService;  
        
		//Not owned; pointer to instance of prsece sub service        
        MVIMPSTEnginePresenceSubService* iPresenceSubService;
      
        //Not owned; blocked list array pointer
        RPointerArray<HBufC>* iBlockedList;
        
        //Doesnt own - reference to Command handler for this service
        MVIMPSTCmdHandler& iCommandHandler;
	};

#endif      // CVIMPSTUIBLOCKVIEW_H   
            
// End of File
