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
* Description:  Search view control.
*
*/


#ifndef     __CVIMPSTUISEARCHVIEWCONTROL_H
#define     __CVIMPSTUISEARCHVIEWCONTROL_H

//  INCLUDES
#include <bldvariant.hrh>
#include <eiklbo.h>
#include <coecntrl.h>
#include <eikbtgpc.h>
#include "tvimpstenums.h"

// FORWARD DECLARATIONS
class CAknDoubleStyleListBox;
class MPbk2KeyEventHandler;
class MVIMPSTEngine;
class MVIMPSTEngineSearchExtentionEventObserver;
class MVIMPSTCmdHandler;
class CVIMPSTUiSearchView;



// CLASS DECLARATION

/**
 * Search view control. Shows search results
 *  @lib vimpstui.lib
 *  @since S60 v5.0 
 */
class CVIMPSTUiSearchViewControl :  public CCoeControl,
                                	public MEikListBoxObserver,
									public MCoeControlObserver
    {
           
    public: // Constructor and destructor
        /**
         * Creates a new CVIMPSTUiSearchViewControl.
         * @param aKeyEventHandler Key event handler to be used
         * @param aServiceId service id
         * @param aEngine reference to engine.
         * @param aCommandHandler reference to commandhandler
         * @param aSearchView reference to search view.
         */
        static CVIMPSTUiSearchViewControl* NewL(MPbk2KeyEventHandler* aKeyEventHandler, 
        										TUint32 aServiceId,
        										MVIMPSTEngine& aEngine,
        										MVIMPSTCmdHandler& aCommandHandler,
        										CVIMPSTUiSearchView& aSearchView );
        
        /**
         * Creates a new CVIMPSTUiSearchViewControl and leaves it to
         * cleanup stack.
         * @see NewL
         */
        static CVIMPSTUiSearchViewControl* NewLC( MPbk2KeyEventHandler* aKeyEventHandler,
        											TUint32 aServiceId,
        											MVIMPSTEngine& aEngine,
        											MVIMPSTCmdHandler& aCommandHandler,
        											CVIMPSTUiSearchView& aSearchView );
        
        /**
         * Destructor.
         */
        ~CVIMPSTUiSearchViewControl();

	
    private: 
    
        /**
         * Standard C++ constructor
         * @param aKeyEventHandler Key event handler to be used
         * @param aServiceId service id
         * @param aEngine reference to engine
         * @param aCommandHandler reference to commandhandler
         * @param aSearchView reference to search view.
         * 
         */    
        CVIMPSTUiSearchViewControl(	MPbk2KeyEventHandler* aKeyEventHandler,
        							TUint32 aServiceId,
        							MVIMPSTEngine& aEngine,
        							MVIMPSTCmdHandler& aCommandHandler,
        							CVIMPSTUiSearchView& aSearchView );
        
        /**
         * Performs the 2nd phase of construction.
         *
         */        
        void ConstructL();

    public: // New functions
    
	   /**
        * sets the primary and secondary text of this view
        */
       void SetEmptyTextsToListboxL();
       
       /**
        * sets the searching text text of this view
        */
       void SetSearchingTextToListboxL();
       
      /**
       * Updates the list box with search data
       * @param aSerachData - search data for listbox to be updated with
       */
       void UpdateListBoxL(RArray<TVIMPSTSearchData> aSerachData);
      
      
       /**
        * returns the list box
        */
		CEikListBox* ListBox() const;

       /**
        * extracts the contactId ,allocates memory for contactId and returns
        * Owership of the contactId is transfered to caller 
        * 
        * @return contactId  : Owenership to caller
        */    
        HBufC* GetFocusedContactLC() const;

		/**
        * Clears list box contents. I.e. item array.
        */
        void ClearListBoxContents();
        
       	/**
        * Starts the conversation with the selected contact
        * @param aContact : id of the contact 
        */
        void OpenConversationL(const TDesC& aContactId );	    
    

	    /**
        * SearchResultL
        * @param aSerachData, search data 
        */
		void SearchResultL(RArray<TVIMPSTSearchData> aSerachData) ;
		
	   /**
        * SearchFinishedL
        * @param aError, error any
        */
		void SearchFinishedL(TInt aError);
		
	    /**
         * Updates Softkeys according to current focus
         * @param aUseDefaultCba Flag to force the default sofkeys for view.
         * Default value is false.
         */                     
        void UpdateCbaL( TBool aUseDefaultCba = EFalse );
               
   
    private: // Functions MCoeControlObserver
    
    	/**
	    * Handles events from findpane and forwards them to listbox filter.
	    * @see MCoeControlObserver
	    */
	    void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );  

        /**
        * From MEikListBoxObserver, Handles event's generated by listbox
        * @param aListBox Pointer to listbox from where this event originated
        * @param aEventType Type of event received.
        */
		void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );

        
        /**
        * From CoeControl, Returns the number of control contained by this class.
        * @return Number of controls contained
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl, Returns handle to control pointed by aIndex
        * @param aIndex Wanted control's index [0..n]
        * @return Handle to wanted control
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl, Handles key-events
        * @param aEvent Event that occured
		* @param aType Type of key-event (EEventKey, EEventKeyUp or EEventKeyDown)
        * @return Containers response to event (EKeyWasNotConsumed/ EKeyWasConsumed)
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aEvent, TEventCode aType );
		
		/**
        * From CCoeControl,respond to size chnaged
        */
		void SizeChanged();
		
     	
	   	
    private:    // Data
    
    	// Not owned: key event handler
    	MPbk2KeyEventHandler* iKeyEventHandler;
    	
        //Service Id 
		TUint32 iServiceId ;
		
	    //Owns
		CAknDoubleStyleListBox* iListBox;

      	//Owns
		CDesCArray* iItemArray;
     	
        //Doesnt own - reference to engine for this service
        MVIMPSTEngine& iEngine;
        
        //Doesnt own - reference to Command handler for this service
        MVIMPSTCmdHandler& iCommandHandler;
        
        //doesn't own - reference to Searh View
        CVIMPSTUiSearchView& iSearchView;
        
        // CBA. not owned
        CEikButtonGroupContainer* iCba;     

        // Storage for previous focus
        TVIMPSTEnums::TItem iLastFocusedItemType;
		
		//indicate command to be executed on enter key event.	
		TInt iCurrentCmdToExe;
             
	};

#endif      // __CVIMPSTUISEARCHVIEWCONTROL_H

// End of File
