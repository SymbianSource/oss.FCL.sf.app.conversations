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
* Description:  command process for change status
*
*/


// INCLUDE FILES
#include "cvimpstcmdchangeownstatus.h"
#include "mvimpstenginepresencesubservice.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginesubservice.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::CVIMPSTCmdChangeOwnStatus
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnStatus::CVIMPSTCmdChangeOwnStatus( 
        const TInt aCommandId ,TStatusAndStatusText aStatus,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iStatus(aStatus.iStatus),
            iEngine(aEngine)
    {
    iStatusText.Copy(aStatus.iStatusText);
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::~CVIMPSTCmdChangeOwnStatus
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnStatus::~CVIMPSTCmdChangeOwnStatus()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnStatus* CVIMPSTCmdChangeOwnStatus::NewL( 
        const TInt aCommandId,TStatusAndStatusText aStatus,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdChangeOwnStatus* self = new (ELeave ) CVIMPSTCmdChangeOwnStatus( aCommandId ,aStatus, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnStatus::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnStatus::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnStatus::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );   
	
	//Get Presence SubService  			
   	MVIMPSTEngineSubService* subService =    	   
   	   			(iEngine.SubService(TVIMPSTEnums::EPresence));
    
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        		MVIMPSTEnginePresenceSubService::Cast (*subService);
                        
   	    if ( iStatus == TVIMPSTEnums::EInvisible)
   	        {
			// Status set to Appear Offline
   	        iError = presence.PublishOwnPresenceL(iStatus, KNullDesC);
   	        }
   	    else
   	        {
   	        iError = presence.PublishOwnPresenceL(iStatus, iStatusText);
   	        }
   	    }       
   	
    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }
        
	
    
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnStatus:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdChangeOwnStatus::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnStatus::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnStatus::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnStatus::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnStatus::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
