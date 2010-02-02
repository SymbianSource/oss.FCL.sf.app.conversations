
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

#ifndef S_STORAGEITEMMODEL_H_
#define S_STORAGEITEMMODEL_H_


#include "mvimpststorageitemmodel.h"
class MVIMPSTStorageContact;
class MVIMPSTStorageContactList;
//  INCLUDES
#include <e32std.h>
#include <e32base.h>



class vimpststorageitemlist_stub : public MVIMPSTStorageItemModel
    {
      public: // New functions

          
          static MVIMPSTStorageItemModel* NewL();
          /**
             * @return TInt Count of items
             */
            TInt Count() const;

            /**
             * Returns List item at given index.
             * @param aIndex
             * @return SItem List item
             */
            MVIMPSTStorageItemModel::SItem Item( TInt aIndex ) const;
            
            
            /**
             * Get index for contact.
             * @param aContact. Contact item to be found.
             * @return TInt Index of contact. If contact not found returns KErrNotFound
             */
            TInt IndexOfContact( MVIMPSTStorageContact* aContact ) const;
            
            /**
             * Get index for list
             * @param aList. Contact item to be found.
             * @param aIgnoreOwnItem. If ETrue own item is not taken into account
             *                        when resolving index.
             * @param aIgnoreEmptyLists If ETrue, empty lists are ignored.
             * @return TInt Index of list. If list not found returns KErrNotFound
             */
            TInt IndexOfList( MVIMPSTStorageContactList* aList,
                                      TBool aIgnoreOwnItem = EFalse,
                                      TBool aIgnoreEmptyLists = ETrue ) const;
            
            /**
                   * @see MDesCArray
                   */
                   TInt MdcaCount() const;

                   /**
                   * @see MDesCArray
                   */
                   TPtrC MdcaPoint(TInt aIndex) const;

      
           
       
            
     

    };

#endif /* S_STORAGEITEMMODEL_H_ */

// End of File
