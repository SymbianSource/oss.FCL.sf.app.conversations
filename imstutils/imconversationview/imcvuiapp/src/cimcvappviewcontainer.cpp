/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - Initial contribution
* 
* Contributors:
* 
* 
* Description:
* Export help related files.
*
*/

// INCLUDE FILES
#include	"cimcvappviewcontainer.h"

#include    "cimcvengine.h"
#include 	"mimcvenginemessagesreadinterface.h"
#include 	"mimcvenginemessageswriteinterface.h"
#include    "cimcvappviewcontrol.h"
#include 	"cimcvenginemessagecreator.h"
#include    "mimcvenginemessage.h"
#include	"cimcvappui.h"
#include 	"imcvuiappnotemapper.h"
#include 	"mimcvapplayoutinfo.h"
#include 	"imcvlogger.h"
#include 	"cimcvappbitmapmapping.h"
#include 	"mimcvuiappskinvariant.h"
#include 	"imcvuiapputils.h"
#include 	"cimcvappmessageextensionshandler.h"
#include 	"mimcvenginemessagecontainerinfo.h"
#include 	"mimcvenginemessagehandler.h"
#include 	"mimcvrecipientobserver.h"
#include    "imcvuiapp.hrh"
//consts
#include 	"imcvuiliterals.h"
#include    "imcvuiapp.h"
#include    <StringLoader.h>
#include 	<txtrich.h>
#include    <aknlayout.cdl.h> //cdl 
#include    <applayout.cdl.h> //cdl 
#include    <AknsUtils.h>
#include    <aknlayoutscalable_apps.cdl.h>
#include    <aknnotewrappers.h>
#include    <AknFepGlobalEnums.h> // for EAknFepSoftkeySpell
#include    <AknIconUtils.h>

#include 	<imcvuiapp.rsg>
#include 	<imerrors.hrh>

#include "conversations.h"

#include "cimcvapplogineventlistener.h"
#include "cimcvappwaitnote.h"
#include <aknEditStateIndicator.h>
#include <AknIndicatorContainer.h>

#include "mimcvenginecchhandler.h"
#include "mimcvenginemessageswriteinterface.h"
#include "mimcvenginechatinterface.h"

#include <finditemengine.h>

#include <e32property.h>
#include "cimcvappview.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::NewL
// Symbian constructor can leave.
// -----------------------------------------------------------------------------
//
CIMCVAppViewContainer* CIMCVAppViewContainer::NewL( 
            const TRect& aRect,
            CIMCVEngine* aEngine,
            MIMCVEngineMessageReadInterface* aReadInterface,
		    MIMCVEngineMessageWriteInterface* aWriteInterface,
		    CIMCVAppView* aParent,
            CIMCVAppMessageExtensionsHandler& aMessageHandler,
			CEikButtonGroupContainer* aCba,
			const TDesC& aRecipientId,			
			MIMCVRecipientObserver* aRecipientObserver,
            CIMCVAppRichTextContainerCache* aRTContainerCache,
            CGulIcon* aFromMe, CGulIcon* aToMe)
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::NewL()  start") );
	CIMCVAppViewContainer* self = new ( ELeave )
			 CIMCVAppViewContainer(aEngine,aReadInterface,aWriteInterface,
			aMessageHandler, aCba, aRecipientObserver,*aParent, aRTContainerCache );

	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aFromMe, aToMe,aRecipientId);
	CleanupStack::Pop( self );
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::NewL()  end") );
	return self;
	}

// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::CIMCVAppViewContainer
// Default constructor must not leave
// -----------------------------------------------------------------------------
//
CIMCVAppViewContainer::CIMCVAppViewContainer(CIMCVEngine* aEngine,
			MIMCVEngineMessageReadInterface* aReadInterface,
		    MIMCVEngineMessageWriteInterface* aWriteInterface, 
            CIMCVAppMessageExtensionsHandler& aMessageHandler,
			CEikButtonGroupContainer* aCba,
			MIMCVRecipientObserver* aRecipientObserver,
			CIMCVAppView& aAppView,
            CIMCVAppRichTextContainerCache* aRTContainerCache):
            iActiveEngine(aEngine),
  iReadInterface(aReadInterface),
  iWriteInterface(aWriteInterface),
  iMessageExtensionsHandler( aMessageHandler ),
  iCba( aCba ),
  iRecipientObserver( aRecipientObserver ),  
  iAppView(aAppView),
  iRTContainerCache(aRTContainerCache)
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::CIMCVAppViewContainer() start") );
    // take virtual ownership of interfaces, so that they are not deleted
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::CIMCVAppViewContainer() end") );
   	}

// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppViewContainer::ConstructL( const TRect& aRect, 
                                            MObjectProvider* aParent,                                            
                                            CGulIcon* aFromMe, CGulIcon* aToMe,
                                            const TDesC& aRecipientId )
	{	
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ConstructL() start") );
	// for getting mbm path from CIMCVAPPAppUi
	iRecipientId = aRecipientId.AllocL();
	
	iAppUi = static_cast<CIMCVAppUi*>( CEikonEnv::Static()->AppUi() );
	iAppUi->AddLayoutChangeObserver( this );

	// This must be done first.
	SetMopParent( aParent );
	CreateWindowL();

	iSkinVaiant = iAppUi->SkinVariantL();

	iFromMe = aFromMe;
	iToMe = aToMe;
	//Load from-me and to-me icons
	if(!iFromMe)
	    {
	    iFromMe = LoadBitmapL(KEMbmImcvuiappQgnpropimfrom,EMbmImcvuiappQgn_prop_im_from,EMbmImcvuiappQgn_prop_im_from_mask);
	    }
	if(!iToMe)
	    {
	    iToMe = LoadBitmapL(KEMbmImcvuiappQgnpropimtome,EMbmImcvuiappQgn_prop_im_tome,EMbmImcvuiappQgn_prop_im_tome_mask );
	    }


	iMsgEditor = CIMCVAppMessageEditor::NewL(this, iAppUi->MbmFullPath(), iAppView,
	        iMessageExtensionsHandler );
	iMsgEditor->SetMessageEditorObserver(this);
	//Sets the previously typed message in the editor
	iMsgEditor->SetTextL( iReadInterface->GetEditerMessage() );
	iChatView = CIMCVAppViewControl::NewL( aRect,
                            	        *this,
                            	        *iReadInterface,
                            	        iMessageExtensionsHandler,
                            	        iFromMe, 
                            	        iToMe,
                            	        *iRTContainerCache,
                            	        *iActiveEngine );
	iChatView->SetObserver( this );
	iMsgEditor->SetObserver( this );

	iAppUi->LayoutInfo()->SetLayoutID( TChatListBoxLayout::EWithEditor );
	iAppUi->LayoutInfo()->SetEditorVisibility( ETrue );

	SetRect( aRect );
	
	iMsgEditor->Editor().SetCursorPosL(0, EFalse);
	iChatView->SetFocus( EFalse );
	iMsgEditor->SetFocus(EFalse);

	ActivateL();

	iChatView->SetTapObserver( this, EMessageList );
	iMsgEditor->SetTapObserver( this, EMessageEditor );

	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ConstructL() end") );
	}


// ---------------------------------------------------------
// CIMCVAppViewContainer::~CIMCVAppViewContainer()
// ---------------------------------------------------------
//
CIMCVAppViewContainer::~CIMCVAppViewContainer()
    {
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::DESTRUCTING() DESTRUCTER GETTING CALLED end") );    
    if(iChatView)
        {
        delete iChatView;
        }
    if(iEditorMessage)
        {
        delete iEditorMessage;
        }
    delete iMsgEditor;
    if( iAppUi )
        {
        if( !iAppUi->IsUnderDestruction() )
            {
            iAppUi->LayoutInfo()->SetEditorVisibility( EFalse );
            iAppUi->RemoveLayoutChangeObserver( this );
            }
        }	
    }
// ---------------------------------------------------------
// CIMCVAppViewContainer::SetServiceParametersL()
// This method is called when scrolling is started again
// ---------------------------------------------------------
//          
void CIMCVAppViewContainer::SetServiceParametersL( 
                                CIMCVEngine* aNewEngine )
    {
    iActiveEngine = aNewEngine;
    }
// ---------------------------------------------------------
// CIMCVAppViewContainer::StartScrollingL()
// This method is called when scrolling is started again
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::StartScrollingL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::StartScrollingL() start") );
	// disable highlight and set focus to editor
    if( iChatView->Highlighted() )
        {
        iChatView->DisableHighlightL();
        }       
    iMsgEditor->SetFocus( ETrue, EDrawNow );
    iMsgEditor->DrawDeferred();
    UpdateCbaL();
    iAppView.UpdateToolbarButtonsStateL();
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::StartScrollingL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::StopScrollingL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::StopScrollingL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::StopScrollingL() start") );
	// unfocus editor (highlighting is enabled 
	// automatically in OfferKeyEvent of ChatView)
	iMsgEditor->SetFocus( EFalse, EDrawNow );
	iMsgEditor->DrawDeferred();
    // Set editor cursor pos to end of text
	iMsgEditor->Editor().SetCursorPosL( iMsgEditor->TextLength(), EFalse );
    UpdateCbaL();
    iAppView.UpdateToolbarButtonsStateL();
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::StopScrollingL() end") );
	}

void CIMCVAppViewContainer :: SmileyDialogLaunched (TBool aLaunched)
    {
    iChatView->SmileyDialogLaunched (aLaunched);
    }
void CIMCVAppViewContainer :: UpdateChatViewL ()
    {
    iChatView->UpdateChatViewL ();
    }
	
