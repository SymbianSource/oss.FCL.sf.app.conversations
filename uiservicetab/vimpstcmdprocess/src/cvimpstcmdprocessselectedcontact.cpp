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
* Description:  command process for selected contact
*
*/


//include files
#include "cvimpstcmdprocessselectedcontact.h"

#include <MVPbkStoreContact.h>
#include <MVPbkFieldType.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldUriData.h>

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"

#include "uiservicetabtracer.h"

#include <e32def.h>
#include <MVPbkContactLinkArray.h>
#include "mvimpstengine.h"

#include "mvimpststorageserviceview.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpstenginecontactmgmtextention.h"
#include "tvimpstconsts.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"

// Constants
_LIT( KListNameAllBuddy ,"buddylist" );

// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::CVIMPSTCmdProcessSelectedContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdProcessSelectedContact::CVIMPSTCmdProcessSelectedContact( const TInt aCommandId ,
                                                                    TVPbkSelectedData& aSelectedEntries,
                                                                    MVIMPSTEngine& aEngine ):            
                                                                    iCommandId( aCommandId ),
                                                                    iSelectedEntries( aSelectedEntries ),
                                                                    iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::~CVIMPSTCmdProcessSelectedContact
// --------------------------------------------------------------------------
//
CVIMPSTCmdProcessSelectedContact::~CVIMPSTCmdProcessSelectedContact()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdProcessSelectedContact* CVIMPSTCmdProcessSelectedContact::NewL( const TInt aCommandId,
                                                                          TVPbkSelectedData& aSelectedEntries,
                                                                          MVIMPSTEngine& aEngine )
    {
    CVIMPSTCmdProcessSelectedContact* self = new (ELeave ) CVIMPSTCmdProcessSelectedContact( aCommandId ,aSelectedEntries, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdProcessSelectedContact::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdProcessSelectedContact::ExecuteLD()
    {
    
    TRACER_AUTO;
    //push to the cleanupstack
    CleanupStack::PushL( this );
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId() ) ;
    if(storage)
        {
        TBool localStore = storage->IsLocalStore();
        //retrieve contact from links  
        TPtrC8 pacledLinks = iSelectedEntries.iPackedLinks->Des();
        TInt  count = storage->RetriveLinkXSPIdsL( pacledLinks  );
        if( !count )
            {
            iError = KErrGeneral;	
            }
	    if( (iCommandId == ECmdProcessSelectedContactNew  ||
	            iCommandId == ECmdProcessSelectedContactExist )&& 
	            iError == KErrNone )
	        {
	        MVIMPSTEngineExtentionFeatures* contactFeature = iEngine.ExtentionFeatures(TVIMPSTEnums::EContactManagement);
	        MVIMPSTEngineSubService* presenceService = iEngine.SubService(TVIMPSTEnums::EPresence);
            for(TInt index = 0; index < count; index++ )
	            {
	            TPtrC xspIdPtr = storage->GetRetrieveXSPIdL( index ); // xspId is owned here
	            if( localStore )
					{
					iError = KErrNone;
					if( xspIdPtr.Length() )
						{
						if( IsContactBlocked( xspIdPtr ) )
							{
							iError = EErrExistInBlockList;	
							}
						else if( storage->FindContactByUserId( xspIdPtr ) )
							{
							iError = EErrExistInContactList;	
							}
						else if( contactFeature  )
							{
							// add to server and store if sucess
							MVIMPSTEngineContactMgmtExtention& contactMgr = 
											MVIMPSTEngineContactMgmtExtention::Cast (*contactFeature);
							iError = contactMgr.AddServerContactL( KListNameAllBuddy, xspIdPtr ,KNullDesC );			            
							}
						}
					if(KErrNone == iError )
						{
						iError = storage->CreateNewContactFromRetrivedIdL( index );
						}
					else if( iCommandId == ECmdProcessSelectedContactNew  )
					    {
					    // delete the contact which got created in phonebook but failed to add in servicetab
					    storage->DeleteNewContactFromRetrivedIdL( index );
					    }
					}
		         else if( xspIdPtr.Length() )
					{
					// xspId ownership is transfered to iXSPList
					iSelectedEntries.iXSPList.AppendL( xspIdPtr.AllocL() );
					}
	           	}
	        }
       else if( ( iCommandId == ECmdProcessAcceptedContactNew || 
               iCommandId == ECmdProcessAcceptedContactExist )&& 
               iError == KErrNone )// for accepted contact 
	       {
	       // just add and return
	       // contact is already added in storage in RetrieveContactL
	       // if sucess ,storage will take ownership else we need to delete contact
	       iError = storage->CreateNewContactFromRetrivedIdL( 0 ); // for accepting invitation take the first contact which is retirved
	       if( iError != KErrNone && iCommandId == ECmdProcessAcceptedContactNew )
	           {
	           // delete the contact which got created in phonebook but failed to add in servicetab
	           storage->DeleteNewContactFromRetrivedIdL( 0 );
	           }
	       }
	  if( !localStore )
		  {
		  // in case of server we have a selection dialog 
		  // so this error does not matter 
		  // set to 0
		  iError = KErrNone;	
		  }
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
// CVIMPSTCmdProcessSelectedContact::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdProcessSelectedContact::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdProcessSelectedContact::CommandId() const
    {
    return iCommandId;  
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdProcessSelectedContact::Result() const
    {
    //return valid data regd the command operation
    return iError;
    }
 
 // --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::Result
// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// CVIMPSTCmdProcessSelectedContact::IsContactBlocked
// --------------------------------------------------------------------------
TBool CVIMPSTCmdProcessSelectedContact::IsContactBlocked(const TDesC& aUserId)
	{
	TRACER_AUTO;
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
// End of File
