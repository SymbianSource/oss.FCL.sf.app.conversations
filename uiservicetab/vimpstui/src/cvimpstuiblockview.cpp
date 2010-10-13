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
*  Description:  Implementation for CVIMPSTUiBlockView.
 *
*/

// INCLUDE FILES

#include "cvimpstuiblockview.h"
#include "cvimpstuiblockviewcontrol.h"
#include "cvimpstenginepresencesubservice.h"
#include "vimpstutilswaitnote.h"
#include "mvimpstengine.h"
#include "vimpstextentionuiuid.h"
#include "cvimpstuiextensionservice.h"
#include "vimpstui.hrh"
#include "cvimpstuistatuspanehandler.h"
#include "cvimpstcmdhandler.h"
// System includes
#include <StringLoader.h>
// Pbk2
#include <MPbk2AppUi.h>
#include <CPbk2UIExtensionView.h>
#include <MPbk2ViewActivationTransaction.h>
#include <MPbk2ViewExplorer.h>
#include <MPbk2StartupMonitor.h>
#include <eikmenup.h>
#include <AiwCommon.hrh>
//resource
#include <vimpstuires.rsg>
#include <ExtensionManagerRes.rsg>
#include "uiservicetabtracer.h"

using namespace Phonebook2;

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CVIMPSTUiBlockView::CVIMPSTUiBlockView
// --------------------------------------------------------------------------
//
CVIMPSTUiBlockView::CVIMPSTUiBlockView( 
				MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aBlockViewId,
			    CPbk2UIExtensionView& aView,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine ,
			    MVIMPSTCmdHandler& aCommandHandler) :
			    CxSPBaseView( aMapper, aContactManager, aView ),
			 	iBlockViewId( aBlockViewId ),
			    iEngine(aEngine),
			    iExtensionService( aServiceData ),
			    iCommandHandler( aCommandHandler )
    {
	
    }
    
// --------------------------------------------------------------------------
// CVIMPSTUiBlockView::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::ConstructL()
    {
	TRACER_AUTO;
    ViewBaseConstructL( KIMCONTACTSEXTNIMPLEMENTATIONUID, iBlockViewId );

    iPresenceSubService = iEngine.GetPreseceSubService();
    if(iPresenceSubService)
        {
        iBlockedList = iPresenceSubService->GetBlockedList();
        }
    
    }


// --------------------------------------------------------------------------
// CVIMPSTUiBlockView::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiBlockView* CVIMPSTUiBlockView::NewL( MxSPViewMapper& aMapper, 
			    MxSPContactManager& aContactManager, 
			    TUint aBlockViewId,
			    CPbk2UIExtensionView& aView,
			    CVIMPSTUIExtensionService& aServiceData,
			    MVIMPSTEngine& aEngine ,
			    MVIMPSTCmdHandler& aCommandHandler)
    {
	TRACER_AUTO;
    CVIMPSTUiBlockView* self = new(ELeave) CVIMPSTUiBlockView( aMapper, 
            aContactManager,
            aBlockViewId,
            aView, 
            aServiceData,
            aEngine,
            aCommandHandler );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }



// ---------------------------------------------------------
// CVIMPSTUiBlockView::~CVIMPSTUiBlockView()
// C++ Destructor 
// ---------------------------------------------------------
//
CVIMPSTUiBlockView::~CVIMPSTUiBlockView()
    {
    delete iNaviDecorator;
    }

