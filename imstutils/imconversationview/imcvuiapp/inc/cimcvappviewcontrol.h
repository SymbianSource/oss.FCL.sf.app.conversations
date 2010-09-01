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
* Description:  cimcvappviewcontrol.h
*
*/



#ifndef _CIMCVAPPVIEWCONTROL_H
#define _CIMCVAPPVIEWCONTROL_H

//  INCLUDES
#include "cimcvapprichtextcontainercache.h"
#include "cimcvapptextviewcontrol.h"
#include "cimcvapprichtextcontainer.h"
#include "cimcvengine.h"
// FORWARD DECLARATIONS
class CIMCVAppMessageFlowHandler;
class MIMCVEngineMessageReadInterface;
class CIMCVAppMessageExtensionsHandler;
class MIMCVTapEventObserver;
// CLASS DECLARATION

/**
*  @imcvuiapp.exe
*  @since 5.0
*/
class CIMCVAppViewControl : public CIMCVAppTextViewControl
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aRect Draw area
         * @param aParent Parent control
         * @param aReadInterface Message read interface
         * @param aExtensionsHandler Message extensions handler
         * @param aMessageUtils Message utils
		 * @param aRTContainerCache - cache
         * @param aScrollOver Should we scroll over the top and bottom,
         *                    i.e. disabling highlight when pressing up at top.
         *                    EFalse used with recorded chats.
         * @return Instance of this class
         */
        static CIMCVAppViewControl* NewL( const TRect& aRect, 
                    const CCoeControl& aParent,
                    MIMCVEngineMessageReadInterface& aReadInterface,
                    CIMCVAppMessageExtensionsHandler& aExtensionsHandler,
                    CGulIcon* aFromMe,
                    CGulIcon* aToMe,
                    CIMCVAppRichTextContainerCache& aRTContainerCache,
                    CIMCVEngine& aActiveEngine,
                    TBool aScrollOver = ETrue);

        /**
         * Destructor.
         */
        virtual ~CIMCVAppViewControl();

    public: // New functions
    
        
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
         * Disable all highlighting
         */
        void DisableHighlightL();
        
        /**
         * @return ETrue if some item is highlighted
         */
        TBool Highlighted();

        /**
        * Move text's highlight up and down. 
        * @param aMovement In which direction we move the higlight
        */
        void MoveHighlightL( 
                CIMCVAppRichTextContainer::THighlightMovement aMovement);       
    
       
       /**
         * @returns the total messages in the view
         */
    	TInt GetMessageCount();
    
    	 /**
         * Set scrolling method
         * @param aHihghlight If ETrue, scroll by highlighting items
         * otherwise scroll by rows
         */
        void SetHighlight( TBool aHighlight );
        
         /**
         * Set observer for handling tap events. 
         * Supports only single observer.
         * @param aObserver observer to handle tap events
         * @param aId control id which is used by the observer to 
         *        identify this object. 
         */
        void SetTapObserver( MIMCVTapEventObserver* aObserver, TUint aId );
        
        
        /**
         * Should we fetch new messages or not
         * @param aFetch ETrue if this chat is in foreground and should receive 
         *               messages, EFalse otherwise.
         */
        void FetchMessages( TBool aFetch );

        
    protected:  // Functions from CIMCVAppTextViewControl

        /**
         * @see CIMCVAppTextViewControl
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                     TEventCode aType );

        /**
         * @see CIMCVAppTextViewControl
         */
        void ProcessStylusEventL( const TPointerEvent& aPointerEvent );        
        
        /**
         * @see CIMCVAppTextViewControl
         */
        void SizeChanged();

    private:

        /**
         * C++ default constructor.
         */
        CIMCVAppViewControl();

        /**
         * By default Symbian 2nd phase constructor is private.
         * @param aRect Draw area
         * @param aParent Parent control
         * @param aReadInterface Message read interface
         * @param aExtensionsHandler Message extensions handler
         * @param aMessageUtils Message utils
		 * @param aRTContainerCache - cache
         * @param aScrollOver Should we scroll over the top and bottom,
         *                    i.e. disabling highlight when pressing up at top.
         */
        void ConstructL( const TRect& aRect, const CCoeControl& aParent,
        			MIMCVEngineMessageReadInterface& aReadInterface,
                    CIMCVAppMessageExtensionsHandler& aExtensionsHandler,
        			CGulIcon* aFromMe,
                    CGulIcon* aToMe,
                    CIMCVAppRichTextContainerCache& aRTContainerCache,
                    CIMCVEngine& aActiveEngine,
                    TBool aScrollOver );


    private:    // Data

        // owns. Text container
        CIMCVAppRichTextContainer* iTextContainer;

        // owns. Message flow handler
        CIMCVAppMessageFlowHandler* iFlowHandler;
        
		// Not owned. Pointer to observer
		MIMCVTapEventObserver* iTapObserver;

		// ID which needs to be reported back to TapObserver
		TUint iTapControlId;
             
        // Tap waiting flags touch screen.
        TBool iWaitForDoubleTap;
        TBool iWaitForSingleTap;
        };

#endif      // _CIMCVAPPVIEWCONTROL_H

// End of File
