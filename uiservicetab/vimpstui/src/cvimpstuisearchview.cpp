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
* Description:   Implementation for CVIMPSTUiSearchView.
*
*/


// INCLUDE FILES

#include "cvimpstuisearchview.h"
#include "cvimpstuisearchviewcontrol.h"

#include "cvimpstuisearchfieldaray.h"
#include "cvimpstuisearchquerydialog.h"

#include "cvimpstcmdhandler.h"
#include "vimpstcmd.hrh"

#include "mvimpstengine.h"
#include "vimpstextentionuiuid.h"
#include "tvimpstconsts.h"
#include "cvimpstuiextensionservice.h"
#include "vimpstutilsdialog.h"
#include "vimpstcmd.hrh"
#include "mvimpstcmd.h"
#include "vimpstutilswaitnote.h"
#include "vimpstui.hrh"
#include "tvimpstconsts.h"

#include "mvimpstengineextentionfeatures.h"
#include "mvimpstenginesearchmgrextention.h"

#include "cvimpstuistatuspanehandler.h"
// System includes
#include <eikmenup.h>
#include <eikbtgpc.h>
#include <avkon.rsg>
#include <aknnotewrappers.h> 
#include <StringLoader.h>
#include <eikenv.h>

// Pbk2
#include <MPbk2AppUi.h>
#include <CPbk2UIExtensionView.h>
#include <MPbk2ViewActivationTransaction.h>
#include <MPbk2ViewExplorer.h>
#include <CPbk2ViewState.h>
#include <MPbk2StartupMonitor.h>
#include <Pbk2Commands.hrh>

// xSP extension manager
#include "MxSPViewMapper.h"
#include "MxSPContactManager.h"

//resource
#include <ExtensionManagerRes.rsg>
#include <vimpstuires.rsg>

// System includes
#include <AknGlobalNote.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <StringLoader.h>
#include <avkon.rsg>
#include <eikbtgpc.h>
#include <eikmenup.h>
#include <avkon.hrh>
#include <eikspane.h>
#include <StringLoader.h>

// help launch
#include <hlplch.h>
#include <coehelp.h>
#include "meco.hlp.hrh"
using namespace Phonebook2;

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::CVIMPSTUiSearchView
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchView::CVIMPSTUiSearchView( 
				MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aSearchViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid /*aId*/,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler ) :
			    CxSPBaseView( aMapper, aContactManager, aView ),
			    iSearchViewId( aSearchViewId ),
			    iServiceId(aServiceId),
			    iEngine(aEngine),
			    iExtensionService( aServiceData ),
			    iCommandHandler( aCommandHandler )
    
    {
    
    }
    
// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchView::ConstructL()
    {
    ViewBaseConstructL( KIMCONTACTSEXTNIMPLEMENTATIONUID, iSearchViewId ); 
    
    //Get SearchMgr
	MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);
	if (feature)
		{
		MVIMPSTEngineSearchMgrExtention& searchMgr = 
	   	   	MVIMPSTEngineSearchMgrExtention::Cast
	   	   	(*feature);	
	   	
	   	searchMgr.RegisterObserver(this);   	
		} 	
	iCallBack = new ( ELeave ) CAsyncCallBack( CActive::EPriorityHigh );
    }


// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchView* CVIMPSTUiSearchView::NewL( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aSearchViewId,
			    TUint aServiceId,
			    CPbk2UIExtensionView& aView,
			    TUid aId,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine,
			    MVIMPSTCmdHandler& aCommandHandler  )
    {
    CVIMPSTUiSearchView* self = new(ELeave) CVIMPSTUiSearchView( aMapper, 
            aContactManager,
            aSearchViewId,
            aServiceId, 
            aView, 
            aId,
            aServiceData,
            aEngine,
            aCommandHandler);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }



// ---------------------------------------------------------
// CVIMPSTUiSearchView::~CVIMPSTUiSearchView()
// C++ Destructor 
// ---------------------------------------------------------
//
CVIMPSTUiSearchView::~CVIMPSTUiSearchView()
    {
    
    //Get SearchMgr
	MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);
	if (feature)
		{
		MVIMPSTEngineSearchMgrExtention& searchMgr = 
	   	   	MVIMPSTEngineSearchMgrExtention::Cast
	   	   	(*feature);	
	   	
	   	searchMgr.UnRegisterObserver(this);   	
		}		
    
    delete iNaviDecorator;
    if (iContainer)
	    {
        CCoeEnv::Static()->AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        }
    delete iCallBack;
    }