// ---------------------------------------------------------
// CIMCVAppViewContainer::UpdateCbaL()
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::UpdateCbaL()
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::UpdateCbaL() start") );
    if( iChatView->Highlighted() )
        {
        // focus on chatview
        HBufC* item = iChatView->SelectedItemL();
        CleanupStack::PushL( item );
        if( item && item->Length() > 0 )
            {
            // automatic find item selected
            TInt cbaRes = R_CV_SOFTKEYS_OPTIONS_BACK__CONTEXTOPTIONS;
            iCba->SetCommandSetL( cbaRes );
            iCba->DrawNow();
            }
        CleanupStack::PopAndDestroy( item );        
        }
    
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::UpdateCbaL() end") );    
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::HandleMessageEditorEventL()
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::HandleMessageEditorEventL( TEditorEvent aEvent )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleMessageEditorEventL() start") );
	
	switch( aEvent )
        {
	    case EEditorContentCleared: 
	        {	        
	        // Call StartScrollingL to update CBA if needed
	        StartScrollingL();
	        break;	
	        }
	    case EEditorContentChanged:
	        {            
	        // Call StartScrollingL to update CBA if needed
	        if(!iMsgEditor->IsFocused())//!iEditor->IsFocused())
	            {
	            StartScrollingL();	
	            }
	        break;
	        }
	    default:
	        {
	        break;
	        }
        }    
    // need to implements	
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleMessageEditorEventL() end") );	
	}


void CIMCVAppViewContainer::StartConversationL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::StartConversationL() start") );
	// start a conversation with selected user id,
	// exist cache info fetch that
	
	iActiveEngine->MessageHandler().StartNewConversationL( *iRecipientId );
	
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::StartConversationL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::CloseConversationL()
// This method handles message sending to engine
// ---------------------------------------------------------
//
TInt CIMCVAppViewContainer::CloseConversationL(const TDesC& aBuddyId )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::CloseConversationL()") );
	// close the conversation with selected user id,
	// exist cache info fetch that
	iRTContainerCache->DeleteContainer(iReadInterface->Name() ,iActiveEngine->GetServiceId()  );
	return iActiveEngine->MessageHandler().CloseConversationL( aBuddyId );
	}
// ---------------------------------------------------------
// CIMCVAppViewContainer::DeactivateConversationL()
// This method handles message sending to engine
// ---------------------------------------------------------
//
TInt CIMCVAppViewContainer::DeactivateConversationL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::DeactivateConversationL()") );
	// start a conversation with selected user id,
	// exist cache info fetch that
	return iActiveEngine->MessageHandler().DeactivateConversationL();
	}	
// ---------------------------------------------------------
// CIMCVAppViewContainer::SendMessageL()
// This method handles message sending to engine
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::SendMessageL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() start") );
	
	TBool loggedIn = IsLoggedIn();
	if( !loggedIn )
		{
		// load the promp from lOC file + server name
		HBufC* prompt = StringLoader::LoadLC( R_SERVTAB_CONNECTION_NEEDED_TO, 
		        iActiveEngine->ServiceName() );

	    TInt ret = IMCVUiAppNoteMapper::DisplayQueryDialogL( R_GENERIC_YES_NO_CONFIRMATION_QUERY,
	    												 *prompt );
	    												 
	    CleanupStack::PopAndDestroy( prompt );
	    

    	if( ( ret == EAknSoftkeyOk ) || ( ret == EAknSoftkeyYes ) )
	    	{
			// load the promp from lOC file + server name
			HBufC* prompt = StringLoader::LoadLC( R_SERVTAB_LOGIN_ONGOING, 
			        iActiveEngine->ServiceName() );

			CIMCVAppWaitNote* waitDialog = CIMCVAppWaitNote::ShowWaitNoteL( 
			        *prompt, 
			        ETrue );         
						        
			CleanupStack::PushL(waitDialog);
			
			// start capturing the events
			iAppUi->CaptureEventsL();
			
			IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() BEFORE CALLING LOGIN") );
			TRAPD(error, iActiveEngine->LoginL());
			IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() AFTER CALLING LOGIN") );					
			// stop capturing the events
    		iAppUi->ReleaseCapturingL(); 
			
			IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() start") );
			loggedIn = IsLoggedIn();
			if (KErrNone != error)
				{
				IMCVUiAppNoteMapper::ShowNoteL( KImErrServiceGeneralError , KNullDesC );				
				}
			else
			    {
			    // subscribe for presence notification
			    iActiveEngine->MessageHandler().SubscribePresenceL( *iRecipientId );
			    }
						
			CleanupStack::PopAndDestroy(); //waitDialog
			CleanupStack::PopAndDestroy(); //prompt
	    	}		    	
		}
	
	if( loggedIn )
		{
		if(iEditorMessage)
			{
			delete iEditorMessage;
			iEditorMessage = NULL;	
			}
		iEditorMessage = iMessageExtensionsHandler.ProcessMessageLC( iMsgEditor->Editor(),
		        iActiveEngine->GetServiceId() );
		
	    TPtr msgPtr( iEditorMessage->Des() );
	    CleanupStack::Pop();//iEditorMessage   
		
		if( msgPtr.Length() == 0 )
			{
			return;	
			}
		
		TPtrC ownUserId = iActiveEngine->GetLoggedInUserId() ;
		
		MIMCVEngineMessageCreator::SMessageData data =
											{
											KMessageDataVersion,
											ownUserId,
											*iRecipientId, // Recipient
											msgPtr,
											MIMCVEngineMessage::EContentText,
											MIMCVEngineMessage::EMessageSent            
											};
	    MIMCVEngineMessage* message = iActiveEngine->MessageHandler().MessageCreator().CreateMessageL( data );
	    
	    iWriteInterface->AppendL(message );
	   		
	    iActiveEngine->MessageHandler().SendMessageL( *message );
		
		ResetAfterSendL();
		StopScrollingL();
		}
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() end") );
	}


