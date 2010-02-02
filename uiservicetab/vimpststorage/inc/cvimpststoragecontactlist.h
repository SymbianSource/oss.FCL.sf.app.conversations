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
* Description:  Contact list container implementation
*
*/



#ifndef CVIMPSTSTORAGECONTACTLIST_H
#define CVIMPSTSTORAGECONTACTLIST_H

//  INCLUDES
#include <e32base.h>
#include "mvimpststoragecontactlist.h"
#include "mvimpststoragecontact.h"
#include "tvimpstenums.h"

// FORWARD DECLARATIONS
class CVIMPSTStorageContactSorter;
class MVPbkContactLink;
class MVPbkBaseContact;


// CLASS DECLARATION

/**
 *  Contact list data implementation
 */
NONSHARABLE_CLASS( CVIMPSTStorageContactList ) : public CBase, 
											     public MVIMPSTStorageContactList
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aObserver. Observer for events
         * @param aSorter. Sorter for contaclist.
         * @param aInfo. Information from storage
         * @param alistId. List id of list. Default value KNullDesC, if not set.
         * @param aDisplayname. Name of list.
         */
        static CVIMPSTStorageContactList* NewL( CVIMPSTStorageContactSorter& aSorter,
		                                        const TDesC& aListId,
		                                        const TDesC& aDisplayName = KNullDesC );
        /**
         * Two-phased constructor.
         * @param aObserver. Observer for events
         * @param aSorter. Sorter for contaclist.
         * @param aInfo. Information from storage
         * @param alistId. List id of list.
         * @param alistId. List id of list. Default value KNullDesC, if not set.
         */
        static CVIMPSTStorageContactList* NewLC( CVIMPSTStorageContactSorter& aSorter,
		                                        const TDesC& aListId,
		                                        const TDesC& aDisplayName = KNullDesC );

        /**
         * Destructor.
         */
        ~CVIMPSTStorageContactList();


    public:

        /**
         * @see MVIMPSTStorageContactList
         */
        const TDesC& ListId() const;

        /**
         * @see MVIMPSTStorageContactList
         */
        TPtrC DisplayName() const;

        /**
         * @see MVIMPSTStorageContactList
         */
        TInt Count() const;

        /**
         * @see MVIMPSTContactList
         */
        MVIMPSTStorageContact& operator[]( TInt aIndex ) const;

        /**
         * @see MVIMPSTStorageContactList
         */
        MVIMPSTStorageContact& FilteredContact(
              TInt aIndex, TVIMPSTEnums::TFilterType aFilter ) const;

        /**
         * @see MVIMPSTStorageContactList
         */
        virtual TInt FilteredCount(
                           TVIMPSTEnums::TFilterType aFilter ) const;

        
        /**
         * @see MVIMPSTStorageContactList
         */
        void Sort();

        /**
         * @see MVIMPSTStorageContactList
         */
        void ResortContact( MVIMPSTStorageContact* aContact );
  
        /**
         * @see MVIMPSTStorageContactList
         */
        TInt FindIndexOfContact(
                        const MVIMPSTStorageContact* aContact,
                        TVIMPSTEnums::TFilterType aFilter =
                         TVIMPSTEnums::EFilterAll ) const;

        /**
         * @see MVIMPSTStorageContactList
         */
        TBool FilterAllowsContact( const MVIMPSTStorageContact* aContact,
                        TVIMPSTEnums::TFilterType aFilter ) const;
    public: // New functions
		
		MVIMPSTStorageContact* FindContact( const TDesC& aContactId );
		
                
        /**
         * Find contact by MVPbkContactLink Contact Link VPBK.
         * @param aContactLink. Contact Link to find.
         * @return Pointer to found contact. NULL if not found.
         */
        MVIMPSTStorageContact* FindContactByContactLink( const MVPbkContactLink& aContactLink );

        
        /**
         * Remove contact from list
         * @param aContactId. Id of contact to be removed.
         */
        TInt RemoveContactFromCacheL( const TDesC& aContactId, TInt& index );
        
        
        /**
         * Remove contact from list
         * @param aContactLink. Contact Link of the item to be removed.
         */
        TInt RemoveContactFromCacheL( const MVPbkContactLink& aContactLink, TInt& index );

		 /**
         * add contact to list 
         * @param aContact. Contact  item to be added.
         * @param index of index
         */
		TInt AddStorageContactToCacheL( MVIMPSTStorageContact* aContact, TInt& aIndex );
       
        /**
         * Count of contacts in list. Skipp all ofline contacts.
         */
        TInt ContactCount( TBool aSkipOfflineContacts,
						   TBool aSkipBlocekedContacts ) const;

        /**
         * Count of online items in list
         * @return count of online contacts
         */
        TInt OnlineCount() const;

        
        /**
         * Get online list item by index from list
         * @param aIndex. Index of online item.
         * @return Online contact in index.
         */
        MVIMPSTStorageContact& OnlineContact( TInt aIndex ) const;

        
    private: // Own methods

        /**
         * Constructor
         */
        CVIMPSTStorageContactList( CVIMPSTStorageContactSorter& aSorter);
                        
        /**
         * Two-phased constructor
         */
        void ConstructL( const TDesC& aListId, const TDesC& aDisplayName );

       	/**
         * Find Contact based on contact link
         */
		TInt FindContactByLinkL( const MVPbkContactLink& aContactLink,
                                 TInt& aIndexOrderedArray ) const;
       
        /**
	     * Gets the index of given aContact
	     */
		TInt FindContactEntry( const MVIMPSTStorageContact* aContact ) const;

	    					 
        TInt FindContactIndex( const TDesC& aContactId,
                               TInt& aOrderedIndex ) const;

    private:    // Data

        /// Owns Stored contacts in contact list.
        RPointerArray< MVIMPSTStorageContact > iContactArray;

        /// Not owns Alphabeticaly ordered Contacts
        RPointerArray< MVIMPSTStorageContact > iOrderedContacts;

        /// Sorter for contacts.
        CVIMPSTStorageContactSorter& iSorter;

        /// Id of list
        HBufC* iListId;

        /// Displayname of list
        HBufC* iDisplayName;
       
    };

#endif      // CVIMPSTSTORAGECONTACTLIST_H

// End of File
