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
* Description:  layout info class
*
*/



// INCLUDE FILES
#include	"cimcvapplayoutinfo.h"
#include	<AknUtils.h>
#include	<applayout.cdl.h> //cdl
#include	<f32file.h>
#include    <aknlayoutscalable_apps.cdl.h>
#include 	"imcvlogger.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CIMCVAppLayoutInfo::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppLayoutInfo* CIMCVAppLayoutInfo::NewL( TChatListBoxLayout::TLayout aLayoutID )
    {
    IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::NewL()	start" ));
    CIMCVAppLayoutInfo* self = new (ELeave) CIMCVAppLayoutInfo( aLayoutID );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::NewL()	end" ));
    return self;
  
    }

// -----------------------------------------------------------------------------
// CIMCVAppLayoutInfo::~CIMCVAppLayoutInfo
// Destructor
// -----------------------------------------------------------------------------
//
CIMCVAppLayoutInfo::~CIMCVAppLayoutInfo()
    {
	if ( iReadingPane )
		{
		iReadingPane->Reset();
		delete iReadingPane;
		}

	if ( iWrittingPane )
		{
		iWrittingPane->Reset();
		delete iWrittingPane;
		}

	if ( iReadingField )
		{
		iReadingField->Reset();
		delete iReadingField;
		}

	if ( iReadingPaneTextsLine1 )
		{
		iReadingPaneTextsLine1->Reset();
		delete iReadingPaneTextsLine1;
		}

	if ( iIMTextElements )
		{
		iIMTextElements->Reset();
		delete iIMTextElements;
		}

	if ( iWrittingPaneTextsLine )
		{
		iWrittingPaneTextsLine->Reset();
		delete iWrittingPaneTextsLine;
		}

	if ( iIMWritingFieldLine1 )
		{
		iIMWritingFieldLine1->Reset();
		delete iIMWritingFieldLine1;
		}

	if ( iIMWritingFieldLine3 )
		{
		iIMWritingFieldLine3->Reset();
		delete iIMWritingFieldLine3;
		}

	if ( iIMNaviPaneTextsLine1 )
		{
		iIMNaviPaneTextsLine1->Reset();
		delete iIMNaviPaneTextsLine1;
		}

	if ( iIMNaviPaneTextsLine2 )
		{
		iIMNaviPaneTextsLine2->Reset();
		delete iIMNaviPaneTextsLine2;
		}
    }

// -----------------------------------------------------------------------------
// CIMCVAppLayoutInfo::CIMCVAppLayoutInfo
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppLayoutInfo::CIMCVAppLayoutInfo( TChatListBoxLayout::TLayout aLayoutID ) :
	iLayoutID( aLayoutID )
    {
    IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::CIMCVAppLayoutInfo()	start" ));
    IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::CIMCVAppLayoutInfo()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppLayoutInfo::ConstructL
// Second phase constructor
// -----------------------------------------------------------------------------
//
void CIMCVAppLayoutInfo::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::ConstructL()	start" ));
	SetupMainPaneRect();
	FillLayoutL();
	LogLayoutL();
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::ConstructL()	end" ));
    }

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::LayoutChangedL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::LayoutChangedL( TInt /*aType = 0*/ )
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::LayoutChangedL()	start" ));
	FillLayoutL();

	TAknLayoutRect myLayoutRect;

	TAknLayoutRect myLine;

	//update MainPaneRect
	SetupMainPaneRect();

	TAknWindowLineLayout readingPane = im_reading_pane( iLayoutID );

	myLayoutRect.LayoutRect( iMainPaneRect, readingPane );

	//line height
	TAknWindowLineLayout lineLayout = im_reading_field( 0 );

	myLine.LayoutRect( myLayoutRect.Rect(), lineLayout );

	TAknTextLineLayout myMessageHeaderLayout =
		IM_reading_pane_texts_Line_1( 1 );
		
	TAknLayoutText myTextLayout;
	TAknLayoutText myTextLayout2;

	myTextLayout.LayoutText( myLine.Rect(), myMessageHeaderLayout);

	// IconSkip
	myMessageHeaderLayout = IM_reading_pane_texts_Line_1( 2 );

	myTextLayout2.LayoutText( myLine.Rect(), myMessageHeaderLayout);

	// MsgFont

	TAknTextLineLayout myMessageLayout = IM_reading_pane_texts_Line_2();

	//normal message text with indent
	myTextLayout.LayoutText( myLine.Rect(), myMessageLayout);

	//header with full width
	myMessageHeaderLayout = IM_reading_pane_texts_Line_1( 1 );
	myTextLayout2.LayoutText( myLine.Rect(), myMessageHeaderLayout);

	
	// MaxNickWidth

	myMessageHeaderLayout = IM_reading_pane_texts_Line_1( 0 );

	myTextLayout.LayoutText( myLine.Rect(), myMessageHeaderLayout);

	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::LayoutChangedL()	end" ));
	}


