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
* Description:  implementation for CVIMPSTCmdAddToPbk
*
*/


// INCLUDE FILES
#include "cvimpstcmdaddtopbk.h"
#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"

#include "uiservicetabtracer.h"

#include <e32def.h>
#include <MVPbkStoreContact.h>
#include "mvimpstengine.h"
#include "mvimpststorageserviceview.h"
#include "cvimpststoragemanagerfactory.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::CVIMPSTCmdAddToPbk
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddToPbk::CVIMPSTCmdAddToPbk( const TInt aCommandId ,
                                        const MVIMPSTStorageContact& aStoreContact,
                                        MVIMPSTEngine& aEngine) :            
                                        iCommandId( aCommandId ),
                                        iStoreContact( aStoreContact ),
                                        iEngine( aEngine )
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::~CVIMPSTCmdAddToPbk
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddToPbk::~CVIMPSTCmdAddToPbk()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddToPbk* CVIMPSTCmdAddToPbk::NewL( const TInt aCommandId,
                                              const MVIMPSTStorageContact& aStoreContact,
                                              MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdAddToPbk* self = new (ELeave ) CVIMPSTCmdAddToPbk( aCommandId, aStoreContact, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddToPbk::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddToPbk::ExecuteLD()
    {
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iEngine.ServiceId()) ;
    if(storage)
        {
        //storage->AddContactToPhoneBookL(iStoreContact);
        }
    else
        {
        iError = KErrGeneral;
        }
    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }
       
    CleanupStack::PopAndDestroy();  
    
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddToPbk::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddToPbk::CommandId() const
    {
    return iCommandId;  
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddToPbk::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddToPbk::Result() const
    {
    //return valid data regd the command operation
    return iError;
    }

// End of File

