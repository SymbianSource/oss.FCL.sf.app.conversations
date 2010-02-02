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
* Description:  textview class
*
*/


#ifndef _CIMCVAPPTEXTVIEWCONTROL_H
#define _CIMCVAPPTEXTVIEWCONTROL_H

//  INCLUDES
#include <eiksbobs.h> // MEikScrollBarObserver
#include <eikbctrl.h>
#include <frmtlay.h> // TCursorSelection
#include "mimcvapptextview.h"

// FORWARD DECLARATIONS
class CTextLayout;
class CTextView;
class CEikScrollBarFrame;
class CAknsBasicBackgroundControlContext;
class CIMCVAppCustomDraw;

// CLASS DECLARATION

/**
*  Text viewer component
*  @since 5.0
*/
class CIMCVAppTextViewControl : public CCoeControl,
						   public MIMCVAppTextView,
                           public MEikScrollBarObserver
    {
    
    public:  // Constructors and destructor
        
        /**
         * Two-phased constructor.
         * @param aRect Draw area
         * @param aParent Parent control
         * @param aTextLayout Text to be viewed
         * @return Instance of this class
         */
        static CIMCVAppTextViewControl* NewL( const TRect& aRect, 
                                         const CCoeControl& aParent,
                                         MLayDoc* aTextLayout );
        
        /**
         * Destructor.
         */
        virtual ~CIMCVAppTextViewControl();


		void UpdateChatViewL ();
		/**
		 * Update the view
		 */

		void SmileyDialogLaunched (TBool aLaunched);
		
		void UpdateViewL();

    protected:  // New functions
        
        /**
         * Updates scrollbars
         */
        void UpdateScrollBarsL( TBool aScrollDraw = ETrue );
        

        /**
         * Set highlight state
         */
        void SetHighlighted( TBool aIsHighlighted );
        
        /**
         * Get scrollbars
         */
        const CEikScrollBarFrame* ScrollBarFrame();
        
        /**
         * Finds the document position which is located at aPoint
         * @param aPoint Location to find
         * @return Document position or 
         *         KErrNotFound if not found
         */
        TInt FindDocPos( TPoint aPoint );
    protected:  // Functions from MIMCVAppTextView
        
        /**
         * @see MIMCVAppTextView
         */
        void HandleFormatChangedL( TCursorSelection& aChanged,
                                   TBool aPreserverFocus = EFalse );
        
        /**
         * @see MIMCVAppTextView
         */
        void HandleInsertDeleteL( TCursorSelection& aChanged, 
                                  TInt aDeletedChars );
                                  
        /**
         * @see MIMCVAppTextView
         */
        void HandleAdditionL( TBool aFirst, 
                              TCursorSelection& aSelection, 
                              TBool aTop  );


        /**
         * @see MIMCVAppTextView
         */
        void ScrollVisibleL( TCursorSelection& aSelection, TBool aTop );
        
        /**
         * @see MIMCVAppTextView
         */
        TCursorSelection ScrollSelection();
        
        /**
         * @see MIMCVAppTextView
         */
        void ScrollLinesL( TInt aAmount );
        
        /**
         * @see MIMCVAppTextView
        */
   public:
     
        TBool IsVisible( TInt aPos );
    
        /**
         * @see MIMCVAppTextView
         */
     protected:
        TBool ParagraphFits( TInt aPos );
        
        /**
         * @see MIMCVAppTextView
         */
        void Redraw();

        /**
         * @see MIMCVAppTextView
         */
        void SetPenBehaviour( TPenBehaviour aPen );

        /**
         * @see MIMCVAppTextView
         */
        MIMCVAppTextView::TPenBehaviour PenBehaviour();

        /**
         * @see MIMCVAppTextView
         */
        void ProcessStylusEventL( const TPointerEvent& aPointerEvent );
        
        /**
         * @see MIMCVAppTextView
         */
        void HandleGlobalChangeNoRedrawL(
            TViewYPosQualifier aYPosQualifier = TViewYPosQualifier() );
        
        
    public: // from MObjectProvider
    
	    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);        

    protected: // from MEikScrollBarObserver

    	void HandleScrollEventL( CEikScrollBar* aScrollBar, 
    	                         TEikScrollEvent aEventType );

    protected:  // Functions from CCoeControl
    
        /**
         * @see CCoeControl
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                     TEventCode aType );
        
        /**
         * @see CCoeControl
         */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );        
        
        /**
         * @see CCoeControl
         */
        void Draw( const TRect& aRect ) const;
        
        /**
         * @see CCoeControl
         */
        void SizeChanged();
		
        /**
         * @see CCoeControl
         */
	    TInt CountComponentControls() const;
	    
	    
        /**
         * @see CCoeControl
         */
        CCoeControl* ComponentControl(TInt aIndex) const;
          
        
    protected:

        /**
        * C++ default constructor.
        */
        CIMCVAppTextViewControl();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aRect Draw area
        * @param aParent Parent control
        * @param aTextLayout Text to be viewed
        */
        void ConstructL( const TRect& aRect, const CCoeControl& aParent,
                         MLayDoc* aTextLayout );
        
       
    private:    // Data

        // Owns. rectangle in which to view text
        TRect iViewRect;
        
        // Owns. text layout
        CTextLayout* iLayout;
        
        // Owns. text view           
        CTextView* iTextView;
                
        // Owns. custom drawer for skins
        CIMCVAppCustomDraw* iCustomDraw;
        
        // Owns. scroll bar container
        CEikScrollBarFrame* iScrollBar;
        
        // Is control in stack?
        TBool iAddedToStack;

        //owns
        CAknsBasicBackgroundControlContext* iBgContext; // Skin background control context
        
        // Own. Clipping region to prevent flickering
        RRegion* iClipping;

        // Highlight state inside text view
        TBool iIsHighlighted;
        
        // Determines how this contol reacts to pointer events
        TPenBehaviour iPenType;

		TBool iSmileyDialogLaunched;
       
    };

#endif      // _CIMCVAPPTEXTVIEWCONTROL_H
            
// End of File

