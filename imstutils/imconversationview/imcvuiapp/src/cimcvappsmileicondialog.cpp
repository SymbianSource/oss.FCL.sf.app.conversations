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
* Description:  Dialog for selecting smile icon.
*
*/




// INCLUDE FILES

#include "cimcvappsmileicondialog.h"
#include "cimcvappsmileicongrid.h"
#include "imcvappsmileutility.hrh"
#include <AknDef.h>
#include <aknappui.h>
#include <AknUtils.h>
#include <aknconsts.h>
#include <aknborders.h>
#include <AknsDrawUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlists.h> 
#include <AknUtils.h> 
#include "imcvlogger.h"
#include "imcvuiapputils.h"

#include "e32property.h"
// CONSTANTS

const TInt KAknSctCBaButtonDirections = 3; // bottom, right and left





// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::NewL
// (other items were commented in a header).
// ---------------------------------------------------------
CIMCVAppSmileIconDialog* CIMCVAppSmileIconDialog::NewL( 
            const RPointerArray<CGulIcon>& aIconArray,
			TInt& aSelectedIconId )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::NewL() start") );
    CIMCVAppSmileIconDialog* self =
        new (ELeave) CIMCVAppSmileIconDialog( aIconArray, aSelectedIconId );
	IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::NewL() end") );
    return self;
    }



// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::~CIMCVAppSmileIconDialog
// ---------------------------------------------------------
//
CIMCVAppSmileIconDialog::~CIMCVAppSmileIconDialog()
    {
	
    }


// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::CIMCVAppSmileIconDialog
// (other items were commented in a header).
// ---------------------------------------------------------
//
CIMCVAppSmileIconDialog::CIMCVAppSmileIconDialog( 
                            const RPointerArray<CGulIcon>& aIconArray,
							TInt& aSelectedIconId )
:iIconArray( aIconArray ), iIconId( aSelectedIconId )
    {
    
    }



// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::OfferKeyEventL
// Handle key event.
// If key code is other than OK and arrow key, no action.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CIMCVAppSmileIconDialog::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::OfferKeyEventL() start") );
    TKeyResponse response( EKeyWasConsumed );
    switch( aKeyEvent.iCode )
        {
		// fall through
        case EKeyLeftArrow:
        case EKeyRightArrow:
        case EKeyUpArrow:
        case EKeyDownArrow:
            {
            CIMCVAppSmileIconGrid* grid =
				static_cast< CIMCVAppSmileIconGrid*>(
				Control( EIMIdSelectSmileGrid ) );
            grid->MoveCursor( aKeyEvent.iCode );
            break;
            }
        case EKeyEscape: // handle dialog shutdown.
            {
			TryExitL( EAknSoftkeyExit );
            return EKeyWasNotConsumed;
            }
        default:
            {
            response = CAknDialog::OfferKeyEventL(aKeyEvent, aType);
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::OfferKeyEventL() end") );
    return response;
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::OkToExitL
// Called when CBA-key or OK-key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconDialog::HandleResourceChange( TInt aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::HandleResourceChange() start") );
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetLayout();
        }
	CAknDialog::HandleResourceChange( aType );
	IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::HandleResourceChange() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::OkToExitL
// Called when CBA-key or OK-key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppSmileIconDialog::OkToExitL( TInt aButtonId )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::OkToExitL() start") );
    if( aButtonId == EAknSoftkeySelect ||
        aButtonId == EEikBidOk )
        {
		CIMCVAppSmileIconGrid* grid =
			static_cast< CIMCVAppSmileIconGrid*>(
			Control( EIMIdSelectSmileGrid ) );
		iIconId = grid->SelectedBitmapId();
        }
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::OkToExitL() end") );
    return ETrue;
    }



// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::PreLayoutDynInitL
// Prepare for drawing dialog.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconDialog::PreLayoutDynInitL()
    {
    SetLineNonFocusing( EIMIdSelectSmileLabel );
    EnableDragEvents();
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::SetSizeAndPosition
// Control size is set. This is called just after PreLayoutDynInitL().
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconDialog::SetSizeAndPosition(const TSize& /*aSize*/)
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::SetSizeAndPosition() start") );
	SetLayout();
	IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::SetSizeAndPosition() end") );
    }



// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::CalculatePopupVariety
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CIMCVAppSmileIconDialog::CalculatePopupVariety()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::CalculatePopupVariety() start") );

    TAknLayoutScalableParameterLimits smileyDialogVariety = 
    	AknLayoutScalable_Avkon::popup_grid_graphic_window_ParamLimits();
    TInt maxVariety = smileyDialogVariety.LastVariety();

    // CBA location affects popup layout
    AknLayoutUtils::TAknCbaLocation cbaLocation = AknLayoutUtils::CbaLocation();

    // bottom variety
    TInt maxVarietyOffset = 0;
    TInt varietyOffset = maxVariety + 1;
    // right variety
    if (cbaLocation == AknLayoutUtils::EAknCbaLocationRight)
        {
    	varietyOffset = (maxVariety + 1)/KAknSctCBaButtonDirections;
    	maxVarietyOffset = varietyOffset;
        }

    // left variety
    else if (cbaLocation == AknLayoutUtils::EAknCbaLocationLeft)
        {
    	varietyOffset = (maxVariety + 1)/KAknSctCBaButtonDirections;
    	maxVarietyOffset = varietyOffset + varietyOffset;
        }
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::CalculatePopupVariety() end") );
	
	if ((maxVarietyOffset + varietyOffset)>maxVariety)
		return maxVariety;
	else
    	return maxVarietyOffset + varietyOffset;
	
    }



// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::SetLayout
// Set dialog layout.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconDialog::SetLayout()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::SetLayout() start"));
    iIsMirrored = AknLayoutUtils::LayoutMirrored();
    TRect clientRect; 
    //check LAF docs for constants
    //AknLayoutUtils::LayoutMetricsRect
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EApplicationWindow,
            clientRect);
    CIMCVAppSmileIconGrid* grid =

    static_cast<CIMCVAppSmileIconGrid*> (

    Control(EIMIdSelectSmileGrid));

    // Select correct popup layout

    TInt varietyNumber = CalculatePopupVariety();
    TRect rect = grid->GetFirstCellRect();

    TInt iconCount = iIconArray.Count();
    // calculating the number of rows required by the icon grid.

    TInt viewableWidth = 0;
    
    for (TInt i = varietyNumber; i >= 0; i--)
        {

        TAknWindowLineLayout popupGridDialogLayout(
                AknLayoutScalable_Avkon::popup_grid_graphic_window(i)); 

        TAknLayoutRect dialogRect;

        dialogRect.LayoutRect(clientRect, popupGridDialogLayout);
        
        viewableWidth = CheckDialog(dialogRect, rect);
        if (viewableWidth)
            {
            varietyNumber = i;
            break;
            }
        }
    if (viewableWidth)
        {
        TAknWindowLineLayout popupGridDialogLayout(
                AknLayoutScalable_Avkon::popup_grid_graphic_window(
                        varietyNumber));
        TAknLayoutRect dialogRect;
        dialogRect.LayoutRect(clientRect, popupGridDialogLayout);
        grid->SetViewableWindowWidth(viewableWidth);
        SetRect(dialogRect.Rect());
        }
    // If there is no suitable solution for the smiley dialog layout
    else
        {
        // No variety of the popup_grid_graphic_window has provided sufficient area
        // for the grid display. Hence selecting 0 variety for displaying the grid
        // because it has the greatest area.
        varietyNumber = 0;
        TAknWindowLineLayout popupGridDialogLayout(
                AknLayoutScalable_Avkon::popup_grid_graphic_window(
                        varietyNumber));

        TAknLayoutRect dialogRect;

        dialogRect.LayoutRect(clientRect, popupGridDialogLayout);

        if (!iIsMirrored)
            {
            // Right Margin of the top left (top right) icon's X
            TInt leftMargin = rect.iTl.iX;

            // Use the max viewable width
            viewableWidth = dialogRect.Rect().iBr.iX
                    - dialogRect.Rect().iTl.iX - leftMargin;
            }
        else
            {
            // Left Margin of the top left (top right) icon's X
            TInt rightMargin = rect.iBr.iX;

            // Use the max viewable width
            viewableWidth = dialogRect.Rect().iBr.iX
                    - dialogRect.Rect().iTl.iX + rightMargin;
            }

        grid->SetViewableWindowWidth(viewableWidth);

        if (!iIsMirrored)
            SetRect(dialogRect.Rect());
        // When the Hebrew is the language as well as the Arb languages
        // The align is not right
        else
            {
            TRect mirroredDialogRect;

            // Align to right.
            mirroredDialogRect = dialogRect.Rect();

            // Get the rect of the cba pane.
            TRect controlPaneRect;
            AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EControlPane,
                    controlPaneRect);

            TInt cbaWidth = controlPaneRect.iBr.iX - controlPaneRect.iTl.iX;

            mirroredDialogRect.iTl.iX -= cbaWidth;
            mirroredDialogRect.iBr.iX -= cbaWidth;

            SetRect(mirroredDialogRect);
            }
        }
    }



