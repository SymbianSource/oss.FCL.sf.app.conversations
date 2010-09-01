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
* Description:  custom control class
*
*/




// INCLUDE FILES
#include    "cimcvappcustomdraw.h"

#include <AknsSkinInstance.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h>
#include <eikenv.h>
#include <eikappui.h>

#include	<AknsDrawUtils.h>
#include	<AknsBasicBackgroundControlContext.h>
#include	<biditext.h>	// A/H
#include	<AknBidiTextUtils.h>
#include	<aknlayout.cdl.h> //cdl 
#include	<applayout.cdl.h> //cdl 
#include    <AknUtils.h>
#include 	"imcvlogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppCustomDraw::CIMCVAppCustomDraw
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppCustomDraw::CIMCVAppCustomDraw( TRect aViewRect )
    :iViewRect( aViewRect )
    {
    }

// Destructor
CIMCVAppCustomDraw::~CIMCVAppCustomDraw()
    {

    }

// -----------------------------------------------------------------------------
// CIMCVAppCustomDraw::SetRect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppCustomDraw::SetRect( const TRect& aRect )
    {
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::SetRect()	start" ));
    iViewRect = aRect;
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::SetRect()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppCustomDraw::DrawBackground
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppCustomDraw::DrawBackground( const TParam& aParam,
									const TRgb& aBackground,
									TRect& aDrawn ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::DrawBackground()	start" ));
    if( aBackground != KRgbWhite || !iBgContext )
        {
        // draw background with color
        aParam.iGc.SetPenStyle( CGraphicsContext::ENullPen );
        aParam.iGc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        aParam.iGc.SetBrushColor( aBackground );
        aParam.iGc.DrawRect( aParam.iDrawRect );      
        }
    // if background is white -> we draw the skinned background
    else
        {
		//Draw the skin background
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
		AknsDrawUtils::Background( skin, 
		                           iBgContext, 
		                           iParent,
		                           static_cast<CWindowGc&>(aParam.iGc),
		                           aParam.iDrawRect);

        }
    aDrawn = aParam.iDrawRect;
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::DrawBackground()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppCustomDraw::SetBackgroundContext
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppCustomDraw::SetBackgroundContext( CAknsBasicBackgroundControlContext* aBgContext,
                                          CCoeControl* aParent )
    {
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::SetBackgroundContext()	start" ));
    iBgContext = aBgContext;
    iParent = aParent;
    IM_CV_LOGS(TXT("CIMCVAppCustomDraw::SetBackgroundContext()	end" ));
    }

//  End of File
