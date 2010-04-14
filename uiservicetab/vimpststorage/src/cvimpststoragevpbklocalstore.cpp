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
* Description:  Class that provides handling of vpbk local store
*
*/

// INCLUDE FILES
#include <AknUtils.h> 
#include "cvimpststoragevpbklocalstore.h"

#include "cvimpststoragevpbkstorehandler.h"

#include <MVPbkContactView.h>
#include <CVPbkContactIdConverter.h>
#include "mvimpststorageserviceview.h"
#include "uiservicetabtracer.h"
#include "tvimpstconsts.h"
#include <CVPbkContactLinkArray.h>
#include <MVPbkContactLink.h>
#include "cvimpststoragecontact.h"
#include "vimpststorageutils.h"

#include <f32file.h>
#include <sysutil.h>


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::
//      CVIMPSTStorageVPbkLocalStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkLocalStore:: CVIMPSTStorageVPbkLocalStore( MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter ):
	CActive( CActive::EPriorityIdle ),
    iServiceCacheWriter( aServiceCacheWriter ),
    iFetchStep( EFetchingUnknown )
    { 
	TRACER_AUTO;
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkLocalStore* CVIMPSTStorageVPbkLocalStore::NewL( const TDesC& aContactDb,
    															 const TDesC& aServiceName,
    															 MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter )
    {
	TRACER_AUTO;
    CVIMPSTStorageVPbkLocalStore* self = NewLC( aContactDb,  
    										   aServiceName,
											  aServiceCacheWriter);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::NewLC
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkLocalStore* 
    CVIMPSTStorageVPbkLocalStore::NewLC( const TDesC& aContactDb, 
    									const TDesC& aServiceName,
    									MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter  )
    {
	TRACER_AUTO;
    
    CVIMPSTStorageVPbkLocalStore* self =
        new (ELeave) CVIMPSTStorageVPbkLocalStore(aServiceCacheWriter);
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb, aServiceName );
   
    return self;
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ConstructL( const TDesC& aContactDb, const TDesC& aServiceName )
    {
	TRACER_AUTO;
   
    iVPbkStoreHandler = CVIMPSTStorageVPbkStoreHandler::NewL( aContactDb ,aServiceName, *this,ETrue );
    TRACE( "iVPbkStoreHandler created" );
     //construct the persistent store name
    iLocalDBName = HBufC::NewL( aServiceName.Length()+ KStorageExtn().Length());
    TPtr iLocalDBNamePtr = iLocalDBName->Des();
    iLocalDBNamePtr.Append(aServiceName);
    _LIT(KSpecialChar ,"/\\:*?<>\"");
    AknTextUtils :: StripCharacters(iLocalDBNamePtr,KSpecialChar); 
    iLocalDBNamePtr.Append(KStorageExtn());    
    TRACE( "iLocalDBNamePtr: %S", &iLocalDBNamePtr );
    MVPbkContactStore* defaultStore = iVPbkStoreHandler->GetDefaultStoreL( aContactDb );
    User::LeaveIfNull(defaultStore);  
    TRACE("defaultStore retrived" );
    iIdConverter = CVPbkContactIdConverter::NewL(*defaultStore);
    TRACE( "iIdConverter created" ); 
    User::LeaveIfError( iFs.Connect() );
    // Open existing or create new database.
    TRACE( "connected to file server" ); 
	if ( DbExists() )
		{
		TRACE( "local db exist" ); 
        OpenDbL();
        TRACE( "open success" );
		}
	else
		{
		TRACE("local db not exist" );
        DoFreespaceLevelCheckL( KEmptyDbSize );
        CreateDbL();
        TRACE("local db created");
		}
    OpenTableL();
    TRACE("OpenTableL success " );
    iColset = iDb.ColSetL( KContactTable );    
   	TRACE( " iColset is set" );
  
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::
//      ~CVIMPSTStorageVPbkLocalStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkLocalStore::~CVIMPSTStorageVPbkLocalStore()
    {
	TRACER_AUTO;
    
    iRetrivedContactArray.ResetAndDestroy();
    TRAP_IGNORE( ResetAndDestroyLocalArrayL() );
        
    CloseTable();
    delete iColset;
    CloseDb();
    iFs.Close();    
    
    delete iLocalDBName;
    delete iIdConverter;	        
    delete iInviteId;
    delete iVPbkStoreHandler;
   
    }
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::
//      ResetAndDestroyLocalArrayL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ResetAndDestroyLocalArrayL()
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
// CVIMPSTStorageVPbkLocalStore::LocalStore
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::LocalStore() const 
	{
	TRACER_AUTO;
	return ETrue;
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent) 
	{
	TRACER_AUTO;
	switch( aVPbkStoreEvent.iEventType )
	    {
	    case EVPbkContactReadCompleted:
	    	{
	    	TRACE( " EVPbkContactReadCompleted started" );
	    	iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactReadComplete);
	    	if( iFetchStep == EFetchingCalled )
		    	{
		    	TRACE( " EVPbkContactReadCompleted EFetchingCalled " );
		    	IssueRequest();		
		    	}
	    	iFetchStep = EContactReadComplete;
	    	TRACE( "EContactReadComplete completed " );
		  	break;
	    	}
	    case EVPbkContactReading:
		    {
		    // a contact entry has been created in virtual database store
			// create the cache contact
			TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
			TDbColNo colNo = iColset->ColNo( KContactId );
		    if ( SeekRowL( colNo, linkId ) )
				{
				TRACE( "EVPbkContactReading contact" );
				/*
					*	This is local store, that is phone book and service tab shares only one cdb file.
					* 	in that case, if somebody edits the cdb file from contacts tab, then also we get the 
					* 	callback, then checking for entry in db table makes sens.. 
				*/
				/*
				 * 	in case when the user adds a contact from the service tab, then it first tries to add it in the
				 *	cdb file, after that we get this callback, then , there is no way that it will be present in the
				 *	db table, in that case, we will not add it in the cache - which is wrong.. 
				 *	so we need to make a differentiation, on how the contact was added.. whether from cotnacts tab
				 *	or from the service tab. --- to solve this issue.. we can store the userid in memberVariable.. 
				 *	before making a add request to the VPBK (cdb file).. then in this function, we check if
				 *	the contactid is same as the member variable.  aVPbkStoreEvent contains the userid that was added.
				 *	then if it matches, then we know that it was added from our service tab.. and then we can add it in our DB.
				 */
				// contatc found in local database ,add to storage
				iServiceCacheWriter.AddContactToCacheL (*aVPbkStoreEvent.iContactLink , 
										 aVPbkStoreEvent.iUserId ,
										 aVPbkStoreEvent.iDisplayName,
										 aVPbkStoreEvent.iAvatarContent,
										 TVIMPSTEnums::EStorageContactReading );
				}
			
		    break;	
		    }
		case EVPbkContactFetching:
	   	case EVPbkContactAdded:
			{
			// a contact entry has been created in virtual database store
			// create the cache contact
			TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
			TDbColNo colNo = iColset->ColNo( KContactId );
			if( iFetchStep  == EFetchingOn )
		         {
		         TRACE( "EVPbkContactFetching contact" );
		         WriteToStoreDbL( linkId );
		         iServiceCacheWriter.AddContactToCacheL (*aVPbkStoreEvent.iContactLink , 
		                 aVPbkStoreEvent.iUserId ,
		                 aVPbkStoreEvent.iDisplayName ,
		                 aVPbkStoreEvent.iAvatarContent,
		                 TVIMPSTEnums::EStorageContactFetching );
		      	IssueRequest();		
				}
			else if ( SeekRowL( colNo, linkId ) )
				{
				TRACE( " EVPbkContactAdded start" );
				/*
					*	This is local store, that is phone book and service tab shares only one cdb file.
					* 	in that case, if somebody edits the cdb file from contacts tab, then also we get the 
					* 	callback, then checking for entry in db table makes sens.. 
				*/
				/*
				 * 	in case when the user adds a contact from the service tab, then it first tries to add it in the
				 *	cdb file, after that we get this callback, then , there is no way that it will be present in the
				 *	db table, in that case, we will not add it in the cache - which is wrong.. 
				 *	so we need to make a differentiation, on how the contact was added.. whether from cotnacts tab
				 *	or from the service tab. --- to solve this issue.. we can store the userid in memberVariable.. 
				 *	before making a add request to the VPBK (cdb file).. then in this function, we check if
				 *	the contactid is same as the member variable.  aVPbkStoreEvent contains the userid that was added.
				 *	then if it matches, then we know that it was added from our service tab.. and then we can add it in our DB.
				 */
				// contatc found in local database ,add to storage
				iServiceCacheWriter.AddContactToCacheL (*aVPbkStoreEvent.iContactLink , 
										 aVPbkStoreEvent.iUserId ,
										 aVPbkStoreEvent.iDisplayName,
										 aVPbkStoreEvent.iAvatarContent,
										 TVIMPSTEnums::EStorageEventContactAddition  );
				TRACE( "EVPbkContactAdded completed" );
				}
			else if(  iInviteId )
			    {
			    TRACE( "iInviteId accepted" );
			    if( IsSameContactIdL( *iInviteId, aVPbkStoreEvent.iUserId ) )
			        {
			        WriteToStoreDbL( linkId );
			        iServiceCacheWriter.AddContactToCacheL (*aVPbkStoreEvent.iContactLink , 
			                aVPbkStoreEvent.iUserId ,
			                aVPbkStoreEvent.iDisplayName,
			                aVPbkStoreEvent.iAvatarContent,
			                TVIMPSTEnums::EStorageEventContactAddition );
			        delete iInviteId;
			        iInviteId = NULL;
			        }
                 TRACE( "iInviteId accepted sucess" );
			    }
			break;	
			}
	   	case EVPbkContactSynchronizing:
	   	    {
	   	    TRACE( "EVPbkContactSynchronizing "  );
	   	    // a contact entry has been created in virtual database store
	   	    // create the cache contact
	   	    TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
	   	    TDbColNo colNo = iColset->ColNo( KContactId );
	   	    if ( SeekRowL( colNo, linkId ) )
	   	        {
	   	        /*
	   	         *   This is local store, that is phone book and service tab shares only one cdb file.
	   	         *   in that case, if somebody edits the cdb file from contacts tab, then also we get the 
	   	         *   callback, then checking for entry in db table makes sens.. 
	   	         */
	   	        /*
	   	         *  in case when the user adds a contact from the service tab, then it first tries to add it in the
	   	         *  cdb file, after that we get this callback, then , there is no way that it will be present in the
	   	         *  db table, in that case, we will not add it in the cache - which is wrong.. 
	   	         *  so we need to make a differentiation, on how the contact was added.. whether from cotnacts tab
	   	         *  or from the service tab. --- to solve this issue.. we can store the userid in memberVariable.. 
	   	         *  before making a add request to the VPBK (cdb file).. then in this function, we check if
	   	         *  the contactid is same as the member variable.  aVPbkStoreEvent contains the userid that was added.
	   	         *  then if it matches, then we know that it was added from our service tab.. and then we can add it in our DB.
	   	         */
	   	        // contatc found in local database ,add to storage
	   	        iServiceCacheWriter.AddContactToCacheL (*aVPbkStoreEvent.iContactLink , 
	   	                aVPbkStoreEvent.iUserId ,
	   	                aVPbkStoreEvent.iDisplayName,
	   	                aVPbkStoreEvent.iAvatarContent,
	   	                TVIMPSTEnums::EStorageContactFetching  );
	   	        }
	   	   if(iFetchStep == EFetchingOn)          
               {
               TRACE( "EVPbkContactSynchronizing fetchin on" );
               IssueRequest();
               }
	   	    TRACE(" EVPbkContactSynchronizing completed"  );
	   	    break;
	   	    }
		case EVPbkContactDeleted:
		    {
		    TRACE( "EVPbkContactDeleted" );
		    TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
		    // a contact entry has been deleted from virtual database store
		    TDbColNo colNo = iColset->ColNo( KContactId );
		    if ( SeekRowL( colNo, linkId ) )
		        { 
		        TRACE( "EVPbkContactDeleted found in db" );
		        // contatc found in local database ,delete from local database 
		        DeleteContactFromInternalStoreL( linkId );
		        TRACE( "EVPbkContactDeleted deleted from db" );
		         // delete from cache contact
		    	iServiceCacheWriter.RemoveContactFromCacheL(*aVPbkStoreEvent.iContactLink, 
		    												TVIMPSTEnums::EStorageEventDeleteFromPbk );
		    	TRACE( "EVPbkContactDeleted deleted from cache" );
		        }
		    break;	 
		    }
		case EVPbkContactChanged:
		    {
		    TRACE( "EVPbkContactChanged ");
		    TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
		    TDbColNo colNo = iColset->ColNo( KContactId );
		    if ( SeekRowL( colNo, linkId ) )
		       {
		       TRACE( "EVPbkContactChanged found in local db" );
		       // found in the list update it
		       iServiceCacheWriter.UpdateCacheContactL(*aVPbkStoreEvent.iContactLink , 
					    						 	 aVPbkStoreEvent.iUserId ,
					    							 aVPbkStoreEvent.iDisplayName,
					    							 aVPbkStoreEvent.iAvatarContent );
			   TRACE( "EVPbkContactChanged changed in cache");
		       }
		    break;	
		    }
	    case EVPbkContactRetriving:
		    {
		    TRACE( "EVPbkContactRetriving");
		    // in case of local we allow all 
		    TRACE( "EVPbkContactRetriving valid Id");
			CVIMPSTStorageContact* retrivedContact = CVIMPSTStorageContact::NewL( aVPbkStoreEvent.iUserId ,
											    								  aVPbkStoreEvent.iDisplayName,
											    								  *aVPbkStoreEvent.iContactLink,
											    								  aVPbkStoreEvent.iAvatarContent);
			iRetrivedContactArray.Append( retrivedContact ); // takes the ownership of retrivedContact
			TRACE( "EVPbkContactRetriving valid Id added" );
			// keep the backup of retrieved contacts
			// user can add later once it sent to server
			break;	
		    }
		case EVPbkUnknownChanges:
		    {
		    TRACE("EVPbkUnknownChanges start" );
		    DeleteAllContactFromInternalStoreL();
		    TRACE( " EVPbkUnknownChanges all delete from db" );
		     // unknow mean all contact deleted from store
		    iServiceCacheWriter.RemoveAllCacheContactsL();
		    TRACE( " EVPbkUnknownChanges all delete from cache" );
		    break;	
		    }
	    default:
		 	break;
	    }
	
	}


// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::IssueRequest()
    {
	TRACER_AUTO;
    if( !IsActive() )
	    {
	    TRACE( "SetActive" );
	    TRequestStatus* status = &iStatus;
	    User::RequestComplete( status, KErrNone );
	    SetActive();
	    }
	
    }
    
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::RunL()
    {
	TRACER_AUTO;
    TInt count = iFetchContactsToBeAdded.Count();
    TRACE( "count to be added = %d ",count );
    if( !count )
        {
        TRACE(" fetch completed "  );
        iFetchStep = EContactReadComplete;
        iServiceCacheWriter.NotifyServiceViewL( TVIMPSTEnums::EStorageContactFetchComplete );
		TRACE( "fetch completed notification sent"  );     
	    }
    else
        {
        TRACE( "fetch in progress"  );     
        iFetchStep  = EFetchingOn;
        TVIMPSTContactInfo contactInfoToAdd = iFetchContactsToBeAdded[0];
        MVIMPSTStorageContact* exist = iServiceCacheWriter.FindCacheContactByUserId( *contactInfoToAdd.iUserId );
        if( exist )
	        {
	        // contact is already exist ,send the notification about this
	        iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactFetchExistInStore, exist );
	        TRACE( " fetch in progress id exist" ); 
	        // process next
	        IssueRequest();
	        }
	    else
		    {
		    TRACE( "fetch in progress create id in store called"  ); 
		     // pass the display NAme in place of NULL
        	iVPbkStoreHandler->CreateVPbkContactL( *contactInfoToAdd.iUserId, *contactInfoToAdd.iDisplayName ); // process always 0th item
            }
        iFetchContactsToBeAdded.Remove( 0 );
        delete contactInfoToAdd.iUserId;
        contactInfoToAdd.iUserId = NULL;
        delete contactInfoToAdd.iDisplayName;
        contactInfoToAdd.iDisplayName = NULL ;
        iFetchContactsToBeAdded.Compress();
        TRACE( "fetch in progress iFetchContactsToBeAdded ,one entry removed"  ); 
        }
    
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DoCancel()
    {
	TRACER_AUTO;
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::RunError( TInt aError )
    { 
	TRACER_AUTO;  
    return aError;
    }

//////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL(const TDesC& aUserId, 
        										   	  const TDesC& aDisplayName ,
        										   	  TBool aInvitationAutoAccept /* = EFalse */ )   
	{
	TRACER_AUTO;
	TInt error = KErrGeneral;
	if( aInvitationAutoAccept)
	    {
	    TRACE("autoaccept add contact" ); 
	    delete iInviteId;
	    iInviteId = NULL;
	    iInviteId = aUserId.AllocL();
	    error = iVPbkStoreHandler->CreateVPbkContactL( *iInviteId,aDisplayName ); 
	    }
	TRACE( "error = %d",error ); 
	
	return error;
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL(const MVPbkContactLink& aContactLink) 
	{
	TRACER_AUTO;; 
	TInt error = KErrNotFound;
	// text now holds the name (or first field of the contact) 
	TInt32 linkId = iIdConverter->LinkToIdentifier(aContactLink);
	// a contact entry has been deleted from virtual database store
	TDbColNo colNo = iColset->ColNo( KContactId );
	if ( SeekRowL( colNo, linkId ) )
		{
		TRACE( "link exist in db" ); 
		// contatc found in local database ,delete from local database 
		DeleteContactFromInternalStoreL( linkId );
		TRACE( "link deleted from db" ); 
		// delete from cache contact
		error = iServiceCacheWriter.RemoveContactFromCacheL(aContactLink, 
															TVIMPSTEnums::EStorageEventContactDelete );
		TRACE( "link deleted from cache" ); 
		}
	TRACE("error %d",error );
	
    return error;
  	}
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() 
	{
	TRACER_AUTO;
	iRetrivedContactArray.ResetAndDestroy(); 
	DeleteAllContactFromInternalStoreL();
	TRACE( "all contacts deleted from db" );
	iServiceCacheWriter.RemoveAllCacheContactsL();
	TRACE( "all contacts deleted from cache" );
	
	return KErrNone;// local store : delete from local store ,own user has changed
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks ) 
	{
	TRACER_AUTO;
	iRetrivedContactArray.ResetAndDestroy();
	TRACE( " iRetrivedContactArray Reset"  );
    iVPbkStoreHandler->RetrieveVPbkContactL( aPackedLinks );
    TRACE( "iRetrivedContactArray count= %d " ,iRetrivedContactArray.Count() );
    
    return iRetrivedContactArray.Count();
    }

 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL(TInt aIndex ) 
	{
	TRACER_AUTO;
	TRACE( "aIndex = %d" ,aIndex );
	MVIMPSTStorageContact* contact = NULL;
	if( aIndex >= 0 && aIndex < iRetrivedContactArray.Count() )
        {
        contact = iRetrivedContactArray[ aIndex ];
        }
       
    return contact ? contact->UserId() : KNullDesC ; 	
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL( TInt aIndexToUse  )   
    {
	TRACER_AUTO;
    if( aIndexToUse < 0 && aIndexToUse >= iRetrivedContactArray.Count() )
        {
        TRACE( " argument error"  );
        return KErrArgument;
        }
    TInt error = KErrAlreadyExists;
    MVIMPSTStorageContact* retrivedContact = iRetrivedContactArray[ aIndexToUse ];
    
    MVPbkContactLink* contactLink = retrivedContact->ContactLink();
    MVIMPSTStorageContact* exist = NULL;
    if( contactLink )
	    {
	    exist = iServiceCacheWriter.FindCacheContactByLink( *contactLink  );		
	    }
    if( !exist )
        {
        TRACE( "adding to  cache" );
        MVPbkContactLink* link = retrivedContact->ContactLink();
        if( link )
			{
			CVIMPSTStorageContact* contactToAdd = CVIMPSTStorageContact::NewL( retrivedContact->UserId() ,
												    					 	  retrivedContact->Name(),
																	    	 *link,
																	    	 retrivedContact->AvatarContent()
																	    	 );
			CleanupStack::PushL( contactToAdd );
			TInt32 linkId = iIdConverter->LinkToIdentifier( *link );
			error = iServiceCacheWriter.AddStorageContactToCacheL( contactToAdd ); // contactToAdd ownership is transfered
			TRACE( " adding to  cache done"  );
			if( error == KErrNone )
				{
				CleanupStack::Pop(); // contactToAdd , Ownership is transfered to AddStorageContactToCacheL
				WriteToStoreDbL( linkId );	
				}
			else
				{
				CleanupStack::PopAndDestroy(); // contactToAdd	
				}
			TRACE( " adding to  db done"  );
			}
        }
    TRACE( "error =%d" ,error );
    
    return error;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL( TInt aIndexToUse  )   
    {
	TRACER_AUTO;
    if( aIndexToUse < 0 && aIndexToUse >= iRetrivedContactArray.Count() )
        {
        TRACE( "argument error" );
        return KErrArgument;
        }
    MVIMPSTStorageContact* retrivedContact = iRetrivedContactArray[ aIndexToUse ];
    MVPbkContactLink* contactLink = retrivedContact->ContactLink();
    // delete from cache contact
    CVPbkContactLinkArray* contactsToDelete = CVPbkContactLinkArray::NewLC();
    MVPbkContactLink* link = contactLink->CloneLC();    
    contactsToDelete->AppendL( link ); // takes ownership
    CleanupStack::Pop(); // link
    TInt error = iVPbkStoreHandler->RemoveVPbkContactL( *contactsToDelete );
    CleanupStack::PopAndDestroy(); // contactsToDelete
    
    return error;
    }
// CVIMPSTStorageVPbkLocalStore::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														  const TDesC8& aAvatartData )
    {
	TRACER_AUTO;
    return iVPbkStoreHandler->UpdateAvatarFieldDataL( aContactLink, aAvatartData );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACER_AUTO;
	return iVPbkStoreHandler->RemoveAvatarFieldL( aStoreContact );	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL()
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL( RArray <TPtrC>& aFirstNameList, 
                                                           RArray <TPtrC> &aServiceField ) 
     {
	TRACER_AUTO;
     // in case of server contacts delete all contacts
     ResetAndDestroyLocalArrayL();
     TInt count = aServiceField.Count();
     TRACE( " count=%d" ,count );
     for( TInt i = 0; i<count; i++ )
         {
         TVIMPSTContactInfo contactInf =
							         {
							         aServiceField[i].AllocL() ,
							         aFirstNameList[i].AllocL()	
							         };
         // not exist need to add 
         iFetchContactsToBeAdded.Append( contactInf );
         }
     if( iFetchStep == EContactReadComplete )
	     {
	     TRACE( "start fetcing"  );
	     IssueRequest();	
	     }
	 else
		 {
		 TRACE( " EFetchingCalled"  );
		 iFetchStep = EFetchingCalled;	
		 }
	
     }

//******************* Database Operations *********************************//
// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DbExists
// -----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::DbExists()
	{
	TRACER_AUTO;
	RFile temp;

	TBuf< KMaxPath > storagePath;

	storagePath.Append( KDbPath );    
	storagePath.Append( *iLocalDBName );
	TBool ret = ETrue;
	TInt err( temp.Open( iFs, *(&storagePath), EFileShareReadersOrWriters|EFileRead ) );
	TInt size( 0 );
	if ( err == KErrNone )
		{
		temp.Size( size );
		}
	temp.Close();
	if ( (size == 0) || ( err != KErrNone ) )
		{
		ret = EFalse;
		}
	
	return ret;
	}


// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateDbL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CreateDbL()
	{
	TRACER_AUTO;
	TBuf< KMaxPath > storagePath;
	storagePath.Append( KDbPath );
	iFs.MkDirAll( storagePath );    // make sure the directory exists	
    storagePath.Append( *iLocalDBName );
    iFs.SetAtt( *iLocalDBName ,KEntryAttHidden|KEntryAttSystem , KEntryAttNormal);
    iFileStore = CPermanentFileStore::ReplaceL( iFs, storagePath, EFileShareReadersOrWriters|EFileWrite );
    
	iFileStore->SetTypeL( iFileStore->Layout() );
    
    TStreamId streamId( 0 );
	TRAPD( err, streamId = iDb.CreateL( iFileStore ) );

    if( err ) 
        {
        TRACE( " err = %d" ,err );
        delete iFileStore;
        iFileStore = NULL;

        // remember to keep this updated
        iFs.Delete( storagePath );
        User::Leave( err );
        }

    iFileStore->SetRootL( streamId );
    TInt err2( iFileStore->Commit() );
    User::LeaveIfError( err2 );
    
	TRAPD( err3, CreateTablesL() );
    if( err3 )
        {
        delete iFileStore;
        iFileStore = NULL;
        iDb.Close();
        iFs.Delete( storagePath );
        User::Leave( err3 );
        }
    
    }    

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::OpenDbL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::OpenDbL()
    {
	TRACER_AUTO;
    TBuf< KMaxPath > storagePath;
    storagePath.Append( KDbPath );
    storagePath.Append( *iLocalDBName );
    iFileStore = CPermanentFileStore::OpenL( iFs, storagePath, EFileShareReadersOrWriters|EFileWrite );   
    iFileStore->SetTypeL( iFileStore->Layout() );
    iDb.OpenL( iFileStore, iFileStore->Root() );
    
    }

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CloseDb
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CloseDb()
	{ 
	TRACER_AUTO;
    delete iFileStore;
    iFileStore = NULL;
    iDb.Close();
    
    }

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateTablesL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CreateTablesL()
	{
	TRACER_AUTO;
	CDbColSet* contactId = CDbColSet::NewLC();

	AddColumnL( KContactId, EDbColInt32, contactId );

	TInt err = iDb.CreateTable( KContactTable, *contactId );
	TRACE( "err = %d" ,err );
	User::LeaveIfError( err );

	CleanupStack::PopAndDestroy( contactId );
	
	}

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::AddColumnL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::AddColumnL( const TDesC& aName, TDbColType aType, CDbColSet* aColset  )
	{
	TRACER_AUTO;
	TDbCol column( aName, aType );
	aColset->AddL( column );
	
	}

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL( TInt aSize )
	{
	TRACER_AUTO;
    if ( SysUtil::FFSSpaceBelowCriticalLevelL( &iFs, aSize ) )
        {
		User::Leave( KErrDiskFull );
        }
    
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL()
	{
	TRACER_AUTO;
	iColset->ColNo( KContactId );
	iTable.LastL();	
	while( !iTable.IsEmptyL() )
		{
		iTable.GetL();
		iTable.DeleteL();
		iDb.Compact();
		// delete alwasy first item untill table is empty
		iTable.LastL();
		}
	
	}
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL( TInt32& aIdentifier )
    {
	TRACER_AUTO;
    TDbColNo colNo = iColset->ColNo( KContactId );
    if ( SeekRowL( colNo, aIdentifier ) )
        {
        iTable.DeleteL();
        iDb.Compact();
        TRACE( "found deleted"  );
        }
   
    }
 
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL( TInt32& aIdentifier )
    {
	TRACER_AUTO;
    TDbColNo colNo = iColset->ColNo( KContactId );
    if (!SeekRowL( colNo, aIdentifier ) )        
        {
        iTable.InsertL();
     
        colNo = iColset->ColNo( KContactId );
        iTable.SetColL( colNo, aIdentifier );
            
        // Write row to table
        TRAPD( err, iTable.PutL() );
        if( err != KErrNone )
            {
            // recover
            iTable.Cancel();
            iTable.Reset();
            } 
        }
   
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::OpenTableL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::OpenTableL()
    {
	TRACER_AUTO;
    TInt err( iTable.Open( iDb, KContactTable ) );
    if ( err != KErrNone )
        {
        TRACE( " err =%d" ,err );
        iTable.Close();
        User::Leave( err );
        }
    
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CloseTable
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CloseTable()
    {
	TRACER_AUTO;
    iTable.Close();
   
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadFirstL
// ----------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::ReadFirstL(  TInt32& aIdentifier )
    { 
	TRACER_AUTO;
    iTable.BeginningL();
    
    return ReadNextL( aIdentifier );
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadNextL
// ----------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::ReadNextL( TInt32& aIdentifier )
    { 
	TRACER_AUTO;
    TInt err = KErrNotFound;   
    if ( iTable.NextL() )
        {
        TRAP( err, ReadCurrentL( aIdentifier ) );
		// For some reason ReadCurrentL leaves with KErrEof,
        // even if the contact was read succesfully.
        }
    TRACE( " err = %d" ,err );
   
    return err;
    }


// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadCurrentL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ReadCurrentL( TInt32& aIdentifier )
    {
	TRACER_AUTO;
    iTable.GetL();
	 // Get Column number for contact data size
    TDbColNo colNo = iColset->ColNo( KContactId );
    aIdentifier = iTable.ColInt32(colNo);
   
    }
    
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::SeekRowL
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::SeekRowL( TDbColNo aColNo, TInt32& aIdentifier )
    {
	TRACER_AUTO;
    TBool ret = EFalse;
    iTable.BeginningL();
    while ( iTable.NextL() )
        {
        iTable.GetL();
        if ( iTable.ColInt32( aColNo ) == aIdentifier )
            {
            ret = ETrue;
            break; 
            }
        }
   
    return ret;
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL()
    {
	TRACER_AUTO;
    TBuf< KMaxPath > storagePath;
    storagePath.Append( KDbPath ); 
    iFs.MkDirAll( storagePath );   
    storagePath.Append( *iLocalDBName );
    //close db before deleting it.
    CloseDb();
    iFs.Delete( storagePath );
   
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::IsSameContactIdL
// ----------------------------------------------------------
TBool CVIMPSTStorageVPbkLocalStore::IsSameContactIdL(const TDesC& aFirstId, const TDesC& aSecondId )
    {
	TRACER_AUTO;
    TInt colIndex = aFirstId.Find(_L(":"));
    TPtrC firstId = aFirstId.Right( aFirstId.Length() - colIndex -1);
    colIndex = aSecondId.Find(_L(":"));
    TPtrC secondId = aSecondId.Right( aSecondId.Length() - colIndex -1);
    //if( Compare( firstId,secondId  ) == 0 )
    if(firstId.Compare(aSecondId) ==0 )
        {
        return ETrue;
        }
    return EFalse;
    }
// End of file

