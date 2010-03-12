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
*  Description : Implementation for cvimpstuiblockviewcontrol.
 *
*/

// INCLUDE FILES
#include "cvimpstuiblockviewcontrol.h"
#include "cvimpstuiblockview.h"
#include "vimpstutils.h"
#include "tvimpstconsts.h"
#include "vimpstui.hrh"

// system include
#include	<aknlists.h>
#include	<eikclbd.h> 
#include <vimpstuires.rsg>

#include <MPbk2KeyEventHandler.h>


// ================= MEMBER FUNCTIONS =======================


// --------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::CVIMPSTUiBlockViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiBlockViewControl::CVIMPSTUiBlockViewControl( CVIMPSTUiBlockView& aBlockedView,MPbk2KeyEventHandler& aKeyEventHandler,
													   	MVIMPSTEngine& aEngine,
													   	RPointerArray<HBufC>* aBlockedList)
	:iBlockedView(aBlockedView),
	 iKeyEventHandler(aKeyEventHandler),
	 iEngine( aEngine ),
	 iBlockedList(aBlockedList)
    {
    
    }


// --------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::ConstructL()
    {
    CreateWindowL();
    iItemArray = new ( ELeave ) CDesCArrayFlat( KArrayGranularity );   
    // Create listbox
    iListBox = new ( ELeave ) CAknSingleGraphicStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    
    iListBox->SetContainerWindowL(*this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL
        (CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
        
    // set marquee on
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
   	iListBox->Model()->SetItemTextArray( iItemArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	
	iListBox->SetListBoxObserver( this );
	
	iCurrentItemIndex = 0;
	UpdateBlockedListL();
	ActivateL();

    }


// --------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiBlockViewControl* CVIMPSTUiBlockViewControl::NewL( CVIMPSTUiBlockView& aBlockedView,
        								    MPbk2KeyEventHandler& aKeyEventHandler, 
        									MVIMPSTEngine& aEngine,
        									RPointerArray<HBufC>* aBlockedList )
    {
    CVIMPSTUiBlockViewControl* self = NewLC(aBlockedView,aKeyEventHandler,aEngine, aBlockedList );
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::NewLC
// --------------------------------------------------------------------------
//
CVIMPSTUiBlockViewControl* CVIMPSTUiBlockViewControl::NewLC( CVIMPSTUiBlockView& aBlockedView,
									          MPbk2KeyEventHandler& aKeyEventHandler,
									          MVIMPSTEngine& aEngine,
									          RPointerArray<HBufC>* aBlockedList )
    {
    CVIMPSTUiBlockViewControl* self =
        new (ELeave) CVIMPSTUiBlockViewControl(aBlockedView,aKeyEventHandler, aEngine, aBlockedList );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTUiBlockViewControl::~CVIMPSTUiBlockViewControl()
    {
   
	delete iItemArray;
	iItemArray = NULL;
    delete iListBox;
    iListBox = NULL;
	}

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::HandleControlEventL
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    }


// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::HandleListBoxEventL
// Handles list box events
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::HandleListBoxEventL( CEikListBox* /*aListBox*/,
												 TListBoxEvent aEventType )
	{
	iCurrentItemIndex = iListBox->CurrentItemIndex();
	if(aEventType == EEventEnterKeyPressed)
	    {
	    iBlockedView.HandleCommandL(ECmdUnBlock);                          
	    }
	}


// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::SizeChanged()
// Called by framework
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::SizeChanged()
	{
	if ( iListBox )
        {
        iListBox->SetRect(Rect());
        }  
    }

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::CountComponentControls()
// Returns number of controls
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiBlockViewControl::CountComponentControls() const
	{
	//Only one compound control so returning 1
	return 1;
	}

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::ComponentControl( TInt aIndex )
// Returns control for given index
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeControl* CVIMPSTUiBlockViewControl::ComponentControl( TInt aIndex ) const
	{
	if( aIndex == 0 )
		{
        return iListBox;
        }
    else
        {
        return NULL;
    	}
	}


// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::UpdateBlockedListL()
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::UpdateBlockedListL()
    {
	iItemArray->Reset();
	
	TInt granularity( iItemArray->Count() );
	++granularity;
	
   	CDesCArray* tempArray = new ( ELeave ) CDesCArrayFlat( granularity );
	CleanupStack::PushL( tempArray );
    
    iListBox->Model()->SetItemTextArray( tempArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	iListBox->HandleItemAdditionL();
	for ( TInt i =0; i < iBlockedList->Count() ; ++ i)
	    {
	    TInt sizeOfItemText;
	    // Calculating the size of the text that will be entered into listbox.
	    // 3 * KTab().Length() calculates the length of the tabs that is 
	    // appended to the actual text.
	    sizeOfItemText = (*iBlockedList)[i]->Length() + 3 * KTab().Length();
	    HBufC* itemText = HBufC::NewLC( sizeOfItemText ); // changing stuff..
	    TPtr itemTextPtr( itemText->Des() );
	    itemTextPtr.Append( KTab );
	    itemTextPtr.Append( (*iBlockedList)[i]->Des());
	    itemTextPtr.Append( KTab );
	    itemTextPtr.Append( KTab );

	    iItemArray->AppendL( itemTextPtr );
	    CleanupStack::PopAndDestroy( itemText );
	    }
	CleanupStack::PopAndDestroy(tempArray );  // tempArray
	
	iItemArray->Sort();
	iListBox->Model()->SetItemTextArray( iItemArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iListBox->HandleItemAdditionL();

    if(0==iBlockedList->Count())
	    {
	    SetEmptyTextsToListboxL();	
	    }
	    
	if ( iItemArray->MdcaCount() > 0 )
		{
		iListBox->SetCurrentItemIndexAndDraw( 0 );
		}
    iListBox->UpdateScrollBarsL();
    SetCurrentIndex(iCurrentItemIndex);
    UpdateCbaL();
    }
    
    
// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::ListBox()
// (other items were commented in a header).
// ---------------------------------------------------------
//
CEikListBox* CVIMPSTUiBlockViewControl::ListBox() const
	{
	return iListBox;
	}

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::GetFocusedItemText
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* CVIMPSTUiBlockViewControl::GetFocusedContactLC() const
	{
	HBufC* retText = NULL;
	TInt curIndex = iListBox->CurrentItemIndex();
	if(KErrNotFound != curIndex)
	    {
	    TPtrC ptr= iItemArray->MdcaPoint(curIndex);
	    ptr.Set(ptr.Mid(1));  // set pointer from next char :since first char is KTab
        TInt pos=ptr.FindC(KTab);
        if( pos !=KErrNotFound &&  (pos != ptr.Length()-1))
            {
            ptr.Set(ptr.Mid( 0, pos));
            retText = ptr.AllocLC();
            }
	    }
    return retText;
	}

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::GetCurrentIndex
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiBlockViewControl::GetCurrentIndex() const
	{
	return iCurrentItemIndex ;
	}

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::GetCurrentIndex
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::SetCurrentIndex(TInt aIndex)
	{
	if(aIndex>0)
		{
	    TInt blockedCount = iListBox->Model()->NumberOfItems();
		if(blockedCount > 0)
			{
			TInt itemIndex = aIndex % blockedCount;
			iListBox->SetCurrentItemIndex(itemIndex);
			}
		}
	}


// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::UpdateCbaL()
// Updates Softkeys according to current focus.
// ---------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::UpdateCbaL()
    {
    TInt cbaRes = R_SERVTAB_SOFTKEYS_EMPTY_EMPTY__BACK;    
    if ( !iCba ) 
	    {
	    iCba = CEikButtonGroupContainer::Current();
	    }

  	if( iCba )
        {
        iCba->SetCommandSetL( cbaRes );
     	iCba->DrawNow();
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiBlockViewControl::OfferKeyEventL()
// Handles user key inputs
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CVIMPSTUiBlockViewControl::OfferKeyEventL( const TKeyEvent& aKeyEvent,
													TEventCode aType )
	{
	if ( !aKeyEvent.iCode ) 
	        {
	        //The character code generated 
	        //for an EEventKey, or 0 for a down or up event.
	        //For down or up key return key consumed.. handle only the events
	        
	        if((EEventKeyDown == aType) && (EStdKeyDevice3 == aKeyEvent.iScanCode))
	            {
	            //for setting the command to be executed for the "Selection key"
	            //in case of selection key, container's OfferKeyEventL() is called only for EEventKeyDown and EEventKeyUp
	            //and not for EEventKey
	            UpdateCbaL();
	            }
	        return EKeyWasConsumed;         
	        }
	TKeyResponse result = EKeyWasNotConsumed;
	if (iListBox)
		{
		iCurrentItemIndex = iListBox->CurrentItemIndex();
		}
    // Offer key event first to the key event handler
    if ( (&iKeyEventHandler ) && iKeyEventHandler.Pbk2ProcessKeyEventL
            ( aKeyEvent, aType ) )
        {
        result = EKeyWasConsumed;
        }
    switch(aKeyEvent.iCode )
        {
      /*  case EKeyEnter:
           {
           iBlockedView.HandleCommandL(ECmdUnBlock);
           result = EKeyWasConsumed;
           break;
           } */
        default:
           {
           break;
           }
        }
    if( result == EKeyWasConsumed )
       {
       // user might has choosen back/end key/application key etc
       // application might switch to background 
       // return from here
       return result; 
       }
    // Not consumed by the key handler, offer to the control next
    if ( result == EKeyWasNotConsumed )
        {
        if ( iListBox )
            {
            result = iListBox->OfferKeyEventL(aKeyEvent, aType);
            }
        }
    UpdateCbaL();
    return result;
    
	}

// ---------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::SetEmptyTextsToListboxL()
// See header for details. 
// ---------------------------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::SetEmptyTextsToListboxL()
    {    
    HBufC* emptyFinal = VIMPSTUtils::LoadResourceL(R_SERVTAB_BLOCKLIST_BKGD);
    CleanupStack::PushL(emptyFinal);
    iListBox->View()->SetListEmptyTextL( *emptyFinal );
    CleanupStack::PopAndDestroy(emptyFinal);
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTUiBlockViewControl::SetUpdatingTextsToListboxL()
// See header for details. 
// ---------------------------------------------------------------------------
//
void CVIMPSTUiBlockViewControl::SetUpdatingTextsToListboxL()
    {    
    HBufC* updating = VIMPSTUtils::LoadResourceL(R_SERVTAB_BLOCKLIST_BKGDTO);
    CleanupStack::PushL(updating);
    iListBox->View()->SetListEmptyTextL( *updating );
    CleanupStack::PopAndDestroy(updating);
    }
    

//  End of File
