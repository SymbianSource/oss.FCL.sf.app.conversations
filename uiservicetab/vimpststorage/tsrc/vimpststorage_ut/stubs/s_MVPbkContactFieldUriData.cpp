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


#include "s_MVPbkContactFieldUriData_stub.h"

_LIT(KTestUriData, "gizmo:someuser@gizmo.com");

/**
 * Casts MVPbkContactFieldData to this interface.
 *
 * @precond aFieldData.DataType() == EVPbkFieldStorageTypeUri
 *          VPbkError::Panic(VPbkError::EFieldDataTypeMismatch)
 *          is raised if the precondition does not hold.
 * @param aFieldData    Data of the field.
 * @return Casted inteface for the text field data.
 */
EXPORT_C MVPbkContactFieldUriData& MVPbkContactFieldUriData::Cast(
        MVPbkContactFieldData& aFieldData)
{
return static_cast<MVPbkContactFieldUriData&>(aFieldData);	
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
EXPORT_C const MVPbkContactFieldUriData& MVPbkContactFieldUriData::Cast(
        const MVPbkContactFieldData& aFieldData)
{
return static_cast<const MVPbkContactFieldUriData&>(aFieldData);	
};

/**
 * Returns the URI scheme.
 * @return Pointer to the URI scheme.
 */
TPtrC CVPbkContactFieldUriData_Stub::Scheme() const
{
return KNullDesC();	
}

/**
 * Returns the URI .
 * @return Pointer to the URI text.
 */
TPtrC CVPbkContactFieldUriData_Stub::Text() const 
{
return KTestUriData();	
}

/**
 * Returns the string data.
 * @return Pointer to the whole URI.
 */
TPtrC CVPbkContactFieldUriData_Stub::Uri() const
{
return KNullDesC();	
}

/**
 * Sets the URI data.
 * @exception KErrOverflow if (aUri.Length() > MaxLength())
 */
void CVPbkContactFieldUriData_Stub::SetUriL(const TDesC& /*aUri*/)
{
	
}

/**
 * Sets the URI data.
 * @exception KErrOverflow if (aScheme.Length() + aText.Length()
 *              + 1 > MaxLength())
 */
void CVPbkContactFieldUriData_Stub::SetUriL(const TDesC& /*aScheme*/, const TDesC& /*aText*/)
{
	
}

/**
 * Returns maximum length of the field.
 * @return Maximum length of the field or KVPbkUnlimitedFieldLength 
 *         if length is only limited by available memory.
 */
TInt CVPbkContactFieldUriData_Stub::MaxLength() const
{
return 1;	
}


/**
 * Returns the derived data type id.
 * @return Contact field storage type.
 */
TVPbkFieldStorageType CVPbkContactFieldUriData_Stub::DataType() const
	{
	return EVPbkFieldStorageTypeUri;	
	}

/**
 * Returns true if the storage is empty.
 * @return ETrue if the data field is empty.
 */
TBool CVPbkContactFieldUriData_Stub::IsEmpty() const
{
return EFalse;	
}

/**
 * Copies data from given field data.
 * @param aFieldData    Data that is copied to this object.
 * @precond aFieldData.DataType() == this->DataType()
 */
void CVPbkContactFieldUriData_Stub::CopyL(const MVPbkContactFieldData& /*aFieldData*/) 
{
	
}
   
CVPbkContactFieldUriData_Stub::CVPbkContactFieldUriData_Stub()
{
	
}

CVPbkContactFieldUriData_Stub::~CVPbkContactFieldUriData_Stub()
{
	
}

// End of File
