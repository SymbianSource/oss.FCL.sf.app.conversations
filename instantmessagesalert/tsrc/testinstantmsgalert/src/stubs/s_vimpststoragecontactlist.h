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
#include "TVIMPSTEnums.h"
#include "MVIMPSTStorageContactList.h"

// FORWARD DECLARATIONS
class CVIMPSTStorageContactSorter;
class MVIMPSTStorageContactsObserver;
class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;

// CLASS DECLARATION

/**
 *  Contact list data implementation
 */
class MyVIMPSTStorageContactListStub : public MVIMPSTStorageContactList
    {  
    
    public:
	    MyVIMPSTStorageContactListStub()
	    {
	    	
	    }
     /**
     * Destructor.
     */
      ~MyVIMPSTStorageContactListStub()
	    {
	    	
	    }

  

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
        void SetDisplayNameL( const TDesC& aDisplayName );
        

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
        TInt FilteredCount(
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
        void ResortUnKnownContact( MVIMPSTStorageContact* aContact );
        
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
   
         };

#endif      // CVIMPSTSTORAGECONTACTLIST_H

// End of File
