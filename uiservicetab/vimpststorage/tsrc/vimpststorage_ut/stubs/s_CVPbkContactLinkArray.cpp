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
#include <CVPbkContactLinkArray.h>
/**
 * Creates an empty link array.
 *
 * @return A new instance of this class
 */
EXPORT_C  CVPbkContactLinkArray* CVPbkContactLinkArray::NewL(){ return new (ELeave) CVPbkContactLinkArray; }

/**
 * Creates an empty link array. Leaves the created array object on
 * the cleanup stack.
 *
 * @return A new instance of this class
 */
EXPORT_C  CVPbkContactLinkArray* CVPbkContactLinkArray::NewLC()
{ 
CVPbkContactLinkArray* self = new (ELeave) CVPbkContactLinkArray; 
CleanupStack::PushL(self);
return self;
}

/**
 * Creates a link array from a packed descriptor of links.
 *
 * @param aPackedLinks a buffer that was created using PackLC
 *        of MVPbkContactLinkArray or MVPbkContactLink
 * @param aStoreList the list of stores that contain the
 *        stores that links belong.
 * @return A new instance of this class
 */
EXPORT_C  CVPbkContactLinkArray* CVPbkContactLinkArray::NewLC(
        const TDesC8& /*aPackedLinks*/,
        const MVPbkContactStoreList& /*aStoreList*/)
	{ 
	CVPbkContactLinkArray* self = new (ELeave) CVPbkContactLinkArray; 
	CleanupStack::PushL(self);
	return self;
	}

/**
 * Creates a link array from a stream of links.
 *
 * @param aStream a stream of contact link that was created
 *                using MVPbkStreamable interface of
 *                MVPbkContactLink
 * @param aStoreList the list of stores that contain the
 *                stores that links belong.
 * @return A new instance of this class
 */
EXPORT_C  CVPbkContactLinkArray* CVPbkContactLinkArray::NewLC(
        RReadStream& /*aStream*/,
        const MVPbkContactStoreList& /*aStoreList*/)
        { 
		CVPbkContactLinkArray* self = new (ELeave) CVPbkContactLinkArray; 
		CleanupStack::PushL(self);
		return self;
		}

/**
 * Creates a link array from a stream of links. Uses aLoader
 * to find and load stores.
 * aLoader must exist the life time of this object.
 *
 * @param aStream a stream of contact link that was created
 *                using MVPbkStreamable interface of
 *                MVPbkContactLink
 * @param aLoader the contact store loader for internalization.
 * @return A new instance of this class
 */
 CVPbkContactLinkArray* CVPbkContactLinkArray::NewLC(
        RReadStream& /*aStream*/,
        MVPbkContactStoreLoader& /*aLoader*/ )
 		{ 
		CVPbkContactLinkArray* self = new (ELeave) CVPbkContactLinkArray; 
		CleanupStack::PushL(self);
		return self;
		}

/**
 * Destructor.
 */
CVPbkContactLinkArray::~CVPbkContactLinkArray(){ }


/**
 * Appends a link to this array.
 *
 * @param aLink the link object to append. This array takes ownership
 *              of the link if append is succesful.
 * @pre aLink != NULL
 * @post Array count is increased by one and the link is in the last 
 * element of the array.
 */
EXPORT_C void CVPbkContactLinkArray::AppendL(MVPbkContactLink* /*aLink*/){ }

/**
 * Inserts a link to this array.
 *
 * @param aLink the link object to insert. This array takes ownership
 *              of the link if insertion is succesful.
 * @param aIndex    position where to insert the link.
 * @exception KErrNoMemory  if out of memory.
 * @pre aLink != NULL
 * @pre aIndex >= 0 && aIndex <= Count()
 * @post Array count is increased by one and the link is in the position 
 * marked by the aIndex element of the array.
 */
EXPORT_C void CVPbkContactLinkArray::InsertL(MVPbkContactLink* /*aLink*/, TInt /*aIndex*/){ }

/**
 * Removes a link at aIndex from this array.
 *  
 * @pre aIndex >= 0 && aIndex <= Count()
 * @post The array count is decreased by one.
 */
EXPORT_C void CVPbkContactLinkArray::Remove(TInt /*aIndex*/){ }

/**
 * Removes and deletes a link at aIndex from this array.
 *
 * @pre aIndex >= 0 && aIndex <= Count()
 * @post The array count is decreased by one.
 */
EXPORT_C void CVPbkContactLinkArray::Delete(TInt /*aIndex*/){ }

/**
 * Resets the array and destroys the links
 */
EXPORT_C void CVPbkContactLinkArray::ResetAndDestroy(){ }

/**
 * Resets this array but doesn't destroy links
 * 
 * @post Count() == 0
 */
EXPORT_C void CVPbkContactLinkArray::Reset(){ }

/**
 * Returns the size of the packed link array
 */
EXPORT_C TInt CVPbkContactLinkArray::PackedBufferSize() const{ return 0; }


TInt CVPbkContactLinkArray::Count() const{ return 0; }
const MVPbkContactLink& CVPbkContactLinkArray::At(TInt /*aIndex*/) const
	{ 
	MVPbkContactLink* ret = NULL;
	return *ret;
	}

TInt CVPbkContactLinkArray::Find(const MVPbkContactLink& /*aLink*/) const{ return 0; }
HBufC8* CVPbkContactLinkArray::PackLC() const
{  
    HBufC8* temp = HBufC8::NewLC( 10 );
    //CleanupStack::PushL( temp );
    return temp;
}
const MVPbkStreamable* CVPbkContactLinkArray::Streamable() const
{ 
return NULL;
}


CVPbkContactLinkArray::CVPbkContactLinkArray(){ }
void CVPbkContactLinkArray::ConstructL(){ }
void CVPbkContactLinkArray::ConstructL(const TDesC8& /*aPackedLinks*/, 
        const MVPbkContactStoreList& /*aOperations*/){ }
void CVPbkContactLinkArray::ConstructL(RReadStream& /*aStream*/, 
        const MVPbkContactStoreList& /*aOperations*/){ }
TInt CVPbkContactLinkArray::DoCalculatePackedBufferSizeV2() const{ return 0; }
void CVPbkContactLinkArray::DoFillPackedBufferV2L(RWriteStream& /*aWriteStream*/) const{ }
void CVPbkContactLinkArray::InternalizeFromBufferL( RReadStream& /*aReadStream*/, 
        const MVPbkContactStoreList& /*aStoreList*/ ){ }
void CVPbkContactLinkArray::DoInternalizeFromBufferL( RReadStream& /*aReadStream*/, 
        const MVPbkContactStoreList& /*aStoreList*/){ }
void CVPbkContactLinkArray::DoInternalizeFromBufferV2L(RReadStream& /*aReadStream*/, 
        const MVPbkContactStoreList& /*aStoreList*/){ }

    
// End of file
