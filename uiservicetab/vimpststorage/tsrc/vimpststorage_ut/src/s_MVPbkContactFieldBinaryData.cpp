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



#include "s_MVPbkContactFieldBinaryDatastub.h"

_LIT8(KTestBinaryData, "some binary dataJunk");

/**
 * Casts MVPbkContactFieldData to this interface.
 *
 * @precond aFieldData.DataType() == EVPbkFieldStorageTypeBinary
 *          VPbkError::Panic(VPbkError::EFieldDataTypeMismatch)
 *          is raised if the precondition does not hold.
 * @param aFieldData    Data of the field.
 * @return Casted inteface for the binary field data.
 */
EXPORT_C MVPbkContactFieldBinaryData& MVPbkContactFieldBinaryData::Cast(
        MVPbkContactFieldData& aFieldData )
{
return static_cast<MVPbkContactFieldBinaryData&>(aFieldData);	
};

/**
 * Casts const MVPbkContactFieldData to this interface.
 *
 * @precond aFieldData.DataType() == EVPbkFieldStorageTypeBinary
 *          VPbkError::Panic(VPbkError::EFieldDataTypeMismatch)
 *          is raised if the precondition does not hold.
 * @param aFieldData    Data of the field.
 * @return Casted inteface for the binary field data.
 */
EXPORT_C const MVPbkContactFieldBinaryData& MVPbkContactFieldBinaryData::Cast(
        const MVPbkContactFieldData& aFieldData )
{
return static_cast<const MVPbkContactFieldBinaryData&>(aFieldData);	
};
/**
 * Returns the binary data pointer.
 * @return Pointer to binary data.
 */
TPtrC8 CVPbkContactFieldBinaryDataStub::BinaryData() const
{
return KTestBinaryData();	
}
/**
 * Sets the binary data.
 * @exception KErrOverflow if (aBinaryData.Length() > MaxLength())
 * @param aBinaryData   Binary data to be set.
 */
void CVPbkContactFieldBinaryDataStub::SetBinaryDataL( const TDesC8& /*aBinaryData*/ )
{
	
}

/**
 * Returns the derived data type id.
 * @return Contact field storage type.
 */
TVPbkFieldStorageType CVPbkContactFieldBinaryDataStub::DataType() const
{
return EVPbkFieldStorageTypeBinary;	
}

/**
 * Returns true if the storage is empty.
 * @return ETrue if the data field is empty.
 */
TBool CVPbkContactFieldBinaryDataStub::IsEmpty() const
{
return EFalse;	
}

/**
 * Copies data from given field data.
 * @param aFieldData    Data that is copied to this object.
 * @precond aFieldData.DataType() == this->DataType()
 */
void CVPbkContactFieldBinaryDataStub::CopyL(const MVPbkContactFieldData& /*aFieldData*/) 
{
	
}
   
CVPbkContactFieldBinaryDataStub::CVPbkContactFieldBinaryDataStub()
{
	
}

CVPbkContactFieldBinaryDataStub::~CVPbkContactFieldBinaryDataStub()
{
	
}
// End of File
