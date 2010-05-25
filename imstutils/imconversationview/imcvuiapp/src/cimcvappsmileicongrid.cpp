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
* Description:  Grid for smile icons
*
*/


// INCLUDE FILES
#include "cimcvappsmileicongrid.h"
#include "imcvappsmileutility.hrh"
#include <AknUtils.h>
#include <AknIconUtils.h>
#include <AknLayout.lag>
#include <gulicon.h>
#include <eikdialg.h>
#include <AknsDrawUtils.h>
#include <aknappui.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknDef.h>
#include "imcvlogger.h"
#include "imcvuiapputils.h"
#include "mimcvtapeventobserver.h"


// CONSTANTS
// The (KTopLeft_x,KTopLeft_y) and (KBottomRight_x,KBottomRight_y) are the co ordinates of the 
// topleft and bottomright points of the first cell in the grid.
// These co-ordinates to draw the grid lines and the logic to draw the grid lines is based on the topleft 
// and bottomright points of the first cell in the grid.
#define KTopLeft_x 56
#define KTopLeft_y 55
#define KBottomRight_x 105
#define KBottomRight_y 104


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// Two-phased constructor.
CIMCVAppSmileIconGrid* CIMCVAppSmileIconGrid::NewL( CEikDialog* aParent,
							    const RPointerArray<CGulIcon>& aIconArray )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::NewL() start") );
    CIMCVAppSmileIconGrid* self = new (ELeave) CIMCVAppSmileIconGrid( aParent, 
                                                            aIconArray );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::NewL() end") );
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::~CIMCVAppSmileIconGrid
// -----------------------------------------------------------------------------
CIMCVAppSmileIconGrid::~CIMCVAppSmileIconGrid()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::CIMCVAppSmileIconGrid
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// C++ constructor can NOT contain any code, that
// might leave.
//
CIMCVAppSmileIconGrid::CIMCVAppSmileIconGrid( CEikDialog* aParent,
                                    const RPointerArray<CGulIcon>& aIconArray )
:iParent( aParent ), iIconArray( aIconArray )
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
// default constructor can leave.
void CIMCVAppSmileIconGrid::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::ConstructL() start") );
	iIconCount = iIconArray.Count();
	
	iIsMirrored = ETrue ;
    SetContainerWindowL( *iParent );
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::ConstructL() end") );
    }


// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::MoveCursor
// Move cursor and redraw highlight.
// Called when arrow-key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconGrid::MoveCursor( TInt aKeyCode )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::MoveCursor() start") );
    iPrevCursorPos = iCursorPos;

    switch( aKeyCode )
        {
        case EKeyLeftArrow:
            {
			if( AknLayoutUtils::LayoutMirrored() )
				{
				iCursorPos++;
				if( iCursorPos >= iIconCount )
					{
					iCursorPos = 0;
					}
				}
			else
				{
				iCursorPos--;
				if( iCursorPos < 0 )
					{
					iCursorPos = iIconCount - 1;
					}
				}
            break;
            }
        case EKeyRightArrow:
            {
			if( AknLayoutUtils::LayoutMirrored() )
				{
				iCursorPos--;
				if( iCursorPos < 0 )
					{
					iCursorPos = iIconCount - 1;
					}
				}
			else
				{
				iCursorPos++;
				if( iCursorPos >= iIconCount )
					{
					iCursorPos = 0;
					}
				}
            break;
            }
        case EKeyUpArrow:
            {
			if( iCursorPos < iMaxColumns )
                {
                iCursorPos += ( iRowCount * iMaxColumns ) - 1;

                if( iCursorPos >= iIconCount )
                    {
                    iCursorPos -= iMaxColumns;
                    }

				if( iCursorPos < 0 )
					{
					iCursorPos = iIconCount - 1;
					}
				}
			else
				{
                iCursorPos -= iMaxColumns;
				}
            break;
            }
        case EKeyDownArrow:
            {
            if( iCursorPos < ( iIconCount - iMaxColumns ) )
                {
                iCursorPos += iMaxColumns;
                }
			else
				{
				iCursorPos %= iMaxColumns;
				iCursorPos++;
				if( iCursorPos >= iMaxColumns ||
					iCursorPos >= iIconCount )
					{
					iCursorPos = 0;
					}
				}
            break;
            }
        default:
            {
            return;
            }
        }

    DrawDeferred();
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::MoveCursor() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::SelectedBitmapId
// Called when OK-key or Select-softkey is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppSmileIconGrid::SelectedBitmapId()
    {
    return iCursorPos;
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::HeightInRows
// Return row count of grid.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppSmileIconGrid::HeightInRows()
    {
    return iRowCount;
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::SetLayout
// Set layout of identifier grid.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconGrid::SetLayout()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::SetLayout() start"));
    iIsMirrored = AknLayoutUtils::LayoutMirrored();

    // popup_grid_graphic_window (Parent)
    TRect parentRect(iParent->Rect());

    // listscroll_popup_graphic_pane (this compoment)
    TAknLayoutRect listLayoutRect;
    listLayoutRect.LayoutRect(parentRect,
            AknLayoutScalable_Avkon::listscroll_popup_graphic_pane(0));

    // grid_graphic_popup_pane
    TAknLayoutRect gridLayoutRect;
    gridLayoutRect.LayoutRect(listLayoutRect.Rect(),
            AknLayoutScalable_Avkon::grid_graphic_popup_pane(0));

    // cell_graphic_popup_pane (upper left cell)
    TAknLayoutRect oneCellRect;
    oneCellRect.LayoutRect(gridLayoutRect.Rect(),
            AknLayoutScalable_Avkon::cell_graphic_popup_pane(0, 0, 0));

    iFirstCell = oneCellRect.Rect();
    iCellWidth = iFirstCell.Width();
    iCellHeight = iFirstCell.Height();

    // cell_graphic_popup_pane_g1 (icon size)
    TAknLayoutRect myIconRect;
    myIconRect.LayoutRect(iFirstCell,
            AknLayoutScalable_Avkon::cell_graphic_popup_pane_g1(0));
    iIconSize = myIconRect.Rect().Size();
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::SetLayout() end") );
    }
   
// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::MinimumSize
// Return minimum size to use the control.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSize CIMCVAppSmileIconGrid::MinimumSize()
    {       
    TAknLayoutRect gridRect;
    gridRect.LayoutRect(
        iParent->Rect(),
        AknLayoutScalable_Avkon::listscroll_popup_graphic_pane() );
    return gridRect.Rect().Size();
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::SizeChanged
// Control rectangle is set.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconGrid::SizeChanged()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::SizeChanged() start") );
    SetLayout();
    
    TInt iconCount( iIconArray.Count() );
	for ( TInt i = 0; i < iconCount; i++  )
	    {
		AknIconUtils::SetSize( iIconArray[i]->Bitmap(), 
		                       iIconSize );
	    }
	IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::SizeChanged() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::HandleResourceChange
// Notifier for changing layout
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CIMCVAppSmileIconGrid::HandleResourceChange( TInt aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::HandleResourceChange() start") );
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetLayout();
        }
    else
        {
        CCoeControl::HandleResourceChange( aType );
        }
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::HandleResourceChange() end") );
    }


// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::Draw
// Drawing control.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconGrid::Draw( const TRect& /* aRect */ ) const
	{
	IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::Draw() start") );
	CWindowGc& gc = SystemGc();
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );

	if( !iDragEvent )
		{
		// Draw the grid
		gc.SetPenStyle(CGraphicsContext::ESolidPen);    
		gc.SetBrushStyle(CGraphicsContext::ENullBrush);
		gc.SetPenSize(TSize(1,1));    
		gc.SetPenColor(AKN_LAF_COLOR(215));

		// For some reason the dialog does not draw it's background
		// completely, so we'll have to do it here.
		// This should work with bgRect = Rect(), but it doesn't.
		// Change this if you know how it should be done.
		TRect bgRect = iParent->Rect();
		bgRect.iTl.iY = Rect().iTl.iY;
		// ---

		TBool skins = AknsDrawUtils::Background( skin, cc, this, gc, bgRect );

		if ( !skins )
			{
			TRgb color = AKN_LAF_COLOR( 0 );
            AknsUtils::GetCachedColor(skin,color,KAknsIIDQsnIconColors,EAknsCIQsnIconColorsCG1); 
			gc.SetBrushColor(color);
			gc.Clear( bgRect );
			}

    	TInt lastRowIconsCount = iIconCount % iMaxColumns;
		if( lastRowIconsCount == 0 && iIconCount > 0 )
			{
			// Last row is full
    	    lastRowIconsCount = iMaxColumns;
			}

		TInt i( 0 );
		TPoint cellLeftTop( KTopLeft_x,KTopLeft_y );
		TPoint cellBottomRight( KBottomRight_x,KBottomRight_y );
		if(!iIsMirrored)	
			{
			// Draw horizontal lines
			for( i = 0; i <= iRowCount; ++i )
				{
				TPoint startPoint( cellLeftTop );
				TPoint endPoint( cellLeftTop );
				startPoint.iY += i * iCellHeight;
				endPoint.iY += i * iCellHeight;
				endPoint.iX += ( ( i == iRowCount ) || ( i == 0 && iRowCount == 1 )
				    ? ( lastRowIconsCount ) 
    		    * iCellWidth : iMaxColumns * iCellWidth );
				// add 1 pixel to remove the gap from bottom right corners
				++endPoint.iX;
				gc.DrawLine( startPoint, endPoint );
				}

			// Draw vertical lines
			for( i = 0; i <= iMaxColumns; ++i )
				{
				TPoint startPoint( cellLeftTop );
				TPoint endPoint( cellLeftTop );
				startPoint.iX += i * iCellWidth;
				endPoint.iX += i * iCellWidth;
				endPoint.iY += ( i <= lastRowIconsCount ? 
				    iCellHeight * iRowCount : iCellHeight * ( iRowCount - 1  ) );
				gc.DrawLine( startPoint, endPoint );
				}
			}
		else
			{
			// Draw horizontal lines
			for( i = 0; i <= iRowCount; ++i )
				{
				TPoint startPoint( cellLeftTop );
				TPoint endPoint( cellLeftTop );
				startPoint.iY += i * iCellHeight;
				endPoint.iY += i * iCellHeight;
				endPoint.iX += ( ( i == iRowCount ) || ( i == 0 && iRowCount == 1 )
								    ? ( lastRowIconsCount ) 
				    		    * iCellWidth : iMaxColumns * iCellWidth );
				// subtract one pixel to remove the gap from bottom left corners
				--endPoint.iX;
				gc.DrawLine( startPoint, endPoint );
				}

			// Draw vertical lines
			for( i = 0; i <= iMaxColumns; ++i )
				{
				TPoint startPoint( cellLeftTop );
				TPoint endPoint( cellLeftTop );
				startPoint.iX += i * iCellWidth;
				endPoint.iX += i * iCellWidth;
				endPoint.iY += ( i <= lastRowIconsCount ? 
				    iCellHeight * iRowCount : iCellHeight * ( iRowCount - 1  ) );
				gc.DrawLine( startPoint, endPoint );
				}

			}

		// Draw icons
		for( i = 0; i < iIconCount; ++i )
			{
			DrawItem( gc, skins, skin, cc, i, i == iCursorPos );
			}
		}
	else
		{
		// Only selection changed, highlight new pos
		DrawItem( gc, cc != NULL, skin, cc, iCursorPos, ETrue );        
		// And clear old
		DrawItem( gc, cc != NULL, skin, cc, iPrevCursorPos, EFalse );
		}
	IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::Draw() end") );    
	}

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::DrawItem
// (other items were commented in a header).
// ---------------------------------------------------------
void CIMCVAppSmileIconGrid::DrawItem( CWindowGc& aGc, 
                       TBool aSkinEnabled,
                       MAknsSkinInstance* aSkin, 
                       MAknsControlContext* aSkinCc,
                       TInt aIndex, TBool aSelected ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::DrawItem() start") );
    //lets count current cell
    TPoint cellLeftTop( KTopLeft_x,KTopLeft_y );
    TPoint cellBottomRight( KBottomRight_x,KBottomRight_y );

    TRect myRect;
    myRect.SetRect( cellLeftTop, cellBottomRight );
    TPoint offset;
    
    if(!iIsMirrored)
    {
    	offset.iX = ( aIndex % iMaxColumns ) * iCellWidth;	
    }
    else
    {
    	offset.iX = ( aIndex % iMaxColumns ) * iCellWidth;
    }
    
    offset.iY = aIndex / iMaxColumns * iCellHeight;
    
    myRect.Move( offset );

    // don't draw bg/highlight over borderlines
    TRect myHighLightRect = myRect;
    myHighLightRect.iTl.iX++;
    myHighLightRect.iTl.iY++;
    
    if( aSelected )
        {
        // HIGHLIGHT
        if( !aSkinEnabled )
            {
            aGc.SetBrushStyle( CGraphicsContext::ESolidBrush );
            }            

        TRgb color = AKN_LAF_COLOR(210);
        AknsUtils::GetCachedColor( aSkin, color, KAknsIIDQsnIconColors,
                                   EAknsCIQsnComponentColorsCG17 ); 
        aGc.SetBrushColor( color );
        aGc.Clear( myHighLightRect );                
        }
    else
        {
        TRgb color = AKN_LAF_COLOR( 0 );
        AknsUtils::GetCachedColor( aSkin, color, KAknsIIDQsnIconColors,
                                   EAknsCIQsnIconColorsCG1 );
        aGc.SetBrushColor( color );
        if ( aSkinEnabled )
            {
            AknsDrawUtils::Background( aSkin, aSkinCc, aGc, myHighLightRect );                    
            }
        else
            {
            aGc.DrawRect( myHighLightRect );    
            }
        }
    

    TAknWindowLineLayout myIconLayout( AknLayoutScalable_Avkon::cell_graphic_popup_pane_g1(0) );
    
    TAknLayoutRect myIconRect;
    myIconRect.LayoutRect( myRect, myIconLayout );

	if( aIndex < iIconArray.Count() )
	    {
		myIconRect.DrawImage( aGc,
			iIconArray[ aIndex ]->Bitmap(),
			iIconArray[ aIndex ]->Mask() );
	    }
	IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::DrawItem() end") );    
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconGrid::HandlePointerEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconGrid::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::HandlePointerEventL() start") );
    if( !IMCVUiAppUtils::PenEnabled() )
        {
        // Ignore event
        //return;
        }
        
    TInt oldSelection = iCursorPos;
    TPoint hitPos = aPointerEvent.iPosition;
    
    // Convert XY position to linear cursor position    
    TPoint cellLeftTop( KTopLeft_x,KTopLeft_y );
    hitPos -= cellLeftTop;
    
    TInt xPos = hitPos.iX / iCellWidth;
    TInt yPos = hitPos.iY / iCellHeight;
    TInt newSelection = yPos * iMaxColumns + xPos;
    
    // Check that the selection is inside the grid
    TBool validItem = xPos <= iMaxColumns - 1 &&
                      Rng( 0, newSelection, iIconCount - 1 ) && 
                      hitPos.iX >= 0 && 
                      hitPos.iY >= 0;
    if( !validItem )
        {
        // not valid point --> ignore event
        return;
        }
        
    // Only update if something new was selected
    if( newSelection != oldSelection )
        {
        // New valid item
        iPrevCursorPos = iCursorPos;
        iCursorPos = newSelection;
        iDragEvent = ETrue;
        DrawDeferred();
        iDragEvent = EFalse;
        }
    
    // Notify observer
    if( iTapObserver &&
        aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        // Smiley was tapped
        iTapObserver->HandleTapEventL( MIMCVTapEventObserver::ESingleTap, 
                                       iTapControlId );
        }

    if( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        // enable dragging when button1 is down
        EnableDragEvents();
       // single tap has to insert the smiley.
       //  this code is not required
       // Window().SetPointerGrab( ETrue );
        
        // and make sure that we get the dragging events
     //   ClaimPointerGrab( ETrue );
        }
    
    CCoeControl::HandlePointerEventL( aPointerEvent );
    IM_CV_LOGS(TXT("CIMCVAppSmileIconGrid::HandlePointerEventL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::SetTapObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileIconGrid::SetTapObserver( MIMCVTapEventObserver* aObserver, 
                                       TUint aId )
    {
    iTapObserver = aObserver;
    iTapControlId = aId;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::SetViewableWindowWidth
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileIconGrid::SetViewableWindowWidth(TInt aViewableWidth)
{
    TRect parentRect( iParent->Rect() );

    // listscroll_popup_graphic_pane (this compoment)
    TAknLayoutRect listLayoutRect;
    listLayoutRect.LayoutRect( 
        parentRect, 
        AknLayoutScalable_Avkon::listscroll_popup_graphic_pane() );
	
    // grid_graphic_popup_pane
    TAknLayoutRect gridLayoutRect;
    gridLayoutRect.LayoutRect( 
        listLayoutRect.Rect(), 
        AknLayoutScalable_Avkon::grid_graphic_popup_pane( 0 ) );

	// cell_graphic_popup_pane (upper left cell)
	TAknLayoutRect oneCellRect;
	oneCellRect.LayoutRect( 
        gridLayoutRect.Rect(), 
        AknLayoutScalable_Avkon::cell_graphic_popup_pane( 0, 0, 0 ) );
	
	TInt cellWidth = oneCellRect.Rect().Width();
	
	iMaxColumns = aViewableWidth/cellWidth;    
	
	iRowCount = iIconCount / iMaxColumns;
	
    if (iIconCount % iMaxColumns)
        {
        iRowCount++;
        }
	
}

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconGrid::GetFirstCellRect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TRect CIMCVAppSmileIconGrid::GetFirstCellRect()
	{
	// popup_grid_graphic_window (Parent)
    TRect parentRect( iParent->Rect() );

    // listscroll_popup_graphic_pane (this compoment)
    TAknLayoutRect listLayoutRect;
    listLayoutRect.LayoutRect( 
        parentRect, 
        AknLayoutScalable_Avkon::listscroll_popup_graphic_pane() );
	
    // grid_graphic_popup_pane
    TAknLayoutRect gridLayoutRect;
    gridLayoutRect.LayoutRect( 
        listLayoutRect.Rect(), 
        AknLayoutScalable_Avkon::grid_graphic_popup_pane( 0 ) );

	// cell_graphic_popup_pane (upper left cell)
	TAknLayoutRect oneCellRect;
	oneCellRect.LayoutRect( 
        gridLayoutRect.Rect(), 
        AknLayoutScalable_Avkon::cell_graphic_popup_pane( 0, 0, 0 ) );
	
	return oneCellRect.Rect();
	//return iFirstCell;
	
	}
// End of File
