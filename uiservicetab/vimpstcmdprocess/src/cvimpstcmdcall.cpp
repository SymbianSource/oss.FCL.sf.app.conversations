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
* Description:  implementation for CVIMPSTCmdCall
*
*/


// INCLUDE FILES


#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "cvimpstcmdcall.h"
#include "mvimpstprocessarray.h"


#include <aiwdialdataext.h>
#include <aknpopup.h>
#include <aiwcommon.hrh>
#include <aiwservicehandler.h>
#include <MVPbkContactLink.h>



// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdCall::CVIMPSTCmdCall
// --------------------------------------------------------------------------
//
CVIMPSTCmdCall::CVIMPSTCmdCall( 
        const TInt aCommandId,TInt& aIndex, 
        MVIMPSTProcessArray& mArrayProcess,
        TInt aWindowGroup,
        TUint32 aServiceId) :            
            iCommandId( aCommandId ),
            iIndex(aIndex),
            iArrayProcess(mArrayProcess),
            iWindowGroup(aWindowGroup),
            iServiceId(aServiceId)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCall::~CVIMPSTCmdCall
// --------------------------------------------------------------------------
//
CVIMPSTCmdCall::~CVIMPSTCmdCall()
    {

    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCall::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdCall* CVIMPSTCmdCall::NewL( 
        const TInt aCommandId,TInt& aIndex,
        MVIMPSTProcessArray& aArrayProcess,
        TInt aWindowGroup,
        TUint32 aServiceId)
    {
    CVIMPSTCmdCall* self = new (ELeave ) CVIMPSTCmdCall( aCommandId,aIndex,
    					aArrayProcess, aWindowGroup,
    					aServiceId);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdCall::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCall::ConstructL()
    {
 
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCall::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCall::ExecuteLD()
    {
   	// CHAT_DP_FUNC_ENTER("CVIMPSTCallCmd::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );

	CAiwServiceHandler * aiwServiceHandler = CAiwServiceHandler::NewL();
	
	CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewL();
    CleanupStack::PushL( dialDataExt );
    
    dialDataExt->SetWindowGroup( iWindowGroup );
        
    HBufC8* pack = iArrayProcess.ContactLink(iIndex)->PackLC();
    
    dialDataExt->SetContactLinkL( *pack );
    
    dialDataExt->SetCallType( CAiwDialData::EAIWVoiP );
    
    //dialDataExt->SetServiceId( iServiceId );

    
    CAiwGenericParamList& paramList = 
        aiwServiceHandler->InParamListL();
    dialDataExt->FillInParamListL( paramList );
    
    aiwServiceHandler->ExecuteServiceCmdL(
            KAiwCmdCall,
            paramList,
            aiwServiceHandler->OutParamListL(),
            NULL);  // No need for callback    
    
    
    CleanupStack::PopAndDestroy( pack );
    CleanupStack::PopAndDestroy( dialDataExt );
    
    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	// CHAT_DP_FUNC_ENTER("CVIMPSTCallCmd:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	// CHAT_DP_FUNC_DONE("CVIMPSTCallCmd::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCall::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCall::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCall::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCall::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdCall::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCall::Result() const
	{
	//return valid data regd the command operation
	return KErrNone;
	}



// End of File
