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
#include "vimpstdebugtrace.h"
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::CVIMPSTStorageServiceView()") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::ConstructL() begin") );
    
    iActiveHandler =  CVIMPSTStorageActiveHandler::NewL(this);
					 
    // create sorter for contacts
    iContactSorter = CVIMPSTStorageContactSorter::NewL();
    
    //set the sorting algorithem to order by presence.
    iContactSorter->SetSortAlgorithm( MVIMPSTStorageContact::ECompareByPresence);
    
    TRACE( T_LIT("SubServiceType() Type : %S"), &aStoreName );
    //find whether its a local store or xsp store
	if ( VPbkContactStoreUris::DefaultCntDbUri().CompareC( aStoreName, 1, NULL )
	                        != 0 )
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::ConstructL() service store created") ); 
		//Create XSP Store 
    	iVPbkContactStore = 
    			CVIMPSTStorageVPbkServerStore::NewL(aStoreName,aServiceName ,*this );
		}
	else
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::ConstructL() local store created ") ); 
		//Create Local Contacts.cdb store 
		iVPbkContactStore = CVIMPSTStorageVPbkLocalStore::NewL(VPbkContactStoreUris::DefaultCntDbUri(), 
															   aServiceName,
															   *this );	
		}
    TRACE( T_LIT("CVIMPSTStorageServiceView::ConstructL() end") );;
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::NewL() begin") );
    
    CVIMPSTStorageServiceView* self = new( ELeave ) CVIMPSTStorageServiceView(aServiceId);
    CleanupStack::PushL( self );
    self->ConstructL(aStoreName, aServiceName);
    CleanupStack::Pop( self );
    TRACE( T_LIT("CVIMPSTStorageServiceView::NewL() end") );
    return self;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView
// Two-phased constructor.
// -----------------------------------------------------------------------------
// Destructor
CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView()
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView() begin") );
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
        
    TRACE( T_LIT("CVIMPSTStorageServiceView::~CVIMPSTStorageServiceView() end") );
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::AddObserverL
// From MVIMPSTStorageServiceView
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::AddObserverL( MVIMPSTStorageContactsObserver*
        aObserver )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::AddObserverL() begin") );
    User::LeaveIfError( aObserver ? KErrNone : KErrArgument );
    TInt index = iContactObservers.Find( aObserver );
    if( index == KErrNotFound )
	    {
	    TRACE( T_LIT("CVIMPSTStorageServiceView::AddObserverL() aObserver added ") );
	    User::LeaveIfError( iContactObservers.Append( aObserver ) );	
	    }
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddObserverL() end") );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveObserverL
// From MVIMPSTStorageServiceView
// -----------------------------------------------------------------------------
void CVIMPSTStorageServiceView::RemoveObserver( MVIMPSTStorageContactsObserver*
        aObserver )
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveObserver() begin") );
	__ASSERT_ALWAYS( aObserver, Panic( EObserverIsNull ));
	const TInt index( iContactObservers.Find( aObserver ) );
	if ( index >=0)
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveObserver() aObserver found") );
		iContactObservers.Remove( index );
		iContactObservers.Compress();
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveObserver() begin") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateContactListL() begin") );
    CVIMPSTStorageContactList* contactList = 
    						CVIMPSTStorageContactList::NewLC(*iContactSorter,
                                                         	  aContactListId, 
                                                         	  aDisplayName );
                                                         
    TIdentityRelation< CVIMPSTStorageContactList > findBy( 
                                CVIMPSTStorageServiceView::ContactListFindByContactListId);
    TInt indexOfList( iContactListArray.Find( contactList, findBy ) );
    if( indexOfList == KErrNotFound )
        {
        TRACE( T_LIT("CVIMPSTStorageServiceView::CreateContactListL() list not exist in list") );
        TLinearOrder< CVIMPSTStorageContactList > order( 
                             CVIMPSTStorageServiceView::ContactListOrderByDisplayName );
        iContactListArray.InsertInOrderAllowRepeats( contactList, order );
        CleanupStack::Pop( contactList ); 
        TRACE( T_LIT("CVIMPSTStorageServiceView::CreateContactListL() new list created") );       
        }
    else
        {
        TRACE( T_LIT("CVIMPSTStorageServiceView::CreateContactListL() list  already exist in list") );
        CleanupStack::PopAndDestroy( contactList );
        contactList = iContactListArray[ indexOfList ];
        }
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateContactListL() end") );
    return contactList; 
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveContactList
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::RemoveContactList( const TDesC& aContactListId )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactList() begin") );
    TInt pos( FindContactListById( aContactListId ) );
    if( pos >= 0 )
        {
        TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactList() aContactListId found") );
        delete iContactListArray[ pos ];
        iContactListArray.Remove( pos );  
        iContactListArray.Compress();      
        }
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactList() end") );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ListCount
// Count of contactlists
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::ListCount() const
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListCount()") );
    return iContactListArray.Count();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ListAt
