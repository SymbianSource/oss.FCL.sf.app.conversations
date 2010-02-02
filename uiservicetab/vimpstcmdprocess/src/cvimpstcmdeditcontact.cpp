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
* Description:  Edit Contact for local store and server store
*
*/


// INCLUDE FILES
#include "cvimpstcmdeditcontact.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginecontactmgmtextention.h"
#include "mvimpstengineextentionfeatures.h"

#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::CVIMPSTCmdEditContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdEditContact::CVIMPSTCmdEditContact( 
        const TInt aCommandId ,MVPbkContactLink* aContactLink,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactLink( aContactLink ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::~CVIMPSTCmdEditContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdEditContact::~CVIMPSTCmdEditContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdEditContact* CVIMPSTCmdEditContact::NewL( 
        const TInt aCommandId,MVPbkContactLink* aContactLink,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdEditContact* self = new (ELeave ) CVIMPSTCmdEditContact( aCommandId ,aContactLink, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdEditContact::ConstructL()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdEditContact::ExecuteLD()
    {
   	CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );   
/*	
	MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::EContactManagement);
	if (feature)	
		{		
	    // to be implimented for serverstore editcontact
		iError = KErrNone;
		}	  	
   	
	MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iEngine.ServiceId()) ;
	if(storage)
		{
		storage->EditContactL(*iContactLink);	
		}
			
    CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd:: CommandFinished");
	*/	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTEnableServiceCmd::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdEditContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdEditContact::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdEditContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdEditContact::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
