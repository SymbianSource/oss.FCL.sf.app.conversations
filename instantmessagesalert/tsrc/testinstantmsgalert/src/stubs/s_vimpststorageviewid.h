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


#ifndef CVIMPSTSTORAGEVIEWID_H
#define CVIMPSTSTORAGEVIEWID_H

#include "TVIMPSTEnums.h"
#include "s_mvimpststoragecontacts.h"
//  INCLUDES

// FORWARD DECLARATIONS
class MVIMPSTStorageContactList;
class MVPbkContactLink;
class CVPbkContactManager;
class MVPbkStoreContact;
class MVIMPSTStorageVpbkStoreHandler;
class MVIMPSTStorageContactManagementObserver;
class MVIMPSTStorageContactsObserver;

// CLASS DECLARATION

class  MyMVIMPSTStorageViewIdStub :  public MVIMPSTStorageContacts
    {
     
     
         
    public: // Functions from base classes
    
       void UpdateAvatarDataL( const TDesC& aContactId  ,const TDesC8& aData);
    	MyMVIMPSTStorageViewIdStub()
	       {
	       	
	       }

       ~MyMVIMPSTStorageViewIdStub()
	       {
	       	
	       }
    
        /**
         * From MVIMPSTStorageContacts
         */
        MVIMPSTStorageVpbkStoreHandler& GetStoreHandler( );
        
        /**
         * From MVIMPSTStorageContacts
         */
        MVPbkStoreContact* CreateNewContactLC( );
        
        /**
         * From MVIMPSTStorageContacts
         */
        void AddObserverL( MVIMPSTStorageContactsObserver* aObserver );
	        

        /**
         * From MVIMPSTStorageContacts
         */
        void RemoveObserver( MVIMPSTStorageContactsObserver* aObserver );
	       

        /**
         * From MVIMPSTStorageContacts
         */
        MVIMPSTStorageContactList* AddContactListIdL( const TDesC& aContactListId, 
                                const TDesC& aDisplayName,
                                TBool aNotify = ETrue );
        
        /**
         * From MVIMPSTStorageContacts
         */
        void RemoveContactList( const TDesC& aContactListId );
        
        /**
         * From MVIMPSTStorageContacts
         */
        void RemoveContact( const TDesC& aListId, const TDesC& aContactId );

        /**
         * From MVIMPSTStorageContacts
         */
        
        void SetOwnDataL( const TDesC& aOwnId  );
        
        
        MVIMPSTStorageContact* FindContact( const TDesC& aContactListId, 
                                       const TDesC& aId );
        
        MVIMPSTStorageContact* FindContact( const TDesC& aContactListId, 
									   const TDesC& aId,
									   MVIMPSTStorageContactList*& aContactList ); 
        /**
         * From MVIMPSTStorageContacts
         */
        MVIMPSTStorageContact* FindAnyContact( const TDesC& aContactId );
       
        /**
         * Find a contact from the store by using Service ID         
         * @param aServiceId the Service ID of the contact to find
         * @return The contact
         */
         MVIMPSTStorageContact* FindContactByServiceField( const TDesC& aServiceId );

        /**
        * Find any contact which id is aContactId
        * @param aContactId Contactid to recognize contact.
        * @return First one which matches contactId. If not found return NULL
        */
       MVIMPSTStorageContact* FindAnyContactByName( const TDesC& aContactId );
       
       /**
               * Find any contact which has contact link same as aContactLink
               * @param aContactId Contactid to recognize contact.
               * @return First one which matches contactId. If not found return NULL
               */
       MVIMPSTStorageContact* FindContact( const MVPbkContactLink& aContactLink,
                                                              MVIMPSTStorageContactList*& aContactList ) ;

       
       /**
       * Find a contact from the store by Name
       * @param aContactListId List which contains contact.
       * @param aId the User ID of the contact to find
       * @return The contact
       */
        MVIMPSTStorageContact* FindContactByName( const TDesC& aContactListId, 
                                                 const TDesC& aId ) ;
                                                 
        MVIMPSTStorageContact* FindContactByName( const TDesC& aContactListId, 
                                                 const TDesC& aId,
                                                 MVIMPSTStorageContactList*& aContactList ) ;                         
            
        /**
          * add contact ot the virtual phonebook.
          * @param aContact contact id to be added.
          */
       MVIMPSTStorageContact* AddContactL( const TDesC& aFirstName = KNullDesC, 
                                     const TDesC& aServiceField = KNullDesC );

        /**
         * From MVIMPSTStorageContacts
         */
        TInt FindContact( const MVIMPSTStorageContact& aContact ) const;
        
        /**
         * From MVIMPSTStorageContacts
         */
        TBool ContactInAllLists( const TDesC& aContactId ) const;        

        /**
         * From MVIMPSTStorageContacts
         */
        TInt ListCount() const;
        
        /**
         * From MVIMPSTStorageContacts
         */
        MVIMPSTStorageContactList& ListAt( TInt aIndex ) const;

        /**
         * From MVIMPSTStorageContacts
         */
        void FindContacts( RPointerArray< MVIMPSTStorageContact >& aArray, 
                           const TDesC& aContactId ) const;

        /**
         * From MVIMPSTStorageContacts
         */
        TInt ContactCount( TBool aSkipOfflineContacts = EFalse ) const;
               
                               
        /**
         * From MVIMPSTStorageContacts
         */
        void Sort( const TDesC& aContactListId = KNullDesC );

        /**
         * Resort one contact, what ever contact list it is in
         */
        void ResortContactInAll( MVIMPSTStorageContact* aContact );
        
        /**
         * From MVIMPSTStorageContacts
         */        
        void SetSortAlgorithm( MVIMPSTStorageContact::TSortAlgorithm aAlgorithm );

        /**
         * From MVIMPSTStorageContacts
         */        
        TVIMPSTEnums::TOnlineStatus OnlineStatus( 
                                                    const TDesC& aContactId );
        
        /**
         * From MVIMPSTStorageContacts
         */        
        MVIMPSTStorageContact* UpdatePresenceL(   const TDesC& aContactId, 
                                TVIMPSTEnums::TOnlineStatus aStatus,
                                const TDesC& aAlias,
                                const TDesC& aStatusText);
        /**
         * From MVIMPSTStorageContacts
         */        
        MVIMPSTStorageContact& OwnStatus();
        
        /**
         * From MVIMPSTStorageContacts
         */
        void SetNotifyOnSave( TBool aStatus );
        
       
        /**
         * MVIMPSTStorageContacts
         */
        MVIMPSTStorageContact* CreateContactL( 
                const TDesC& aContactList,
                const TDesC& aWVID,
                MVPbkContactLink* aContactLink,
				CVPbkContactManager& aContactManager,
				MVIMPSTStorageContactManagementObserver& aContactManagementObserver);
        
        /**
         * MVIMPSTStorageContacts
         */
		MVIMPSTStorageContact* CreateContactL(
				MVIMPSTStorageContactList& aContactList,
                const TDesC& aWVID,
                TVIMPSTEnums::TOnlineStatus aStatus, 
                const TDesC& aStatusText,
                MVPbkContactLink* aContactLink,
				CVPbkContactManager& aContactManager ,
				MVIMPSTStorageContactManagementObserver& aContactManagementObserver);
        
        /**
         * MVIMPSTStorageContacts
         */
        void SendContactListUpdatedEvent( MVIMPSTStorageContactList& aList );                
        
        
        /**
         * From MVIMPSTStorageContacts
         */
        void RemoveAllContactsL();
               
       
        /**
         * From MVIMPSTStorageContacts
         */
        TBool IsContactValid( const MVIMPSTStorageContact* aContact ) const;

        /**
         * From MVIMPSTStorageContacts
         */
        void SortContactLists();
        

        
        /**
         * add contact to the virtual phonebook.
         */
        void AddServerContactL( const TDesC& aContact);
        
        
        /**
         * add selected pbk2 contact to the service store.
         * @param aContact contact selected from phone book.
         */
        void AddPhoneBookContactL( const MVPbkStoreContact& aContact );
        
        /**
         * add selected contact to the phone book.
         * @param aContact selected contact.
         */
        void AddContactToPhoneBookL( const MVIMPSTStorageContact& aContact );
        
         /**
         * removes contact from the virtual phonebook.
         */
        void RemoveServerContactL( const TDesC& aContact);
        
         /**
         * Find contact list by id
         * @param aListId. Id to find.
         * @return Contactlist. If not found return NULL
         */        
        MVIMPSTStorageContactList* FindContactList( const TDesC& aListId ) ;
        /**
               * removes contact from the virtual phonebook.
               * @param aContact contact id to be removed.
               */
              void RemoveContactL( const MVPbkContactLink& aContactLink);
              
              /**
               * removes contact from the virtual phonebook.
               * @param aContact contact id to be removed.
               */
              void RemoveContactL( const TDesC& aServiceContactId);
              
              
              
               
        
        TVIMPSTEnums::TOnlineStatus iStatus;
        
        HBufC* iOwnId;
        

      
    };
    
#endif      // CVIMPSTVIEWID_H

// End of File