// Reference to contact list
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList& CVIMPSTStorageServiceView::ListAt( TInt aIndex ) const
    {   
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListAt() begin") );
    // User::LeaveIfError( aIndex < iContactListArray.Count() ? KErrNone : KErrArgument ); 
    __ASSERT_ALWAYS( (aIndex < iContactListArray.Count() && aIndex >=0 ), Panic( EContactsArrayOutOfSync ));  
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListAt() end") );
    return *iContactListArray[ aIndex ];
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::ContactCount
// Count of contacts
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::ContactCount( TBool aSkipOfflineContacts ) const
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListAt() begin") );
    TInt contactCount( 0 );
    TInt count( iContactListArray.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
        contactCount += iContactListArray[ a ]->ContactCount( aSkipOfflineContacts, 
                                                         EFalse );
        }
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListAt() contactCount = %d") ,contactCount );
    TRACE( T_LIT("CVIMPSTStorageServiceView::ListAt() end") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() begin") );
	MVIMPSTStorageContact* contact = NULL;
	if( iOwnContact && ( aContactId.Compare( iOwnContact->UserId() ) == 0  ) )
	    {
	    TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() own Item presence") );
	    if( aAvatarData.Compare(iOwnContact->AvatarContent() ) != 0 )
	        {
	        if((aIsClearingAvatar)||(aAvatarData.Length() > 0))
	            {
	            TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() own avatar change NotifyAllObserversL") );
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
		    TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() own presene NotifyAllObserversL") );
		    NotifyAllObserversL(TVIMPSTEnums::EStorageOwnPresenceChange,NULL, iOwnContact,0);
		    }
	    contact = iOwnContact;
		}
	else
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact item ") );
		contact = FindContactByUserId( aContactId );
		if(contact)
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact exist ") );
			TInt index = KErrNotFound;
			TVIMPSTEnums::TOnlineStatus oldOnlineStatus = contact->OnlineStatus();
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact oldOnlineStatus = %d"), oldOnlineStatus );
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact new presence = %d"), aStatus );
			//status text can be different for the sam status hence not inside the if condition.
			if(  oldOnlineStatus != aStatus )
			    {
			    TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact presence change ") );
			    contact->SetOnlineStatus( aStatus );
			    TInt count = iContactListArray.Count();
			    for(TInt i = 0; i < count; i++)
			        {
			        index = iContactListArray[i]->FindIndexOfContact(contact);
			        if(KErrNotFound != index )
			            {
			            iContactListArray[i]->ResortContact(contact);
			            TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() contact ResortContact ") );
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
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() aStatusText = %S "), &aStatusText );
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() NotifyAllObserversL called") );
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() NotifyAllObserversL finish") );	
			}
	 	}
	TRACE( T_LIT("CVIMPSTStorageServiceView::UpdatePresenceL() end") );
    return contact;
    }
 
 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::UpdateAvatarL
// -----------------------------------------------------------------------------
//
 MVIMPSTStorageContact* CVIMPSTStorageServiceView::UpdateAvatarL(const TDesC& aContactId,
							                  const TDesC8& aAvatarData ) 
 {
 TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() begin") );
 MVIMPSTStorageContact* contact = NULL;
 if( iOwnContact && ( aContactId.Compare( iOwnContact->UserId() ) == 0  ) )
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() own avatar") );
		iOwnContact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);	
		NotifyAllObserversL(TVIMPSTEnums::EStorageOwnPresenceChange,NULL, iOwnContact,0);
	    contact = iOwnContact;
		}
	else
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() buddy avatar") );
	    contact = FindContactByUserId( aContactId );
		if(contact)
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() contact exit") );
			contact->SetAvatarContentL( aAvatarData ,*iVPbkContactStore);	
			TInt index = IndexOfContact(contact);
		    NotifyAllObserversL(TVIMPSTEnums::EStorageAvatarChange,NULL, contact,index);
		    TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() NotifyAllObserversL called") );		
			}
	   	}
   TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateAvatarL() end") );
   return contact;
   }
           
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListInternal
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContactList* CVIMPSTStorageServiceView::FindContactListInternal( 
                                                const TDesC& aListId )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactListInternal begin") );    
	TInt contactListIndex( FindContactListById( aListId ) );
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactListInternal end ") );
    return ( contactListIndex >= 0 ? iContactListArray[ contactListIndex ] : NULL );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* CVIMPSTStorageServiceView::FindContactList( const TDesC& aListId )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactList") ); 
    return FindContactListInternal( aListId );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListById
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::FindContactListById(const TDesC& aListId )
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactList begin") );
	TInt index = KErrNotFound;
	TInt count( iContactListArray.Count() );
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactList count = %d"), count );
	for( TInt i= 0  ; i < count ; i++ )
		{
		if( KErrNone == VIMPSTStorageUtils::NeutralCompare( 
							 aListId, 
                             iContactListArray[ i ]->ListId(), EFalse )   )
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactList contact list found") );
			index = i;
			break;
			}
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactList end") );
	return index;
	}
	
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactListById
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindContactByLink(const MVPbkContactLink& aContactLink )
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByLink begin") );
	MVIMPSTStorageContact* contact = NULL;
	TInt count( iContactListArray.Count() );
    for( TInt i= 0  ; i < count ; i++ )
        {
        CVIMPSTStorageContactList* contactList = iContactListArray[ i ];
        contact = contactList->FindContactByContactLink( aContactLink );
        if( contact )
            { 
            TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByLink contact found = %d "), i );           
            break;
            }
        }
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByLink end") );
    return contact;
  	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetServiceId
// -----------------------------------------------------------------------------
//
TUint32 CVIMPSTStorageServiceView::GetServiceId()
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::GetServiceId") );
    return iServiceId;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::LocalStore
// -----------------------------------------------------------------------------
//
TBool CVIMPSTStorageServiceView::IsLocalStore() const
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::IsLocalStore") );
	return iVPbkContactStore->LocalStore();	
	}  
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::OwnContactL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& CVIMPSTStorageServiceView::OwnContactL()
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::OwnContact begin") );
	if( !iOwnContact )
		{
		iOwnContact = CVIMPSTStorageContact::NewL(KNullDesC,
		        								  KNullDesC )	;	
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::OwnContact end") );
	return *iOwnContact;	
	} 

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::SetOwnUserIdL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::SetOwnUserIdL(const TDesC& aUserId ) 
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::SetOwnUserIdL begin") );
	if( !iOwnContact )
		{
		iOwnContact = CVIMPSTStorageContact::NewL(aUserId,
		        								  KNullDesC
		        								  )	;
		TRACE( T_LIT("CVIMPSTStorageServiceView::SetOwnUserIdL iOwnContact created") );
		}
	else if( aUserId.Compare( iOwnContact->UserId() ) != 0 )
		{
	    _LIT (KNullWithSpace, " ");
		TRACE( T_LIT("CVIMPSTStorageServiceView::SetOwnUserIdL iOwnContact was existing") );
	    if(KNullWithSpace ().Compare(iOwnContact->UserId())!=0)
	        { 
	        iVPbkContactStore->RemoveAllVPbkContactsL(); // if user id is changed remove all contacts 	
	        RemoveAllCacheContactsL();
	        }
		iOwnContact->SetUserIdL( aUserId );
		iOwnContact->SetAvatarContentL(KNullDesC8,*iVPbkContactStore );

		NotifyAllObserversL( TVIMPSTEnums::EStorageEventOwnUserChanged,NULL,iOwnContact,0 );
		TRACE( T_LIT("CVIMPSTStorageServiceView::SetOwnUserIdL NotifyAllObserversL delivered") );
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::SetOwnUserIdL end") );
	}    
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactByUserId
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindContactByUserId( const TDesC& aUserId )
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByUserId begin") );
	MVIMPSTStorageContact* contact = NULL;
	CVIMPSTStorageContactList* contactList = NULL;
	TInt count( iContactListArray.Count() );
    for( TInt a( 0 ); a < count; ++a )
        {
        contactList = iContactListArray[ a ];
        contact = contactList->FindContact( aUserId );
        if( contact )
            {  
            TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByUserId contact found ") );          
            break;
            }
        }
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindContactByUserId end") );
    return contact;
  	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindContactByUserId
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindCacheContactByUserId( const TDesC& aUserId )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindCacheContactByUserId") );
    return FindContactByUserId( aUserId );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::FindCacheContactByLink
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* CVIMPSTStorageServiceView::FindCacheContactByLink(const MVPbkContactLink& aContactLink )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::FindCacheContactByLink ") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewContactL begin") );
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    MVIMPSTStorageContact* contact = FindCacheContactByUserId( aUserId );
    if( !contact )
	    {
	    iIsInvitationItem = aIsInvitationItem;
	    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewContactL contact does not exist in list") );
	    iVPbkContactStore->CreateVPbkContactL( aUserId ,aDisplayName, aInvitationAutoAccept );	
	    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewContactL new contact created") );
	    }
	TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewContactL end") );
    return contact;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateNewFetchContactsL
