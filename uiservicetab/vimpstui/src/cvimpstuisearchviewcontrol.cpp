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
* Description:  Implementation for CVIMPSTUiSearchViewControl.
*
*/



// INCLUDE FILES
#include "cvimpstuisearchviewcontrol.h"

#include "mvimpstcmdhandler.h"
#include "vimpstutils.h"
#include "tvimpstconsts.h"
#include "mvimpstengine.h"
#include "cvimpstuisearchview.h"
#include "vimpstui.hrh"
#include "vimpstutilsdialog.h"
#include    "vimpstdebugprint.h" 
// imlauncher
#include <imcvlauncher.h>


// system include
#include 	<aknenv.h>
#include	<aknlists.h>
#include    <AknIconArray.h>
#include 	<AknUtils.h> 
#include 	<StringLoader.h>

// Pbk2
#include <MPbk2KeyEventHandler.h>
#include <vimpstuires.rsg>



// ================= MEMBER FUNCTIONS =======================


// --------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::CVIMPSTUiSearchViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchViewControl::CVIMPSTUiSearchViewControl( MPbk2KeyEventHandler* aKeyEventHandler,
														TUint32 aServiceId,
													   	MVIMPSTEngine& aEngine,
													   	MVIMPSTCmdHandler& aCommandHandler,
													   	CVIMPSTUiSearchView& aSearchView )
	:iKeyEventHandler(aKeyEventHandler),
	 iServiceId(aServiceId),
	 iEngine( aEngine ),
	 iCommandHandler( aCommandHandler ),
	 iSearchView( aSearchView )
    {
    
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::ConstructL()
    {
    CreateWindowL();
    iItemArray = new ( ELeave ) CDesCArrayFlat( KArrayGranularity );   
    // Create listbox
    iListBox = new ( ELeave ) CAknDoubleStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    
    //iListBox->SetListBoxObserver( this );
    
    iListBox->SetContainerWindowL(*this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL
        (CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
        
    // set marquee on
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
   	iListBox->Model()->SetItemTextArray( iItemArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	
	iListBox->SetListBoxObserver( this );
			
	ActivateL();

    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchViewControl* CVIMPSTUiSearchViewControl::NewL
        								  ( MPbk2KeyEventHandler* aKeyEventHandler, 
        									TUint32 aServiceId,
        									MVIMPSTEngine& aEngine,
        									MVIMPSTCmdHandler& aCommandHandler,
        									CVIMPSTUiSearchView& aSearchView )
    {
    CVIMPSTUiSearchViewControl* self = NewLC(aKeyEventHandler,
    										 aServiceId, aEngine, 
    										 aCommandHandler, aSearchView );
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::NewLC
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchViewControl* CVIMPSTUiSearchViewControl::NewLC
									        ( MPbk2KeyEventHandler* aKeyEventHandler,
									          TUint32 aServiceId,
									          MVIMPSTEngine& aEngine,
									          MVIMPSTCmdHandler& aCommandHandler,
									          CVIMPSTUiSearchView& aSearchView )
    {
    CVIMPSTUiSearchViewControl* self =
        new (ELeave) CVIMPSTUiSearchViewControl(aKeyEventHandler,
        									 aServiceId, aEngine,
        									 aCommandHandler, aSearchView);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTUiSearchViewControl::~CVIMPSTUiSearchViewControl()
    {
   
	delete iItemArray;
    delete iListBox;
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::SearchResultL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::SearchResultL(RArray<TVIMPSTSearchData> aSearchData )
    {
   	UpdateListBoxL(aSearchData);
    }


// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::SearchFinishedL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::SearchFinishedL(TInt /*aError*/)
    {
    // if any error show error note
    if( ! iItemArray->Count() )
	    {
	    SetEmptyTextsToListboxL();	
	    iListBox->DrawNow();
	    }
	UpdateCbaL();
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::HandleControlEventL
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent aEventType)
    {
    // We only observe iFindbox
    if( EEventStateChanged == aEventType )
        {

      	((CAknFilteredTextListBoxModel*) iListBox->Model() )
            ->Filter()->HandleOfferkeyEventL();        
        
        }
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::SetEmptyTextsToListboxL()
// See header for details. 
// ---------------------------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::SetEmptyTextsToListboxL()
    {    
    HBufC* emptyFinal = VIMPSTUtils::CombineStringFromResourceLC
    									( R_QTN_CHAT_EMPTY_SEARCH_PRIMARY, 
    									  R_QTN_CHAT_EMPTY_SEARCH);
    				
    iListBox->View()->SetListEmptyTextL( *emptyFinal );
    CleanupStack::PopAndDestroy(); // emptyFinal    
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiSearchViewControl::SetSearchingTextToListboxL()
// See header for details. 
// ---------------------------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::SetSearchingTextToListboxL()
    {    
    iListBox->View()->SetListEmptyTextL( KSpace );
    iListBox->DrawNow();
    }


// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::HandleListBoxEventL
// Handles list box events
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::HandleListBoxEventL( CEikListBox* /*aListBox*/,
												 TListBoxEvent aEventType )
    {
    switch(aEventType)
        {

        case EEventItemSingleClicked:

            {
            TInt curIndex = iListBox->CurrentItemIndex();
            if( curIndex >=0)
                {
            HBufC* contactId = GetFocusedContactLC() ;

            if ( contactId->Length() )
                {
            if( iCommandHandler.GetProcessInterface().FindAnyContact( *contactId ) )
                {
            //contact is already there in contact list
            TInt index = iCommandHandler.GetProcessInterface().FindContactIndexL(*contactId);
            TVIMPSTEnums::TOnlineStatus onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(index);
            if(TVIMPSTEnums::EPending != onlinestatus) //if contact is not remote pending
                {
            //open conversation view
            OpenConversationL( *contactId );   
                }

                }
            else 
                {	
            //ask the query for adding contact
            HBufC* queryTxt = StringLoader::LoadLC( R_QTN_CHAT_REACTIVE_ADD_LIST, *contactId );
            TInt response = VIMPSTUtilsDialog::DisplayQueryDialogL(
                    R_GENERIC_YES_NO_CONFIRMATION_QUERY,
                    *queryTxt );

            if ( ( response == EAknSoftkeyOk ) || ( response == EAknSoftkeyYes ) )
                {
            iSearchView.HandleCommandL( ESearchAddFriends );
                }
            CleanupStack::PopAndDestroy( queryTxt );	 
                }
                }
            CleanupStack::PopAndDestroy( contactId );	 
                }
            break;
            }
            // Add enter key handling
        case EEventEnterKeyPressed:
            {
            CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
            TInt commandId( iCba->ButtonGroup()->CommandId( CEikButtonGroupContainer::EMiddleSoftkeyPosition));

            iSearchView.HandleCommandL(commandId);
            break;
            }
        }
    UpdateCbaL();	  
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::OpenConversationL(
// (other items were commented in a header).
// ---------------------------------------------------------
void CVIMPSTUiSearchViewControl::OpenConversationL(const TDesC& aContactId )
	{
	MVIMPSTProcessArray& arrayProcess = iCommandHandler.GetProcessInterface();
	TInt indexInTabbedView = arrayProcess.FindContactIndexL( aContactId );
	TInt result = EAknSoftkeyOk;

	//get the status from engine.
	TVIMPSTEnums::TOnlineStatus status = arrayProcess.GetOnlineStatusL(indexInTabbedView);

	if( status == TVIMPSTEnums::EOffline )
	    {
	    TBool isConvExist = arrayProcess.IsConversationExistL( aContactId );
	    
	    if( !isConvExist)
		    {
		
			HBufC* msg = StringLoader::LoadLC( R_QTN_CHAT_CONTACT_MAY_NOT_RECEIVE_MESSAGE,
										        aContactId );                    

			 result = VIMPSTUtilsDialog::DisplayQueryDialogL(
			        R_CHAT_CONTACT_MAY_NOT_RECEIVE_MESSAGE_DIALOG,
			        msg->Des() );
			CleanupStack::PopAndDestroy( msg );
		    }
	    }

	if( ( result == EAknSoftkeyOk ) || ( result == EAknSoftkeyYes ) )
		{
		TVwsViewId activeViewId;
	    CCoeEnv::Static()->AppUi()->GetActiveViewId( activeViewId );
	    TPtrC itemName = arrayProcess.GetItemNameText(indexInTabbedView) ;
	    // not own this p
	    MVPbkContactLink* contactLink = arrayProcess.ContactLink( indexInTabbedView );
		arrayProcess.ResetPendingMsg( indexInTabbedView );
		// imlauncher call for luanching the conversationview with seletced contact
		//IMCVLauncher::LaunchImConversationViewL(activeViewId, iServiceId, aContactId );	
		// imlauncher call for luanching the conversationview with seletced user id
		IMCVLauncher::LaunchImConversationViewL(activeViewId, iServiceId, aContactId, itemName, contactLink);
		}
	}


// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::SizeChanged()
// Called by framework
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::SizeChanged()
	{
	if ( iListBox )
        {
        iListBox->SetRect(Rect());
        }  
    }

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::CountComponentControls()
// Returns number of controls
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiSearchViewControl::CountComponentControls() const
	{
	//Only one compound control so returning 1
	return 1;
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::ComponentControl( TInt aIndex )
// Returns control for given index
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeControl* CVIMPSTUiSearchViewControl::ComponentControl( TInt aIndex ) const
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
// CVIMPSTUiSearchViewControl::OfferKeyEventL()
// Handles user key inputs
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CVIMPSTUiSearchViewControl::OfferKeyEventL( const TKeyEvent& aKeyEvent,
													TEventCode aType )
	{
	TKeyResponse result = EKeyWasNotConsumed;

    // Offer key event first to the key event handler
    if ( iKeyEventHandler && iKeyEventHandler->Pbk2ProcessKeyEventL
            ( aKeyEvent, aType ) )
        {
        result = EKeyWasConsumed;
        }
        
    switch ( aKeyEvent.iCode )
           {
         /*  case EKeyEnter:
				{
				iSearchView.HandleCommandL(iCurrentCmdToExe);
				result = EKeyWasConsumed;
				break;
				}*/
			default:
				break;
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


// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::UpdateListBoxL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::UpdateListBoxL(RArray<TVIMPSTSearchData> aSerachData)
    {
	
	TInt granularity( iItemArray->Count() );
	++granularity;
	
   	CDesCArray* tempArray = new ( ELeave ) CDesCArrayFlat( granularity );
	CleanupStack::PushL( tempArray );
    
    iListBox->Model()->SetItemTextArray( tempArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	iListBox->HandleItemAdditionL();
	for ( TInt i =0; i < aSerachData.Count() ; ++ i)
	    {
	    TVIMPSTSearchData data = aSerachData [i];
	    HBufC* itemText = HBufC::NewLC( 100 );
	    TPtr itemTextPtr( itemText->Des() );

	    itemTextPtr.Append( KTab );

	    itemTextPtr.Append( VIMPSTUtils::DisplayId( data.iEmailId ));
	    itemTextPtr.Append( KTab );

	    itemTextPtr.Append(data.iEmailId);
	    itemTextPtr.Append( KTab );

	    iItemArray->AppendL( itemTextPtr );
	    CleanupStack::PopAndDestroy( itemText );
	    }
    
	CleanupStack::PopAndDestroy(tempArray );  // tempArray
	
	
	iListBox->Model()->SetItemTextArray( iItemArray );
	iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iListBox->HandleItemAdditionL();
    
	if ( iItemArray->MdcaCount() > 0 )
		{
		iListBox->SetCurrentItemIndexAndDraw( 0 );
		}
		
    iListBox->UpdateScrollBarsL();
    
    UpdateCbaL();
       
    }
    
    
// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::ListBox()
// (other items were commented in a header).
// ---------------------------------------------------------
//
CEikListBox* CVIMPSTUiSearchViewControl::ListBox() const
	{
	return iListBox;
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::GetFocusedItemText
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* CVIMPSTUiSearchViewControl::GetFocusedContactLC() const
	{
	HBufC* retText=NULL;
	TInt curIndex = iListBox->CurrentItemIndex();
    TPtrC ptr= iItemArray->MdcaPoint(curIndex);
    ptr.Set(ptr.Mid(1));  // set pointer from next char :since first char is KTab
    
    TInt pos=ptr.FindC(KTab);
   
    if( pos !=KErrNotFound &&  (pos != ptr.Length()-1))
      {
      ptr.Set(ptr.Mid( pos+1, ( ptr.Length()-2-pos )));
      retText = ptr.AllocLC();
      }
    return retText;
	}

// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::ClearListBoxContents()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::ClearListBoxContents()
    {
	iItemArray->Reset();
    TRAPD( err, iListBox->HandleItemRemovalL() );
    if( err != KErrNone )
        {
        CActiveScheduler::Current()->Error( err );
        }
    iListBox->DrawNow();
    }
// ---------------------------------------------------------
// CVIMPSTUiSearchViewControl::UpdateCbaL()
// Updates Softkeys according to current focus.
// ---------------------------------------------------------
//
void CVIMPSTUiSearchViewControl::UpdateCbaL( TBool  aUseDefaultCba /*= EFalse*/ )
    {
    TInt cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_BACK__EMPTY;
    iCurrentCmdToExe = -1;
    TInt index = KErrNotFound;
    if ( !iCba ) 
        {
        iCba = CEikButtonGroupContainer::Current();
        }
    if( iListBox )
	    {
	    index = iListBox->CurrentItemIndex();	
	    }
    if( aUseDefaultCba )  
	    {
	    cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_BACK__EMPTY;
	    }
    else if( KErrNotFound == index )
		{
		cbaRes = R_SERVTAB_SOFTKEYS_OPTION_NEW_SEARCH__BACK;
		iCurrentCmdToExe = ESearchNew;
		}
	else //some results are there, hence update it to add to contacts.
		{
		if(iSearchView.HideAddToContactsL())
		    {			
		    CHAT_DP( D_CHAT_LIT("CVIMPSTUiDoubleListboxArray::HideAddToContactsL after" ) );		
		    HBufC* contactId = GetFocusedContactLC();
		    TInt curIndex = iCommandHandler.GetProcessInterface().FindContactIndexL(*contactId); 
		    TVIMPSTEnums::TOnlineStatus onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(curIndex);   
		    CleanupStack::PopAndDestroy(contactId);
		    if( TVIMPSTEnums::EPending == onlinestatus )
		        {
		        CHAT_DP( D_CHAT_LIT("CVIMPSTUiDoubleListboxArray::EPending in" ) );		
		        cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_BACK__EMPTY;
		        iCurrentCmdToExe = -1;    
		        }
		    else
		        {
		        CHAT_DP( D_CHAT_LIT("CVIMPSTUiDoubleListboxArray::ESearchOpenConversation" ) );		
		        cbaRes = R_SERVTAB_SOFTKEYS_OPTION_SEARCH_OPENCONV__BACK;
		        iCurrentCmdToExe = ESearchOpenConversation;    
		        }

		    }
		else
			{
			cbaRes = R_SERVTAB_SOFTKEYS_OPTION_ADD_TO_CONTACTS__BACK;	
			iCurrentCmdToExe = ESearchAddFriends;
			}   
        }
  	if( iCba )
        {
        iCba->SetCommandSetL( cbaRes );
     	iCba->DrawNow();
        }
    }

//  End of File
