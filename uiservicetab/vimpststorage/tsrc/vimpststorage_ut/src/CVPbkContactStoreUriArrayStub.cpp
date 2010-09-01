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



#include <CVPbkContactStoreUriArray.h>

#include <tvpbkcontactstoreuriptr.h>
/**
 * Creates a new instance of this class.
 * @return Newly created instance of this class.
 */
EXPORT_C CVPbkContactStoreUriArray* CVPbkContactStoreUriArray::NewL()
{
return new (ELeave) CVPbkContactStoreUriArray();
}

/**
 * Creates a new instance of this class. Leaves
 * the instance to the cleanup stack.
 * @return Newly created instance of this class.
 */
EXPORT_C CVPbkContactStoreUriArray* CVPbkContactStoreUriArray::NewLC(){ return NULL; }

/**
 * Creates a new instance of this class internalizing the
 * contents of the descriptor.
 * @param aPackedUris	Packed URIs created by using the 
 * 						PackLC functionality of this class.
 * @return Newly created instance of this class.
 */
EXPORT_C CVPbkContactStoreUriArray* CVPbkContactStoreUriArray::NewLC(const TDesC8& /*aPackedUris*/){ return NULL; }
        
/**
 * Destructor.
 */
CVPbkContactStoreUriArray::~CVPbkContactStoreUriArray(){}


/**
 * Appends StoreUriPtr to this array.
 *
 * @param aStoreUri	Store URI to add array.
 */
EXPORT_C void CVPbkContactStoreUriArray::AppendL(const TVPbkContactStoreUriPtr& /*aStoreUri*/){}

/**
 * Appends array of StoreUriPtr to this array.
 *
 * @param aUriArray	Array of Store URIs to add array.
 */
EXPORT_C void CVPbkContactStoreUriArray::AppendAllL(const CVPbkContactStoreUriArray& /*aUriArray*/){}

/**
 * Removes StoreUriPtr from this array.
 *
 * @param aStoreUri Store Uri to add array.
 */
EXPORT_C void CVPbkContactStoreUriArray::Remove(const TVPbkContactStoreUriPtr& /*aStoreUri*/){}

/**
 * Returns the count of Store URIs in this array.
 *
 * @return Count of Store URIs in array.
 */
EXPORT_C TInt CVPbkContactStoreUriArray::Count() const{ return 0; }

/**
 * Gets the URI pointer descriptor from this array at 
 * the given index.
 *
 * @param aIndex Index which URI Ptr is needed
 * @return URI pointer descriptor.
 */
EXPORT_C TVPbkContactStoreUriPtr CVPbkContactStoreUriArray::operator[](TInt /*aIndex*/) const
{
TVPbkContactStoreUriPtr ret;
return ret;
}

/**
 * Packs this URI array to a heap descriptor.
 *
 * @return Packed URI array in a heap descriptor.
 */        
EXPORT_C HBufC8* CVPbkContactStoreUriArray::PackLC() const{ return NULL; }

/**
 * Checks whether the given URI is already in this array.
 *
 * @param aStoreUri Store URI to check.
 * @return ETrue if Store URI is in this array, EFalse if not.
 */        
EXPORT_C TBool CVPbkContactStoreUriArray::IsIncluded(const TVPbkContactStoreUriPtr& /*aStoreUri*/) const{ return EFalse; }

CVPbkContactStoreUriArray::CVPbkContactStoreUriArray(){}
void CVPbkContactStoreUriArray::ConstructL(){}
void CVPbkContactStoreUriArray::ConstructL(const TDesC8& /*aPackedUris*/){}
TInt CVPbkContactStoreUriArray::CalculateExternalizedSize() const{ return 0; }

// End of File
