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
* Description:  UI dialog for showing the Terms of use of a certain server
*
*/

#include    "cvimpstuitermsofusedlg.h"

#include    <txtrich.h>
#include    <aknenv.h>
#include    <AknSettingCache.h>
#include    <bldvariant.hrh>
#include    <AknQueryDialog.h>
#include    <StringLoader.h>
#include    <eikmenup.h>
#include    <avkon.rsg>
#include    <avkon.hrh>
#include    <eikappui.h>
#include    <eikenv.h>
#include    <AknUtils.h>
#include    <AknsUtils.h>
#include    <hlplch.h>
#include    <featmgr.h>
#include    <AknsBasicBackgroundControlContext.h>
#include    <aknEditStateIndicator.h>
#include    <eikrted.h> // CEikRichTextEditor
#include    <barsread.h>

#include    <applayout.cdl.h> //cdl
#include    <aknlayoutscalable_apps.cdl.h>

#include    <vimpstuires.rsg>  
// item finder (Automatic highlight)
#include    <finditemmenu.rsg>
#include    <finditemmenu.h>
#include    <finditem.hrh>
#include    <finditemdialog.h>
// title pane handling
#include    <akntitle.h>
#include    <aknlists.h>
#include    <aknnavi.h>
#include    <utf.h>
#include    <txtetext.h>

#include    <StringLoader.h>
#include    "vimpstui.hrh"
#include "uiservicetabtracer.h"
const TInt KTextMargin = 100;  // normal text margin in twips
const TInt KSpaceBefore = 10;  // space before each paragraph (twips)
const TInt KPixelsBetweenLines = 5; // pixels between text lines


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// RunDialogLD
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiTermsOfUseDlg::RunDialogLD( TInt aResourceId )
    {
    TInt retVal;
    iRetVal = &retVal;
    
    ExecuteLD( aResourceId );
    
    return retVal;
    }

