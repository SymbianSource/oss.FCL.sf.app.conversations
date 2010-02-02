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
* Description:  application ui
*
*/


#include 	"cimcvappui.h"
#include 	<avkon.hrh>
#include 	<aknnotewrappers.h> 
#include 	"cimcvappview.h"
#include 	"imcvuiapp.hrh"
#include 	<imcvuiapp.rsg>
#include 	<aknview.h>
#include 	"cimcvapplayoutinfo.h"
#include 	"cimcvappmessageextensionshandler.h"
#include 	"cimcvengine.h"
#include 	"imcvuiliterals.h"
#include 	"cimcvappstatuspanehandler.h"
#include 	"mimcvenginemessagehandler.h"
// logs
#include 	"imcvlogger.h"
#include 	"mimcvapplayoutchangeobserver.h"
#include 	<aknapp.h>
#include  	<AknDef.h>
#include 	<layoutmetadata.cdl.h>
#include 	<aknlayoutscalable_avkon.cdl.h>
#include 	<AknStatuspaneUtils.h>
#include 	<AknLayoutScalable_Avkon.cdl.common.h>
#include 	"imcvuicommonuids.h"
#include    "cimcvuiappskinvariant.h"
#include 	"imcvuiparams.h"
#include 	<e32property.h>
#include    <apgtask.h>
#include    <eikenv.h>
#include   <StringLoader.h>
#include   <e32property.h>

#include    "imcvuiliterals.h"
#include    "conversations.h"
#include    "imcvuiapp.h"
#include "cimcvenginefactory.h"
#include "mimcvenginefactory.h"
// imconnectionprovider
#include "cimcvappinputabsorber.h"
#include "cimcvengine.h"

// ConstructL is called by the application framework
void CIMCVAppUi::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::ConstructL() start") );
    iDestructingFlag = EFalse;
    
    BaseConstructL( EAknEnableSkin | ENoAppResourceFile |
                    EAppOrientationAutomatic | EAknEnableMSK );
    //Message extension handler. This must be created before views.
    
    iChatMbmFullPath = Application()->BitmapStoreName();
    
    // Input absorber
    iInputAbsorber = CIMCVAppInputAbsorber::NewL();    
    
    ///////////////////////////////
    
    TDriveName drive( TParsePtrC( iChatMbmFullPath ).Drive( ) ); // solve drive

    // leave to stack, LC method
    HBufC* resourceFilePath = HBufC::NewLC( KAMaxResourcePathLength );
    TPtr ptr( resourceFilePath->Des() );
    ptr.Append( KADefaultIconFile() ); // without drive
    ptr.Insert( 0, drive ); // insert drive

    // NearestLanguageFile takes only TFileName
    iChatMbmFullPath.Zero();
    iChatMbmFullPath.Append( *resourceFilePath );
    /////////////////////////////////
    CleanupStack::PopAndDestroy();
    
    iSkinVariant = CIMCVUiAppSkinVariant::NewL();   
    
    iEngineFactory = CIMCVEngineFactory::InstanceL();
       
    iMessageHandler = CIMCVAppMessageExtensionsHandler::NewL(*iSkinVariant, *this, *iEngineFactory );
    
    
    
    iLayoutInfo = CIMCVAppLayoutInfo::NewL( TChatListBoxLayout::EWithoutEditor );
    iLayoutInfo->LayoutChangedL();
    CalculatePreviewPaneWidth();
    
	// connected create view and activate it
	iCIMCVAppView = new (ELeave) CIMCVAppView();
	iCIMCVAppView->ConstructL( R_CONVERSATIONVIEW_VIEW, KUidConversationView ,*iMessageHandler);
	AddViewL( iCIMCVAppView );      // transfer ownership to CAknViewAppUi

    IM_CV_LOGS(TXT("CIMCVAppUi::ConstructL() end ") );
    }
// ---------------------------------------------------------
// CIMCVAppUi::CIMCVAppUi()
// ---------------------------------------------------------
//
CIMCVAppUi::CIMCVAppUi() :
iIsAppForeground(ETrue)                             
    {
	// no implementation required
    }
