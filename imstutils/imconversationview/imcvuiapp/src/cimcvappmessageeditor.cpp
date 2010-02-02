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
* Description:  message editer
*
*/

// INCLUDE FILES
#include <AknsBasicBackgroundControlContext.h>
#include "cimcvappmessageeditor.h"
#include <eikmenub.h>
#include <eikdef.h>
#include <akninfrm.h>
#include <eikon.rsg>
#include <eikenv.h>
#include <txtglobl.h>
#include <gulfont.h>
#include <txtfrmat.h>
#include <eikrted.h>
#include <aknconsts.h>
#include <aknEditStateIndicator.h>
#include <featmgr.h>
#include <fepbase.h>
#include <AknUtils.h>
#include <aknappui.h>
#include <imcvuiapp.mbg>
#include <eikcolib.h>               //CEikAppUiFactory
#include <avkon.mbg>
#include <skinlayout.cdl.h>
#include <txtrich.h>
#include <eikedwin.h>
#include "cimcvappview.h"
#include "mimcvtapeventobserver.h"
#include "cimcvappmessageextensionshandler.h"
#include <aknlayoutscalable_avkon.cdl.h>
using namespace SkinLayout;


// CONSTANTS
const TInt KChatKeyEventHotkeyLimit = 100;
const TInt KChatResourceBufferSize = 24;
const TInt KMaxMsgLength = 400;




// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::NewL
// ---------------------------------------------------------------------------
//
CIMCVAppMessageEditor* CIMCVAppMessageEditor::NewL(const CCoeControl* aParent, 
        TDesC& mbmPath,
        CIMCVAppView& aAppView,
        CIMCVAppMessageExtensionsHandler& aExtensionsHandler)
    {
    CIMCVAppMessageEditor* msgEditor = new(ELeave)CIMCVAppMessageEditor(aAppView,
            aExtensionsHandler);
    CleanupStack::PushL( msgEditor );
    msgEditor->ConstructL( aParent, mbmPath );   
    msgEditor->SetFocus( ETrue );
    CleanupStack::Pop();    // msgEditor
    return msgEditor;
    }


// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::~CBrowserGotoPane
// ---------------------------------------------------------------------------
//
CIMCVAppMessageEditor::~CIMCVAppMessageEditor()
    { 
    if(iBgContext)
     {
    delete iBgContext;
     }
    delete iEditor;
    delete iInputFrame;   
    iEditorObserver = NULL;
    }

// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::CIMCVAppMessageEditor
// ---------------------------------------------------------------------------
//
CIMCVAppMessageEditor::CIMCVAppMessageEditor(CIMCVAppView& aAppView,
        CIMCVAppMessageExtensionsHandler& aExtensionHandler):
    iAppView (aAppView),
    iExtensionHandler(aExtensionHandler)
    {
    }

// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::ConstructL
// ---------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::ConstructL ( const CCoeControl* aParent, TDesC& mbmPath )
    {
    CreateWindowL();
    
    // remove 'const' modifier and set parent control
    SetMopParent( CONST_CAST( CCoeControl*, aParent ) );

    iEditor = new (ELeave) CEikRichTextEditor;

    iInputFrame = CAknInputFrame::NewL(
                                        iEditor,
                                        EFalse,
                                        mbmPath,
                                        EMbmImcvuiappQgn_indi_im_msg_editor,
                                        EMbmImcvuiappQgn_indi_im_msg_editor_mask,
                                        0 );

    iInputFrame->SetContainerWindowL( *this );

    AknEditUtils::ConstructEditingL (   iEditor,
                                        KMaxMsgLength,
                                        1,
                                        EAknEditorCharactersLowerCase,
                                        EAknEditorAlignRight,
                                        EFalse,
                                        ETrue,
                                        EFalse );

    iEditor->SetContainerWindowL( *this );
    iEditor->SetObserver( this );
    iEditor->SetBorder( TGulBorder::ENone );
    iEditor->SetAknEditorCase( EAknEditorLowerCase );
    iEditor->SetAknEditorInputMode( EAknEditorAllInputModes );
    iEditor->SetAllowPictures(ETrue);    
   // iEditor->SetAknEditorFlags(CEikRichTextEditor::ENoTextParsers);
    iEditor->SetAknEditorPermittedCaseModes ( EAknEditorAllCaseModes );    
    iEditor->CreateTextViewL();     
    TRect screen( 0,0,0,0 );
            
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screen );
    //get the background skin context
    iBgContext = CAknsBasicBackgroundControlContext::NewL( 
            KAknsIIDQsnBgAreaMain, screen, EFalse );
    //set the editor background color to background skin context
    iEditor->SetSkinBackgroundControlContextL( iBgContext );   
    iEditor->OverrideColorL(EColorControlSurroundBackground,KRgbRed);
    iEditor->EnableCcpuSupportL(ETrue);
    iEditor->SetEdwinObserver(this);
    iServiceId = iAppView.ActiveEngine().GetServiceId();
	ActivateL();
    }

// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::HandleControlEventL
// ---------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::HandleControlEventL ( CCoeControl* /*aControl*/,
        TCoeEvent aEventType )
    {
    switch (aEventType)
        {        
        case EEventStateChanged:
            {
            iEditor->DrawNow();
            iAppView.UpdateToolbarButtonsStateL();
            break;
            }
        case EEventRequestFocus:
            {
            FocusChanged( EDrawNow );
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::HandleControlEventL
// ---------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::HandleEdwinEventL(CEikEdwin* /*aEdwin*/,
        TEdwinEvent /*aEventType*/)
    {
    iEditor->DrawNow();
    }
// -----------------------------------------------------------------------------
// CIMCVAppMessageEditor::NotifyObserverIfNeededL
// Called to force refresh of editor contents after inserting a smiley
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::NotifyObserverIfNeededL( TInt aOldLen, TInt aCurLen ) const
    {
    if ( iEditorObserver )
        {        
        if ( aCurLen == 0 && aOldLen > 0 )
            {
            iEditorObserver->HandleMessageEditorEventL(
                MIMCVAppMessageEditorObserver::EEditorContentCleared );
            
            return;
            }
        if (aOldLen != aCurLen)
            {
            iEditorObserver->HandleMessageEditorEventL(
                MIMCVAppMessageEditorObserver::EEditorContentChanged );            
            return;
            }
        }
    
    }
// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CIMCVAppMessageEditor::OfferKeyEventL
( const TKeyEvent& aKeyEvent, TEventCode aType )
    {    
    TInt oldLen = iEditor->TextLength();	    
    if ( iEditor->TextLength() == iEditor->MaxLength() )
        {
        if(( aType == EEventKey && 
                ( aKeyEvent.iCode != EKeyBackspace && 
                  aKeyEvent.iCode < EKeyDelete ) 
           ) ||
           ( aType == EEventKeyUp && 
            aKeyEvent.iScanCode >= '0' && 
            aKeyEvent.iScanCode <= '9'
           )
          )
            {

            return EKeyWasConsumed;
            }
        }

    TKeyResponse res = EKeyWasNotConsumed;
    
    // This logic comes from CEikEdwin
    // We need to handle hot keys cut and copy
    TBool select = aKeyEvent.iModifiers & EModifierShift;
    TBool magnify = aKeyEvent.iModifiers & EModifierCtrl;
    if ( magnify && aKeyEvent.iCode < KChatKeyEventHotkeyLimit
            && aKeyEvent.iCode != ' ' )
        {
        TBuf<KChatResourceBufferSize> buf;
        if ( select )
            {
            iCoeEnv->ReadResourceL( buf, R_EIK_EDWIN_SHIFT_CTRL_HOTKEYS );
            }           
        else
            {
            iCoeEnv->ReadResourceL( buf, R_EIK_EDWIN_CTRL_HOTKEYS );
            }

        const TInt ret = buf.Locate( TChar( aKeyEvent.iCode + 'a' - 1 ) );
        switch( ret )
            {            
            case CEikEdwin::EHotKeyCut:
                {
                iEditor->CcpuCutL();
                res = EKeyWasConsumed;
                break;
                }
            case CEikEdwin::EHotKeyCopy:
                {
                iEditor->CcpuCopyL();
                res = EKeyWasConsumed;
                break;
                }                
            default:
                {
                // No need to handle other hot keys here
                break;
                }
            }
        }
    // --------------------

    if ( res != EKeyWasConsumed )
        {        
        res = iEditor->OfferKeyEventL( aKeyEvent, aType );
        }
    
    NotifyObserverIfNeededL(oldLen, iEditor->TextLength());
    return res;

    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::HandlePointerEventL
// ----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    if( !AknLayoutUtils::PenEnabled() )
        {
        return;
        }
    iEditor->HandlePointerEventL( aPointerEvent );
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::CountComponentControls
// ----------------------------------------------------------------------------
//
TInt CIMCVAppMessageEditor::CountComponentControls() const
    {
    return 2;   // iEditor and input frame
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::ComponentControl
// ----------------------------------------------------------------------------
//
CCoeControl* CIMCVAppMessageEditor::ComponentControl ( TInt aIndex ) const
    {
    switch(aIndex)
        {
        case 0:
            {
			return iInputFrame;
            }
        case 1:
            {
            return iEditor;
            }
        default:
            return NULL;
        }
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::SizeChanged
// ----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::SizeChanged()
    {
    TAknLayoutRect layoutRectTemp;
    TAknWindowLineLayout windowLayoutTmp, windowLayoutTmp2;

    windowLayoutTmp = AknLayout::Find_pane_elements_Line_4();
    windowLayoutTmp2 = windowLayoutTmp;
    layoutRectTemp.LayoutRect( Rect(), windowLayoutTmp2 );
    iEditor->SetRect( layoutRectTemp.Rect() );
    iInputFrame->SetRect( Rect() );    
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::FocusChanged
// ----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::FocusChanged( TDrawNow aDrawNow )
    {
    iEditor->SetFocus( IsFocused(), aDrawNow );
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::TextLength
// ----------------------------------------------------------------------------
//
TInt CIMCVAppMessageEditor::TextLength() const
    {
    return iEditor->TextLength();
    }

// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::SetTextL
// ----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::SetTextL( const TDesC& aTxt )
    {
    iEditor->SetTextL( &aTxt );
    TInt curPos = TextLength();
    // Cursor to end, no selection.
    iEditor->SetSelectionL( curPos, curPos );
	iEditor->DrawNow();
    }


// ----------------------------------------------------------------------------
// CIMCVAppMessageEditor::Editor
// ----------------------------------------------------------------------------
CEikRichTextEditor& CIMCVAppMessageEditor::Editor() const
    {
    return *iEditor;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageEditor::SetMessageEditorObserver
// Called to force refresh of editor contents after inserting a smiley
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::SetMessageEditorObserver( MIMCVAppMessageEditorObserver* aObserver )
    {    
    iEditorObserver = aObserver;    
    }

// ---------------------------------------------------------------------------
// CIMCVAppMessageEditor::HandleFindSizeChanged
// ---------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::HandleFindSizeChanged()
    {    
    // Lay out Goto Pane as if it was the old Find Pane.    
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,rect);
    TAknWindowLineLayout msgPane=AknLayoutScalable_Avkon::find_pane();

    
    AknLayoutUtils::LayoutControl( this, rect, msgPane ); 
       
    SizeChanged();
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageEditor::SetTapObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageEditor::SetTapObserver( MIMCVTapEventObserver* aObserver, TUint aId )
    {
    iTapObserver = aObserver;
    iTapControlId = aId;    
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageEditor::MopSupplyObject
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CIMCVAppMessageEditor::MopSupplyObject( TTypeUid aId )
    {
    if( aId.iUid == MAknsControlContext::ETypeId && iBgContext )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }
    return CCoeControl::MopSupplyObject(aId);
    }

//  END OF FILE