// ---------------------------------------------------------------------------
// NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTUiTermsOfUseDlg* CVIMPSTUiTermsOfUseDlg::NewL( )
    {
    CVIMPSTUiTermsOfUseDlg* self = CVIMPSTUiTermsOfUseDlg::NewLC( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTUiTermsOfUseDlg* CVIMPSTUiTermsOfUseDlg::NewLC( )
    {
    CVIMPSTUiTermsOfUseDlg* self = new( ELeave ) CVIMPSTUiTermsOfUseDlg;
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::ConstructL()
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::ConstructL( )
    {
	TRACER_AUTO;
    iBgContext = CAknsBasicBackgroundControlContext::NewL( 
        KAknsIIDQsnBgAreaMainMessage, Rect(), ETrue );
    
    iItemFinder = CItemFinder::NewL();
    iItemFinder->AddObserver( *this );
 
    SetNewTitleTextL();
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CVIMPSTUiTermsOfUseDlg::~CVIMPSTUiTermsOfUseDlg()
    {
    TRAP_IGNORE( RestoreTitlePaneTextL() );
    
    AknsUtils::DeregisterControlPosition( this );

    delete iBgContext;
    delete iItemFinder;    
    delete iOldTitleText;
    if(iToUText)
	    {
	    delete iToUText;
	    }
    }

// ---------------------------------------------------------------------------
//  C++ constructor
// ---------------------------------------------------------------------------
//
CVIMPSTUiTermsOfUseDlg::CVIMPSTUiTermsOfUseDlg()
    {
    }



// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::HandleParsingComplete()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::HandleParsingComplete()
    {
    //not implemented
    }
// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::PreLayoutDynInitL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::PreLayoutDynInitL()
	{
	TRACER_AUTO;
	CAknDialog::PreLayoutDynInitL();
    AknsUtils::RegisterControlPosition( this );
    
    iEditor = static_cast<CEikRichTextEditor*>( Control( ETOURichEditorId ) );   

    iParaFormatLayer = CParaFormatLayer::NewL(); 
    iEditor->SetParaFormatLayer( iParaFormatLayer );    // takes the ownership
    
    iCharFormatLayer = CCharFormatLayer::NewL();
    iEditor->SetCharFormatLayer( iCharFormatLayer );    // takes the ownership
    
    // Update text color from skin
    UpdateTextColorL();
    
    // Get ToU message
    if(iToUText)
	    {
	    iEditor->SetTextL( iToUText );
	    delete iToUText;
	    iToUText = NULL;
	    }
    
    // Insert paragraph delimiter to make
    // iItemFinder scroll the view all the way down
    TChar lb( CEditableText::EParagraphDelimiter );
    iEditor->Text()->InsertL( iEditor->TextLength(), lb );

    iEditor->CreateScrollBarFrameL();
    // Only vertical scrollbar is needed
    iEditor->ScrollBarFrame()->SetScrollBarVisibilityL(
            CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn);
    iEditor->ScrollBarFrame()->SetScrollBarFrameObserver(this);
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::PostLayoutDynInitL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::PostLayoutDynInitL()
    {
	TRACER_AUTO;
    CAknDialog::PostLayoutDynInitL();
    
   
    iItemFinder->SetEditor( &iEditor );
    
    if ( iBgContext )
        {
        iBgContext->SetRect( Rect() );
        iBgContext->SetParentPos( PositionRelativeToScreen() );
        }
    // Try to scroll down
    if (! iItemFinder->NextItemOrScrollL( CItemFinder::ENextDown ) )
        {
        // at bottom => return
        UpdateCbaL();
        //return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::SetSizeAndPosition
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::SetSizeAndPosition( const TSize& /*aSize*/ )
    {    
    // dialog fills the mainpane
    TRect mainPane;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPane );
    SetRect( mainPane );
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::OkToExitL( TInt aButtonId )
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TBool CVIMPSTUiTermsOfUseDlg::OkToExitL( TInt aButtonId )
	{
	switch( aButtonId )
	    {
	    case EAknSoftkeyCancel:
	        {
	        *iRetVal = EAknSoftkeyCancel;
	        return ETrue;
	        }
	    case EIMPSCommonUITOUContinue:
	        {            
	        *iRetVal = EAknSoftkeyOk;
	        return ETrue;
	        }	   
	    case EAknSoftkeyEmpty:
	        {
	        return EFalse;
	        }
        default:
            {            
        	return ETrue;
            }
	    }
	}

// -----------------------------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CVIMPSTUiTermsOfUseDlg::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                   TEventCode aType )
	{
	TRACER_AUTO;
    if( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }

    switch( aKeyEvent.iCode )
        {        
        case EKeyBackspace:
            {
            return EKeyWasConsumed;
            }
         
        case EKeyUpArrow:
            {
            TKeyResponse res =
                iItemFinder->NextItemOrScrollL( CItemFinder::ENextUp ) ?
                   EKeyWasConsumed : EKeyWasNotConsumed;           
            return res;
            }
            
        case EKeyDownArrow:
            {
            TKeyResponse res =
                iItemFinder->NextItemOrScrollL( CItemFinder::ENextDown ) ?
                   EKeyWasConsumed : EKeyWasNotConsumed;
            if ( res == EKeyWasConsumed )
                {
                UpdateCbaL();
                }
            if ( !iItemFinder->NextItemOrScrollL( CItemFinder::ENextDown ) )
                {
                UpdateCbaL();
                }
            return res;
            }
        
        case EKeyRightArrow:
            {
            return EKeyWasConsumed;
            }
            
        case EKeyEscape: // handle dialog shutdown.
            {
			TryExitL( EAknSoftkeyExit );
            return EKeyWasConsumed;
            }
        default:
            {
            return EKeyWasConsumed;
            }
        }
    }

void CVIMPSTUiTermsOfUseDlg::HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType)
    {
	TRACER_AUTO;
    if( !aScrollBar )
        {
        return ;
        }
    switch( aEventType )
        {
        
        case EEikScrollThumbDragVert :
        case EEikScrollDown:
        case EEikScrollBottom:
        case EEikScrollPageDown:
        case EEikScrollUp:
        case EEikScrollTop:
        case EEikScrollPageUp:
            {

            TInt pos = iEditor->ScrollBarFrame()->VerticalScrollBar()->ThumbPosition();
            iEditor->HandleScrollEventL(aScrollBar, aEventType);
            if( pos == iThumbMaxPos )
                {
                UpdateCbaL();
                }
            break;
            }
        default:            
            break;
        }

    }
// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::MopSupplyObject
//
// ---------------------------------------------------------
//

TTypeUid::Ptr CVIMPSTUiTermsOfUseDlg::MopSupplyObject( TTypeUid aId )
    { 
    if ( aId.iUid == MAknsControlContext::ETypeId )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }

    return CCoeControl::MopSupplyObject( aId );
    }
    
    
// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::SizeChanged
//
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::SizeChanged()
    {   
    // base class method
    CAknDialog::SizeChanged();
    
    if ( iBgContext )
        {
        iBgContext->SetRect( Rect() );
        iBgContext->SetParentPos( PositionRelativeToScreen() );
        }

    if( iEditor )
        {
        TRect temp = Rect();
        temp.SetWidth(Rect().Width()-iEditor->ScrollBarFrame()->VerticalScrollBar()->ScrollBarBreadth());
        iEditor->SetRect( temp );
        }    
        
    // Update the font to correct size
    if( iParaFormatLayer && iCharFormatLayer )
        {
        // Correct font
        const CFont* font = AknLayoutUtils::FontFromId( EAknLogicalFontSecondaryFont );
        
        // format richtext
        TInt lineSpacing( font->HeightInPixels() + KPixelsBetweenLines );
        CParaFormat paraFormat;
        TParaFormatMask paraFormatMask;
        paraFormat.iLineSpacingControl = 
                        CParaFormat::ELineSpacingExactlyInPixels;
        paraFormatMask.SetAttrib( EAttLineSpacingControl );
        paraFormat.iLineSpacingInTwips = lineSpacing;
        paraFormatMask.SetAttrib( EAttLineSpacing );
    	paraFormat.iLanguage = 
    	                CAknEnv::Static()->SettingCache().InputLanguage();
    	paraFormatMask.SetAttrib( EAttParaLanguage );
        paraFormat.iLeftMarginInTwips = KTextMargin;
        paraFormatMask.SetAttrib( EAttLeftMargin );
        paraFormat.iSpaceBeforeInTwips = KSpaceBefore;
        paraFormatMask.SetAttrib( EAttSpaceBefore );

        TCharFormat charFormat;
        TCharFormatMask charFormatMask;
        charFormat.iFontSpec = font->FontSpecInTwips();
        charFormatMask.SetAttrib( EAttFontTypeface );
        charFormatMask.SetAttrib( EAttFontHeight );
        
        //can't leave here
        if( iEditor )
        	 {
	         TRAPD(err, iParaFormatLayer->SetL( &paraFormat, paraFormatMask );
	                   iCharFormatLayer->SetL( charFormat, charFormatMask );
	                   iEditor->HandleTextChangedL(); );
					 
        
		        if( err != KErrNone )
		            {
		            CActiveScheduler::Current()->Error( err );
		            }       
		     }
        TRAP_IGNORE(iEditor->MoveDisplayL(TCursorPosition::EFPageDown) );
        TRAP_IGNORE(iEditor->MoveDisplayL(TCursorPosition::EFPageDown) );
        
        iThumbMaxPos = iEditor->ScrollBarFrame()->VerticalScrollBar()->ThumbPosition();
        
        TRAP_IGNORE(iEditor->MoveDisplayL(TCursorPosition::EFPageUp) );
        TRAP_IGNORE(iEditor->MoveDisplayL(TCursorPosition::EFPageUp) );
        }
    }
    
// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::SetNewTitleText
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::SetNewTitleTextL()
    {
	TRACER_AUTO;
    CAknTitlePane* title = GetTitlePaneInstanceL();
    
    // Store old one
    HBufC* tmp = title->Text()->AllocL();
    delete iOldTitleText;
    iOldTitleText = tmp;

	// Clear the navi pane
	// Get pointer to status-pane
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->
		    StatusPane();
	// Get pointer to navi-pane
    CAknNavigationControlContainer* naviPane = 
            static_cast<CAknNavigationControlContainer*>(
		        statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
	naviPane->PushDefaultL( ETrue );    
    
    // Set the new text
    HBufC* tempTitle = iEikonEnv->AllocReadResourceLC( R_QTN_SERVTAB_TOU_TITLE );
    title->SetTextL( *tempTitle );
    CleanupStack::PopAndDestroy( tempTitle );
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::GetTitlePaneInstanceL
// Gets titlepane instance from CEikonEnv
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknTitlePane* CVIMPSTUiTermsOfUseDlg::GetTitlePaneInstanceL() const
    {
	TRACER_AUTO;
    CAknTitlePane* title = static_cast< CAknTitlePane* >
        ( CEikonEnv::Static()->AppUiFactory()->StatusPane()->ControlL(
                                        TUid::Uid( EEikStatusPaneUidTitle ) ) );
	if ( !title )
		{
		User::Leave( KErrNotSupported );
		}
    return title;
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::DoRestoreTitlePaneTextL() const
// Does the actual restoring.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::RestoreTitlePaneTextL() const
    {
	TRACER_AUTO;
    CAknTitlePane* title = GetTitlePaneInstanceL();

    if ( iOldTitleText )
        {
        title->SetTextL( *iOldTitleText );
        }
    else
        {
        title->SetTextToDefaultL();
        }

	// restore the navi pane
	// Get pointer to status-pane
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->
		    StatusPane();
	// Get pointer to navi-pane
    CAknNavigationControlContainer* naviPane = 
        static_cast<CAknNavigationControlContainer*>(
		    statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
	naviPane->Pop();
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::UpdateCbaL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::UpdateCbaL()
    {
	TRACER_AUTO;
    TInt cbaRes = R_CNUI_TERMS_OF_USE_DIALOG_CBA;  
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    cba->SetCommandSetL( cbaRes );
    cba->DrawNow();
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::UpdateTextColorL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::UpdateTextColorL()
    {
	TRACER_AUTO;
    if( !iEditor )
        {
        return;
        }
        
    // Get text color from skin
    TRgb textColor( KRgbBlack );
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(), 
                               textColor,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG6 );
	TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormat.iFontPresentation.iTextColor = textColor;
    charFormatMask.SetAttrib( EAttColor );
    
    // Apply format
    int length = iEditor->RichText()->DocumentLength();
    iEditor->RichText()->SetInsertCharFormatL( charFormat, charFormatMask, length );
    iEditor->RichText()->ApplyCharFormatL( charFormat, charFormatMask, 0, length );
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::HandleResourceChange
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::HandleResourceChange( TInt aType )
    {
    if ( aType == KAknsMessageSkinChange )
        {
        // When skin changes, we need to update text color
        TRAP_IGNORE( UpdateTextColorL();
                     iEditor->HandleTextChangedL(); );
        }
    CAknDialog::HandleResourceChange( aType );
    }

// ---------------------------------------------------------
// CVIMPSTUiTermsOfUseDlg::SetToUMsg
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiTermsOfUseDlg::SetToUMsgL(HBufC*  aToUText)
	{
	iToUText = aToUText;
	}



// End of file
