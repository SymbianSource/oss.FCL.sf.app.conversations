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
* Description:  search fields editor implementation 
*
*/


// INCLUDE FILES
#include "cvimpstuisearchquerydialog.h"

#include "cvimpstuisearchfieldaray.h"
#include "vimpstui.hrh"
#include "vimpstutils.h"
// resource
#include <vimpstuires.rsg>

// System includes
#include <akntitle.h>
#include <aknnavi.h>
#include <StringLoader.h>
#include <eikcapc.h>
#include <barsread.h>
#include <aknnotewrappers.h>
#include <hlplch.h>
#include "uiservicetabtracer.h"



// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::CVIMPSTUiSearchQueryDialog
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchQueryDialog::CVIMPSTUiSearchQueryDialog(CVIMPSTUiSearchFieldArray& aUiFieldArray )
	: iUiFieldArray( aUiFieldArray )
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::~CVIMPSTUiSearchQueryDialog
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchQueryDialog::~CVIMPSTUiSearchQueryDialog()
	{
	// Restore the titlepane text
	if (iTitlePane && iStoredTitlePaneText)
		{
		// iTitlePane takes ownership of iStoredTitlePaneText
		TRAP_IGNORE( iTitlePane->SetTextL( iStoredTitlePaneText->Des() ) );
		delete iStoredTitlePaneText;
		}
	 // Remove empty status pane
    if (iNaviContainer)
        {
        iNaviContainer->Pop();
        }

	}


// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchQueryDialog* CVIMPSTUiSearchQueryDialog::NewL(CVIMPSTUiSearchFieldArray& aUiFieldArray )
    {
    CVIMPSTUiSearchQueryDialog* self =
        new ( ELeave ) CVIMPSTUiSearchQueryDialog(aUiFieldArray);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::ConstructL()
    {
	TRACER_AUTO;
    CAknDialog::ConstructL(R_SEARCH_EDITOR_MENUBAR);
    
    StoreTitlePaneTextL();
   
    ConstructNaviPaneL();
    
    iSearchType = iUiFieldArray.GetSearchType();
  
  	if( !IsBasicFieldAvailableL() )
	  	{
	  	iSearchType = EIMAdvancedSearch ;
	  	iUiFieldArray.SetSearchType( EIMAdvancedSearch );	
	  	}
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::ExecuteLD()
    {
	TRACER_AUTO;
    CleanupStack::PushL( this );

    // Set status pane layout to the Phonebook 2 one
    CEikStatusPane* statusPane = iAvkonAppUi->StatusPane();
    if (statusPane)
        {
        statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);
        }
    CleanupStack::Pop( this );

    CAknForm::ExecuteLD(R_SEARCH_ENTRY_DIALOG);
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::OfferKeyEventL
// --------------------------------------------------------------------------
//
TKeyResponse CVIMPSTUiSearchQueryDialog::OfferKeyEventL
        ( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
	TRACER_AUTO;
    TKeyResponse ret(EKeyWasNotConsumed);
 
    ret = CAknForm::OfferKeyEventL( aKeyEvent, aType );
    isEndKeyPressed = ETrue;
    if ( ret == EKeyWasConsumed && aKeyEvent.iCode != EKeyEscape )
        {
        isEndKeyPressed = EFalse;
        UpdateCbasL();
        }
    return ret;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::PreLayoutDynInitL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::PreLayoutDynInitL()
    {
    SetEditableL(ETrue);

    iUiFieldArray.CreateUiFieldsFromArrayL( *this );
    
    ShowSearchUiFieldsL();
    
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::PostLayoutDynInitL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::PostLayoutDynInitL()
    {
    UpdateTitleL();
    }
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::SetInitialCurrentLineL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::SetInitialCurrentLineL()
    {
   	ActivateFirstPageL();
  
    CAknForm::SetInitialCurrentLine();
    
    iUiFieldArray.SetFocusL(0);
       
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::OkToExitL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchQueryDialog::OkToExitL( TInt aKeycode )
    {
	TRACER_AUTO;
   TBool ret = EFalse;
   switch( aKeycode )
	   {
	   	case EAknSoftkeyEmpty :
		   	{
		   	ret = EFalse; 
		   	break;	
		   	}
	   	case EEikCmdExit :
	   	case EAknSoftkeyExit :
	   	case EAknSoftkeyCancel :
		   	{
		   	ret = ETrue; 
		   	break;	
		   	}
		case EAknSoftkeyDone :
	   	case EAknSoftkeyOk :
		   	{
		   	CmdDoneL( );
			ret = ETrue; 
		   	break;	
		   	}
		   	
		case EBasicSearch :
		   	{
		   	iSearchType = EIMBasicSearch;
		   	ShowSearchUiFieldsL();
		   	UpdateTitleL();
		   	ret = EFalse; 
		   	break;	
		   	}
		case EAdvancedSearch :
		   	{
		   	iSearchType = EIMAdvancedSearch ;
		   	ShowSearchUiFieldsL();
		   	UpdateTitleL();
		   	ret = EFalse; 
		   	break;	
		   	}
	   	default :
		   	{
		   	ret = CAknForm::OkToExitL( aKeycode ) ;	
		   	}
	   }
	iUiFieldArray.SetSearchType( iSearchType );
    UpdateCbasL();
    return ret;
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::CreateLineL
// --------------------------------------------------------------------------
//
CCoeControl* CVIMPSTUiSearchQueryDialog::CreateLineL
        ( const TDesC& aCaption, TInt aControlId, TInt aControlType )
    {
    return CreateLineByTypeL( aCaption, aControlId, aControlType, NULL );
    }

   
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::LineControl
// --------------------------------------------------------------------------
//
CEikCaptionedControl* CVIMPSTUiSearchQueryDialog::LineControl
        ( TInt aControlId ) const
    {
    return Line( aControlId );
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::SetEditableL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::SetEditableL( TBool aState )
    {
    CAknForm::SetEditableL( aState );
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::AreAllControlsEmptyL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchQueryDialog::AreAllControlsEmptyL() const
    {
    // before saving entered data 
    return iUiFieldArray.AreAllUiControlsEmptyL();
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::ConstructContextMenuL
// --------------------------------------------------------------------------
//
inline void CVIMPSTUiSearchQueryDialog::ConstructContextMenuL()
    {
	TRACER_AUTO;
    CEikMenuBar* newMenuBar = new ( ELeave ) CEikMenuBar();
    CleanupStack::PushL( newMenuBar );
    newMenuBar->ConstructL
        ( this, NULL, R_SEARCH_EDITOR_CONTEXT_MENUBAR );
    iEikonEnv->EikAppUi()->AddToStackL( newMenuBar, ECoeStackPriorityMenu,
        ECoeStackFlagRefusesFocus );
    iContextMenuBar = newMenuBar;
    CleanupStack::Pop( newMenuBar );
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::ConstructNaviPaneL
// --------------------------------------------------------------------------
//
inline void CVIMPSTUiSearchQueryDialog::ConstructNaviPaneL()
    {
	TRACER_AUTO;
    CEikStatusPane* statusPane = iEikonEnv->AppUiFactory()->StatusPane();
    if (statusPane && statusPane->PaneCapabilities
            ( TUid::Uid( EEikStatusPaneUidNavi ) ).IsPresent() )
        {
        // Create a default empty status pane, otherwise sync field will show
        iNaviContainer = static_cast<CAknNavigationControlContainer *>
            ( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        iNaviContainer->PushDefaultL();
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::UpdateTitleL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::UpdateTitleL()
	{
	if (iTitlePane)
		{ 
		HBufC* titleText = NULL;
		if( iSearchType == EIMBasicSearch )
			{
		    titleText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_BASIC ); 		
			iTitlePane->SetTextL( *titleText );
			}
		else
			{
			titleText =  VIMPSTUtils::LoadResourceL( R_QTN_SERVTAB_SEARCH_ADVANCED ); 
			iTitlePane->SetTextL( *titleText);
			}
		delete titleText;
		DrawNow();
		}

	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::CmdDoneL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::CmdDoneL( )
    {
    TInt count = iUiFieldArray.Count();
    for( TInt i = 0; i< count ; i++ )
	    {
	    HBufC* data = iUiFieldArray.At(i).ControlTextL() ;	
	    if(data)
		    {
		    iUiFieldArray.At(i).SetFieldDataL( *data )	;
		    delete data;
		    }
	    }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::TryChangeFocusL
// --------------------------------------------------------------------------
//    
void CVIMPSTUiSearchQueryDialog::TryChangeFocusL(
    TInt aControlId )
	{
	if ( ControlOrNull( aControlId ) )
        {
        TryChangeFocusToL( aControlId );
        }	
	}
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::SetCurrentLineCaptionL
// --------------------------------------------------------------------------
//    
void CVIMPSTUiSearchQueryDialog::SetCurrentLineCaptionL(
    const TDesC& aText )
	{
	SetControlCaptionL(IdOfFocusControl(), aText);	
	}
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::Control
// --------------------------------------------------------------------------
//    
CCoeControl* CVIMPSTUiSearchQueryDialog::Control(
    TInt aControlId ) const
	    {
	    return ControlOrNull(aControlId);	
	    }
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::StoreTitlePaneTextL
// --------------------------------------------------------------------------
//
inline void CVIMPSTUiSearchQueryDialog::StoreTitlePaneTextL()
    {
    CEikStatusPane* statusPane = iEikonEnv->AppUiFactory()->StatusPane();
    if (statusPane && statusPane->PaneCapabilities(
        TUid::Uid(EEikStatusPaneUidTitle)).IsPresent())
        {
        iTitlePane = static_cast<CAknTitlePane*>
            (statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
        if (iTitlePane->Text())
            {
            iStoredTitlePaneText = iTitlePane->Text()->AllocL();
            }
        }
    }
    

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::UpdateCbasL
// --------------------------------------------------------------------------
//    
void CVIMPSTUiSearchQueryDialog::UpdateCbasL()
    {
	TRACER_AUTO;
    if ( !AreAllControlsEmptyL() )
        {
        if ( iCbaCommandSet != R_AVKON_SOFTKEYS_OK_CANCEL__OK )
            {
            SetCbaCommandSetL( R_AVKON_SOFTKEYS_OK_CANCEL__OK  );   
            } 
         }
     else 
        {
        if( iSearchType == EIMBasicSearch && iCbaCommandSet != R_SEARCH_SOFTKEYS_ADVANCED_CANCEL__EDIT )
			{
			SetCbaCommandSetL( R_SEARCH_SOFTKEYS_ADVANCED_CANCEL__EDIT  );   
			}    	
        else if( iSearchType == EIMAdvancedSearch && iCbaCommandSet != R_SEARCH_SOFTKEYS_BASIC_CANCEL__EDIT )
	        {
	        SetCbaCommandSetL( R_SEARCH_SOFTKEYS_BASIC_CANCEL__EDIT  ); 	
	        }  
        }
   
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::SetCbaCommandSetL
// --------------------------------------------------------------------------
//    
void CVIMPSTUiSearchQueryDialog::SetCbaCommandSetL( TInt aResourceId )
    {
	TRACER_AUTO;
    CEikButtonGroupContainer& cba = ButtonGroupContainer();
    cba.SetCommandSetL( aResourceId );
    iCbaCommandSet = aResourceId;
    cba.DrawDeferred();
    }  
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::SetLineDimmed
// --------------------------------------------------------------------------
//    
void CVIMPSTUiSearchQueryDialog::SetLineDimmed(
    				TInt aControlId , TBool aDimmed) 
    {
    SetLineDimmedNow(aControlId, aDimmed);	
    }
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::ShowSearchUiFieldsL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchQueryDialog::ShowSearchUiFieldsL()
    {
	TRACER_AUTO;
    TInt count = iUiFieldArray.Count();
    for( TInt i = 0; i< count; i++ )
	    {
	    MVIMPSTUiSearchField& field = iUiFieldArray.At(i);
	    if( iSearchType == EIMBasicSearch )
		    {
		    if( !field.IsBasicField() )
			    {
			    SetLineDimmed( field.ControlId(), ETrue );	
			    }
		    }
		else 
			{
			 SetLineDimmed( field.ControlId(), EFalse );	
			}
	    }
	SetInitialCurrentLineL();
    UpdateCbasL();
    }
   
// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::IsBasicFieldAvailableL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchQueryDialog::IsBasicFieldAvailableL()
    {
	TRACER_AUTO;
    TInt count = iUiFieldArray.Count();
    for( TInt i = 0; i< count; i++ )
	    {
	    MVIMPSTUiSearchField& field = iUiFieldArray.At(i);
	    if( field.IsBasicField() )
		    {
		    return ETrue;	
		    }
	    }
	return EFalse;
    } 

// --------------------------------------------------------------------------
// CVIMPSTUiSearchQueryDialog::IsEndKeyPressed()
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchQueryDialog::IsEndKeyPressed()
    {
    return isEndKeyPressed;
    } 
// End of File
