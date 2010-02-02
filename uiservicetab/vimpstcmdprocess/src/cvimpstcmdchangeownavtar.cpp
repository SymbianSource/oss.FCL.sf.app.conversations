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
* Description:  command process for set own avtar
*
*/


// INCLUDE FILES
#include "cvimpstcmdchangeownavtar.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginesubservice.h"


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::CVIMPSTCmdChangeOwnAvtar
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnAvtar::CVIMPSTCmdChangeOwnAvtar( 
        const TInt aCommandId ,const TAvatarData& aData,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iData(aData),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::~CVIMPSTCmdChangeOwnAvtar
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnAvtar::~CVIMPSTCmdChangeOwnAvtar()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdChangeOwnAvtar* CVIMPSTCmdChangeOwnAvtar::NewL( 
        const TInt aCommandId,const TAvatarData& aData,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdChangeOwnAvtar* self = new (ELeave ) CVIMPSTCmdChangeOwnAvtar( aCommandId ,aData, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnAvtar::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnAvtar::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnAvtar::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );   
	
	//Get Presence SubService  			
   	MVIMPSTEngineSubService* subService =    	   
   	   			(iEngine.SubService(TVIMPSTEnums::EPresence));
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        		MVIMPSTEnginePresenceSubService::Cast (*subService);
        presence.PublishOwnPresenceL(iData.iStatus, iData.iStatusText, iData.iFileName , iData.iMimeType, ETrue );
        }      
  
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnAvtar:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdChangeOwnAvtar:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdChangeOwnAvtar::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdChangeOwnAvtar::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnAvtar::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdChangeOwnAvtar::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdChangeOwnAvtar::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
