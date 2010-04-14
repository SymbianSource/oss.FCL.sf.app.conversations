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
#include "uiservicetabtracer.h"
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
	TRACER_AUTO;
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::ConstructL( const TDesC& aContactDb, const TDesC& aServiceName)
    {
	TRACER_AUTO;
    iVPbkStoreHandler = CVIMPSTStorageVPbkStoreHandler::NewL( aContactDb,aServiceName, *this, EFalse );
    
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
	TRACER_AUTO;
    CVIMPSTStorageVPbkServerStore* self = NewLC( aContactDb, 
    											 aServiceName, 
    											 aServiceCacheWriter );
    CleanupStack::Pop( self );
   
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
	TRACER_AUTO;
    CVIMPSTStorageVPbkServerStore* self =
        new (ELeave) CVIMPSTStorageVPbkServerStore(aServiceCacheWriter);
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb,aServiceName );
    
    return self;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::
//      ~CVIMPSTStorageVPbkServerStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkServerStore::~CVIMPSTStorageVPbkServerStore()
    {
	TRACER_AUTO;
    iRetrivedXSPIdArray.ResetAndDestroy();
    TRAP_IGNORE( ResetAndDestroyLocalArrayL() );
    delete iVPbkStoreHandler;
    
    }

 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::ResetAndDestroyLocalArrayL()
    {
	TRACER_AUTO;
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
   
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::LocalStore
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkServerStore::LocalStore() const 
	{
	TRACER_AUTO;
	return EFalse;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent) 
    {
	TRACER_AUTO;
    switch( aVPbkStoreEvent.iEventType )
        {
        case EVPbkContactReadCompleted:
            {
            TRACE( "EVPbkContactReadCompleted started" );
            iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactReadComplete);
            if( iFetchStep == EFetchingCalled )
                {
                TRACE( "EVPbkContactReadCompleted fetcing in progress " );
                IssueRequest();		
                }
            TRACE( "iFetchStep set to state EContactReadComplete" );
            iFetchStep = EContactReadComplete;
            TRACE( " completed"  );
            break;
            }
        case EVPbkContactReading:
            {
            TRACE(" EVPbkContactReading started");
             // a contact entry has been created in virtual database store
            // create the cache contact
            // fetched one by one contact from cdb file
            // Add to storage cache
            
            iServiceCacheWriter.AddContactToCacheL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName ,
                    aVPbkStoreEvent.iAvatarContent,
                    TVIMPSTEnums::EStorageContactReading  );
            TRACE( "EVPbkContactReading completed"  );
		    break;	
		    }
        case EVPbkContactFetching:
        case EVPbkContactAdded:
            {
            TRACE( "EVPbkContactAdded or EVPbkContactFetching"  );
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
                TRACE( "EVPbkContactAdded or EVPbkContactFetching fetchin on"  );
                IssueRequest();
                }
            TRACE( " EVPbkContactAdded or EVPbkContactFetching completed"  );
            break;	
            }
        case EVPbkContactSynchronizing:
            {
            TRACE( "EVPbkContactSynchronizing "  );
            // a contact entry has been created in virtual database store
            // create the cache contact
            // fetched one by one contact from cdb file
            // Add to storage cache
            iServiceCacheWriter.AddContactToCacheL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName ,
                    aVPbkStoreEvent.iAvatarContent,
                    TVIMPSTEnums::EStorageContactSynchronizing );
            TRACE( "EVPbkContactSynchronizing(iFetchStep): %d " , iFetchStep );
            if(iFetchStep == EFetchingOn)          
                   {
                   TRACE( " EVPbkContactSynchronizing fetchin on"  );
                   IssueRequest();
                   }
            TRACE( " EVPbkContactSynchronizing completed"  );
            break;
            }
        case EVPbkContactDeleted:
            {
            TRACE( "EVPbkContactDeleted Event" );
            // a contact entry has been deleted from virtual database store
            // delete from cache contact
            // not Needed in case of server contact
            iServiceCacheWriter.RemoveContactFromCacheL(*aVPbkStoreEvent.iContactLink,
                    TVIMPSTEnums::EStorageEventContactDelete );
            TRACE( " EVPbkContactDeleted Event done" );
            break;	 
            }
        case EVPbkContactChanged:
            {
            TRACE( " EVPbkContactChanged started" );
            // a contact entry has been updated to virtual database store
            // update to cache contact
            // not Needed in case of server contact
            iServiceCacheWriter.UpdateCacheContactL(*aVPbkStoreEvent.iContactLink , 
                    aVPbkStoreEvent.iUserId ,
                    aVPbkStoreEvent.iDisplayName,
                    aVPbkStoreEvent.iAvatarContent);
            if(iFetchStep == EFetchingOn)         
                  {
                  TRACE(" EVPbkContactAdded or EVPbkContactFetching fetchin on"  );
                  IssueRequest();
                  }
            TRACE( " EVPbkContactChanged started" );
            break;	
            }
        case EVPbkContactRetriving:
            {
            TRACE( " EVPbkContactRetriving started " );
            // ONLY VALID USER ID ALLOWED
            if( aVPbkStoreEvent.iUserId.Length() )
                {
                TRACE( "EVPbkContactRetriving valid id " );
                HBufC* sxpId = aVPbkStoreEvent.iUserId.AllocL();
                iRetrivedXSPIdArray.Append( sxpId ); // takes the ownership of retrivedContact
                // keep the backup of retrieved contacts
                // user can add later once it sent to server
                }
            TRACE( " EVPbkContactRetriving done" );
            break;	
            }
        case EVPbkUnknownChanges:
            {
            TRACE( " EVPbkUnknownChanges Event in" );
            // unknow mean all contact deleted from store
            // Not supported in case of server contact
            iServiceCacheWriter.RemoveAllCacheContactsL();
            if(iFetchStep == EFetchingCalled )
                { 
                TRACE( " EVPbkUnknownChanges fetching on" );               
                IssueRequest();
                }
            TRACE( "EVPbkUnknownChanges Event done" );
            break;	
            }
        default:
            {
            TRACE( "default" );
            break;
            }
        }
   
    }


// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::IssueRequest()
    {
	TRACER_AUTO;
    if( !IsActive() )
        {
        TRequestStatus* status = &iStatus;
        TRACE( "RequestComplete");
        User::RequestComplete( status, KErrNone );
        SetActive();
        }
   
    }
    
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::RunL()
    {
	TRACER_AUTO;
    TInt count = iFetchContactsToBeAdded.Count();
    TRACE( " count = %d",count );
    if( !count )
        {
        TRACE(" set to state EContactReadComplete" );
        iFetchStep = EContactReadComplete;
        iServiceCacheWriter.NotifyServiceViewL( TVIMPSTEnums::EStorageContactFetchComplete );
        TRACE( "calling NotifyFetchCompleteL" );
        }
    else
        {
        TRACE( " set to state EFetchingOn" );
        iFetchStep  = EFetchingOn;
        TVIMPSTContactInfo contactInfoToAdd = iFetchContactsToBeAdded[0];
        // pass the display NAme in place of NULL
        TRACE( "calling CreateVPbkContactL" );
       	iVPbkStoreHandler->CreateVPbkContactL( *contactInfoToAdd.iUserId, *contactInfoToAdd.iDisplayName ); // process always 0th item	
	    
	    iFetchContactsToBeAdded.Remove( 0 );
        delete contactInfoToAdd.iUserId;
        contactInfoToAdd.iUserId = NULL;
        delete contactInfoToAdd.iDisplayName;
        contactInfoToAdd.iDisplayName = NULL ;
        
        iFetchContactsToBeAdded.Compress();
        }
   
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkServerStore::DoCancel()
    {
	TRACER_AUTO;
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::RunError( TInt aError )
    {
	TRACER_AUTO;
    TRACE( "error code = %d", aError );
   
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
	TRACER_AUTO;
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
	 TRACER_AUTO;
     TInt oldContactCount = RemoveAllVPbkContactsL();
     TRACE( " oldContactCount = %d", oldContactCount ); 
     ResetAndDestroyLocalArrayL();
     TInt fetchContactCount = aServiceField.Count();
     for( TInt i = 0; i<fetchContactCount; i++ )
         {
         TRACE( " Append = %d",i );
         TVIMPSTContactInfo contactInf =
							         {
							         aServiceField[i].AllocL() ,
							         aFirstNameList[i].AllocL()	
							         };
         iFetchContactsToBeAdded.Append( contactInf );
         }
     TRACE( "iFetchStep = %d", iFetchStep ); 
     if( iFetchStep == EContactReadComplete || !oldContactCount)
	     {
	     TRACE( "inside if" );
	     IssueRequest();	
	     }
	 else
		 {
         TRACE( " set to state EFetchingCalled" );
		 iFetchStep = EFetchingCalled;	
		 }
    
	 }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkServerStore::RemoveVPbkContactL(const MVPbkContactLink& aContactLink) 
    {
	TRACER_AUTO;;  
    // text now holds the name (or first field of the contact) 
    CVPbkContactLinkArray* contactsToDelete = CVPbkContactLinkArray::NewLC();
    MVPbkContactLink* link = aContactLink.CloneLC();    
    contactsToDelete->AppendL( link ); // takes ownership
    CleanupStack::Pop(); // link
    TInt error = iVPbkStoreHandler->RemoveVPbkContactL( *contactsToDelete );
    CleanupStack::PopAndDestroy(); // contactsToDelete
    TRACE( " error code = %d", error );
   
    return error;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveAllVPbkContactsL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::RemoveAllVPbkContactsL() 
	{
	TRACER_AUTO;
	iRetrivedXSPIdArray.ResetAndDestroy();
	return iVPbkStoreHandler->RemoveAllVPbkContactsL();
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkServerStore::RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks ) 
	{
	TRACER_AUTO;
	iRetrivedXSPIdArray.ResetAndDestroy();
	TRACE( " iRetrivedXSPIdArray Reset"  );
	iVPbkStoreHandler->RetrieveVPbkContactL( aPackedLinks );
	
    return iRetrivedXSPIdArray.Count();
	}
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::GetRetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageVPbkServerStore::GetRetrieveVPbkXSPIdL(TInt aIndex ) 
	{
	TRACER_AUTO;
	HBufC* contactId = NULL;
	if( aIndex >= 0 && aIndex < iRetrivedXSPIdArray.Count() )
        {
        contactId = iRetrivedXSPIdArray[ aIndex ];
        }
    if( contactId  )
	    {
		TRACE( "RetrieveVPbkXSPIdL() end");
	    return 	*contactId;
	    }
	
    return KNullDesC ; 
    }
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::CreateRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::CreateRetriveVPbkContactL( TInt /*aIndexToUse*/ )   
    {
	TRACER_AUTO;
    // Not applicable for server store
    return KErrNotSupported;
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::deleteRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkServerStore::deleteRetriveVPbkContactL( TInt /*aIndexToUse*/ )   
    {
	TRACER_AUTO;
    // Not applicable for server store
    return KErrNotSupported;
    }
// CVIMPSTStorageVPbkServerStore::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														  const TDesC8& aAvatartData )
    {
	TRACER_AUTO;
    return iVPbkStoreHandler->UpdateAvatarFieldDataL( aContactLink, aAvatartData );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkServerStore::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACER_AUTO;
	return iVPbkStoreHandler->RemoveAvatarFieldL( aStoreContact );	
	}

// ----------------------------------------------------------
// CVIMPSTStorageVPbkServerStore::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageVPbkServerStore::DeleteDatabaseL()
    {
	TRACER_AUTO;
   	// TODO : Not supported yet
   
    }
// End of file


