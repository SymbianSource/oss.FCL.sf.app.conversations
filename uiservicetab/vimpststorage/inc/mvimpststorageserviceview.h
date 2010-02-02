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


#ifndef MVIMPSTSTORAGESERVICEVIEW_H
#define MVIMPSTSTORAGESERVICEVIEW_H


#include "tvimpstenums.h"

#include <e32std.h>
#include <badesca.h>
#include "mvimpststoragecontact.h"

// FORWARD DECLARATIONS
class MVIMPSTStorageContactsObserver;
class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;
class MVPbkContactLink;

// CLASS DECLARATION

class MVIMPSTStorageServiceCacheWriter
	{
	public:

		/**
		* update existing contact 
		* @param aContactLink contact id to be updatated
		* @param aUserId contact id to be updated
		* @param aDisplayName contact display name to be updated
		* @reurn the storage contact pointer if found else NULL
		*/
		virtual MVIMPSTStorageContact* UpdateCacheContactL(const MVPbkContactLink& aContactLink,
												 const TDesC& aUserId, 
								   	   	         const TDesC& aDisplayName,
								   	   	         const TDesC8& aAvatarContent ) = 0 ;

		/**
		* add a new contact to cache  
		* @param aContactLink contact id to be add
		* @param aUserId contact id to be add
		* @param aDisplayName contact display name to be add 
		* @reurn the new storage contact pointer if contact is already exist return the smae 
		
		*/		 					   	   	       
		virtual MVIMPSTStorageContact* AddContactToCacheL(const MVPbkContactLink& aContactLink,
										 const TDesC& aUserId, 
						   	   	         const TDesC& aDisplayName,
						   	   	         const TDesC8& aAvatarContent,
						   	   	         TVIMPSTEnums::TVIMPSTStorgaeEventType aType ) =0  ;
					   	   	               
		/*
		* remopve existing contact 
		* @param aContactLink contact id to be removed
		* @param aUserId contact id to be removed
		* @param aDisplayName contact display name to be removed
		* @reurn the error if any
		*/
		virtual TInt RemoveContactFromCacheL(const MVPbkContactLink& aContactLink,TVIMPSTEnums::TVIMPSTStorgaeEventType aType ) = 0;


		/**
		* Removes all contacts from cache and notify to observers
		*/
		virtual void RemoveAllCacheContactsL() = 0;

		/**
		* Read the default contact list if exist else create default list
		* @return contact list pointer
		*/
		virtual MVIMPSTStorageContactList* GetDefaultContactListL() =0;

		/**
		* send the notificatoion about fetch completion
		*/
		virtual void NotifyServiceViewL(TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType, MVIMPSTStorageContact* aContact = NULL ) = 0 ;

		/**
		* find contact by user id
		* @param aUserId contact id to be find
		* @return storage contact pointer if found else NULL
		*/
		virtual MVIMPSTStorageContact* FindCacheContactByUserId( const TDesC& aUserId ) = 0 ;
		
		
		/**
		* find contact by user id
		* @param aContactLink contact link to be find
		* @return storage contact pointer if found else NULL
		*/
		virtual MVIMPSTStorageContact* FindCacheContactByLink(const MVPbkContactLink& aContactLink) = 0 ;

		/**
		* add contact to the contact cache and notify the observers
		* @param aContact contact id to be added.
		*/
	    virtual	TInt AddStorageContactToCacheL(MVIMPSTStorageContact* aContactToAdd ) = 0 ;
		
	
	};
