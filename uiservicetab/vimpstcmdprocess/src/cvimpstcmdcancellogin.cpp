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
* Description:  cancel login command implementation 
*
*/


// INCLUDE FILES

#include "cvimpstcmdcancellogin.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"


// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::CVIMPSTCmdLogout
// --------------------------------------------------------------------------
//
CVIMPSTCmdCancelLogin::CVIMPSTCmdCancelLogin( const TInt aCommandId, 
        MVIMPSTEngine& aEngine) :     
        iCommandId( aCommandId ),
        iEngine(aEngine)
        {
        }

// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::~CVIMPSTCmdCancelLogin
// --------------------------------------------------------------------------
//
CVIMPSTCmdCancelLogin::~CVIMPSTCmdCancelLogin()
    {    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdCancelLogin* CVIMPSTCmdCancelLogin::NewL( 
        const TInt aCommandId,
        MVIMPSTEngine& aEngine)
    {
    // return the object created 
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdCancelLogin::NewL");
    return  new (ELeave ) CVIMPSTCmdCancelLogin( aCommandId , aEngine);
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCancelLogin::ExecuteLD()
    {
   	CHAT_DP_FUNC_ENTER("CVIMPSTCmdCancelLogin::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );    
    
    // now  call logout for cancel login
    // protocol adaptation has to take care of this 
    iEngine.LogoutL();
   	
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdCancelLogin:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdCancelLogin:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdCancelLogin::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCancelLogin::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCancelLogin::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdCancelLogin::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCancelLogin::Result() const
	{
	// return KErrNone since iEngine.Logout() doesnot return any error value
	return KErrNone;
	}

// End of File