// ---------------------------------------------------------
// CIMCVAppUi::CIMCVAppUi()
// ---------------------------------------------------------
//
CIMCVAppUi::~CIMCVAppUi()
    {
    iLayoutObservers.Close();
    
    iResourceObservers.Close();
    
    delete iLayoutInfo;
     
    delete iInputAbsorber;
    
    iDestructingFlag = ETrue;
    
    if( iSkinVariant )
	    {
	    delete iSkinVariant;
	    }
    
   
    delete iMessageHandler;
    
    CIMCVEngineFactory::Release();
   
    }
// -----------------------------------------------------------------------------
// CIMCVAppUi::IsAppInForeground
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppUi::IsAppInForeground()
	{
	return iIsAppForeground;	
	}
   
  // -----------------------------------------------------------------------------
// CIMCVAppUi::IsUnderDestruction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppUi::IsUnderDestruction()
    {
    return iDestructingFlag;
    }
// ---------------------------------------------------------
// CIMCVAppUi::CIMCVAppUi()
// ---------------------------------------------------------
// handle any menu commands
void CIMCVAppUi::HandleCommandL(TInt aCommand)
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleCommandL() start") );
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
        case EIMCVCmdExit:
        case EAknSoftkeyBack:
            {
            //Deactivate conversation is done in HandleForegroundEventL so that 
            //notification can be received in UI in case of new messages
            if( iPbkApplicationUid != 0 )
                {				
                // activate previous application
                BringToForeground( TUid::Uid(iPbkApplicationUid ) );							  
                }            
            TApaTask task(iEikonEnv->WsSession());
            task.SetWgId( CEikonEnv::Static()->RootWin().Identifier());
            task.SendToBackground();
            break;  
            }
      
	   default:
            break;
        }
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleCommandL() end") );
    }
// -----------------------------------------------------------------------------
// CIMCVAPPAppUi::HandleWsEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppUi::HandleWsEventL( const TWsEvent& aEvent,CCoeControl* aDestination )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleWsEventL() start") );
    switch( aEvent.Type() )
        {
        case  KAknUidValueEndKeyCloseEvent:
            {
            
            break;
            }
        default:
            {
            CAknViewAppUi::HandleWsEventL( aEvent, aDestination );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleWsEventL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppUi::LayoutInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MIMCVAppLayoutInfo* CIMCVAppUi::LayoutInfo()
    {
    return iLayoutInfo;
    }
// -----------------------------------------------------------------------------
// CIMCVAppUi::LayoutInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CIMCVEngine& CIMCVAppUi::ActiveEngine() const
    {
    return iCIMCVAppView->ActiveEngine();
    }
    
// ---------------------------------------------------------
// CIMCVAPPAppUi::AddLayoutChangeObserver
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::AddLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::AddLayoutChangeObserver() start") );
    TInt err( iLayoutObservers.Append( aObserver ) );
    if ( err )
        {
        CActiveScheduler::Current()->Error( err );
        }
    IM_CV_LOGS(TXT("CIMCVAppUi::AddLayoutChangeObserver() End") );
    }

// ---------------------------------------------------------
// CIMCVAPPAppUi::RemoveLayoutChangeObserver
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::RemoveLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::RemoveLayoutChangeObserver() start") );
    // remove from end first (LIFO) to avoid
    // tweaking the index while removing
    TInt count( iLayoutObservers.Count() );
    for( TInt i( count - 1 ); i >= 0; i-- )
        {
        if( iLayoutObservers[i] == aObserver )
            {
            iLayoutObservers.Remove( i );
            }
        }
    iLayoutObservers.Compress();
    IM_CV_LOGS(TXT("CIMCVAppUi::RemoveLayoutChangeObserver() end") );
    }

// ---------------------------------------------------------
// CIMCVAPPAppUi::AddResourceChangeObserver
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::AddResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::AddResourceChangeObserver() start") );
    TInt err( iResourceObservers.Append( aObserver ) );
    if ( err )
        {
        CActiveScheduler::Current()->Error( err );
        }
    IM_CV_LOGS(TXT("CIMCVAppUi::AddResourceChangeObserver() end") );
    }

// ---------------------------------------------------------
// CIMCVAPPAppUi::RemoveResourceChangeObserver
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::RemoveResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::RemoveResourceChangeObserver() start") );
    // remove from end first (LIFO) to avoid
    // tweaking the index while removing
    TInt count( iResourceObservers.Count() );
    for( TInt i( count - 1 ); i >= 0; i-- )
        {
        if( iResourceObservers[i] == aObserver )
            {
            iResourceObservers.Remove( i );
            }
        }
    iResourceObservers.Compress();
    IM_CV_LOGS(TXT("CIMCVAppUi::RemoveResourceChangeObserver() start") );
    }


