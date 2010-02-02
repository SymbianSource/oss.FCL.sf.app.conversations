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

#ifndef CVIMPSTSTORAGESERVICEVIEW_H
#define CVIMPSTSTORAGESERVICEVIEW_H


//  INCLUDES
#include <e32std.h>
#include <f32file.h>
#include <babackup.h>

#include "mvimpststorageserviceview.h"
#include "tvimpststoragepanics.h"
#include "tvimpstenums.h"
#include "mvimpststorageitemmodel.h"
#include "mvimpststorageactiveobserver.h"

// FORWARD DECLARATIONS
class CVIMPSTStorageContactList;
class CVIMPSTStorageContactSorter;
class MVIMPSTStorageVPbkContactStore;
class MVPbkContactLink;
class CVIMPSTStorageActiveHandler;

// CLASS DECLARATION

NONSHARABLE_CLASS ( CVIMPSTStorageServiceView ): public CBase,
												 public MVIMPSTStorageServiceView,
												 public MVIMPSTStorageServiceCacheWriter,
												 public MVIMPSTStorageItemModel,
												 public MVIMPSTStorageActiveObserver
												   
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CVIMPSTStorageServiceView* NewL(TUint32 aServiceId, const TDesC& aStoreName,
        								const TDesC& aServiceName);

        /**
         * Destructor.
         */
        ~CVIMPSTStorageServiceView();

    public: // New functions        
        
        
        TUint32 GetServiceId();
        
    public: // Functions from base classes
    	
    	/**
         * From MVIMPSTStorageServiceView
         */
        void AddObserverL( MVIMPSTStorageContactsObserver* aObserver ) ;
     
        /**
         * From MVIMPSTStorageServiceView
         */
        void RemoveObserver( MVIMPSTStorageContactsObserver* aObserver ) ;
        
      	/**
         * From MVIMPSTStorageServiceView
         */
		MVIMPSTStorageContact* FindContactByUserId( const TDesC& aUserId ) ;
		
		/**
         * From MVIMPSTStorageServiceView
         */
		MVIMPSTStorageContact* FindContactByLink(const MVPbkContactLink& aContactLink ) ;
		
        /**
         * From MVIMPSTStorageServiceView
         */
        TInt ListCount() const ;
        
        
        /**
         * From MVIMPSTStorageServiceView
         */
        MVIMPSTStorageContactList& ListAt( TInt aIndex ) const ;
        

        /**
         * From MVIMPSTStorageServiceView
         */        
        MVIMPSTStorageContactList* FindContactList( const TDesC& aListId ) ;
        
     
        /**
         * From MVIMPSTStorageServiceView
         */
        MVIMPSTStorageContactList* CreateContactListL( const TDesC& aContactListId, 
                                        const TDesC& aDisplayName ) ;
                                        
        
        /**
         * From MVIMPSTStorageServiceView
         */
        void RemoveContactList( const TDesC& aContactListId ) ;
                
        

        /**
         * From MVIMPSTStorageServiceView
         */
        TInt ContactCount( TBool aSkipOfflineContacts = EFalse ) const ;
                        
                        
         /** 
         * Update presence information of contact.
         * @param aContactId Id of contact which presence is updated.
         * @param aStatus. New presence status
         * @param aAlias. New alias.
         * @param aStatusText New status message text.
         * @return MVIMPSTStorageContact* pointer of the contact
         */
        MVIMPSTStorageContact* UpdatePresenceL(const TDesC& aContactId, 
							                           TVIMPSTEnums::TOnlineStatus aStatus, 
							                           const TDesC& aStatusText,
							                           const TDesC8& aAvatarData,
							                           TBool aIsClearingAvatar = EFalse) ;
         
         /** 
         * Update presence information of contact.
         * @param aContactId Id of contact which presence is updated.
         * @param aStatus. New presence status
         * @param aAlias. New alias.
         * @param aStatusText New status message text.
         * @return MVIMPSTStorageContact* pointer of the contact
         */
        MVIMPSTStorageContact* UpdateAvatarL(const TDesC& aContactId,
							                  const TDesC8& aAvatarData ) ;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        MVIMPSTStorageContact* CreateNewContactL(const TDesC& aUserId, 
        								  const TDesC& aDisplayName = KNullDesC,
        								  TBool aIsInvitationItem =  EFalse,
        								  TBool aInvitationAutoAccept = EFalse) ;
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        TInt CreateNewContactFromRetrivedIdL( TInt aIndexToUse );
        
        /**
         * remove contact ot the virtual phonebook.
         * @param aContact contact id to be removed.
         */
        TInt DeleteNewContactFromRetrivedIdL(  TInt aIndexToUse  );
        
        /**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        void CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
                                            RArray <TPtrC> &aServiceField ) ;
               
        
		/**
         * add contact ot the virtual phonebook.
         * @param aContact contact id to be added.
         */
        TInt RemoveContactL( MVIMPSTStorageContact* aContact  ) ;
        
        
        /**
         * From MVIMPSTStorageServiceView
         */
        TBool  IsLocalStore() const ;
        
  		/**
         * From MVIMPSTStorageServiceView
         */
  		void SetOwnUserIdL( const TDesC& aUserId );
  		
  	
  		/**
         * From MVIMPSTStorageServiceView
         */
  		TInt RetriveLinkXSPIdsL(const TDesC8& aContactPackLink );
  		
  		/**
         * From MVIMPSTStorageServiceView
         */
        const TDesC& GetRetrieveXSPIdL(TInt aIndex )  ;
  		
  	   	/**
         * From MVIMPSTStorageServiceView
         */
  	    MVIMPSTStorageContact& OwnContactL();
  	    
  	    /**
         * From MVIMPSTStorageServiceView
         */
  	    void DeleteDatabaseL();
  	    
  	     /**
         * From MVIMPSTStorageServiceView
         */
  	    void SetUnnamedTextL(HBufC* aUnnamedText );
  		
  	public: //  MVIMPSTStorageServiceCacheWriter
  	
  		/**
		* update existing contact 
		* @param aContactLink contact id to be updatated
		* @param aUserId contact id to be updated
		* @param aDisplayName contact display name to be updated
		* @reurn the storage contact pointer if found else NULL
		*/
		MVIMPSTStorageContact* UpdateCacheContactL(const MVPbkContactLink& aContactLink,
												 const TDesC& aUserId, 
								   	   	         const TDesC& aDisplayName,
								   	   	         const TDesC8& aAvatarContent )  ;

		/**
		* add a new contact to cache  
		* @param aContactLink contact id to be add
		* @param aUserId contact id to be add
		* @param aDisplayName contact display name to be add 
		* @reurn the new storage contact pointer if contact is already exist return the smae 
		
		*/		 					   	   	       
		MVIMPSTStorageContact* AddContactToCacheL(const MVPbkContactLink& aContactLink,
										 const TDesC& aUserId, 
						   	   	         const TDesC& aDisplayName,
						   	   	         const TDesC8& aAvatarContent,
						   	   	         TVIMPSTEnums::TVIMPSTStorgaeEventType aType )   ;
					   	   	               
		/*
		* remopve existing contact 
		* @param aContactLink contact id to be removed
		* @param aUserId contact id to be removed
		* @param aDisplayName contact display name to be removed
		* @reurn the error if any
		*/
		TInt RemoveContactFromCacheL(const MVPbkContactLink& aContactLink,TVIMPSTEnums::TVIMPSTStorgaeEventType aType ) ;


		/**
		* Removes all contacts from cache and notify to observers
		*/
		void RemoveAllCacheContactsL() ;

		/**
		* Read the default contact list if exist else create default list
		* @return contact list pointer
		*/
		MVIMPSTStorageContactList* GetDefaultContactListL() ;

		/**
		* find contact by user id
		* @param aUserId contact id to be find
		* @return storage contact pointer if found else NULL
		*/
		 MVIMPSTStorageContact* FindCacheContactByUserId( const TDesC& aUserId ) ;
		 
		 
		 /**
		* find contact by user id
		* @param aContactLink contact link to be find
		* @return storage contact pointer if found else NULL
		*/
		 MVIMPSTStorageContact* FindCacheContactByLink(const MVPbkContactLink& aContactLink) ;

		/**
		* add contact to the contact cache and notify the observers
		* @param aContact contact id to be added.
		*/
		TInt AddStorageContactToCacheL(MVIMPSTStorageContact* aContactToAdd );

		/**
		* add contact to the contact cache and notify the observers
		* @param aContact contact id to be added.
		*/
		void NotifyServiceViewL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
								 MVIMPSTStorageContact* aContact = NULL )  ;
  		
  	public : //MVIMPSTStorageActiveObserver
  		
  			/**
		* This call is made when the waiting completes
		* @param aError error code for waiting error, KErrNone if completes without error
		*
		*/
		void HandleDelayedNotificationL(TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
									   MVIMPSTStorageContactList *aList, 
			                           MVIMPSTStorageContact* aContact,
			                           TInt aContactIndex) ;
  	private:
           

        /**
         * C++ default constructor.
         */
        CVIMPSTStorageServiceView(TUint32 aServiceId);

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL(const TDesC& aStoreName, const TDesC& aServiceName);
        
        
        CVIMPSTStorageContactList* FindContactListInternal( const TDesC& aListId );
                                              
         
        TInt FindContactListById(const TDesC& aListId );
		
		 // TLinearOrder
        static TInt ContactListOrderByDisplayName( 
                                    const CVIMPSTStorageContactList& aContactA, 
                                    const CVIMPSTStorageContactList& aContactB );
                                    
        static TBool ContactListFindByContactListId( 
                            const CVIMPSTStorageContactList& aContactA, 
                            const CVIMPSTStorageContactList& aContactB );
        
   		
   		/**
         * Notify all the observers about the given event
         * @param aType The type of observer event
         * @param aContact The contact in question
         * (or -1 == KCAStorageDontKnowPos to force search for given contact id
         * in aParameter)
         * @param aFromPEC, ETrue, if event is coming from PEC
         */
        void NotifyAllObserversL( TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
        						  MVIMPSTStorageContactList* aContactList,
                                  MVIMPSTStorageContact* aContact,
                                  TInt aContactIndex );

        /**
         * Notify all the observer about the given event after small delay
         * If new notify events come during delay, then notify is "shifted"
         * with new delay time and notify event is change to "multiplechanges".
         * @param aType The type of observer event
         * @param aContact The contact in question
         * (or -1 == KCAStorageDontKnowPos to force search for given contact id
         * in aParameter)
         * @param aFromPEC, ETrue, if event is coming from PEC
         */
        void NotifyAllObserversWithDelay( TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
		        						  MVIMPSTStorageContactList* aContactList,
		                                  MVIMPSTStorageContact* aContact,
		                                  TInt aContactIndex );
                
   	
    public : // from MVIMPSTStorageItemModel
    
    	/**
         * @return Count of items
         */
        TInt Count() const;

		/**
		 * Returns List item at given index.
		 * @param aIndex
		 * @return List item
		 */
        MVIMPSTStorageItemModel::SItem Item( TInt aIndex ) const;
            
        /**
         * @see MVIMPSTStorageItemModel
         */
        TInt IndexOfContact( MVIMPSTStorageContact* aContact ) const;
        
        /**
         * @see MVIMPSTStorageItemModel
         */
        TInt IndexOfList( MVIMPSTStorageContactList* aList,
                          TBool aIgnoreOwnItem /*= EFalse*/,
                          TBool aIgnoreEmptyLists /*= ETrue*/ ) const;
 
 public :       
               
        /**
         * Sets the correct sorting method to storage
         * according to current setting value
         */
        void SetSort();
        
        /**
         * sort the contacts of list
         * according to current setting value
         */
        void Sort( const TDesC& aContactListId = KNullDesC  );
        
    
    protected:  // Functions from MDesCArray
        
        /**
        * @see MDesCArray
        */
        TInt MdcaCount() const;

        /**
        * @see MDesCArray
        */
        TPtrC MdcaPoint(TInt aIndex) const;
         
    private:    // Data

        // observers list, used with MVIMPSTStorageServiceView inherited methods
        RPointerArray< MVIMPSTStorageContactsObserver > iContactObservers;

        // list of contacts
        RPointerArray< CVIMPSTStorageContactList > iContactListArray;   // owns
       
        //Service ID
  	    TUint32 iServiceId;        
        // owns : pointer to service store ,Local/Server
        MVIMPSTStorageVPbkContactStore* iVPbkContactStore;
        
         /// Sorter for contacts. Owns
        CVIMPSTStorageContactSorter* iContactSorter;
        
         /// Contact for own data
         MVIMPSTStorageContact* iOwnContact;
          /// Owns : refreshTimer
         CVIMPSTStorageActiveHandler* iActiveHandler;
         
         // if ETrue meaning a invitation is getting added to cache
         TBool iIsInvitationItem; 
         
         // owns : unnamed text
    	 HBufC* iUnNamedText; 
        
    };
    
#endif      // CVIMPSTSTORAGESERVICEVIEW_H

// End of File