// -----------------------------------------------------------------------------
//
 void CVIMPSTStorageServiceView::CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
                                                         RArray <TPtrC> &aServiceField ) 
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewFetchContactsL begin") );
    if( !iVPbkContactStore->LocalStore() )
        {
        RemoveAllCacheContactsL();
        }
    iVPbkContactStore->AddVPbkFetchContactsL( aFirstNameList, aServiceField );
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewFetchContactsL end") );
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveContactL
// From MVIMPSTStorageServiceView.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------

TInt CVIMPSTStorageServiceView::RemoveContactL( MVIMPSTStorageContact* aContact ) 
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactL begin") );
	TInt error = KErrArgument;
	if( aContact )
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactL contact found") );
		error = iVPbkContactStore->RemoveVPbkContactL( *aContact->ContactLink() );
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactL end") );
	return error;	
	}


 // -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RetriveLinkXSPIdsL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::RetriveLinkXSPIdsL(const TDesC8& aContactPackLink )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::RetriveLinkXSPIdsL") );
    // return the no of retrived xsp of contacts 
    return iVPbkContactStore->RetrieveVPbkXSPIdL( aContactPackLink );
    }
 
 // ---------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetRetrieveXSPIdL
// ---------------------------------------------------------------------------
//  
const TDesC& CVIMPSTStorageServiceView::GetRetrieveXSPIdL(TInt aIndex ) 
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::GetRetrieveXSPIdL aIndex = %d"),aIndex  );
	return iVPbkContactStore->GetRetrieveVPbkXSPIdL( aIndex );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::CreateNewContactFromRetrivedIdL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::CreateNewContactFromRetrivedIdL( TInt aIndexToUse )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::CreateNewContactFromRetrivedIdL aIndexToUse = %d"),aIndexToUse  );
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    return iVPbkContactStore->CreateRetriveVPbkContactL( aIndexToUse );    
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::DeleteNewContactFromRetrivedIdL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::DeleteNewContactFromRetrivedIdL( TInt aIndexToUse )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::DeleteNewContactFromRetrivedIdL aIndexToUse = %d"),aIndexToUse  );
    // This will create a contact in the CDB file.  It returns MVPbkContactLink link to the CDB file.
    return iVPbkContactStore->deleteRetriveVPbkContactL( aIndexToUse );    
    }

 // ----------------------------------------------------------