// ---------------------------------------------------------
// CIMCVAPPAppUi::MbmFullPath
// (other items were commented in a header).
// ---------------------------------------------------------
//
TDes& CIMCVAppUi::MbmFullPath()
    {
    return iChatMbmFullPath;
    }
// ---------------------------------------------------------
// CIMCVAPPAppUi::StorePreviousViewDetails
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::StorePreviousViewDetails(TInt aPreviousApplicationUid,TInt aPreviousViewUid )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::StorePreviousViewDetails() start") );
    iPbkApplicationUid = aPreviousApplicationUid;
    iPbkActiveViewUid = aPreviousViewUid;
    IM_CV_LOGS(TXT("CIMCVAppUi::StorePreviousViewDetails() end") );
    }

// ---------------------------------------------------------------------------
//// CIMCVAPPAppUi::SendMeToBackground
// Send this application to background
// ---------------------------------------------------------------------------
//
void CIMCVAppUi::SendMeToBackground()
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::SendMeToBackground() start") );
    TApaTaskList taskList( iEikonEnv->WsSession() );
    TApaTask xmppImApp = taskList.FindApp( TUid::Uid(KIMCVUIAPPEXEUID3 ) );
    xmppImApp.SendToBackground();
    IM_CV_LOGS(TXT("CIMCVAppUi::SendMeToBackground() end") );
    }
 
 // ---------------------------------------------------------------------------
//// CIMCVAppUi::ShutDownAndCloseApplicationL
// Send this application to background
// ---------------------------------------------------------------------------
//
void CIMCVAppUi::ShutDownAndCloseApplicationL()
	{
	IM_CV_LOGS(TXT("CIMCVAppUi::ShutDownAndCloseApplicationL() start") );
	TApaTaskList taskList( iEikonEnv->WsSession() );
	TApaTask xmppImApp = taskList.FindApp( TUid::Uid(KIMCVUIAPPEXEUID3 ) );
	xmppImApp.KillTask();
    IM_CV_LOGS(TXT("CIMCVAppUi::ShutDownAndCloseApplicationL() end") );
	}
// ---------------------------------------------------------
// CIMCVAppUi::HandleResourceChangeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::HandleResourceChangeL( TInt aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleResourceChangeL() Start") );
    CAknViewAppUi::HandleResourceChangeL( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        // skin change for this app ui class
        iLayoutInfo->LayoutChangedL( aType );
        
        CalculatePreviewPaneWidth();
        
        iSkinVariant->UpdateLayout();
	   }
        
        
    if ( aType == KEikDynamicLayoutVariantSwitch ||
        aType == KAknsMessageSkinChange )
        {
        TInt count( iLayoutObservers.Count() );
        for( TInt i( 0 ); i < count; ++i )
            {
            iLayoutObservers[i]->LayoutChangedL( aType );
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleResourceChangeL() end") );
    }
// ---------------------------------------------------------
// CIMCVAppUi::CalculatePreviewPaneWidth()
// ---------------------------------------------------------
//   
void CIMCVAppUi::CalculatePreviewPaneWidth()
	{
	IM_CV_LOGS(TXT("CIMCVAppUi::CalculatePreviewPaneWidth() Start") );
	// Get parameter and table limits for popup preview text window
    TAknLayoutScalableParameterLimits limits = 
        AknLayoutScalable_Avkon::popup_preview_text_window_ParamLimits();

    TAknLayoutScalableTableLimits tableLimits =
        AknLayoutScalable_Avkon::popup_preview_text_window_t_Limits();

    // Get layout rects
    TRect rectScreen = iAvkonAppUi->ApplicationRect();
    TRect rectMainPane = rectScreen;

    // Use first variety to be able to get the font for text parsing
    TInt firstVariety = SelectWindowVariety( 1, limits );

    TRect rectPopupWindow = RectFromLayout( rectMainPane,
        AknLayoutScalable_Avkon::popup_preview_text_window( firstVariety ) );

    TInt firstIndex = tableLimits.FirstIndex();
    TInt firstLineVariety = AknLayoutScalable_Avkon::
        popup_preview_text_window_t_ParamLimits( firstIndex ).FirstVariety();
        
    TAknTextLineLayout popupTextLayout =
        AknLayoutScalable_Avkon::popup_preview_text_window_t(
            firstIndex, firstLineVariety );

    TAknLayoutText layoutText;
    layoutText.LayoutText( rectPopupWindow, popupTextLayout );
    
    IM_CV_LOGS(TXT("CIMCVAppUi::CalculatePreviewPaneWidth() End") );   
	}
// ---------------------------------------------------------
// CIMCVAppUi::SelectWindowVariety()
// ---------------------------------------------------------
//   
TInt CIMCVAppUi::SelectWindowVariety( const TInt aNumberOfLines,
        const TAknLayoutScalableParameterLimits& aLimits ) const
    {
    TInt index = aNumberOfLines - 1;
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        index += 5;
        }

    return Min( Max( index, aLimits.FirstVariety() ),
                aLimits.LastVariety() );
    }
    
