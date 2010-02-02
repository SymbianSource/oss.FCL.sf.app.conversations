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
* Description:  command process for friend request
*
*/


// INCLUDE FILES
#include "cvimstcmdfriendrequest.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginesubservice.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::CVIMPSTCmdFriendRequest
// --------------------------------------------------------------------------
//
CVIMPSTCmdFriendRequest::CVIMPSTCmdFriendRequest( 
        const TInt aCommandId ,const TDesC& aContactId,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactId( aContactId ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::~CVIMPSTCmdFriendRequest
// --------------------------------------------------------------------------
//
CVIMPSTCmdFriendRequest::~CVIMPSTCmdFriendRequest()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdFriendRequest* CVIMPSTCmdFriendRequest::NewL( 
        const TInt aCommandId,const TDesC& aContactId,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdFriendRequest* self = new (ELeave ) CVIMPSTCmdFriendRequest( aCommandId ,aContactId, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFriendRequest::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFriendRequest::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdFriendRequest::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );     
    
    //Get Presence SubService 
    MVIMPSTEngineSubService* subService =    	   
   	   			(iEngine.SubService(TVIMPSTEnums::EPresence));
    
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        		MVIMPSTEnginePresenceSubService::Cast (*subService);
        presence.SendPresenceGrantPresentityL(iContactId, ( iCommandId == EFriendAccpeted  ));
        }
    
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdFriendRequest:: CommandFinished");

    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdFriendRequest:: CommandFinished");    
    CleanupStack::PopAndDestroy();  
    CHAT_DP_FUNC_DONE("CVIMPSTCmdFriendRequest::ExecuteLD");    
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFriendRequest::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdFriendRequest::CommandId() const
    {
    return iCommandId;  
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFriendRequest::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdFriendRequest::Result() const
    {
    //return valid data regd the command operation
    return iError;
    }

// End of File
