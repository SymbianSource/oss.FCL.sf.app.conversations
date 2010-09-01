/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

// INCLUDE FILES
#include "s_storageitemmodel.h"
#include "s_vimpststoragecontact.h"


// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
MVIMPSTStorageItemModel* vimpststorageitemlist_stub::NewL()
    {
    MVIMPSTStorageItemModel* list = new(ELeave)vimpststorageitemlist_stub;
        return list;
    }


    
TInt vimpststorageitemlist_stub::Count() const
{
return 1;
}

           /**
            * Returns List item at given index.
            * @param aIndex
            * @return SItem List item
  
            */
MVIMPSTStorageItemModel::SItem vimpststorageitemlist_stub::Item( TInt aIndex  ) const
{
MVIMPSTStorageItemModel::SItem item;
item.iType = MVIMPSTStorageItemModel::EContactItem;
item.iContactList = NULL;
item.iContact = vimpststoragecontact_stub::NewL();
return item;
}
           
           
           /**
            * Get index for contact.
            * @param aContact. Contact item to be found.
            * @return TInt Index of contact. If contact not found returns KErrNotFound
            */
           TInt vimpststorageitemlist_stub::IndexOfContact( MVIMPSTStorageContact* aContact ) const 
           {
           return 1;
           }
           
           /**
            * Get index for list
            * @param aList. Contact item to be found.
            * @param aIgnoreOwnItem. If ETrue own item is not taken into account
            *                        when resolving index.
            * @param aIgnoreEmptyLists If ETrue, empty lists are ignored.
            * @return TInt Index of list. If list not found returns KErrNotFound
            */
           TInt vimpststorageitemlist_stub::IndexOfList( MVIMPSTStorageContactList* aList,
                                     TBool aIgnoreOwnItem ,
                                     TBool aIgnoreEmptyLists ) const
                                     {
                                     return 1;
                                     }
           
           // -----------------------------------------------------------------------------
           // CVIMPSTContactListModel::MdcaCount
           // (other items were commented in a header).
           // -----------------------------------------------------------------------------
           //
           TInt vimpststorageitemlist_stub::MdcaCount() const
               {
               
               return Count();
               }

           // -----------------------------------------------------------------------------
           // CVIMPSTContactListModel::MdcaPoint
           // (other items were commented in a header).
           // -----------------------------------------------------------------------------
           //
           TPtrC vimpststorageitemlist_stub::MdcaPoint( TInt /*aIndex */) const
               {
               
               // These will be filtered out
               return KNullDesC();
               } 

//  End of File  