/**
 *  Interface for stored contacts container
 *
 *  @lib VIMPSTStorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageServiceView
    {
    public: // Definitions
       
        /**
         * Add an observer.
         * @param aObserver The observer
         */
        virtual void AddObserverL( MVIMPSTStorageContactsObserver* aObserver ) = 0;

        /**
         * Remove an observer.
         * @param aObserver The observer
         */
        virtual void RemoveObserver( MVIMPSTStorageContactsObserver* aObserver ) = 0;

      	
		/**
         * Find a contact from the store by using Service ID         
         * @param aServiceId the Service ID of the contact to find
         * @return The contact
         */
		virtual MVIMPSTStorageContact* FindContactByUserId( const TDesC& aUserId ) = 0;
        
       
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
         * Add new contact list.
         * @param aContactListId Id for new contact list.
         * @param aDisplayName Display name of new contact list.
         */
        virtual MVIMPSTStorageContactList* CreateContactListL( const TDesC& aContactListId, 
                                        				      const TDesC& aDisplayName ) = 0;
        
        /**
         * Remove contact list from store.
         * @param aContactListId. Id of contact list to be removed.
         */
        virtual void RemoveContactList( const TDesC& aContactListId ) = 0;
                
       
        /**
         * Count of stored contacts
         * @param aSkipOfflineContacts ETrue skip offline contacts. 
         *                             EFalse Do not skip.
         * @return TInt Count of contacts
         */
        virtual TInt ContactCount(TBool aSkipOfflineContacts = EFalse ) const = 0;
            
          
        /** 
         * Update presence information of contact.
         * @param aContactId Id of contact which presence is updated.
         * @param aStatus. New presence status
         * @param aAlias. New alias.
         * @param aStatusText New status message text.
         * @param aAvatarData  avatar content,
         * @param aIsClearingAvatar set to ETrue while clearing the avatar
         * @return MVIMPSTStorageContact* pointer of the contact
         */
        virtual MVIMPSTStorageContact* UpdatePresenceL(const TDesC& aContactId, 
							                           TVIMPSTEnums::TOnlineStatus aStatus, 
							                           const TDesC& aStatusText,
							                           const TDesC8& aAvatarData,
							                           TBool aIsClearingAvatar = EFalse) = 0;
         
         /** 
         * Update presence information of contact.
         * @param aContactId Id of contact which presence is updated.
         * @param aStatus. New presence status
         * @param aAlias. New alias.
         * @param aStatusText New status message text.
         * @return MVIMPSTStorageContact* pointer of the contact
         */
        virtual MVIMPSTStorageContact* UpdateAvatarL(const TDesC& aContactId,
							                         const TDesC8& aAvatarData ) = 0;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual MVIMPSTStorageContact* CreateNewContactL(const TDesC& aUserId, 
        										         const TDesC& aDisplayName = KNullDesC,
        										         TBool aIsInvitationItem = EFalse ,
        										         TBool aInvitationAutoAccept = EFalse) = 0;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual TInt CreateNewContactFromRetrivedIdL(  TInt aIndexToUse  ) = 0;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual TInt DeleteNewContactFromRetrivedIdL(  TInt aIndexToUse  ) = 0;
        		
		/**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual TInt RemoveContactL( MVIMPSTStorageContact* aContact  )  = 0 ;
        
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        virtual void CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
                                             RArray <TPtrC> &aServiceField ) = 0;
        
        
        /**
         * returns ETrue if Local Store, EFalse if XSP Store
         * @return TBool
         */
        virtual TBool  IsLocalStore() const = 0;
        
      	
  		/**
         * From MVIMPSTStorageServiceView
         */
  		virtual TInt RetriveLinkXSPIdsL(const TDesC8& aContactPackLink ) = 0 ;
        
        /**
         * From MVIMPSTStorageServiceView
         */
        virtual const TDesC& GetRetrieveXSPIdL(TInt aIndex )  = 0 ;
       
       	/**
         * From MVIMPSTStorageServiceView
         */
	   virtual MVIMPSTStorageContact* FindContactByLink(const MVPbkContactLink& aContactLink ) = 0  ;
	   
	   
	   virtual void Sort( const TDesC& aContactListId  = KNullDesC ) = 0 ;
	   
	   virtual MVIMPSTStorageContact& OwnContactL() = 0 ;
	   
	   virtual void DeleteDatabaseL() = 0 ;
	   
	   virtual void SetOwnUserIdL(const TDesC& aUserId ) = 0 ; 
	   
		/**
		* From MVIMPSTStorageServiceView
		* takes the ownership of aUnnamedText
		*/
  	   virtual void SetUnnamedTextL(HBufC* aUnnamedText ) = 0 ;
	
    protected:
        /**
         * virtual destructor.
         */ 
        virtual ~MVIMPSTStorageServiceView(){};
    };
    
#endif      // MVIMPSTSTORAGESERVICEVIEW_H

// End of File
