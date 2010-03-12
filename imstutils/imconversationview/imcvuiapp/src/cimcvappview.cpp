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
* Description:  conversation main view
*
*/

// INCLUDE FILES
#include	"cimcvappview.h"
#include    "cimcvmenuextensionmanager.h"

#include	"cimcvappviewcontainer.h"
#include 	"cimcvappui.h"
#include 	"cimcvengine.h"
#include 	"mimcvenginechatinterface.h"
#include 	"imcvuiapp.hrh"
#include 	"imcvuiliterals.h"
#include 	"imcvlogger.h"
#include 	"cimcvappstatuspanehandler.h"
#include 	"cimcvappmessageextensionshandler.h"
#include 	"imcvuiapputils.h"
#include 	"imcvuiappnotemapper.h"
#include 	"mimcvenginemessagehandler.h"
#include 	"imcvuiapp.h"

#include	<aknnotewrappers.h>
#include    <CommonUiInternalCRKeys.h>
#include    <AknIconUtils.h>
#include    <finditem.hrh>
#include    <finditemmenu.h>
#include 	<imcvuiapp.mbg>

#include 	<imcvuiapp.rsg>
#include	<imerrors.hrh>
//brandingserver
#include 	<mbsaccess.h>
#include    "conversations.h"

#include    <conversations.mbg>
#include 	<StringLoader.h> 
#include 	<aknEditStateIndicator.h>
#include 	<AknIndicatorContainer.h>
// help launch
#include <hlplch.h>
#include <coehelp.h>
#include "meco.hlp.hrh"

#include "imcvuicommonuids.h"
#include "cimcvappmessageeditor.h"
#include <akntoolbar.h>

#include <ccafactory.h>
#include <mccaconnection.h>
#include <mccaparameter.h>
#include "vimpstdetailsviewpluginuids.hrh"
#include "mimcvenginemessagecontainerinfo.h"
#include "mimcvenginemessageswriteinterface.h"
#include "cimcvenginefactory.h"
#include "mimcvenginefactory.h"
#include <imconnectionproviderconsts.h>
#include <e32property.h>

// AIW Include Files 
#include <aiwgenericparam.h>            
#include <aiwcontactassigndatatypes.h>
#include <aiwdialdataext.h>
#include <aiwcommon.hrh>
#include <aiwservicehandler.h>
#include <aiwdialdata.h>
#include <CommonPhoneParser.h>


// Dll Uid of vimpstui, to maintain uniqueness of help uid
const TUid KHelpUid = { 0x2001FDC2 } ;  
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppView::CIMCVAppView()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppView::CIMCVAppView()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppView::ConstructL( TInt aAvkonViewResourceId, TUid aViewId, 
             CIMCVAppMessageExtensionsHandler& aMessageHandler )
	{
	IM_CV_LOGS(TXT("CIMCVAppView::ConstructL() start") );
	
	iRTContainerCache = CIMCVAppRichTextContainerCache::NewL();
	
	iViewId = aViewId;
	
	BaseConstructL( aAvkonViewResourceId );

	iExtensionHandler = &aMessageHandler;
    
    iFindContextMenu = new( ELeave ) CEikMenuBar;
    iFindContextMenu->ConstructL( this, 0, R_CONVERSATIONVIEW_MENUBAR_FINDITEMUI );
		                  
	iEngineFactory = CIMCVEngineFactory::InstanceL(); 
	iEngineFactory->RegisterServiceObserver( this );
		
	iFindMenu = CFindItemMenu::NewL( EIMCVCmdFindMenuPlaceHolder ); 
    iFindMenu->AttachItemFinderMenuL( R_CONVERSATIONVIEW_VIEW_MENU );
    iFindMenu->AttachItemFinderMenuL( R_CONVERSATIONVIEW_MENUBAR_FINDITEMUI );
    // automatic finder on by default
    iFindMenu->HandleItemFinderCommandL( EFindItemCmdEnableAutoFind );
    iItemFinder = CItemFinder::NewL();
    
	iAppUi = static_cast <CIMCVAppUi*> ( CCoeEnv::Static()->AppUi() );
		
	iRecipientUserId = HBufC::NewL(512);
	
	 // to set the tile, label icon in title pane
    iStatusPane = CIMCVAppStatusPaneHandler::NewL(*iEngineFactory) ;
	
    if ( AknLayoutUtils::PenEnabled() )
        {
        Toolbar()->SetToolbarObserver(this);       
        }    
    iFromMe = NULL; 
    iToMe = NULL;

    //Creates manager for menu extension
    iMenuExtManager = CIMCVMenuExtensionManager::NewL();
        iAiwServiceHandler = CAiwServiceHandler::NewL();    
    iAiwServiceHandler->AttachL( AIW_INTERNETCALL);
    iAiwServiceHandler->AttachMenuL( R_CONVERSATIONVIEW_VIEW_MENU,AIW_INTERNETCALL );  

    iConvViewDel = EFalse;
    iDetailViewOpen = EFalse;
	IM_CV_LOGS(TXT("CIMCVAppView::ConstructL() end") );	
	}

