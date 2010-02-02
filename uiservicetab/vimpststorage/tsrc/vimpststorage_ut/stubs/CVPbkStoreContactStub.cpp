/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CVPbkStoreContactStub.h"
#include "s_cVPbkBaseContactFieldCollection.h"
#include <MVPbkBaseContactFieldCollection.h>
#include "s_MVPbkContactLinkstub.h"

CVPbkBaseContactFieldCollectionStub gFeildCollection;

CVPbkStoreContactStub::CVPbkStoreContactStub()
{
	
}


MVPbkStoreContactFieldCollection& CVPbkStoreContactStub::Fields()
	{
	return gFeildCollection;	
	}
/**
 * Returns this contact's fields (read only).
 * @return This contact's fields in read only mode.
 */
const MVPbkStoreContactFieldCollection& CVPbkStoreContactStub::Fields() const 
	{
	return gFeildCollection;	
	}

/**
 * Returns true if this a representation of the same contact.
 *
 * @param aOtherContact a contact this contact is compared against.
 * @return ETrue if this and aOtherContact represent the same contact.
 */
TBool CVPbkStoreContactStub::IsSame(const MVPbkBaseContact& /*aOtherContact*/) const {return ETrue;}

/**
 * Returns true if this a representation of the same contact.
 *
 * @param aOtherContact a contact this contact is compared against.
 * @return ETrue if this and aOtherContact represent the same contact.
 */
TBool CVPbkStoreContactStub::IsSame(const MVPbkStoreContact& /*aOtherContact*/) const { return ETrue; }

/**
 * Returns true if this a representation of the same contact.
 *
 * @param aOtherContact a contact this contact is compared against.
 * @return ETrue if this and aOtherContact represent the same contact.
 */
TBool CVPbkStoreContactStub::IsSame(const MVPbkViewContact& /*aOtherContact*/) const { return ETrue; }

/**
 * Creates link representing this contact.
 *
 * @return A link or NULL if the contact doesn't exist in the store, 
 *         e.g. a folding contact in the view could be that kind or
 *         a new contact that hasn't been committed to the store.
 *         NULL is not put into the CleanupStack.
 */
MVPbkContactLink* CVPbkStoreContactStub::CreateLinkLC() const 
	{
	CVPbkContactLinkStub* contactLink = new (ELeave) CVPbkContactLinkStub();
	CleanupStack::PushL(contactLink);

	return contactLink;

	}

/**
 * Deletes this contact from store asynchronously.
 *
 * @param aObserver The observer to call back when this operation 
 *                  completes. The observer will not be called 
 *                  if this function leaves.
 * @exception KErrInUse If another asynchronous operation is already 
 *                      in progress.
 * @exception KErrAccessDenied if the contact can not be modified.
 */
void CVPbkStoreContactStub::DeleteL(MVPbkContactObserver& /*aObserver*/) const {}

/**
 * Returns ETrue if this view contact is from given store
 *
 * @param aContactStoreUri the URI of the store to compare
 * @return ETrue if the view contact was from the given store
 */
TBool CVPbkStoreContactStub::MatchContactStore(
        const TDesC& /*aContactStoreUri*/) const { return EFalse; }

/**
 * Returns ETrue if this contact is from the same store 
 * domain as the given one.
 *
 * @param aContactStoreDomain the the store domain
 * @return ETrue if the contact was from the same store domain.
 */
TBool CVPbkStoreContactStub::MatchContactStoreDomain(
        const TDesC& /*aContactStoreDomain*/) const { return EFalse; }

/**
 * Creates and returns a bookmark that points to the contact.
 * Bookmark can be used to retrieve an index of the contact in 
 * the view.
 *
 * @return A new bookmark to the contact
 */
MVPbkContactBookmark* CVPbkStoreContactStub::CreateBookmarkLC() const { return NULL; }

/**
 * Returns the parent object of this object. For the root of the 
 * hierarchy returns self.
 * @return The parent object.
 */
MVPbkObjectHierarchy& CVPbkStoreContactStub::ParentObject() const 
{
MVPbkObjectHierarchy* ret = NULL;

return *ret;	
}

/**
 * Returns the parent view of the contact.
 *
 * @return The parent view of the contact.
 */
MVPbkContactViewBase& CVPbkStoreContactStub::ParentView() const 
{
MVPbkContactViewBase* ret=NULL;
return *ret;	
}

/**
 * Reads the whole contact from the store asynchronously.
 *
 * A client can use this if it has a reference of the view contact but
 * it needs all the data fields instead of only those that are included
 * in the view contact.
 *
 * The contact is returned in a call back.
 *
 * @param aObserver An observer to call back when this operation 
 *                  completes. The observer will not be called if this
 *                  function leaves.
 * @exception KErrInUse If another asynchronous operation is already 
 *                      in progress.
 */
void CVPbkStoreContactStub::ReadL( MVPbkContactObserver& /*aObserver*/ ) const 
{
	
}

/**
 * Reads the whole contact and locks it for modification 
 * asynchronously.
 *
 * A client can use this if it has a reference of the view contact and
 * it needs to modify the contact.
 *
 * The contact is returned in a call back.
 *
 * @param aObserver An observer to call back when this operation 
 *                  completes. The observer will not be called if this
 *                  function leaves.
 * @exception KErrInUse If another asynchronous operation is already 
 *                      in progress.
 * @exception KErrAccessDenied if contact is read-only
 */
void CVPbkStoreContactStub::ReadAndLockL( MVPbkContactObserver& /*aObserver*/ ) const 
{
	
}

/**
 * Returns expandable interface for this object or NULL if expanding is 
 * not supported.
 *
 * Examples are a contact group that can be expanded to its members view
 * or a folding view that can be extended to its internals. Client can
 * get the type of the view from MVPbkContactViewBase::Type which is useful
 * for knowing the type of expansion.
 *
 * @return An expansion or NULL.
 */
MVPbkExpandable* CVPbkStoreContactStub::Expandable() const 
{
return NULL;	
};

/**
 * Returns ETrue if two contacts are same.
 *
 * @param aOtherContact A contact to be compared.
 * @param aContactStore The store of the aOtherContact.
 * @return ETrue if two contacts are same.
 */
TBool CVPbkStoreContactStub::IsSame( const MVPbkViewContact& /*aOtherContact*/, 
                      const MVPbkContactStore* /*aContactStore*/ ) const 
                      {
                      return EFalse;	
                      }

/**
 * Returns ETrue if two contacts are same.
 *
 * @param aOtherContact A contact to be compared.
 * @param aContactStore The store of the aOtherContact.
 * @return ETrue if two contacts are same.
 */                    
TBool CVPbkStoreContactStub::IsSame( const MVPbkStoreContact& /*aOtherContact*/, 
                      const MVPbkContactStore* /*aContactStore*/ ) const 
                      {
                      	return EFalse;
                      }