// ---------------------------------------------------------
// CIMCVAppUi::RectFromLayout()
// ---------------------------------------------------------
//   
TRect CIMCVAppUi::RectFromLayout( const TRect& aParent,
        const TAknWindowComponentLayout& aComponentLayout ) const
    {
    TAknWindowLineLayout lineLayout = aComponentLayout.LayoutLine();
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect(aParent, lineLayout);
    return layoutRect.Rect();
    }    

// -----------------------------------------------------------------------------
// CIMCVAppUi::SkinVariantL
// -----------------------------------------------------------------------------
//
MIMCVUiAppSkinVariant*	CIMCVAppUi::SkinVariantL()
	{
	return iSkinVariant;
	} 

// ---------------------------------------------------------------------------
// Proceses message from external application
// Used from Phonebooks external tab (Buddylist) to start instant messaging
// or from xmpp server to show new messages
// ---------------------------------------------------------------------------
//
void CIMCVAppUi::ProcessMessageL( TUid /*aUid*/, const TDesC8& aParamData )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::ProcessMessageL() Start") );
    
    iCIMCVAppView->ShowMeL(aParamData);    
    
    IM_CV_LOGS(TXT("CIMCVAppUi::ProcessMessageL() End ") );
    }

/// -----------------------------------------------------------------------------
// CIMCVAppUi::HandleServiceCloseExitL
// -----------------------------------------------------------------------------
//
void CIMCVAppUi::HandleServiceCloseExitL(TInt /*aEventValue*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleServiceCloseExitL() Start") );
    
    IM_CV_LOGS(TXT("CIMCVAppUi::HandleServiceCloseExitL() Start") );
    }

// ---------------------------------------------------------
// CIMCVAppUi::CaptureEventsL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::CaptureEventsL()
    {
    if ( !AreEventsCaptured() )
        {
        iInputAbsorber->CaptureEventsL();
        }
    }

// ---------------------------------------------------------
// CIMCVAppUi::ReleaseCapturingL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::ReleaseCapturingL()
    {
    if ( AreEventsCaptured() )
        {
        iInputAbsorber->Release();
        }
    }

// ---------------------------------------------------------
// CIMCVAppUi::AreEventsCaptured
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVAppUi::AreEventsCaptured() const
    {
    return iInputAbsorber->IsCaptured();
    }  
    
// ---------------------------------------------------------
// CIMCVAppUi::BringToForeground
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppUi::BringToForeground( TUid aAppUid )
	{
	
	IM_CV_LOGS(TXT("CIMCVAppUi::BringToForeground() start") );
    TApaTaskList taskList( iEikonEnv->WsSession() );
    TApaTask task = taskList.FindApp( aAppUid );
    if ( task.Exists() )
        {        
        task.BringToForeground();
        }    
    IM_CV_LOGS(TXT("CIMCVAppUi::BringToForeground() end") );
			    	
	}

	
// --------------------------------------------------------------------------
// CIMCVAppUi::HandleForegroundEventL
// --------------------------------------------------------------------------
//
void CIMCVAppUi::HandleForegroundEventL( TBool aForeground )
    {
    iIsAppForeground = aForeground;

    if( !aForeground )
	    {
	    ActiveEngine().MessageHandler().DeactivateConversationL();	
	    }

    CAknViewAppUi::HandleForegroundEventL( aForeground );
    }		       
// end of file