// ---------------------------------------------------------
// CIMCVAppView::~CIMCVAppView()
// ---------------------------------------------------------
//
CIMCVAppView::~CIMCVAppView()
    {  
    if( iEngineFactory )
        {
        iEngineFactory->UnRegisterServiceObserver();
        iEngineFactory = NULL;
        }
   
    if(iRTContainerCache)
        {
        delete iRTContainerCache;
        }
    delete iStatusPane;
    if ( iContainer && iAppUi )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    if ( iFindContextMenu && iAppUi )
        {
        iAppUi->RemoveFromStack( iFindContextMenu );
        }
    delete iFindContextMenu;

    delete iFindMenu;
    delete iItemFinder;
    delete iRecipientUserId;

    if(iParameter)
        {
        iParameter->Close();
        iParameter = NULL;
        }
    if(iConnection)
        {
        iConnection->Close();
        iConnection = NULL;
        }
    if(iFromMe)
        {
        delete iFromMe;
        }
    if(iToMe)
        {
        delete iToMe;
        }
    if(iMenuExtManager)
        {
        delete iMenuExtManager;
        }
    if (iAiwServiceHandler)
        {
        iAiwServiceHandler->Reset();
        delete iAiwServiceHandler; 
        iAiwServiceHandler = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CIMCVAppView::HandleCbaChangeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandleCbaChangeL( TInt aResourceId )
    {
   	IM_CV_LOGS(TXT("CIMCVAppView::HandleCbaChangeL() start") );
    Cba()->SetCommandSetL( aResourceId );
    Cba()->DrawNow();
    IM_CV_LOGS(TXT("CIMCVAppView::HandleCbaChangeL() end") );
    }


// ---------------------------------------------------------
// CIMCVAppView::DynInitMenuPaneL
// Called when menupane is about to be shown.
// ---------------------------------------------------------
//
void CIMCVAppView::DynInitMenuPaneL( 
                TInt aResourceId,CEikMenuPane* aMenuPane )
    {    
    IM_CV_LOGS(TXT("CIMCVAppView::DynInitMenuPaneL() start") );   
    if( !iContainer )
        {
        return;
        }
    switch( aResourceId )
	    {
	    case R_CONVERSATIONVIEW_VIEW_MENU:
		    {
		    if(iActiveEngine->IsVoipServiceL())// internet call
		        {
		        aMenuPane->SetItemDimmed(KAiwCmdCall, EFalse);
		        CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewL();
		        CleanupStack::PushL( dialDataExt );
		        dialDataExt->SetServiceId( iServiceId );
		        CAiwGenericParamList& paramList = iAiwServiceHandler->InParamListL();
		        dialDataExt->FillInParamListL( paramList );                          
		        TAiwGenericParam param (EGenericParamSIPAddress);
		        paramList.AppendL( param );
		        iAiwServiceHandler->InitializeMenuPaneL( *aMenuPane, 
		                R_CONVERSATIONVIEW_VIEW_MENU,EIMCVCmdInternetCall,paramList );
		        CleanupStack::PopAndDestroy( dialDataExt ); 
		        }
		    else
		        {
		        aMenuPane->SetItemDimmed(KAiwCmdCall, ETrue);
		        }
		    if( iContainer->Editor().Editor().TextLength() > 0 )
			    {
			    aMenuPane->SetItemDimmed(EIMCVCmdSend, EFalse);	
			    }
		    else
			    {
			    aMenuPane->SetItemDimmed(EIMCVCmdSend, ETrue);	
			    }
			// "insert smiley"            
		    if( !iContainer->Editor().Editor().IsFocused() )
                {
                aMenuPane->SetItemDimmed( 
                                EIMCVCmdSmiley, ETrue );
                }
            else
                {            
                aMenuPane->SetItemDimmed( EIMCVCmdSmiley, EFalse );
                }
            // item finder menu
            aMenuPane->SetItemDimmed( EIMCVCmdFindMenuPlaceHolder, ETrue);
            
            if( iContainer->IsStopped() )
                {
                UpdateItemTypeL();            
                iFindMenu->AddItemFindMenuL( iItemFinder, aMenuPane, 
                    EIMCVCmdFindMenuPlaceHolder, 
                    KNullDesC ); // we can ignore the sender here
                }   
            //offers CV MenuPane to plugins to add their options
            iMenuExtManager->OfferMenuPaneToPlugins(EIMCVCmdSmiley,*aMenuPane,iServiceId);
		    break;	
		    }
	    case R_CONVERSATIONVIEW_FINDITEMUI_MENU :       
	        {
            if( iContainer->IsStopped() )
                {
                UpdateItemTypeL();            
                iFindMenu->AddItemFindMenuL( iItemFinder, aMenuPane, 
                    EIMCVCmdFindMenuPlaceHolder,
                    KNullDesC, // we can ignore the sender here
                    EFalse, ETrue ); // context menu
                }
            break;
            }
	    
		default:
            {
            // update find menu
            iFindMenu->UpdateItemFinderMenuL( aResourceId, aMenuPane );
            break;
            }
	
	    }
    IM_CV_LOGS(TXT("CIMCVAppView::DynInitMenuPaneL() end") );
    }
    

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::LaunchCcaL
// --------------------------------------------------------------------------
//
void CIMCVAppView::LaunchCcaL()
    {   


    if(!iConnection)
        {
        iConnection = TCCAFactory::NewConnectionL();
        }

    if ( iParameter )
        {
        iParameter->Close();
        iParameter = NULL;
        }

    // The lifetime of this cmd object is too short to hold
    // iParameterRef, caller of this must own it    
    iParameter = TCCAFactory::NewParameterL();
    iParameter->SetConnectionFlag(MCCAParameter::ENormal);
    iParameter->SetContactDataFlag(MCCAParameter::EContactLink);

    TUid uid( TUid::Null() );
    uid.iUid = KVIMPSTDetailsViewPluginImplmentationUid  + iServiceId ; // service details view implementation uid
    iParameter->SetLaunchedViewUid( uid );

   // RProperty::Set(KMeCoPropertyUid, KMeCoConvViewServiceIdKey, iServiceId ); 
    MIMCVEngineMessageReadInterface& readInterface = 
        		iActiveChatInterface->MessageReadInterfaceL( iServiceId, *iRecipientUserId );
    HBufC8* link8 = readInterface.ContactLink().AllocLC();
    HBufC16* link16 = HBufC16::NewLC( link8->Length() );
    link16->Des().Copy( *link8 );
    iParameter->SetContactDataL( *link16 );
    CleanupStack::PopAndDestroy( 2 );  //pop and destroy link8 and link16

    // Sync call
    iConnection->LaunchApplicationL(*iParameter);
    
    iDetailViewOpen = ETrue;
    }
// ---------------------------------------------------------
// CIMCVAppView::HandleCommandL(TInt aCommand)
// Handles commands
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::HandleCommandL( TInt aCommand )
    {
    IM_CV_LOGS(TXT("CIMCVAppView::HandleCommandL() start") );
    
    //Offer commands to menu extension plugins
    if(iMenuExtManager->OfferHandleCommandToPlugins(aCommand))
        {
        //command is executed, hence return.
        return;
        }
    
    switch ( aCommand )
        {
        case EIMCVCmdInternetCall:
            {
            // Truncating till ':' if exists - Extracting Userid from "ServiceName:Userid".
            TInt indexposition = iRecipientUserId->Locate(':');
            HBufC* phonenumber = NULL; // Phonenumber = Userid    
            if ( KErrNotFound != indexposition )
                {
                phonenumber = iRecipientUserId->Right(iRecipientUserId->Length() - (indexposition+1)).AllocLC();
                }
            else
                {
                phonenumber = iRecipientUserId->AllocLC(); // does't contain ':'
                }
            TPtr phonenumberptr( phonenumber->Des ());
            CommonPhoneParser::ParsePhoneNumber ( phonenumberptr,CommonPhoneParser::EPhoneClientNumber); 
            CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewLC();
            dialDataExt->SetPhoneNumberL ( phonenumber->Des ());
            dialDataExt->SetServiceId(iServiceId);
            dialDataExt->SetCallType( CAiwDialData::EAIWVoiP);
            dialDataExt->SetWindowGroup ( CCoeEnv::Static()->RootWin().Identifier() );
            CAiwGenericParamList& paramList = iAiwServiceHandler->InParamListL();
            dialDataExt->FillInParamListL ( paramList);
            iAiwServiceHandler->ExecuteServiceCmdL ( KAiwCmdCall,paramList,
                    iAiwServiceHandler->OutParamListL(),0,NULL);
            CleanupStack::PopAndDestroy(dialDataExt);
            CleanupStack::PopAndDestroy(phonenumber);
            break;
            }
        case EIMCVCmdToolbarDetails:
    	    //Launch the cca
    	    LaunchCcaL();
    	    break;
    	// flow through
    	case EIMCVCmdSend:
    	case EIMCVCmdToolbarSend:
    	case EAknSoftkeySend:
			{
			if( iContainer )
			    {
			    iContainer->SendMessageL();
			    }
		   	break;
        	}
    	case EIMCVCmdToolbarInsertSmiley:	
    	case EIMCVCmdSmiley	:
			{			
			TBool dialogCancelled = EFalse;
			TSize size (iContainer->TextRect ().Height(), iContainer->TextRect ().Height() );
			iContainer->SmileyDialogLaunched (ETrue);
			iExtensionHandler->InsertExtensionL( iContainer->Editor().Editor(), 
												iServiceId,
	                                           EMessageExtensionSmiley,
	                                           dialogCancelled,
	                                           size);
	        iContainer->UpdateCbaL();
	        // Resize icons, use reading pane text line element as parent
	        
	        if ( !dialogCancelled )
	            {
	            iContainer->RefreshEditorL();
	            iContainer->Editor().SetFocus(ETrue);
	            }
			iContainer->SmileyDialogLaunched (EFalse);
			iContainer->UpdateChatViewL();
	            
			break;
			}
    	case EIMCVCmdCloseConversation:
	    	{
            HBufC* queryTxt = StringLoader::LoadLC( R_CHAT_CQUERY_CLOSE_CONV, *iRecipientUserId );
            TInt response = DisplayQueryDialogL(
                                                R_GENERIC_YES_NO_CONFIRMATION_QUERY,
                                                *queryTxt );
            CleanupStack::PopAndDestroy( queryTxt );                             
            if(response)
                { 
                iViewDeactivated = ETrue;
                TInt error = iContainer->CloseConversationL( *iRecipientUserId );
                iActiveChatInterface->DeleteChatL( iServiceId, *iRecipientUserId );
                ViewRefreshL();
                // set this to KNullDesC since conversation is getting closed.
                iRecipientUserId->Des().Copy( KNullDesC());
                // after closing conversation application will go to servicetab
                if( error== KErrNone )
                    {
                    iAppUi->HandleCommandL( EAknSoftkeyBack );
                    }
                }
	    	break;	
	    	} 
        	// flow through
        case EFindItemCmdEnableAutoFind:
        case EFindItemCmdDisableAutoFind:
            {
            if( iContainer )
                {
                iContainer->SetItemHighlightL( 
                        aCommand == EFindItemCmdEnableAutoFind );
                iFindMenu->HandleItemFinderCommandL( aCommand );
                if( MenuBar()->IsDisplayed() )
                    {
                    MenuBar()->StopDisplayingMenuBar();
                    }
                if( iFindContextMenu->IsDisplayed() )
                    {
                    iFindContextMenu->StopDisplayingMenuBar();
                    }
                }
            break;
            }
        case EIMCVCmdCall:
            {
            UpdateItemTypeL();
            CItemFinder::CFindItemExt& item = iItemFinder->CurrentItemExt();
            if(item.iItemType == CItemFinder::EEmailAddress)
                {
                iFindMenu->HandleVoIPCallL( *iItemFinder->CurrentItemExt().iItemDescriptor );
                }
            else
                {
                iFindMenu->HandleCallL( *iItemFinder->CurrentItemExt().iItemDescriptor );
                }
            break;
            }      
	    case EAknCmdHelp:
	    	{
	    	LaunchHelpL( KSERVTAB_HLP_CONVERSATION );
	    	break;	
	    	} 

	    case EEikCmdExit:
	    case EAknSoftkeyExit:
	    case EAknCmdExit:
	    case EAknSoftkeyBack:
			{
			iViewDeactivated = ETrue;
			if( iFindMenu->CommandIsValidL( aCommand ) )
			    {
			    iFindMenu->HandleItemFinderCommandL( aCommand );
			    }
			iContainer->StoreEditorMessageL();
			ViewRefreshL();
			iAppUi->HandleCommandL( aCommand );   		
			break;	
			}
    
        default:
            {
            //As per new UI spec change own status is not avilable in conversation
            //view menu  
             // check if it is find menu command
            if( iFindMenu->CommandIsValidL( aCommand ) )
                {
                Toolbar()->HideItemsAndDrawOnlyBackground(ETrue);
                iFindMenu->HandleItemFinderCommandL( aCommand );
                Toolbar()->HideItemsAndDrawOnlyBackground(EFalse);

                }
            else
	            {
	            iAppUi->HandleCommandL( aCommand );
	            }
                           
            break;
            }
    	}
    IM_CV_LOGS(TXT("CIMCVAppView::HandleCommandL() end") );	
    }

// ---------------------------------------------------------
// CIMCVAppView::HandleForegroundEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::HandleForegroundEventL( TBool aForeground )
    {
    IM_CV_LOGS(TXT("CIMCVAppView::HandleForegroundEventL() start") );   
  
    if(aForeground && iDetailViewOpen)
        {
        iDetailViewOpen = EFalse;
        }
    
    if(iConvViewDel)
        {
        TApaTask task(iEikonEnv->WsSession());
        task.SetWgId( CEikonEnv::Static()->RootWin().Identifier());
        task.SendToBackground();
        iConvViewDel = EFalse;
        return;
        }
    
    if (!aForeground)
	    {	    
	    if (iStatusPane)
		    {
		    iStatusPane->SendBackGroundL();
		    }
	    iViewDeactivated = ETrue;
		}
	else
		{
		TPtrC activeRecipientId = iEngineFactory->GetActiveItemL ();
		iActiveEngine->MessageHandler().StartNewConversationL (activeRecipientId);
		SetStatusPaneIconsL();
		if (iStatusPane)
		    {
		    iStatusPane->BringToForegroundL();
		    }		
		iViewDeactivated = EFalse;	
		}			    	

    if( iContainer )
        {
        //don't read messages if we're not in foreground
     //   iContainer->FetchMessages( aForeground );
        }
        
    IM_CV_LOGS(TXT("CIMCVAppView::HandleForegroundEventL() end") );
    }

    

// ---------------------------------------------------------
// CIMCVAppView::DoActivateL(...)
// Handles view activation
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::DoActivateL(
        const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
        const TDesC8& aCustomMessage)
    {
    IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() start") );
    // Register switch back if conversation view is opened from phonebook im service tab
    // in case if something lost
	
    ActivateViewL(ViewId());    
    
    
    TPckgBuf<TIMCVUiParams> params;
    params.Copy( aCustomMessage );

    //Get the service id  - this property is published by imcvlauncher
     TInt error = RProperty::Get( KMeCoPropertyUid, KMeCoConvViewServiceIdKey, iServiceId );
     User::LeaveIfError(error);
      
    TPtrC recieptId = params().iBuddyId;
    TPtrC buddyName = params().iBuddyName;
    TPtrC8 contactlinkPtr = params().iContactLink;
    //CCH has logged in, but conversation is not binded to the connection
    //so we need to login in conversation view, for the user to use this  view to 
    //send the message  
    iActiveEngine = &iEngineFactory->GetServiceEngineL( iServiceId );
    iActiveEngine->ResolveServiceStateL();    
	/* Get the active chat recipient id from the engine factory so that we check if 
	   container should switch */
	
	TPtrC activeRecipientId = iEngineFactory->GetActiveItemL ();
   	// store previous view details if launch from service tab
    iAppUi->StorePreviousViewDetails(params().iAppUid, params().iViewUid ); 
    
	iRecipientUserId->Des().Copy(recieptId);
    
    // subscribe for presence notification
    if ( iActiveEngine->IsLoggedIn() )
	    {
	    IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() LOGGED IN..  BEFORE subscribing.. *****************") );
	    iActiveEngine->MessageHandler().SubscribePresenceL( recieptId );
		IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() LOGGED IN..  AFTER subscribing.. *****************") );
	    }
    iActiveChatInterface = &iActiveEngine->ChatInterface();
    // To prevent looping between this view and refresh view
    // resolve message read interface
    MIMCVEngineMessageReadInterface& readInterface = 
        iActiveChatInterface->MessageReadInterfaceL( iServiceId, *iRecipientUserId, buddyName, contactlinkPtr );
    MIMCVEngineMessageWriteInterface& writeInterface = 
        iActiveChatInterface->MessageWriteInterfaceL( iServiceId, *iRecipientUserId, buddyName, contactlinkPtr );
    
    TPtrC displayId = readInterface.Name();   
    
    if (iStatusPane)
		{		
		iStatusPane->SetTitleL( displayId );
		//hide navi pane decorators
  
    	iStatusPane->IMessageIndicator()->AddObserver( this );
    	//if activation is from contact list and not from switching between
	    //tabs then create tabs and show them
		iStatusPane->ShowTabGroupL( iRecipientUserId->Des());
		iStatusPane->AddObserver(this) ;	
		}
        SetStatusPaneIconsL();
    // When forwarding from contact to contact DoDeactivate is never called.
    // We have to cleanup the necessary stuff ourself.

   	if( !iContainer )
        {
        iContainer = CIMCVAppViewContainer::NewL( ClientRect(),iActiveEngine,
                                        &readInterface,&writeInterface, 
                                        this, *iExtensionHandler,Cba(),
                                        *iRecipientUserId,this, iRTContainerCache,
                                        iFromMe, iToMe);    
		iAppUi->AddToViewStackL( *this, iContainer );
		iAppUi->AddToStackL( *this, iFindContextMenu, ECoeStackPriorityDefault, 
							  ECoeStackFlagRefusesFocus );

		IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() creating iContainer") );

        }
	else 
		{
			IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() CONTAINER exist******************************") );
			// If the recipient ID does not match the active chat, then switch the container */
			if (0 != recieptId.Compare(activeRecipientId))
				{
				IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() SWITCHING TAB ******************************") );
				iContainer->SwitchViewL (&readInterface, &writeInterface, recieptId);
				}
		}
	// Restore find state
    TInt findState = IMCVUiAppUtils::CRKeyL( KCRUidCommonUi, KCuiAutomaticHighlight );
    iContainer->SetItemHighlightL( findState );
    

    // start a conversation since we have single conversation at a time

  	iContainer->StartConversationL ();

   readInterface.SetActive (ETrue);
   	if( recieptId == KNullDesC )
	    {
	    // open from idle 
	    // not valid user id selected
	    // Construct view identification for buddylist view activation
		// launch servicetab for buddy list selection
		TVwsViewId viewIdentification(KPhonebook2AppUid,KUidIMServiceTabViewUid );
		ActivateViewL( viewIdentification );
		// close cv ,no user defined for this
	    iAppUi->ShutDownAndCloseApplicationL();	
	    }
   	MenuBar()->SetContextMenuTitleResourceId( R_CONVERSATIONVIEW_MENUBAR_FINDITEMUI );   
   	UpdateToolbarButtonsStateL();
    IM_CV_LOGS(TXT("CIMCVAppView::DoActivateL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppView::HandleCommandL()
// Handles view deactivation
// ---------------------------------------------------------
//
void CIMCVAppView::DoDeactivate()
    {
    IM_CV_LOGS(TXT("CIMCVAppView::DoDeactivate() start") );	
    //TSW bug Id - KBAA-6TM946
	if (iStatusPane)
		{
		iStatusPane->RemoveObserver(this);
		iStatusPane->IMessageIndicator()->AddObserver( NULL );
	    iStatusPane->HideFadeText();		
		}
		
    if ( iContainer )
		{            
		iAppUi->RemoveFromViewStack( *this, iContainer );
		// not in a middle of sending message
		delete iContainer;
		iContainer = NULL;
		      
		}
     if ( iFindContextMenu )
        {
        iAppUi->RemoveFromViewStack( *this, iFindContextMenu );
        }
   
     IM_CV_LOGS(TXT("CIMCVAppView::DoDeactivate() end") );
     }

// ---------------------------------------------------------
// CIMCVAPPView::Id()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CIMCVAppView::Id() const
	{
	return iViewId;
	}
// ---------------------------------------------------------
// CIMCVAppView::ActiveEngine()
// (other items were commented in a header).
// ---------------------------------------------------------
//
CIMCVEngine& CIMCVAppView::ActiveEngine() const
    {
    return *iActiveEngine;
    }
// ---------------------------------------------------------
// CIMCVAppView::SetStatusPaneIconsL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::SetStatusPaneIconsL()
	{
	IM_CV_LOGS(TXT("CIMCVAppView::SetStatusPaneIconsL() Start ") );
	if(! iStatusPane )
		{
		return;			
		}
	
	if (!iActiveEngine)
		return;
	// if the status is EServiceOut then no need to set the icon, only title needs to be set.
	if(EServiceOut == iActiveEngine->MessageHandler().GetPresenceStatus())
	    {
	    return;
	    }
	// Load status icon
	CGulIcon* connStatusIcon = 	LoadBitmapL( );
	connStatusIcon->SetBitmapsOwnedExternally( EFalse );
	CleanupStack::PushL( connStatusIcon );

	// in portrait mode we are setting the status icon to status pane
	iStatusPane->SetPicture( connStatusIcon->Bitmap(), 
										 connStatusIcon->Mask() 
										 );
	// ownership was transferred
	connStatusIcon->SetBitmapsOwnedExternally( ETrue );    

	CleanupStack::PopAndDestroy( connStatusIcon );
	
	IM_CV_LOGS(TXT("CIMCVAppView::SetStatusPaneIconsL() End ") );
	}

// -----------------------------------------------------------------------------
// CIMCVAppView::LoadDefaultBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CIMCVAppView::LoadBitmapL( )
	{
	IM_CV_LOGS(TXT("CIMCVAppView::LoadBitmapL() Start ") );
	CGulIcon* gulIcon;
	HBufC8* brandIconId = HBufC8::NewLC(KVIMPSTUISPSMaxPropertyLength);
	TPtr8 brandIconIdPtr = brandIconId->Des();
	TIMCVOnlineStatus status = 	iActiveEngine->MessageHandler().GetPresenceStatus();
	// default offline
	switch( status ) 
		{
		case EOnline:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendon);
			break;	
			}
		case EInvisible:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendinvisible);	
			break;	
			}
		case EAway:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendaway);		
			break;	
			}
		case EBusy:
		case EDoNotDisturb:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendbusy);
			break;	
			}
		case EOnPhone:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendonmobile);
			break;	
			}
		case EBlock:
		    {
		     brandIconIdPtr.Copy(KEMbmConversationsQgnpropimuserblocked);	 
		    break;
		    }
		case ECallForward:
		    {
		    brandIconIdPtr.Copy(KEMbmConversationsQgnpropimusercallforward);  
		    break;
		    }
		case EOffline :
		default:
			{
			brandIconIdPtr.Copy(KEMbmConversationsQgnpropimfriendoff);
			// offline only
			}
		}


    IM_CV_LOGS(TXT("CIMCVAppView::LoadBitmapL()::if(iAppUi->BrandingAccess()) Start ") );    
    gulIcon = iActiveEngine->GetPresenceIconL( *brandIconId );
    IM_CV_LOGS(TXT("CIMCVAppView::LoadBitmapL()::if(iAppUi->BrandingAccess()) End ") );

    CleanupStack::PopAndDestroy(brandIconId);
    IM_CV_LOGS(TXT("CIMCVAppView::LoadBitmapL() End ") );
    return gulIcon;
    }

