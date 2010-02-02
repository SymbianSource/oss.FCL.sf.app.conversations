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
* Description:  MVIMPSTStorageItemModel
*
*/



#ifndef MVIMPSTSTORAGECONTACTLISTMODEL_H
#define MVIMPSTSTORAGECONTACTLISTMODEL_H

//  INCLUDES
#include "tvimpstenums.h"

#include <e32base.h>
#include <bamdesca.h>

// FORWARD DECLARATIONS
class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;

// CLASS DECLARATION

/**
 *  Contact list model for UI.
 *  Wrapper between UI and storage, hides multiple contact list handling logic.
 *
 *  @lib vimpststorage.lib
 *  @since 5.0
 */
class MVIMPSTStorageItemModel : public MDesCArray
    {

   public:	// Enumerations and typedefs
        
       /**
        * An Enum
        * To define Type of the item.
        * not used.
        */
		enum TItemType
			{
			EInvalid, /* Not a valid item*/
			EOwnStatus, /* own data item*/
			EContactList, /* Contact list item*/
			EContactItem, /* contact item*/
			EAll /* all item includes owndata, contactlist, contact */
			};
        
		/**
         * structure
         * To package the data of type of item(contact list/contact)
         * if its a contactl list the third variable iContact will be null.
         * not used.
         */
        struct SItem
			{
			TItemType iType;/*Type of the item*/
			MVIMPSTStorageContactList* iContactList; /*ocntact list*/
			MVIMPSTStorageContact* iContact;/*contact*/
			};

    public: // New functions

        /**
         * @return TInt Count of items
         */
        virtual TInt Count() const = 0;

		/**
		 * Returns List item at given index.
		 * @param aIndex
		 * @return SItem List item
		 */
        virtual SItem Item( TInt aIndex ) const = 0;
        
        
        /**
         * Get index for contact.
         * @param aContact. Contact item to be found.
         * @return TInt Index of contact. If contact not found returns KErrNotFound
         */
        virtual TInt IndexOfContact( MVIMPSTStorageContact* aContact ) const = 0;
        
        /**
         * Get index for list
         * @param aList. Contact item to be found.
         * @param aIgnoreOwnItem. If ETrue own item is not taken into account
         *                        when resolving index.
         * @param aIgnoreEmptyLists If ETrue, empty lists are ignored.
         * @return TInt Index of list. If list not found returns KErrNotFound
         */
        virtual TInt IndexOfList( MVIMPSTStorageContactList* aList,
                                  TBool aIgnoreOwnItem = EFalse,
                                  TBool aIgnoreEmptyLists = ETrue ) const = 0;

  
       
   
        
   public:  // From MDesCArray
    
        /**
        * @see MDesCArray
        */
        TPtrC MdcaPoint( TInt aIndex ) const = 0;
        
        /**
        * @see MDesCArray
        */
        TInt MdcaCount() const = 0;
        
    
    
	protected:	// prevent deletion through this interface

        /**
		 * Destructor.
		 */
        virtual ~MVIMPSTStorageItemModel() {};

    };

#endif      // MVIMPSTStorageContactListModel_H

// End of File
