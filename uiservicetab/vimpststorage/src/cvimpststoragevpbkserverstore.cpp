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
* Description:  Class that provides handling of vpbk server store
*
*/



// INCLUDE FILES
#include "cvimpststoragevpbkserverstore.h"

#include "cvimpststoragevpbkstorehandler.h"

#include <MVPbkContactView.h>
#include <CVPbkContactIdConverter.h>
#include "vimpstdebugtrace.h" 
#include "tvimpstconsts.h"
#include "mvimpststorageserviceview.h"
#include <CVPbkContactLinkArray.h>
#include <MVPbkContactLink.h>
#include "cvimpststoragecontact.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::
//      CVIMPSTStorageVPbkServerStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkServerStore:: CVIMPSTStorageVPbkServerStore( MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter ):
    CActive( CActive::EPriorityIdle ),
    iServiceCacheWriter( aServiceCacheWriter ),
    iFetchStep( EFetchingUnknown )
    { 
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::CVIMPSTStorageVPbkServerStore()" )); 
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::ConstructL( const TDesC& aContactDb, const TDesC& aServiceName)
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::ConstructL() begin" ));
    iVPbkStoreHandler = CVIMPSTStorageVPbkStoreHandler::NewL( aContactDb,aServiceName, *this, EFalse );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::ConstructL() end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkServerStore* 
    CVIMPSTStorageVPbkServerStore::NewL( const TDesC& aContactDb,
    									 const TDesC& aServiceName,
    									MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::NewL() begin" ));
    CVIMPSTStorageVPbkServerStore* self = NewLC( aContactDb, 
    											 aServiceName, 
    											 aServiceCacheWriter );
    CleanupStack::Pop( self );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::NewL() end" ));
    return self;
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::NewLC
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkServerStore* 
    CVIMPSTStorageVPbkServerStore::NewLC(const TDesC& aContactDb,
    									const TDesC& aServiceName,
    									MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::NewLC() begin" ));
    CVIMPSTStorageVPbkServerStore* self =
        new (ELeave) CVIMPSTStorageVPbkServerStore(aServiceCacheWriter);
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb,aServiceName );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::NewLC() end" ));
    return self;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::
//      ~CVIMPSTStorageVPbkServerStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkServerStore::~CVIMPSTStorageVPbkServerStore()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::~CVIMPSTStorageVPbkServerStore() begin"));
    iRetrivedXSPIdArray.ResetAndDestroy();
    TRAP_IGNORE( ResetAndDestroyLocalArrayL() );
    delete iVPbkStoreHandler;
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::~CVIMPSTStorageVPbkServerStore() end"));
    }

 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL() begin") );
    TInt count = iFetchContactsToBeAdded.Count();
   	while( count )
	   	{
	   	TVIMPSTContactInfo contactInf = iFetchContactsToBeAdded[ count-1 ];
	   	iFetchContactsToBeAdded.Remove( count-1 );
		delete contactInf.iUserId;
		contactInf.iUserId = NULL;	
		delete contactInf.iDisplayName;
		contactInf.iDisplayName = NULL;				         
         // not exist need to add 
        iFetchContactsToBeAdded.Compress();	
        count = iFetchContactsToBeAdded.Count();
	   	}
	iFetchContactsToBeAdded.Reset();
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL() end") );
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::LocalStore
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkServerStore::LocalStore() const 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::LocalStore Function begin") );
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::LocalStore Function End") );
	return EFalse;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent) 
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL() begin") );
    switch( aVPbkStoreEvent.iEventType )
        {
        case EVPbkContactReadCompleted:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL() EVPbkContactReadCompleted started") );
            iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactReadComplete);
            if( iFetchStep == EFetchingCalled )
                {
                TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL() EVPbkContactReadCompleted fetcing in progress ") );
                IssueRequest();		
                }
            TRACE( T_LIT("iFetchStep set to state EContactReadComplete") );
            iFetchStep = EContactReadComplete;
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL completed")  );
            break;
            }
        case EVPbkContactReading:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactReading started") );
             // a contact entry has been created in virtual database store
            // create the cache contact
            // fetched one by one contact from cdb file
            // Add to storage cache
            
            iServiceCacheWriter.AddContactToCacheL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName ,
                    aVPbkStoreEvent.iAvatarContent,
                    TVIMPSTEnums::EStorageContactReading  );
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactReading completed" ) );
		    break;	
		    }
        case EVPbkContactFetching:
        case EVPbkContactAdded:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactAdded or EVPbkContactFetching" ) );
            TVIMPSTEnums::TVIMPSTStorgaeEventType event = TVIMPSTEnums::EStorageEventContactAddition;
            // a contact entry has been created in virtual database store
            // create the cache contact
            // fetched one by one contact from cdb file
            // Add to storage cache
            if( iFetchStep == EFetchingOn )
                {
                event = TVIMPSTEnums::EStorageContactFetching;
                } 
            iServiceCacheWriter.AddContactToCacheL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName ,
                    aVPbkStoreEvent.iAvatarContent,
                    event );
            if(iFetchStep == EFetchingOn)			
                {
                TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactAdded or EVPbkContactFetching fetchin on" ) );
                IssueRequest();
                }
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactAdded or EVPbkContactFetching completed" ) );
            break;	
            }
        case EVPbkContactSynchronizing:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactSynchronizing " ) );
            // a contact entry has been created in virtual database store
            // create the cache contact
            // fetched one by one contact from cdb file
            // Add to storage cache
            iServiceCacheWriter.AddContactToCacheL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName ,
                    aVPbkStoreEvent.iAvatarContent,
                    TVIMPSTEnums::EStorageContactSynchronizing );
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactSynchronizing(iFetchStep): %d " ), iFetchStep );
            if(iFetchStep == EFetchingOn)          
                   {
                   TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL  EVPbkContactSynchronizing fetchin on" ) );
                   IssueRequest();
                   }
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactSynchronizing completed" ) );
            break;
            }
        case EVPbkContactDeleted:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactDeleted Event") );
            // a contact entry has been deleted from virtual database store
            // delete from cache contact
            // not Needed in case of server contact
            iServiceCacheWriter.RemoveContactFromCacheL(*aVPbkStoreEvent.iContactLink,
                    TVIMPSTEnums::EStorageEventContactDelete );
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactDeleted Event done") );
            break;	 
            }
        case EVPbkContactChanged:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactChanged started") );
            // a contact entry has been updated to virtual database store
            // update to cache contact
            // not Needed in case of server contact
            iServiceCacheWriter.UpdateCacheContactL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName,
                    aVPbkStoreEvent.iAvatarContent);
            if(iFetchStep == EFetchingOn)         
                  {
                  TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactAdded or EVPbkContactFetching fetchin on" ) );
                  IssueRequest();
                  }
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactChanged started") );
            break;	
            }
        case EVPbkContactRetriving:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactRetriving started ") );
            // ONLY VALID USER ID ALLOWED
            if( aVPbkStoreEvent.iUserId.Length() )
                {
                TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactRetriving valid id ") );
                HBufC* sxpId = aVPbkStoreEvent.iUserId.AllocL();
                iRetrivedXSPIdArray.Append( sxpId ); // takes the ownership of retrivedContact
                // keep the backup of retrieved contacts
                // user can add later once it sent to server
                }
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactRetriving done") );
            break;	
            }
        case EVPbkUnknownChanges:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkUnknownChanges Event in") );
            // unknow mean all contact deleted from store
            // Not supported in case of server contact
            iServiceCacheWriter.RemoveAllCacheContactsL();
            if(iFetchStep == EFetchingCalled )
                { 
                TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkUnknownChanges fetching on") );               
                IssueRequest();
                }
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkUnknownChanges Event done") );
            break;	
            }
        default:
            {
            TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL default") );
            break;
            }
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL() End") );
    }


// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::IssueRequest()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::IssueRequest() begin") );
    if( !IsActive() )
        {
        TRequestStatus* status = &iStatus;
        TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::IssueRequest() RequestComplete"));
        User::RequestComplete( status, KErrNone );
        SetActive();
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::IssueRequest() end") );
    }
    
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::RunL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RunL() begin") );
    TInt count = iFetchContactsToBeAdded.Count();
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::FetchContactsToBeAdded count = %d"),count );
    if( !count )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::iFetchStep set to state EContactReadComplete") );
        iFetchStep = EContactReadComplete;
        iServiceCacheWriter.NotifyServiceViewL( TVIMPSTEnums::EStorageContactFetchComplete );
        TRACE( T_LIT("calling NotifyFetchCompleteL") );
        }
    else
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::iFetchStep set to state EFetchingOn") );
        iFetchStep  = EFetchingOn;
        TVIMPSTContactInfo contactInfoToAdd = iFetchContactsToBeAdded[0];
        // pass the display NAme in place of NULL
        TRACE( T_LIT("calling CreateVPbkContactL") );
       	iVPbkStoreHandler->CreateVPbkContactL( *contactInfoToAdd.iUserId, *contactInfoToAdd.iDisplayName ); // process always 0th item	
	    
	    iFetchContactsToBeAdded.Remove( 0 );
        delete contactInfoToAdd.iUserId;
        contactInfoToAdd.iUserId = NULL;
        delete contactInfoToAdd.iDisplayName;
        contactInfoToAdd.iDisplayName = NULL ;
        
        iFetchContactsToBeAdded.Compress();
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RunL() end") );
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::DoCancel()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::DoCancel begin") );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::DoCancel end") );
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::RunError( TInt aError )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RunError begin") );
    TRACE( T_LIT("error code = %d"), aError );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RunError end") );
    return aError;    
    }