// CIMCVAppLayoutInfo::MainPaneRect
// (other items were commented in a header).
// ---------------------------------------------------------
//
TRect CIMCVAppLayoutInfo::MainPaneRect()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::MainPaneRect()	start" ));
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::MainPaneRect()	end" ));
	return TRect( iMainPaneRect );
	}
// ---------------------------------------------------------
// CIMCVAppLayoutInfo::SetLayoutID
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::SetLayoutID( TChatListBoxLayout::TLayout aLayout )
	{
	iLayoutID = aLayout;
	TRAPD( err, LayoutChangedL() );
	if( err != KErrNone )
		{
		CActiveScheduler::Current()->Error( err );
		}
	}


// ---------------------------------------------------------
// CIMCVAppLayoutInfo::ScreenRect
// (other items were commented in a header).
// ---------------------------------------------------------
//
TRect CIMCVAppLayoutInfo::ScreenRect()
	{
	return iScreenRect;
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::SetEditorVisibility
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::SetEditorVisibility( TBool aVisible )
	{
	iEditorVisibility = aVisible;
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::EditorVisibility
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppLayoutInfo::EditorVisibility()
	{
	return iEditorVisibility;
	}

// CIMCVAppLayoutInfo::SetupMainPaneRect
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::SetupMainPaneRect()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::SetupMainPaneRect()	start" ));
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
		iMainPaneRect );
	_LIT( KMain, "MainPane");

	iMainPaneRect.Move( -iMainPaneRect.iTl.iX, -iMainPaneRect.iTl.iY );

	DPRect( KMain, iMainPaneRect );

	TRect myControlPane;
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPane,
		myControlPane );
	_LIT( KControlPane, "ControlPane");
	DPRect( KControlPane, myControlPane );
	//this is screen relative lets move back to origin.
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::SetupMainPaneRect()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::im_reading_pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::im_reading_pane(TInt aIndex_H)
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::im_reading_pane()	start" ));
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::im_reading_pane()	end" ));
	return (*iReadingPane)[ aIndex_H ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::im_reading_field
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::im_reading_field(TInt aIndex_t)
	{
	return (*iReadingField)[ aIndex_t ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_text_elements
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::IM_text_elements(TInt aLineIndex)
	{
	return (*iIMTextElements)[ aLineIndex ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_reading_pane_texts_Line_1
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknTextLineLayout CIMCVAppLayoutInfo::IM_reading_pane_texts_Line_1(TInt aCommon1)
	{
	return (*iReadingPaneTextsLine1)[ aCommon1 ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_reading_pane_texts_Line_2
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknTextLineLayout CIMCVAppLayoutInfo::IM_reading_pane_texts_Line_2()
	{
	return iReadingPaneTextsLine2;
	}

// CIMCVAppLayoutInfo::im_writing_pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::im_writing_pane( TInt aCommon1 )
	{
	return (*iWrittingPane)[ aCommon1 ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_writing_field_elements_Line_1
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::IM_writing_field_elements_Line_1(
		TInt aIndex_H )
	{
	return (*iIMWritingFieldLine1)[ aIndex_H ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_writing_field_elements_Line_2
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::IM_writing_field_elements_Line_2()
	{
	return iIMWritingFieldLine2;
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_writing_field_elements_Line_3
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknWindowLineLayout CIMCVAppLayoutInfo::IM_writing_field_elements_Line_3(
		TInt aIndex_H )
	{
	return (*iIMWritingFieldLine3)[ aIndex_H ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::Multiline_IM_writing_pane_texts_Line_1
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknMultiLineTextLayout CIMCVAppLayoutInfo::Multiline_IM_writing_pane_texts_Line_1(
		TInt aNumberOfLinesShown)
	{
	return (*iWrittingPaneTextsLine)[ aNumberOfLinesShown ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::IM_navi_pane_texts_Line_1
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAknTextLineLayout CIMCVAppLayoutInfo::IM_navi_pane_texts_Line_1(
		TInt aIndex_C, TInt /*aIndex_W*/)
	{
	return (*iIMNaviPaneTextsLine1)[ aIndex_C ];
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::LogLayout
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::LogLayoutL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::LogLayoutL()	start" ));
#ifdef _DEBUG

	_LIT( K1,"im_reading_pane( 0 ) ");
	DPLayout( K1, AppLayout::im_reading_pane( 0 ) );
	DPLayout( K1, im_reading_pane( 0 ) );
	_LIT( K2,"im_reading_pane( 1 ) ");
	DPLayout( K2, AppLayout::im_reading_pane( 1 ) );
	DPLayout( K2, im_reading_pane( 1 ) );
	_LIT( K3,"im_reading_pane( 2 ) ");
	DPLayout( K3, AppLayout::im_reading_pane( 2 ) );
	DPLayout( K3, im_reading_pane( 2 ) );

	_LIT( KWP1,"im_writing_pane( 0 )");
	DPLayout( KWP1, AppLayout::im_writing_pane( 0 ) );
	DPLayout( KWP1, im_writing_pane( 0 ) );
	_LIT( KWP2,"im_writing_pane( 1 )");
	DPLayout( KWP2, AppLayout::im_writing_pane( 1 ) );
	DPLayout( KWP2, im_writing_pane( 1 ) );

	_LIT( K4,"im_reading_field( 0 ) ");
	DPLayout( K4, AppLayout::im_reading_field( 0 ) );
	DPLayout( K4, im_reading_field( 0 ) );
	_LIT( K5,"im_reading_field( 1 ) ");
	DPLayout( K5, AppLayout::im_reading_field( 1 ) );
	DPLayout( K5, im_reading_field( 1 ) );
	_LIT( K6,"im_reading_field( 2 ) ");
	DPLayout( K6, AppLayout::im_reading_field( 2 ) );
	DPLayout( K6, im_reading_field( 2 ) );
	_LIT( K7,"im_reading_field( 3 ) ");
	DPLayout( K7, AppLayout::im_reading_field( 3 ) );
	DPLayout( K7, im_reading_field( 3 ) );
	_LIT( K8,"im_reading_field( 4 ) ");
	DPLayout( K8, AppLayout::im_reading_field( 4 ) );
	DPLayout( K8, im_reading_field( 4 ) );
	_LIT( K9,"im_reading_field( 5 ) ");
	DPLayout( K9, AppLayout::im_reading_field( 5 ) );
	DPLayout( K9, im_reading_field( 5 ) );
	_LIT( K10,"im_reading_field( 6 ) ");
	DPLayout( K10, AppLayout::im_reading_field( 6 ) );
	DPLayout( K10, im_reading_field( 6 ) );
	
    TAknLayoutScalableParameterLimits limits( 
        AknLayoutScalable_Apps::list_im_single_pane_ParamLimits() );

    if( 7 <= limits.LastRow() )
        {
        _LIT( K11,"im_reading_field( 7 ) ");
    	DPLayout( K11, AppLayout::im_reading_field( 7 ) );
    	DPLayout( K11, im_reading_field( 7 ) );    
        }

	_LIT( KRPT0,"IM_reading_pane_texts_Line_1( 0 )");
	DPLayout( KRPT0, AppLayout::IM_reading_pane_texts_Line_1( 0 ) );
	DPLayout( KRPT0, IM_reading_pane_texts_Line_1( 0 ) );
	_LIT( KRPT1,"IM_reading_pane_texts_Line_1( 1 )");
	DPLayout( KRPT1, AppLayout::IM_reading_pane_texts_Line_1( 1 ) );
	DPLayout( KRPT1, IM_reading_pane_texts_Line_1( 1 ) );
	_LIT( KRPT2,"IM_reading_pane_texts_Line_1( 2 )");
	DPLayout( KRPT2, AppLayout::IM_reading_pane_texts_Line_1( 2 ) );
	DPLayout( KRPT2, IM_reading_pane_texts_Line_1( 2 ) );
	_LIT( KRPT3,"IM_reading_pane_texts_Line_2()");
	DPLayout( KRPT3, AppLayout::IM_reading_pane_texts_Line_2() );
	DPLayout( KRPT3, IM_reading_pane_texts_Line_2() );

	_LIT( KIMT1,"IM_text_elements( 0 )");
	DPLayout( KIMT1, AppLayout::IM_text_elements( 0 ) );
	DPLayout( KIMT1, IM_text_elements( 0 ) );
	_LIT( KIMT2,"IM_text_elements( 1 )");
	DPLayout( KIMT2, AppLayout::IM_text_elements( 1 ) );
	DPLayout( KIMT2, IM_text_elements( 1 ) );
	_LIT( KIMT3,"IM_text_elements( 2 )");
	DPLayout( KIMT3, AppLayout::IM_text_elements( 2 ) );
	DPLayout( KIMT3, IM_text_elements( 2 ) );

	_LIT( KMWPT0,"Multiline_IM_writing_pane_texts_Line_1( 0 )");
	DPLayout( KMWPT0, AppLayout::Multiline_IM_writing_pane_texts_Line_1( 0 ) );
	DPLayout( KMWPT0, Multiline_IM_writing_pane_texts_Line_1( 0 ) );

	_LIT( KMWPT1,"Multiline_IM_writing_pane_texts_Line_1( 1 )");
	DPLayout( KMWPT1, AppLayout::Multiline_IM_writing_pane_texts_Line_1( 1 ) );
	DPLayout( KMWPT1, Multiline_IM_writing_pane_texts_Line_1( 1 ) );

	_LIT( KMWPT2,"Multiline_IM_writing_pane_texts_Line_1( 2 )");
	DPLayout( KMWPT2, AppLayout::Multiline_IM_writing_pane_texts_Line_1( 2 ) );
	DPLayout( KMWPT2, Multiline_IM_writing_pane_texts_Line_1( 2 ) );

	_LIT( KWFL10,"IM_writing_field_elements_Line_1( 0 )");
	DPLayout( KWFL10, AppLayout::IM_writing_field_elements_Line_1( 0 ) );
	DPLayout( KWFL10, IM_writing_field_elements_Line_1( 0 ) );

	_LIT( KWFL11,"IM_writing_field_elements_Line_1( 1 )");
	DPLayout( KWFL11, AppLayout::IM_writing_field_elements_Line_1( 1 ) );
	DPLayout( KWFL11, IM_writing_field_elements_Line_1( 1 ) );

	_LIT( KWFL2,"IM_writing_field_elements_Line_2()");
	DPLayout( KWFL2, AppLayout::IM_writing_field_elements_Line_2() );
	DPLayout( KWFL2, IM_writing_field_elements_Line_2() );

	_LIT( KWFL30,"IM_writing_field_elements_Line_3( 0 )");
	DPLayout( KWFL30, AppLayout::IM_writing_field_elements_Line_3( 0 ) );
	DPLayout( KWFL30, IM_writing_field_elements_Line_3( 0 ) );

	_LIT( KWFL31,"IM_writing_field_elements_Line_3( 1 )");
	DPLayout( KWFL31, AppLayout::IM_writing_field_elements_Line_3( 1 ) );
	DPLayout( KWFL31, IM_writing_field_elements_Line_3( 1 ) );

	HBufC* myStr = HBufC::NewLC( 60 );
	TPtr myPtr = myStr->Des();

	_LIT( KNPTL,"IM_navi_pane_texts_Line_1( ");
	_LIT( KNPTL0,"AppLayout::");
	_LIT( KNPTL1,", 0 )");
	_LIT( KNPTL2,", 1 )");
	TInt i(0);
	for( i=0; i<10; i++ )
		{
		myPtr.Copy( KNPTL0() );
		myPtr.Append( KNPTL() );
		myPtr.AppendNum( i );
		myPtr.Append( KNPTL1() );
		DPLayout( myPtr, IM_navi_pane_texts_Line_1( i ,0 ) );

		myPtr.Copy( KNPTL() );
		myPtr.AppendNum( i );
		myPtr.Append( KNPTL1() );
		DPLayout( myPtr, IM_navi_pane_texts_Line_1( i ,0 ) );
		}
	for( i =0; i<10; i++ )
		{
		myPtr.Copy( KNPTL0() );
		myPtr.Append( KNPTL() );
		myPtr.AppendNum( i );
		myPtr.Append( KNPTL2() );
		DPLayout( myPtr, IM_navi_pane_texts_Line_1( i ,1 ) );

		myPtr.Copy( KNPTL() );
		myPtr.AppendNum( i );
		myPtr.Append( KNPTL2() );
		DPLayout( myPtr, IM_navi_pane_texts_Line_1( i ,1 ) );
		}

	CleanupStack::PopAndDestroy( myStr );

#endif // _DEBUG
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::LogLayoutL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::DPLayout
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::DPLayout( const TDesC& /*aStr*/, const TAknTextLineLayout /*aLayout*/ )
	{
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::DPLayout
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::DPLayout( const TDesC& /*aStr*/,
		const TAknWindowLineLayout /*aLayout*/ )
	{
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::DPRect
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::DPRect( const TDesC& /*aStr*/, const TRect& /*aRect*/ )
	{
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillLayoutL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillLayoutL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillLayoutL()	start" ));
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, iScreenRect );
	if ( iLastScreenRect != iScreenRect )
		{
		FillReadingPaneL();
		FillWrittingPaneL();
		FillReadingFieldL();
		FillReadingPaneTextsLine1L();
		FillReadingPaneTextsLine2();
		FillIMTextElementsL();
		FillWrittingPaneTextsLineL();
		FillIMWritingFieldLine1L();
		FillIMWritingFieldLine2();
		FillIMWritingFieldLine3L();
		FillIMNaviPaneTextsLineL();
		}
	iLastScreenRect = iScreenRect;
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillLayoutL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillReadingPaneL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillReadingPaneL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneL()	start" ));
	if ( iReadingPane )
		{
		iReadingPane->Reset();
		delete iReadingPane;
		iReadingPane = NULL;
		}

	iReadingPane = new( ELeave )CArrayFixFlat<TAknWindowLineLayout>( 3 );
	for( TInt i(0); i < 3; i++ )
		{
		iReadingPane->AppendL( AppLayout::im_reading_pane( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillReadingFieldL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillReadingFieldL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingFieldL()	start" ));
	if ( iReadingField )
		{
		iReadingField->Reset();
		delete iReadingField;
		iReadingField = NULL;
		}

    TAknLayoutScalableParameterLimits limits( 
        AknLayoutScalable_Apps::list_im_single_pane_ParamLimits() );

    // items from FirstRow to LastRow
	iReadingField = new( ELeave )CArrayFixFlat<TAknWindowLineLayout>( 
	        limits.LastRow() - limits.FirstRow() + 1 );
    
	for( TInt i( limits.FirstRow() ); i <= limits.LastRow(); ++i )
		{
		iReadingField->AppendL( AppLayout::im_reading_field( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingFieldL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillReadingPaneTextsLine1
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillReadingPaneTextsLine1L()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneTextsLine1L()	start" ));
	if ( iReadingPaneTextsLine1 )
		{
		iReadingPaneTextsLine1->Reset();
		delete iReadingPaneTextsLine1;
		iReadingPaneTextsLine1 = NULL;
		}

	iReadingPaneTextsLine1 = new( ELeave )CArrayFixFlat<TAknTextLineLayout>( 3 );
	for( TInt i(0); i < 3; i++ )
		{
		iReadingPaneTextsLine1->AppendL(
			AppLayout::IM_reading_pane_texts_Line_1( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneTextsLine1L()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillIMTextElementsL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillIMTextElementsL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMTextElementsL()	start" ));
	if ( iIMTextElements )
		{
		iIMTextElements->Reset();
		delete iIMTextElements;
		iIMTextElements = NULL;
		}

	iIMTextElements = new( ELeave )CArrayFixFlat<TAknWindowLineLayout>( 3 );
	for( TInt i(0); i < 3; i++ )
		{
		iIMTextElements->AppendL( AppLayout::IM_text_elements( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMTextElementsL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillReadingPaneTextsLine2
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillReadingPaneTextsLine2()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneTextsLine2()	start" ));
	iReadingPaneTextsLine2 = AppLayout::IM_reading_pane_texts_Line_2();
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillReadingPaneTextsLine2()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillWrittingPaneTextsLineL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillWrittingPaneTextsLineL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillWrittingPaneTextsLineL()	start" ));
	if ( iWrittingPaneTextsLine )
		{
		iWrittingPaneTextsLine->Reset();
		delete iWrittingPaneTextsLine;
		iWrittingPaneTextsLine = NULL;
		}

	iWrittingPaneTextsLine = new( ELeave )CArrayFixFlat<TAknMultiLineTextLayout>( 3 );
	for( TInt i(0); i < 3; i++ )
		{
		iWrittingPaneTextsLine->AppendL(
			AppLayout::Multiline_IM_writing_pane_texts_Line_1( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillWrittingPaneTextsLineL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillIMWritingFieldLine1
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillIMWritingFieldLine1L()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine1L()	start" ));
	if ( iIMWritingFieldLine1 )
		{
		iIMWritingFieldLine1->Reset();
		delete iIMWritingFieldLine1;
		iIMWritingFieldLine1 = NULL;
		}

	iIMWritingFieldLine1 = new( ELeave )CArrayFixFlat<TAknWindowLineLayout>( 3 );
	for( TInt i(0); i < 2; i++ )
		{
		iIMWritingFieldLine1->AppendL(
			AppLayout::IM_writing_field_elements_Line_1( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine1L()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillIMWritingFieldLine2
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillIMWritingFieldLine2()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine2()	start" ));
	iIMWritingFieldLine2 =
		AppLayout::IM_writing_field_elements_Line_2();
	iIMWritingFieldLine2.iH = 1;
	iIMWritingFieldLine2.iW = ELayoutP;
	iIMWritingFieldLine2.iC = 221;
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine2()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillIMWritingFieldLine3L
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillIMWritingFieldLine3L()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine3L()	start" ));
	if ( iIMWritingFieldLine3 )
		{
		iIMWritingFieldLine3->Reset();
		delete iIMWritingFieldLine3;
		iIMWritingFieldLine3 = NULL;
		}

	iIMWritingFieldLine3 = new ( ELeave ) CArrayFixFlat< TAknWindowLineLayout >(
		 3 );
	for( TInt i(0); i < 2; i++ )
		{
		iIMWritingFieldLine3->AppendL(
			AppLayout::IM_writing_field_elements_Line_3( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMWritingFieldLine3L()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillWrittingPaneL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillWrittingPaneL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillWrittingPaneL()	start" ));
	if ( iWrittingPane )
		{
		iWrittingPane->Reset();
		delete iWrittingPane;
		iWrittingPane = NULL;
		}

	iWrittingPane = new( ELeave )CArrayFixFlat<TAknWindowLineLayout>( 2 );
	for( TInt i(0); i < 2; i++ )
		{
		iWrittingPane->AppendL( AppLayout::im_writing_pane( i ) );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillWrittingPaneL()	end" ));
	}

// ---------------------------------------------------------
// CIMCVAppLayoutInfo::FillIMNaviPaneTextsLine
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLayoutInfo::FillIMNaviPaneTextsLineL()
	{
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMNaviPaneTextsLineL()	start" ));
	// fader colors from LAF. These are hardcoded as the LAF (impl.) adviced. 
	const TInt16 colors[] = { 234, 232, 230, 228, 226, 0, 226, 227, 228, 229 };

	if ( iIMNaviPaneTextsLine1 )
		{
		iIMNaviPaneTextsLine1->Reset();
		delete iIMNaviPaneTextsLine1;
		iIMNaviPaneTextsLine1 = NULL;
		}

	iIMNaviPaneTextsLine1 = 
	    new ( ELeave ) CArrayFixFlat<TAknTextLineLayout>( 10 );
    
    TAknTextLineLayout text;
	for( TInt i(0); i < 10; i++ )
		{
		text = AknLayoutScalable_Apps::navi_pane_im_t1();
		text.iC = colors[ i ];
		iIMNaviPaneTextsLine1->AppendL( text );
		}
	IM_CV_LOGS(TXT("CIMCVAppLayoutInfo::FillIMNaviPaneTextsLineL()	end" ));
	}

// End of File
