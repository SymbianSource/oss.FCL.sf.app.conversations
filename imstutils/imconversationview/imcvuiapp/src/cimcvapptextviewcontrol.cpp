/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - Initial contribution
* 
* Contributors:
* 
* 
* Description:
* Export help related files.
*
*/


// INCLUDE FILES
#include    "cimcvapptextviewcontrol.h"

#include 	"cimcvappcustomdraw.h"
#include    <frmtview.h>    // CTextView
#include    <AknUtils.h>
#include    <avkon.mbg>
#include    <aknappui.h>
#include 	"mimcvapptextview.h"
#include    <aknlayoutscalable_apps.cdl.h>
#include    <AknsBasicBackgroundControlContext.h>
#include    <AknsDrawUtils.h>
#include 	"imcvlogger.h"
#include 	"imcvuiapputils.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::CIMCVAppTextViewControl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppTextViewControl::CIMCVAppTextViewControl() 
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::ConstructL( const TRect& aRect, 
                                     const CCoeControl& aParent,
                                     MLayDoc* aTextLayout )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ConstructL()	start") ); 
    CreateWindowL( &aParent );
    
    // get the drawing device
    CWindowGc& gc = SystemGc();
    CBitmapDevice* device = static_cast<CBitmapDevice*>( gc.Device() );    
    
    // create text layout and text view

    iViewRect = aRect;
    iClipping = new ( ELeave ) RRegion( aRect );
    
    iLayout = CTextLayout::NewL( aTextLayout, iViewRect.Width() );
    iTextView = CTextView::NewL( iLayout, iViewRect,
                device, device, &Window(), NULL, &iCoeEnv->WsSession() );

    iTextView->EnableFlickerFreeRedraw();
    iTextView->SetOpaque( ETrue );    
    
    // create our background drawer
    iCustomDraw = new( ELeave )CIMCVAppCustomDraw( iViewRect );
    iLayout->SetCustomDraw( iCustomDraw );

    // and scrollbar
    if( IMCVUiAppUtils::PenEnabled() )
        {
        // Observer
        iScrollBar = new( ELeave )CEikScrollBarFrame( this, this );
        }
    else
        {
        // No observer
        iScrollBar = new( ELeave )CEikScrollBarFrame( this, NULL );
        }
      
   
    if (AknLayoutUtils::DefaultScrollBarType(iAvkonAppUi) == CEikScrollBarFrame::EDoubleSpan)
        { 
        
        // For EDoubleSpan type scrollbar
        // If pen is supported, SB needs to own a window
        iScrollBar->CreateDoubleSpanScrollBarsL( IMCVUiAppUtils::PenEnabled(), 
                                                 EFalse, ETrue, EFalse );
        iScrollBar->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);
        
        }
    else
        {
        // For EArrowHead type scrollbar
        iScrollBar->SetTypeOfVScrollBar(CEikScrollBarFrame::EArrowHead);
        }
    iScrollBar->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, 
                                         CEikScrollBarFrame::EOn );
    TEikScrollBarModel vSbarModel;
    vSbarModel.iThumbPosition = 0; // current position
    vSbarModel.iScrollSpan = 0; // how many items
    vSbarModel.iThumbSpan = 0; // ignored

    iScrollBar->Tile( &vSbarModel );
    iScrollBar->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
    
    iBgContext = CAknsBasicBackgroundControlContext::NewL( 
        KAknsIIDQsnBgAreaMain, Rect(), ETrue );
    iCustomDraw->SetBackgroundContext( iBgContext, this );
    
    // set extensions
    ActivateL();    
    
    // we're ready
    static_cast<CCoeAppUi*>( iEikonEnv->AppUi() )->AddToStackL( this );
    iAddedToStack = ETrue;
    
    // Enable draging
    if ( DrawableWindow() && IMCVUiAppUtils::PenEnabled() )
        {
        EnableDragEvents();       
        Window().SetPointerGrab( ETrue );
        }
        
    // Enable draging
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ConstructL()	end") ); 
    }


