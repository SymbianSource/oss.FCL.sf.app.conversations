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
* Description:  Storage service view implementation
*
*/


// INCLUDE FILES
#include "cvimpststorageserviceview.h"

#include "cvimpststoragecontactlist.h"
#include "cvimpststoragecontact.h"
#include "mvimpststoragevpbkcontactstore.h"
#include "cvimpststoragevpbkserverstore.h"
#include "cvimpststoragevpbklocalstore.h"
#include "cvimpststoragecontactsorter.h"
#include "cvimpststorageactivehandler.h"
#include "mvimpststoragecontactsobserver.h"
#include "vimpststorageutils.h"

#include <MVPbkStoreContact.h>
#include <VPbkContactStoreUris.h>
#include <TVPbkContactStoreUriPtr.h>
#include "uiservicetabtracer.h"
#include "tvimpstconsts.h"
#include "mvimpststoragecontact.h"

 
// ============================ MEMBER FUNCTIONS ===============================


// panic handler
GLDEF_C void Panic( TVIMPSTStoragePanics aPanic )
    {
    User::Panic( KPanicCategory, aPanic );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CVIMPSTStorageServiceView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageServiceView::CVIMPSTStorageServiceView(TUint32 aServiceId) :
    iServiceId(aServiceId),
    iOwnContact( NULL ),
    iUnNamedText( NULL )
    {
	TRACER_AUTO;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ConstructL
// Symbian 2nd phase constructor can leave.
// The constructor also does synchronizing with Phone Book by removing the
// deleted elements from the persistent storage
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::ConstructL(const TDesC& aStoreName, const TDesC& aServiceName)
    {
	TRACER_AUTO;
    
    iActiveHandler =  CVIMPSTStorageActiveHandler::NewL(this);
					 
    // create sorter for contacts
    iContactSorter = CVIMPSTStorageContactSorter::NewL();
    
    //set the sorting algorithem to order by presence.
    iContactSorter->SetSortAlgorithm( MVIMPSTStorageContact::ECompareByPresence);
    
    TRACE( "SubServiceType() Type : %S", &aStoreName );
    //find whether its a local store or xsp store
	if ( VPbkContactStoreUris::DefaultCntDbUri().CompareC( aStoreName, 1, NULL )
	                        != 0 )
		{
	TRACE( " service store created" );
		//Create XSP Store 
    	iVPbkContactStore = 
    			CVIMPSTStorageVPbkServerStore::NewL(aStoreName,aServiceName ,*this );
		}
	else
		{
	TRACE( "local store created " ); 
		//Create Local Contacts.cdb store 
		iVPbkContactStore = CVIMPSTStorageVPbkLocalStore::NewL(VPbkContactStoreUris::DefaultCntDbUri(), 
															   aServiceName,
															   *this );	
		}
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageServiceView* CVIMPSTStorageServiceView::NewL(TUint32 aServiceId,
										const TDesC& aStoreName,
										const TDesC& aServiceName)
    {
	TRACER_AUTO;
    
    CVIMPSTStorageServiceView* self = new( ELeave ) CVIMPSTStorageServiceView(aServiceId);
    CleanupStack::PushL( self );
    self->ConstructL(aStoreName, aServiceName);
    CleanupStack::Pop( self );
    return self;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView
// Two-phased constructor.
// -----------------------------------------------------------------------------
// Destructor
CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView()
    {
	TRACER_AUTO;
    if( iActiveHandler )
	    {
	    iActiveHandler->Cancel();
	    delete iActiveHandler;	
	    }
    
    iContactListArray.ResetAndDestroy();

	delete iOwnContact;
 	delete iContactSorter;
	delete iVPbkContactStore;
    iContactObservers.Reset();
    iContactObservers.Close();
    
    if(iUnNamedText)
        {
        delete iUnNamedText;   
        iUnNamedText = NULL;
        }        
        
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::AddObserverL
// From MVIMPSTStorageServiceView
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::AddObserverL( MVIMPSTStorageContactsObserver*
        aObserver )
    {
	TRACER_AUTO;
    User::LeaveIfError( aObserver ? KErrNone : KErrArgument );
    TInt index = iContactObservers.Find( aObserver );
    if( index == KErrNotFound )
	    {
		TRACE( "aObserver added ");
	    User::LeaveIfError( iContactObservers.Append( aObserver ) );	
	    }
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveObserverL
// From MVIMPSTStorageServiceView
// -----------------------------------------------------------------------------
void CVIMPSTStorageServiceView::RemoveObserver( MVIMPSTStorageContactsObserver*
        aObserver )
	{
	TRACER_AUTO;
	__ASSERT_ALWAYS( aObserver, Panic( EObserverIsNull ));
	const TInt index( iContactObservers.Find( aObserver ) );
	if ( index >=0)
		{
		TRACE( "aObserver found" );
		iContactObservers.Remove( index );
		iContactObservers.Compress();
		}
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::SetUnnamedTextL
// From MVIMPSTStorageServiceView
// -----------------------------------------------------------------------------
void CVIMPSTStorageServiceView::SetUnnamedTextL(HBufC* aUnnamedText )
	{
	// takes ownership
	iUnNamedText = aUnnamedText;	
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateContactListL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* CVIMPSTStorageServiceView::CreateContactListL( 
										   const TDesC& aContactListId, 
                                           const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    CVIMPSTStorageContactList* contactList = 
    						CVIMPSTStorageContactList::NewLC(*iContactSorter,
                                                         	  aContactListId, 
                                                         	  aDisplayName );
                                                         
    TIdentityRelation< CVIMPSTStorageContactList > findBy( 
                                CVIMPSTStorageServiceView::ContactListFindByContactListId);
    TInt indexOfList( iContactListArray.Find( contactList, findBy ) );
    if( indexOfList == KErrNotFound )
        {
		TRACE( "list not exist in list");
        TLinearOrder< CVIMPSTStorageContactList > order( 
                             CVIMPSTStorageServiceView::ContactListOrderByDisplayName );
        iContactListArray.InsertInOrderAllowRepeats( contactList, order );
        CleanupStack::Pop( contactList ); 
        TRACE( " new list created");
        }
    else
        {
		TRACE( " list  already exist in list" );
        CleanupStack::PopAndDestroy( contactList );
        contactList = iContactListArray[ indexOfList ];
        }
    return contactList; 
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveContactList
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::RemoveContactList( const TDesC& aContactListId )
    {
	TRACER_AUTO;
    TInt pos( FindContactListById( aContactListId ) );
    if( pos >= 0 )
        {
		TRACE( "aContactListId found" );
        delete iContactListArray[ pos ];
        iContactListArray.Remove( pos );  
        iContactListArray.Compress();      
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ListCount
// Count of contactlists
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::ListCount() const
    {
	TRACER_AUTO;
    return iContactListArray.Count();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ListAt
// Reference to contact list
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList& CVIMPSTStorageServiceView::ListAt( TInt aIndex ) const
    {  
	TRACER_AUTO;
    // User::LeaveIfError( aIndex < iContactListArray.Count() ? KErrNone : KErrArgument ); 
    __ASSERT_ALWAYS( (aIndex < iContactListArray.Count() && aIndex >=0 ), Panic( EContactsArrayOutOfSync ));  
    return *iContactListArray[ aIndex ];
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ContactCount
// Count of contacts
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::ContactCount( TBool aSkipOfflineContacts ) const
    {
	TRACER_AUTO;
    TInt contactCount( 0 );
    TInt count( iContactListArray.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
        contactCount += iContactListArray[ a ]->ContactCount( aSkipOfflineContacts, 
                                                         EFalse );
        }
		TRACE(" contactCount = %d" ,contactCount );
    return contactCount;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::UpdatepresenceL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageServiceView::UpdatePresenceL( 
					                             const TDesC& aContactId, 
					                             TVIMPSTEnums::TOnlineStatus aStatus,
					                             const TDesC& aStatusText,
					                             const TDesC8& aAvatarData,
					                             TBool aIsClearingAvatar /*= EFalse*/)
	{
	TRACER_AUTO;
	MVIMPSTStorageContact* contact = NULL;
	if( iOwnContact && ( aContactId.Compare( iOwnContact->UserId() ) == 0  ) )
	    {
		TRACE("own Item presence");
	    if( aAvatarData.Compare(iOwnContact->AvatarContent() ) != 0 )
	        {
	        if((aIsClearingAvatar)||(aAvatarData.Length() > 0))
	            {
				TRACE( "own avatar change NotifyAllObserversL");
	            iOwnContact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore );  
	            NotifyAllObserversL(TVIMPSTEnums::EStorageAvatarChange,NULL,iOwnContact,0);
	            }
	        }
	    if( ( iOwnContact->OnlineStatus() !=  aStatus ) || ( aStatusText.Compare( iOwnContact->StatusText() ) != 0 ) )
	        {
		    // own presence has changes
		    // update to own presence
		    iOwnContact->SetOnlineStatus( aStatus );
		    iOwnContact->SetStatusTextL(aStatusText);
		    TRACE( " own presene NotifyAllObserversL");
		    NotifyAllObserversL(TVIMPSTEnums::EStorageOwnPresenceChange,NULL, iOwnContact,0);
		    }
	    contact = iOwnContact;
		}
	else
		{
		TRACE( "contact item ");
		contact = FindContactByUserId( aContactId );
		if(contact)
			{
			TRACE( "contact exist " );
			TInt index = KErrNotFound;
			TVIMPSTEnums::TOnlineStatus oldOnlineStatus = contact->OnlineStatus();
			TRACE( "contact oldOnlineStatus = %d", oldOnlineStatus );
			TRACE( "contact new presence = %d", aStatus );
			//status text can be different for the sam status hence not inside the if condition.
			if(  oldOnlineStatus != aStatus )
			    {
				TRACE( "contact presence change ");

			    contact->SetOnlineStatus( aStatus );
			    TInt count = iContactListArray.Count();
			    for(TInt i = 0; i < count; i++)
			        {
			        index = iContactListArray[i]->FindIndexOfContact(contact);
			        if(KErrNotFound != index )
			            {
			            iContactListArray[i]->ResortContact(contact);
			            TRACE( "contact ResortContact " );
			            // get the index of the sorted contact, as after sorting the 
			            // index would have changed based on presence.
			            // break after the contact list is found and the contact is found
			            break;
			            }
			        }
			    contact->SetStatusTextL(aStatusText);
			    if(aAvatarData.Length()>0)
			    {
			     TInt oldIndex = IndexOfContact (contact);   
			     contact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);  
			     NotifyAllObserversL(TVIMPSTEnums::EStorageAvatarChange,NULL, contact, oldIndex );
			    }
			
				// once this event occued ,apply the sorting
				NotifyAllObserversWithDelay(TVIMPSTEnums::EStorageMultiplePresenceChange,NULL, contact, -1);
			    }
			else if( aStatusText.Compare( contact->StatusText() ) != 0 )
			    {
			    TInt oldIndex = IndexOfContact (contact);	
			    contact->SetStatusTextL(aStatusText);
			    NotifyAllObserversL(TVIMPSTEnums::EStoragePresenceChange,NULL, contact, oldIndex );	
			    }
			else if( aAvatarData.Length() && aAvatarData.Compare( contact->AvatarContent() ) != 0  )
			    {
			    TInt oldIndex = IndexOfContact (contact);   
			    contact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);  
			    NotifyAllObserversL(TVIMPSTEnums::EStorageAvatarChange,NULL, contact, oldIndex );
			    }
			TRACE("aStatusText = %S ", &aStatusText );
			TRACE( "NotifyAllObserversL called" );
			TRACE( "NotifyAllObserversL finish" );	
			}
	 	}	
    return contact;
    }
 
 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::UpdateAvatarL
// -----------------------------------------------------------------------------
//
 MVIMPSTStorageContact* CVIMPSTStorageServiceView::UpdateAvatarL(const TDesC& aContactId,
							                  const TDesC8& aAvatarData ) 
 {
 TRACER_AUTO;
 MVIMPSTStorageContact* contact = NULL;
 if( iOwnContact && ( aContactId.Compare( iOwnContact->UserId() ) == 0  ) )
		{
		TRACE( "own avatar" );
		iOwnContact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);	
		NotifyAllObserversL(TVIMPSTEnums::EStorageOwnPresenceChange,NULL, iOwnContact,0);
	    contact = iOwnContact;
		}
	else
		{
		TRACE( "buddy avatar" );
	    contact = FindContactByUserId( aContactId );
		if(contact)
			{
			TRACE( "contact exit");
			contact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);	
			TInt index = IndexOfContact(contact);
		    NotifyAllObserversL(TVIMPSTEnums::EStorageAvatarChange,NULL, contact,index);
		    TRACE( " NotifyAllObserversL called" );		
			}
	   	}
   return contact;
   }
           
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListInternal
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList* CVIMPSTStorageServiceView::FindContactListInternal( 
                                                const TDesC& aListId )
    {
	TRACER_AUTO;
	TInt contactListIndex( FindContactListById( aListId ) );
    return ( contactListIndex >= 0 ? iContactListArray[ contactListIndex ] : NULL );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* CVIMPSTStorageServiceView::FindContactList( const TDesC& aListId )
    {
	TRACER_AUTO;     
    return FindContactListInternal( aListId );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListById
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::FindContactListById(const TDesC& aListId )
	{
	TRACER_AUTO;
	TInt index = KErrNotFound;
	TInt count( iContactListArray.Count() );
	TRACE( " count = %d", count );
	for( TInt i= 0  ; i < count ; i++ )
		{
		if( KErrNone == VIMPSTStorageUtils::NeutralCompare( 
							 aListId, 
                             iContactListArray[ i ]->ListId(), EFalse )   )
			{
			TRACE( " contact list found" );
			index = i;
			break;
			}
		}
	return index;
	}
	
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListById
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindContactByLink(const MVPbkContactLink& aContactLink )
	{
	TRACER_AUTO;
	MVIMPSTStorageContact* contact = NULL;
	TInt count( iContactListArray.Count() );
    for( TInt i= 0  ; i < count ; i++ )
        {
        CVIMPSTStorageContactList* contactList = iContactListArray[ i ];
        contact = contactList->FindContactByContactLink( aContactLink );
        if( contact )
            { 
            TRACE( " contact found = %d ", i );           
            break;
            }
        }
    return contact;
  	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetServiceId
// -----------------------------------------------------------------------------
//
TUint32 CVIMPSTStorageServiceView::GetServiceId()
    {
	TRACER_AUTO;
    return iServiceId;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::LocalStore
// -----------------------------------------------------------------------------
//
TBool CVIMPSTStorageServiceView::IsLocalStore() const
	{
	TRACER_AUTO;
	return iVPbkContactStore->LocalStore();	
	}  
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::OwnContactL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& CVIMPSTStorageServiceView::OwnContactL()
	{
	TRACER_AUTO;
	if( !iOwnContact )
		{
		iOwnContact = CVIMPSTStorageContact::NewL(KNullDesC,
		        								  KNullDesC )	;	
		}
	return *iOwnContact;	
	} 

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::SetOwnUserIdL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::SetOwnUserIdL(const TDesC& aUserId ) 
	{
	TRACER_AUTO;
	if( !iOwnContact )
		{
		iOwnContact = CVIMPSTStorageContact::NewL(aUserId,
		        								  KNullDesC
		        								  )	;
		TRACE( "iOwnContact created" );
		}
	else if( aUserId.Compare( iOwnContact->UserId() ) != 0 )
		{
	    _LIT (KNullWithSpace, " ");
		TRACE( "SetOwnUserIdL iOwnContact was existing" );
	    if(KNullWithSpace ().Compare(iOwnContact->UserId())!=0)
	        { 
	        iVPbkContactStore->RemoveAllVPbkContactsL(); // if user id is changed remove all contacts 	
	        RemoveAllCacheContactsL();
	        }
		iOwnContact->SetUserIdL( aUserId );
		iOwnContact->SetAvatarContentL(KNullDesC8,*iVPbkContactStore );

		NotifyAllObserversL( TVIMPSTEnums::EStorageEventOwnUserChanged,NULL,iOwnContact,0 );
		TRACE( "NotifyAllObserversL delivered" );
		}
	}    
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactByUserId
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindContactByUserId( const TDesC& aUserId )
	{
	TRACER_AUTO;
	MVIMPSTStorageContact* contact = NULL;
	CVIMPSTStorageContactList* contactList = NULL;
	TInt count( iContactListArray.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
        contactList = iContactListArray[ a ];
        contact = contactList->FindContact( aUserId );
        if( contact )
            {  
            TRACE(" contact found " );          
            break;
            }
        if(!contact)
        	{
			TInt acount =contactList->Count();
			for( TInt i( 0 ); i < acount; ++i )
				{
				MVIMPSTStorageContact* acontact = &(contactList->operator [](i));
				if( acontact->UserId().Compare( aUserId) == 0 )
					{
					contact=acontact;
					break;
					}
				}
        	}
        }
    
    return contact;
  	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactByUserId
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindCacheContactByUserId( const TDesC& aUserId )
    {
	TRACER_AUTO;
    return FindContactByUserId( aUserId );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindCacheContactByLink
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindCacheContactByLink(const MVPbkContactLink& aContactLink )
    {
	TRACER_AUTO;
    return FindContactByLink( aContactLink );
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateContactL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageServiceView::CreateNewContactL(const TDesC& aUserId, 
        										   	   	         	const TDesC& aDisplayName /*= KNullDesC*/,
        										   	   	         	TBool aIsInvitationItem /*=EFalse*/,
        										   	   	         	TBool aInvitationAutoAccept /*= EFalse */)
    {
	TRACER_AUTO;
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    MVIMPSTStorageContact* contact = FindCacheContactByUserId( aUserId );
    if( !contact )
	    {
	    iIsInvitationItem = aIsInvitationItem;
	    TRACE( "contact does not exist in list" );
	    iVPbkContactStore->CreateVPbkContactL( aUserId ,aDisplayName, aInvitationAutoAccept );	
	    TRACE( "new contact created");
	    }
    return contact;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateNewFetchContactsL
// -----------------------------------------------------------------------------
//
 void CVIMPSTStorageServiceView::CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
                                                         RArray <TPtrC> &aServiceField ) 
    {
	 TRACER_AUTO;
    if( !iVPbkContactStore->LocalStore() )
        {
        RemoveAllCacheContactsL();
        }
    iVPbkContactStore->AddVPbkFetchContactsL( aFirstNameList, aServiceField );
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveContactL
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------

TInt CVIMPSTStorageServiceView::RemoveContactL( MVIMPSTStorageContact* aContact ) 
	{
	TRACER_AUTO;
	TInt error = KErrArgument;
	if( aContact )
		{
		TRACE( "contact found" );
		error = iVPbkContactStore->RemoveVPbkContactL( *aContact->ContactLink() );
		}
	return error;	
	}


 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RetriveLinkXSPIdsL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::RetriveLinkXSPIdsL(const TDesC8& aContactPackLink )
    {
	TRACER_AUTO;
    // return the no of retrived xsp of contacts 
    return iVPbkContactStore->RetrieveVPbkXSPIdL( aContactPackLink );
    }
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetRetrieveXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageServiceView::GetRetrieveXSPIdL(TInt aIndex ) 
	{
	TRACER_AUTO;
	TRACE( " aIndex = %d",aIndex  );
	return iVPbkContactStore->GetRetrieveVPbkXSPIdL( aIndex );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateNewContactFromRetrivedIdL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::CreateNewContactFromRetrivedIdL( TInt aIndexToUse )
    {
	TRACER_AUTO;
    TRACE( "aIndexToUse = %d",aIndexToUse  );
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    return iVPbkContactStore->CreateRetriveVPbkContactL( aIndexToUse );    
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::DeleteNewContactFromRetrivedIdL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::DeleteNewContactFromRetrivedIdL( TInt aIndexToUse )
    {
	TRACER_AUTO;
    TRACE( "aIndexToUse = %d",aIndexToUse  );
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    return iVPbkContactStore->deleteRetriveVPbkContactL( aIndexToUse );    
    }

 // ----------------------------------------------------------
// CVIMPSTStorageServiceView::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageServiceView::DeleteDatabaseL()
    {
	TRACER_AUTO;
   	iVPbkContactStore->DeleteDatabaseL();
    }
 
//================================================== from writer interface=======================
 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::UpdateCacheContactL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageServiceView::UpdateCacheContactL(const MVPbkContactLink& aContactLink,
																 const TDesC& aUserId, 
        										   	   	         const TDesC& aDisplayName,
        										   	   	         const TDesC8& aAvatarContent)
    {
	TRACER_AUTO;
    MVIMPSTStorageContact* contact = FindContactByLink(aContactLink);
    if( contact )
	    {
	    TInt oldIndex = IndexOfContact( contact );
	    TBool sortNeeded = EFalse;
	    if( contact->UserId().Compare( aUserId) != 0 )
	        {
	        TRACE( "user id changed" );	        
	        NotifyAllObserversL( TVIMPSTEnums::EStorageEventUserIdPreChange,NULL, contact,oldIndex);
	        contact->SetUserIdL( aUserId );
	        if(aUserId.Length()== 0)
               {
                sortNeeded = ETrue;
               contact->SetOnlineStatus(TVIMPSTEnums::EUnknown);
               }
	        NotifyAllObserversL( TVIMPSTEnums::EStorageEventUserIdPostChange,NULL, contact,oldIndex);
	        TRACE( "NotifyAllObserversL delivered" );
	        }
	    if(contact->Name().Compare(aDisplayName) != 0 )
	        {
            sortNeeded = ETrue;
	        TRACE( "display name changed" );
	        if( aDisplayName.Length() <= 0 && iUnNamedText )
				{
				contact->SetNameL( *iUnNamedText );	
				}
			else
				{
				contact->SetNameL( aDisplayName );	
				}
	        }
	    if(sortNeeded)
	        {
		    TInt index = KErrNotFound;
        	TInt count = iContactListArray.Count();
	     	for(TInt i = 0; i < count; i++)
		        {
		        index = iContactListArray[i]->FindIndexOfContact(contact);
		        if(KErrNotFound != index )
		            {
		            iContactListArray[i]->ResortContact(contact);
		            // get the index of the sorted contact, as after sorting the 
		            // index would have changed based on presence.
		            // break after the contact list is found and the contact is found
		            break;
		            }
		        }
	   NotifyAllObserversL( TVIMPSTEnums::EStorageEventContactChange,NULL,contact, oldIndex );
	        }
	   if( contact->AvatarContent().Compare( aAvatarContent ) != 0 )
	        {
	        TRACE( "User id changed" );           
	        contact->SetAvatarContentL( aAvatarContent,*iVPbkContactStore  );
	        NotifyAllObserversL( TVIMPSTEnums::EStorageAvatarChange,NULL, contact, KErrNotFound);
	        TRACE( " NotifyAllObserversL delivered" );
	        }
	    }
    return contact;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::AddContactToCacheL
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::AddContactToCacheL (const MVPbkContactLink& aContactLink,
																 const TDesC& aUserId, 
        										   	   	         const TDesC& aDisplayName,
        										   	   	         const TDesC8& aAvatarContent,
        										   	   	         TVIMPSTEnums::TVIMPSTStorgaeEventType aType )
	{
	TRACER_AUTO;
	TRACE(" user id: %S", &aUserId);
	TRACE( " storageeventtype: %d", aType );
	MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
	if( !contactList )
		{
		contactList = CreateContactListL(KFriendList,KNullDesC); 
		}
	TRACE(" Before findcontactlink" ); 
	MVIMPSTStorageContact* contact = FindContactByLink(aContactLink);
	TRACE( " After findcontactlink check for findcontactuserid" ); 
	if(!contact && aUserId.Length() )
		{
		MVIMPSTStorageContact* contact = FindContactByUserId( aUserId );
		}
	TRACE( " After findcontactuserid " ); 
	if( !contact )
		{
		TRACE( " contact not exist "); 
		TInt error = KErrGeneral;
		TInt index = KErrNotFound;
		if( aDisplayName.Length() <= 0 && iUnNamedText )
			{
			TRACE(" no display name " ); 
			contact = CVIMPSTStorageContact::NewLC( aUserId, *iUnNamedText , aContactLink ,aAvatarContent );	
			}
		else
			{
			TRACE( " has display name " ); 
			contact = CVIMPSTStorageContact::NewLC( aUserId, aDisplayName , aContactLink, aAvatarContent);		
			}
		// if the xsp id is numm, then the presence state should be EUnknown;
		if(0 == aUserId.Length())
		    {
		    contact->SetOnlineStatus(TVIMPSTEnums::EUnknown);
		    }
		else if( aType == TVIMPSTEnums::EStorageEventContactAddition && !iIsInvitationItem )
	        {
	        // if a invitation item getting accepted do not set to pending again
	        // if user has choosen add contact set the default status as pending
	        // to avoid any kind of flickering
	        contact->SetOnlineStatus(TVIMPSTEnums::EPending);
	        }
		error = contactList->AddStorageContactToCacheL( contact, index );
		TRACE( "AddStorageContactToCacheL error= %d ",error ); 
		if( error == KErrNone )
			{
			TRACE( "success " );
			CleanupStack::Pop(); // contact
			TInt count = iContactListArray.Count();
			for(TInt i = 0; i < count; i++)
				{
				index = iContactListArray[i]->FindIndexOfContact(contact);
				if(KErrNotFound != index )
				    {
				    iContactListArray[i]->ResortContact(contact);
				    TRACE( " contact ResortContact " );
				    // get the index of the sorted contact, as after sorting the 
				    // index would have changed based on presence.
				    // break after the contact list is found and the contact is found
				    break;
				    }
				}
			index = IndexOfContact(contact);
		  	NotifyAllObserversL( aType,NULL, contact, index );	
			}
		else
			{
			TRACE( " not success " );
			CleanupStack::PopAndDestroy(); // contact	
			contact = NULL;
			}
		}
	iIsInvitationItem = EFalse; // reset the flag 
	return contact;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::AddStorageContactToCacheL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::AddStorageContactToCacheL(MVIMPSTStorageContact* aContactToAdd ) 
	{
	TRACER_AUTO;
	TInt error = KErrGeneral;
    MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
    if( !contactList )
        {        
        contactList = CreateContactListL(KFriendList,KNullDesC); // create the default list :TODO chnage to proper sol
        }
    if( aContactToAdd )
		{
		if( aContactToAdd->Name().Length() <= 0 && iUnNamedText )
			{
			aContactToAdd->SetNameL( *iUnNamedText );	
			}
		TInt index = KErrNotFound;
        TRACE( " online status = %d ", TVIMPSTEnums::EPending);
        // if the userid(xsp field is null then the presence is unknown.)
        if(aContactToAdd->UserId().Length() == 0)
            {
            aContactToAdd->SetOnlineStatus(TVIMPSTEnums::EUnknown); 
            }
        else
            {
            aContactToAdd->SetOnlineStatus(TVIMPSTEnums::EPending);    
            }
		error = contactList->AddStorageContactToCacheL( aContactToAdd, index ); // aContactToAdd ownership is transfered
		TRACE( "index = %d ",index );
		if( error == KErrNone )
			{
			TInt count = iContactListArray.Count();
			for(TInt i = 0; i < count; i++)
				{
				index = iContactListArray[i]->FindIndexOfContact(aContactToAdd);
				if(KErrNotFound != index )
					{
					iContactListArray[i]->ResortContact(aContactToAdd);
					TRACE( "contact ResortContact " );
					// get the index of the sorted contact, as after sorting the 
					// index would have changed based on presence.
					// break after the contact list is found and the contact is found
					break;
					}
				}
			index = IndexOfContact(aContactToAdd);
			NotifyAllObserversL( TVIMPSTEnums::EStorageEventContactAddition,NULL, aContactToAdd, index );	
			}
		}
	TRACE( " error = %d ",error );
	return error;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveContactFromCacheL
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------

TInt CVIMPSTStorageServiceView::RemoveContactFromCacheL(const MVPbkContactLink& aContactLink,
														 TVIMPSTEnums::TVIMPSTStorgaeEventType aType) 
	{
	TRACER_AUTO;
	TInt error = KErrNotFound;
	MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
    if( !contactList )
        {        
        contactList = CreateContactListL( KFriendList,KNullDesC ); // create the default list :TODO chnage to proper sol
        }
	MVIMPSTStorageContact* contact = FindContactByLink(aContactLink);
    if( contact )
		{
		// do not change the order of below line
		// contact can be access just before deletion but not after delete from cache
		if( aType == TVIMPSTEnums::EStorageEventDeleteFromPbk )
			{
			// in EStorageEventDeleteFromPbk event ,caller should not remove from his local list
			// this just to inform that if user caller want to do any pre operation like unsubscriibtion etc
			NotifyAllObserversL( aType ,NULL,contact,0); //contact is still valid	
			}
		TInt index = 0;
	    error = contactList->RemoveContactFromCacheL( aContactLink, index );
	    NotifyAllObserversL( TVIMPSTEnums::EStorageEventContactDelete ,NULL,NULL,index); //contact is not valid ,deleted
		}
	TRACE("error = %d ",error );
    return error;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveAllCacheContactsL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::RemoveAllCacheContactsL()
    {
	TRACER_AUTO;
    TRACE( "  count = %d", iContactListArray.Count());
    iContactListArray.ResetAndDestroy();
    TRACE( " remove sucess" );
    NotifyAllObserversL(TVIMPSTEnums::EStorageAllContactRemoved,NULL,NULL,0 );
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetDefaultContactListL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* CVIMPSTStorageServiceView::GetDefaultContactListL()
    {
	TRACER_AUTO;
    MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
    return contactList;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::NotifyServiceViewL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::NotifyServiceViewL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
													MVIMPSTStorageContact* aContact /*= NULL */ )
    {
	TRACER_AUTO;
    NotifyAllObserversL(aEventType,NULL, aContact,0 ); 
    }
 
 //================================================== from writer interface end ===========================
// TLinearOrder
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ContactListOrderByDisplayName
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::ContactListOrderByDisplayName(
                                       const CVIMPSTStorageContactList& aContactListA, 
                                       const CVIMPSTStorageContactList& aContactListB )
    {
	TRACER_AUTO;
    return aContactListA.DisplayName().CompareC( aContactListB.DisplayName() );
    }

// TIdentityRelation
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ContactListFindByContactListId 
// -----------------------------------------------------------------------------
//
TBool CVIMPSTStorageServiceView::ContactListFindByContactListId( 
                                       const CVIMPSTStorageContactList& aContactListA, 
                                       const CVIMPSTStorageContactList& aContactListB )
    {
	TRACER_AUTO;
    const MVIMPSTStorageContactList& listA = aContactListA;
    const MVIMPSTStorageContactList& listB = aContactListB;
   TRACE(" return" );
    return ( VIMPSTStorageUtils::NeutralCompare( listA.ListId(), 
                                      listB.ListId(), EFalse ) == 0 );
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::Count
// Returns item count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::Count() const
	{
	TRACER_AUTO;
	// fetch contact list count from storage
	// and add contact counts from expanded contact lists
	TInt listCount( ListCount() );
	TInt itemCount( 0 );
	TInt contactCount = 0;
	for( TInt i(0); i<listCount; ++i )
	    {
	    MVIMPSTStorageContactList& list = ListAt(i);
        contactCount = list.Count() ;
		 ++itemCount;
	    if( contactCount > 0 )
			{
			// if list is expanded, add count of count of contacts
			// in this list to item count
			itemCount += contactCount;
		   //check for this
			}
	    }
	TRACE( "itemCount =%d",itemCount );
   	return itemCount;
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::Item
// Returns item at given index
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MVIMPSTStorageItemModel::SItem CVIMPSTStorageServiceView::Item( TInt aIndex  ) const
    {
	TRACER_AUTO;
    MVIMPSTStorageItemModel::SItem item;
    item.iType = MVIMPSTStorageItemModel::EInvalid;
    item.iContactList = NULL;
    item.iContact = NULL;
	TInt contactCount = 0;
    TInt count( 0 );
    TInt listCount( ListCount() );
	for( TInt i( 0 ); i < listCount; ++i )
	    {
	    MVIMPSTStorageContactList& list = ListAt( i );
	    item.iContactList = &list;

        ++count;
	    if( count > aIndex )
	        {
	        // there's a list in given index
	        item.iType = MVIMPSTStorageItemModel::EContactList;
	        TRACE( "Item end" );
	        return item;
	        }
	    contactCount = list.Count() ;
        if( contactCount > 0  )
			{
			// list is expanded, consider also contacts in this list
			if( count + contactCount > aIndex )
			    {
			    // there's a contact in given index
			    TInt contactIndex( aIndex - count );
			    item.iType = MVIMPSTStorageItemModel::EContactItem;
			    item.iContact = &list[contactIndex];
			    TRACE( "Item end" );
			    return item;
			    }
			// add expanded list's contact count
			count += contactCount;
			}
	    }
    // not found
    return item;
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::IndexOfContact
// Gets index of contact in list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::IndexOfContact( MVIMPSTStorageContact* aContact ) const
	{
	TRACER_AUTO;
	TInt listCount( ListCount() );
	TInt itemCount( 0 );
	TInt contactCount = 0;
	TInt indexOfContact = KErrNotFound;
	for( TInt i(0); i < listCount; ++i )
		{
		MVIMPSTStorageContactList& list = ListAt(i);
		contactCount = list.Count( ) ;
		indexOfContact = list.FindIndexOfContact( aContact );
		// if we're showing all contacts or
		// there are online contacts in this list,
		// the list-item is shown => increase count  check this?
		//itemCount; //++itemCount;
		if( indexOfContact != KErrNotFound )
			{
			// add earlier items to index.
			indexOfContact += itemCount;
			break;
			}
		else
			{	
			itemCount += contactCount;
			}
		//add the code for if contact not found in first list
		}
	TRACE( " indexOfContact = %d", indexOfContact );
	return indexOfContact;
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::IndexOfList
// Gets index of contact in list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::IndexOfList( MVIMPSTStorageContactList* aList,
                                       TBool /*aIgnoreOwnItem = EFalse*/,
                                       TBool /*aIgnoreEmptyLists = ETrue*/ ) const
    {
	TRACER_AUTO;
	TInt listCount( ListCount() );
   	TInt itemCount( 0 );
	for( TInt i(0); i<listCount; ++i )
	    {
	    MVIMPSTStorageContactList& list = ListAt(i);
	    if( &list == aList )
	        {
	        TRACE( "list matched");
	        break;
	        }
	    else
		    {
		    // list will be consider for calculating the index
	    	// at initial state of fetching the list.count will be zero
	    	// because all the lists will be fetched first and the contacts
	    	itemCount = itemCount+ list.Count() + 1;	
		    }
	    }
	TRACE( "itemCount = %d",itemCount );
	return itemCount;
    }
// -----------------------------------------------------------------------------
// CVIMPSTContactListModel::MdcaCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::MdcaCount() const
    {
	TRACER_AUTO;
    return Count();
    }

// -----------------------------------------------------------------------------
// CVIMPSTContactListModel::MdcaPoint
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTStorageServiceView::MdcaPoint( TInt /*aIndex */) const
    {
	TRACER_AUTO;
    // These will be filtered out
    return KNullDesC();
    } 
 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::Sort
/// KNullDesC -> sort all
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::Sort( const TDesC& aContactListId /* = KNullDesC */ )
    {
	TRACER_AUTO;
    if( aContactListId.Length() != 0 )
        {
        MVIMPSTStorageContactList* list = FindContactList( aContactListId );
        if( list )
            {
            TRACE( " list sort" );
            list->Sort();
            }
        }
    else
        {
        TInt count( iContactListArray.Count() );
        for( TInt a( 0 ); a < count; ++a )
            {
            MVIMPSTStorageContactList& list = *iContactListArray[ a ];
            TRACE( " all list sort" );
            list.Sort();
            }
        } 
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::NotifyAllObservers
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::NotifyAllObserversL( TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
					        						  MVIMPSTStorageContactList *aList, 
					                                  MVIMPSTStorageContact* aContact,
					                                  TInt aContactIndex )
    {
	TRACER_AUTO;
	TInt count = iContactObservers.Count();
	for( TInt i=0; i<count; i++ )
		{
		iContactObservers[i]->HandleStorageChangeL(  aType, aList, aContact, aContactIndex );	
		}
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::NotifyAllObserversWithDelay
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::NotifyAllObserversWithDelay( TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
							        						 MVIMPSTStorageContactList *aList, 
							                                 MVIMPSTStorageContact* aContact,
							                                 TInt aContactIndex )
	{
	TRACER_AUTO;
    if(iActiveHandler->IsActive() )
        {
        iActiveHandler->Cancel();
        }
    iActiveHandler->IssueRequest( aType, aList, aContact, aContactIndex );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::HandleDelayedNotificationL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::HandleDelayedNotificationL(TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
														   MVIMPSTStorageContactList *aList,
								                           MVIMPSTStorageContact* aContact,
								                           TInt aContactIndex) 
	{
	TRACER_AUTO;
	NotifyAllObserversL( aType, aList, aContact, aContactIndex );
	}
//  End of File
