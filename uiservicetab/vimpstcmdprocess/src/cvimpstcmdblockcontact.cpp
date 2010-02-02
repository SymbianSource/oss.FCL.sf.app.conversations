/* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Execute command for blocking contact
*
*/

// INCLUDE FILES
#include "cvimpstcmdblockcontact.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"

#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "tvimpstconsts.h"
#include "mvimpstprocessarray.h"
// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::CVIMPSTCmdBlockContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdBlockContact::CVIMPSTCmdBlockContact( 
        const TInt aCommandId , const TDesC& aContactId,
        MVIMPSTProcessArray& aArrayProcess,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactId( aContactId ),
            iProcessArray( aArrayProcess ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::~CVIMPSTCmdBlockContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdBlockContact::~CVIMPSTCmdBlockContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdBlockContact* CVIMPSTCmdBlockContact::NewL( 
        const TInt aCommandId, const TDesC& aContactId,
        MVIMPSTProcessArray& aArrayProcess,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdBlockContact* self = new (ELeave ) CVIMPSTCmdBlockContact( aCommandId ,aContactId, aArrayProcess ,aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdBlockContact::ConstructL()
    {    
    iError = KErrNotFound;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdBlockContact::ExecuteLD()
    {
   	CHAT_DP_FUNC_ENTER("CVIMPSTCmdBlockContact::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );       
    iError = KErrGeneral;
    if(iContactId.Length())
        {
        //Get Presence SubService           
        MVIMPSTEngineSubService* subService =          
        (iEngine.SubService(TVIMPSTEnums::EPresence));
        if (subService && iEngine.IsBlockSupportedL())
            {
            MVIMPSTEnginePresenceSubService& presence = 
                MVIMPSTEnginePresenceSubService::Cast (*subService);
            iError = presence.AddToBlockListL(iContactId);
            CHAT_DP( D_CHAT_LIT(" -> After iError: %d" ), iError);
            }
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdBlockContact:: CommandFinished");
    if( iError == KErrNone )
        {
        iProcessArray.RemoveFromUnknonOrInvitationListL( iContactId ,ETrue );
        }
    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdBlockContact:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdBlockContact::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdBlockContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdBlockContact::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdBlockContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdBlockContact::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
