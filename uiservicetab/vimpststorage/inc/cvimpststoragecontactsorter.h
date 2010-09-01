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
* Description:  Contact sorter
*
*/


#ifndef CVIMPSTSTORAGECONTACTSORTER_H
#define CVIMPSTSTORAGECONTACTSORTER_H

//  INCLUDES
#include <e32base.h>
#include "mvimpststoragecontact.h"

// CLASS DECLARATION

/**
*  contact items array sorter
*  Changes used sorting algorithm based on setting item value. 
*  It also listen to changes
*  in setting item value.  
*  Offers also an API to compare two contacts.
*
*/
NONSHARABLE_CLASS( CVIMPSTStorageContactSorter ) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CVIMPSTStorageContactSorter* NewL();

        /**
        * Destructor.
        */
        ~CVIMPSTStorageContactSorter();

    public: //New Methods

        /**
        * Sorts given array.
        * @param aList Reference to list which is sorted.
        */
        void Sort( RPointerArray< MVIMPSTStorageContact >& aList );

        /**
        * Returns which given contact is first in list.
        * @param aFirst Reference to first contact.
        * @param aSecond Reference to second contact.
        * @return Integer, -1 if aFirst is greater, 0 if equal, 
        *                   1 if aSecond is greater.
        */
        TInt Compare( const MVIMPSTStorageContact& aFirst, 
            const MVIMPSTStorageContact& aSecond ) const;
            
        /**
         * Set the Sort Algorithm 
         * @param aAlgorithm - ECompareAlphabetically/ECompareByPresence
         */         
        void SetSortAlgorithm( MVIMPSTStorageContact::TSortAlgorithm aAlgorithm );
        
        /** 
         * Gets the algorithm chosen 
         * returns the algorithm used for sorting
         */
        TLinearOrder< MVIMPSTStorageContact >& InsertOrder();
        
    private:
        /**
        * C++ default constructor.
        */
        CVIMPSTStorageContactSorter();

    private:    // Data

        // This contains a pointer to a function used in sorting and comparing.
        TLinearOrder< MVIMPSTStorageContact > iAlgorithm;
    };

#endif // CVIMPSTSTORAGECONTACTSORTER_H

//End Of File

