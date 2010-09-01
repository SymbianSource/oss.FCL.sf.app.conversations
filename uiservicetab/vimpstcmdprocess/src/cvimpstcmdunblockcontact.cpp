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
* Description:  execute command for unblocking contact
*
*/

// INCLUDE FILES
#include "cvimpstcmdunblockcontact.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"

#include "uiservicetabtracer.h"

#include <e32def.h>
#include "mvimpstengine.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"

#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "tvimpstconsts.h"
// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::CVIMPSTCmdDeleteContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdUnBlockContact::CVIMPSTCmdUnBlockContact( 
        const TInt aCommandId ,  const TDesC& aContactId,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactId( aContactId ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::~CVIMPSTCmdUnBlockContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdUnBlockContact::~CVIMPSTCmdUnBlockContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdUnBlockContact* CVIMPSTCmdUnBlockContact::NewL( 
        const TInt aCommandId, const TDesC& aContactId,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdUnBlockContact* self = new (ELeave ) CVIMPSTCmdUnBlockContact( aCommandId ,aContactId, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdUnBlockContact::ConstructL()
    {    
    iError = KErrNotFound;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdUnBlockContact::ExecuteLD()
    {
   
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );       
    if(iContactId.Length())
        {
        //Get Presence SubService           
        MVIMPSTEngineSubService* subService =          
        (iEngine.SubService(TVIMPSTEnums::EPresence));
        if (subService && iEngine.IsBlockSupportedL())
            {
            MVIMPSTEnginePresenceSubService& presence = 
            MVIMPSTEnginePresenceSubService::Cast (*subService);
            HBufC* userId = HBufC::NewLC(KPropertyMaxLength); //  2 onto cleanup stack
            TPtr userIdPtr = userId->Des();
            userIdPtr = iContactId;
            TRACE("userId: %s", &userIdPtr);            
            iError = presence.RemoveFromBlockListL(*userId);
            TRACE("iError: %d", iError);           
            CleanupStack::PopAndDestroy(userId);//2
          
            }
        }
   

    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }   
    CleanupStack::PopAndDestroy();  
  
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdUnBlockContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdUnBlockContact::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdUnBlockContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdUnBlockContact::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