// ---------------------------------------------------------
// CVIMPSTUiSearchView::HandleCommandL( TInt aCommand )
// This class handles all commands from menu
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleCommandL( TInt aCommand )
    {
	
   	switch ( aCommand ) 
		{
		//Next two commands have same function
		case EAknSoftkeyBack:
			{
			
			    // this might get called when the view is not active
    			// back key pressed, activate tabbed view.
				// CPbk2ViewState is not utilized here, but here the
				// focused contact info could be delivered to next view, if required later
				CPbk2ViewState* state = NULL; 
				
				TInt tabViewId;
				TInt err = iMapper.GetNewView( KIMCONTACTSEXTNIMPLEMENTATIONUID, 
										   iExtensionService.TabbedViewId(), 
										   tabViewId );
				User::LeaveIfError( err );            	
				Pbk2AppUi()->Pbk2ViewExplorer()->ActivatePhonebook2ViewL( 
											TUid::Uid(tabViewId), 
											state );
	
				break;
				}
		
		case ESearchNew:
			{	
			SearchContactsL();
			break;
			}
	    case ESearchPrev:
			{
			CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
			informationNote->ExecuteLD(KNotSupportedMessage);
			break;	
			}
		case ESearchMore:
			{
			CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
			informationNote->ExecuteLD(KNotSupportedMessage);
			break;	
			}			
		case ESearchAddFriends:
    		{
    		
    		HBufC* userId = iContainer->GetFocusedContactLC() ;
    		if ( userId->Length() )
	    		{
	    		CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_SERVTAB_SAVING_CONTACT,ETrue );
 				iCommandHandler.HandleCommandL(EAddContact,this,userId);
        		CleanupStack::PopAndDestroy( note );
    		    CleanupStack::PopAndDestroy( userId );	 	
	    		}
		    
		    break;
		    }
		
		case ESearchOpenConversation:
			{
			HBufC* userId = iContainer->GetFocusedContactLC() ;
			if ( userId->Length() )
	    		{
	    	    iContainer->OpenConversationL( *userId );
	    	    }
	    	CleanupStack::PopAndDestroy( userId );	 	
			break;	
			}			
		case  EAknCmdHelp:
			{
			LaunchHelpL( KSERVTAB_HLP_SEARCH );
			break;
			}
		case EAknSoftkeyExit:
		case ECmdExit:
			{
			Pbk2AppUi()->HandleCommandL( EAknSoftkeyExit );
			break;  
			}
						
		default:
            {
            Pbk2AppUi()->HandleCommandL( aCommand );
            break;
            }
		
	    }
    }

