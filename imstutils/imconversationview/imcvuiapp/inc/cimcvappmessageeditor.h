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
* Description:  message editer class
*
*/

#ifndef __CIMCVAPPMESSAGEEDITOR__H
#define __CIMCVAPPMESSAGEEDITOR__H


// INCLUDE FILES
#include <coecntrl.h>
#include <eikedwin.h>
#include <eikrted.h>
#include <eikedwin.h>
#include <AknUtils.h>

// FORWARD DECLARATION
class CAknInputFrame;
class MIMCVTapEventObserver;
class CIMCVAppView;
class CIMCVAppMessageExtensionsHandler;
/**
 * Helper class to get editor content events.
 * @since S60 v5.0
 */
class MIMCVAppMessageEditorObserver
    {
    public: // Enumerations
        enum TEditorEvent
            {
            EEditorContentCleared = 0,
            EEditorContentChanged
            };
            
    public: // New methods
        
        /**
         * Handle editor event.
         */
        virtual void HandleMessageEditorEventL( TEditorEvent aEvent ) = 0;
        
    protected:
        
        /**
         * Destructor for protection.
         */
        virtual ~MIMCVAppMessageEditorObserver() {};
    };

/**
* Message Editor for meco application. 
* It looks just like a search field.
* 
* @imcvappui.exe
* @since Series 60 1.2
*/
class CIMCVAppMessageEditor:
        public CCoeControl, 
        public MCoeControlObserver,
        public MEikEdwinObserver
    {
    public:
        /**
        * Two-phased constructor. Leaves on failure.
        * @since Series60 1.2
        * @param aParent Parent control        
        * @return The constructed Goto Pane control
        */
        static CIMCVAppMessageEditor* NewL
              ( 
              const CCoeControl* aParent, 
              TDesC& mbmPath,
              CIMCVAppView& aAppView,
              CIMCVAppMessageExtensionsHandler& aExtensionHandler
              );

        /**
        * Destructor.
        */
        virtual ~CIMCVAppMessageEditor(); 

        /**
        * See description in calslbs.h.
        */
        void HandleFindSizeChanged();       
        
        
    public:    // from MCoeControlObserver

        /**
         * Handle control event.
         * @since Series60 1.2
         * @param aControl The control
         * @param aEventType Event type
         */
        void HandleControlEventL
        ( CCoeControl* aControl, TCoeEvent aEventType );
        /**
         * Catches EEventTextUpdate event.
         * @since Series60 1.2
         * @param aKeyEvent
         * @param aType
         * @return
         */
        virtual void HandleEdwinEventL(CEikEdwin* aEdwin,TEdwinEvent aEventType);

    public: // from CCoeControl

        /**
        * Handle key event.
        * @since Series60 1.2
        * @param aKeyEvent The key event
        * @param aType Key event type
        * @return Response (was the key event consumed?)
        */
        virtual TKeyResponse OfferKeyEventL
            ( const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * HandlePointerEventL
        * From CCoeControl
        *
        */
        void HandlePointerEventL(const TPointerEvent& /*aPointerEvent*/);
        
    private:    // From MObjectProvider
        
            /**
             * @see MObjectProvider
             */
           TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

    public:     // from CCoeControl

        /**
        * Count component controls.
        * @since Series60 1.2
        * @return Number of component controls
        */
        virtual TInt CountComponentControls() const;

        /**
        * Get a component control by index.
        * @since Series60 1.2
        * @param aIndex Index of component control to be returned
        * @return Component control or NULL
        */
        virtual CCoeControl* ComponentControl( TInt aIndex ) const;
      

    public:     // Text manipulation

        /**
        * Lengt of the text in the editor.
        * @since Series60 1.2
        * @return The text length
        */
        TInt TextLength() const;
    

        /**
        * Set text.
        * @since Series60 1.2
        * @param aTxt Text to set
        * appended if needed.
        */
        virtual void SetTextL( const TDesC& aTxt ); 

        /**
        * Returns editor control of goto pane.
        * @since Series60 1.2
        * @return editor control of goto pane
        */
        CEikRichTextEditor& Editor() const;
        
        /**
         * Sets message editor observer.
         * @since S60 v3.2
         * @param aObserver Pointer to observer, ownership not transfered.
         */
        void SetMessageEditorObserver( MIMCVAppMessageEditorObserver* aObserver );
        /**
         * This method is called when observed object is tapped
         * @since S60 v5.0
         * @param aEvent Event which happened in touchable control
         * @param aControlId in which control the event occured
         * @return none
         */
        void SetTapObserver( MIMCVTapEventObserver* aObserver, TUint aId );       
		

    private:  // Construct / destruct
        /**
        * Constructor.
        * @param 
        */
        CIMCVAppMessageEditor(CIMCVAppView& aAppView,
                CIMCVAppMessageExtensionsHandler& aExtensionHandler);

        /**
        * Second-phase constructor. Leaves on failure.
        * @param aParent Parent control
        */
        void ConstructL( const CCoeControl* aParent, TDesC& mbmPath );
        
    private:
        
        /**
         * Notifies message editor observer when needed.
         * @since S60 v5.0
         * @param aOldLen Editor content length before editing.
         * @param aCurLen Editor content current length
         */
        void NotifyObserverIfNeededL( TInt aOldLen, TInt aCurLen ) const;
        

    protected:  // from CCoeControl

        /**
        * Handle size change, lay out and draw components.
        */
        virtual void SizeChanged();

        /**
        * Handle focus change.
        * @since Series60 1.2
        * @param aDrawNow Draw now?
        */
        virtual void FocusChanged( TDrawNow aDrawNow );
        
    private:    // data		
		
        // Editor. Owned.
		CEikRichTextEditor* iEditor;  
        
        // Input frame. Owned.
        CAknInputFrame* iInputFrame;        
        // Not owned.
        MIMCVAppMessageEditorObserver* iEditorObserver;
        // Not owned. Pointer to observer
        MIMCVTapEventObserver* iTapObserver;
        // ID which needs to be reported back to TapObserver
        TUint iTapControlId;
        // does not own. Reference to the AppView object
        CIMCVAppView& iAppView;
        //does not own. reference to the smiley extension handler
        CIMCVAppMessageExtensionsHandler& iExtensionHandler;
        
        TInt iServiceId;
        
        // Owns
        CAknsBasicBackgroundControlContext* iBgContext;
    
      
    };

#endif
