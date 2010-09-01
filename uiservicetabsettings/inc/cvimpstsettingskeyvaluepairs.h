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
* Description:  Container for individual key-value pairs.
*
*/

#ifndef __CVIMPSTSETTINGSKEYVALUEPAIRS_H
#define __CVIMPSTSETTINGSKEYVALUEPAIRS_H

//  INCLUDES

#include "cvimpstsettingskeyvaluepair.h"

#include <e32base.h>
#include <s32strm.h>


// FORWARD DECLARATIONS
class CVIMPSTSettingsLookupKeyValuePair;


// CLASS DECLARATION
/**
 * Container for individual key-value pairs.
 *
 * @since 3.0
 **/
NONSHARABLE_CLASS( CVIMPSTSettingsKeyValuePairs ): public CBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         *
         * @since 3.0
         */
         static CVIMPSTSettingsKeyValuePairs* NewL();


        /**
         * Destructor.
         */
         virtual ~CVIMPSTSettingsKeyValuePairs();


    private:  // Constructors


        /**
         * C++ default constructor.
         */
        CVIMPSTSettingsKeyValuePairs();

        /**
         * Symbian OS constructor.
         */
        void ConstructL();


    public: // Key value access


        /**
         * Key-value setters.
         *        
         *
         * @since 3.0
         * @param aKey The key to use identify the value.
         * @param aValue The value to assign for keyword.
         * @return Error code. One of following:
         * KErrNone: Value assigned/updated succesfully.
         * KErrGeneral: Different data type already assigned for given key.
         * KErrNoMemory: No memory to allocate the new value.
         *               Original value for key remains if assigning
         *               value fails,
         * KErrTooBig: Key and value are too big for storing. Original value
         *			   remains.
         *
         */        
        TInt SetValueDesC16( const TDesC& aKey, const TDesC16& aValue );


        /**
         * Key-value getters.
         *
         *
         * @since 3.0
         * @param aKey The key to use identify the value.
         * @param aValue The value to get. Given aValue isn't
         * changed if reading fails,
         * @return Error code. One of following:
         * KErrNone: Value read succesfully.
         * KErrNotFound: No value yet assigned.
         * KErrGeneral: Different data type assigned for given key.
         */        
        TInt GetValueDesC16( const TDesC& aKey, TPtrC16& aValue );


        /**
         * Key value remove.
         *
         * @since 3.0
         * @param aKey The key to remove.
         * @return Error code. One of following:
         * KErrNone: Key value removed succesfully.
         * KErrNotFound: Key value not found.
         */
        TInt DeletePair( const TDesC& aKey );

        /**
         * Return Value-pair array.
         *
         * @since 3.0
         * @return Array of value-pairs:
         */
        RPointerArray< CVIMPSTSettingsKeyValuePair >& Pairs();


    private:    // Data

        mutable CVIMPSTSettingsLookupKeyValuePair*   iLookupKey;  //OWN: Lookup pair

        RPointerArray< CVIMPSTSettingsKeyValuePair > iPairs;      //OWN: Pairs
    };

#endif      // __CVIMPSTSETTINGSKEYVALUEPAIRS_H


//  End of File



