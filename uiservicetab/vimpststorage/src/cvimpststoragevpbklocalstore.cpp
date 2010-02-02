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
#include "vimpstdebugtrace.h" 
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CVIMPSTStorageVPbkLocalStore()") ); 
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::NewL() begin") ); 
    CVIMPSTStorageVPbkLocalStore* self = NewLC( aContactDb,  
    										   aServiceName,
											  aServiceCacheWriter);
    CleanupStack::Pop( self );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::NewL() end") ); 
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::NewLC() begin") );
    CVIMPSTStorageVPbkLocalStore* self =
        new (ELeave) CVIMPSTStorageVPbkLocalStore(aServiceCacheWriter);
    CleanupStack::PushL( self );
    self->ConstructL( aContactDb, aServiceName );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::NewLC() end") );
    return self;
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ConstructL( const TDesC& aContactDb, const TDesC& aServiceName )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() begin") );
    iVPbkStoreHandler = CVIMPSTStorageVPbkStoreHandler::NewL( aContactDb ,aServiceName, *this,ETrue );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() iVPbkStoreHandler created") );
     //construct the persistent store name
    iLocalDBName = HBufC::NewL( aServiceName.Length()+ KStorageExtn().Length());
    TPtr iLocalDBNamePtr = iLocalDBName->Des();
    iLocalDBNamePtr.Append(aServiceName);
    _LIT(KSpecialChar ,"/\\:*?<>\"");
    AknTextUtils :: StripCharacters(iLocalDBNamePtr,KSpecialChar); 
    iLocalDBNamePtr.Append(KStorageExtn());    
    TRACE( T_LIT("iLocalDBNamePtr: %S"), &iLocalDBNamePtr );
    MVPbkContactStore* defaultStore = iVPbkStoreHandler->GetDefaultStoreL( aContactDb );
    User::LeaveIfNull(defaultStore);  
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() defaultStore retrived") );
    iIdConverter = CVPbkContactIdConverter::NewL(*defaultStore);
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() iIdConverter created") ); 
    User::LeaveIfError( iFs.Connect() );
    // Open existing or create new database.
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() connected to file server") ); 
	if ( DbExists() )
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() local db exist") ); 
        OpenDbL();
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() open success") );
		}
	else
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() local db not exist") );
        DoFreespaceLevelCheckL( KEmptyDbSize );
        CreateDbL();
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() local db created") );
		}
    OpenTableL();
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() OpenTableL success ") );
    iColset = iDb.ColSetL( KContactTable );    
   	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() iColset is set") );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ConstructL() end") );	
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::
//      ~CVIMPSTStorageVPbkLocalStore
// ---------------------------------------------------------------------------
//
CVIMPSTStorageVPbkLocalStore::~CVIMPSTStorageVPbkLocalStore()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::~CVIMPSTStorageVPbkLocalStore() begin") );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::~CVIMPSTStorageVPbkLocalStore() end") );
    }
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::
//      ResetAndDestroyLocalArrayL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ResetAndDestroyLocalArrayL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ResetAndDestroyLocalArrayL() begin") );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ResetAndDestroyLocalArrayL() end") );
    }
 
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::LocalStore
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::LocalStore() const 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::LocalStore() local store") );
	return ETrue;
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() begin") );
	switch( aVPbkStoreEvent.iEventType )
	    {
	    case EVPbkContactReadCompleted:
	    	{
	    	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactReadCompleted started") );
	    	iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactReadComplete);
	    	if( iFetchStep == EFetchingCalled )
		    	{
		    	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactReadCompleted EFetchingCalled ") );
		    	IssueRequest();		
		    	}
	    	iFetchStep = EContactReadComplete;
	    	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EContactReadComplete completed ") );
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
				TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactReading contact") );
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
		         TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactFetching contact") );
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
				TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactAdded start") );
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
				TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactAdded completed") );
				}
			else if(  iInviteId )
			    {
			    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() iInviteId accepted") );
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
                 TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() iInviteId accepted sucess") );
			    }
			break;	
			}
	   	case EVPbkContactSynchronizing:
	   	    {
	   	    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactSynchronizing " ) );
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
               TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL  EVPbkContactSynchronizing fetchin on" ) );
               IssueRequest();
               }
	   	    TRACE( T_LIT("CVIMPSTStorageVPbkServerStore::HandleVPbkStoreEventL EVPbkContactSynchronizing completed" ) );
	   	    break;
	   	    }
		case EVPbkContactDeleted:
		    {
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactDeleted") );
		    TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
		    // a contact entry has been deleted from virtual database store
		    TDbColNo colNo = iColset->ColNo( KContactId );
		    if ( SeekRowL( colNo, linkId ) )
		        { 
		        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactDeleted found in db") );
		        // contatc found in local database ,delete from local database 
		        DeleteContactFromInternalStoreL( linkId );
		        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactDeleted deleted from db") );
		         // delete from cache contact
		    	iServiceCacheWriter.RemoveContactFromCacheL(*aVPbkStoreEvent.iContactLink, 
		    												TVIMPSTEnums::EStorageEventDeleteFromPbk );
		    	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactDeleted deleted from cache") );
		        }
		    break;	 
		    }
		case EVPbkContactChanged:
		    {
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactChanged ") );
		    TInt32 linkId = iIdConverter->LinkToIdentifier(*aVPbkStoreEvent.iContactLink );
		    TDbColNo colNo = iColset->ColNo( KContactId );
		    if ( SeekRowL( colNo, linkId ) )
		       {
		       TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactChanged found in local db") );
		       // found in the list update it
		       iServiceCacheWriter.UpdateCacheContactL(*aVPbkStoreEvent.iContactLink , 
					    						 	 aVPbkStoreEvent.iUserId ,
					    							 aVPbkStoreEvent.iDisplayName,
					    							 aVPbkStoreEvent.iAvatarContent );
			   TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactChanged changed in cache") );
		       }
		    break;	
		    }
	    case EVPbkContactRetriving:
		    {
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactRetriving") );
		    // in case of local we allow all 
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactRetriving valid Id") );
			CVIMPSTStorageContact* retrivedContact = CVIMPSTStorageContact::NewL( aVPbkStoreEvent.iUserId ,
											    								  aVPbkStoreEvent.iDisplayName,
											    								  *aVPbkStoreEvent.iContactLink,
											    								  aVPbkStoreEvent.iAvatarContent);
			iRetrivedContactArray.Append( retrivedContact ); // takes the ownership of retrivedContact
			TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkContactRetriving valid Id added") );
			// keep the backup of retrieved contacts
			// user can add later once it sent to server
			break;	
		    }
		case EVPbkUnknownChanges:
		    {
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkUnknownChanges start") );
		    DeleteAllContactFromInternalStoreL();
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkUnknownChanges all delete from db") );
		     // unknow mean all contact deleted from store
		    iServiceCacheWriter.RemoveAllCacheContactsL();
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() EVPbkUnknownChanges all delete from cache") );
		    break;	
		    }
	    default:
		 	break;
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::HandleVPbkStoreEventL() end") );
	}


// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::IssueRequest
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::IssueRequest()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::IssueRequest() begin") );
    if( !IsActive() )
	    {
	    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::IssueRequest() SetActive") );
	    TRequestStatus* status = &iStatus;
	    User::RequestComplete( status, KErrNone );
	    SetActive();
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::IssueRequest() begin") );
    }
    
// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RunL
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::RunL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() begin") );
    TInt count = iFetchContactsToBeAdded.Count();
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() count to be added = %d "),count );
    if( !count )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch completed " ) );
        iFetchStep = EContactReadComplete;
        iServiceCacheWriter.NotifyServiceViewL( TVIMPSTEnums::EStorageContactFetchComplete );
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch completed notification sent" ) );     
	    }
    else
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch in progress" ) );     
        iFetchStep  = EFetchingOn;
        TVIMPSTContactInfo contactInfoToAdd = iFetchContactsToBeAdded[0];
        MVIMPSTStorageContact* exist = iServiceCacheWriter.FindCacheContactByUserId( *contactInfoToAdd.iUserId );
        if( exist )
	        {
	        // contact is already exist ,send the notification about this
	        iServiceCacheWriter.NotifyServiceViewL(TVIMPSTEnums::EStorageContactFetchExistInStore, exist );
	        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch in progress id exist" ) ); 
	        // process next
	        IssueRequest();
	        }
	    else
		    {
		    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch in progress create id in store called" ) ); 
		     // pass the display NAme in place of NULL
        	iVPbkStoreHandler->CreateVPbkContactL( *contactInfoToAdd.iUserId, *contactInfoToAdd.iDisplayName ); // process always 0th item
            }
        iFetchContactsToBeAdded.Remove( 0 );
        delete contactInfoToAdd.iUserId;
        contactInfoToAdd.iUserId = NULL;
        delete contactInfoToAdd.iDisplayName;
        contactInfoToAdd.iDisplayName = NULL ;
        iFetchContactsToBeAdded.Compress();
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() fetch in progress iFetchContactsToBeAdded ,one entry removed" ) ); 
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunL() end") );
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DoCancel
// --------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DoCancel()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DoCancel()") );
    }

