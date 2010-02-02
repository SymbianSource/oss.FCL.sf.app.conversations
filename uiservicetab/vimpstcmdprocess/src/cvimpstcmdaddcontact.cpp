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
* Description:   VIMPST command for add contact
*
*/


// INCLUDE FILES
#include "cvimpstcmdaddcontact.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginecontactmgmtextention.h"
#include "mvimpstengineextentionfeatures.h"
#include <MVPbkStoreContact.h>
#include <MVPbkFieldType.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldUriData.h>

#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"
#include "mvimpststoragecontact.h"
#include "tvimpstenums.h" 
#include "tvimpstconsts.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"

// Constants
_LIT( KListNameAllBuddy ,"buddylist" );

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::CVIMPSTCmdAddContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddContact::CVIMPSTCmdAddContact( 
        const TInt aCommandId ,const TDesC& aContactId,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactId( aContactId ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::~CVIMPSTCmdAddContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddContact::~CVIMPSTCmdAddContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddContact* CVIMPSTCmdAddContact::NewL( 
        const TInt aCommandId,const TDesC& aContactId,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdAddContact* self = new (ELeave ) CVIMPSTCmdAddContact( aCommandId ,aContactId, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddContact::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddContact::ExecuteLD()
    {
   	CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iEngine.ServiceId()) ;
    if(storage )
        {
        if( !IsContactBlocked(iContactId) )
	        {
	        MVIMPSTStorageContact* alreadyExist = storage->FindContactByUserId( iContactId );
	        if( !alreadyExist )
				{
				iError = KErrNone;
				MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::EContactManagement);
				if (feature)	
					{
					MVIMPSTEngineContactMgmtExtention& contactMgr = 
											MVIMPSTEngineContactMgmtExtention::Cast (*feature);
					// if there is a scenario when the user enters only the firstname , then userid is null.
					//In this case don't send it to the server, Add the contact to the servicetab and 
					//presence will not be visible.
					// check if contact is in the blocked list.
					// iError already exist
					if( iContactId.Length() )
						{
						iError = contactMgr.AddServerContactL(KListNameAllBuddy,iContactId, KNullDesC() );
						}//End if(iContactId)
					}	//End if(feature)
				if(KErrNone == iError)
					{
					storage->CreateNewContactL( iContactId );	
					}
				}//End if(alreadyExist)
		    else	
	            {
	            iError = EErrExistInContactList;		
	            }
	        }
        else
	        {
	        iError = EErrExistInBlockList;		
	        }
        }//End  if(storage)
   			
    CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTEnableServiceCmd:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTEnableServiceCmd::ExecuteLD");	
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::IsContactBlocked
// --------------------------------------------------------------------------
TBool CVIMPSTCmdAddContact::IsContactBlocked(const TDesC& aUserId)
	{
   	MVIMPSTEngineSubService* subService =    	   
   	   					(iEngine.SubService(TVIMPSTEnums::EPresence));
    if(subService)
        {
        MVIMPSTEnginePresenceSubService& presence = 
        		MVIMPSTEnginePresenceSubService::Cast (*subService);
        		
        RPointerArray<HBufC> *blockedlist = presence .GetBlockedList();
        
        if(blockedlist)
	        {
	        for(TInt i=0; i<blockedlist->Count(); ++i)
				{
				if(0 == aUserId.CompareC((*blockedlist)[i]->Des()))
					{
					return ETrue;
					}
				}
	        }
        }
    return EFalse;  
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddContact::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdAddContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddContact::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