// ---------------------------------------------------------
// CVIMPSTUiSearchView::DynInitMenuPaneL()
// Initialises menu pane when needed
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::DynInitMenuPaneL( TInt aResourceId,
									 CEikMenuPane* aMenuPane )
	{
    if( aResourceId == R_EXTENSION_MANAGER_MENU)
	    {
	    //add search view menu items 
	    aMenuPane->AddMenuItemsL( R_SERVTAB_SEARCH_VIEW_MENU ); 
	    //add general menu items - help and exit 
		aMenuPane->AddMenuItemsL( R_TABBED_VIEW_GENERAL_MENU );	
		
		aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState,ETrue);
		aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState,ETrue);
		
		aMenuPane->SetItemDimmed(ECmdLogin,ETrue);
		aMenuPane->SetItemDimmed(ECmdLogout,ETrue);
		aMenuPane->SetItemDimmed(ECmdCancelLogin,ETrue);
		aMenuPane->SetItemDimmed(ECmdSettings,ETrue);
		
		aMenuPane->SetItemDimmed(ESearchPrev, ETrue);
		aMenuPane->SetItemDimmed(ESearchMore, ETrue);
		aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
		TInt index = KErrNotFound;
		if(iContainer)
		    {
		    index = iContainer->ListBox()->CurrentItemIndex(); 
		    }
		if( index == KErrNotFound )
            {
            aMenuPane->SetItemDimmed( ESearchAddFriends, ETrue );
            aMenuPane->SetItemDimmed( ESearchOpenConversation, ETrue );
            }
		else if( HideAddToContactsL())	//contact is there in his contact list
		    {
		    HBufC* contactId = iContainer->GetFocusedContactLC() ;
		    TVIMPSTEnums::TOnlineStatus onlinestatus = TVIMPSTEnums::EUnknown;
		    if ( contactId->Length() )
		        {
		        TInt index = iCommandHandler.GetProcessInterface().FindContactIndexL(*contactId);
		        onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(index);
		        CleanupStack::PopAndDestroy(contactId);
		        }
		    if( TVIMPSTEnums::EPending == onlinestatus )//contacts pr status is pending hide open convrsation
		        {
		        aMenuPane->SetItemDimmed( ESearchOpenConversation, ETrue );	    
		        }
		    aMenuPane->SetItemDimmed( ESearchAddFriends, ETrue );	

		    }

	    else  //contact is not there in his contactlist
	       	{
	       	aMenuPane->SetItemDimmed( ESearchOpenConversation, ETrue );
	       	}
		
	    }
   	}

// ---------------------------------------------------------
// CVIMPSTUiSearchView::DoActivateViewL()
// Activates view
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::DoActivateViewL( const TVwsViewId& aPrevViewId,
        									TUid /*aCustomMessageId*/, 
        									const TDesC8&  /*aCustomMessage*/ )
    {
    // the title pane is set to " Basic Search" 
    // which is same as Search Form s  title pane text 
 
    HBufC* titleText = StringLoader::LoadLC( R_QTN_SERVTAB_SEARCH_BASIC );
    
    //to set the icon ( context pane image) as in tabbed view
    MPbk2ViewActivationTransaction* viewActivationTransaction =
        Pbk2AppUi()->Pbk2ViewExplorer()->HandleViewActivationLC(
            iView.Id(),
            aPrevViewId,
            titleText,
            NULL,
            EUpdateAll );            
    

	// Notifies startup monitor of view activation
    if( Pbk2AppUi()->Pbk2StartupMonitor() )
        {
        Pbk2AppUi()->Pbk2StartupMonitor()->
            NotifyViewActivationL( iView.Id() );
        }
	 	
    if (!iContainer)
        {
        iContainer = CVIMPSTUiSearchViewControl::NewL(
        				&iView, iServiceId,
        				iEngine, iCommandHandler, *this  );
        CCoeEnv::Static()->AppUi()->AddToStackL( iView, iContainer );
        iContainer->SetMopParent( &iView );        
        iContainer->SetSearchingTextToListboxL();  
        iContainer->SetRect( iView.ClientRect() );
        iContainer->ActivateL();
        } 
   
    // Commit application-wide state changes
    viewActivationTransaction->Commit();
    CleanupStack::PopAndDestroy(2); //viewActivationTransaction ,titleText 
   
    // Change the navigation pane from tabs to text label    
    UpdateNaviPaneTextL();  
    
    // this is initiated from tabbed view ; hence call search functionality
    if (!iSearchInitiated)
        { 
        // search is called  using CAsynCallBack  
        iCallBack->Set(TCallBack( SearchCallbackL , this));
        iCallBack->CallBack();
        }
    

    }

