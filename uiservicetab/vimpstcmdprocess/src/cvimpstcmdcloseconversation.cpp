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
* Description:  command process for close conversation
*
*/


// INCLUDE FILES
#include "cvimpstcmdcloseconversation.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstengineimsubservice.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::CVIMPSTCmdCloseConversation
// --------------------------------------------------------------------------
//
CVIMPSTCmdCloseConversation::CVIMPSTCmdCloseConversation( const TInt aCommandId,
                                                          const TDesC& aContactId,
                                                          MVIMPSTEngine& aEngine ):            
                                                          iCommandId( aCommandId ),
                                                          iContactId( aContactId ),
                                                          iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::~CVIMPSTCmdCloseConversation
// --------------------------------------------------------------------------
//
CVIMPSTCmdCloseConversation::~CVIMPSTCmdCloseConversation()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdCloseConversation* CVIMPSTCmdCloseConversation::NewL( const TInt aCommandId,
                                                                const TDesC& aContactId,
                                                                MVIMPSTEngine& aEngine )
    {
    CVIMPSTCmdCloseConversation* self = new (ELeave ) CVIMPSTCmdCloseConversation( aCommandId ,aContactId, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCloseConversation::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCloseConversation::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdCloseConversation::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );
    
    //Get IM SubService     
    MVIMPSTEngineSubService* subService1 =    	   
   	   			(iEngine.SubService(TVIMPSTEnums::EIM));
    
    if(subService1)
        {
        MVIMPSTEngineIMSubService& imSubService = 
        		MVIMPSTEngineIMSubService::Cast (*subService1);
        imSubService.CloseConversationL( iContactId );
        }
   
    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }
    
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdCloseConversation:: CommandFinished");    
    CleanupStack::PopAndDestroy();  
    CHAT_DP_FUNC_DONE("CVIMPSTCmdCloseConversation::ExecuteLD");    
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCloseConversation::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCloseConversation::CommandId() const
    {
    return iCommandId;  
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCloseConversation::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCloseConversation::Result() const
    {
    //return valid data regd the command operation
    return iError;
    }

// End of File
