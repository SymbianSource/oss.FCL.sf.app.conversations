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
* Description:  richtext container
*  Description : Handles statuspane, context pane, navi pane
*
*/


#ifndef _CIMCVAPPRICHTEXTCONTAINER_H
#define _CIMCVAPPRICHTEXTCONTAINER_H

//  INCLUDES
#include "mimcvenginemessagecontainer.h"
#include "mimcvapplayoutchangeobserver.h"
#include "mimcvappresourcechangeobserver.h"
#include <gulicon.h>
#include <e32base.h>
#include <e32std.h>
#include <frmtlay.h> // TCursorSelection


// FORWARD DECLARATIONS
class CIMCVAppMessageWrapper;
class CParaFormatLayer;
class CCharFormatLayer;
class CRichText;
class MLayDoc;
class MIMCVAppTextView;
class CIMCVAppMessageExtensionsHandler;
class MIMCVAppUi;
class MIMCVAppResourceChangeObserver;
// CLASS DECLARATION

/**
*  Rich text container
*  @since Series 60 5.0
*/
class CIMCVAppRichTextContainer : public CBase, 
                             public MIMCVEngineMessageContainer,
                             public MIMCVAppLayoutChangeObserver,
                             public MIMCVAppResourceChangeObserver
    {
    public:  // enumerations
        enum THighlightMovement
            {
            ENext = 0,
            EPrevious
            };
            
    	static CGulIcon*& iTempIcon;


	private:    // enums
    
        enum THighlightState
            {
            ENoHighlight = 0,
            EItemSelected,
            EMessageTop,
            EMessageBottom
            };
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aTextView Viewer of the text
         * @param aExtensionsHandler Message extensions handler
         * @param aMessageUtils Message utils
         * @param aScrollOver Should we scroll over the top and bottom,
         *                    i.e. disabling highlight when pressing up at top.
		 * @param aPrimaryKey primary key of the object - used for storage and searching
         */
        static CIMCVAppRichTextContainer* NewL(MIMCVAppUi* aAppUi,
                MIMCVAppTextView* aTextView, 
                CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
                CGulIcon* aFromMe,
                CGulIcon* aToMe,
                MGraphicsDeviceMap* aMap,                              
                const TDesC& aPrimaryKey,
                TInt aServiceId,
                TBool aScrollOver /*= ETrue*/
                                );

        /**
         * Destructor.
         */
        virtual ~CIMCVAppRichTextContainer();

	 public: // From MIMCVAppResourceChangeObserver
    
        /**
         * Reload all icons on resource change
         */
        void ResourceChangedL();
    public: // New functions
    
           
        /**
         * @return Buffer containing the currently selected text
         * (or empty buffer if nothing selected)
         */
        HBufC* SelectedItemL();
        
        /**
         * @return Type of selected item
         */
        TInt SelectedItemType();

        /**
         * @return Text layout interface for displaying the text
         */
        MLayDoc* TextLayout();

        /**
         * Moves current highlight selection
         * @return KErrNotFound If there are no items to highlight. 
         *         KErrNone otherwise.
         */
        TInt MoveHighlightL( THighlightMovement aDirection );
        
        /**
         * Set all highlighting off
         */
        void DisableHighlightL();
        
              
        /**
         * @return ETrue if some item is highlighted
         */
        TBool Highlighted();
        
        /**
         * Highlights whole message or item at given position.
         * @param aPos Highlights the message or item which has 
         *             the craracter at aPos
         * @return KErrNone           - if highlighted a new item
         *         KErrAlreadyExists  - if item is already highlighted
         *         KErrNotFound       - if can't find message or item at aPos
         */
        TInt HighlightItemAtPosL( TInt aPos );
        
        
        /**
         * @returns the total messages in the view
         */
    	TInt GetMessageCount();
    	
        /**
         * Sets item highlight on or off
         * @param aItemHighlight ETrue if the items inside messages 
         *        should be highlighted
         */
        void SetItemHighlight( TBool aItemHighlight );       

    public: // From MIMCVAppLayoutChangeObserver
    
        /**
         * From MIMCVAppLayoutChangeObserver
         */
        void LayoutChangedL( TInt aType );
        
    public:
         /**
         * Returns the Primary key of the object
         */
        const TDesC& GetPrimaryKey();
    
	/**
	* Updates the color of messages according to current skin.
	* Only the theme colored part of messages are updated.
	* This method should be called when the skin is changed.
	* @return None
	*/
	void UpdateSkinnedTextColourL();
      
    protected:  // Functions from MIMCVEngineMessageContainer

        /**
         * Add new message to container
         * @param aMessage Message
         */
        void AddMessageL( MIMCVEngineMessage& aMessage );
                

        /**
         * @see MIMCVEngineMessageContainer
         */
        void InsertMessageL( MIMCVEngineMessage& aMessage );
        
    protected: // New functions
    
        /**
         * Updates all wrappers after (and including) aFirstIndex
         * Adds aChange to wrapper positions (in rich text)
         * @param aFirstIndex Index of first modified wrapper
         * @param aChange Removed or added number of characters in rich
         *                text before wrapper at aFirstIndex
         */
        void UpdateWrappers( TInt aFirstIndex, TInt aChange );
    
        /**
         * @return Union of two selections
         * @param aSel1, cursor selection position1
         * @param aSel2, cursor selection position2
         */
        TCursorSelection Union( const TCursorSelection& aSel1, 
        	const TCursorSelection& aSel2 );
        
        /**
         * @return Currently highlighted selection, or last 
         *         message if highlighting is disabled
         */
        TCursorSelection CurrentSelection() const;
        
        /**
         * Sets the background color of selected text
         * @param aSel, cursor selection position
         * @param aColor, color to use
         */
        void TextBackColorL( const TCursorSelection& aSel, const TRgb& aColor );
        
        /**
         * Sets the color of selected text
         * @param aSel, cursor selection position
         * @param aColor, color to use
         */
        void TextColorL( const TCursorSelection& aSel, const TRgb& aColor );
        
        /**
         * Sets the background color of selected paragraph
         * @param aSel, cursor selection position
         * @param aColor, color to use
         */
        void BackColorL( const TCursorSelection& aSel, const TRgb& aColor );
        
        /**
         * Format the highlight item
         * @param aSel, cursor selection position
         */
        void HighLightItemL( const TCursorSelection& aSel );
        
        /**
         * Sets the selection bold
         * @param aSel, cursor selection position
         */
        void BoldL( const TCursorSelection& aSel );
        
        /**
         * Parses text for highlight items (phone numbers, urls) and places
         * the positions to messagewrapper.
         * @param aText Text to be parsed
         * @param aStartPos Start position of aText in rich text
         * @param aRelativePos Relative position of aText inside one message
         * @param aMessageWrapper Message wrapper where positions are stored
         */
        void ParseTextL( const TDesC& aText, TInt aStartPos, TInt aRelativePos, 
        	CIMCVAppMessageWrapper& aMessageWrapper );
    
        /**
         * Add nickname of a message to rich text
         * @param aMessageWrapper Message wrapper
         * @return Selection for nickname (for coloring)
         */
        TCursorSelection AddNickL( CIMCVAppMessageWrapper& aMessageWrapper );

        /**
         * Add timestamp of a message to rich text
         * @param aMessageWrapper Message wrapper
         * @return Selection of timestamp
         */
        TCursorSelection AddTimeStampL( CIMCVAppMessageWrapper& aMessageWrapper );
        
        /**
         * Add content of a message to rich text
         * @param aMsgStart Start position of this message in rich text
         * @param aMessageWrapper Message wrapper
         */
        void AddContentL( TInt aMsgStart, CIMCVAppMessageWrapper& aMessageWrapper );
        
        

        
        /**
        * Helper method to determine which part of the message really is
        * colored by the active skin.
        * @param aMessageWrapper  Message to inspect
        * @param aUseDefaultColors Variation for default colours
        * @param aCustomDefinedColor Has user customized the message colour
        * @return Selection of the text which follows the skin color
        */
        TCursorSelection SkinColoredSelection(
                              CIMCVAppMessageWrapper& aMessageWrapper,
                              TBool aUseDefaultColors, 
                              TBool aCustomDefinedColor );
        
        /**
         * Helper method for replacing all paragraph delimiters
         * inside message content with line breaks, needed for
         * keeping text alignment consistent inside one message.
         * Used in AddContentL.
         * @since S60 v5.0
         * @param aSource Source descriptor.
         * @param aTarget Target descriptor. Caller is responsible
         *                to provide a large enough descriptor,
         *                aTarget size must not be less than
         *                size of aSource.
         */
        void ReplaceParaDelimsWithLineBreaks(
                const TPtrC& aSource,
                TPtr& aTarget ) const;
        
        /**
         * Helper method to update selection inside rich text.
         * This will remove the highlight from previously selected item
         * and add highlight to newly selected item.
         * @since S60 v5.0
         */
        void UpdateSelectionL();

        /**
         * Insert nickname of a message to rich text start.
         * @param aMessageWrapper Message wrapper.
         * @return Selection for nickname.
         */
        TCursorSelection InsertNickL( CIMCVAppMessageWrapper& aMessageWrapper );

        /**
         * Insert timestamp of a message to rich text.
         * @param aMessageWrapper Message wrapper.
         * @return Selection of timestamp.
         */
        TCursorSelection InsertTimeStampL( CIMCVAppMessageWrapper& aMessageWrapper );
        
        /**
         * Insert content of a message to rich text.
         * @param aMessageWrapper Message wrapper.
         * @return Length of content added.
         */
        TInt InsertContentL( CIMCVAppMessageWrapper& aMessageWrapper );
        
    public:
	        /**
         * Initializes the object with its cintainer's object
         * @param aTextView - container object
         */
        void Initialize( MIMCVAppUi* aAppUi,
                MIMCVAppTextView* aTextView, 
                CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
                CGulIcon* aFromMe,
                CGulIcon* aToMe,
                MGraphicsDeviceMap* aMap,                              
                TBool aScrollOver /*= ETrue*/);
        
        TInt ServiceId() const;
        
        
    private:

        /**
         * constructor.
         * @param aTextView Viewer of the text
         * @param aExtensionsHandler Message extensions handler
         * @param aMessageUtils Message utils
         * @param aScrollOver Should we scroll over the top and bottom,
         *                    i.e. disabling highlight when pressing up at top.
         */
        CIMCVAppRichTextContainer( MIMCVAppUi* aAppUi,
                MIMCVAppTextView* aTextView, 
                CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
                CGulIcon* aFromMe,
                CGulIcon* aToMe,
                MGraphicsDeviceMap* aMap,
                TInt aServiceId,
                TBool aScrollOver /*= ETrue*/ );

        /**
         * By default Symbian 2nd phase constructor is private.
		 * @ params aPrimaryKey - primary key of the object
         */
        void ConstructL(const TDesC& aPrimaryKey);

		CGulIcon* LoadDefaultBitmapL( TInt aBitmapId, TInt aMaskId, 
                                const TDesC& aFullPath, TBool aOverrideBrand = EFalse) ;

    private:    // Data
    
    	MIMCVAppUi* iAppUi;  
        // doesn't own, interface to text viewer
        MIMCVAppTextView* iTextView;
        
          // item highlight
        TBool iItemHighlight;
        	// current highlight state
		THighlightState iHighlightState;
			// message extensions handler
		CIMCVAppMessageExtensionsHandler* iExtensionsHandler;
	
		// Should we scroll over the top and bottom,
        // i.e. disabling highlight when pressing up at top.
        TBool iScrollOver;
      
        // Owns. Paragraph formatter
        CParaFormatLayer* iParaFormatLayer;

        // Owns. Character formatter
        CCharFormatLayer* iCharFormatLayer;

        // Owns. rich text document
        CRichText* iRichText;

        // Owns. message wrappers
        RPointerArray<CIMCVAppMessageWrapper> iMessages;
        
        // selected message
		TInt iSelected;
		
		// selected item in message
		TInt iSelectedItem;
		// previous selection
		TCursorSelection iPrevious;
		
	  // should whole message be colored
        TBool iColorWholeMessage;
        // alternative colors
        TBool iOwnMsgColorInUse;
        TRgb iOwnMsgColor;
        TBool iMsgColorInUse;
        TRgb iMsgColor;
        
        // Show timestamp setting value
        TBool iShowTimeStamps;
        
       
        // doesn't own
        const CFont* iPrimaryFont;
        const CFont* iSecondaryFont;
        
                // "From me" icon. Not owned.
        // Pointer to a pointer because this can change
        CGulIcon* iFromMe;
        
        // "To me" icon. Not owned.
        // Pointer to a pointer because this can change
        CGulIcon* iToMe;
        
        		
	    // interface to graphics device for converting pixels to twips
        MGraphicsDeviceMap* iMap;
        
		//Owns Primary key - Used while storing the object
        HBufC* iPrimaryKey;
        
        // service Id
        TInt iServiceId;
        
    };

#endif      // _CIMCVAPPRICHTEXTCONTAINER_H

// End of File
