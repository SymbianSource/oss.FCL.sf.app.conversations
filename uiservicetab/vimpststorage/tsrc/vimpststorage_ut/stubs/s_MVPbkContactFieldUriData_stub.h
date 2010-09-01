/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/



#ifndef CVPBKCONTACTFIELDURIDATASTUB_H
#define CVPBKCONTACTFIELDURIDATASTUB_H


// INCLUDES
#include <MVPbkContactFieldUriData.h>

// CLASS DECLARATIONS

/**
 * Virtual Phonebook Contact field URI data.
 * This interface is used to manipulate URI contact field data.
 */
class CVPbkContactFieldUriData_Stub : public MVPbkContactFieldUriData
    {
    public:  // Destructor
        /**
         * Destructor.
         */
        ~CVPbkContactFieldUriData_Stub();
		
		CVPbkContactFieldUriData_Stub();
    
    public:  // Interface
        
        /**
         * Returns the URI scheme.
         * @return Pointer to the URI scheme.
         */
        TPtrC Scheme() const ;

        /**
         * Returns the URI .
         * @return Pointer to the URI text.
         */
        TPtrC Text() const ;

        /**
         * Returns the string data.
         * @return Pointer to the whole URI.
         */
        TPtrC Uri() const ;

        /**
         * Sets the URI data.
         * @exception KErrOverflow if (aUri.Length() > MaxLength())
         */
        void SetUriL(const TDesC& aUri) ;

        /**
         * Sets the URI data.
         * @exception KErrOverflow if (aScheme.Length() + aText.Length()
         *              + 1 > MaxLength())
         */
        void SetUriL(const TDesC& aScheme, const TDesC& aText) ;

        /**
         * Returns maximum length of the field.
         * @return Maximum length of the field or KVPbkUnlimitedFieldLength 
         *         if length is only limited by available memory.
         */
        TInt MaxLength() const ;
        
        /**
         * Returns the derived data type id.
         * @return Contact field storage type.
         */
        TVPbkFieldStorageType DataType() const;

        /**
         * Returns true if the storage is empty.
         * @return ETrue if the data field is empty.
         */
        TBool IsEmpty() const;

        /**
         * Copies data from given field data.
         * @param aFieldData    Data that is copied to this object.
         * @precond aFieldData.DataType() == this->DataType()
         */
        void CopyL(const MVPbkContactFieldData& aFieldData) ;

        
    };

#endif  // CVPBKCONTACTFIELDURIDATASTUB_H

// End of File