// CVIMPSTStorageServiceView::DeleteDatabaseL
// ----------------------------------------------------------
void CVIMPSTStorageServiceView::DeleteDatabaseL()
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::DeleteDatabaseL") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL begin") );
    MVIMPSTStorageContact* contact = FindContactByLink(aContactLink);
    if( contact )
	    {
	    TInt oldIndex = IndexOfContact( contact );
	    TBool sortNeeded = EFalse;
	    if( contact->UserId().Compare( aUserId) != 0 )
	        {
	        TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL user id changed") );	        
	        NotifyAllObserversL( TVIMPSTEnums::EStorageEventUserIdPreChange,NULL, contact,oldIndex);
	        contact->SetUserIdL( aUserId );
	        if(aUserId.Length()== 0)
               {
                sortNeeded = ETrue;
               contact->SetOnlineStatus(TVIMPSTEnums::EUnknown);
               }
	        NotifyAllObserversL( TVIMPSTEnums::EStorageEventUserIdPostChange,NULL, contact,oldIndex);
	        TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL NotifyAllObserversL delivered") );
	        }
	    if(contact->Name().Compare(aDisplayName) != 0 )
	        {
            sortNeeded = ETrue;
	        TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL display name changed") );
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
	        TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL user id changed") );           
	        contact->SetAvatarContentL( aAvatarContent,*iVPbkContactStore  );
	        NotifyAllObserversL( TVIMPSTEnums::EStorageAvatarChange,NULL, contact, KErrNotFound);
	        TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL NotifyAllObserversL delivered") );
	        }
	    }
	TRACE( T_LIT("CVIMPSTStorageServiceView::UpdateCacheContactL end") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL begin") );
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL user id: %S"), &aUserId);
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL storageeventtype: %d"), aType );
	MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
	if( !contactList )
		{
		contactList = CreateContactListL(KFriendList,KNullDesC); 
		}
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL before findcontactlink") ); 
	MVIMPSTStorageContact* contact = FindContactByLink(aContactLink);
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL after findcontactlink ") ); 
	if( !contact )
		{
		TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL contact not exist ") ); 
		TInt error = KErrGeneral;
		TInt index = KErrNotFound;
		if( aDisplayName.Length() <= 0 && iUnNamedText )
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL no display name ") ); 
			contact = CVIMPSTStorageContact::NewLC( aUserId, *iUnNamedText , aContactLink ,aAvatarContent );	
			}
		else
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL has display name ") ); 
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
		TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL AddStorageContactToCacheL error= %d "),error ); 
		if( error == KErrNone )
			{
			TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL success ") );
			CleanupStack::Pop(); // contact
			TInt count = iContactListArray.Count();
			for(TInt i = 0; i < count; i++)
				{
				index = iContactListArray[i]->FindIndexOfContact(contact);
				if(KErrNotFound != index )
				    {
				    iContactListArray[i]->ResortContact(contact);
				    TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL() contact ResortContact ") );
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
			TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL not success ") );
			CleanupStack::PopAndDestroy(); // contact	
			contact = NULL;
			}
		}
	iIsInvitationItem = EFalse; // reset the flag 
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL end") );
	return contact;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::AddStorageContactToCacheL
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::AddStorageContactToCacheL(MVIMPSTStorageContact* aContactToAdd ) 
	{
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddStorageContactToCacheL begin") );
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
        TRACE( T_LIT("CVIMPSTStorageServiceView::AddStorageContactToCacheL online status = %d "), TVIMPSTEnums::EPending);
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
		TRACE( T_LIT("CVIMPSTStorageServiceView::AddStorageContactToCacheL index = %d "),index );
		if( error == KErrNone )
			{
			TInt count = iContactListArray.Count();
			for(TInt i = 0; i < count; i++)
				{
				index = iContactListArray[i]->FindIndexOfContact(aContactToAdd);
				if(KErrNotFound != index )
					{
					iContactListArray[i]->ResortContact(aContactToAdd);
					TRACE( T_LIT("CVIMPSTStorageServiceView::AddContactToCacheL() contact ResortContact ") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddStorageContactToCacheL error = %d "),error );
	TRACE( T_LIT("CVIMPSTStorageServiceView::AddStorageContactToCacheL end") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL begin") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL error = %d "),error );
	TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL begin") );
    return error;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::RemoveAllCacheContactsL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::RemoveAllCacheContactsL()
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL begin") );
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL  count = %d"), iContactListArray.Count());
    iContactListArray.ResetAndDestroy();
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL remove sucess") );
    NotifyAllObserversL(TVIMPSTEnums::EStorageAllContactRemoved,NULL,NULL,0 );
    TRACE( T_LIT("CVIMPSTStorageServiceView::RemoveContactFromCacheL begin") );
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::GetDefaultContactListL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* CVIMPSTStorageServiceView::GetDefaultContactListL()
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::GetDefaultContactListL begin") );
    MVIMPSTStorageContactList* contactList = FindContactListInternal( KFriendList );
    TRACE( T_LIT("CVIMPSTStorageServiceView::GetDefaultContactListL end") );
    return contactList;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageServiceView::NotifyServiceViewL
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageServiceView::NotifyServiceViewL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
													MVIMPSTStorageContact* aContact /*= NULL */ )
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyServiceViewL begin") );
    NotifyAllObserversL(aEventType,NULL, aContact,0 ); 
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyServiceViewL end") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::ContactListOrderByDisplayName") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::ContactListFindByContactListId start") );
    const MVIMPSTStorageContactList& listA = aContactListA;
    const MVIMPSTStorageContactList& listB = aContactListB;
    TRACE( T_LIT("CVIMPSTStorageServiceView::ContactListFindByContactListId return") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::Count start") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::itemCount =%d"),itemCount );
	TRACE( T_LIT("CVIMPSTStorageServiceView::Count end") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::Item start") );
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
	        TRACE( T_LIT("CVIMPSTStorageServiceView::Item end") );
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
			    TRACE( T_LIT("CVIMPSTStorageServiceView::Item end") );
			    return item;
			    }
			// add expanded list's contact count
			count += contactCount;
			}
	    }
    // not found
    TRACE( T_LIT("CVIMPSTStorageServiceView::Item end") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfContact begin") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfContact indexOfContact = %d"), indexOfContact );
	TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfContact end") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfList begin") );
	TInt listCount( ListCount() );
   	TInt itemCount( 0 );
	for( TInt i(0); i<listCount; ++i )
	    {
	    MVIMPSTStorageContactList& list = ListAt(i);
	    if( &list == aList )
	        {
	        TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfList list matched"));
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfList itemCount = %d"),itemCount );
	TRACE( T_LIT("CVIMPSTStorageServiceView::IndexOfList end") );
	return itemCount;
    }
// -----------------------------------------------------------------------------
// CVIMPSTContactListModel::MdcaCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageServiceView::MdcaCount() const
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::MdcaCount") );
    return Count();
    }

