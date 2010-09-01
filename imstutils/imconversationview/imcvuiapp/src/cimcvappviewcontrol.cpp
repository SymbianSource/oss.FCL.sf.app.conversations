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
* Description:  text view control container 
*
*/


// INCLUDE FILES
#include    "cimcvappviewcontrol.h"

#include    "cimcvapprichtextcontainer.h"
#include 	"cimcvappui.h"
#include	"cimcvappmessageflowhandler.h"
#include    <AknUtils.h>
#include    <AknSettingCache.h>
#include    <aknenv.h>
#include    <aknconsts.h>
#include    <avkon.mbg>
#include    <eikrted.h>
#include 	"imcvlogger.h"
#include	"mimcvtapeventobserver.h"
#include    "mimcvenginemessagesreadinterface.h"
#include    "mimcvenginemessagecontainerinfo.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::CIMCVAppViewControl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppViewControl::CIMCVAppViewControl()
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::ConstructL( const TRect& aRect, 
                        const CCoeControl& aParent,
                        MIMCVEngineMessageReadInterface& aReadInterface,
                        CIMCVAppMessageExtensionsHandler& aExtensionsHandler,
                        CGulIcon* aFromMe,
                        CGulIcon* aToMe,
                        CIMCVAppRichTextContainerCache& aRTContainerCache,
                        CIMCVEngine& aActiveEngine,
                        TBool aScrollOver )
    {
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ConstructL() start") );   
	 
	CWindowGc& gc = SystemGc();
	CBitmapDevice* device = static_cast<CBitmapDevice*>( gc.Device() ); 

	TInt serviceId = aActiveEngine.GetServiceId();
	// text container
	// Load the container from the cache
	iTextContainer = aRTContainerCache.LoadContainer(serviceId, aReadInterface.TargetId().Target() );
	if(NULL == iTextContainer )
	    {
		//If container not found in cache, create a new and add it to cache
	    iTextContainer = CIMCVAppRichTextContainer::NewL(static_cast<CIMCVAppUi*>( iEikonEnv->AppUi() ),
	            this, 
	            &aExtensionsHandler,
	            aFromMe, aToMe,device, 
	            aReadInterface.TargetId().Target(),
	            serviceId,
	            aScrollOver
	             );
	    
	    aRTContainerCache.StoreContainer(iTextContainer);
	    }
	else
	    {
		//If container is loaded from the cache, Then Provide it with the Parent's object address
	    iTextContainer->Initialize( static_cast<CIMCVAppUi*>( iEikonEnv->AppUi() ),
                this, 
                &aExtensionsHandler,
                aFromMe, aToMe,device, 
                aScrollOver );
	    }


	// construct base class
	CIMCVAppTextViewControl::ConstructL( aRect, aParent, 
	                                iTextContainer->TextLayout() );

	// create flow handler and fetch messages
	// Use aScrollOver to determine is this recorded chat or not,
	// it is EFalse only with recorded chats.
	SetPenBehaviour( EPenProcessEvent );
	iFlowHandler = CIMCVAppMessageFlowHandler::NewL( *iTextContainer,
												aReadInterface,
												aActiveEngine,
												!aScrollOver );
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ConstructL() end") );   
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::SetTapObserver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::SetTapObserver( MIMCVTapEventObserver* aObserver, TUint aId )
    {
    iTapObserver = aObserver;
    iTapControlId = aId;
    }


// -----------------------------------------------------------------------------
// CIMCVAppViewControl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppViewControl* CIMCVAppViewControl::NewL( const TRect& aRect, 
                        const CCoeControl& aParent,
                        MIMCVEngineMessageReadInterface& aReadInterface,
                        CIMCVAppMessageExtensionsHandler& aExtensionsHandler,
                        CGulIcon* aFromMe,
                        CGulIcon* aToMe,
                        CIMCVAppRichTextContainerCache& aRTContainerCache,
                        CIMCVEngine& aActiveEngine,
                        TBool aScrollOver /*= ETrue*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::NewL() start") );   
    CIMCVAppViewControl* self = new( ELeave ) CIMCVAppViewControl;
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aParent, aReadInterface,  
                      aExtensionsHandler,aFromMe, aToMe,aRTContainerCache,
                      aActiveEngine,
                      aScrollOver );
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::NewL() end") );   
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::~CIMCVAppViewControl
//  Destructor
// -----------------------------------------------------------------------------

CIMCVAppViewControl::~CIMCVAppViewControl()
    {
    delete iFlowHandler;
    }

    

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::SelectedItem
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CIMCVAppViewControl::SelectedItemL()
    {
    return iTextContainer->SelectedItemL();
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppViewControl::SelectedItemType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppViewControl::SelectedItemType()
    {
    return iTextContainer->SelectedItemType();
    }


// -----------------------------------------------------------------------------
// CIMCVAppViewControl::DisableHighlightL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::DisableHighlightL()
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::DisableHighlightL()	start") );   
    iTextContainer->DisableHighlightL();
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::DisableHighlightL()	end") );   
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppViewControl::Highlighted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppViewControl::Highlighted()
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::Highlighted()	start") );
    
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::Highlighted()	end") );      
    return iTextContainer->Highlighted();
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::SetHighlight
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::SetHighlight( TBool aHighlight )
    {
    iTextContainer->SetItemHighlight( aHighlight );
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CIMCVAppViewControl::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                                 TEventCode aType )
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL()	start") );   
    if( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }

    CIMCVAppRichTextContainer::THighlightMovement movement( 
                                                CIMCVAppRichTextContainer::ENext );
    
    switch( aKeyEvent.iCode )
        {
        case EKeyUpArrow:
            {
            movement = CIMCVAppRichTextContainer::EPrevious;
            // flowthrough
			}
        case EKeyDownArrow:
            {                        
            return iTextContainer->MoveHighlightL( movement ) == KErrNone ? 
                        EKeyWasConsumed : EKeyWasNotConsumed;
            }

        default:
            {
            // No handling for other events
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::OfferKeyEventL()	end") );   
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::ProcessStylusEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::ProcessStylusEventL( 
    const TPointerEvent& aPointerEvent )
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::ProcessStylusEventL()	start") );   
    // We end up here only if the pointer event started inside
    // the text area.
    if( aPointerEvent.iType == TPointerEvent::EButton1Down ||
        aPointerEvent.iType == TPointerEvent::EDrag )
        {
        // Reset poiter flags
        if( aPointerEvent.iType == TPointerEvent::EButton1Down )
            {
            iWaitForDoubleTap = iWaitForSingleTap;
            iWaitForSingleTap = ETrue;
             }
        
        TInt pos = FindDocPos( aPointerEvent.iPosition );
        if( pos != KErrNotFound )
            {
            TInt res = iTextContainer->HighlightItemAtPosL( pos );
            if( res == KErrNone )  
                
                {
                // New item/message was highlighted 
                if( aPointerEvent.iType == TPointerEvent::EDrag )
                    {
                    // Single tap is not possible if highlight was changed
                    // with draging
                    iWaitForSingleTap = EFalse;
                    }
                // doubletap is not possible for now
                iWaitForDoubleTap = EFalse;
                }
            }
         }
    
    if( iTapObserver )
        {
        if( aPointerEvent.iType == TPointerEvent::EButton1Down )
            {
            // Start
            iTapObserver->HandleTapEventL( 
                MIMCVTapEventObserver::EStartTap, iTapControlId );
            }
        }
    
   	IM_CV_LOGS(TXT("CIMCVAppViewContainer::ProcessStylusEventL()	end") );   
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::SizeChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::SizeChanged()
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::SizeChanged()	start") );   
    SetHighlighted( Highlighted() );
    CIMCVAppTextViewControl::SizeChanged();
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::SizeChanged()	end") );   
    }

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::MoveHighlightL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::MoveHighlightL( 
                            CIMCVAppRichTextContainer::THighlightMovement aMovement)
    {
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::MoveHighlightL()	start") );   
    iWaitForSingleTap = ETrue;
    iTextContainer->MoveHighlightL( aMovement );
    IM_CV_LOGS(TXT("CIMCVAppViewContainer::MoveHighlightL()	end") );   
    }
// -----------------------------------------------------------------------------
// CIMCVAppViewControl::GetMessageCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppViewControl::GetMessageCount()
	{
	return iTextContainer->GetMessageCount();	
	}

// -----------------------------------------------------------------------------
// CIMCVAppViewControl::FetchMessages
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppViewControl::FetchMessages( TBool aFetch )
    {
    iFlowHandler->FetchMessages( aFetch );
    }

//  End of File
