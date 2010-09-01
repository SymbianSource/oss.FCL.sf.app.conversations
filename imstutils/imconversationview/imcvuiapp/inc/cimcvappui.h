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
* Description:  application enviroment
*
*/

#ifndef CIMCVAPPUI_H
#define CIMCVAPPUI_H

#include <aknappui.h>
#include <aknViewAppUi.h>
#include "mimcvappui.h"
// Forward reference
class CIMCVAppMessageExtensionsHandler;
class CIMCVAppLayoutInfo;
class MIMCVAppLayoutInfo;
class MIMCVEngineFactory;
class CIMCVAppView;
class CIMCVAppStatusPaneHandler;
class MIMCVAppLayoutChangeObserver;
class MIMCVAppResourceChangeObserver;
class TAknLayoutScalableParameterLimits;
class TAknWindowComponentLayout;
class MIMCVUiAppSkinVariant;
class CIMCVAppInputAbsorber;
class CIMCVEngine;


/*! 
  @class CIMCVAppUi
  
  @discussion An instance of class CIMCVAppUi is the UserInterface part of the AVKON
  application framework for the conversationview example application
  */
class CIMCVAppUi :public CAknViewAppUi,
				  public MIMCVAppUi
		    {
		public:
		/*!
		  @function ConstructL
		  
		  @discussion Perform the second phase construction of a CIMCVAppUi object
		  this needs to be public due to the way the framework constructs the AppUi 
		  */
		    void ConstructL();

		/*!
		  @function CIMCVAppUi
		  
		  @discussion Perform the first phase of two phase construction.
		  This needs to be public due to the way the framework constructs the AppUi 
		  */
		    CIMCVAppUi();


		/*!
		  @function ~CIMCVAppUi
		  
		  @discussion Destroy the object and release all memory objects
		  */
		    ~CIMCVAppUi();
		 
		 /**
		* return ETrue if deletion is happening
		*/ 
		 TBool IsUnderDestruction() ;
		   
		/**
		* return the mbn full resource path
		*/ 
		TDes& MbmFullPath();
		/**
		* StorePreviousViewDetails store the previce application and view details 
		* @param aPreviousApplicationUid application uid
		* @param aPreviousViewUid ,previous active view uid
		*/
		void StorePreviousViewDetails(TInt aPreviousApplicationUid,TInt aPreviousViewUid);
		
		/**
		* switch application to background 	
		*/
		void SendMeToBackground();
		
		
		/**
		* switch application to foreground 
		* @param aAppUid application uid
		*/
		void BringToForeground( TUid aAppUid );
		
	    /**
         * constructs mif and rsc file paths based on branding, else
         * initializes with default s60 path.
         * @since S60 5.0
         */  
		void ConstructBrandL( );
		
		/**
         * reconstructs the mif and rsc's after the uninstallation of the brand.
         * initializes with default s60 path.
         * @since S60 5.0
         */
		void ReInitializeBrandL();
		
		/**
		 * Releases branding access.
		 */
		void ReleaseBrandAccess();        
       
		/**
		 * SkinVariantL
		 */
		
		MIMCVUiAppSkinVariant*	SkinVariantL();
		
		/**
		 * is the application in Foreground
		 * @param Return True - if Application in Foreground else returns False
		 */
		TBool IsAppInForeground();
		
		/*
		 * return active engine isntance
		 */
		CIMCVEngine& ActiveEngine() const ;
		
	public: 		    
	
			 /**
		 * Adds layout change observer
		 * @param aObserver Pointer to observer
		 */
         void AddLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver ) ;

		/**
		 * Removes layout observer
		 * @param aObserver Pointer to removed observer
		 */
         void RemoveLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver ) ;
        
		/**
		 * Adds resource observer
		 * @param aObserver Pointer to observer
		 */
        void AddResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver ) ;

		/**
		 * Removes resource observer
		 * @param aObserver Pointer to removed observer
		 */
         void RemoveResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver ) ;
        
	public: // from CAknAppUi
			/*
			@function HandleCommandL
			@discussion Handle user menu selections
			@param aCommand the enumerated code for the option selected
			*/
			void HandleCommandL(TInt aCommand);
			
			/*
			@function HandleWsEventL
			@param aEvent, event occured Handle user menu selections
			@param aDestination , to delivered
			*/
			void HandleWsEventL( const TWsEvent& aEvent,CCoeControl* aDestination );

			/*
			@function LayoutInfo
			@return  pointer to layout info
			*/
			MIMCVAppLayoutInfo* LayoutInfo();
			/**
			* From CEikAppui, Handles a change to the application's resources
			* which are shared across the environment
			* @param aType The type of resources that have changed
			*/
			void HandleResourceChangeL( TInt aType );
			
			/**
			* Proceses message from external application
			* Used from Phonebooks external tab (Buddylist) to start instant
			* messaging or from xmpp server to show new messages
			*
			* @since S60 v3.0
			* @param aUid       Uid somethig. Needed to get this function called
			* @param aParamData Data for processing
			*/
			void ProcessMessageL( TUid aUid, const TDesC8& aParamData );
  			/**
			* @since S60 v3.0
			* from CAknViewAppUi
			* @param aForeground   , ETrue if in Forground else Background
			*/
			 void HandleForegroundEventL( TBool aForeground );
    
	
    public : // new functions
    			
			
			/**
			* shut down and exit the application 
			*/
			void ShutDownAndCloseApplicationL();			
		
			/**
	         * Capture events if needed
	         */
	        void CaptureEventsL();

	        /**
	         * Releases events capturing
	         */
	        void ReleaseCapturingL();

	        /**
	         * Returns if events are captured
	         * @return ETrue if captured
	         */
	        TBool AreEventsCaptured() const;
	        
	     
        
	private :
		   	        
	        /**
	         * CCalculatePreviewPaneWidth  
	         */
	        void CalculatePreviewPaneWidth();
	        
	        /**
	         * SelectWindowVariety variety from window
	         * @param aNumberOfLines lines to be used
	         * @param aLimits ,limits
	         */	        
	        TInt SelectWindowVariety( const TInt aNumberOfLines, const TAknLayoutScalableParameterLimits& aLimits ) const;
        	/**
	         * RectFromLayout rect from layout
	         * @param aParent parant
	         * @param aComponentLayout layout
	         * @return rect
	         */
        	TRect RectFromLayout( const TRect& aParent,	const TAknWindowComponentLayout& aComponentLayout ) const;
        	
        	/**
             * Prepares branding access
             *
             * @since S60 5.0
             */  
			TInt PrepareBrandingAccess();

                	        
        	 /**
        	  * handle connection close and exit the servicetab also
        	  * @param aEventValue, the event value to publish
        	  */
        	 void HandleServiceCloseExitL(TInt aEventValue );
	
	private:
			
			// owned , refrence to message hadler    
		    CIMCVAppMessageExtensionsHandler* iMessageHandler;
		    
		    // onwed, layout info
			CIMCVAppLayoutInfo* iLayoutInfo;
			
			// owned, reference to imcvuiengine component
			MIMCVEngineFactory* iEngineFactory ;
			      
        	 // owned , used to set the tile, label icon in title pane
       		 CIMCVAppStatusPaneHandler* iStatusPaneHandler;
       		       		 
       		RPointerArray<MIMCVAppLayoutChangeObserver> iLayoutObservers;
        
        	// Doesn't own objects. Array of resource observers
        	RPointerArray<MIMCVAppResourceChangeObserver> iResourceObservers;
       		
       		TFileName		iChatMbmFullPath;
       		
       		// phonebook application uid
        	TInt iPbkApplicationUid;
        	// phonebook active view id
       		TInt iPbkActiveViewUid;
       		// owned
       		MIMCVUiAppSkinVariant* iSkinVariant;
       		
       		TBool iDestructingFlag;
       		
       		TBool iServerDisconnected;
       		
       		//owns the instance of the inputobserver
       		CIMCVAppInputAbsorber* iInputAbsorber;
       		//not Owns : pointer to view
       		CIMCVAppView* iCIMCVAppView;
       		
       		//is app in forground or background
       		TBool iIsAppForeground;
       		
		    };


#endif // CIMCVAPPUI_H