//////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::CreateVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::CreateVPbkContactL( const TDesC& aUserId, 
        						 					   const TDesC& aDisplayName,
        						 					   TBool /*aInvitationAutoAccept = EFalse */ )   
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::CreateVPbkContactL() begin") );	
    return iVPbkStoreHandler->CreateVPbkContactL( aUserId ,aDisplayName );
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL
// -----------------------------------------------------------------------------
//
 void CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL( RArray <TPtrC>& aFirstNameList, 
                                                            RArray <TPtrC> &aServiceField ) 
     {
     // in case of server case we delete all contacts and then add fetch contacts
     // in case of server contacts delete all contacts
     TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL() begin") );  
     TInt oldContactCount = RemoveAllVPbkContactsL();
     TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL() oldContactCount = %d"), oldContactCount ); 
     ResetAndDestroyLocalArrayL();
     TInt fetchContactCount = aServiceField.Count();
     for( TInt i = 0; i<fetchContactCount; i++ )
         {
         TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::FetchContactsToBeAdded Append = %d"),i );
         TVIMPSTContactInfo contactInf =
							         {
							         aServiceField[i].AllocL() ,
							         aFirstNameList[i].AllocL()	
							         };
         iFetchContactsToBeAdded.Append( contactInf );
         }
     TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL() iFetchStep = %d"), iFetchStep ); 
     if( iFetchStep == EContactReadComplete || !oldContactCount)
	     {
	     TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL() inside if") );
	     IssueRequest();	
	     }
	 else
		 {
         TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::iFetchStep set to state EFetchingCalled") );
		 iFetchStep = EFetchingCalled;	
		 }
     TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::AddVPbkFetchContactsL() end") );  
	 }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL(const MVPbkContactLink& aContactLink) 
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL() begin") );  
    // text now holds the name (or first field of the contact) 
    CVPbkContactLinkArray* contactsToDelete = CVPbkContactLinkArray::NewLC();
    MVPbkContactLink* link = aContactLink.CloneLC();    
    contactsToDelete->AppendL( link ); // takes ownership
    CleanupStack::Pop(); // link
    TInt error = iVPbkStoreHandler->RemoveVPbkContactL( *contactsToDelete );
    CleanupStack::PopAndDestroy(); // contactsToDelete
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::returned error code = %d"), error );
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL() end") ); 
    return error;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveAllVPbkContactsL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::RemoveAllVPbkContactsL() 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RemoveAllVPbkContactsL() begin") );
	iRetrivedXSPIdArray.ResetAndDestroy();
	return iVPbkStoreHandler->RemoveAllVPbkContactsL();
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RetrieveVPbkContactL() begin") );
	iRetrivedXSPIdArray.ResetAndDestroy();
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL() iRetrivedXSPIdArray Reset" ) );
	iVPbkStoreHandler->RetrieveVPbkContactL( aPackedLinks );
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RetrieveVPbkContactL() end") );
    return iRetrivedXSPIdArray.Count();
	}
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::GetRetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageVPbkServerStore::GetRetrieveVPbkXSPIdL(TInt aIndex ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL() begin") );
	HBufC* contactId = NULL;
	if( aIndex >= 0 && aIndex < iRetrivedXSPIdArray.Count() )
        {
        contactId = iRetrivedXSPIdArray[ aIndex ];
        }
    if( contactId  )
	    {
		TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL() end") );
	    return 	*contactId;
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL() end") );
    return KNullDesC ; 
    }
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::CreateRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::CreateRetriveVPbkContactL( TInt /*aIndexToUse*/ )   
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::CreateRetriveVPbkContactL()") );
    // Not applicable for server store
    return KErrNotSupported;
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::deleteRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::deleteRetriveVPbkContactL( TInt /*aIndexToUse*/ )   
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::deleteRetriveVPbkContactL()") );
    // Not applicable for server store
    return KErrNotSupported;
    }
// CVIMPSTStorageVPbkServerStore::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														  const TDesC8& aAvatartData )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::UpdateAvatarFieldDataL() ") );
    return iVPbkStoreHandler->UpdateAvatarFieldDataL( aContactLink, aAvatartData );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::RemoveAvatarFieldL()") );
	return iVPbkStoreHandler->RemoveAvatarFieldL( aStoreContact );	
	}

// ----------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageVPbkServerStore::DeleteDatabaseL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::DeleteDatabaseL begin") );
   	// TODO : Not supported yet
    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::DeleteDatabaseL end") );
    }
// End of file


