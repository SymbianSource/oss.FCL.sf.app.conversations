/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Fader control in navi pane
*
*/


// INCLUDE FILES
#include    "cimcvnewmsgfadecontrol.h"
#include	"mimcvnewmsgfadecontrolobserver.h"

#include	"cimcvappui.h"
#include	"mimcvapplayoutinfo.h"

#include	<eikenv.h>
#include	<AknUtils.h>
#include	<aknnavi.h>
#include	<aknconsts.h>
#include    <aknenv.h>
#include    <applayout.cdl.h>
#include    <AknsDrawUtils.h>
#include    <AknsBasicBackgroundControlContext.h>


//CONSTANTS
const TInt KLafCount( 10 ); //Count of LAF indexes
const TInt KDelayColorIndex( 5 ); //From LAF: Color 0 is in index 5
const TTimeIntervalMicroSeconds32 KIndicationDelay( 4000000 ); //4 seconds waiting
const TUint16 KAIMMessageIcon( 0xF808 ); //PUA icon code for AIMMESSAGE

// ========================= MEMBER FUNCTIONS ================================

// -----------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVNewMsgFadeControl* CIMCVNewMsgFadeControl::NewL( const TRect& aRect )
	{
	CIMCVNewMsgFadeControl* self = CIMCVNewMsgFadeControl::NewLC( aRect );
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVNewMsgFadeControl* CIMCVNewMsgFadeControl::NewLC( const TRect& aRect )
	{
	CIMCVNewMsgFadeControl* self = new ( ELeave ) CIMCVNewMsgFadeControl();
	CleanupStack::PushL( self );
	self->ConstructL( aRect );
	return self;
	}   

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CIMCVNewMsgFadeControl::ConstructL( const TRect& aRect )
    {

	iLayoutInfo = static_cast<CIMCVAppUi*>( iEikonEnv->AppUi() )->
			LayoutInfo();
	// Currently we have 2 observers, so granularity of 2 is logical choice.
	iObserverArray = new ( ELeave ) CArrayPtrSeg<MIMCVNewMsgFadeControlObserver>(2);
	iIMessage = HBufC::NewL( 512 + 1);//1 is indication character
	iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
	
	iBgContext = CAknsBasicBackgroundControlContext::NewL( 
	        KAknsIIDQsnBgNavipaneSolid, aRect, EFalse );
	iViewRect = TRect( aRect );
	CreateWindowL();
	MakeVisible( EFalse );
	}

// Constructor
CIMCVNewMsgFadeControl::CIMCVNewMsgFadeControl()
	:iTimerState( ETimerStopped )	
	{
	
	}

// Destructor
CIMCVNewMsgFadeControl::~CIMCVNewMsgFadeControl()
    {
	
	if( iObserverArray ) 
		{
		iObserverArray->Reset();
		delete iObserverArray;
		}
	delete iTimer;
	delete iIMessage;
	delete iClippedMessage;
	delete iBgContext;
	iTextLayouts.Close();
	}

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::SizeChanged()
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CIMCVNewMsgFadeControl::SizeChanged()
    {
	TRect myNavePaneRect;
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::ENaviPane, 
	        myNavePaneRect );

	
    myNavePaneRect.Move( -myNavePaneRect.iTl.iX, -myNavePaneRect.iTl.iY );

	iTextLayouts.Reset();
	for ( TInt i( 0 ); i < KLafCount; i++ )
	    {
		TAknLayoutText text;
		TAknTextLineLayout textLayout;
		textLayout= iLayoutInfo->IM_navi_pane_texts_Line_1( i, 0 );

		text.LayoutText( myNavePaneRect, textLayout );
		iTextLayouts.Append( text );
		
	    }	
	TAknWindowLineLayout navipaneLayout = 
	                        AknLayout::Status_pane_elements_Line_1();
	iNavipaneRect.LayoutRect( myNavePaneRect, navipaneLayout );
	
	UpdateClippedMessage();
	}

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::CountComponentControls() const
//
//  
// ---------------------------------------------------------------------------
//
TInt CIMCVNewMsgFadeControl::CountComponentControls() const
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::ComponentControl(TInt aIndex) const
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CIMCVNewMsgFadeControl::ComponentControl(TInt /*aIndex*/) const
    {
	return NULL;
    }

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::Draw(const TRect& aRect) const
// 
//  
// ---------------------------------------------------------------------------
//
void CIMCVNewMsgFadeControl::Draw( const TRect& /*aRect*/ ) const
	{
	CWindowGc& gc = SystemGc();
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = iBgContext;
	if ( !AknsDrawUtils::Background( skin, cc, gc, iViewRect ) )
		{
		gc.Clear( iViewRect );
		}		
    iTextLayouts[iColorIndex].DrawText( gc, *iClippedMessage );
	
	}