// Destructor
CIMCVAppTextViewControl::~CIMCVAppTextViewControl()
    {
    if( iAddedToStack )
        {
        static_cast<CCoeAppUi*>( iEikonEnv->AppUi() )->RemoveFromStack( this );
        }

    delete iScrollBar;
    delete iCustomDraw;
    delete iTextView;
    delete iLayout;
    
    delete iBgContext;
    if( iClipping )
        {
        iClipping->Destroy();
        iClipping  = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::UpdateScrollBarsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::UpdateScrollBarsL( TBool aScrollDraw /*ETrue*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::UpdateScrollBarsL()	start") ); 
    TInt height = iLayout->FormattedHeightInPixels();
    TInt span = iViewRect.Height();
    TInt pos = iLayout->PixelsAboveBand();
    
    if( span < 0 )
        {
        span = 0;
        }
        
    if( pos < 0 )
        {
        pos = 0;
        }

    TEikScrollBarModel vSbarModel;
    vSbarModel.iThumbPosition = pos; // current position
    vSbarModel.iScrollSpan = height; // total length
    vSbarModel.iThumbSpan = span; // bar length

    // Double span
    if( iScrollBar && 
        iScrollBar->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan )
        {
        TAknDoubleSpanScrollBarModel vDsModel( vSbarModel );
        iScrollBar->Tile( &vDsModel );
        iScrollBar->SetVFocusPosToThumbPos( vDsModel.FocusPosition());
        }
    // ArrowHead
    else
        {
        iScrollBar->Tile( &vSbarModel );
        iScrollBar->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
        }

   if( aScrollDraw )
		{
	    iScrollBar->DrawScrollBarsNow();
	   	}
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::UpdateScrollBarsL()	end") ); 
    }
    
void CIMCVAppTextViewControl::UpdateChatViewL ()
{
	UpdateViewL ();
}


void CIMCVAppTextViewControl:: SmileyDialogLaunched (TBool aLaunched)
{
	iSmileyDialogLaunched = aLaunched;
}
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::UpdateViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::UpdateViewL()
    {
	// do not refresh if smiley dialog is launched.
	if (iSmileyDialogLaunched)
		return;
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::UpdateViewL()	start") ); 
    iTextView->FormatTextL();
    iTextView->SetSelectionVisibilityL( EFalse );
    TCursorSelection sel;
    // Don't scroll if some item is highlighted
    if ( !iIsHighlighted )
        {
        sel = TCursorSelection( iLayout->DocumentLength(), 0 );
        ScrollVisibleL( sel, ETrue );
         }
    else
        {
        sel = iTextView->Selection();
        ScrollVisibleL( sel, ETrue );
         }
        
    DrawDeferred();
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::UpdateViewL()	end") ); 
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::SetHighlighted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::SetHighlighted( TBool aIsHighlighted )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SetHighlighted()	start") ); 
    iIsHighlighted = aIsHighlighted;
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SetHighlighted()	end") ); 
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ScrollBarFrame
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CEikScrollBarFrame* CIMCVAppTextViewControl::ScrollBarFrame()
    {
    return iScrollBar;
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ScrollBarFrame
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppTextViewControl::FindDocPos( TPoint aPoint )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::FindDocPos()	start") ); 
    TTmPosInfo2 posInfo;
    TBool found = EFalse;
    
    TRAPD( err, found = iTextView->FindXyPosL( aPoint, posInfo ) );
    if ( err )
        {
        CActiveScheduler::Current()->Error( err );
        // Return not found
        return KErrNotFound;
        }
        
    if ( found )
        {
  	         
        return posInfo.iDocPos.iPos;
        }
    // not found
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::FindDocPos()	end") );
    return KErrNotFound;
    }
    
// ---------------------------------------------------------
// CIMCVAppTextViewControl::MopSupplyObject
//
// ---------------------------------------------------------
//
TTypeUid::Ptr CIMCVAppTextViewControl::MopSupplyObject( TTypeUid aId )
    {   
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::MopSupplyObject()	start") ); 
    if ( aId.iUid == MAknsControlContext::ETypeId )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::MopSupplyObject()	end") );
    return CCoeControl::MopSupplyObject( aId );
    }

// ---------------------------------------------------------
// CIMCVAppTextViewControl::HandleScrollEventL
//
// ---------------------------------------------------------
//
void CIMCVAppTextViewControl::HandleScrollEventL( CEikScrollBar* /*aScrollBar*/,
                                             TEikScrollEvent aEventType )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandleScrollEventL()	start") ); 
    switch( aEventType )
        {
        case EEikScrollUp:
            {
            ScrollLinesL( 1 );
            break;
            }
    	case EEikScrollDown:
    	    {
            ScrollLinesL( -1 );
    	    break;
    	    }
    	case EEikScrollPageUp:
    	    {
            iTextView->ScrollDisplayL( TCursorPosition::EFPageUp );
            UpdateScrollBarsL();
            DrawDeferred();
    	    break;
    	    }
    	case EEikScrollPageDown:
    	    {
            iTextView->ScrollDisplayL( TCursorPosition::EFPageDown );
            UpdateScrollBarsL();
            DrawDeferred();
    	    break;
    	    }
    	case EEikScrollThumbDragVert:
    	    {
    	    // Get position values from scrollbar
    	    const TEikScrollBarModel* model = 
    	        iScrollBar->VerticalScrollBar()->Model();
    	    TInt thumb = model->iThumbPosition;
            if( CEikScrollBarFrame::EDoubleSpan == 
                iScrollBar->TypeOfVScrollBar() )
                {
                const TAknDoubleSpanScrollBarModel* dsModel = 
                    static_cast <const TAknDoubleSpanScrollBarModel*>( model );
                thumb = dsModel->FocusPosition();
                }
    	    
    	    // Scroll the textview according to scrollbar
    	    TViewYPosQualifier yPosQua;
    	    yPosQua.SetHotSpot( TViewYPosQualifier::EFViewTopOfLine );
    	    TInt topPos = -thumb;
    	    iTextView->SetViewL( 0, topPos, yPosQua );
    	    break;
    	    }
    	case EEikScrollThumbReleaseVert:
    	    {
    	    // Nothing to do here because EEikScrollThumbDragVert gets 
    	    // the scrollbar in correct state
    	    break;
    	    }
    	default:
    	    {
    	    break;
    	    }
        }
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandleScrollEventL()	end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CIMCVAppTextViewControl::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                                 TEventCode aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::OfferKeyEventL()	start") ); 
    // Commented because it generates so much log
    // CHAT_DP( D_CHAT_LIT("CIMCVAppTextViewControl::OfferKeyEventL %d, type %d"), 
    //          aKeyEvent, aType )
    
    if( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }

    switch( aKeyEvent.iCode )
        {
        case EKeyDownArrow:
            {
			//scroll lines one line down
            ScrollLinesL( -1 );
            return EKeyWasConsumed;
      
            }
        case EKeyUpArrow:
            {
			//scroll lines one line up
            ScrollLinesL( 1 );
            return EKeyWasConsumed;

            }
        default:
            {
            // No handling for other events
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::OfferKeyEventL()	end") );
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::HandlePointerEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::HandlePointerEventL( 
    const TPointerEvent& aPointerEvent )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandlePointerEventL()	start") ); 
    
    if( !IMCVUiAppUtils::PenEnabled() )
        {
        return;
        }

    switch( iPenType )
        {
        case EPenForwardToChild:
            {
            // normal (CCoeControl) behaviour
            CCoeControl::HandlePointerEventL( aPointerEvent );
            break;
            }
        case EPenForwardToParent:
            {
            // forward all to parent
            if( Parent() )
                {
                Parent()->HandlePointerEventL( aPointerEvent );
                }
            break;
            }
        // Process event ourself
        case EPenProcessEvent:
            {
            ProcessStylusEventL( aPointerEvent );
            break;
            }
        case EPenIgnoreEvent:
            {
            // Ignore all
            break;
            }
        default:
            {
            // Can't be
            break;
            }
        }
    
    
	IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandlePointerEventL()	end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::Draw( const TRect& aRect ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::Draw()	start") ); 
    CWindowGc& gc = SystemGc();    
          
    // Clip text control and scrollbar to prevent flickering
    gc.SetClippingRegion( *iClipping );
    
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	if (iBgContext)
		{
		// Draw the skin background
		AknsDrawUtils::Background(
			skin, iBgContext, this, gc, aRect);
		}
	else
		{
		// clear the area
		gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
		gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
		gc.Clear( aRect );
		}		
    
    TRAP_IGNORE( iTextView->DrawL( aRect ) );
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::Draw()	end") );
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::SizeChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::SizeChanged()
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SizeChanged()	start") ); 
    TAknWindowLineLayout rpLayout( AknLayoutScalable_Apps::list_im_pane( 3 ) );
    TAknLayoutRect rpLayoutRect;
    rpLayoutRect.LayoutRect( Rect(), rpLayout );    
    
    // update rects
    iViewRect = rpLayoutRect.Rect();
    
    iCustomDraw->SetRect( iViewRect );    
    iTextView->SetViewRect( iViewRect );
    iLayout->SetWrapWidth( iViewRect.Width() );
    
    if (AknLayoutUtils::DefaultScrollBarType(iAvkonAppUi) == CEikScrollBarFrame::EDoubleSpan)
        {
        // For EDoubleSpan type scrollbar
        TAknWindowLineLayout sbLayout( AknLayoutScalable_Apps::scroll_pane_cp07( 3 ) );
        AknLayoutUtils::LayoutVerticalScrollBar( iScrollBar, Rect(), sbLayout );
        TAknLayoutRect sbLayoutRect;
        sbLayoutRect.LayoutRect( Rect(), sbLayout );
        }
    if ( iBgContext )
        {
        iBgContext->SetRect( Parent()->Rect() );
        iBgContext->SetParentPos( Parent()->PositionRelativeToScreen() );
        }        

    // Setup clipping region
    iClipping->Clear();
    iClipping->AddRect( Rect() );
    
    // clip text control
    iClipping->SubRect( iTextView->ViewRect() );
 
    // clip scrollbar
    if ( iScrollBar->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan)
        {    
		//fix for the tsw bug id:EKYT-7B68G9 //start.
        TAknWindowLineLayout sbLayout( AknLayoutScalable_Apps::scroll_pane_cp07( 3 ) );
        AknLayoutUtils::LayoutVerticalScrollBar( iScrollBar, Rect(), sbLayout );
        TAknLayoutRect sbLayoutRect;
        sbLayoutRect.LayoutRect( Rect(), sbLayout );
        iClipping->SubRect( sbLayoutRect.Rect() );
		//fix for the tsw bug id:EKYT-7B68G9 //end.
        }
    
    // and update view
    TRAPD( err, ScrollLinesL( 0 );
                UpdateViewL() );
    if( err != KErrNone )
        {
        CActiveScheduler::Current()->Error( err );
        }

    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SizeChanged()	end") );
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::HandleFormatChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::HandleFormatChangedL( TCursorSelection& aChanged, 
                                               TBool aPreserveFocus /* = EFalse */ )
    {
	if (iSmileyDialogLaunched)
		return;

    // rich text has changed, inform viewer
    iTextView->SetSelectionVisibilityL( EFalse );
    
    if( aPreserveFocus )
        {
        // Preserve focus
        TCursorSelection sel = iTextView->Selection();
        // This seems to be the only method which doesn't change focus.
        // --> so it's possible to preserve focus without flicker
        iTextView->HandleGlobalChangeNoRedrawL();
        ScrollVisibleL( sel, EFalse );
        DrawDeferred();

        }
    else
        {
        // Move focus along with changes
        iTextView->HandleRangeFormatChangeL( aChanged, ETrue );
		//ScrollVisibleL( aChanged, ETrue );
		UpdateViewL ();
        }
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::HandleInsertDeleteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::HandleInsertDeleteL( TCursorSelection& aChanged, 
                                              TInt aDeletedChars )
    {
	if (iSmileyDialogLaunched)
		return;

    // inform about insertion or deletion
    iTextView->SetSelectionVisibilityL( EFalse );
    iTextView->HandleInsertDeleteL( aChanged, aDeletedChars );
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::HandleAdditionL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::HandleAdditionL( TBool aFirst, 
                                          TCursorSelection& aSelection, 
                                          TBool aTop )
    {
    // inform about addition
    if( aFirst )
        {
        iTextView->FormatTextL();
        }
    else
        {
        iTextView->HandleAdditionalCharactersAtEndL();    
        } 

		// if smiley dialog is launched, then dont draw.
		if (iSmileyDialogLaunched)
			return;

	    ScrollVisibleL( aSelection, aTop );
	    DrawDeferred();   
		
    }    

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ScrollVisibleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::ScrollVisibleL( TCursorSelection& aSelection, TBool aTop )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ScrollVisibleL()	start") ); 
	// if smiley dialog is launched, then dont draw.
	if (iSmileyDialogLaunched)
		return;
		

	TCursorSelection sel( aSelection );
    if( !ParagraphFits( sel.LowerPos() ) )
        {
        // selection doesn't fit to screen
        TInt pos( aTop ? aSelection.LowerPos() : aSelection.HigherPos() );
        sel.SetSelection( pos, pos );
        }

    // scroll selection to screen
    iTextView->SetSelectionVisibilityL( EFalse );
    iTextView->SetSelectionL( sel );
    
    // and update scroll bars
    UpdateScrollBarsL();    
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ScrollVisibleL()	end") );
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ScrollSelectionL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppTextViewControl::ScrollSelection()
    {
    return iTextView->Selection();    
    }
    
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ScrollLinesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::ScrollLinesL( TInt aAmount )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ScrollLinesL()	start")); 
    iLayout->ScrollLinesL( aAmount );
    UpdateScrollBarsL(); 
    DrawDeferred();
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ScrollLinesL()	end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::IsVisible
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppTextViewControl::IsVisible( TInt aPos )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::IsVisible()		start") ); 
    TTmDocPos pos( aPos, ETrue );
    TTmLineInfo lineInfo;

    if( !iLayout->PosInBand( pos, &lineInfo ) )
        {
        // current position outside
        return EFalse;
        }
    
    if( !ParagraphFits( aPos ) )
        {
        // paragraph doesn't fit to screen
        // extend scrolling one line up and down
                
        // iLineNumber starts from 0 and FirstCharOnLine assumes first line to be 1
        TInt lineNo( lineInfo.iLineNumber + 1 );
        
        // check previous line (if it exists)
        if( lineNo > 1 )
            {
            pos.iPos = iLayout->FirstCharOnLine( lineNo - 1 );
            
            if( !iLayout->PosInBand( pos ) )
                {
                // previous line outside
                return EFalse;
                }    
            }

        // check next line
        pos.iPos = iLayout->FirstCharOnLine( lineNo + 1 );
        
        return iLayout->PosInBand( pos );
        }
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::IsVisible()	end") );
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ParagraphFits
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppTextViewControl::ParagraphFits( TInt aPos )
    {
    return iLayout->ParagraphHeight( aPos ) <= iViewRect.Height();
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::RedrawL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::Redraw()
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::Redraw()	start" )); 
    DrawDeferred();
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::Redraw()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CIMCVAppTextViewControl::CountComponentControls() const
    {
    if (iScrollBar && iScrollBar->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan)
        {    
        return 1;
        }
    else
        {
        return 0;
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CIMCVAppTextViewControl::ComponentControl(TInt aIndex) const
    {
    if (aIndex == 0 && iScrollBar && 
    	iScrollBar->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan)
        {    
        return iScrollBar->VerticalScrollBar();
        }
    else
        {
        return NULL;
        }    
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::SetPenBehaviour
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::SetPenBehaviour( TPenBehaviour aPen )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SetPenBehaviour()	start" )); 
    iPenType = aPen;
	IM_CV_LOGS(TXT("CIMCVAppTextViewControl::SetPenBehaviour()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::PenBehaviour
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MIMCVAppTextView::TPenBehaviour CIMCVAppTextViewControl::PenBehaviour()
    {
    return iPenType;
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::ProcessStylusEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::ProcessStylusEventL( 
    const TPointerEvent& /*aPointerEvent*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ProcessStylusEventL()	start" )); 
    // nothing to do. Let derived classes to handle text selection.
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::ProcessStylusEventL()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppTextViewControl::HandleGlobalChangeNoRedrawL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppTextViewControl::HandleGlobalChangeNoRedrawL(
    TViewYPosQualifier aYPosQualifier /*= TViewYPosQualifier()*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandleGlobalChangeNoRedrawL()	start" )); 
    iTextView->HandleGlobalChangeNoRedrawL( aYPosQualifier );
    IM_CV_LOGS(TXT("CIMCVAppTextViewControl::HandleGlobalChangeNoRedrawL()	end" ));
    }
    

//  End of File
