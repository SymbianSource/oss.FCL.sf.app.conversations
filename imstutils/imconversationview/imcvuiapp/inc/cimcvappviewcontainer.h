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
* Description:  main view container class
*
*/



#ifndef _CIMCVAPPVIEWCONTAINER_H
#define _CIMCVAPPVIEWCONTAINER_H

//  INCLUDES
#include "cimcvapprichtextcontainercache.h"
#include "mimcvapplayoutchangeobserver.h"
#include "mimcvtapeventobserver.h"
#include "cimcvappmessageeditor.h"

#include <badesca.h> //For the CDesCArray
#include <AknWaitDialog.h>
#include "cimcvengine.h"
// FORWARD DECLARATIONS
class CIMCVAppMessageExtensionsHandler;
class CIMCVAppViewControl;
class MIMCVEngineMessageReadInterface;
class MIMCVEngineMessageWriteInterface;
class MIMCVEngineMessage;
class CIMCVAppUi;
class MIMCVUiAppSkinVariant;  
class MIMCVRecipientObserver;
class CIMCVAppLoginEventListener;
class CIMCVAppView;
class CIMCVAppMessageEditor;
class MIMCVEngineFactory;

// CLASS DECLARATION

/**
*  This is container class for conversation view
*/
class CIMCVAppViewContainer : public CCoeControl,
								  public MEikEdwinSizeObserver,
								  public MIMCVAppLayoutChangeObserver,
								  public MCoeControlObserver,
								  public MIMCVAppMessageEditorObserver,
								   public MIMCVTapEventObserver
    {
    public: // Enumerations
    
        /** 
         * Observed controls 
         */
        enum TTouchableControls
            {
            EMessageList = 1,
            EMessageEditor
            };

    public:  // Constructors and destructor

        /**
        * Symbian OS default constructor.
        * @param aRect Frame rectangle for container.
        * @param aEngine, refence to engine
        * @param aReadInterface to read the messages
		* @param aWriteInterface for writing the messages
		* @param aParent Context access mechanism.
		* @param aMessageHandler Handling messages
		* @param aCba Pointer to cba of view.
		* @param aRecipientId, recipient user id
		* @param aBrandingAccess, brand accessor
	    */
		static CIMCVAppViewContainer* NewL( const TRect& aRect,
			CIMCVEngine* aEngine,
			MIMCVEngineMessageReadInterface* aReadInterface,
		    MIMCVEngineMessageWriteInterface* aWriteInterface,
		    CIMCVAppView* aParent,
            CIMCVAppMessageExtensionsHandler& aMessageHandler,
			CEikButtonGroupContainer* aCba,					
			const TDesC& aRecipientId,			
			MIMCVRecipientObserver* aRecipientObserver,
            CIMCVAppRichTextContainerCache* aRTContainerCache,
            CGulIcon* aFromMe, CGulIcon* aToMe);

	
	    /**
        * Destructor.
        */
        virtual ~CIMCVAppViewContainer();


	private:

        /**
        * Symbian OS default constructor.
        * @param aRect Frame rectangle for container.
		* @param aParent Context access mechanism.
        */
        void ConstructL( const TRect& aRect, MObjectProvider* aParent,
                CGulIcon* aFromMe, CGulIcon* aToMe ,
                const TDesC& aRecipientId );


    public: // New functions
        const TRect TextRect() const;        
        
        
        /**
        * Switches the Containers ReadInterface & WriteInterface
        * @param aReadInterface to read the messages
		* @param aWriteInterface for writing the messages
		*/
        void SwitchViewL( MIMCVEngineMessageReadInterface* aReadInterface,
        						MIMCVEngineMessageWriteInterface* aWriteInterface,
        						const TDesC& aRecipientId );        
        
        /*
         * assigned new engine instance
         */
        void SetServiceParametersL(CIMCVEngine* aNewEngine ) ; 
        /**
         * Returns buffer containing the currently selected text
         * (or empty buffer if nothing selected)
         */
        HBufC* SelectedItemL();
        
        /**
         * @return Type of selected item
         */
        TInt SelectedItemType();

       
		/**
        * This method starts scrolling
		* @since 5.0
        */
        void StartScrollingL();

		/**
		 * This method stops scrolling
		 * @since 5.0
		 */
		void StopScrollingL();

		void StartConversationL();

			
		/**
        * This closes the active conversation 
		* @since 5.0
        */
		TInt CloseConversationL(const TDesC& aBuddyId );
		
		/**
        * This will deactivate the active conversation 
		* @since 5.0
        */
		TInt DeactivateConversationL();
		
		/**
        * This method handles message sending
		* @since 5.0
        */
        void SendMessageL();
        
        /**
        * This method insert a message about the presence status changes
		* @since 5.0
        */
        void InsertStatusChangedMessageL(CIMCVEngine& aEngine, TInt aServiceId,const TDesC& aBuddyId );
        
		/**
		 * Return reference of CIMCVAppMessageEditor object.
		 */
        CIMCVAppMessageEditor& Editor()	const ;
        
        /**
        * This checks if scroll mode is on or off
		* @since 3.2
        * @return Returns if scrolling is stopped or not (TBool)
        */
        TBool IsStopped() const;
        
        /**
         * Sets item highlighting on or off (automatic find)
         * @param aHihghlight ETrue to set finder on
         */
        void SetItemHighlightL( TBool aHighlight );
        
        
        /**
         * Should we fetch new messages or not
         * @param aFetch ETrue if this chat is in foreground and should receive 
         *               messages, EFalse otherwise.
         */
        void FetchMessages( TBool aFetch );
        
      	      
         /**
         * Method to force refresh in the rich text editor,
         * called from view when adding a smiley.
         * @since 3.0
         */
        void RefreshEditorL();

         /**
         * Loads fome/tome icons 
         * @param aId, Id of the element required by the brnading server
         * aBitmapId, id of the icon
         * aMaskId of the icon
         */
        CGulIcon* LoadBitmapL(const TDesC8& aId, const TInt aBitmapId,const TInt aMaskId );
        
		/**
		* IsLoggedIn
		* @return ETrue if logged in else EFalse
		* @since 5.0
		*/                             
		TBool IsLoggedIn() const;
		
		        
        /**
         * Update the CBA labels
         */
        void UpdateCbaL();
        
        /*
         * Saves the message typed in the message editor of 
         * container
         */
        void StoreEditorMessageL();
		void SmileyDialogLaunched (TBool aLaunched);
		void UpdateChatViewL ();
	                                  
    private: 
        
        /**
         * This function does basic clearing after the message is sent
         */
        void ResetAfterSendL();
        
		/**
		 * This function displays information notes
		 * @param aResourceId Resource ID of note
		 */
		void DisplayInfoNoteL( const TInt aResourceId );

        void ResizeIcons( TRect aParentRect ) ;
        
        
	private:
	
		void MapFromBitmapIdToAknsItemId( TInt aBitmapId, 
                                TAknsItemID& aItemId, TInt& aColorGroupId );
                                	
   		//does not Owns. "From me" icon
		CGulIcon* iFromMe;
		
		//does n0t Owns. "To me" icon
		CGulIcon* iToMe;
		

    private: // Functions from base classes    

		/**
		 * From MIMCVAppLayoutChangeObserver, for layout change observing
		 */
		void LayoutChangedL( TInt aType );
        
		
		/**
         * From MEikEdwinSizeObserver, This method handles edwin's size change
         * @param aEdwin The edwin for which the size event is being handled
		 * @param aEventType The event type
		 * @param aDesirableEdwinSize The desired size for the edwin identified 
		 *                            by aEdwin
         * @return The return value depends on your implementation. 
         *         Return ETrue if you wish to redraw the edwin. 
         *         EFalse otherwise
         */
        TBool HandleEdwinSizeEventL( CEikEdwin* aEdwin,
			TEdwinSizeEvent aEventType, TSize aDesirableEdwinSize );
		
  
        
    private: // From MCoeControlObserver

		/**
         * @see MCoeControlObserver
         */
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );

	private: // Functions CCoeControl

        /**
        * From CoeControl, Handles "size changed"-events..
        */
        void SizeChanged();

        /**
        * From CoeControl, 
        * Returns the number of control contained by this class.
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
        * From CCoeControl, Gets called when drawing is needed (by framework)
        * @param aRect Rect that needs to be drawed
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CCoeControl, Handles key-events
        * @param aKeyEvent Event that occured
		* @param aType Type of key-event 
		*              (EEventKey, EEventKeyUp or EEventKeyDown)
        * @return Containers response to event 
        *                    (EKeyWasNotConsumed/ EKeyWasConsumed)
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
		                             TEventCode aType );

		
	private:    // From MIMCVAppMessageEditorObserver
	
	    /**
         * Handle editor event.
         */
        void HandleMessageEditorEventL( TEditorEvent aEvent );
        	
    private:    // new functions
    		/**
        * Constructor
        * @param aEngine, refence to engine
        * @param aReadInterface to read the messages
		* @param aWriteInterface for writing the messages
		* @param aMessageHandler Handling messages
		* @param aCba Pointer to cba of view.
		* @param aRecipientId, recipient user id
	    */
		CIMCVAppViewContainer( 
			CIMCVEngine* aEngine,
		    MIMCVEngineMessageReadInterface* aReadInterface,
		    MIMCVEngineMessageWriteInterface* aWriteInterface,
		    CIMCVAppMessageExtensionsHandler& aMessageHandler,
			CEikButtonGroupContainer* aCba,
			MIMCVRecipientObserver* aRecipientObserver,
			CIMCVAppView& aAppView,
			CIMCVAppRichTextContainerCache* aRTContainerCache
			);
			
    private: // From MIMCVTapEventObserver

		/**
         * @see MIMCVTapEventObserver
         */
        void HandleTapEventL( TTapEvent aEvent, TUint aControlId );
            
      
    private:    // Data
        // not owned, reference to uiengine
        CIMCVEngine* iActiveEngine ;
        
        // not owned., reference to appui	
        CIMCVAppUi* iAppUi;
        // Owns. Chat view control
        CIMCVAppViewControl* iChatView;
        // not owned, message read interface
        MIMCVEngineMessageReadInterface* iReadInterface;
        // not owned, message witre interface
		MIMCVEngineMessageWriteInterface* iWriteInterface;
       	

		// owned, outer line
		TAknLayoutRect iOutLine;
		// owned, horizontal  line
		TAknLayoutRect iHoriline;
		// owned, vertical line
		TAknLayoutRect iVertLine;
			

		// not owned, message extention handler
        CIMCVAppMessageExtensionsHandler& iMessageExtensionsHandler;
        
               
        // Does not own. Pointer to cba of view.
        CEikButtonGroupContainer* iCba;
        
              
        //holds the reference to the contact to which 
        //conversation has been established.
        HBufC* iRecipientId;
              
        // owned, message content
        HBufC* iEditorMessage;
        
        // not owned
        MIMCVUiAppSkinVariant* iSkinVaiant;       
      
        
        //Doesn't own. Handle to recipient observer
		MIMCVRecipientObserver* iRecipientObserver;  
        
        //Reference to the AppView object
       	CIMCVAppView& iAppView;
       	//owns. meesage editor
       	CIMCVAppMessageEditor* iMsgEditor;
       	//Does nt Owns. Cache to store CIMCVAppRichTextContainer objects
       	CIMCVAppRichTextContainerCache* iRTContainerCache;
    };

#endif      // _CIMCVAPPVIEWCONTAINER_H

// End of File
