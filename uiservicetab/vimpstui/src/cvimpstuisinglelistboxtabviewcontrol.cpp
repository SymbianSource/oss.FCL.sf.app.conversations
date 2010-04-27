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
* Description:  Implementation for CVIMPSTUiDoubleListBoxTabViewControl.
*
*/


// INCLUDE FILES
#include "cvimpstuisinglelistboxtabviewcontrol.h"

#include "cvimpstuisinglelistboxarray.h"
#include "tvimpstenums.h"
#include "mvimpstcmdhandler.h"
#include "cvimpstuibranddata.h"
#include "imcvuiliterals.h"
#include "cvimpstuilistboxmodel.h"
#include "vimpstutilsdialog.h"
#include "vimpstui.hrh"
#include "cvimpstuitabbedview.h"
#include "mvimpstengine.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"

// system include
#include 	<aknenv.h>
#include	<aknlists.h>
#include    <AknIconArray.h>
#include	<eikclbd.h> 
#include	<aknsfld.h> 
#include 	<AknUtils.h> 
#include	<AknLayout.lag>   
#include 	<StringLoader.h>
// Pbk2
#include <MPbk2KeyEventHandler.h>
#include <vimpstuires.rsg>


#include    <MVPbkContactStoreListObserver.h>
#include    "mvimpstenginepresencesubservice.h"
#include "uiservicetabtracer.h"

#include <aknlayoutscalable_avkon.cdl.h>
// imlauncher
#include <imcvlauncher.h>
const TInt KTextLimit( 40 ); // Text-limit for find-field

const TInt KMinContact(1);   // minmimum number of contacts to show findpane

// ================= MEMBER FUNCTIONS =======================
// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleAvatarChangeL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleAvatarChangeL( const TDesC& /*aUserId*/ ) 
    {
  
    }   

// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::CVIMPSTUiDoubleListBoxTabViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiSingleListBoxTabViewControl::CVIMPSTUiSingleListBoxTabViewControl(CVIMPSTUiTabbedView& aTabbedView, MPbk2KeyEventHandler* aKeyEventHandler,
														MVIMPSTCmdHandler& aCommandHandler,
														TUint32 aServiceId,
														CVIMPSTUiBrandData& aBrandHandler,
														MVIMPSTEngine& aEngine )
	:iTabbedView(aTabbedView),
	 iKeyEventHandler(aKeyEventHandler),
	 iCommandHandler(aCommandHandler),
	 iArrayProcess(iCommandHandler.GetProcessInterface()),
	 iServiceId(aServiceId),
	 iBrandHandler(aBrandHandler),
	 iEngine(aEngine)
    {

    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::ConstructL()
    {
	TRACER_AUTO;
    CreateWindowL();
   
    // register to get the call back for any array data change
     iArrayProcess.AddObserver(this); 
    
    // Create and construct listbox
    iListBox = new ( ELeave ) CVIMPSTUiSingleStyleListBox;
 
    iListBox->ConstructL(this);
    
    iListBox->SetContactListModelL(iArrayProcess);
    
    iListBox->SetListBoxObserver( this );
    
    iListBox->SetContainerWindowL(*this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL
        (CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	// set marquee on
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
    // If engine is uninstalled, do not construct the list view.
    // Display empty message
    if (iEngine.IsUnInstalled())
        {
        SetListEmptyTextL( R_QTN_SERVTAB_SWUPDATE_RESTART );
        return;
        }
    else
        {
    // Construction of the listbox view. Engine is not uninstalled.    
        CVIMPSTUiSingleListboxArray* friendsArray =
                CVIMPSTUiSingleListboxArray::NewL(iArrayProcess,
                        iListBox->ItemDrawer()->ColumnData(), *iListBox);
        // now set the array
        iListBox->Model()->SetItemTextArray(friendsArray);

        iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);
        // if there is any contact other than owndata , show findpane
        if (iListBox->Model()->NumberOfItems() > KMinContact)
            {
            // Create find-pane
            ActivateFindPaneL();
            }
        LoadBitmapsL();
        SetCbaLockL(EFalse);
        }

    
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSingleListBoxTabViewControl* CVIMPSTUiSingleListBoxTabViewControl::NewL
        (CVIMPSTUiTabbedView& aTabbedView,MPbk2KeyEventHandler* aKeyEventHandler, 
        MVIMPSTCmdHandler& aCommandHandler, TUint32 aServiceId,
        CVIMPSTUiBrandData& aBrandHandler,
        MVIMPSTEngine& aEngine)
    {
	TRACER_AUTO;
    CVIMPSTUiSingleListBoxTabViewControl* self = NewLC(aTabbedView,aKeyEventHandler,
    			aCommandHandler, aServiceId, aBrandHandler,aEngine);
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::NewLC
// --------------------------------------------------------------------------
//
CVIMPSTUiSingleListBoxTabViewControl* CVIMPSTUiSingleListBoxTabViewControl::NewLC
        (CVIMPSTUiTabbedView& aTabbedView,MPbk2KeyEventHandler* aKeyEventHandler,
        MVIMPSTCmdHandler& aCommandHandler, TUint32 aServiceId,
        CVIMPSTUiBrandData& aBrandHandler,
        MVIMPSTEngine& aEngine)
    {
	TRACER_AUTO;
    CVIMPSTUiSingleListBoxTabViewControl* self =
        new (ELeave) CVIMPSTUiSingleListBoxTabViewControl(aTabbedView,aKeyEventHandler,
        aCommandHandler, aServiceId, aBrandHandler,aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::~CVIMPSTUiSingleListBoxTabViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiSingleListBoxTabViewControl::~CVIMPSTUiSingleListBoxTabViewControl()
    {
    
    delete iListBox;
    iArrayProcess.RemoveObserver();
    if( iFindbox )
    	{
    	delete iFindbox;
    	iFindbox = NULL;
    	}
      
    }
	
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleAddition
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleAdditionL(TVIMPSTEnums::TItem aType, TInt aIndex)
	{
  	if(iListBox )
		{
		iListBox->HandleItemAdditionL();
		UpdateViewL(aIndex,aType) ;
		}
	}

// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleAddition
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleDeletionL(TVIMPSTEnums::TItem aType, TInt aIndex )
	{
    if(iListBox)
		{
		iListBox->HandleItemRemovalL();  
		TInt count = iListBox->Model()->NumberOfItems();
		if( count == aIndex )
			{
			aIndex = 0;	
			}
		UpdateViewL(aIndex,aType) ;
		}
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleItemAdditionL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleItemAdditionL()
    {
   if(iListBox)
  	 	{
        iListBox->HandleItemAdditionL();  
        TInt index  = CurrentItemIndex();
    	TVIMPSTEnums::TItem type = iArrayProcess.GetType(index);
       	UpdateViewL(index,type) ;  
   		}
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleItemRemovalL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleItemRemovalL()
    {
    if(iListBox)
  	 	{
        iListBox->HandleItemRemovalL();  
        TInt index  = CurrentItemIndex();
    	TVIMPSTEnums::TItem type = iArrayProcess.GetType(index);
       	UpdateViewL(index,type) ;  
    	}
    }



// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SizeChanged
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SizeChanged()
    {
    SetLayout(); //layouting controls
    if ( iFindbox && iFindPaneIsVisible  )
        {
        iFindbox->MakeVisible( ETrue );
        }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::CountComponentControls
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiSingleListBoxTabViewControl::CountComponentControls() const
    { 
     // return number of controls inside this container
    if( !iFindbox )
        {
        // Find-pane does not exist when there is no data in listbox,
        // so only one control exists (listbox)
        return 1; //Ignore CodeScanner warning
        }
    else
        {
        // listbox and findbox exists. that makes it two
        return 2; //Ignore CodeScanner warning
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::FocusChanged
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::FocusChanged( TDrawNow aDrawNow )
    {
    if ( iFindbox && iFindbox->IsVisible() )
            {
            iFindbox->SetFocus( IsFocused(), aDrawNow );
            if ( aDrawNow == EDrawNow && IsFocused() )
                {
                iFindbox->DrawDeferred();
                }
            }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::ComponentControl
// --------------------------------------------------------------------------
//
CCoeControl* CVIMPSTUiSingleListBoxTabViewControl::ComponentControl(TInt aIndex) const
    {
   // return iListBox;
    switch ( aIndex )
        {
        // the caller wants to have the first control
        // so it's always listbox
        case 0:
            {
			return iListBox;
            }
        // the caller wants to have the second control
        // so it's always the findbox if it exists
        case 1:
            {
            if( iFindbox )
                {
                return iFindbox;
                }
            return NULL;
            }
        default:
            {
            return NULL;
            }
        }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::OfferKeyEventL
// --------------------------------------------------------------------------
//
TKeyResponse CVIMPSTUiSingleListBoxTabViewControl::OfferKeyEventL
        (const TKeyEvent& aKeyEvent,TEventCode aType)
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
    UpdateCbaL();
    TKeyResponse ret = EKeyWasNotConsumed;
    // Offer key event first to the key event handler
    if ( iKeyEventHandler && iKeyEventHandler->Pbk2ProcessKeyEventL
            ( aKeyEvent, aType ) )
        {
        ret = EKeyWasConsumed;
        }
    
    switch ( aKeyEvent.iCode )
       {
       case EKeyBackspace:
           {
           if ( iFindbox )  
               {
               if( iFindbox->TextLength() == 0 )
                   {
                   TVIMPSTEnums::TItem item = iArrayProcess.GetType( CurrentItemIndex() );
                   if(TVIMPSTEnums::EContactItem == item)
                       {
                       TInt index = CurrentItemIndex();
                       if( iArrayProcess.IsConversationExist(index) )
                           {
                           iTabbedView.CloseConversationL();
                           ret = EKeyWasConsumed;
                           }
                       else if(TVIMPSTEnums::EContactItem == iArrayProcess.GetType(  index ) )
                           {
                           if( TVIMPSTEnums::ESVCERegistered == iArrayProcess.GetLoginState()  )                          
                               {
                               iTabbedView.DeleteContactL();
                               }
                           ret = EKeyWasConsumed;
                           }
                       }
                   }
               }
           break;
           }
  
            // Case to handle send key press consumption.
        case EKeyYes:
            {
            TInt index = iListBox->CurrentItemIndex();
            if( TVIMPSTEnums::EOwnStatusItem == iArrayProcess.GetType( index ) )
                {
                ret = EKeyWasConsumed;
                }
            break;
            }
       default:
           break;
       }
    
     if( EKeyPhoneSend == aKeyEvent.iCode  && ret != EKeyWasConsumed )
    	{
    	if( iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EVoip))
		    {
		    iTabbedView.MakeVoipCallL();
		   	ret = EKeyWasConsumed;
		    }
	
    	}
    if( ret == EKeyWasConsumed )
	    {
	    // user might has choosen back/end key/application key etc
	    // application might switch to background 
	    // return from here
	    return ret;	
	    }
    if ( iFindPaneIsVisible && iFindbox ) 
	    {
	    ret = iFindbox->OfferKeyEventL( aKeyEvent, aType )	;
	    }
    // Not consumed by the key handler and findpane , offer to the control next
    if ( ret == EKeyWasNotConsumed )
        {
        ret = iListBox->OfferKeyEventL( aKeyEvent, aType );
        }
    iTabbedView.UpdateToolbarL();
    return ret;
    }
  
// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::CurrentItemIndex
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiSingleListBoxTabViewControl::CurrentItemIndex() const
    {
    if( !iListBox )
        {
        return KErrNotFound;
        }
    if( !iListBox->View() )
        {
        // CurrentItemIndex panics if the listbox has no view
        return KErrNotFound;
        }

    TInt curIndex( iListBox->CurrentItemIndex() );
    if ( curIndex < 0 )
        {
        return KErrNotFound;
        }
    CVIMPSTUiListBoxModel* model =
        static_cast<CVIMPSTUiListBoxModel*>( iListBox->Model() );

    if( model )
        {
        return model->FilteredItemIndex( curIndex );
        }
    return KErrNotFound;
    }



// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetCurrentItemIndex
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SetCurrentItemIndex(TInt aIndex)
    {
    if (iListBox->CurrentItemIndex() != aIndex &&
        aIndex >= 0 &&
        aIndex < iListBox->Model()->NumberOfItems() )
        {
        iListBox->SetCurrentItemIndex(aIndex);
        TRAP_IGNORE(UpdateCbaL());
        }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetCurrentItemIndexAndDraw
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SetCurrentItemIndexAndDraw(TInt aIndex)
    {
    SetCurrentItemIndex(aIndex);
    this->DrawDeferred();
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetListEmptyTextL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SetListEmptyTextL(TInt aResourceId)
    {
    HBufC* msgText;
    TRACER_AUTO;    
    // Get Service Name from Engine , load string from resource and display.
    // This text is shown to tell the user to restart phone to get the service again.
    TPtrC serviceNamePtr(iEngine.ServiceName());
    msgText = StringLoader::LoadLC(aResourceId, serviceNamePtr, iCoeEnv);
    iListBox->View()->SetListEmptyTextL(*msgText);
    TRACE( "Display Text %S", msgText );
    
    CleanupStack::PopAndDestroy(msgText);
    }

// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::LoadBitmapsL
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::LoadBitmapsL()
    {
  
    
	CColumnListBoxData* listBoxData = iListBox->ItemDrawer()->ColumnData();
	if ( !listBoxData )
		{
		//No listbox data
		User::Leave( KErrNotFound );
		}
	
	CAknIconArray* icons = iBrandHandler.LoadIconsLC( );
	
    CArrayPtr<CGulIcon>* oldIconArray = listBoxData->IconArray();
    if( oldIconArray )
        {
        oldIconArray->ResetAndDestroy();
		delete oldIconArray;
        listBoxData->SetIconArray( NULL );
        }

    // Set icon-array to listbox's drawer
    listBoxData->SetIconArray( icons );
    CleanupStack::Pop( icons );
    }

// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleListBoxEventL(
//			CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/ )
// From MEikListBoxObserver, Handles event's generated by listbox
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleListBoxEventL(
        CEikListBox* /*aListBox*/, TListBoxEvent aEventType )
    {
    iTabbedView.UpdateToolbarL();
    UpdateCbaL();   
    switch(aEventType)
        {
        case EEventItemSingleClicked:
            {
    		TBool presenceEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EPresence);
    		TBool imEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EIM);
    		TBool voipEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EVoip);
    		
    		TInt index = CurrentItemIndex();
    		TVIMPSTEnums::TItem itemType = iArrayProcess.GetType(index);    		
    		TVIMPSTEnums::TVIMPSTRegistrationState serviceState = iTabbedView.GetServiceState();
    		
    		if(TVIMPSTEnums::EOwnStatusItem == itemType && 
    		        TVIMPSTEnums::ESVCENotRegistered ==  serviceState)
    			{
    			iTabbedView.HandleCommandL( ECmdLogin );
    			}
    		else if(TVIMPSTEnums::EOwnStatusItem == itemType 
    				&& presenceEnabled &&
    				TVIMPSTEnums::ESVCERegistered ==  serviceState )
    			{
    			iTabbedView.HandleCommandL( ECmdChangeStatus );
    			}
    		else if( ( TVIMPSTEnums::EContactItem == itemType || 
    		           TVIMPSTEnums::EUnknonContactItem == itemType )&& imEnabled &&
    		           TVIMPSTEnums::ESVCERegistered == serviceState )
    		    {
    		    iTabbedView.HandleCommandL( ECmdOpenConversation );
    		    }
    		else if ( itemType == TVIMPSTEnums::EFriendRequestItem
    		        && TVIMPSTEnums::ESVCERegistered == serviceState)
    		    {
    		    iTabbedView.HandleCommandL( ECmdOpenFriendRequest );    		                    
    		    }
            else if( TVIMPSTEnums::EContactItem == itemType )
                  {
                  if( !(TVIMPSTEnums::ESVCEUpdatingContacts ==  serviceState ||
                          TVIMPSTEnums::ESVCENetworkConnecting ==  serviceState || 
                          TVIMPSTEnums::ESVCENetworkDisConnecting ==  serviceState))
                      {
                      iTabbedView.HandleCommandL( ECmdContactDetails );
                      }
                  }
    		break;
    		}
    	       // Add enter key handling
    	                        case EEventEnterKeyPressed:
    	                        {
    	                        CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    	                        TInt commandId( iCba->ButtonGroup()->CommandId( CEikButtonGroupContainer::EMiddleSoftkeyPosition));
    	                       
    	                        iTabbedView.HandleCommandL(commandId);
    	                        break;
    	                        }
    	default:
    		{
    		break;
    		}
    	}
  
       }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SendMessageL(
//			CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/ )
// From MEikListBoxObserver, Handles event's generated by listbox
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SendMessageL()
	{
	TInt index = CurrentItemIndex();   
	TPtrC seletctedItem = iArrayProcess.GetItemUserId(index) ;
	if( index< 0 )
		{
		// return if item not found.
		return;	
		}
      if(seletctedItem.Length())
		{
   // No need to check for the presence of the contact as in singleline 
   // list box there is no presence, hence directly open the ocnversation view.
    TVwsViewId activeViewId;
    CCoeEnv::Static()->AppUi()->GetActiveViewId( activeViewId );
    
    iArrayProcess.ResetPendingMsg( index );
    // imlauncher call for luanching the conversationview with seletced user id
    IMCVLauncher::LaunchImConversationViewL(activeViewId, iServiceId, seletctedItem );  
		}
	else
		{
	      //When there is no XSP Id present it comes to here
		//Displaying a note that there is no XSP ID
	      HBufC* note = NULL;
	      note = StringLoader::LoadLC( R_QTN_SERVTAB_NOXSP_ERROR );
	      VIMPSTUtilsDialog::DisplayNoteDialogL( *note );
		CleanupStack::PopAndDestroy( note );
		}
	}
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::UpdateViewL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::UpdateViewL( 
				TInt aIndex, TVIMPSTEnums::TItem aType)
    {
	TRACER_AUTO;
    TInt index = KErrNotFound;
    if( aIndex < 0 )
    	{
    	aIndex = CurrentItemIndex()<0?0:CurrentItemIndex();		
    	}
    switch(aType)
	    {
	    case TVIMPSTEnums::EContactItem:
	    case TVIMPSTEnums::EOwnStatusItem:
	    	{
	    	/*Not attempting to get the index of contact because aIndex is the 
	    	same as this index and we pass it on to the contactlistboxmodel's 
	    	visibleitem method*/
	    	CVIMPSTUiListBoxModel* list =
	    	        static_cast<CVIMPSTUiListBoxModel*>( iListBox->Model() );
	    	index = list->VisibleItemIndex(aIndex); 
	    	if( index != KErrNotFound  && iFindbox )
		        {
		         if(iClrFindPaneFlag)
	                {
	                iFindbox->SetSearchTextL( KNullDesC );
	                iFindbox->DrawNow();
	                iClrFindPaneFlag = EFalse;
	                }
	            else
	                {
	                TBuf<KTextLimit> temp;
	                iFindbox->GetSearchText(temp);
	                if(0 != temp.Compare(KNullDesC))
	                    {
	                    ((CAknFilteredTextListBoxModel*) iListBox->Model() )
	                            ->Filter()->HandleOfferkeyEventL();
	                    }
	                }		        
		        }	
		    break;     	
	    	}	    	
		default:
			{
			//nothing to do
			break;
			}    	
	    }
	iListBox->DrawNow();
	SetCurrentItemIndex(aIndex);
	SetFocusAndMakeItemVisible();
	TInt count = iArrayProcess.Count();
    if(count > KMinContact && ! iFindPaneIsVisible)  
		{
		ActivateFindPaneL();		
		}
    else if(count <= KMinContact && iFindPaneIsVisible) // extra check needed here to improve performance
	    {
	    DeactivateFindPaneL();	    
	    }
    
	//Update the Toolbar after moving the focus to the current item.
	iTabbedView.UpdateToolbarL();   
	UpdateCbaL();		
    }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::ActivateFindPaneL()
// Activates find-pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::ActivateFindPaneL()
    {
	TRACER_AUTO;
    if( !iFindbox )
        {
        iFindbox = CAknSearchField::NewL( *this,
                CAknSearchField::ESearch,
                NULL, KTextLimit );
        static_cast<CAknFilteredTextListBoxModel*>( iListBox->Model() )
            ->CreateFilterL( iListBox, iFindbox );

        }
    iFindbox->SetObserver( this );
    iFindbox->ActivateL();
    iFindbox->ResetL();
    iFindbox->SetSearchTextL( KNullDesC );    
   
    iFindPaneIsVisible = ETrue;
    SetLayout();

    if( IsActivated() )
        {
        iFindbox->MakeVisible( ETrue );
        iFindbox->SetFocus( ETrue );
        iListBox->SizeChanged();
        DrawNow();
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::DeactivateFindPaneL()
// De-activates find-pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::DeactivateFindPaneL()
    {
	TRACER_AUTO;
    if( !iFindbox )
	    {
	    return;	
	    }
    static_cast<CAknFilteredTextListBoxModel*>( iListBox->Model() )
        ->Filter()->ResetFilteringL();


    iFindbox->MakeVisible( EFalse );
    iFindbox->SetFocus( EFalse );

    iFindPaneIsVisible = EFalse;
    SetLayout();

    if( IsActivated() )
        {
        DrawNow();
        }
    }
    
    
  
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetLayout()
// Layouts current components according to AVKON LAF
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SetLayout()
    {
    const TRect rect( Rect() );
    if( iListBox && iFindPaneIsVisible )
        {

        TAknWindowLineLayout empty;
        empty.iC = 0;
        empty.il = 0;
        empty.it = 0;
        empty.ir = 0;
        empty.ib = 0;
        empty.iW = ELayoutEmpty;
        empty.iH = ELayoutEmpty;

        TAknWindowLineLayout findPane = AknLayoutScalable_Avkon::find_pane().LayoutLine();
        TAknWindowLineLayout listPane;

        // listPane should equal to parent's rect
        listPane.il = Rect().iTl.iX;
        listPane.ir = ELayoutEmpty;
        listPane.it = Rect().iTl.iY;
        listPane.ib = ELayoutEmpty;
        // this check is required since it's not guaranteed that listbox's SetRect
        // has been called when this code is executed
        if ( Size().iWidth > 0 )
            {
            listPane.iW = Size().iWidth;
            listPane.ir = ELayoutEmpty;
            }
        else
            {
            listPane.iW = ELayoutEmpty;
            listPane.ir = 0;
            }
        // but findpane's height should be subtracted
        listPane.iH = Size().iHeight - findPane.iH;

        // findPane's width should equal to listPane's width
        findPane.iW = listPane.iW;
        CAknFilteredTextListBoxModel* m = static_cast <CAknFilteredTextListBoxModel*> ( iListBox->Model() );
        if ( m->Filter() )
            {
            m->Filter()->SetParentControl( this );
            }
        TAknLayoutRect lrect;
        lrect.LayoutRect( Rect(), empty );
        AknLayoutUtils::LayoutControl( iFindbox, lrect.Rect(), findPane );
        AknLayoutUtils::LayoutControl( iListBox, Rect(), listPane );
                                      
        }
    else if( iListBox )
        {
        AknLayoutUtils::LayoutControl( iListBox,
                                       rect,
                                       AKN_LAYOUT_WINDOW_list_gen_pane( 0 ) );
        }
    }
    
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleControlEventL
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent aEventType)
    {
    // We only observe iFindbox
    if( EEventStateChanged == aEventType )
        {

      	((CAknFilteredTextListBoxModel*) iListBox->Model() )
            ->Filter()->HandleOfferkeyEventL();        

        }
    iListBox->DrawNow(); 
    iTabbedView.UpdateToolbarL();
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::CheckAndSetLastItemFlag()
// Checks if the last contact is focused and Sets the iLastItemFlag
// ---------------------------------------------------------
void CVIMPSTUiSingleListBoxTabViewControl::CheckAndSetLastItemFlag()
    {
    if(iListBox->CurrentItemIndex() == (iArrayProcess.Count() - 1))
        {
        iLastItemFlag = ETrue;
        }
    else
        {
        iLastItemFlag = EFalse;
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetFocusAndMakeItemVisible()
// Compares the Saved Contact in iFocussedID with the contacts in the List and focuses if it is found 
// ---------------------------------------------------------
void CVIMPSTUiSingleListBoxTabViewControl::SetFocusAndMakeItemVisible()
    {  
    TInt count = iArrayProcess.Count();
    TInt index = CurrentItemIndex();
    if( index == count )
        {   
        if( iLastItemFlag )
            {
            index = count - 1;
            iLastItemFlag = EFalse;
            }
        else
            {
            return;//do nothing let list box handle this.
            }
        }

    CVIMPSTUiListBoxModel* model =
        static_cast<CVIMPSTUiListBoxModel*>( iListBox->Model() );

    if( model )
        {
        index = model->VisibleItemIndex( index );
        }
    
    iListBox->ScrollToMakeItemVisible( index );
    SetCurrentItemIndex(index);
    iListBox->DrawNow();

    }

// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::GetFocussedId()
// To get the pointer to the iFocussedID
// ---------------------------------------------------------
TPtrC CVIMPSTUiSingleListBoxTabViewControl::GetFocussedId()
    {
    TInt index = CurrentItemIndex();
    if( index < 0 )
	    {
	    index = 0;	
	    }
    return iCommandHandler.GetProcessInterface().GetItemUserId(  index );
    }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetFocussedId()
// Saves the Contact ID in iFocussedID
// ---------------------------------------------------------
void CVIMPSTUiSingleListBoxTabViewControl::SetFocussedId(const TDesC& aContact)
    {
    TInt index = iCommandHandler.GetProcessInterface().GetSelectedItemIndex( aContact );
    if( index < 0 )
	    {
	    index = 0;	
	    }
    SetCurrentItemIndex( index );
    SetFocusAndMakeItemVisible();
    }

// --------------------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetClrFindPaneFlag(TBool aFlag)
// --------------------------------------------------------------------

void CVIMPSTUiSingleListBoxTabViewControl::SetClrFindPaneFlag(TBool aFlag)
    {
    iClrFindPaneFlag = aFlag;
    }
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::CoeControl()
// ---------------------------------------------------------

CCoeControl* CVIMPSTUiSingleListBoxTabViewControl::CoeControl()
    { 
    return this; // return this pointer since its a parent class pointer of  CCoeControl
    }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::SetCbaLockL()
// Sets the lock status of cba keys.
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::SetCbaLockL( TBool aLock )
    {
    iCbaLock = aLock;
    if( !aLock )
        {
        UpdateCbaL();
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::UpdateCbaL()
// Updates Softkeys according to current focus.
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListBoxTabViewControl::UpdateCbaL( TBool aUseDefaultCba /*= EFalse*/ )
    {
	TRACER_AUTO;
    TInt cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__EMPTY;
    iCurrentCmdToExe = -1;
    if( !iCbaLock )
        {
        TInt index = CurrentItemIndex();
        //if invalid index then dont set any command
        if(KErrNotFound == index)
            {
            if ( iCba )
                {               
                iCba->SetCommandSetL( cbaRes );
                iCba->DrawNow();                
                }
            return; 
            }
        
        TVIMPSTEnums::TItem itemtype = iArrayProcess.GetType(index);
        TVIMPSTEnums::TVIMPSTRegistrationState servicestate = iEngine.ServiceState();
        TBool updateinvalid( iLastFocusedItemType == TVIMPSTEnums::EInvalid );
         if ( !iCba )
            {
            iCba = CEikButtonGroupContainer::Current();
            }

        // default softkeys
        if( aUseDefaultCba || updateinvalid )
            {
            if( aUseDefaultCba )
                {
                iLastFocusedItemType = TVIMPSTEnums::EOwnStatusItem;
                }
            else
                {
                iLastFocusedItemType = itemtype;
                }

            if( iCba )
                {
               if ( itemtype == TVIMPSTEnums::EOwnStatusItem &&  TVIMPSTEnums::ESVCENetworkConnecting == servicestate )
                    {
                    if(iTabbedView.CancelLoginSupportedL())
                        {
                        cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CANCEL__EXIT;
                        iCurrentCmdToExe = ECmdCancelLogin;
                        }
                    else
                        {
                        cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__EMPTY;
                        iCurrentCmdToExe = -1;
                        }
                    }
                    else if ( itemtype == TVIMPSTEnums::EOwnStatusItem &&  TVIMPSTEnums::ESVCEWaitingForNetwork == servicestate )
                        {
                        cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CHANGECONN__EXIT;
                        iCurrentCmdToExe = ECmdChangeConnectioninOnlineState;
                        }
                    else 
                      {
                      cbaRes = R_SERVTAB_SOFTKEYS_OPTION_LOGIN__EXIT;
                      iCurrentCmdToExe = ECmdLogin;
                      }
                iCba->SetCommandSetL( cbaRes );
                iCba->DrawNow();
                }
            }

        // collapse softkey ( when focus is on contact )
        else if( updateinvalid || itemtype == TVIMPSTEnums::EContactItem )
            {
            iLastFocusedItemType = itemtype;
            if( iCba )
                {
                // Options/Collapse/Chat
                if(TVIMPSTEnums::ESVCERegistered == servicestate)
                    {
                    // im is supported
                    if(iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM))
                        {
                        iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTION_OPENCONV__EXIT );
                        iCurrentCmdToExe = ECmdOpenConversation;
                        }
                    else //im is not supporteed
                        {
                        iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTIONS_DETAILS__EXIT );
                        iCurrentCmdToExe = ECmdContactDetails;
                        }
                    }
                else if(TVIMPSTEnums::ESVCENotRegistered == servicestate)
                    {
                    iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTIONS_DETAILS__EXIT );
                    iCurrentCmdToExe = ECmdContactDetails;
                    }
                iCba->DrawNow();
                }
            }
        else if(  itemtype == TVIMPSTEnums::EUnknonContactItem  )
            {
            iLastFocusedItemType = itemtype;
            if(TVIMPSTEnums::ESVCERegistered == servicestate && 
                    iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM))
                {
                iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTION_OPENCONV__EXIT );
                iCurrentCmdToExe = ECmdOpenConversation;
                }
            else //im is not supporteed
                {
                iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__EMPTY );
                iCurrentCmdToExe = -1;
                }                     
            iCba->DrawNow();   
            }
        // Focus on own contact item
        else if( updateinvalid || itemtype == TVIMPSTEnums::EOwnStatusItem )
            {
            iLastFocusedItemType = itemtype;
            if( iCba )
                {
                if ( itemtype == TVIMPSTEnums::EOwnStatusItem &&  TVIMPSTEnums::ESVCERegistered == servicestate )
                    {
                    // Own status
                   // Options/Back/Change status
                   //Get Presence SubService           
                   MVIMPSTEngineSubService* subService =          
                               (iEngine.SubService(TVIMPSTEnums::EPresence));
                   TBool changeownstatussupported = EFalse;
                   if(subService)
                       {
                       MVIMPSTEnginePresenceSubService& presence = 
                               MVIMPSTEnginePresenceSubService::Cast (*subService);
                       changeownstatussupported = presence.IsChangeOwnStatusSupported();
                       }
                   if(changeownstatussupported)
                       {
                       cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__CHANGESTATUS;
                       iCurrentCmdToExe = ECmdChangeStatus;
                       }
                   //else its cbres is set to options exit empty
                    }
              else if ( itemtype == TVIMPSTEnums::EOwnStatusItem &&  TVIMPSTEnums::ESVCENetworkConnecting == servicestate )
                    {
                    if(iTabbedView.CancelLoginSupportedL())
                        {
                        cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CANCEL__EXIT;
                        iCurrentCmdToExe = ECmdCancelLogin;
                        }
                    else
                        {
                        cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__EMPTY;
                        iCurrentCmdToExe = -1;
                        }
                    }
              else if ( itemtype == TVIMPSTEnums::EOwnStatusItem &&  TVIMPSTEnums::ESVCEWaitingForNetwork == servicestate )
                    {
                    cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CHANGECONN__EXIT;
                    iCurrentCmdToExe = ECmdChangeConnectioninOnlineState;
                    }
              else 
                  {
                  cbaRes = R_SERVTAB_SOFTKEYS_OPTION_LOGIN__EXIT;
                  iCurrentCmdToExe = ECmdLogin;
                  }
                //add for connecting state and waiting for connection state.
                iCba->SetCommandSetL( cbaRes );
                iCba->DrawNow();
                }
            }
        }
    }
    
// ---------------------------------------------------------
// CVIMPSTUiSingleListBoxTabViewControl::HandleAvatarRefreshL()
// ---------------------------------------------------------
void CVIMPSTUiSingleListBoxTabViewControl::HandleAvatarRefreshL( const TDesC&/* aUserId */,TBool /*aBlock*/)
	{
	//no avatar implementation for single list box.
	}

// End of File
