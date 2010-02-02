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
* Description:  command process for change own message
*
*/


// INCLUDE FILES
#include "cvimpstcmdchangeownmessage.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::CVIMPSTCmdChangeOwnMessage
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnMessage::CVIMPSTCmdChangeOwnMessage( 
        const TInt aCommandId ,TStatusAndStatusText aStatus,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iStatus(aStatus.iStatus),
            iEngine(aEngine)
    {
    iStatusText.Copy(aStatus.iStatusText);
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::~CVIMPSTCmdChangeOwnMessage
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnMessage::~CVIMPSTCmdChangeOwnMessage()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnMessage* CVIMPSTCmdChangeOwnMessage::NewL( 
        const TInt aCommandId,TStatusAndStatusText aStatus,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdChangeOwnMessage* self = new (ELeave ) CVIMPSTCmdChangeOwnMessage( aCommandId ,aStatus, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnMessage::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnMessage::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnMessage::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );   
	
	//Get Presence SubService  	
   	MVIMPSTEngineSubService* subService =    	   	
   	   			(iEngine.SubService(TVIMPSTEnums::EPresence));
    
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        		MVIMPSTEnginePresenceSubService::Cast (*subService);
        presence.PublishOwnPresenceL(iStatus, iStatusText);
        }
   	
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnMessage:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnMessage:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdChangeOwnMessage::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnMessage::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnMessage::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnMessage::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnMessage::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