// ---------------------------------------------------------
// CIMCVAppView::UpdateItemTypeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::UpdateItemTypeL()
    {    
    if( !iContainer )
        {
        return;
        }
    CItemFinder::CFindItemExt& item = iItemFinder->CurrentItemExt();
    switch(  iContainer->SelectedItemType() )
        {
        case CFindItemEngine::EFindItemSearchPhoneNumberBin:
            {
            item.iItemType = CItemFinder::EPhoneNumber;
            break;
            }                
        case CFindItemEngine::EFindItemSearchMailAddressBin:
            {
            item.iItemType = CItemFinder::EEmailAddress;
            break;
            }
        case CFindItemEngine::EFindItemSearchURLBin:
            {
            item.iItemType = CItemFinder::EUrlAddress;
            break;
            }
        case CFindItemEngine::EFindItemSearchScheme:
            {
            item.iItemType = CItemFinder::EUriScheme;
            break;
            }
        case KErrNotFound:  // flowthrough
        default:
            {
            item.iItemType = CItemFinder::ENoneSelected;
            break;
            }    
        }
    delete item.iItemDescriptor;
    item.iItemDescriptor = NULL;
    item.iItemDescriptor = iContainer->SelectedItemL();  // takes the ownership
    
    // this logic comes from CItemFinder::ResolveAndSetItemTypeL.
    // should be in ItemFinder engine, but for some reason it isn't,
    // so, next few lines are copypasted from AknItemFinder.cpp..    
    if( item.iItemType == CItemFinder::EUrlAddress ) 
        { 
        // old url types need prefix in order to work w/ schemehandler
        const TDesC& pref = item.iItemDescriptor->Des().Left( 4 );
        
		//fix for bug id  TJUO-79NETM ( Instant Messaging - 
		// IM opening rtsp link from message don't work)		
		//To avoid adding :http://" to rtsp links, we check it before it is done
		//and return. 
        if (pref.CompareF (KIMRTSP().Left (4)) == 0)
	        {
        	return;
	        }
        
        if ( pref.CompareF( KIMHTTPPREFIX().Left( 4 ) ) != 0 )
            {
            HBufC* tmp = item.iItemDescriptor->ReAlloc( 
                        item.iItemDescriptor->Length() + KIMHTTPPREFIX().Length() );
            if( tmp )
                {
                // realloc succeeded
                item.iItemDescriptor = tmp;
                item.iItemDescriptor->Des().Insert( 0, KIMHTTPPREFIX );
                }
            }        
        }
    if( item.iItemType == CItemFinder::EUriScheme )
        {
        // some schemes (ie. "old ones") have special handling 
        const TDesC& pref = item.iItemDescriptor->Des().Left( 4 );
        if ( pref.CompareF( KIMHTTPPREFIX().Left( 4 ) ) == 0 
            || pref.CompareF( KIMRTSP ) == 0 )
            {
            item.iItemType = CItemFinder::EUrlAddress;
            }
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppView::DisplayQueryDialogL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppView::DisplayQueryDialogL( TInt aDialogResourceId, 
            							const TDesC& aPrompt )
	{
	CAknQueryDialog* dlg = new (ELeave) CAknQueryDialog(
											CAknQueryDialog::ENoTone );
	CleanupStack::PushL( dlg );
	if( aPrompt != KNullDesC )
		{		
		dlg->SetPromptL( aPrompt );		
		}
	CleanupStack::Pop( dlg );
	return dlg->ExecuteLD( aDialogResourceId );
	}
    
// ---------------------------------------------------------
// CIMCVAppView::ShowPopUpMenuL()
// This method shows popup menu
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::ShowPopUpMenuL( )
    {
    if( iContainer && iContainer->SelectedItemType() != KErrNotFound )
        {
#ifndef RD_30_DISABLE_TOUCH
        MenuBar()->StopDisplayingMenuBar();
        MenuBar()->TryDisplayContextMenuBarL();
#else
        iFindContextMenu->StopDisplayingMenuBar();
        iFindContextMenu->TryDisplayMenuBarL();
#endif
        }
    }

// ---------------------------------------------------------
// CIMCVAppView::SwitchTabL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::SwitchTabL(TInt aActiveServiceId ,const TDesC& aChatId, const TInt /*aTabIndex*/)
	{
	//change the contact id based on tteh currenttab    	
	iRecipientUserId->Des().Copy(aChatId);
    if( aActiveServiceId != iServiceId )
        {
        SwitchToValidServiceL( aActiveServiceId );
        }
    // subscribe for presence notification
    if (iActiveEngine->IsLoggedIn())
	    {
	    iActiveEngine->MessageHandler().SubscribePresenceL( aChatId );
	    }
    // To prevent looping between this view and refresh view
    // resolve message read interface
    MIMCVEngineMessageReadInterface& readInterface = 
        iActiveChatInterface->MessageReadInterfaceL( iServiceId, *iRecipientUserId );
    MIMCVEngineMessageWriteInterface& writeInterface = 
    iActiveChatInterface->MessageWriteInterfaceL( iServiceId, *iRecipientUserId );
                
   	TPtrC displayId = readInterface.Name();
    
    if (iStatusPane)
	    {
	    iStatusPane->SetTitleL( displayId );
	    }
    SetStatusPaneIconsL();
    
    //remove the controls from view stack
    if ( iContainer )
        {            
        iAppUi->RemoveFromViewStack( *this, iContainer );        
        }

    if ( iFindContextMenu )
        {
        iAppUi->RemoveFromViewStack( *this, iFindContextMenu );
        }
	
	//again add those controls to view stack
	if ( iContainer )
        {
        iContainer->SwitchViewL(&readInterface, &writeInterface, *iRecipientUserId );            
        iAppUi->AddToViewStackL( *this, iContainer );      
        }
    
    if ( iFindContextMenu )
        {
        iAppUi->AddToStackL( *this, iFindContextMenu, ECoeStackPriorityDefault, 
                          ECoeStackFlagRefusesFocus );
        }
    UpdateToolbarButtonsStateL();
  	}    


// ---------------------------------------------------------
// CIMCVAppView::NotifyWhenCompleted()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::NotifyWhenCompleted()
    {
    MAknEditingStateIndicator* editStateIndicator = 
                                    CAknEnv::Static()->EditingStateIndicator();                                   
    
    if ( editStateIndicator )
        {
        CAknIndicatorContainer* indicatorContainer = editStateIndicator->IndicatorContainer();
              
        if (( indicatorContainer ) && (iStatusPane))
            {
            iStatusPane->HideFadeText();
            iStatusPane->HideNavipaneIndicators();            
            }
        }
    
    if ( iContainer )
        {
        CEikRichTextEditor& editor = iContainer->Editor().Editor();

		if (!editor.IsFocused())
			{
			editor.SetAknEditorFlags( editor.AknEdwinFlags()|EAknEditorFlagNoEditIndicators );	
			if (iStatusPane)
				{
				TRAP_IGNORE( iStatusPane->ShowTabL() );
				}
			}
		else
			{
			editor.SetAknEditorFlags( editor.AknEdwinFlags()&~EAknEditorFlagNoEditIndicators );			
			}
        }
    
    
    }


// ---------------------------------------------------------
// CIMCVAppView::NotifyWhenStarted()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppView::NotifyWhenStarted()
    {    
    if ( iContainer )
        {
        CEikRichTextEditor& editor = iContainer->Editor().Editor();
		if (editor.IsFocused())
			{
			editor.SetAknEditorFlags( editor.AknEdwinFlags()|EAknEditorFlagNoEditIndicators );
			}
        }
    }
        
// ---------------------------------------------------------
// CIMCVAppView::ViewRefresh()
// This method refreshes the view.
// (other items were commented in a header).
// ---------------------------------------------------------
void CIMCVAppView::ViewRefreshL()
    {
    IM_CV_LOGS(TXT("CIMCVAppView::ViewRefreshL() start") );
    
     if ( iContainer )
        {            
        iAppUi->RemoveFromViewStack( *this, iContainer );
        // not in a middle of sending message
        delete iContainer;
        iContainer = NULL;                  
        }
     if ( iFindContextMenu )
        {
        iAppUi->RemoveFromViewStack( *this, iFindContextMenu );
        }
  
     UpdateToolbarButtonsStateL();
     IM_CV_LOGS(TXT("CIMCVAppView::ViewRefreshL() End") );
    }
    
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::LaunchHelpL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//


void CIMCVAppView::LaunchHelpL( const TDesC& aContext )
    {
    CArrayFix< TCoeHelpContext >* cntx = new( ELeave ) CArrayFixFlat< TCoeHelpContext >( KHelpContextSize );
    CleanupStack::PushL( cntx );

    cntx->AppendL( TCoeHelpContext( KHelpUid, aContext ) );
    CleanupStack::Pop( cntx );
    
    //and launch help - takes ownership of context array
    HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), cntx );    
    }
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::ShowMeL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//    
void CIMCVAppView::ShowMeL( const TDesC8& aParamData )
	{
    IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() Start" ));
	TPckgBuf<TIMCVUiParams> params;
    params.Copy( aParamData );
   
    if(iAppUi) 
        {
        IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() Inside if(iAppUi)" ));
        if(iConnection)
            {
            iConnection->Close();
            iConnection = NULL;
            }
        // show me may called before ActivateViewL is called
        //Get the service id  - this property is published by imcvlauncher
        TInt error = RProperty::Get( KMeCoPropertyUid, KMeCoConvViewServiceIdKey, iServiceId );
        User::LeaveIfError(error);
        TInt serviceId = iActiveEngine->GetServiceId();
		if( serviceId != iServiceId )
			{
			SwitchToValidServiceL( iServiceId );
			}
		/* Get the current active REcipient ID, if that is not matching, then launch cv */
		TPtrC activeRecipientId = iEngineFactory->GetActiveItemL ();
	//	IM_CV_LOGS(TXT("CIMCVAppView::SHOWMEL() ACTIVE RECIPIENT ID %S *****************"), &activeRecipientId );
		
        TPtrC recieptId = params().iBuddyId;  
        TPtrC displayName = params().iBuddyName;
        iRecipientUserId->Des().Copy(recieptId);
        // show me may called before ActivateViewL is called
        // if its only update, which is true only when a contact is edited.
        IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() Inside if(iAppUi) : %d" ), params().iUpdate);

        if(params().iUpdate )
            {
            IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() if(params().iUpdate)" ));

            MIMCVEngineMessageWriteInterface& messageWriteInterface = iActiveChatInterface->
                                                                        MessageWriteInterfaceL(params().iServiceId,recieptId);
            messageWriteInterface.SetNameL(displayName);
            if(iStatusPane)
                {
                // refresh the view if the update has tacke place when the conversation view is open.
                IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() calling RefreshTabL)" ));
                iStatusPane->RefreshTabL(recieptId,displayName);
                }
          
            }
        else
            {
            IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() else" ));
            iAppUi->StorePreviousViewDetails(params().iAppUid, params().iViewUid );
            if ( (!iViewDeactivated) && (iRecipientUserId) && 
                   (serviceId == iServiceId) && 
                   (0 == recieptId.Compare(activeRecipientId)) )
                {       
                SetStatusPaneIconsL();
                if (iStatusPane)
                    {
                    iStatusPane->BringToForegroundL();  
                    }
                iAppUi->BringToForeground(TUid::Uid(KIMCVUIAPPEXEUID3));
                // when view moved to background ,active conversation was deactivated
                // comming to forground activate it
                iActiveEngine->MessageHandler().StartNewConversationL( recieptId );           
                }
            else
               {
               IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() NOT MATCHING AT ALL" ));
               iAppUi->ActivateLocalViewL(KUidConversationsView, KNullUid, aParamData);                
               }
            UpdateToolbarButtonsStateL();
            }
        }
	IM_CV_LOGS(TXT("CIMCVAppView::ShowMeL() End" ));
	}
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::SwitchToValidServiceL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//    	
void CIMCVAppView::SwitchToValidServiceL( TInt aServiceId )
    {
    if( aServiceId <= 0 )
        {
        return;
        }
    iServiceId = aServiceId;
    iActiveEngine = &iEngineFactory->GetServiceEngineL( iServiceId );
    iActiveEngine->ResolveServiceStateL();    
    iActiveChatInterface = &iActiveEngine->ChatInterface();
    if( iContainer )
        {
        // resolve message read interface
        iContainer->SetServiceParametersL( iActiveEngine );
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::OfferTabEventL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//    
TKeyResponse CIMCVAppView::OfferTabEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    TKeyResponse keyResponse = EKeyWasConsumed;
     
    CAknTabGroup* tabGroup= NULL;
    if(iStatusPane)
        {
        tabGroup = iStatusPane->TabGroup();
        
    
        if(tabGroup)
            {
            keyResponse = tabGroup->OfferKeyEventL(aKeyEvent,aType);
            }
        }
    
    return keyResponse;

    }

// ---------------------------------------------------------------------------
// CIMCVAppView::UpdateToolbarButtonsState
// ---------------------------------------------------------------------------
//
void CIMCVAppView::UpdateToolbarButtonsStateL()
    {
    if ( iContainer && iRecipientUserId )        
        {
        MIMCVEngineMessageReadInterface& readInterface = 
            iActiveChatInterface->MessageReadInterfaceL( iServiceId, *iRecipientUserId );

        if ( readInterface.ContactLink().Length()> 0 )
            {
            Toolbar()->SetItemDimmed(EIMCVCmdToolbarDetails, EFalse, ETrue);
            }
        else {
            Toolbar()->SetItemDimmed(EIMCVCmdToolbarDetails, ETrue, ETrue);
            }
        
        TBool needToDimSendBtn = ETrue;
        if(iContainer->Editor().TextLength()> 0)
            {
            needToDimSendBtn = EFalse;    
            } 
        Toolbar()->SetItemDimmed( EIMCVCmdToolbarSend, needToDimSendBtn, ETrue );
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppView: HandleNewServiceL
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandleNewServiceL( TInt aServiceId )  
    {
    if( iExtensionHandler )
        {
        iExtensionHandler->CreateServiceSmileyL( aServiceId );
        }
    }
// -----------------------------------------------------------------------------
// CIMCVAppView: HandleServiceDeletedL
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandleServiceDeletedL( TInt aServiceId ) 
    {
    if( iExtensionHandler )
        {
        iExtensionHandler->DeleteServiceSmileyL( aServiceId );
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppView::HandlePresenceChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId )
    {
    IM_CV_LOGS(TXT("CIMCVAppView::HandlePresenceChangedL() start") );
    if ( (iServiceId == aServiceId ) && ( iRecipientUserId->Compare( aBuddyId ) == 0 ) )
        {
        SetStatusPaneIconsL();
        }
    if (iContainer)
        {
        CIMCVEngine& engine = iEngineFactory->GetServiceEngineL( aServiceId );
        iContainer->InsertStatusChangedMessageL(engine, aServiceId, aBuddyId );
        }
    IM_CV_LOGS(TXT("CIMCVAppView::HandlePresenceChangedL() End") );
    }
// ---------------------------------------------------------
// CIMCVAppView::HandleOperationCompletdL
// Handles operation completion
// ---------------------------------------------------------
//
void CIMCVAppView::HandleOperationCompletdL(TInt /*aServiceId*/, MIMCVEngineMessage::TMessagerType /*aType*/ , TInt aResult )
    {
    IM_CV_LOGS(TXT("CIMCVAppView::HandleOperationCompletdL() start") ); 
    
    if( aResult!= KErrNone )
        {
        // show the corresponding error or information note
        IMCVUiAppNoteMapper::ShowNoteL( aResult,*iRecipientUserId );    
        }

    IM_CV_LOGS(TXT("CIMCVAppView::HandleOperationCompletdL() end") );  
    }

// -----------------------------------------------------------------------------
// CIMCVAppView::HandleConnectionTerminatedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandleConnectionEventL( TInt aServiceId, TIMCVConnectionState aState, TInt aReason )
    {
    IM_CV_LOGS(TXT("CIMCVAppView::HandleConnectionEventL() start") );

    switch (aState)
        {
        case EUnKnown:
            break;
        case ELoggedIn:
            // Nothing to be done.
            break;
        case ELoggedOut:
        case EForcedLoggedOut:
            {
            /*
             * Only when the application is in Foreground, we need to show the note
             * before closing the application. Once after that we need to shutdown the 
             * application
             */
            if ( iAppUi->IsAppInForeground() && ( aServiceId == iServiceId ) )
                {
                // active service 
                if (KErrNone != aReason)
                    IMCVUiAppNoteMapper::ShowNoteL( aReason , KNullDesC );
                }
			// This will be deleted by Engine factory later.
			//iActiveEngine = NULL;
			break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppView::HandleConnectionEventL() End") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppView: HandleChatListEvent
// -----------------------------------------------------------------------------
//
void CIMCVAppView::HandleChatListEvent(TInt aServiceId,TChatListEventType aEvent, 
                                      MIMCVEngineMessageReadInterface* aContainerInfo
                                     ) 
    {
    IM_CV_LOGS (TXT("CIMCVAPPVIEW "));
    // Handles the close Conversation event triggered from service tab.    
    switch (aEvent)
        {
        case EChatItemDeleted:
            {               
            DeleteContainerCache( aContainerInfo->TargetId().Target(),aServiceId );
            break;
            }
        case EChatAllItemDeleted:
            {               
			IM_CV_LOGS (TXT("CIMCVAPPVIEW  :: DELETE ALL CHATS ***************************"));
            iRTContainerCache->DeleteAllServiceContainer( aServiceId );
            break;
            }
        default:
            break;
        }
    
    }

//--------------------------------------------------------------------------------
//CIMCVAppView::DeleteContainerCache
//--------------------------------------------------------------------------------
void CIMCVAppView::DeleteContainerCache(const TDesC& aPrimaryKey, TInt aServiceId )
    {
    iRTContainerCache->DeleteContainer( aPrimaryKey,aServiceId );
    //iActiveEngine->MessageHandler().CloseConversationL( *iRecipientUserId );
    ViewRefreshL();
    iRecipientUserId->Des().Copy( KNullDesC());

    if( !iViewDeactivated ) //if conv. view is active, we should go back to ST..
        {
        //this will execute when Back operation from Detail view is performed before call back (xsp id deletion call back)
        //reaches the conv. view.
        //In this case Conv. view will be active view and Conv. view need to be closed in this scenario.
        TApaTask task(iEikonEnv->WsSession());
        task.SetWgId( CEikonEnv::Static()->RootWin().Identifier());
        task.SendToBackground();
        }
    else    //if view is not active (Current view is detail view).
        {
        if(iDetailViewOpen)
            {
            //xsp id deleted from detail view.
            //conv. view need to be closed when doing back operation from detail view.
            iConvViewDel = ETrue;
            }
        }
    
    iViewDeactivated = ETrue;
    }
//  End of File
