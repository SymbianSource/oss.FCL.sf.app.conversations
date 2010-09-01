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



#ifndef CVPBKCONTACTFIELDBINARYDATASTUB_H
#define CVPBKCONTACTFIELDBINARYDATASTUB_H

// INCLUDES
#include <MVPbkContactFieldBinaryData.h>


// CLASS DECLARATIONS

/**
 * Virtual Phonebook binary field data.
 * This interface is used to manipulate binary contact field data.
 */
class CVPbkContactFieldBinaryDataStub : public MVPbkContactFieldBinaryData
    {
    public:  // Destructor
        /**
         * Destructor.
         */
        ~CVPbkContactFieldBinaryDataStub();
        
        CVPbkContactFieldBinaryDataStub();


    
    public:  // Interface
    
        /**
         * Returns the binary data pointer.
         * @return Pointer to binary data.
         */
        TPtrC8 BinaryData() const ;

        /**
         * Sets the binary data.
         * @exception KErrOverflow if (aBinaryData.Length() > MaxLength())
         * @param aBinaryData   Binary data to be set.
         */
        void SetBinaryDataL( const TDesC8& aBinaryData ) ;

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

#endif  // CVPBKCONTACTFIELDBINARYDATASTUB_H

// End of File