// --------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RunError
// --------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::RunError( TInt aError )
    { 
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RunError()") );   
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
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL() begin") ); 
	TInt error = KErrGeneral;
	if( aInvitationAutoAccept)
	    {
	    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL() autoaccept add contact") ); 
	    delete iInviteId;
	    iInviteId = NULL;
	    iInviteId = aUserId.AllocL();
	    error = iVPbkStoreHandler->CreateVPbkContactL( *iInviteId,aDisplayName ); 
	    }
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL() error = %d"),error ); 
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateVPbkContactL() begin") ); 
	return error;
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL(const MVPbkContactLink& aContactLink) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() begin") ); 
	TInt error = KErrNotFound;
	// text now holds the name (or first field of the contact) 
	TInt32 linkId = iIdConverter->LinkToIdentifier(aContactLink);
	// a contact entry has been deleted from virtual database store
	TDbColNo colNo = iColset->ColNo( KContactId );
	if ( SeekRowL( colNo, linkId ) )
		{
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() link exist in db") ); 
		// contatc found in local database ,delete from local database 
		DeleteContactFromInternalStoreL( linkId );
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() link deleted from db") ); 
		// delete from cache contact
		error = iServiceCacheWriter.RemoveContactFromCacheL(aContactLink, 
															TVIMPSTEnums::EStorageEventContactDelete );
		TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() link deleted from cache") ); 
		}
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() error %d"),error );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveVPbkContactL() end") ); 
    return error;
  	}
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() begin" ) );
	iRetrivedContactArray.ResetAndDestroy(); 
	DeleteAllContactFromInternalStoreL();
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() all contacts deleted from db") );
	iServiceCacheWriter.RemoveAllCacheContactsL();
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() all contacts deleted from cache") );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveAllVPbkContactsL() end" ) );
	return KErrNone;// local store : delete from local store ,own user has changed
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL() begin" ) );
	iRetrivedContactArray.ResetAndDestroy();
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL() iRetrivedContactArray Reset" ) );
    iVPbkStoreHandler->RetrieveVPbkContactL( aPackedLinks );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL() iRetrivedContactArray count= %d " ),iRetrivedContactArray.Count() );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RetrieveVPbkXSPIdL() end " ) );
    return iRetrivedContactArray.Count();
    }

 // ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL(TInt aIndex ) 
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL() begin" ) );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL() aIndex = %d" ),aIndex );
	MVIMPSTStorageContact* contact = NULL;
	if( aIndex >= 0 && aIndex < iRetrivedContactArray.Count() )
        {
        contact = iRetrivedContactArray[ aIndex ];
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::GetRetrieveVPbkXSPIdL() end" ) );   
    return contact ? contact->UserId() : KNullDesC ; 	
	}
// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL( TInt aIndexToUse  )   
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() begin" ) );
    if( aIndexToUse < 0 && aIndexToUse >= iRetrivedContactArray.Count() )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() argument error" ) );
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
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() adding to  cache" ) );
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
			TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() adding to  cache done" ) );
			if( error == KErrNone )
				{
				CleanupStack::Pop(); // contactToAdd , Ownership is transfered to AddStorageContactToCacheL
				WriteToStoreDbL( linkId );	
				}
			else
				{
				CleanupStack::PopAndDestroy(); // contactToAdd	
				}
			TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() adding to  db done" ) );
			}
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() error =%d" ),error );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateRetriveVPbkContactL() end" ) );
    return error;
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL( TInt aIndexToUse  )   
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL() begin" ) );
    if( aIndexToUse < 0 && aIndexToUse >= iRetrivedContactArray.Count() )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL() argument error" ) );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::deleteRetriveVPbkContactL() begin" ) );
    return error;
    }
// CVIMPSTStorageVPbkLocalStore::UpdateAvatarFieldDataL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
														  const TDesC8& aAvatartData )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::UpdateAvatarFieldDataL()" ) );
    return iVPbkStoreHandler->UpdateAvatarFieldDataL( aContactLink, aAvatartData );
    }

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::RemoveAvatarFieldL()
// ---------------------------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact)
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::RemoveAvatarFieldL()" ) );
	return iVPbkStoreHandler->RemoveAvatarFieldL( aStoreContact );	
	}

// ---------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL()
// ---------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL( RArray <TPtrC>& aFirstNameList, 
                                                           RArray <TPtrC> &aServiceField ) 
     {
     TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL() begin" ) );
     // in case of server contacts delete all contacts
     ResetAndDestroyLocalArrayL();
     TInt count = aServiceField.Count();
     TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL() count=%d" ),count );
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
	     TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL() start fetcing" ) );
	     IssueRequest();	
	     }
	 else
		 {
		 TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL() EFetchingCalled" ) );
		 iFetchStep = EFetchingCalled;	
		 }
	 TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddVPbkFetchContactsL() end" ) );
     }

//******************* Database Operations *********************************//
// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DbExists
// -----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::DbExists()
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DbExists() begin" ) );
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
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DbExists() end" ) );
	return ret;
	}


// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateDbL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CreateDbL()
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateDbL() begin" ) );
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
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateDbL() err = %d" ),err );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateDbL() end" ) );
    }    

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::OpenDbL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::OpenDbL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::OpenDbL() begin" ) );
    TBuf< KMaxPath > storagePath;
    storagePath.Append( KDbPath );
    storagePath.Append( *iLocalDBName );
    iFileStore = CPermanentFileStore::OpenL( iFs, storagePath, EFileShareReadersOrWriters|EFileWrite );   
    iFileStore->SetTypeL( iFileStore->Layout() );
    iDb.OpenL( iFileStore, iFileStore->Root() );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::OpenDbL() begin" ) );
    }

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CloseDb
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CloseDb()
	{ 
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CloseDb() begin" ) );
    delete iFileStore;
    iFileStore = NULL;
    iDb.Close();
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CloseDb() end" ) );
    }

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CreateTablesL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CreateTablesL()
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateTablesL() begin" ) );
	CDbColSet* contactId = CDbColSet::NewLC();

	AddColumnL( KContactId, EDbColInt32, contactId );

	TInt err = iDb.CreateTable( KContactTable, *contactId );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateTablesL() err = %d" ),err );
	User::LeaveIfError( err );

	CleanupStack::PopAndDestroy( contactId );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CreateTablesL() end" ) );	
	}

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::AddColumnL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::AddColumnL( const TDesC& aName, TDbColType aType, CDbColSet* aColset  )
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddColumnL() begin" ) );
	TDbCol column( aName, aType );
	aColset->AddL( column );
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::AddColumnL() end" ) );
	}

