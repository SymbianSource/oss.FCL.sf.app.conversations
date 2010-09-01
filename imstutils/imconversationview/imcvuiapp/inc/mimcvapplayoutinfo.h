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
* Description:  layout info interface
*
*/


#ifndef MIMCVAPPLAYOUTINFO_H
#define MIMCVAPPLAYOUTINFO_H

//INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CFont;
class TAknWindowLineLayout;
class TAknTextLineLayout;
class TAknMultiLineTextLayout;

class TChatListBoxLayout
	{

	public:
		enum TLayout
			{
			EWithEditor2Lines = 0,
			EWithEditor,
			EWithoutEditor
			};
	};
// CLASS DECLARATION

/**
 *  Interface for Layout and render info for Chat Listbox's item drawer and view
 *
 *  @since 5.0
 */
class MIMCVAppLayoutInfo
    {
    public: // Interface

	   	/**
        * MainPaneRect 
        * rect of main pane.
        */     
		virtual TRect MainPaneRect() =0;
		/**
        * im_reading_pane 
        * im reading pande 
        * param@ aIndex_h, index of reading pane
        */
		virtual TAknWindowLineLayout im_reading_pane(TInt aIndex_H) =0;
		/**
        * im_reading_field 
        * im reading pane.
        * param@ aIndex_t, index of reading field
        */
		virtual TAknWindowLineLayout im_reading_field(TInt aIndex_t) =0;
		/**
        * IM_text_elements 
        * im text elements
        * @param aLineIndex, index of text element
        */
		virtual TAknWindowLineLayout IM_text_elements(TInt aLineIndex) =0;
		/**
        * IM_reading_pane_texts_Line_1 
        * im reading pane text line 1
        * @param aCommon1, command to excute
        */
		virtual TAknTextLineLayout IM_reading_pane_texts_Line_1(
		                                    TInt aCommon1) =0;
		/**
        * IM_reading_pane_texts_Line_2 
        * imreading pane text line2
        */
		virtual TAknTextLineLayout IM_reading_pane_texts_Line_2() =0;

		/**
        * im_writing_pane 
        * im writing pane
        * @param aCommon1, command to excute
        */
		virtual TAknWindowLineLayout im_writing_pane(TInt aCommon1) =0;
		/**
        * IM_writing_field_elements_Line_1 
        * im writing field element line 1
        * @param aIndex_H, index of writing element
        */
		virtual TAknWindowLineLayout IM_writing_field_elements_Line_1(
		                                    TInt aIndex_H) =0;
		/**
        * IM_writing_field_elements_Line_2 
        * im writing fields elements line 2
        */
		virtual TAknWindowLineLayout IM_writing_field_elements_Line_2() =0;
		/**
        * IM_writing_field_elements_Line_3 
        * im writing fields elements line 3
        * @param aIndex_H, index of writing field element
        */
		virtual TAknWindowLineLayout IM_writing_field_elements_Line_3(
		                                    TInt aIndex_H) =0;
		/**
        * Multiline_IM_writing_pane_texts_Line_1 
        * im writing pane text line 1
        * @param aNumberOfLinesShown, no of line to shown in the pane
        */
		virtual TAknMultiLineTextLayout Multiline_IM_writing_pane_texts_Line_1(
		                                    TInt aNumberOfLinesShown) =0;
		/**
        * IM_navi_pane_texts_Line_1 
        * im navi pane text line 1
        * @param aIndex_C, index of text line
        * @param aIndex_W, index of pane
        */
		virtual TAknTextLineLayout IM_navi_pane_texts_Line_1(
		                                    TInt aIndex_C, TInt aIndex_W) =0;
		/**
        * DPLayout 
        * dp layout
        * @param aStr,  string to be draw
        * @param aLayout, layout 
        */
		virtual void DPLayout( const TDesC& aStr, 
		                       const TAknTextLineLayout aLayout ) =0;
		/**
        * DPLayout 
        * rect of main pane.
        * @param aStr,  string to be draw
        * @param aLayout, layout 
        */
		virtual void DPLayout( const TDesC& aStr, 
		                       const TAknWindowLineLayout aLayout ) =0;
			/**
        * DPRect 
        * dp rect
        * @param aStr, string to be draw
        * @param aRect, rect for the layout
        */
		virtual void DPRect( const TDesC& aStr, const TRect& aRect ) =0;
		
		/**
        * SetLayoutID 
        * dp rect
        * @param aLayout, to be to be draw
        */
		virtual void SetLayoutID( TChatListBoxLayout::TLayout aLayout ) =0;
		
		/**
        * SetEditorVisibility 
        * @param aVisible, ETrue or EFalse
        */
		virtual	void SetEditorVisibility( TBool aVisible ) = 0;
			
	protected:
		/**
        * ~MIMCVAppLayoutInfo 
        * virtual destrauctor
        */
		virtual ~MIMCVAppLayoutInfo() {};

    };

#endif      // MIMCVAPPLAYOUTINFO_H

// End of File
