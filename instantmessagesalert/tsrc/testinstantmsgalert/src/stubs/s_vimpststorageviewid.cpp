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
* Description:  Storage view implementation
 *
*/


// INCLUDE FILES
#include "s_vimpststorageviewid.h"
   
#include "MVIMPSTStorageContactList.h"


// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::AddContactListIdL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* MyMVIMPSTStorageViewIdStub::AddContactListIdL( 
										   const TDesC& /*aContactListId*/, 
                                           const TDesC& /*aDisplayName*/,
                                           TBool /*aNotify*/ )
    {
    //CVIMPSTStorageContactList* contactList = CVIMPSTStorageContactList::NewL();
   
    return NULL;     
    }



// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::RemoveContactList
// -----------------------------------------------------------------------------
//
void MyMVIMPSTStorageViewIdStub::RemoveContactList( const TDesC& /*aContactListId*/ )
    {
   	
    }

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::RemoveContact
// From MVIMPSTStorageContacts.
// -----------------------------------------------------------------------------
void MyMVIMPSTStorageViewIdStub::RemoveContact( const TDesC& /*aListId*/, 
                                       const TDesC& /*aContactId*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::FindContact
// From MVIMPSTStorageContacts.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindContact( const TDesC& /*aContactListId*/, 
                                                  const TDesC& /*aId*/ )
    {
    return NULL;        
    }

MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindContact( const TDesC& /*aContactListId*/, 
                                                  const TDesC& /*aId*/ ,
											   MVIMPSTStorageContactList*& /*aContactList*/ )
	{
    return NULL;     	
	}
											    
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::ContactInAnyList
// From MVIMPSTStorageContacts.
// -----------------------------------------------------------------------------
TBool MyMVIMPSTStorageViewIdStub::ContactInAllLists( const TDesC& /*aContactId*/ ) const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::FindContact
// From MVIMPSTStorageContacts.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindAnyContact( const TDesC& /*aContactId*/ )
    {
    return NULL;
    }
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::FindContact
// From MVIMPSTStorageContacts.
// Try to load with given ID, return NULL if not found.
// -----------------------------------------------------------------------------
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindAnyContactByName( const TDesC& /*aContactId*/ )
    {
    return NULL;
    }


// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::ListCount
// Count of contactlists
// -----------------------------------------------------------------------------
//
TInt MyMVIMPSTStorageViewIdStub::ListCount() const
    {
    return 1;
    }


// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::ListAt
// Reference to contact list
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList& MyMVIMPSTStorageViewIdStub::ListAt( TInt /*aIndex*/ ) const
    {
    // FIXME ASSERT
	MVIMPSTStorageContactList *abc = NULL;
	
    return *abc;
    }
    
  
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::ContactCount
// Count of contacts
// -----------------------------------------------------------------------------
//
TInt MyMVIMPSTStorageViewIdStub::ContactCount( TBool /*aSkipOfflineContacts*/ ) const
    {
    TInt contactCount( 0 );
    
    return contactCount;
    }


// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::CreateContactL
// From MVIMPSTExtendedStorageContact
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::CreateContactL(
        const TDesC& /*aContactList*/,
        const TDesC& /*aWVID*/,
        MVPbkContactLink* /*aContactLink*/,
		CVPbkContactManager& /*aContactManager*/,
        MVIMPSTStorageContactManagementObserver& /*aContactManagementObserver*/ )
    {
   
    return NULL;	        
    }
    
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::CreateContactL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::CreateContactL(
							MVIMPSTStorageContactList& /*aContactList*/,
                			const TDesC& /*aWVID*/,
                			TVIMPSTEnums::TOnlineStatus /*aStatus*/, 
                			const TDesC& /*aStatusText*/,
                			MVPbkContactLink* /*aContactLink*/,
							CVPbkContactManager& /*aContactManager*/,
							MVIMPSTStorageContactManagementObserver& /* aContactManagementObserver*/)
	{

    // this is used at fetching, so do not notify any observer, will be done for whole list
    return NULL;	
	}

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::RemoveAllContactsL
// -----------------------------------------------------------------------------
//
void MyMVIMPSTStorageViewIdStub::RemoveAllContactsL()
    {
    
    }


    
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::UpdatepresenceL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::UpdatePresenceL( 
                             const TDesC& /*aContactId*/, 
                             TVIMPSTEnums::TOnlineStatus/* aStatus*/,
                             const TDesC& /*aAlias*/,
                             const TDesC& /*aStatusText*/)
    {
    return NULL;
    }
    
    
    
// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::OwnStatus
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& MyMVIMPSTStorageViewIdStub::OwnStatus()
    {
    MVIMPSTStorageContact* own = NULL;
    return *own;
    }
    

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::IsContactValid
// -----------------------------------------------------------------------------
//
TBool MyMVIMPSTStorageViewIdStub::IsContactValid( const MVIMPSTStorageContact* /*aContact*/ ) const
    {
    // contact not found
    return EFalse;
    }

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::SortContactLists
// -----------------------------------------------------------------------------
//
void MyMVIMPSTStorageViewIdStub::SortContactLists()
    {
    
    }

// -----------------------------------------------------------------------------
// MyMVIMPSTStorageViewIdStub::FindContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContactList* MyMVIMPSTStorageViewIdStub::FindContactList( const TDesC& /*aListId */)
    {
    return NULL;
    }

