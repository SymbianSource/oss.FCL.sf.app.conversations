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



#ifndef CVPBKCONTACTFIELDTEXTDATASTUB_H
#define CVPBKCONTACTFIELDTEXTDATASTUB_H


// INCLUDES
#include <MVPbkContactFieldTextData.h>

// CLASS DECLARATIONS

/**
 * Virtual Phonebook Contact field text data.
 * This interface is used to manipulate text contact field data.
 */
class CVPbkContactFieldTextData_stub : public MVPbkContactFieldTextData
    {
    public:  // Destructor
        /**
         * Destructor.
         */
        ~CVPbkContactFieldTextData_stub();
        CVPbkContactFieldTextData_stub();


    public:  // Interface
        
        /**
         * Returns the string data.
         * @return Pointer to the text data.
         */
        TPtrC Text() const;

        /**
         * Sets the string data.
         * @exception KErrOverflow if (aText.Length() > MaxLength())
         */
        void SetTextL( const TDesC& aText ) ;

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

#endif  // CVPBKCONTACTFIELDTEXTDATASTUB_H

// End of File
