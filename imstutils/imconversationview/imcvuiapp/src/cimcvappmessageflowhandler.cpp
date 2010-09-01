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
* Description:  Message flow handler class
*
*/


// INCLUDE FILES
#include    <coemain.h>

#include    "cimcvappmessageflowhandler.h"
#include    "mimcvenginemessagecontainer.h"
#include    "cimcvenginecontextobserver.h"
#include    "mimcvenginemessagesreadinterface.h"
#include 	"imcvlogger.h"
#include	"cimcvappui.h"
#include 	"cimcvengine.h"
#include	"mimcvenginechatinterface.h"


// CONSTANTS
const TInt KMilliToMicro     = 1000;    // multiplier for converting milliseconds to microseconds

const TInt KTimeIntervalSlow = 2000;    // slowest message speed (in milliseconds)
const TInt KTimeIntervalFast =  200;    // fastest message speed (in milliseconds)

const TInt KTimeIntervalOpen =    0;    // message fetching speed when opening the view
const TInt KInitialMessages  =    3;    // fetch n messages at once when opening the view

const TInt KSettingValueMin  =    1;    // minimum value for flow control setting
const TInt KSettingValueMax  =    3;    // maximum value for flow control setting

const TInt KIMFeatBackgroundGroupOpening = 0x00000008;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::CIMCVAppMessageFlowHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppMessageFlowHandler::CIMCVAppMessageFlowHandler( MIMCVEngineMessageContainer& aMessageContainer, 
                MIMCVEngineMessageReadInterface& aReadInterface,CIMCVEngine& aActiveEngine )
    : CTimer( EPriorityStandard ), 
      iMessages( aMessageContainer ),
      iReadInterface( aReadInterface ),
      iActiveEngine( aActiveEngine ),
      iFetchMessages( ETrue )
     {
	CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CCAMessageFlowHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppMessageFlowHandler* CIMCVAppMessageFlowHandler::NewL( 
                                    MIMCVEngineMessageContainer& aMessageContainer, 
                                    MIMCVEngineMessageReadInterface& aReadInterface,
                                    CIMCVEngine& aActiveEngine,
                                    TBool aScrollOver )
	{
	CIMCVAppMessageFlowHandler* self = new( ELeave ) CIMCVAppMessageFlowHandler( 
            aMessageContainer,
            aReadInterface ,
            aActiveEngine );

    CleanupStack::PushL( self );
    self->ConstructL( aScrollOver );
    CleanupStack::Pop( self );

    return self;
	}
// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::ConstructL( TBool aScrollOver )
    {
    // construct base class
    CTimer::ConstructL();
	
    // for getting chat interface from CIMCVAPPAppUi
    iActiveEngine.ChatInterface().RegisterChatListObserver(this);	
	
	// and observe changes
    iReadInterface.SetObserver( this );
    iReadInterface.SetActive(ETrue);
    
    // fetch flow control value from settings
    UpdateTimeIntervalL();        
    
    if ( aScrollOver )
        {
        iBgOpeningMode = EFalse;
        }
    else
        {
    	iBgOpeningMode = 0 & KIMFeatBackgroundGroupOpening;
        }
    
    // start the timer if there are messages
    iInitialMsgCount = iReadInterface.MessageCount();
    if( iInitialMsgCount + iReadInterface.UnreadCount() > 0 )
        {
        // lock the buffer
        iReadInterface.Lock( ETrue );
    
        // use faster timer when constructing
        iTimeInterval = KTimeIntervalOpen * KMilliToMicro;
        After( iTimeInterval );
        }        
  }


// Destructor
CIMCVAppMessageFlowHandler::~CIMCVAppMessageFlowHandler()
    {
    // for getting chat interface from CIMCVAPPAppUi
    iActiveEngine.ChatInterface().UnregisterChatListObserver(this);	
		    
    if ( !iChatDeleted )
        {
        iReadInterface.SetActive(EFalse);
        iReadInterface.SetObserver( NULL );
        iReadInterface.Lock( EFalse );
        }
    Cancel();
	}


// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::FetchMessages
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::FetchMessages( TBool aFetch )
    {
    TBool reallyInBg =
        CCoeEnv::Static()->RootWin().OrdinalPosition() == 0 ? EFalse : ETrue;
    
    if ( !reallyInBg && !aFetch )
        {
        // Application is not really in background, this can happen
        // if key lock is activated while application is in foreground
        aFetch = ETrue;
        }
    
    iFetchMessages = aFetch;
    if( iFetchMessages && !iChatDeleted )
        {
        // we're allowed to fetch messages again
        if( iReadInterface.UnreadCount() > 0 && !IsActive() )
            {
            // there are some unread messages
            // => start the timer (if not active already)
            After( iTimeInterval );
            }
        }   
    
    else if( iMsgIndex < iInitialMsgCount )
        {
        Cancel();
        After( iTimeInterval );        
        return;
        }
    else
        {
        // we're not allowed to fetch new messages any more, so cancel the timer
        Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::HandleMessageEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::HandleMessageEvent( TMessageEventType aEvent, 
                                                TInt aIndex )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageFlowHandler::HandleMessageEvent event %d, \
                         index %d"), aEvent, aIndex );
    switch( aEvent )
        {
        case ENewMessage:
            {
            if( !IsActive() && iFetchMessages )
                {
                // if not active, start timer
                After( iTimeInterval );
                }
            
            if ( IsOpening() && iBgOpeningMode )
                {
                // New message during opening phase
                iNewMsgWhileOpening = ETrue;
                }
                
            // otherwise do nothing as timer fetches the messages
            break;
            }
		
        default:
            {            
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::RunL()
    {    
    TBool messages( ETrue );
    TBool opening = IsOpening();
    
    // If we're opening the view, fetch KInitialMessages at a time, otherwise just one
    TInt count( opening ? KInitialMessages : 1 );
    
    while( count-- > 0 && messages )
        {
        if ( iBgOpeningMode && opening )
            {
            // Opening the chat with background message fetching.
            // Perform steps in following order:
            // 1. Add possible new messages that are received during
            //    the opening phase to the end of chat normally
            // 2. Insert unread messages to the beginning of chat

            if ( iNewMsgWhileOpening && iFetchMessages ) 
                {
                // Add the new message now
                iMessages.AddMessageL( iReadInterface.ReadNextUnread() );                						
            	
                iNewMsgWhileOpening = EFalse;
                }
            else if ( iReadInterface.UnreadCount() > 0 && iFetchMessages ) 
                {
                // Insert unread messages, insert in last-to-first order
                iAddedUnreadMsgs++;
                
                iMessages.InsertMessageL(  
                	iReadInterface.ReadUnreadFromIndex(
                    iReadInterface.MessageCount()
                    + iReadInterface.UnreadCount()
                    - iAddedUnreadMsgs )  );
                
                
                }

            else
                {
                // Check if the initial speed was active
                if ( opening )
                    {
                    UpdateTimeIntervalL();
                    }
                messages = EFalse;
                iReadInterface.Lock( EFalse );
                }
            }
        else
            {
            // Functionality in opening in releases 3.1 and earlier
            // and normal functionality when the chat is already fully opened
            // and new messages are received.
            
            //  Add unread messages in first-to-last order
            if ( iReadInterface.UnreadCount() > 0 )
                {
                // Add unread messages
                iMessages.AddMessageL( iReadInterface.ReadNextUnread() );
				
                }
            else
                {
                // Check if the initial speed was active
                if ( opening )
                    {
                    UpdateTimeIntervalL();
                    }
                messages = EFalse;
                iReadInterface.Lock( EFalse );
                }
            }
        }

    // And restart timer if needed    
    if ( messages )        
        {
        Cancel();
        After( iTimeInterval );
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::RunError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppMessageFlowHandler::RunError( TInt aError )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageFlowHandler::RunError (%d)"), aError );
    
    // Something leaved in RunL
    if( aError == KErrNoMemory )
        {
        // inform user about low memory
        CActiveScheduler::Current()->Error( aError );
        }
    
    if( IsActive() )
        {
        // stop processing messages
        Cancel();
        }
    
    return KErrNone;
    }



// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::UpdateTimeIntervalL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::UpdateTimeIntervalL()
    {
    // codescanner warning can be ignored
    TInt flowSetting( 10 );
    
    // flowSetting is from KSettingValueMin to KSettingValueMax
    TInt range( KSettingValueMax - KSettingValueMin );
    TInt newRange( KTimeIntervalFast - KTimeIntervalSlow );
    
    // convert it to a range from KTimeIntervalSlow to KTimeIntervalFast
    TInt flowSpeed( (flowSetting-KSettingValueMin)*newRange / range );
    
    // update the end point
    flowSpeed += KTimeIntervalSlow;
    
    // validate the result
    if( flowSpeed > KTimeIntervalSlow )
        {
        flowSpeed = KTimeIntervalSlow;
        }
        
    if( flowSpeed < KTimeIntervalFast )
        {
        flowSpeed = KTimeIntervalFast;
        }

    // and convert from milliseconds to microseconds            
    iTimeInterval = flowSpeed * KMilliToMicro;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler::IsOpening
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppMessageFlowHandler::IsOpening() const
    {
    return iTimeInterval == 
            TTimeIntervalMicroSeconds32( KTimeIntervalOpen * KMilliToMicro );
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageFlowHandler: HandleChatListEvent
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageFlowHandler::HandleChatListEvent( TInt /*aServiceId*/, TChatListEventType aEvent, 
									MIMCVEngineMessageReadInterface* /*aContainerInfo*/
									) 
	{
		
	switch (aEvent)
		{
		case EChatItemAdded:
			{							
			break;
			}

		case EChatItemDeleted:
			{				
			iChatDeleted = ETrue;
			break;
			}

		default:
			break;
		}
	
	}
    
//  End of File
