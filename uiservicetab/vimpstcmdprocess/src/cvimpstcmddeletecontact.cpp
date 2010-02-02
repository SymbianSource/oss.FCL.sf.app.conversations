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
* Description:  command process for delete contact
*
*/


// INCLUDE FILES
#include "cvimpstcmddeletecontact.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstenginecontactmgmtextention.h"
#include "mvimpstengineextentionfeatures.h"
#include "mvimpstengineimsubservice.h"
#include "tvimpstenums.h"
#include "tvimpstconsts.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginepresencesubservice.h"

// Constants
_LIT( KListNameAllBuddy ,"buddylist" );
_LIT( KPresenceBlockedList, "blockedcontacts");


// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::CVIMPSTCmdDeleteContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdDeleteContact::CVIMPSTCmdDeleteContact( 
        const TInt aCommandId , MVPbkContactLink* aContactLink,
        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iContactLink( aContactLink ),
            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::~CVIMPSTCmdDeleteContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdDeleteContact::~CVIMPSTCmdDeleteContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdDeleteContact* CVIMPSTCmdDeleteContact::NewL( 
        const TInt aCommandId, MVPbkContactLink* aContactLink,
        MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdDeleteContact* self = new (ELeave ) CVIMPSTCmdDeleteContact( aCommandId ,aContactLink, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdDeleteContact::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdDeleteContact::ExecuteLD()
    {
   	CHAT_DP_FUNC_ENTER("CVIMPSTCmdDeleteContact::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );       
    iError = KErrNone;
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId()) ;         
	if(storage)
		{
		MVIMPSTStorageContact* contact = storage->FindContactByLink( *iContactLink );
		if (contact  )
			{
			TBuf<KUsernameMaxLength> listId;
			listId.Zero();
			listId.Append(KListNameAllBuddy);
			if(TVIMPSTEnums::EBlocked ==  contact->OnlineStatus())
				{
				listId.Zero();
				listId.Append(KPresenceBlockedList);
				}
			TPtrC userIdPtr = contact->UserId();
			MVIMPSTEngineSubService* subService = iEngine.SubService(TVIMPSTEnums::EPresence);
			MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::EContactManagement);
			//Get IM SubService     
			MVIMPSTEngineSubService* imService =  iEngine.SubService(TVIMPSTEnums::EIM) ;   
			//for each userid in the service details, unsubscribe 
			if( userIdPtr.Length() )
				{
				if( subService )
					{
					MVIMPSTEnginePresenceSubService& presenceSubService = MVIMPSTEnginePresenceSubService::Cast(*subService);
					presenceSubService.UnSubscribePresenceOfSingleContactL( userIdPtr );	
					}
				if (feature )	
					{
					MVIMPSTEngineContactMgmtExtention& contactMgr = MVIMPSTEngineContactMgmtExtention::Cast(*feature);
					TRAP(iError, contactMgr.DeleteServerContactL(listId, userIdPtr ) );
					}
				if(imService )
					{
					MVIMPSTEngineIMSubService& imSubService = 
												MVIMPSTEngineIMSubService::Cast (*imService);
					TRAP_IGNORE( imSubService.CloseConversationL( userIdPtr ));
					}		
				}
			if( iError == KErrNone )
				{
		 		storage->RemoveContactL( contact );	
       			}
			} // contact
		} // storage
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdDeleteContact:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdDeleteContact:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdDeleteContact::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdDeleteContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdDeleteContact::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdDeleteContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdDeleteContact::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
