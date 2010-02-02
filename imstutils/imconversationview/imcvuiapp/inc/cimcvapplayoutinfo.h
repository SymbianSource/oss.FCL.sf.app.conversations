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


#ifndef CIMCVAPPLAYOUTINFO_H
#define CIMCVAPPLAYOUTINFO_H

// INCLUDES

#include "mimcvapplayoutinfo.h"
#include "mimcvapplayoutchangeobserver.h"

#include <e32base.h>
#include <AknLayout2Def.h>
// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Layout and render info for im Listbox's item drawer and view
*  Helps to calculate and draw items.
*
*  @lib imcvuiapp.exe
*  @since 5.0
*/
class CIMCVAppLayoutInfo : public CBase, 
					  public MIMCVAppLayoutInfo,
					  public MIMCVAppLayoutChangeObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor using default smile icon array.
        */
        static CIMCVAppLayoutInfo* NewL( TChatListBoxLayout::TLayout aLayoutID );

        /**
        * Destructor.
        */
        virtual ~CIMCVAppLayoutInfo();

	public: // New methods


	public: // from MIMCVAppLayoutInfo

		/**
        * MainPaneRect 
        * rect of main pane.
        */
		void SetLayoutID( TChatListBoxLayout::TLayout aLayout );
		
	   /**
        * MainPaneRect 
        * rect of main pane.
        */
		TRect MainPaneRect();

	
		/**
        * ScreenRect 
        * rect of main pane.
        */
		TRect ScreenRect();

		/**
        * SetEditorVisibility 
        */
		void SetEditorVisibility( TBool aVisible );

		/**
        * EditorVisibility 
        */
		TBool EditorVisibility();

		/**
        * im_reading_pane 
        * im reading pande 
        * param@ aIndex_h, index of reading pane
        */
		TAknWindowLineLayout im_reading_pane(TInt aIndex_H);

		/**
        * im_reading_field 
        * im reading pane.
        * param@ aIndex_t, index of reading field
        */
		TAknWindowLineLayout im_reading_field(TInt aIndex_t);

		/**
        * IM_text_elements 
        * im text elements
        * @param aLineIndex, index of text element
        */
		TAknWindowLineLayout IM_text_elements(TInt aLineIndex);

		/**
        * IM_reading_pane_texts_Line_1 
        * im reading pane text line 1
        * @param aCommon1, command to excute
        */
		TAknTextLineLayout IM_reading_pane_texts_Line_1(TInt aCommon1);

		/**
        * IM_reading_pane_texts_Line_2 
        * imreading pane text line2
        */
		TAknTextLineLayout IM_reading_pane_texts_Line_2();

	
		/**
        * im_writing_pane 
        * im writing pane
        * @param aCommon1, command to excute
        */
		TAknWindowLineLayout im_writing_pane(TInt aCommon1);

		/**
        * IM_writing_field_elements_Line_1 
        * im writing field element line 1
        * @param aIndex_H, index of writing element
        */
		TAknWindowLineLayout IM_writing_field_elements_Line_1(TInt aIndex_H);

		/**
        * IM_writing_field_elements_Line_2 
        * im writing fields elements line 2
        */
		TAknWindowLineLayout IM_writing_field_elements_Line_2();

		/**
        * IM_writing_field_elements_Line_3 
        * im writing fields elements line 3
        * @param aIndex_H, index of writing field element
        */
		TAknWindowLineLayout IM_writing_field_elements_Line_3(TInt aIndex_H);

		/**
        * Multiline_IM_writing_pane_texts_Line_1 
        * im writing pane text line 1
        * @param aNumberOfLinesShown, no of line to shown in the pane
        */
		TAknMultiLineTextLayout Multiline_IM_writing_pane_texts_Line_1(
			TInt aNumberOfLinesShown);
		/**
        * IM_navi_pane_texts_Line_1 
        * im navi pane text line 1
        * @param aIndex_C, index of text line
        * @param aIndex_W, index of pane
        */
		TAknTextLineLayout IM_navi_pane_texts_Line_1(TInt aIndex_C, TInt aIndex_W);
		/**
        * DPLayout 
        * dp layout
        * @param aStr,  string to be draw
        * @param aLayout, layout 
        */
		void DPLayout( const TDesC& aStr, const TAknTextLineLayout aLayout );
		/**
        * DPLayout 
        * rect of main pane.
        * @param aStr,  string to be draw
        * @param aLayout, layout 
        */
		void DPLayout( const TDesC& aStr, const TAknWindowLineLayout aLayout );
		/**
        * DPRect 
        * dp rect
        * @param aStr, string to be draw
        * @param aRect, rect for the layout
        */
		void DPRect( const TDesC& aStr, const TRect& aRect );
		
	public: // from MIMCVAppLayoutChangeObserver
		/**
        * MainPaneRect 
        * change of layout
        * @param aType, type of layout changes
        */
		virtual void LayoutChangedL( TInt aType = 0 );

    private:  // Constructors and destructor

        /**
        * C++ constructor.
        * @param aLayoutID, id of layout
        */
        CIMCVAppLayoutInfo( TChatListBoxLayout::TLayout aLayoutID );

        /**
        * Second-phased constructor
        */
        void ConstructL();

	private:	// New methods     
		/**
        * LogLayoutL 
        * log layout 
        */
		void LogLayoutL();		
		/**
        * FillLayoutL 
        * fill the layout
        */
		void FillLayoutL();
		/**
        * FillReadingPaneL 
        * fill the reading pane
        */
		void FillReadingPaneL();
		/**
        * FillReadingFieldL 
        * fill the reading field
        */
		void FillReadingFieldL();
		/**
        * FillReadingPaneTextsLine1L 
        * fill the reading pane text line1
        */
		void FillReadingPaneTextsLine1L();
		/**
        * FillReadingPaneTextsLine2 
        * fill the reading pane text line2
        */
		void FillReadingPaneTextsLine2();
		/**
        * FillIMTextElementsL 
        * fill the text elements
        */
		void FillIMTextElementsL();
		/**
        * FillWrittingPaneTextsLineL 
        * fill writing pane text line 
        */
		void FillWrittingPaneTextsLineL();
		/**
        * FillIMWritingFieldLine1L 
        * ffill writing field line 1
        */
		void FillIMWritingFieldLine1L();
		/**
        * FillIMWritingFieldLine2 
        * fill writing field line 2
        */
		void FillIMWritingFieldLine2();
		/**
        * FillIMWritingFieldLine3L 
        * fill writing field line 3
        */
		void FillIMWritingFieldLine3L();
		/**
        * FillWrittingPaneL 
        * fill writing pane
        */
		void FillWrittingPaneL();
		/**
        * FillIMNaviPaneTextsLineL 
        * fill navi pane text
        */
		void FillIMNaviPaneTextsLineL();
		/**
        * setMainPaneRect 
        * set rect of main pane.
        */		
		void SetupMainPaneRect();	

    private: // Data
		
		// owned, layout id
		TChatListBoxLayout::TLayout iLayoutID;
		// owned, max line toi display 
	   	TInt iMaxLines;
		// owned, main pane layout 
		TRect iMainPaneRect;
		// owned, screen rect 
		TRect iScreenRect;
		// owned, last screen rect 
		TRect iLastScreenRect;
		// owned, reading pane
		CArrayFixFlat<TAknWindowLineLayout>* iReadingPane;
		// owned, writing pane
		CArrayFixFlat<TAknWindowLineLayout>* iWrittingPane;
		// owned, reading fields
		CArrayFixFlat<TAknWindowLineLayout>* iReadingField;
		// owned, reading pane text layout
		CArrayFixFlat<TAknTextLineLayout>* iReadingPaneTextsLine1;
		// owned, text elements
		CArrayFixFlat<TAknWindowLineLayout>* iIMTextElements;
		// owned, reading text line
		TAknTextLineLayout iReadingPaneTextsLine2;
		// owned, writing text line
		CArrayFixFlat<TAknMultiLineTextLayout>* iWrittingPaneTextsLine;
		// owned, writing field line 1
		CArrayFixFlat<TAknWindowLineLayout>* iIMWritingFieldLine1;
		// owned, writing field line 2
		TAknWindowLineLayout iIMWritingFieldLine2;
		// owned, writing field line 3
		CArrayFixFlat<TAknWindowLineLayout>* iIMWritingFieldLine3;
		// owned, writing text line 1
		CArrayFixFlat<TAknTextLineLayout>* iIMNaviPaneTextsLine1;
		
		CArrayFixFlat<TAknTextLineLayout>* iIMNaviPaneTextsLine2;

		TBool iEditorVisibility;
    };

#endif  // CIMCVAPPLAYOUTINFO_H
