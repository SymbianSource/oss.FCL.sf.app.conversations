/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Dialog for showing the Terms of Use for a chat server
*                
*
*/


#ifndef CVIMPSTUITERMSOFUSEDLG_H
#define CVIMPSTUITERMSOFUSEDLG_H

#include <AknDialog.h>
#include <ItemFinder.h>
#include <eiksbobs.h>

class CEikRichTextEditor;
class CParaFormatLayer;
class CCharFormatLayer;
class CAknsBasicBackgroundControlContext;
class CAknTitlePane;
class CEikScrollBar;


/**
 *  Dialog for showing the Terms of use -text
 *
 *  @lib vimpstui.lib
 *  @since S60 3.1u
 */
NONSHARABLE_CLASS( CVIMPSTUiTermsOfUseDlg ) : public CAknDialog,
                                              public MItemFinderObserver,  
                                          public MEikScrollBarObserver
    {
    public:
        
        /**
         * Executes the dialog and destroys it before returning
         * @since 3.1u
         * @return EAknSoftkeyOk if user accepted.
         *         EAknSoftkeyCancel if user cancelled
         */        
        TInt RunDialogLD( TInt aResourceId );
        
        static CVIMPSTUiTermsOfUseDlg* NewL( );

        static CVIMPSTUiTermsOfUseDlg* NewLC( );

        virtual ~CVIMPSTUiTermsOfUseDlg();

    public: // from MObjectProvider
	    
	    TTypeUid::Ptr MopSupplyObject(TTypeUid aId); 
	    
  	    /*
	    *to set ToUString
	    *owner-ship transfered 
	    */
	    void SetToUMsgL(HBufC* aToUMsg);

	    
    public: //MEikScrollBarObserver
        void HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);     
    
    protected:  // from CEikDialog

        /**
        * @see CEikDialog
        */
        void PreLayoutDynInitL();

        /**
        * @see CEikDialog
        */
        void PostLayoutDynInitL();

        /**
        * @see CEikDialog
        */
        TBool OkToExitL( TInt aButtonId );

        /**
        * @see CEikDialog
        */
        void SetSizeAndPosition( const TSize& aSize );

        /**
        * @see CEikDialog
        */
		void SizeChanged();
		
    protected:  // From MItemFinderObserver

        /**
         * @see MItemFinderObserver
         */
        void HandleParsingComplete();

    protected:  //from base class CCoeControl

        /**
        * @see CCoeControl
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                     TEventCode aType );                                   
                                       
        
        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );     
  
        
    private:

        CVIMPSTUiTermsOfUseDlg();

        void ConstructL( );

    private:    // new functions
    
        /**
         * Inserts line to rich text object
         * @since 3.1u
         * @param aTextResourceId Resource for text in line
         * @param aBold Should text be bolded
         */
        void InsertLineL( TInt aTextResourceId, TBool aBold = EFalse );

        /**
         * Inserts line to rich text object
         * @since 3.1u
         * @param aText Text for line
         * @param aBold Should text be bolded
         */
        void InsertLineL( const TDesC& aText, TBool aBold = EFalse );
        
        /**
        * Search the LAF specified font for invite dialog
        * @since 3.1u
        * @return LAF specified font
        */
        const CFont* GetLayoutFont();
        
        /**
        * Gets the instance of the title pane
        * @return Title pane pointer
        * @since 3.1u
        */            
        CAknTitlePane* GetTitlePaneInstanceL() const;
        
        /**
        * Stores the old title pane text and sets a custom one
        * @since 3.1u
        */          
        void SetNewTitleTextL();
        
        /**
        * Retores the old title pane text
        * @since 3.1u
        */          
        void RestoreTitlePaneTextL() const;

        /**
         * Updates CBA.
         * @since S60 v3.2
         */
        void UpdateCbaL();
        
        /**
         * Helper method to update TOU message text color according to 
         * current skin.
         * Note! This will not update text control
         * @since S60 v3.1u
         */
        void UpdateTextColorL();
        
        
    private: // data
                
        // not own
        CEikRichTextEditor* iEditor;

        // own        
        CItemFinder* iItemFinder;
        
        // Return value of this dialog
        TInt iReturnVal;
        
        // Not Own. Paragraph formatter
        CParaFormatLayer* iParaFormatLayer;
        
        // Not Own. Character formatter
        CCharFormatLayer* iCharFormatLayer;

        // Own. For controlling the background image
        CAknsBasicBackgroundControlContext* iBgContext;
        
        // Own.
        HBufC* iOldTitleText;
        
        TInt* iRetVal; // Not own, return value of the dialog,
        
        HBufC* iToUText;
        
		// Own - stores the maximum position of the vertical scrollbar thumb
        TInt iThumbMaxPos;
    };

#endif // CVIMPSTUITERMSOFUSEDLG_H
