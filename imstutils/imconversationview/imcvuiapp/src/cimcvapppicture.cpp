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
* Description:  Picture wrapper for icons and smileys
*
*/


// INCLUDE FILES
#include    "cimcvapppicture.h"
#include    "fbs.h"
#include	<gulicon.h>
#include 	"imcvlogger.h"

_LIT( KPanicText, "Smiley" );

// -----------------------------------------------------------------------------
// CIMCVAppPicture::CIMCVAppPicture
// -----------------------------------------------------------------------------
//    
CIMCVAppPicture::CIMCVAppPicture( MGraphicsDeviceMap& aMap, CGulIcon* aIcon,
                TInt aIndex /* = -1 */, TBool aExternalIcon /* = ETrue  */)
	: iIndex( aIndex ), 
	  iIcon( aIcon ), 
	  iExternalIcon( aExternalIcon ),
	  iGfxMap( aMap )
    {    
	__ASSERT_ALWAYS( iIcon, 
    User::Panic( KPanicText, 1 ) );
    
    SetTwips();
    }    

// -----------------------------------------------------------------------------
// CIMCVAppPicture::~CIMCVAppPicture
// -----------------------------------------------------------------------------
//    
CIMCVAppPicture::~CIMCVAppPicture()
    {
    if ( !iExternalIcon )
        {
        delete iIcon;
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppPicture::LineBreakPossible
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppPicture::LineBreakPossible( TUint /*aClass*/,
										   TBool /*aBeforePicture*/,
										   TBool /*aHaveSpaces*/ ) const
	{
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CIMCVAppPicture::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppPicture::Draw( CGraphicsContext& aGc,
					     const TPoint& aTopLeft,
					     const TRect& aClipRect,
					     MGraphicsDeviceMap* /*aMap*/ ) const
	{
	IM_CV_LOGS(TXT("CIMCVAppPicture::Draw() start") );
	TSize size( iIcon->Bitmap()->SizeInPixels() );
    CBitmapContext& bc = static_cast<CBitmapContext&>( aGc );
    
	
	aGc.Reset();
	aGc.SetClippingRect( aClipRect );	
    
    if( iIcon->Mask() )
        {
        bc.BitBltMasked( aTopLeft, iIcon->Bitmap(), size, iIcon->Mask(), ETrue );    
        }
    else
        {
        bc.BitBlt( aTopLeft, iIcon->Bitmap(), size );
        }
    IM_CV_LOGS(TXT("CIMCVAppPicture::Draw() end") );	
	}

// -----------------------------------------------------------------------------
// CIMCVAppPicture::ExternalizeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppPicture::ExternalizeL( RWriteStream& /*aStream*/ ) const
	{
	// nothing to be done here
	}

// -----------------------------------------------------------------------------
// CIMCVAppPicture::GetOriginalSizeInTwips(
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppPicture::GetOriginalSizeInTwips( TSize& aSize ) const
	{
	IM_CV_LOGS(TXT("CIMCVAppPicture::GetOriginalSizeInTwips() start") );
	if ( iIcon->Bitmap() )
		{
		aSize = iIcon->Bitmap()->SizeInTwips();
		
		if( aSize.iHeight == 0 || aSize.iWidth == 0 )
		    {
		    SetTwips();
		    aSize = iIcon->Bitmap()->SizeInTwips();
		    }
		}
	IM_CV_LOGS(TXT("CIMCVAppPicture::GetOriginalSizeInTwips() end") );
	}
	
// -----------------------------------------------------------------------------
// CIMCVAppPicture::Index
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppPicture::Index() const
	{
	return iIndex;
	}	

// -----------------------------------------------------------------------------
// CIMCVAppPicture::SetTwips
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppPicture::SetTwips() const
    {
    IM_CV_LOGS(TXT("CIMCVAppPicture::SetTwips() start") );
    TSize sizeP( iIcon->Bitmap()->SizeInPixels() );    
    TSize sizeT( iGfxMap.HorizontalPixelsToTwips( sizeP.iWidth ),
                 iGfxMap.VerticalPixelsToTwips( sizeP.iHeight ) );
    iIcon->Bitmap()->SetSizeInTwips( sizeT );
    
    IM_CV_LOGS(TXT("CIMCVAppPicture::SetTwips() end") );
    }
