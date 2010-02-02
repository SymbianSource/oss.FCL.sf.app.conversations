/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "s_CVPbkBaseContactFieldCollection.h"
#include "s_CVPbkStoreContactField.h"

TInt gGiveFieldCount = 0;
CVPbkStoreContactFieldStub gField0(0);
CVPbkStoreContactFieldStub gField1(1);
CVPbkStoreContactFieldStub gField2(2);
CVPbkStoreContactFieldStub gJunkField(-1);


CVPbkBaseContactFieldCollectionStub::CVPbkBaseContactFieldCollectionStub(){}

/**
 * Destructor.
 */
CVPbkBaseContactFieldCollectionStub::~CVPbkBaseContactFieldCollectionStub(){}

    
 const MVPbkStoreContactField& CVPbkBaseContactFieldCollectionStub::FieldAt(TInt /*aIndex*/) const 
 { 
 MVPbkStoreContactField* ret = NULL;
 return *ret;
 }


/**
 * Returns a field in this collection.
 *
 * @param aIndex A zero-based index of the field to return.
 * @return A reference to the field at aIndex. Reference is valid until 
 *         FieldAt is called again. If you need a permanent copy,
 *         call MVPbkStoreContactField::CloneLC to the returned
 *         field.
 * @see MVPbkStoreContactField::CloneLC
 * @precond aIndex >= 0 && aIndex < FieldCount()
 *          VPbkError::Panic(VPbkError::EInvalidFieldIndex) is raised
 *          if the precondition does not hold.
 */
MVPbkStoreContactField& CVPbkBaseContactFieldCollectionStub::FieldAt( TInt aIndex ) 
	{ 
	if (aIndex == 0)
		{
		return gField0;	
		}

	if (aIndex == 1)
		{
		return gField1;	
		}

	if (aIndex == 2)
		{
		return gField2;	
		}

	return gJunkField;
	}

/**
 * Returns a copy of the field in this collection.
 *
 * Client gets the ownership of the field.
 *
 * @param aIndex A zero-based index of the field to return.
 * @return  A new instance to the field at aIndex. The field is valid
 *          as long as the parent contact is valid
 * @precond aIndex >= 0 && aIndex < FieldCount()
 *          VPbkError::Panic(VPbkError::EInvalidFieldIndex) is raised
 *          if the precondition does not hold.
 */
MVPbkStoreContactField* CVPbkBaseContactFieldCollectionStub::FieldAtLC( TInt /*aIndex*/ ) const { return NULL; }

/**
 * Returns the parent contact of the field collection
 *
 * @return the parent contact of the field collection
 */
MVPbkStoreContact& CVPbkBaseContactFieldCollectionStub::ParentStoreContact() const 
{ 
MVPbkStoreContact* ret = NULL;
return *ret;
}

/**
 * Returns a field in this collection identified by a contact field 
 * link.
 *
 * A contact field link can be created using the MVPbkStoreContactField
 * interface. The same link works then as a contact link and 
 * a field link.
 *
 * NOTE: implementations of stores are possibly using an index
 *       of the field as an identifier so clients should prefer not
 *       to save field links permanently. E.g modifying the contact
 *       can invalidate the link in some store implementations.
 *       A field link is practical in use cases where the link is
 *       created and immediately given to another component.
 *
 * @param aContactLink A valid contact field link.
 * @return A field in this collection identified by aContactLink or NULL
 *          if the link does not contain field information or if the 
 *          link does not refer to the parent contact of this field collection.
 */
MVPbkStoreContactField* CVPbkBaseContactFieldCollectionStub::RetrieveField(
    const MVPbkContactLink& /*aContactLink*/ ) const { return NULL; }



/**
 * Returns the parent contact of this field set.
 * @return The parent contact of this field set.
 */
MVPbkBaseContact& CVPbkBaseContactFieldCollectionStub::ParentContact() const 
{ 
MVPbkBaseContact* ret=NULL;
return *ret;
}

/**
 * Returns the number of fields in this collection.
 * @return The number of fields in this collection.
 */
TInt CVPbkBaseContactFieldCollectionStub::FieldCount() const { return gGiveFieldCount; }

/**
 * Returns a read-only field in this collection.
 *
 * @param aIndex    Zero-based index of the field to return.
 * @return  Reference to a field at aIndex. Reference is 
 *          valid until FieldAt is called again.
 * @see MVPbkBaseContactFieldCollection::ConstFieldAt
 * @precond aIndex >= 0 && aIndex < FieldCount()
 *          Panic VPbkError::EInvalidFieldIndex is raised if the
 *          precondition does not hold.
 */
//const MVPbkBaseContactField& CVPbkBaseContactFieldCollectionStub::FieldAt(
  //      TInt aIndex) const { }
//End of file
