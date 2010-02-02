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
* Description:  Implementation of detailsview plugin control container
 *
*/


// INCLUDE FILES
#include "cvimpstdetailsviewcontainer.h"

#include "cvimpstdetailsheadercontrol.h"
#include "cvimpstdetailsviewlistboxmodel.h"
#include "cvimpstdetailsviewbrandhandler.h"
#include "mvimpstdetailspresencehandler.h"
#include "cvimpstfieldpropertyarray.h"
#include "cvimpstfieldproperty.h"
#include "vimpstdetailsview.hrh"
#include <vimpstdetailsviewpluginrsc.rsg>

// System includes
#include <aknlists.h>
#include <bautils.h>
#include <layoutmetadata.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsBasicBackgroundControlContext.h>
// virtual phonebook 
#include <MVPbkStoreContact.h>

#include "vimpstextentionuiuid.h"
#include "meco.hlp.hrh"

_LIT(KVIMPSTDetailsViewDllResFileName, "\\resource\\vimpstdetailsviewpluginrsc.rsc" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::ConstructL()
	{
	// Create the listbox
	RFs& fsSession( CCoeEnv::Static()->FsSession() );
    TFileName fileName( KVIMPSTDetailsViewDllResFileName );
    BaflUtils::NearestLanguageFile( fsSession , fileName);
   
    iResourceFile.OpenL(fsSession, fileName);
    iResourceFile.ConfirmSignatureL();
  
    // Create the header
    iHeaderCtrl = CVIMPSTDetailsHeaderControl::NewL( iBrandHandler, iPresenceHandler );
    iHeaderCtrl->SetContainerWindowL( *this );

     // Get the skin background for the view
    iBackground = CAknsBasicBackgroundControlContext::NewL(
        					KAknsIIDQsnBgAreaMain, TRect( 0, 0, 0, 0 ), EFalse );
    
    
    iListBox = new(ELeave) CAknFormDoubleGraphicStyleListBox;

	iListBox->ConstructL(this, EAknListBoxLoopScrolling);
	iListBox->View()->SetListEmptyTextL(KNullDesC);
	iListBox->SetContainerWindowL(*this);
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,
	    CEikScrollBarFrame::EAuto);
	iListBox->SetMopParent(this);

	iListBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );

	CAknIconArray* iconArray = iBrandHandler.LoadIconsLC();
	// Set icon array
	iListBox->ItemDrawer()->ColumnData()->SetIconArray(iconArray);
	CleanupStack::Pop(); // iconArray
	
    // Read field property resource strucutre array
    HBufC8* fieldBuffer = iResourceFile.AllocReadLC( R_VIMPST_FIELD_PROPERTIES ) ;
  	// reader ownership to model
	iListBoxModel = CVIMPSTDetailsViewListBoxModel::NewL(*iListBox, iPresenceHandler, fieldBuffer);
	CleanupStack::Pop(); // fieldBuffer
	iListBox->Model()->SetItemTextArray(iListBoxModel);
	iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);

	 //Set listbox observer
    iListBox->SetListBoxObserver( this );
	iListBox->ActivateL();
 
    }
        
// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::CVIMPSTDetailsViewContainer()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTDetailsViewContainer::CVIMPSTDetailsViewContainer(const TUid& aPluginUid,
														CVIMPSTDetailsViewBrandHandler& aBrandHandler,
														MVIMPSTDetailsPresenceHandler& aPresenceHandler,
														TInt aFocusedListIndex )
	 : 
    iPluginUid(aPluginUid),
    iBrandHandler( aBrandHandler ),
    iPresenceHandler( aPresenceHandler ),
    iFocusedListIndex( aFocusedListIndex )
	{
	//No implementation needed.
	}


// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::~CVIMPSTDetailsViewContainer()
// Destructor.
// -----------------------------------------------------------------------------
//
CVIMPSTDetailsViewContainer::~CVIMPSTDetailsViewContainer()
	{
	delete iBackground;
    delete iListBoxModel;
    delete iListBox;
    delete iHeaderCtrl;
    iResourceFile.Close();
	}

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::Draw()
// -----------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::Draw(const TRect& /*aRect*/) const
    {
    // Draw the background using skin
    if ( iBackground )
        {
        AknsDrawUtils::Background( 
            AknsUtils::SkinInstance(), iBackground, this, SystemGc(), Rect() );
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::CountComponentControls
// --------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewContainer::CountComponentControls() const
	{
	// two control has been used , one header control and another list box
	return 2;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::ComponentControl
// --------------------------------------------------------------------------
//
CCoeControl* CVIMPSTDetailsViewContainer::ComponentControl(TInt aIndex) const
	{
	switch ( aIndex )
		{
		case 0:
		    return iHeaderCtrl;
		case 1:
		    return iListBox;
		default:
		    return NULL;
		}
	}

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::MopSupplyObject()
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CVIMPSTDetailsViewContainer::MopSupplyObject(TTypeUid aId)
    {
    // For skinning
    if ( iBackground && aId.iUid == MAknsControlContext::ETypeId )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBackground );
        }
    return CCoeControl::MopSupplyObject( aId );
    }
 
 // -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::FocusChanged()
// -----------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::FocusChanged( TDrawNow aDrawNow )
    {
    // For enabling animated hilight (if in skin supports it)
    CCoeControl::FocusChanged( aDrawNow );
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::ListBox
// --------------------------------------------------------------------------
//
CAknFormDoubleGraphicStyleListBox& CVIMPSTDetailsViewContainer::ListBox() const
	{
	return *iListBox;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::ListBoxModel
// --------------------------------------------------------------------------
//
CVIMPSTDetailsViewListBoxModel& CVIMPSTDetailsViewContainer::ListBoxModel() const
	{
	return *iListBoxModel;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::OfferKeyEventL
// --------------------------------------------------------------------------
//
TKeyResponse CVIMPSTDetailsViewContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType)
	{
	//Temporary solution for handling key events.
	TKeyResponse result = EKeyWasNotConsumed;

	switch (aKeyEvent.iCode)
	    {
	    case EKeyLeftArrow:
	    case EKeyRightArrow:
	        return result;
	    }

	result = iListBox->OfferKeyEventL(aKeyEvent, aType);

    /* FIX for  KBAA-7R3AXC bug */
	if (result == EKeyWasNotConsumed && IsFocused())
	  {
		  // Ignore Send Key up and down events to prevent Dialer appearance
		  // on top of cca application.
		  if ((aType == EEventKeyDown || aType == EEventKeyUp)
			  && aKeyEvent.iScanCode == EStdKeyYes)
		  {
			  result = EKeyWasConsumed;
		  }
	  }

	return result;
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::SizeChanged
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::SizeChanged()
	{
    TRect drawRect( Rect() );
    
    // Listbox and header
    if (!Layout_Meta_Data::IsLandscapeOrientation()) 
        {// Portrait
        AknLayoutUtils::LayoutControl(
            iListBox, drawRect, AknLayoutScalable_Apps::cl_listscroll_pane(0));
        TRect listRect = iListBox->Rect();
        listRect.iTl.iX = Rect().iTl.iX;
        listRect.iBr.iX = Rect().iBr.iX;
        iListBox->SetRect(listRect);
        TAknLayoutRect headerRect;
        headerRect.LayoutRect(Rect(), AknLayoutScalable_Apps::cl_header_pane(0));
        iHeaderCtrl->SetRect(headerRect.Rect());
        }
    else 
        {// Landscape
        AknLayoutUtils::LayoutControl(
                iListBox, drawRect, AknLayoutScalable_Apps::cl_listscroll_pane(1));
        TRect listRect = iListBox->Rect();
        listRect.iBr.iX = Rect().iBr.iX;
        iListBox->SetRect(listRect);
        TAknLayoutRect headerRect;
        headerRect.LayoutRect(Rect(), AknLayoutScalable_Apps::cl_header_pane(1));
        iHeaderCtrl->SetRect(headerRect.Rect());
        }
   // Background skin
    if ( iBackground )
        {
        iBackground->SetRect( drawRect );
        }
            
    DrawNow();
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::HandleResourceChange
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::HandleResourceChange(TInt aType)
	{
	CCCAppViewPluginAknContainer::HandleResourceChange(aType);
	}

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::GetHelpContext
// -----------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::GetHelpContext(TCoeHelpContext& aContext ) const
	{
	TUid KHelpUid = { KHELPUID };
	aContext.iMajor =  KHelpUid;
	aContext.iContext = KSERVTAB_HLP_CONTACT_DETAILS;
	}

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::HandleListBoxEventL(
    CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/ )
    {
    // Handle pointer events
    }
	
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::PresenceChangedL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::PresenceChangedL()
	{
	iListBox->HandleItemRemovalL();
	iListBoxModel->AddFieldsDataL();
	iHeaderCtrl->LabelsSizeChangedL();
	iHeaderCtrl->HeaderSizeChangedL();
	} 
	
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::GetTitleTextL
// --------------------------------------------------------------------------
//
HBufC* CVIMPSTDetailsViewContainer::GetTitleTextLC( TStoreType aStoreType )
	{
	TInt count = 0;
	HBufC* titleText = NULL;
	
	if( iListBoxModel )
		{
		count = iListBoxModel->FieldsCount();	
		}
	if( aStoreType == EStoreLocal &&  count >0 )
		{
		CVIMPSTFieldPropertyArray& fieldArry = iListBoxModel->FieldsArray();
		
		CVIMPSTFieldProperty* field = fieldArry.GetFieldById( EVIMPSTFieldIdFirstName );
		TPtrC firstName = field ? field->FieldDataL() : KNullDesC;
		
		field = fieldArry.GetFieldById( EVIMPSTFieldIdLastName );
		TPtrC lastName = field ? field->FieldDataL() : KNullDesC;
		if( firstName.Length() || lastName.Length() )
			{
			titleText = HBufC::NewLC( firstName.Length() + lastName.Length() +1 ); // pushed on clean up stack
			if( titleText)
				{
				TPtr titleTextPtr( titleText->Des() );
				titleTextPtr.Append(firstName);
				titleTextPtr.Append(KSpace);
				titleTextPtr.Append(lastName);	
				}
			}
		}
	else if( count >0)
		{
		CVIMPSTFieldPropertyArray& fieldArry = iListBoxModel->FieldsArray();
		CVIMPSTFieldProperty* field = fieldArry.GetFieldById( EVIMPSTFieldIdImpp );
		TPtrC title = field ? field->FieldDataL() : KNullDesC;
		titleText = iListBoxModel->DisplayId( title ).AllocLC(); // pushed on clean up stack	
		}
	return titleText;
	}	  
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::CreatePresentationL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::CreatePresentationL(MVPbkStoreContact& aContact,TDesC& aServiceName, TStoreType aStoreType )
	{
	iListBoxModel->CreateListPresentationL( aContact,
										   iFocusedListIndex,
										   aServiceName, aStoreType);
	
	iHeaderCtrl->CreateHeaderPresentationL( aContact );
	iHeaderCtrl->HeaderSizeChangedL();	
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewContainer::CreateEmptyPresentationL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewContainer::CreateEmptyPresentationL()
	{
	if( !iListBoxModel->MdcaCount() ) 
		{
	 	iListBoxModel->SetEmptyTextsToListboxL();
		iHeaderCtrl->ClearL(); // clear the header data	
		}
	}		
// End of File





