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
* Description:  settings container.
*
*/

#ifndef __CVIMPSTSETTINGS_H
#define __CVIMPSTSETTINGS_H

//  INCLUDES
#include <e32base.h>
#include <s32strm.h>


class CVIMPSTSettingsKeyValuePairs;
 
/*
 * - Key-value pairs.
 *   Key-value pair mechanism for generic Settings extensions.
 * 
 * @since 3.0
 */

class CVIMPSTSettings : public CBase
    {

    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         *
         * @since 3.0
         */
        static CVIMPSTSettings* NewL();


        /**
         * Two-phased constructor.
         *
         * @since 3.0
         */
        static CVIMPSTSettings* NewLC();


        /**
         * Destructor.
         */
        virtual ~CVIMPSTSettings();


    private:  // Constructors

        /**
         * C++ default constructor.
         */
        CVIMPSTSettings();


    public: //SAP extension

        /**
         * Key-value pair setters.
         *
         * Stores  arbitrary data for given keyword.
         * Key-value pairs are stored to flat data structure
         * in side the target SAP. Different data types
         * may not share same key name.
         * Keyword may not contain string "||". Also, the
         * maximum length for key+value is
         * NCentralRepositoryConstants::KMaxUnicodeStringLength-5
         *
         * @since 3.0
         * @param aKey Keyword to use to identify the value.
         * @param aValue Descriptor / integer value to store.
         * @return KErrNone: Storing the key value was succesful.
         *         KErrGeneral: Different data type was previously
         *         associated with given key. Old value will remain
         *         KErrNoMemory: Out of memory. Old value will remain
         *		   KErrTooBig: Key+value are too big for storing
         */
        
        TInt SetOpaqueDesC16( const TDesC& aKey, const TDesC16& aValue );


        /**
         * Key-value pair getters.
         *
         * Gets data previously stored for given keyword.
         *
         * @since 3.0
         * @param aKey Keyword to identify the value.
         * @param aValue Pointer descriptor / integer where the value is get.
         *        Pointer descriptor is put to point to requested data.
         *        Data is owned by the SAP container and stays valid
         *        untill the container object is destroyed or new
         *        value for key is assigned.
         * @return KErrNone: Retrieving the key value was succesful.
         *         KErrNotFound: No value associated with given key.
         *         KErrGeneral: Different data type associated with key.
         */

        TInt GetOpaqueDesC16( const TDesC& aKey, TPtrC16& aValue ) const;


        /**
         * Key-value pair cleanup.
         *
         * Deletes previously stored key-value pair.
         *
         * @since 3.0
         * @param aKey Keyword to identify the key-value pair.
         * @return KErrNone: Key-value pair removed.
         *         KErrNotFound: Key-value pair not found.
         */
        TInt DeleteOpaque( const TDesC& aKey );
        
        /**
         * return keyvalue pairs 
         */
        CVIMPSTSettingsKeyValuePairs& KeyValuePairs();


    private:    // Data
       
        CVIMPSTSettingsKeyValuePairs*  iKeyValuePairs; ///< OWN: key value pairs

    };

#endif      // __CVIMPSTSETTINGS_H

//  End of File

