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
* Description: command process for login 
*
*/


// INCLUDE FILES
#include "cvimpstcmdlogin.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"

#include "uiservicetabtracer.h"

#include <e32def.h>
#include "mvimpstengine.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::CVIMPSTCmdLogin
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogin::CVIMPSTCmdLogin( 
        const TInt aCommandId ,TUint32 aServiceID,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iServiceId( aServiceID ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::~CVIMPSTCmdLogin
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogin::~CVIMPSTCmdLogin()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogin* CVIMPSTCmdLogin::NewL( 
        const TInt aCommandId,TUint32 aServiceID,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdLogin* self = new (ELeave ) CVIMPSTCmdLogin( aCommandId ,aServiceID, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogin::ConstructL()
    {    
   
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogin::ExecuteLD()
    {
   
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );   
	
	//call login
   	iEngine.Login();
   	
    

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	
	CleanupStack::PopAndDestroy();	
	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogin::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdLogin::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdLogin::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdLogin::Result() const
	{
	//return valid data regd the command operation
	//must be freed in Dtor
	return 0;
	}

// End of File