// ---------------------------------------------------------

// CIMCVAppSmileIconDialog::CheckDialog

// Check if Dialog is suitable to fit all smile icon

// (other items were commented in a header).

// ---------------------------------------------------------

TInt CIMCVAppSmileIconDialog::CheckDialog(TAknLayoutRect aDialogRect,TRect aRect)
		{
        // topMargin is the height of the margin which the smiley grid has 
        // above its top horizontal line and bottom horizontal line.
        TInt topMargin = aRect.Height();
        
        TRect dialogRect = aDialogRect.Rect();
        
        TInt vWidth = 0;
        TInt vheight = 0;
        TInt vArea = 0;
        vheight = dialogRect.iBr.iY - dialogRect.iTl.iY - 2*topMargin;
        if ( !iIsMirrored )
            {
            vWidth = dialogRect.iBr.iX - dialogRect.iTl.iX - 2*aRect.iTl.iX;;
            }
        else 
            {
            vWidth = dialogRect.iBr.iX - dialogRect.iTl.iX + 2*aRect.iBr.iX;
            }
		vArea = vWidth*vheight;
		
	    TInt iconCount = iIconArray.Count();

	    TInt areaOfIcon = iconCount * aRect.Width() * aRect.Height();
	    
	    if(areaOfIcon > vArea)
		    {
			return 0;
		    }
		else
			{
			return vWidth;
			}
		}


// ---------------------------------------------------------
// CIMCVAppSmileIconDialog::CreateCustomControlL
// Create CIMCVAppSmileIconGrid class in order to display
// icons in grid.
// This is called when dialog is created.
// (other items were commented in a header).
// ---------------------------------------------------------
//
SEikControlInfo CIMCVAppSmileIconDialog::CreateCustomControlL(
										TInt aControlType )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::SetLayout() start") );
    SEikControlInfo controlInfo;
    controlInfo.iControl = NULL;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iFlags = 0;
    if( aControlType == EIMCtrlSelectSmileGrid )
        {
        controlInfo.iControl = CIMCVAppSmileIconGrid::NewL( this, iIconArray );

        static_cast<CIMCVAppSmileIconGrid*>

            (controlInfo.iControl)->SetTapObserver( this, 0 );

        }
    IM_CV_LOGS(TXT("CIMCVAppSmileIconDialog::SetLayout() end") );
    return controlInfo;
    }



// ---------------------------------------------------------

// CIMCVAppSmileIconDialog::HandleDialogPageEventL

// (other items were commented in a header).

// ---------------------------------------------------------

//

void CIMCVAppSmileIconDialog::HandleDialogPageEventL( TInt aEventID )

    {
    if( IMCVUiAppUtils::PenEnabled() )
        {
        if ( iSmileyPointed &&
             aEventID == MEikDialogPageObserver::EDialogPageTapped )
            {        
            // Exit requested, exit with ok. 
            TryExitL( EAknSoftkeyOk );
            }
         
         //else keep dialog as it is do not exit.  
        }
    }

// ---------------------------------------------------------

// CIMCVAppSmileIconDialog::HandleTapEventL

// (other items were commented in a header).

// ---------------------------------------------------------

//

void CIMCVAppSmileIconDialog::HandleTapEventL( TTapEvent aEvent, 

                                          TUint /*aControlId*/ )

    {

    // User tapped on smiley.

    if( aEvent == MIMCVTapEventObserver::ESingleTap )

        {

        // Smiley was selected but we can't exit directly from here. 

        // Instead we put the flag on and wait for 

        // HandleDialogPageEventL.

        iSmileyPointed = ETrue;

        }

    }



// End of File