/**
 * From MVIMPSTStorageContacts
 */
MVIMPSTStorageVpbkStoreHandler& MyMVIMPSTStorageViewIdStub::GetStoreHandler( )
{
MVIMPSTStorageVpbkStoreHandler* storehandler = NULL;
return *storehandler;
}

/**
 * From MVIMPSTStorageContacts
 */
MVPbkStoreContact* MyMVIMPSTStorageViewIdStub::CreateNewContactLC( )
{
MVPbkStoreContact* contact = NULL;
return 	contact;
}

/**
 * From MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::AddObserverL( MVIMPSTStorageContactsObserver* /*aObserver*/ )
{
	
}
    

/**
 * From MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::RemoveObserver( MVIMPSTStorageContactsObserver* /*aObserver*/ )
{
	
}
   


/**
 * From MVIMPSTStorageContacts
 */
TInt MyMVIMPSTStorageViewIdStub::FindContact( const MVIMPSTStorageContact& /*aContact*/ ) const
	{
	return 0;	
	}


/**
 * From MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::FindContacts( RPointerArray< MVIMPSTStorageContact >& /*aArray*/, 
                   const TDesC& /*aContactId*/ ) const
	{
	
	}
     
                       
/**
 * From MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::Sort( const TDesC& /*aContactListId*/  )
	{
		
	}

/**
 * Resort one contact, what ever contact list it is in
 */
void MyMVIMPSTStorageViewIdStub::ResortContactInAll( MVIMPSTStorageContact* /*aContact*/ )
	{
		
	}

/**
 * From MVIMPSTStorageContacts
 */        
void MyMVIMPSTStorageViewIdStub::SetSortAlgorithm( MVIMPSTStorageContact::TSortAlgorithm /*aAlgorithm*/ )
	{
		
	}

/**
 * From MVIMPSTStorageContacts
 */        
TVIMPSTEnums::TOnlineStatus MyMVIMPSTStorageViewIdStub::OnlineStatus(   const TDesC& /*aContactId*/ )
	{
	return iStatus;	
	}


/**
 * From MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::SetNotifyOnSave( TBool /*aStatus*/ )
	{
		
	}


/**
 * MVIMPSTStorageContacts
 */
void MyMVIMPSTStorageViewIdStub::SendContactListUpdatedEvent( MVIMPSTStorageContactList& /*aList*/ )
	{
		
	}


/**
 * add contact to the virtual phonebook.
 */
void MyMVIMPSTStorageViewIdStub::AddServerContactL( const TDesC& /*aContact*/)
{
	
}


/**
 * add selected pbk2 contact to the service store.
 * @param aContact contact selected from phone book.
 */
void MyMVIMPSTStorageViewIdStub::AddPhoneBookContactL( const MVPbkStoreContact& /*aContact */)
	{
		
	}

/**
 * add selected contact to the phone book.
 * @param aContact selected contact.
 */
void MyMVIMPSTStorageViewIdStub::AddContactToPhoneBookL( const MVIMPSTStorageContact& /*aContact*/ )
	{
		
	}

 /**
 * removes contact from the virtual phonebook.
 */
void MyMVIMPSTStorageViewIdStub::RemoveServerContactL( const TDesC& /*aContact*/)
	{
		
	}
void MyMVIMPSTStorageViewIdStub::UpdateAvatarDataL( const TDesC& /*aContactId*/  ,const TDesC8& /*aData*/)
    {
    
    }

MVIMPSTStorageContact*  MyMVIMPSTStorageViewIdStub::FindContactByName( const TDesC& /*aContactListId*/, 
        const TDesC& /*aId*/ )
    {
    MVIMPSTStorageContact* contact= NULL;
    return contact;
    }

MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindContactByName( const TDesC& /*aContactListId*/, 
        const TDesC& /*aId*/,
        MVIMPSTStorageContactList*& /*aContactList*/ )
    {
    MVIMPSTStorageContact* contact= NULL;
       return contact;
    }
MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindContact( const MVPbkContactLink& /*aContactLink*/,
                                                             MVIMPSTStorageContactList*& /*aContactList*/ )

    {
    MVIMPSTStorageContact* contact= NULL;
       return contact;
    }

MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::FindContactByServiceField( const TDesC& /*aServiceId */)
    {
    MVIMPSTStorageContact* contact= NULL;
       return contact;
    }

MVIMPSTStorageContact* MyMVIMPSTStorageViewIdStub::AddContactL( const TDesC& /*aFirstName*/ /*= KNullDesC*/,const TDesC& /*aServiceField*/ /*= KNullDesC */)
    {
    MVIMPSTStorageContact* contact= NULL;
       return contact;
    }

void MyMVIMPSTStorageViewIdStub::RemoveContactL( const MVPbkContactLink& /*aContactLink*/) 
    {
    
    }
      
void MyMVIMPSTStorageViewIdStub::RemoveContactL( const TDesC& /*aServiceContactId*/)

    {
    
    }

void MyMVIMPSTStorageViewIdStub::SetOwnDataL( const TDesC& aOwnId  )
    {
    iOwnId = NULL;
    iOwnId = aOwnId.AllocL();
    }

// -----------------------------------------------------------------------------

   

//  End of File