// -----------------------------------------------------------------------------
// CVIMPSTContactListModel::MdcaPoint
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTStorageServiceView::MdcaPoint( TInt /*aIndex */) const
    {
    TRACE( T_LIT("CVIMPSTStorageServiceView::MdcaPoint") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::Sort() begin") );
    if( aContactListId.Length() != 0 )
        {
        MVIMPSTStorageContactList* list = FindContactList( aContactListId );
        if( list )
            {
            TRACE( T_LIT("CVIMPSTStorageServiceView::Sort() list sort") );
            list->Sort();
            }
        }
    else
        {
        TInt count( iContactListArray.Count() );
        for( TInt a( 0 ); a < count; ++a )
            {
            MVIMPSTStorageContactList& list = *iContactListArray[ a ];
            TRACE( T_LIT("CVIMPSTStorageServiceView::Sort() all list sort") );
            list.Sort();
            }
        } 
    TRACE( T_LIT("CVIMPSTStorageServiceView::Sort() end") );   
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyAllObserversL() begin") );
	TInt count = iContactObservers.Count();
	for( TInt i=0; i<count; i++ )
		{
		iContactObservers[i]->HandleStorageChangeL(  aType, aList, aContact, aContactIndex );	
		}
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyAllObserversL() end") );
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
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyAllObserversWithDelay() begin") );
    if(iActiveHandler->IsActive() )
        {
        iActiveHandler->Cancel();
        }
    iActiveHandler->IssueRequest( aType, aList, aContact, aContactIndex );
    TRACE( T_LIT("CVIMPSTStorageServiceView::NotifyAllObserversWithDelay() end") );
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
	TRACE( T_LIT("CVIMPSTStorageServiceView::HandleDelayedNotificationL() begin") );
	NotifyAllObserversL( aType, aList, aContact, aContactIndex );
	TRACE( T_LIT("CVIMPSTStorageServiceView::HandleDelayedNotificationL() end") );	
	}
//  End of File
