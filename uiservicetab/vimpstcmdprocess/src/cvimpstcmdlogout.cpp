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
* Description:  command process for logout
*
*/


// INCLUDE FILES

#include "cvimpstcmdlogout.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"

#include "uiservicetabtracer.h"

#include <e32def.h>

#include "mvimpstengine.h"


// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::CVIMPSTCmdLogout
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogout::CVIMPSTCmdLogout( 
        const TInt aCommandId ,TUint32 aServiceID,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iServiceId( aServiceID ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::~CVIMPSTCmdLogout
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogout::~CVIMPSTCmdLogout()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdLogout* CVIMPSTCmdLogout::NewL( 
        const TInt aCommandId,TUint32 aServiceID,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdLogout* self = new (ELeave ) CVIMPSTCmdLogout( aCommandId ,aServiceID, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogout::ConstructL()
    {
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogout::ExecuteLD()
    {
   
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );    
    
    //call logout
   	iEngine.LogoutL();
   	
    

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	
	CleanupStack::PopAndDestroy();	
	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdLogout::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdLogout::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdLogout::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdLogout::Result() const
	{
	//return valid data regd the command operation
	return 0;
	}

// End of File