// ---------------------------------------------------------
// CIMCVAppViewContainer::InsertStatusChangedMessageL()
// This method handles message sending to engine
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::InsertStatusChangedMessageL(CIMCVEngine& aEngine ,
                                                        TInt aServiceId, 
                                                        const TDesC& aBuddyId )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::InsertStatusChangedMessageL() start") );
	
	TIMCVOnlineStatus status = 	aEngine.MessageHandler().GetPresenceStatus();
	TInt resourceId(0);
	switch( status ) 
		{
		case EOnline:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_ONLINE;
			break;	
			}
		case EInvisible:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_ONTHEPHONE;
			break;	
			}
		case EAway:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_AWAY;	
			break;	
			}
		case EBusy:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_DND;
			break;	
			}
		case EOnPhone:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_ONTHEPHONE;
			break;	
			}
		case EDoNotDisturb:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_DND;
			break;	
			}
		case EBlock:
		    {
		    resourceId = R_SERVTAB_STATUS_CHANGE_BLOCKED;
		    break;
		    }
		case EOffline :
		default:
			{
			resourceId = R_SERVTAB_STATUS_CHANGE_OFFLINE;
			// default offline always
			}
		}
	// not owned , chat interface
	MIMCVEngineChatInterface* chatInterface = &(aEngine.ChatInterface() );
	MIMCVEngineMessageReadInterface& readInterface =  chatInterface->MessageReadInterfaceL( aServiceId, aBuddyId ); 
	
	// cleanup stack
	HBufC* textmessage = StringLoader::LoadLC( resourceId, readInterface.Name()  );
	TPtrC textmessagePtr( textmessage->Des() );
	
	TPtrC ownUserId = aEngine.GetLoggedInUserId() ;
	MIMCVEngineMessageCreator::SMessageData data =
										{
										KMessageDataVersion,
										ownUserId,
										aBuddyId, // Recipient
										textmessagePtr,
										MIMCVEngineMessage::EContentText,
										MIMCVEngineMessage::EMessageOther            
										};
    MIMCVEngineMessage* message = aEngine.MessageHandler().MessageCreator().CreateMessageL( data );
    
    MIMCVEngineMessageWriteInterface& writeInterface =  chatInterface->MessageWriteInterfaceL( aServiceId, aBuddyId );
    writeInterface.AppendL( message );
  	
   	// append message to cache , this is not sent/recieve message
   	// based on status change message is automatically created
  	aEngine.MessageHandler().AppendToCacheL( aBuddyId, textmessagePtr );
   		
	CleanupStack::PopAndDestroy();// textmessage
		
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::InsertStatusChangedMessageL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::ResetAfterSendL()
// Resets arrays, lines and nick length after send
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::ResetAfterSendL()
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ResetAfterSendL() start") );	
	iMsgEditor->SetTextL(KNullDesC());
	StartScrollingL();  // Call to update CBA
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ResetAfterSendL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::IsLoggedIn()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppViewContainer::IsLoggedIn() const
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::IsLoggedIn() ") );
	
	if (!iActiveEngine->IsLoggedIn()) 
		{
		return EFalse;
		}

	return ETrue;
	}
// ---------------------------------------------------------
// CIMCVAppViewContainer::IsStopped()
// This method checks if scroll mode is on or off
// ---------------------------------------------------------
//
TBool CIMCVAppViewContainer::IsStopped() const
	{
	return iChatView->Highlighted();
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::DisplayInfoNoteL()
// Displays info note for corresponding resource id
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::DisplayInfoNoteL( const TInt aResourceId )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::DisplayInfoNoteL() start") );
	CAknNoteDialog* infoNote = new (ELeave) CAknNoteDialog( CAknNoteDialog::
			EConfirmationTone,CAknNoteDialog::EShortTimeout );

	infoNote->ExecuteLD( aResourceId );
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::DisplayInfoNoteL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::LayoutChangedL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::LayoutChangedL( TInt aType )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::LayoutChangedL() start") );
    TRect mainPaneRect;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
    SetRect( mainPaneRect );    
    // Update skin context of editor
    if ( iMsgEditor )
        {
        iMsgEditor->HandleFindSizeChanged();
        }        
	if (iRTContainerCache && aType == KAknsMessageSkinChange)
		{
		iRTContainerCache->UpdateSkinTextColorL ();
		}
    CCoeControl::HandleResourceChange( aType );
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::LayoutChangedL() end") );
	}

    
