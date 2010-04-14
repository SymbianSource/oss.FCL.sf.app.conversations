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
* Description:  execute command for fetching blocked list
*
*/

// INCLUDE FILES

//#include "cvimpstcmdlogout.h"

#include "vimpstcmd.hrh"
#include "cvimpstcmdfetchblockedlist.h"
#include "mvimpstcmdobserver.h"


#include "uiservicetabtracer.h"

#include <e32def.h>

#include "mvimpstengine.h"


// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::CVIMPSTCmdFetchBlockedContactList
// --------------------------------------------------------------------------
//
CVIMPSTCmdFetchBlockedContactList::CVIMPSTCmdFetchBlockedContactList( 
        const TInt aCommandId , MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::~CVIMPSTCmdFetchBlockedContactList
// --------------------------------------------------------------------------
//
CVIMPSTCmdFetchBlockedContactList::~CVIMPSTCmdFetchBlockedContactList()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdFetchBlockedContactList* CVIMPSTCmdFetchBlockedContactList::NewL( 
        const TInt aCommandId,MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdFetchBlockedContactList* self = new (ELeave ) CVIMPSTCmdFetchBlockedContactList( aCommandId , aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFetchBlockedContactList::ConstructL()
    {
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFetchBlockedContactList::ExecuteLD()
    {
   
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );    
    
    //call logout
   	iEngine.FetchBlockedListL();
   	
    

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	
	CleanupStack::PopAndDestroy();	
		
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFetchBlockedContactList::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdFetchBlockedContactList::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdFetchBlockedContactList::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdFetchBlockedContactList::Result() const
	{
	//return valid data regd the command operation
	return 0;
	}
	
	

// End of File
