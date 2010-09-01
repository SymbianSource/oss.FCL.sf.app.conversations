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



#ifndef MVIMPSTSTORAGECONTACTLIST_H
#define MVIMPSTSTORAGECONTACTLIST_H

//  INCLUDES
#include "tvimpstenums.h"

#include <e32base.h>

// FORWARD DECLARATIONS
class MVIMPSTStorageContact;
class MVPbkContactLink;
// CLASS DECLARATION

/**
 *  Contact list data interface
 *
 *  @lib vimpststorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageContactList
    {
    public: // Definitions
        /**
         * An Enum
         * to define the sync state of the contact list.
         * not used.
         */
 	enum TSynchroniseState
            {
            ESynchroniseNotDone, /* sync not done */
            ESynchroniseDone, /* sync done */
            ESynchroniseFailed /* sync failed. */ 
            };
    
    public: // New functions
    
        /**
         * List id of contact list
         * @return TDesC& List id of contact list.
         */
        virtual const TDesC& ListId() const = 0;
        
        /**
         * Displayname of contact list
         * @return TPtrC Display name of list. 
         *          If displayname is not set, Id part of list id is returned.    
         */
        virtual TPtrC DisplayName() const = 0;
        
       
        /**
         * Count of items in list
         * @return count of contacts.
         */
        virtual TInt Count() const = 0;

        /**
         * Get list item by index from list.
         * @param  aIndex. Index of item.
         * @return MVIMPSTStorageContact& Contact in index.
         */
        virtual MVIMPSTStorageContact& operator[]( TInt aIndex ) const = 0;

        /**
         * Get Filtered list item by index from list
         * @param aIndex. Index of Filtered item.
         * @param aFilter. Filters the contacts according to this filter.
         * @return MVIMPSTStorageContact& contact in index upon the filter.
         */
        virtual MVIMPSTStorageContact& FilteredContact( 
                TInt aIndex, TVIMPSTEnums::TFilterType aFilter ) const = 0;

        /**
         * Count of non-blocked items in list
         * @return TInt count of non-blocked contacts
         */         
        virtual TInt FilteredCount( TVIMPSTEnums::TFilterType aFilter ) const = 0;
        
       /**
        * Sort the contact list
        */
        virtual void Sort()= 0;
               
        /**
         * Find index of contact in list.
         * @param aContact. Contact item to be found.
         * @param aFilter. Filters the contacts according to this filter.
         * @return TInt index of contact or KErrNotFound, if contact is not found.
         */
        virtual TInt FindIndexOfContact( 
                        const MVIMPSTStorageContact* aContact,
                        TVIMPSTEnums::TFilterType aFilter = 
                                TVIMPSTEnums::EFilterAll ) const = 0;
                                
        /*
         * Checks if the contact should be shown according to the filter
         * @param aContact Contact to check
         * @param aFilter Tells which contacts should be shown
         * @return TBool ETrue if contact should be shown according 
         *         to the given filter. EFalse otherwise.
         */
        virtual TBool FilterAllowsContact( const MVIMPSTStorageContact* aContact,
                        TVIMPSTEnums::TFilterType aFilter ) const = 0;
                        
        /**
         * Resorts the contacts in the list according the presence updation.
         * @param aContact contact id whose presence has been updated.
         */
        virtual void ResortContact( MVIMPSTStorageContact* aContact ) = 0;
        
        
         /**
         * add contact to list 
         * @param aContact. Contact  item to be added.
         * @param index of index
         */
        virtual TInt AddStorageContactToCacheL( MVIMPSTStorageContact* aContact, TInt& aIndex ) = 0 ;
        
       
  		 /**
         * Remove contact from list
         * @param aContactLink. Contact Link of the item to be removed.
         */
        virtual TInt RemoveContactFromCacheL( const MVPbkContactLink& aContactLink, TInt& index ) = 0 ;

    protected:  // Constructors and destructor

        /**
         * Destructor.
         */
        ~MVIMPSTStorageContactList(){};
    };


#endif      // MVIMPSTSTORAGECONTACTLIST_H

// End of File