// ---------------------------------------------------------
// CIMCVAppViewContainer::SelectedItem()
// ---------------------------------------------------------
//
HBufC* CIMCVAppViewContainer::SelectedItemL()
    {
    return iChatView->SelectedItemL();
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::SelectedItemType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppViewContainer::SelectedItemType()
    {
    return iChatView->SelectedItemType();
    }
// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::SetItemHighlightL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewContainer::SetItemHighlightL( TBool aHighlight )
    {
    iChatView->SetHighlight( aHighlight );
	DrawNow ();
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::HandleEdwinSizeEventL( CEikEdwin* aEdwin,
// TEdwinSizeEvent aEventType, TSize aDesirableEdwinSize )
// This method is called when editor is extended to two lines
// ---------------------------------------------------------
//
TBool CIMCVAppViewContainer::HandleEdwinSizeEventL( CEikEdwin* /*aEdwin*/,
			TEdwinSizeEvent aEventType, TSize /*aDesirableEdwinSize*/ )
	{
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleEdwinSizeEventL() start") );
	if (aEventType == EEventSizeChanging )
		{		
		iMsgEditor->HandleFindSizeChanged();
		DrawNow();
		return ETrue;
		}
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleEdwinSizeEventL() end") );
	return EFalse;
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::HandleControlEventL
// ---------------------------------------------------------
//    
void CIMCVAppViewContainer::HandleControlEventL( CCoeControl* aControl,
                                                     TCoeEvent aEventType )
    {
   
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleControlEventL() start") );
    if( EEventRequestFocus == aEventType )
        {
        if( aControl == static_cast<CCoeControl*>(iMsgEditor) )
            {
            // Focus to editor
            StartScrollingL();
            }
        else // aControl == iChatView
            {
            // Focus to messagelist
            StopScrollingL();
            }
        }

    if( IMCVUiAppUtils::PenEnabled() )
        {
        if( EEventStateChanged == aEventType )
            {
            if( aControl == (CCoeControl*)iMsgEditor )
                {
                // Update MSK - When using virtual keyboard
                // on touch enabled devices, one is bound to receive
                // EEventStateChanged. Hence handle the same
                UpdateCbaL();
                }        
            }
        }

    IM_CV_LOGS(TXT("CIMCVAppViewContainer::HandleControlEventL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::SizeChanged()
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::SizeChanged() start") );

    TAknLayoutRect rp;

    rp.LayoutRect( Rect(), AknLayoutScalable_Apps::im_reading_pane(
                   TChatListBoxLayout::EWithEditor2Lines) );	
    // Set icon sizes, do this before setting the rect for 
    // chat view to prevent icon sizes from changing after chat
	
    // view rect is set.
	ResizeIcons( TextRect() );
	
	// set rect for chat view
	iChatView->SetRect( rp.Rect() );	
    // and editor	
	iMsgEditor->HandleFindSizeChanged();
    DrawNow();
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::SizeChanged() end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CIMCVAppViewContainer::CountComponentControls() const
    {
	// We have 2 controls
	return 2;
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CIMCVAppViewContainer::ComponentControl( TInt aIndex ) const
    {
	//returns control with aIndex
	switch ( aIndex )
		{
		case 0:
			{
			return iChatView;
			}
		case 1:
			{
			return iMsgEditor;
			}
		default:
			{
			return NULL;
			}
		}
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::Draw( const TRect& aRect ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::Draw() const start") );
  	CWindowGc& gc = SystemGc();  	  	 	
    
    iSkinVaiant->ClearChatListBoxBackGround( 
                                    gc, aRect, 
                                    iSkinVaiant->MainPaneBackgroundContext() );
	
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::Draw() const end") );
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
//												  TEventCode aType )
// Handles keyevents
// ---------------------------------------------------------
//
TKeyResponse CIMCVAppViewContainer::OfferKeyEventL( 
            const TKeyEvent& aKeyEvent,
			TEventCode aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL() start") );   
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL() is EKeyPhoneSend Event = %d") , aKeyEvent.iCode );

    //Consume the EEventKeyUp event for send key so that it will not launch the dialler before sending the actual message.
    if ( aKeyEvent.iScanCode == EStdKeyYes && aType != EEventKey )
        {
        return EKeyWasConsumed;
        }
   
    if( (iChatView->Highlighted() ) || ( !iMsgEditor->IsFocused()) )
        {
        if( (aKeyEvent.iScanCode >= '0' && aKeyEvent.iScanCode <= '9') ||
            (aKeyEvent.iScanCode >= 'a' && aKeyEvent.iScanCode <= 'z') ||
            (aKeyEvent.iScanCode >= 'A' && aKeyEvent.iScanCode <= 'Z') ||
            (aKeyEvent.iScanCode >= EStdKeyComma && aKeyEvent.iScanCode <= EStdKeyNkpPlus) ||
            aKeyEvent.iScanCode == EStdKeySpace || aKeyEvent.iScanCode == EStdKeyLeftShift )
            {
            // stop the automatic scrolling and start manual 
            // if user presses any of the number keys (0-9)
			// or user presses any key on the QWERT - key pad
            StartScrollingL();
            return EKeyWasConsumed;
            }
        }
    
    
    switch( aKeyEvent.iCode )
        {
        // flowthrough
        case EKeyEnter:
        case EKeyOK:
        case EKeyPhoneSend:
            {
            if( iMsgEditor->IsFocused() )
			    {
			    IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL() message editor Focused ") );   
                // trapping error so that focus can be returned to editor
                SendMessageL() ;
                IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL() message Sent ") ); 
                iMsgEditor->SetFocus( ETrue );
			    }
            else if( aKeyEvent.iCode == EKeyEnter || aKeyEvent.iCode == EKeyOK                    
                     && iChatView->Highlighted() && iChatView->SelectedItemType()
                     != CFindItemEngine::EFindItemSearchPhoneNumberBin )
                {
                StopScrollingL();
                iRecipientObserver->ShowPopUpMenuL();
                }
            else if ( aKeyEvent.iCode == EKeyPhoneSend
                                && iChatView->Highlighted()
                                && (iChatView->SelectedItemType()
                                    == (CFindItemEngine::EFindItemSearchPhoneNumberBin)
                                    ||(CFindItemEngine::EFindItemSearchMailAddressBin)) )
                {
                 // Phone number highlighted, send key pressed -> call
                 iAppUi->ProcessCommandL( EIMCVCmdCall );
                }
            //iAppView.UpdateToolbarButtonsStateL();
            StopScrollingL();
		    return EKeyWasConsumed;
            }

        case EKeyBackspace:
            {
			// if bitmap is selected, it is deleted "normally" in
			// iEditor's OfferKeyEvent
            if( iMsgEditor->Editor().SelectionLength() > 0 )
				{
				break;
				}

            TInt cursorPos( iMsgEditor->Editor().CursorPos() );
			if( cursorPos > 0 ) // at least one char in editor
				{
                TPtrC ptr = iMsgEditor->Editor().RichText()->Read( ( cursorPos - 1 ), 1 );
    			// if a char to be "backspaced" is a picture
    			if( ptr[ 0 ] == CEditableText::EPictureCharacter )
    				{
                    iMessageExtensionsHandler.DeleteExtensionL( iMsgEditor->Editor(), iActiveEngine->GetServiceId(),
                            EMessageExtensionSmiley, cursorPos - 1 );
                    iAppView.UpdateToolbarButtonsStateL();
					return EKeyWasConsumed;
					}
				}

			if( cursorPos < 2 )
				{
				cursorPos = 2;
				}
		
            break;
            }

        // flowthrough
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            if( !iMsgEditor->IsFocused() )
                {
                iAppView.UpdateToolbarButtonsStateL();
                return iAppView.OfferTabEventL(aKeyEvent, aType);
                }
            break;
            }

        case EKeyDownArrow: // flowthrough
        case EKeyUpArrow:
            {
            if( iMsgEditor->IsFocused() )
                {
                iAppView.UpdateToolbarButtonsStateL();
                // editor is focused, try to offer the key
                if( iMsgEditor->OfferKeyEventL( aKeyEvent, aType ) 
                        == EKeyWasConsumed )
                    {
                    // editor took it
                    return EKeyWasConsumed;
                    }
                }
                
            // offer the key to our chat control
            TKeyResponse res = static_cast<CCoeControl*>(iChatView)
                                        ->OfferKeyEventL( aKeyEvent, aType );
            
            if( iChatView->Highlighted() )
                {
                // change the cba
                StopScrollingL();
                }
            else
                {
                // change the cba
                StartScrollingL();
                }
            iAppView.UpdateToolbarButtonsStateL();
            return res;
            }

        default:
            {
            break;
            }
        }
    if ( iMsgEditor->IsFocused() )
        {           

        TKeyResponse res = iMsgEditor->OfferKeyEventL( aKeyEvent, aType );
        iAppView.UpdateToolbarButtonsStateL();        
        // Update CBA only when there is not a shift held down        
        if ( !( aKeyEvent.iModifiers & EModifierShift ) )
            {
            UpdateCbaL();
            }
        //If focus is on editer and its empty, and if user presses left arrow or right arrow keys. 
        //Move the focus to Message list to switch the tabs
        if((res == EKeyWasNotConsumed)&& (iMsgEditor->TextLength() == 0) && (aType == EEventKeyDown) &&
                ((aKeyEvent.iScanCode == EStdKeyLeftArrow) || (aKeyEvent.iScanCode == EStdKeyRightArrow)))
            {
            StopScrollingL();       //Move the focus to Message list 
            }              
        iAppView.UpdateToolbarButtonsStateL();
        return res;
        }
    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------
// CIMCVAppViewContainer::Editor
// Handles operation completion
//	
CIMCVAppMessageEditor& CIMCVAppViewContainer::Editor()	const
	{
	return *iMsgEditor;
	}
// ---------------------------------------------------------
// CIMCVAppViewContainer::RefreshEditorL
// Handles operation completion
//
void CIMCVAppViewContainer::RefreshEditorL()
    {
    //iMsgEditor->Editor().TextView()->HandleCharEditL(CTextLayout::EFCharacterInsert) ;
    iMsgEditor->Editor().DrawDeferred();
    iAppView.UpdateToolbarButtonsStateL();
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::LoadBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CIMCVAppViewContainer::LoadBitmapL(const TDesC8& /*aId*/,const TInt aBitmapId,const TInt aMaskId)
	{
    
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::LoadBitmapL() start") );
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    CGulIcon* gulIcon;
    
    if(0)
        {
        /* PPARTH
    	TRAPD(err_bitmap, iBrandingAccess->GetBitmapL( aId, bitmap, mask ));
		if(KErrNone != err_bitmap)
			{
			User::Leave( err_bitmap );
			}
		gulIcon = CGulIcon::NewL(bitmap,mask);
         */
        }
    else
    	{
		TRgb* aCustomColor = NULL;

		MAknsSkinInstance* skin = AknsUtils::SkinInstance();	

		TAknsItemID itemId( KAknsIIDNone );
		TInt colorGroupId( 0 );

		MapFromBitmapIdToAknsItemId( aBitmapId, itemId, colorGroupId );

		if( colorGroupId == 0 )
			{

			// Special case, these icons are using the same color 
			// and color group as text
			if ( aCustomColor )
				{
				// If user has set a special color for text
				AknIconUtils::CreateIconLC( bitmap, mask,  iAppUi->MbmFullPath() , 
				        aBitmapId, aMaskId );                
				AknIconUtils::SetIconColor( bitmap, *aCustomColor );		                                    
				}
			else
				{
				AknsUtils::CreateColorIconLC( skin, 
								itemId, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6, 
								bitmap, mask,  iAppUi->MbmFullPath() , 
								aBitmapId, 
								aMaskId, 
								KRgbBlack );		        
				}
		
			}
		else
			{
			AknsUtils::CreateIconLC( skin, 
							itemId, 
							bitmap, mask, iAppUi->MbmFullPath(), 
							aBitmapId,
							aMaskId );    
			}
       	gulIcon = CGulIcon::NewL( bitmap, mask );
        CleanupStack::Pop( 2 ); // bitmap, mask
    	}

       IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::LoadBitmapL() End") );
    return gulIcon;
    
    }


// -----------------------------------------------------------------------------
// CIMCVAppViewContainer::MapFromBitmapIdToAknsItemId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewContainer::MapFromBitmapIdToAknsItemId( TInt aBitmapId, 
                                TAknsItemID& aItemId, TInt& aColorGroupId )
	{
    for( TInt i( 0 ); KCABitmaps[i].iBitmapId != KErrNotFound; i++ )
        {
        if( KCABitmaps[ i ].iBitmapId == aBitmapId )
            {
            // found it
            
            aItemId = *(KCABitmaps[ i ].iItemId);
            aColorGroupId = KCABitmaps[ i ].iColorGroupId;
            return;
            }
        }
    
    // not found
    
    IM_CV_LOGS(TXT("!!!!!!!! every bitmap enumeration should be found from \
                  CABitmapMapping.h !!!!!!!! ") );  
    
    }
    

// ---------------------------------------------------------
// CIMCVAppViewContainer::ResizeIcons
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::ResizeIcons( TRect aParentRect )   
    {
	TSize size( aParentRect.Height(), aParentRect.Height() );
	AknIconUtils::SetSize( iFromMe->Bitmap(), size );
	AknIconUtils::SetSize( iToMe->Bitmap(), size );
	
	iMessageExtensionsHandler.SizeChanged( size, iActiveEngine->GetServiceId() );	    
    }


// ---------------------------------------------------------
// CIMCVAppViewContainer::TextRect
// ---------------------------------------------------------
//    
const TRect CIMCVAppViewContainer::TextRect() const
    
    {
    TAknLayoutRect rp;    
    rp.LayoutRect( Rect(), AknLayoutScalable_Apps::im_reading_pane(
            TChatListBoxLayout::EWithEditor2Lines ) );
    TAknLayoutText text;
    text.LayoutText( rp.Rect(),
            AknLayoutScalable_Apps::im_reading_pane_t1( 1 ).LayoutLine() );
    
    return text.TextRect();
    }    

// ---------------------------------------------------------
// CIMCVAppViewContainer::HandleTapEventL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::HandleTapEventL( TTapEvent aEvent, 
                                                 TUint aControlId )
    {
    // Handle the tap events which occur in our child controls:
    // Message list and Message editor.
    switch( aControlId )
        {
        case EMessageList:
            {
            // Focused message was double tapped
            if( aEvent == MIMCVTapEventObserver::EDoubleTap )
                {
                // To be implemented later
                }
            // Message or item was tapped
            else if( aEvent == MIMCVTapEventObserver::ESingleTap 
                    || aEvent == MIMCVTapEventObserver::EStartTap)
                {
                StopScrollingL();
                // Find objects activate with single tap
                iRecipientObserver->ShowPopUpMenuL();
                }
            break;
            }
        case EMessageEditor:
            {
            // Nothing to do here ATM.
            // Focus transition is handled in HandleControlEventL
            iMsgEditor->SetFocus(ETrue, EDrawNow);
            iAppView.UpdateToolbarButtonsStateL();
            break;
            }
        default:
            {
			break;
            }
        }
    }

//added for multitab support

// ---------------------------------------------------------
// CIMCVAppViewContainer::SwitchViewL
// ---------------------------------------------------------
// 
void CIMCVAppViewContainer::SwitchViewL( MIMCVEngineMessageReadInterface* aReadInterface,
											MIMCVEngineMessageWriteInterface*  aWriteInterface,
											const TDesC& aRecipientId )
	{
	if(iEditorMessage)
        {
        delete iEditorMessage;
        iEditorMessage = NULL;  
        }
    iEditorMessage = iMessageExtensionsHandler.ProcessMessageLC( iMsgEditor->Editor(),iActiveEngine->GetServiceId() );
    CleanupStack::Pop();  //  iEditorMessage
    iReadInterface->StoreEditerMessageL( *iEditorMessage );
    
	//Clears any highlght from the chat before switching away from it
    if( iChatView->Highlighted() )
        {
        iChatView->DisableHighlightL();
        } 
    
	delete iChatView;
	//iChatView is assigned NULL as NewL may leave
	iChatView = NULL;
	
	iReadInterface = aReadInterface;
	iWriteInterface = aWriteInterface;
	
	delete iRecipientId;
	iRecipientId = NULL;
	iRecipientId = aRecipientId.AllocL();
	
	iChatView = CIMCVAppViewControl::NewL( Rect(),
									*this,
									*iReadInterface,
									iMessageExtensionsHandler,
									iFromMe, 
									iToMe,
									*iRTContainerCache,
									*iActiveEngine );
		                
    iChatView->SetObserver( this );        
    
    // Fix of TSW Bug Id - EJWU-72CB2C
    UpdateCbaL(); 
    
    SizeChanged();
    
    // Observe tap events
    iChatView->SetTapObserver( this, EMessageList );
        
    iAppUi->LayoutInfo()->SetLayoutID( TChatListBoxLayout::EWithEditor );
	iAppUi->LayoutInfo()->SetEditorVisibility( ETrue );
		
	iActiveEngine->MessageHandler().StartNewConversationL( *iRecipientId );

	const TDesC& message = iReadInterface->GetEditerMessage();

	iMsgEditor->SetTextL( message);

	iAppView.UpdateToolbarButtonsStateL();
	}

// ---------------------------------------------------------
// CIMCVAppViewContainer::FetchMessages
// ---------------------------------------------------------
//
void CIMCVAppViewContainer::FetchMessages( TBool aFetch )
    {
    if(iActiveEngine->IsLoggedIn())
	    {
    	iChatView->FetchMessages( aFetch );
	    }
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewContainer: StoreEditorMessage
// -----------------------------------------------------------------------------
//TODO: move this function to cimcvappviewcontainer
void CIMCVAppViewContainer::StoreEditorMessageL()
    {
    // save message typed in the editor.
/*    HBufC* EditorMessage = NULL;
    CIMCVAppMessageEditor& MsgEditor = iContainer->Editor();
    EditorMessage = iExtensionHandler->ProcessMessageLC( MsgEditor.Editor(), iActiveEngine->GetServiceId() );
    
    // Getting the readInterface
    MIMCVEngineMessageReadInterface& readInterface = 
                iActiveChatInterface->MessageReadInterfaceL( iServiceId, *iRecipientUserId );
    readInterface.StoreEditerMessageL( *EditorMessage );
    
    CleanupStack::PopAndDestroy();// EditorMessage*/
    
    if(iEditorMessage)
        {
        delete iEditorMessage;
        iEditorMessage = NULL;  
        }
    iEditorMessage = iMessageExtensionsHandler.ProcessMessageLC( iMsgEditor->Editor(),iActiveEngine->GetServiceId() );
    CleanupStack::Pop();  //  iEditorMessage
    iReadInterface->StoreEditerMessageL( *iEditorMessage );
    }



//  End of File