// ---------------------------------------------------------------------------
// CIMCVNewMsgFadeControl::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
//  
//  
// ---------------------------------------------------------------------------
//
void CIMCVNewMsgFadeControl::HandleControlEventL(CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
    {
    }

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::FadeText()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVNewMsgFadeControl::FadeText( TAny* aAnyPtr )
	{
	if ( !aAnyPtr ) 
		{
		return EFalse;
		}
	
	CIMCVNewMsgFadeControl* view = reinterpret_cast< CIMCVNewMsgFadeControl* >( aAnyPtr );
	
	if( view->iColorIndex < KLafCount - 1 ) 
		{
		view->iColorIndex++;
		view->ReportChange();
		if ( view->iColorIndex == KDelayColorIndex )
			{
			//Delayed color is displayed 4 seconds
			view->StartTicker( KIndicationDelay );
			}	
		return ETrue;
		}
	
	view->iColorIndex = 0;
	view->Stop();
	TInt count( view->iObserverArray->Count() );
	for( TInt i(0); i < count; ++i )
		{
        MIMCVNewMsgFadeControlObserver* obs = view->iObserverArray->At( i );
		if( obs ) 
			{
			obs->NotifyWhenCompleted();
			}
		}		
	return EFalse;	
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::Start()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::Start()
	{
	TRect myNavePaneRect;
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::ENaviPane, 
	    myNavePaneRect );
	myNavePaneRect.Move( -myNavePaneRect.iTl.iX, -myNavePaneRect.iTl.iY );
	
	TAknWindowLineLayout navipanelayout = AknLayout::navi_pane( 0 );
	AknLayoutUtils::LayoutControl( this, myNavePaneRect, navipanelayout );
	
	iTimerTick = 10000; //KTimerInterval;
	
	TInt count( iObserverArray->Count() );
	for( TInt i(0); i < count; ++i )
		{
        MIMCVNewMsgFadeControlObserver* obs = iObserverArray->At( i );
		if( obs ) 
			{
			obs->NotifyWhenStarted();
			}
		}
		
	if ( !IsActivated() ) 
		{
		TRAPD( err, ActivateL() );
        if( err != KErrNone )
            {
            CActiveScheduler::Current()->Error( err );
            }
		}
	
	if ( !IsVisible() ) 
		{
		MakeVisible( ETrue );
		}	
	
	StartTicker( 0 );
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::StartTicker()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::StartTicker( 
            const TTimeIntervalMicroSeconds32 aDelayInternal )
	{
	if ( iTimerState == ETimerActivated ) 
		{
		iTimer->Cancel();
		iTimerState = ETimerStopped;
		}
	TCallBack cb( FadeText, this );
	iTimer->Start( aDelayInternal, iTimerTick, cb );
	iTimerState = ETimerActivated;
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::Stop()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::Stop()
	{	
	if ( IsVisible() ) 
		{
		MakeVisible( EFalse );
		}	
	StopTicker();
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::AddObserver()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::AddObserver( MIMCVNewMsgFadeControlObserver* aObserver )
	{	
	TRAPD( err, iObserverArray->AppendL( aObserver ) );
	HandleError( err );
	}	

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::SetMessage()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::SetMessage( TDesC& aMsg )
	{
    HBufC* temp = NULL;
    TRAPD( err, temp = HBufC::NewL( aMsg.Length() + 1 ) );
	HandleError( err );
    if( temp )
        {
        delete iIMessage;
	    iIMessage = temp;
	    TPtr imMsgPtr( iIMessage->Des() );
		imMsgPtr.Append( TChar( KAIMMessageIcon ) ); 
		imMsgPtr.Append( aMsg );

        UpdateClippedMessage();
        }
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::Message()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC CIMCVNewMsgFadeControl::Message() const
	{
	return *iIMessage;
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::IsActive()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCVNewMsgFadeControl::IsActive()
	{
	if ( iTimer ) 
		{
		return iTimer->IsActive();
		}
	return EFalse;
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::Reset()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::Reset()
	{
	if ( iIMessage ) 
		{
		TPtr mesgPtr( iIMessage->Des() );
		mesgPtr.Zero();
		}
    
    if( iClippedMessage )
        {
        TPtr clipPtr( iClippedMessage->Des() );
        clipPtr.Zero();
		DrawNow();
        }
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::StopTicker()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::StopTicker()
	{
	if ( iTimerState == ETimerActivated ) 
		{
		iTimer->Cancel();
		iTimerState = ETimerStopped;
		}	
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::ReportChange()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::ReportChange()
	{
	TRAPD( err, ReportEventL( MCoeControlObserver::EEventStateChanged ) );
	HandleError( err );
	}

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::HandleError
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::HandleError( TInt aError )
    {
    
    if( (aError < KErrNone) )
        {
        // propagate system errors to current active scheduler,
        // it should show a note
        CActiveScheduler::Current()->Error( aError );
        }

    // imps errors are ignored at the moment as we don't have any
    // notes specified for them
    }

// ---------------------------------------------------------
// CIMCVNewMsgFadeControl::UpdateClippedMessage
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVNewMsgFadeControl::UpdateClippedMessage()
    {
    HBufC* temp = NULL;
    TRAPD( err, temp = iIMessage->AllocL() );
    if ( err )
        {
        delete temp;
        HandleError( err );
        }
    
    if ( temp )
        {
        delete iClippedMessage;
        iClippedMessage = temp;
        TPtr clipPtr( iClippedMessage->Des() );
        
        const CFont* font = iTextLayouts[iColorIndex].Font();
        AknTextUtils::ClipToFit( clipPtr, *font, Rect().Width() );
        }
    }

// End of file
