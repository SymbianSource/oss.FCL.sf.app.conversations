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
#include "cvimpstuidoublelistboxtabviewcontrol.h"

#include "cvimpstuidoublelistboxarray.h"
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
#include "cvimpstsettingsstore.h" // settings store in cenrep used for own avatar data
#include "cvimpstengineimagehandler.h"
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

#include <aknlayoutscalable_avkon.cdl.h>
#include "uiservicetabtracer.h"
// imlauncher
#include <imcvlauncher.h>
const TInt KTextLimit( 40 ); // Text-limit for find-field

const TInt KMinContact(1);   // minmimum number of contacts to show findpane

// ================= MEMBER FUNCTIONS =======================

inline CVIMPSTUiDoubleStyleListBox& CVIMPSTUiDoubleListBoxTabViewControl::ListBox() const
    {
    return(*iListBox);
    }
// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::CVIMPSTUiDoubleListBoxTabViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiDoubleListBoxTabViewControl::CVIMPSTUiDoubleListBoxTabViewControl(CVIMPSTUiTabbedView& aTabbedView, MPbk2KeyEventHandler* aKeyEventHandler,
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
// CVIMPSTUiDoubleListBoxTabViewControl::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::ConstructL()
    {
	TRACER_AUTO;
    CreateWindowL();
    // register to get the call back for any array data change
    iArrayProcess.AddObserver(this);  
    // this to make sure that if user id changed storage should be refresh with new user id
    // Initialize the listbox
    // Create and construct listbox
    iListBox = new ( ELeave ) CVIMPSTUiDoubleStyleListBox;
 
    iListBox->ConstructL(this , EAknListBoxSelectionList);
    
    iListBox->SetContactListModelL(iArrayProcess);
    
    iListBox->SetListBoxObserver( this );
    
    iListBox->SetContainerWindowL(*this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL
        (CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	// set marquee on
    iListBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );
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
        CVIMPSTUiDoubleListboxArray* friendsArray =
                CVIMPSTUiDoubleListboxArray::NewL(iArrayProcess,
                        iListBox->ItemDrawer()->ColumnData(), *iListBox,
                        *this);
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
// CVIMPSTUiDoubleListBoxTabViewControl::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiDoubleListBoxTabViewControl* CVIMPSTUiDoubleListBoxTabViewControl::NewL
        (CVIMPSTUiTabbedView& aTabbedView,MPbk2KeyEventHandler* aKeyEventHandler, 
        MVIMPSTCmdHandler& aCommandHandler, TUint32 aServiceId,
        CVIMPSTUiBrandData& aBrandHandler,
        MVIMPSTEngine& aEngine)
    {
	TRACER_AUTO;
    CVIMPSTUiDoubleListBoxTabViewControl* self = NewLC(aTabbedView,aKeyEventHandler,
    			aCommandHandler, aServiceId, aBrandHandler,aEngine);
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::NewLC
// --------------------------------------------------------------------------
//
CVIMPSTUiDoubleListBoxTabViewControl* CVIMPSTUiDoubleListBoxTabViewControl::NewLC
        (CVIMPSTUiTabbedView& aTabbedView,MPbk2KeyEventHandler* aKeyEventHandler,
        MVIMPSTCmdHandler& aCommandHandler, TUint32 aServiceId,
        CVIMPSTUiBrandData& aBrandHandler,
        MVIMPSTEngine& aEngine)
    {
	TRACER_AUTO;
    CVIMPSTUiDoubleListBoxTabViewControl* self =
        new (ELeave) CVIMPSTUiDoubleListBoxTabViewControl(aTabbedView,aKeyEventHandler,
        aCommandHandler, aServiceId, aBrandHandler,aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::~CVIMPSTUiDoubleListBoxTabViewControl
// --------------------------------------------------------------------------
//
CVIMPSTUiDoubleListBoxTabViewControl::~CVIMPSTUiDoubleListBoxTabViewControl()
    {
    delete iListBox;
    iArrayProcess.RemoveObserver();
    if( iFindbox )
    	{
    	delete iFindbox;
    	iFindbox = NULL;
    	}
    }

// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleAvatarChangeL
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleAvatarChangeL( const TDesC& aUserId ) 
    {
	TRACER_AUTO;
    TRACE( "aUserId = %S ", &aUserId );
    CFormattedCellListBoxData* listBoxData = iListBox->ItemDrawer()->ColumnData();
    TPtrC loginUserId = iArrayProcess.LoginUserIdFromStoreL();
    TInt Avindex = 0;
    CGulIcon* newIcon = NULL;
    TRACE( "loginUserId = %S ", &loginUserId );
    if( KErrNone == loginUserId.Compare( aUserId ) )
        {
        TRACE( "inside if ownuserid" );
        TPtrC8 avatardata = iArrayProcess.OwnAvatarContentL();
        TRACE( "after iArrayProcess.OwnAvatarContentL()" );
        Avindex = iArrayProcess.OwnAvatarIndexL();
        if( avatardata.Length() )
            {
            TRACE( "avatardata.Length()" );
            newIcon = AvatarToIconL( avatardata );
            }
        if( newIcon ) // there is a new avatar icon 
            {
            TRACE("newIcon" );
            CArrayPtr<CGulIcon>* currentIconArray = listBoxData->IconArray();
            if( Avindex ) // if there is old icon already
                {
                TRACE( "Avindex " );
                // delete the old icon compress the array and insert at the same position
                currentIconArray->Delete( Avindex );
                currentIconArray->Compress();
                currentIconArray->InsertL( Avindex , newIcon);
                }
            else 
                { 
                TRACE( "else of Avindex " );
                // if there is no old icon append at the last
                currentIconArray->AppendL( newIcon);
                // set the index to cenrep
                TRACE( " currentIconArray->Count()-1 = %d ",currentIconArray->Count()-1  );
                iArrayProcess.SetOwnAvatarIndexL( currentIconArray->Count()-1 );
                }
            }
        else 
            {
            TRACE( "else of avatar got cleared." );
            // avatar got cleared just  set the icon index to '0'
            // icon what was added still remains in the  listbox icon array
            // dont delete the old icon, because that needs to modify the whole 
            // indexes of the  other contacts. 
            // "keeping the icon is better than changing all contacts indexes"
            // The icons will be deleted by listbox, when list box got destructed.
            iArrayProcess.SetOwnAvatarIndexL( KErrNone );
            }
        }
    else
        {
        TRACE( " else  not own id" );
        TRACE( " else  not own id aUserId = %S",&aUserId );
        TInt index = iArrayProcess.GetSelectedItemIndex( aUserId );//Get the index from storage.
        
        TRACE( " after GetSelectedItemIndex" );
        TPtrC8 avatardata = iArrayProcess.AvatarContent( index );
        TRACE("after iArrayProcess.AvatarContent" );
        Avindex = iArrayProcess.AvatarIndex(index);
        TRACE( " after Avindex = %d",Avindex );
        if( avatardata.Length() )
            {
            TRACE( "inside if avatardata.Length()" );
            newIcon = AvatarToIconL( avatardata );
            }
        if( newIcon ) // there is a new avatar icon 
            {
            TRACE( "inside if newIcon" );
            CArrayPtr<CGulIcon>* currentIconArray = listBoxData->IconArray();
            if( Avindex ) // if there is old icon already
                {
                TRACE( " inside Avindex" );
                // delete the old icon compress the array and insert at the same position
                currentIconArray->Delete( Avindex );
                currentIconArray->Compress();
                currentIconArray->InsertL( Avindex , newIcon);
                }
            else 
                { 
                TRACE( " inside Avindex else" );
                // if there is no old icon append at the last
                currentIconArray->AppendL( newIcon);
                // set the index to cenrep
                iArrayProcess.SetAvatarIndex(index, currentIconArray->Count()-1 );
                }
            }
        else 
            {
            TRACE( " avatar got cleared" );
            // avatar got cleared just  set the icon index to '0'
            // icon what was added still remains in the  listbox icon array
            // dont delete the old icon, because that needs to modify the whole 
            // indexes of the  other contacts. 
            // "keeping the icon is better than changing all contacts indexes"
            // The icons will be deleted by listbox, when list box got destructed.
            iArrayProcess.SetAvatarIndex( index , KErrNone );
            }
        }

    iListBox->DrawNow();
    
    }	
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleAddition
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleAdditionL(TVIMPSTEnums::TItem aType, TInt aIndex)
	{
	if(iListBox )
		{
		iListBox->HandleItemAdditionL();
		UpdateViewL(aIndex,aType) ;
		}
	}

// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleAddition
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleDeletionL(TVIMPSTEnums::TItem aType , TInt aIndex )
	{
	TRACER_AUTO;
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
// CVIMPSTUiDoubleListBoxTabViewControl::HandleItemAdditionL
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleItemAdditionL()
    {
	TRACER_AUTO;
    if(iListBox)
  	 	{
        iListBox->HandleItemAdditionL();  
        TInt index  = CurrentItemIndex();
    	TVIMPSTEnums::TItem type = iArrayProcess.GetType(index);
       	UpdateViewL(index,type) ;  
   		}
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleItemRemovalL
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleItemRemovalL()
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
// CVIMPSTUiDoubleListBoxTabViewControl::SizeChanged
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SizeChanged()
    {
    SetLayout(); //layouting controls
    if ( iFindbox && iFindPaneIsVisible  )
        {
        iFindbox->MakeVisible( ETrue );
        }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::CountComponentControls
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiDoubleListBoxTabViewControl::CountComponentControls() const
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
// CVIMPSTUiDoubleListBoxTabViewControl::FocusChanged
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::FocusChanged( TDrawNow aDrawNow )
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
// CVIMPSTUiDoubleListBoxTabViewControl::ComponentControl
// --------------------------------------------------------------------------
//
CCoeControl* CVIMPSTUiDoubleListBoxTabViewControl::ComponentControl(TInt aIndex) const
    {
	TRACER_AUTO;
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
// CVIMPSTUiDoubleListBoxTabViewControl::OfferKeyEventL
// --------------------------------------------------------------------------
//
TKeyResponse CVIMPSTUiDoubleListBoxTabViewControl::OfferKeyEventL
        (const TKeyEvent& aKeyEvent,TEventCode aType)
    {
	TRACER_AUTO;
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
    if (iFindPaneIsVisible && iFindbox ) 
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
// CVIMPSTUiDoubleListBoxTabViewControl::CurrentItemIndex
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiDoubleListBoxTabViewControl::CurrentItemIndex() const
    {
	TRACER_AUTO;
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
// CVIMPSTUiDoubleListBoxTabViewControl::SetCurrentItemIndex
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SetCurrentItemIndex(TInt aIndex)
    {
	TRACER_AUTO;
    if (iListBox->CurrentItemIndex() != aIndex &&
        aIndex >= 0 &&
        aIndex < iListBox->Model()->NumberOfItems() )
        {
        iListBox->SetCurrentItemIndex(aIndex);
        TRAP_IGNORE(UpdateCbaL());
        }
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetCurrentItemIndexAndDraw
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SetCurrentItemIndexAndDraw(TInt aIndex)
    {
    SetCurrentItemIndex(aIndex);
    this->DrawDeferred();
    }


// --------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetListEmptyTextL
// --------------------------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SetListEmptyTextL(TInt aResourceId)
    {
    HBufC* msgText;
    TRACER_AUTO;  
    // Get Service Name from Engine , load string from resource and display.
    // This text is shown to tell the user to restart phone to get the service again.
    TPtrC serviceNamePtr(iEngine.ServiceName());
    msgText = StringLoader::LoadLC(aResourceId, serviceNamePtr, iCoeEnv);
    iListBox->View()->SetListEmptyTextL(*msgText);
    TRACE("Display Text %S", msgText );
    
    CleanupStack::PopAndDestroy(msgText);
    }

// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::LoadBitmapsL
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::LoadBitmapsL()
    {
  
	TRACER_AUTO;
	CFormattedCellListBoxData* listBoxData = iListBox->ItemDrawer()->ColumnData();
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
    TPtrC8 avatardata = iArrayProcess.OwnAvatarContentL();
    CGulIcon* newIcon = NULL;
    if( avatardata.Length() )
        {
        newIcon = AvatarToIconL( avatardata );
        }
    if( newIcon )
        {  
        // if icon creation from avatar is successful then append icon 
        // set the index 
        icons->AppendL(newIcon);
        iArrayProcess.SetOwnAvatarIndexL( icons->Count()-1 );    
        }
    else
        { // set the index to '0'
          // index=0 means default icon
        iArrayProcess.SetOwnAvatarIndexL( KErrNone );
        }
    
    TInt count = iArrayProcess.Count();
    // loop all the items in the array and get the avatar data 
    for (TInt i=0 ;i< count ;++i)
        {
        TPtrC8 avatarContent = iArrayProcess.AvatarContent(i);
        newIcon = NULL;//  make this as NULL since the icon 
                      //ownership will be taken from LIstbox
        if( avatarContent.Length())
            {
            // if that contact has any avatar data then process and create the icon
            newIcon = AvatarToIconL( avatarContent );
            }
        if ( newIcon )
            {
            // if the icon is created successfully the append to icon array
            icons->AppendL(newIcon);
            // set the icon index to the storage 
            iArrayProcess.SetAvatarIndex( i , icons->Count()-1);
            }
        else
            {
            //if icon not found. set the default for buddies
            iArrayProcess.SetAvatarIndex( i , KErrNone);
            }
        }
    // Set icon-array to listbox's drawer
    listBoxData->SetIconArray( icons );
    CleanupStack::Pop( icons );
    }

// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleListBoxEventL(
//			CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/ )
// From MEikListBoxObserver, Handles event's generated by listbox
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleListBoxEventL(
        CEikListBox* /*aListBox*/, TListBoxEvent aEventType )
    {
	TRACER_AUTO;
    iTabbedView.UpdateToolbarL();
    switch(aEventType)
        {
        case EEventItemSingleClicked:

            {
            TInt index = CurrentItemIndex();
            TVIMPSTEnums::TItem itemType = iArrayProcess.GetType(index);
            TBool presenceEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EPresence);
            TBool imEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EIM);
            TBool voipEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EVoip);
            TVIMPSTEnums::TVIMPSTRegistrationState serviceState = iTabbedView.GetServiceState();
            TVIMPSTEnums::TOnlineStatus onlinestatus = iArrayProcess.GetOnlineStatusL(index);

            if(TVIMPSTEnums::EOwnStatusItem == itemType && 
                    TVIMPSTEnums::ESVCENotRegistered ==  serviceState)
                {
            iTabbedView.HandleCommandL( ECmdLogin );
                }
            else if(TVIMPSTEnums::EOwnStatusItem == itemType &&
                                            TVIMPSTEnums::ESVCEWaitingForNetwork == serviceState)
                {
                iTabbedView.HandleCommandL( ECmdChangeConnectioninWaitingState);
                }
            else if(TVIMPSTEnums::EOwnStatusItem == itemType 
                    && presenceEnabled &&
                    TVIMPSTEnums::ESVCERegistered ==  serviceState )
                {
            iTabbedView.HandleCommandL( ECmdChangeStatus );
                }
            else if( ( TVIMPSTEnums::EContactItem == itemType || 
                    TVIMPSTEnums::EUnknonContactItem == itemType )&& imEnabled &&
                    TVIMPSTEnums::ESVCERegistered == serviceState && 
                    TVIMPSTEnums::EServiceOut != onlinestatus )
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
                if((TVIMPSTEnums::EPending != onlinestatus) && 
                        !(TVIMPSTEnums::ESVCEUpdatingContacts ==  serviceState ||
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
// CVIMPSTUiDoubleListBoxTabViewControl::SendMessageL(
//			CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/ )
// From MEikListBoxObserver, Handles event's generated by listbox
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SendMessageL()
	{
	TRACER_AUTO;
//	TInt index = iListBox->CurrentItemIndex();
	TInt index = CurrentItemIndex();        
	if( index< 0 )
		{
		// return if item not found
		return;	
		}
	TPtrC seletctedItem = iArrayProcess.GetItemUserId(index) ;
	if(seletctedItem.Length())
	    {
	    TInt result = EAknSoftkeyOk;
        //get the status from engine.
        TVIMPSTEnums::TOnlineStatus status = iArrayProcess.GetOnlineStatusL(index);
        
        if( status == TVIMPSTEnums::EOffline )
            {
            TBool isConvExist = iArrayProcess.IsConversationExist(index);
            if( !isConvExist)
                {
                // TODO : need to show only once if conversation not exist
                // handling of error message need to be done in cv also ,waiting for ximp changes
                // Show note about offline contact
                HBufC* msg = StringLoader::LoadLC(
                        R_QTN_CHAT_CONTACT_MAY_NOT_RECEIVE_MESSAGE,
                        seletctedItem );                    

                 result = VIMPSTUtilsDialog::DisplayQueryDialogL(
                        R_CHAT_CONTACT_MAY_NOT_RECEIVE_MESSAGE_DIALOG,
                        msg->Des() );
                CleanupStack::PopAndDestroy( msg );
                }
            }
        
        if( ( result == EAknSoftkeyOk ) || ( result == EAknSoftkeyYes ) )
            {
            TPtrC itemName = iArrayProcess.GetItemNameText(index) ;
            // not own this p
            MVPbkContactLink* contactLink = iArrayProcess.ContactLink( index );
        
            TVwsViewId activeViewId;
            CCoeEnv::Static()->AppUi()->GetActiveViewId( activeViewId );
            UpdateCbaL();
            // imlauncher call for luanching the conversationview with seletced user id
            IMCVLauncher::LaunchImConversationViewL(activeViewId, iServiceId, seletctedItem, itemName, contactLink);
            }
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
// CVIMPSTUiDoubleListBoxTabViewControl::UpdateViewL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::UpdateViewL( 
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
	    	if( index != KErrNotFound && iFindbox )
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
// CVIMPSTUiDoubleListBoxTabViewControl::ActivateFindPaneL()
// Activates find-pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::ActivateFindPaneL()
    {
	TRACER_AUTO;
    if( !iFindbox )
        {
        // Use EAdaptiveSearch so that we can search according to columns.
        iFindbox = CAknSearchField::NewL( *this,
                CAknSearchField::EAdaptiveSearch,
                NULL, KTextLimit );

    // This is to filter the presence status from the search
    // username(0)/presencestatus(1) - 1 is to filter the presence status
    // column out of the search. In this case there are only two columns.
    TBitFlags32 columnFilterFlag; 
    columnFilterFlag.Set(1);
    iFindbox->SetListColumnFilterFlags(columnFilterFlag);
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
// CVIMPSTUiDoubleListBoxTabViewControl::DeactivateFindPaneL()
// De-activates find-pane
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::DeactivateFindPaneL()
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
// CVIMPSTUiDoubleListBoxTabViewControl::GetFocussedId()
// To get the pointer to the iFocussedID
// ---------------------------------------------------------
TPtrC CVIMPSTUiDoubleListBoxTabViewControl::GetFocussedId()
    {
    TInt index = CurrentItemIndex();
    if( index < 0 )
	    {
	    index = 0;	
	    }
    return iCommandHandler.GetProcessInterface().GetItemUserId( index  );
    }
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetFocussedId()
// Saves the Contact ID in iFocussedID
// ---------------------------------------------------------
void CVIMPSTUiDoubleListBoxTabViewControl::SetFocussedId(const TDesC& aContact)
    {
    TInt index = iCommandHandler.GetProcessInterface().GetSelectedItemIndex( aContact );
    if( index < 0 )
	    {
	    index = 0;	
	    }
    SetCurrentItemIndex( index );
    SetFocusAndMakeItemVisible();
    } 
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetLayout()
// Layouts current components according to AVKON LAF
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SetLayout()
    {
	TRACER_AUTO;
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
// CVIMPSTUiDoubleListBoxTabViewControl::HandleControlEventL
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent aEventType)
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
// CVIMPSTUiDoubleListBoxTabViewControl::CheckAndSetLastItemFlag()
// Checks if the last contact is focused and Sets the iLastItemFlag
// ---------------------------------------------------------
void CVIMPSTUiDoubleListBoxTabViewControl::CheckAndSetLastItemFlag()
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
// CVIMPSTUiDoubleListBoxTabViewControl::SetFocusAndMakeItemVisible()
// Compares the Saved Contact in iFocussedID with the contacts in the List and focuses if it is found 
// ---------------------------------------------------------
void CVIMPSTUiDoubleListBoxTabViewControl::SetFocusAndMakeItemVisible()
    { 
	TRACER_AUTO;
    TInt count = iArrayProcess.Count();
    TInt index = CurrentItemIndex();
    if( index == count )
        {   
        if( iLastItemFlag )
            {
            index = count - 1;
            iLastItemFlag = EFalse;
            }
        }
    CVIMPSTUiListBoxModel* model =
        static_cast<CVIMPSTUiListBoxModel*>( iListBox->Model() );

    if( model )
        {
        index = model->VisibleItemIndex( index );
        }
    iListBox->ScrollToMakeItemVisible(index );
    SetCurrentItemIndex(index);
    iListBox->DrawDeferred();
    }

// ---------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetClrFindPaneFlag(TBool aFlag)
// ---------------------------------------------------------------------

void CVIMPSTUiDoubleListBoxTabViewControl::SetClrFindPaneFlag(TBool aFlag)
    {
    iClrFindPaneFlag = aFlag;
    }

// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::CoeControl()
// ---------------------------------------------------------

CCoeControl* CVIMPSTUiDoubleListBoxTabViewControl::CoeControl()
    {
    return  this; // return this pointer since its a parent class pointer of  CCoeControl
    }
// ------------------------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::AvatarToIconL
// 
// ------------------------------------------------------------------------
CGulIcon* CVIMPSTUiDoubleListBoxTabViewControl::AvatarToIconL( const TDesC8& aAvatarContent )
    {
	TRACER_AUTO;
    CGulIcon* newIcon = NULL;
    CVIMPSTEngineImageHandler* imageHandler = CVIMPSTEngineImageHandler::NewL();
    CleanupStack::PushL( imageHandler );
    // pass the avatar content and mimetype as NULL
    // mimetype will be recognised by the UTILS  class
    HBufC8* content = imageHandler->ProcessImageFromDataL( aAvatarContent , KNullDesC8);
    if ( iAvatarError == KErrNone)
        {
        newIcon = CGulIcon ::NewL( &(imageHandler->Bitmap()) ); 
        newIcon->SetBitmapsOwnedExternally( EFalse );              
        }
    CleanupStack::PopAndDestroy(); // imageHandler 
    if(content)
        {
        delete content;
        content = NULL;
        }
    return  newIcon; 
    }
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::SetCbaLockL()
// Sets the lock status of cba keys.
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::SetCbaLockL( TBool aLock )
    {
    iCbaLock = aLock;
    if( !aLock )
        {
        UpdateCbaL();
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::UpdateCbaL()
// Updates Softkeys according to current focus.
// ---------------------------------------------------------
//
void CVIMPSTUiDoubleListBoxTabViewControl::UpdateCbaL( TBool aUseDefaultCba /*= EFalse*/ )
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
        TVIMPSTEnums::TOnlineStatus onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(index);        
        TVIMPSTEnums::TVIMPSTRegistrationState servicestate = iEngine.ServiceState();
        TBool updateinvalid( iLastFocusedItemType == TVIMPSTEnums::EInvalid );
         if ( !iCba )
            {
            iCba = CEikButtonGroupContainer::Current();
            }

        // default softkeys
         if(iCba)
             {
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
            
                 if ( itemtype == TVIMPSTEnums::EOwnStatusItem  )
                     {
                     // Own status
                     if(TVIMPSTEnums::ESVCERegistered == servicestate)
                         {
                         // Options/Back/Change status
                          cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__CHANGESTATUS;
                          iCurrentCmdToExe = ECmdChangeStatus;
                         }
                     else if(TVIMPSTEnums::ESVCENetworkConnecting == servicestate)
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
                     else if(TVIMPSTEnums::ESVCEWaitingForNetwork == servicestate)
                         {
                         cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CHANGECONN__EXIT;
                         iCurrentCmdToExe = ECmdChangeConnectioninWaitingState;
                         }
                    else if(TVIMPSTEnums::ESVCEUpdatingContacts != servicestate)
		                    {
		                    cbaRes = R_SERVTAB_SOFTKEYS_OPTION_LOGIN__EXIT;
		                    iCurrentCmdToExe = ECmdLogin;
		                    }
                     }
                 
                 iCba->SetCommandSetL( cbaRes );
                 iCba->DrawNow();
                 }
            
             // collapse softkey ( when focus is on contact )
             else if(  itemtype == TVIMPSTEnums::EContactItem  )
                 {
                 iLastFocusedItemType = itemtype;
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
             else if( itemtype == TVIMPSTEnums::EOwnStatusItem )
                 {
                 iLastFocusedItemType = itemtype;
                 if ( itemtype == TVIMPSTEnums::EOwnStatusItem  )
                     {
                     // Own status
                     if(TVIMPSTEnums::ESVCERegistered == servicestate)
                         {
                         // Options/Back/Change status
                          cbaRes = R_SERVTAB_SOFTKEYS_OPTIONS_EXIT__CHANGESTATUS;
                          iCurrentCmdToExe = ECmdChangeStatus;
                         }
                     else if(TVIMPSTEnums::ESVCENetworkConnecting == servicestate)
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
                     else if(TVIMPSTEnums::ESVCEWaitingForNetwork == servicestate)
                         {
                         cbaRes = R_SERVTAB_SOFTKEYS_OPTION_CHANGECONN__EXIT;
                         iCurrentCmdToExe = ECmdChangeConnectioninWaitingState;
                         }
                    else if(TVIMPSTEnums::ESVCEUpdatingContacts != servicestate)
		                    {
		                    cbaRes = R_SERVTAB_SOFTKEYS_OPTION_LOGIN__EXIT;
		                    iCurrentCmdToExe = ECmdLogin;
		                    }
                     }
                 //add for connecting state and waiting for connection state.
                 iCba->SetCommandSetL( cbaRes );
                 iCba->DrawNow();
                 }
             // Focus on invitation item
             else if( itemtype == TVIMPSTEnums::EFriendRequestItem )
                 {
                 iLastFocusedItemType = itemtype;
                 if(TVIMPSTEnums::ESVCERegistered == servicestate)
                     {
                     iCurrentCmdToExe = ECmdOpenFriendRequest;	
                     iCba->SetCommandSetL( R_SERVTAB_SOFTKEYS_OPTION_OPENFRNDREQ__EXIT );
                     iCba->DrawNow();
                     }
                    
                 }  
             }
          
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiDoubleListBoxTabViewControl::HandleAvatarDeleteL()
// Saves the Contact ID in iFocussedID
// ---------------------------------------------------------
void CVIMPSTUiDoubleListBoxTabViewControl::HandleAvatarDeleteL( const TDesC& aUserId )
	{
	TRACER_AUTO;
	TInt index = iArrayProcess.GetSelectedItemIndex(aUserId );//Get the index from storage.
	if(index >= 0)
		{
		CFormattedCellListBoxData* listBoxData = iListBox->ItemDrawer()->ColumnData();
		CArrayPtr<CGulIcon>* currentIconArray = listBoxData->IconArray();
		TInt avIndex = iArrayProcess.AvatarIndex(index);

		    if( avIndex ) // if there is old icon already
		        {
		        // delete the old icon compress the array and insert at the same position
		        currentIconArray->Delete( avIndex );
		        currentIconArray->Compress();
		        }

		    // avatar got cleared just  set the icon index to '0'
		    // icon what was added still remains in the  listbox icon array
		    // dont delete the old icon, because that needs to modify the whole 
		    // indexes of the  other contacts. 
		    // "keeping the icon is better than changing all contacts indexes"
		    // The icons will be deleted by listbox, when list box got destructed.
		    iArrayProcess.SetAvatarIndex( index , KErrNone );

		}
	}

// End of File
