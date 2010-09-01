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



// INCLUDES

#include "CVPbkContactFieldTextData_stub.h"

_LIT(KTestData, "123@voip");

/**
 * Casts MVPbkContactFieldData to this interface.
 *
 * @precond aFieldData.DataType() == EVPbkFieldStorageTypeText
 *          VPbkError::Panic(VPbkError::EFieldDataTypeMismatch)
 *          is raised if the precondition does not hold.
 * @param aFieldData    Data of the field.
 * @return Casted inteface for the text field data.
 */
EXPORT_C MVPbkContactFieldTextData& MVPbkContactFieldTextData::Cast(
        MVPbkContactFieldData& aFieldData)
{
return static_cast<MVPbkContactFieldTextData&>(aFieldData);	
};

/**
 * Casts const MVPbkContactFieldData to this interface.
 *
 * @precond aFieldData.DataType() == EVPbkFieldStorageTypeText
 *          VPbkError::Panic(VPbkError::EFieldDataTypeMismatch)
 *          is raised if the precondition does not hold.
 * @param aFieldData    Data of the field.
 * @return Casted inteface for the text field data.
 */
EXPORT_C const MVPbkContactFieldTextData& MVPbkContactFieldTextData::Cast(
        const MVPbkContactFieldData& aFieldData)
{
return static_cast<const MVPbkContactFieldTextData&>(aFieldData);		
};

/**
 * Returns the string data.
 * @return Pointer to the text data.
 */
TPtrC CVPbkContactFieldTextData_stub::Text() const
{
return KTestData();	
}

/**
 * Sets the string data.
 * @exception KErrOverflow if (aText.Length() > MaxLength())
 */
void CVPbkContactFieldTextData_stub::SetTextL( const TDesC& /*aText*/ )
{
	
}

/**
 * Returns maximum length of the field.
 * @return Maximum length of the field or KVPbkUnlimitedFieldLength 
 *         if length is only limited by available memory.
 */
TInt CVPbkContactFieldTextData_stub::MaxLength() const
{
return 	KTestData().Length();
}

CVPbkContactFieldTextData_stub::~CVPbkContactFieldTextData_stub()
{
	
}

CVPbkContactFieldTextData_stub::CVPbkContactFieldTextData_stub()
{
	
}
/**
 * Returns the derived data type id.
 * @return Contact field storage type.
 */
TVPbkFieldStorageType CVPbkContactFieldTextData_stub::DataType() const
	{
	return EVPbkFieldStorageTypeText;	
	}

/**
 * Returns true if the storage is empty.
 * @return ETrue if the data field is empty.
 */
TBool CVPbkContactFieldTextData_stub::IsEmpty() const
{
return EFalse;	
}

/**
 * Copies data from given field data.
 * @param aFieldData    Data that is copied to this object.
 * @precond aFieldData.DataType() == this->DataType()
 */
void CVPbkContactFieldTextData_stub::CopyL(const MVPbkContactFieldData& /*aFieldData*/) 
{
	
}
   

// End of File