// ---------------------------------------------------------
// CVIMPSTUiBlockView::HandleCommandL( TInt aCommand )
// This class handles all commands from menu
// ---------------------------------------------------------
//
void CVIMPSTUiBlockView::HandleCommandL( TInt aCommand )
    {
	TRACER_AUTO;
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
		case  ECmdUnBlock:
			{
			//show wait note
			// load, format and show confirmation note
			HBufC* unblockContact = iContainer->GetFocusedContactLC();
		    if(unblockContact)
				{
				HBufC* prompt = StringLoader::LoadLC( R_QTN_CHAT_UNBLOCKING, *unblockContact );
				CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( *prompt );
	        	iCommandHandler.HandleCommandL(ECmdUnBlockContact, this, unblockContact  );
				CleanupStack::PopAndDestroy( 3,unblockContact );//note;prompt;unblockContact.
				}
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
void CVIMPSTUiBlockView::DynInitMenuPaneL( TInt aResourceId,
                                     CEikMenuPane* aMenuPane )
    {
	TRACER_AUTO;
    //using the existing menu resource as it contains unblock item
    if( aResourceId == R_EXTENSION_MANAGER_MENU)
        {
        //add search view menu items 
        aMenuPane->AddMenuItemsL( R_SERVTAB_TABBED_VIEW_MENU ); 
        aMenuPane->SetItemDimmed(ECmdOpenConversation, ETrue);
        aMenuPane->SetItemDimmed(ECmdContactAddAsFriend,ETrue);
        aMenuPane->SetItemDimmed(KAiwCmdCall,ETrue);
        aMenuPane->SetItemDimmed(ECmdCloseConversation,ETrue);
        aMenuPane->SetItemDimmed(ECmdCreateNewOption,ETrue);
        aMenuPane->SetItemDimmed(ECmdContactDetails,ETrue);
        aMenuPane->SetItemDimmed(ECmdOpenFriendRequest,ETrue);
        aMenuPane->SetItemDimmed(ECmdContactEdit,ETrue);
        aMenuPane->SetItemDimmed(ECmdAIWAddToContact,ETrue);
        aMenuPane->SetItemDimmed(ECmdAIWLinkToContact,ETrue);
        aMenuPane->SetItemDimmed(ECmdBlock,ETrue);
        aMenuPane->SetItemDimmed(ECmdContactDelete,ETrue);
        aMenuPane->SetItemDimmed(ECmdMyAvatar,ETrue);
        aMenuPane->SetItemDimmed(ECmdChangeStatus,ETrue);
        if(0==iBlockedList->Count())
            {
            aMenuPane->SetItemDimmed(ECmdUnBlock,ETrue);
            }
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiBlockView::DoActivateViewL()
// Activates view
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiBlockView::DoActivateViewL( const TVwsViewId& aPrevViewId,
        									TUid /*aCustomMessageId*/, 
        									const TDesC8&  /*aCustomMessage*/ )
    {
	TRACER_AUTO;
    HBufC* titleText = StringLoader::LoadLC( R_QTN_SERVTAB_TITLE_BLOCKEDLIST );
    
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
        iContainer = CVIMPSTUiBlockViewControl::NewL(*this,iView, iEngine, iBlockedList );
        CCoeEnv::Static()->AppUi()->AddToStackL( iView, iContainer );
        iContainer->SetMopParent( &iView );        
        iContainer->SetRect( iView.ClientRect() );
        iContainer->ActivateL();
        iContainer->UpdateCbaL();
        } 
        
    // Commit application-wide state changes
    viewActivationTransaction->Commit();
    CleanupStack::PopAndDestroy(); //viewActivationTransaction  
 	CleanupStack::PopAndDestroy( titleText );            
    // Change the navigation pane from tabs to text label    
    UpdateNaviPaneTextL();  
    //resetting the blocked list to empty.and show it in blocked view.
    iBlockedList->ResetAndDestroy();
    iContainer->UpdateBlockedListL();
    //set the text as updating blocked list while fetching blocked list
    iContainer->SetUpdatingTextsToListboxL();
    //fetch the fresh blocked list from server..
    FetchBlockedListFromServerL();
    }

// ---------------------------------------------------------
// CVIMPSTUiBlockView::DoDeactivate()
// Deactivates the view
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiBlockView::DoDeactivate()
	{
    if (iContainer)
	    {
        CCoeEnv::Static()->AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }
   	}


// ---------------------------------------------------------
// CVIMPSTUiBlockView::UpdateNaviPaneTextL()
// ---------------------------------------------------------
//
void CVIMPSTUiBlockView::UpdateNaviPaneTextL( )
	{
	TRACER_AUTO;
	// Change the navigation pane from tabs to text label    
    CEikStatusPane* sp = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CAknNavigationControlContainer* np =
    						static_cast<CAknNavigationControlContainer*>(
    									sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    delete iNaviDecorator;
    iNaviDecorator = NULL;
    //fix for disableing navigation search string text, makeing it null
   	iNaviDecorator = np->CreateNavigationLabelL( iEngine.ServiceName());
	  
    np->PushL( *iNaviDecorator );
    sp->DrawNow();  
  	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleStatusPaneSizeChange
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::HandleStatusPaneSizeChange()
    {
    // Resize the container to fill the client rectangle
    if (iContainer)
        {
        iContainer->SetRect( iView.ClientRect() );
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::FetchBlockedListFromServerL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::FetchBlockedListFromServerL()
	{
	iEngine.RegisterBlockedListObserver(this);
	iCommandHandler.HandleCommandL(EFetchBlockedList, this, NULL );
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleBlockedListFetchCompleteL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::HandleBlockedListFetchCompleteL()
	{
	if(iContainer)
		{
		iContainer->UpdateBlockedListL();	
		}
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::CommandFinishedL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::CommandFinishedL( const MVIMPSTCmd& /*aCommand */)
	{
		
	}

// --------------------------------------------------------------------------
// CVIMPSTUiSearchView::HandleCommandEventL
// --------------------------------------------------------------------------
//
void CVIMPSTUiBlockView::HandleCommandEventL( TVIMPSTEnums::TVIMPSTRegistrationState /*aState*/, 
								TInt /*aServiceError*/) 
	{
	
	}

//  End of File