// ---------------------------------------------------------
// CVIMPSTUiSearchView::DoDeactivate()
// Deactivates the view
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::DoDeactivate()
	{
	// set this flag to Efalse because  user cant initiate a search from search view
	iSearchInitiated = EFalse;
 	if (iContainer)
        {
        CCoeEnv::Static()->AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
   	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleCommandKeyL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchView::HandleCommandKeyL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/)
	{
    TBool result = EFalse;
    switch (aKeyEvent.iCode)
        {        
        default:
            {
            break;
            }
        }
    return result;
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleStatusPaneSizeChange
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleStatusPaneSizeChange()
    {
    // Resize the container to fill the client rectangle
    if (iContainer)
        {
        iContainer->SetRect( iView.ClientRect() );
        }
    }    
 
 
 // --------------------------------------------------------------------------
// CVIMPSTUiSearchView::CommandFinishedL
// --------------------------------------------------------------------------
//

void CVIMPSTUiSearchView::CommandFinishedL(
                const MVIMPSTCmd& aCommand )
{

	//handle the observer for commands issues through HandleCommandL()
	
	switch (aCommand.CommandId())
        {
         
        case ESearch:
            {
            // Search command is complete
            //use aCommand.Result() to get the data 
            //Ownership is not given to the caller
            //some error occured.
  			if(iWaitNote)
	  			{
	  			delete iWaitNote;
	  			iWaitNote = NULL;	
	  			}
            if(aCommand.Result() != KErrNone) 
            	{
            	HBufC* prompt = NULL;
            	prompt = StringLoader::LoadLC(R_IM_ERROR_SERVICE_ERR );
            	TInt ret( VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) );
            	CleanupStack::PopAndDestroy(prompt);
            	} 
            break;
            } 
        case EAddContact: 
	        {
	        //addcontact command is complete
            //use aCommand.Result() to get the data 
            //Ownership is not given to the caller
            //some error occured.
	        if(aCommand.Result() != KErrNone) 
            	{
            	HBufC* prompt = StringLoader::LoadLC( R_QTN_SERVTAB_SAVING_CONTACT_FAILED );
                VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
            	CleanupStack::PopAndDestroy(prompt);
            	} 
              
            break;	
	        }
        default:
            { 
            //should never be here.           
            break;
            }
        }
 } 
 
// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleCommandEventL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleCommandEventL(TVIMPSTEnums::TVIMPSTRegistrationState /*aState*/, TInt /*aServiceError*/) 
	{
	
	}  
// ---------------------------------------------------------
// CVIMPSTUiSearchView::ShowAddToContactsL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CVIMPSTUiSearchView::HideAddToContactsL() const
	{
	TBool hide(ETrue);
	TInt curIndex = iContainer->ListBox()->CurrentItemIndex();
	if( curIndex >=0)
		{
		HBufC* contactId = iContainer->GetFocusedContactLC() ;
		if ( contactId->Length() )
			{
			hide = iCommandHandler.GetProcessInterface().FindAnyContact( *contactId );	
			CleanupStack::PopAndDestroy( contactId );	 
			}
			
		}
    return hide;		
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::SearchContactsL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchView:: SearchContactsL()
	{
	if( iContainer )
		{
		iContainer->UpdateCbaL( ETrue );	
		}
		
    CVIMPSTUiSearchFieldArray& fieldArray = iExtensionService.GetSearchFieldArray(); 
	
	if( !fieldArray.Count() )
		{
		return;	
		}
	
    CVIMPSTUiSearchQueryDialog* querydialog = CVIMPSTUiSearchQueryDialog::NewL( fieldArray );
   
    querydialog->ExecuteLD();
   
    if (querydialog)
        {
        iEndKeyPressed = querydialog->IsEndKeyPressed();
        }
    
    // we have to set the title pane with " Search results " string
    CCVIMPSTUiStatusPaneHandler* StatusPaneHandler = CCVIMPSTUiStatusPaneHandler::NewL();
    CleanupStack::PushL( StatusPaneHandler);
    HBufC* titleText = StringLoader::LoadLC( R_QTN_SERVTAB_SEARCH_RESULTS );
    StatusPaneHandler->SetTextL(*titleText);
    CleanupStack::PopAndDestroy( 2 ); //
   
    
    if( !fieldArray.AreAllUiFieldsEmpty() )
		{
		// make this flag as  True 
		// from now search is initiated from search view 
		iSearchInitiated = ETrue;
		
		// if any field is entered 
		// launch the search view
		HBufC* firstDataBuf = fieldArray.GetFirstEnteredFieldDataL().AllocLC();
		TPtr searchStrPtr( firstDataBuf->Des() );
	
		iExtensionService.SetSearchStringL( searchStrPtr );
		UpdateNaviPaneTextL();
		if ( iContainer->ListBox()->Model()->NumberOfItems() > 0 )
	        {
	        iContainer->ClearListBoxContents();  
	        }
	    // set the empty string blank    
		iContainer->SetSearchingTextToListboxL();   
		
		HBufC* searchWaitNoteText = StringLoader::LoadLC( R_QTN_SERVTAB_SEARCHING , searchStrPtr, CCoeEnv::Static() );
		iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( *searchWaitNoteText, EFalse , ETrue  );  
    	CleanupStack::PopAndDestroy(searchWaitNoteText); 
		//searchWaitNoteText	
		RArray<TVIMPSTSearchKeyData> keyDataArray;
		fieldArray.GetSearchKeyDataL( keyDataArray );
			
		iCommandHandler.HandleCommandL(ESearch, this, &keyDataArray  );
		CleanupStack::PopAndDestroy(); // firstDataBuf
		
		}
    
    // if the serach is initiated from tabbed view and User cancels the search
    // return to Tabbed view
    if ( !iSearchInitiated && !iEndKeyPressed)
        {
        //  return to tabbed view 
        CPbk2ViewState* state = NULL; 

        TInt tabViewId;
        TInt err = iMapper.GetNewView( KIMCONTACTSEXTNIMPLEMENTATIONUID, 
                iExtensionService.TabbedViewId(), 
                tabViewId );
        User::LeaveIfError( err );              
        Pbk2AppUi()->Pbk2ViewExplorer()->ActivatePhonebook2ViewL( 
                TUid::Uid(tabViewId), 
                state );
        }
	}


// ---------------------------------------------------------
// CVIMPSTUiSearchView::UpdateNaviPaneTextL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::UpdateNaviPaneTextL( )
	{
	// Change the navigation pane from tabs to text label    
    CEikStatusPane* sp = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CAknNavigationControlContainer* np =
    						static_cast<CAknNavigationControlContainer*>(
    									sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    delete iNaviDecorator;
    iNaviDecorator = NULL;
    //fix for disableing navigation search string text, makeing it null
   	iNaviDecorator = np->CreateNavigationLabelL( _L(" ") );
   	//previous code commented for reference
   	//iNaviDecorator = np->CreateNavigationLabelL( iExtensionService.SearchString() );
	  
    np->PushL( *iNaviDecorator );
    sp->DrawNow();  
  	}

 // ---------------------------------------------------------
// CVIMPSTUiSearchView::HandleSearchResultL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleSearchResultL(RArray<TVIMPSTSearchData> aSearchData )
    {
   	if( iContainer )
	    {
	    iContainer->SearchResultL( aSearchData );	
	    }
    }


// ---------------------------------------------------------
// CVIMPSTUiSearchView::HandleSearchFinishedL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleSearchFinishedL(TInt aError)
    {
    // if any error show error note
    if( iContainer )
	    {
	    iContainer->SearchFinishedL(aError);	
	    }
	}
	
// ---------------------------------------------------------
// CVIMPSTUiSearchView::HandleSearchKeysEventL()
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::HandleSearchKeysEventL( RArray<TInt>& /*aEnumKeysArray*/, RPointerArray<HBufC>& /*aLebelKeysArray*/ )
    {
    // do nothing here 
   	}
// ---------------------------------------------------------
// CVIMPSTUiSearchView::LaunchHelpL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//
void CVIMPSTUiSearchView::LaunchHelpL( const TDesC& aContext )
    {
    CArrayFix< TCoeHelpContext >* cntx = new( ELeave ) CArrayFixFlat< TCoeHelpContext >( KHelpContextSize );
    CleanupStack::PushL( cntx );

    TUid KHelpUid = { KHELPUID };
    cntx->AppendL( TCoeHelpContext( KHelpUid, aContext ) );
    CleanupStack::Pop( cntx );

    //and launch help - takes ownership of context array
    HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), cntx );    
    }

// ---------------------------------------------------------
// CVIMPSTUiSearchView::SearchCallbackL()
// ---------------------------------------------------------
//
TInt CVIMPSTUiSearchView::SearchCallbackL(TAny* aInstance)
    {
    static_cast<CVIMPSTUiSearchView*> (aInstance)->SearchContactsL();
    return 0; // ignored by CAsychCallBack
    }
	
//  End of File  
