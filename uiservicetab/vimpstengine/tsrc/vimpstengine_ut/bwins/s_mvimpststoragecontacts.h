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
* Description:  Stored contact data container interface
*
*/



#ifndef MVIMPSTSTORAGECONTACTS_H
#define MVIMPSTSTORAGECONTACTS_H


#include "TVIMPSTEnums.h"

#include <e32std.h>
#include <badesca.h>
#include "mvimpststoragecontact.h"

// FORWARD DECLARATIONS
class MVIMPSTStorageContactsObserver;
class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;
class MVPbkContactLink;
class CVPbkContactManager;
class MVPbkStoreContact;
class MVIMPSTStorageVpbkStoreHandler;
class MVIMPSTStorageContactManagementObserver;

                


// CLASS DECLARATION

/**
 *  Interface for stored contacts container
 *
 *  @lib VIMPSTStorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageContacts
    {
    public :
    virtual void UpdateAvatarDataL( const TDesC& aContactId  ,const TDesC8& aData) = 0;
    public: // Definitions
        /**
         * An Enum
         * To define contact list preperty.
         * not used.
         */

        enum TContactListProperty
            {
            EPrimaryCollapseInUse, /*primary collapse. */
            ESecondaryCollapseInUse /* secondary  collapse. */
            };
    
    public: // New functions

        /**
         * Add an observer.
         * @param aObserver The observer
         */
        virtual void AddObserverL( MVIMPSTStorageContactsObserver* aObserver ) = 0;

        /**
         * Remove an observer.
         * @param aObserver The observer
         */
        virtual void RemoveObserver( 
                            MVIMPSTStorageContactsObserver* aObserver ) = 0;

        
        /**
         * Find a contact from the store by Name
         * @param aContactListId List which contains contact.
         * @param aId the User ID of the contact to find
         * @return The contact
         */
        virtual MVIMPSTStorageContact* FindContactByName( const TDesC& aContactListId, 
                                               const TDesC& aId ) = 0;
                                               
		virtual MVIMPSTStorageContact* FindContactByName( const TDesC& aContactListId, 
											   const TDesC& aId,
											   MVIMPSTStorageContactList*& aContactList ) = 0;                         
        
        /**
         * Find any contact which id is aContactId
         * @param aContactId Contactid to recognize contact.
         * @return First one which matches contactId. If not found return NULL
         */
        virtual MVIMPSTStorageContact* FindAnyContactByName( const TDesC& aContactId ) = 0;
        
        /**
         * Find any contact which has contact link same as aContactLink
         * @param aContactId Contactid to recognize contact.
         * @return First one which matches contactId. If not found return NULL
         */
        virtual MVIMPSTStorageContact* FindContact( const MVPbkContactLink& aContactLink,
        												MVIMPSTStorageContactList*& aContactList ) = 0;

		
		/**
         * Find a contact from the store by using Service ID         
         * @param aServiceId the Service ID of the contact to find
         * @return The contact
         */
		virtual MVIMPSTStorageContact* FindContactByServiceField( const TDesC& aServiceId ) = 0;
        
        /**
         * Count of lists.
         * @return Count of lists.
         */
        virtual TInt ListCount() const = 0;
        
        /**
         * List in index.
         * @param aIndex. Index of list.
         * return Contact list.
         */
        virtual MVIMPSTStorageContactList& ListAt( TInt aIndex ) const = 0;

        /**
         * Find contact list by id
         * @param aListId. Id to find.
         * @return Contactlist. If not found return NULL
         */        
        virtual MVIMPSTStorageContactList* FindContactList( const TDesC& aListId ) = 0;

        /**
         * Find out if contact exists in all list.
         * @param aContactId. Id to check.
         * @return ETrue if contact is in all lists. EFalse if not.
         */
        virtual TBool ContactInAllLists( const TDesC& aContactId ) const = 0;

        /**
         * Add new contact list.
         * @param aContactListId Id for new contact list.
         * @param aDisplayName Display name of new contact list.
         */
        virtual MVIMPSTStorageContactList* AddContactListIdL( const TDesC& aContactListId, 
                                        const TDesC& aDisplayName,
                                        TBool aNotify = ETrue ) = 0;
        
        /**
         * Remove contact list from store.
         * @param aContactListId. Id of contact list to be removed.
         */
        virtual void RemoveContactList( const TDesC& aContactListId ) = 0;
                
        
        /**
         * Removes all contacts.
         */
        virtual void RemoveAllContactsL() = 0;

        /**
         * Count of stored contacts
         * @param aSkipOfflineContacts ETrue skip offline contacts. 
         *                             EFalse Do not skip.
         * @return TInt Count of contacts
         */
        virtual TInt ContactCount( 
                        TBool aSkipOfflineContacts = EFalse ) const = 0;
            
        /**
         * Online status of user. 
         * @param aContactId Id which status is wanted.
         * @return Onlinestatus of user.
         */        
        virtual TVIMPSTEnums::TOnlineStatus OnlineStatus( 
                                              const TDesC& aContactId ) = 0;
        
        /** 
         * Update presence information of contact.
         * @param aContactId Id of contact which presence is updated.
         * @param aStatus. New presence status
         * @param aAlias. New alias.
         * @param aStatusText New status message text.
         * @return MVIMPSTStorageContact* pointer of the contact
         */
        virtual MVIMPSTStorageContact* UpdatePresenceL( const TDesC& aContactId, 
                          TVIMPSTEnums::TOnlineStatus aStatus, 
                          const TDesC& aAlias,
                          const TDesC& aStatusText) = 0;
               
        
        /**
         * Check if contact is found in storage. 
         * @param aContact contact to find.
         * @return TBool ETrue if contact is found from storage
         */         
        virtual TBool IsContactValid( const MVIMPSTStorageContact* aContact ) const = 0;
        
        /**
         * Sorts contact lists alphabetically.
         * @since S60 v5.0
         */
        virtual void SortContactLists() = 0;
        
        
         /**
         * From MVIMPSTStorageContacts
         */
        virtual void Sort( const TDesC& aContactListId = KNullDesC ) = 0;

        /**
         * Resort one contact, what ever contact list it is in
         */
        virtual void ResortContactInAll( MVIMPSTStorageContact* aContact ) = 0;
        
        /**
         * From MVIMPSTStorageContacts
         */        
        virtual void SetSortAlgorithm( MVIMPSTStorageContact::TSortAlgorithm aAlgorithm ) = 0;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual MVIMPSTStorageContact* AddContactL( const TDesC& aFirstName = KNullDesC, 
									const TDesC& aServiceField = KNullDesC ) = 0;


        /**
         * removes contact from the virtual phonebook.
         * @param aContact contact id to be removed.
         */
        virtual void RemoveContactL( const MVPbkContactLink& aContactLink) = 0;
        
        /**
         * removes contact from the virtual phonebook.
         * @param aContact contact id to be removed.
         */
        virtual void RemoveContactL( const TDesC& aServiceContactId) = 0;
        
        virtual void SetOwnDataL( const TDesC& aOwnId  ) = 0;
       
    protected:
        /**
         * virtual destructor.
         */ 
        virtual ~MVIMPSTStorageContacts(){};
    };
    
#endif      // MVIMPSTSTORAGECONTACTS_H

// End of File