// -----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL
// -----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL( TInt aSize )
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL() begin" ) );
    if ( SysUtil::FFSSpaceBelowCriticalLevelL( &iFs, aSize ) )
        {
		User::Leave( KErrDiskFull );
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DoFreespaceLevelCheckL() end" ) );
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL()
	{
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL() begin" ) );
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
	TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteAllContactFromInternalStoreL() end" ) );
	}
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL( TInt32& aIdentifier )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL() begin" ) );
    TDbColNo colNo = iColset->ColNo( KContactId );
    if ( SeekRowL( colNo, aIdentifier ) )
        {
        iTable.DeleteL();
        iDb.Compact();
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL() found deleted" ) );
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteContactFromInternalStoreL() end" ) );
    }
 
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL( TInt32& aIdentifier )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL() begin" ) );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::WriteToStoreDbL() end" ) );    
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::OpenTableL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::OpenTableL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::OpenTableL() begin" ) );
    TInt err( iTable.Open( iDb, KContactTable ) );
    if ( err != KErrNone )
        {
        TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::OpenTableL() err =%d" ),err );
        iTable.Close();
        User::Leave( err );
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::OpenTableL() end" ) );
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::CloseTable
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::CloseTable()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CloseTable() begin" ) );
    iTable.Close();
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::CloseTable() end" ) );
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadFirstL
// ----------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::ReadFirstL(  TInt32& aIdentifier )
    { 
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadFirstL() begin" ) );   
    iTable.BeginningL();
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadFirstL() end" ) );
    return ReadNextL( aIdentifier );
    }

// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadNextL
// ----------------------------------------------------------
//
TInt CVIMPSTStorageVPbkLocalStore::ReadNextL( TInt32& aIdentifier )
    { 
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadNextL() begin" ) );
    TInt err = KErrNotFound;   
    if ( iTable.NextL() )
        {
        TRAP( err, ReadCurrentL( aIdentifier ) );
		// For some reason ReadCurrentL leaves with KErrEof,
        // even if the contact was read succesfully.
        }
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadNextL() err = %d" ),err );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadNextL() end" ) );
    return err;
    }


// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::ReadCurrentL
// ----------------------------------------------------------
//
void CVIMPSTStorageVPbkLocalStore::ReadCurrentL( TInt32& aIdentifier )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadCurrentL() begin" ) );
    iTable.GetL();
	 // Get Column number for contact data size
    TDbColNo colNo = iColset->ColNo( KContactId );
    aIdentifier = iTable.ColInt32(colNo);
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::ReadCurrentL() end" ) );
    }
    
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::SeekRowL
// ----------------------------------------------------------
//
TBool CVIMPSTStorageVPbkLocalStore::SeekRowL( TDbColNo aColNo, TInt32& aIdentifier )
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::SeekRowL() begin" ) );
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
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::SeekRowL() end" ) );
    return ret;
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL()
    {
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL() begin" ) );
    TBuf< KMaxPath > storagePath;
    storagePath.Append( KDbPath ); 
    iFs.MkDirAll( storagePath );   
    storagePath.Append( *iLocalDBName );
    //close db before deleting it.
    CloseDb();
    iFs.Delete( storagePath );
    TRACE( T_LIT("CVIMPSTStorageVPbkLocalStore::DeleteDatabaseL() end" ) );
    }
// ----------------------------------------------------------
// CVIMPSTStorageVPbkLocalStore::IsSameContactIdL
// ----------------------------------------------------------
TBool CVIMPSTStorageVPbkLocalStore::IsSameContactIdL(const TDesC& aFirstId, const TDesC& aSecondId )
    {
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

