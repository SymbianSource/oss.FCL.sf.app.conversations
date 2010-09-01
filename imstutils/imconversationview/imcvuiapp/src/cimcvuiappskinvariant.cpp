/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  skin variation  class implementation
*
*/


// INCLUDE FILES
#include    "cimcvuiappskinvariant.h"

#include    "cimcvappbitmapmapping.h"
#include	<aknappui.h>
#include	<AknsSkinInstance.h>	// skins 
#include	<AknsUtils.h>
#include	<AknsDrawUtils.h>
#include	<AknsBasicBackgroundControlContext.h>
#include 	<AknUtils.h> 
#include 	<gulicon.h>
// logs
#include "imcvlogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::CIMCVUiAppSkinVariant
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVUiAppSkinVariant::CIMCVUiAppSkinVariant()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVUiAppSkinVariant::ConstructL( )
    {
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::ConstructL() start") ) ;
	TRect mainPane( 0,0,0,0 );
	
	// Return value can be safely ignored.
	// from AknLayoutUtils::LayoutMetricsRect documentation:
	// 
	// Returns EFalse for status pane descendants if requested
	// layout component is not available in current layout. For
	// other components returns always ETrue (returned rectangle is
	// from layout definition).
	//
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, mainPane );

    iBgContext = CAknsBasicBackgroundControlContext::NewL( 
                        KAknsIIDQsnBgAreaMain, mainPane, EFalse );   
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::ConstructL() start") ) ;    
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVUiAppSkinVariant* CIMCVUiAppSkinVariant::NewL()
    {
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::NewL() start") );
    CIMCVUiAppSkinVariant* self = new( ELeave ) CIMCVUiAppSkinVariant();
    CleanupStack::PushL( self );
    self->ConstructL(  );
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::NewL() end") );
    return self;
    }
// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::~CIMCVUiAppSkinVariant()
// Destructor
// -----------------------------------------------------------------------------
//
CIMCVUiAppSkinVariant::~CIMCVUiAppSkinVariant()
    {
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::~CIMCVUiAppSkinVariant() start") );
	delete iBgContext;
	IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::~CIMCVUiAppSkinVariant() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::ClearChatListBoxBackGround
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiAppSkinVariant::ClearChatListBoxBackGround( CWindowGc& aGc, 
                                            const TRect& aUsedPortionOfViewRect,
    										MAknsControlContext* aContext )
	{
	IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::ClearChatListBoxBackGround() start") );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsDrawUtils::Background( skin, aContext, NULL, aGc, 
                               aUsedPortionOfViewRect );
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::ClearChatListBoxBackGround() end") );
 	}

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::MainPaneBackgroundContext
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknsControlContext* CIMCVUiAppSkinVariant::MainPaneBackgroundContext() const
	{
	return iBgContext;
	}

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::UpdateLayout
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiAppSkinVariant::UpdateLayout()
    {
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::UpdateLayout() start") );
	TRect screen( 0, 0, 0, 0 );
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screen );
    iBgContext->SetRect( screen );
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::UpdateLayout() end") );
    }
    
// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::DrawSkinnedEditor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVUiAppSkinVariant::DrawSkinnedEditor( const CCoeControl* aControl, 
										CWindowGc& aGc, 
										TRect& /*aParentRect*/, 
										TRect& aEditorRect, 
										TRect& aOutlineRect )
	{
	IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::DrawSkinnedEditor() start") );
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();

	// draw editor surroundings
	AknsDrawUtils::BackgroundBetweenRects( skin, MainPaneBackgroundContext(), 
	                                       aControl, aGc, aOutlineRect, 
	                                       aEditorRect );	
	
	TRect innerRect( aOutlineRect );
	//why 6 ? don't ask me. It was 3x3 for 176x208.
	innerRect.Shrink( TSize( 6, 6 ) );
	// draw editor frames
	IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::DrawSkinnedEditor() end") );
	return  AknsDrawUtils::DrawFrame( skin, aGc, aOutlineRect, innerRect,
            KAknsIIDQsnFrInput, KAknsIIDQsnFrInputCenter );

	}


// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::LoadBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CIMCVUiAppSkinVariant::LoadBitmapL( TInt aBitmapId, TInt aMaskId, 
                                const TDesC& aFullPath, TBool /*aOverrideBrand*/,
                                TRgb* /*aCustomColor*/ )
{
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::LoadBitmapL() start") );
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;

	MAknsSkinInstance* skin = AknsUtils::SkinInstance();	

	TAknsItemID itemId( KAknsIIDNone );
    TInt colorGroupId( 0 );
    
	MapFromBitmapIdToAknsItemId( aBitmapId, itemId, colorGroupId );
    
    if( colorGroupId != 0 )
		{
		
	    AknsUtils::CreateColorIconLC( skin, 
                itemId, KAknsIIDQsnIconColors, colorGroupId, 
                bitmap, mask, aFullPath, aBitmapId, aMaskId, KRgbBlack );
        }
    else
        {
        AknsUtils::CreateIconLC( skin, 
                    itemId, 
                    bitmap, mask, aFullPath, aBitmapId, aMaskId );    
        }

    CGulIcon* gulIcon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 ); // bitmap, mask
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::LoadBitmapL() end") );
    return gulIcon;
    }

// -----------------------------------------------------------------------------
// CIMCVUiAppSkinVariant::MapFromBitmapIdToAknsItemId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVUiAppSkinVariant::MapFromBitmapIdToAknsItemId( TInt aBitmapId, 
                                TAknsItemID& aItemId, TInt& aColorGroupId )
	{
	IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::MapFromBitmapIdToAknsItemId() start") );
    for( TInt i( 0 ); KCABitmaps[i].iBitmapId != KErrNotFound; i++ )
        {
        if( KCABitmaps[ i ].iBitmapId == aBitmapId )
            {
            // found it
            aItemId = *(KCABitmaps[ i ].iItemId);
            aColorGroupId = KCABitmaps[ i ].iColorGroupId;
            return;
            }
        }
    
    IM_CV_LOGS(TXT("CIMCVUiAppSkinVariant::MapFromBitmapIdToAknsItemId() start") );
    //__ASSERT_DEBUG( EFalse );
    }

//  End of File
