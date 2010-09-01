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
#include <e32std.h>
#include "s_CVPbkContactStoreList.h"
#include "CVPbkStoreContactStub.h"
#include "s_CVPbkContactView.h"

CVPbkContactStoreStub gLocalStore;
CVPbkContactStoreStub gXSPStore;

// FORWARD DECLARATIONS

/**
 * Returns fixed properties of this contact store.
 *
 * Fixed properties do not change when the store is open. 
 * The store must be opened before retrieving the properties.
 *
 * @return Store properties.
 */
const MVPbkContactStoreProperties& CVPbkContactStoreStub::StoreProperties() const 
{ 
MVPbkContactStoreProperties* ret = NULL;
return *ret;
}

/**
 * Opens this contact store asynchronously.
 *
 * Calls back the observer when the opening completes. Notice
 * that the same store instance can be opened by several observers.
 *
 * @param aObserver An observer for the store.
 * @exception KErrInUse If another asynchronous operation
 *            is already in progress.
 */
void CVPbkContactStoreStub::OpenL(
        MVPbkContactStoreObserver& /*aObserver*/ ) { }

/**
 * Replaces an existing store and opens it asynchronously. 
 * 
 * E.g. If the database is a file then this replaces the database if
 * it's not locked by other clients. If the store 
 * implementation can not implment replacing then this behaves 
 * like OpenL.
 *
 * If the store does not exist, it is created if possible.
 * Calls back the observer when the opening completes.
 *
 * @param aObserver An observer for the store.
 */
void CVPbkContactStoreStub::ReplaceL(
        MVPbkContactStoreObserver& /*aObserver*/ ) { }

/**
 * Closes this contact store from a single observer. 
 * 
 * This can be always called safely even if OpenL or ReplaceL
 * hasn't been called. 
 * If the client calls OpenL it must also call this after usage,
 * The observer will no longer receive events from this store.
 * If there are other observers for the store then the store
 * will remain open for them.
 *
 * @param aObserver An observer that was given in OpenL or ReplaceL.
 */
void CVPbkContactStoreStub::Close(
        MVPbkContactStoreObserver& /*aObserver*/ ) { }

/**
 * Creates a new contact associated to this store.
 * 
 * The contact can be edited and then it must be committed by
 * calling MVPbkStoreContact::CommitL for actually saving
 * it to the store.
 *
 * @return A new contact associated to this store. Caller takes
 *         ownership of the returned contact.
 * @see MVPbkStoreContact
 * @see MVPbkStoreContact::CommitL
 */
MVPbkStoreContact* CVPbkContactStoreStub::CreateNewContactLC() 
{ 
return new (ELeave) CVPbkStoreContactStub;
}

/**
 * Creates a new contact group associated to this store. 
 * 
 * MVPbkContactStoreProperties::SupportsContactGroups must be
 * true if this is used.
 * The contact group might be saved to the store immeadiately
 * depending on the store implementation. 
 * It is left open for editing. Use CommitL to commit 
 * the creation of the group and its content.
 *
 * @return A new contact group associated to this store. Caller takes
 *         the ownership of the returned contact group.
 * @exception KErrNotSupported if the store doesn't support groups.
 *              Client should check store properties before
 *              calling this.
 * @see MVPbkContactStoreProperties
 * @see MVPbkContactGroup
 * @see MVPbkStoreContact
 * @see MVPbkStoreContact::CommitL
 */
MVPbkContactGroup* CVPbkContactStoreStub::CreateNewContactGroupLC() { return NULL; }

/**
 * Creates a new contact view from the store asynchronously.
 *
 * Client gets the ownership of the view. The content of
 * the view depends on the CVPbkContactViewDefinition. The
 * client must wait the observer event before using the view.
 *
 * @param aViewDefinition Defines the properties of the new view.
 * @param aObserver An observer for the view events.
 * @param aSortOrder Field types that are used in the view contact
 *                   in the same order as in this list. Notice that
 *                   stores may not support all possible field types
 *                   in a view contact. The implementation
 *                   of the view contact must have as many fields as
 *                   the sort order. If the store doesn't support the
 *                   field type in a view contact then it sets empty
 *                   data to the field.
 * @return  A new contact view object. Caller takes ownership of the
 *          returned contact.
 * @see MVPbkContactView
 * @see CVPbkContactViewDefinition
 */
MVPbkContactView* CVPbkContactStoreStub::CreateViewLC(
        const CVPbkContactViewDefinition& /*aViewDefinition*/,
        MVPbkContactViewObserver& /*aObserver*/,
        const MVPbkFieldTypeList& /*aSortOrder*/ ) 
        { 
        CVPbkContactViewStub* some = new (ELeave) CVPbkContactViewStub;
        CleanupStack::PushL(some);
        return some;
        }
    
/**
 * Returns contact groups contained in this store.
 *
 * MVPbkContactStoreProperties::SupportsContactGroups must be
 * true if this is used. Implementation should return an empty
 * link array and not NULL.
 *
 * @return Contact group identifiers contained in this store.
 */
MVPbkContactLinkArray* CVPbkContactStoreStub::ContactGroupsLC() const { return NULL; }

/**
 * Returns a contact store information. Information can vary
 * runtime.
 *
 * @return Contact store information.
 */
const MVPbkContactStoreInfo& CVPbkContactStoreStub::StoreInfo() const 
{ 
MVPbkContactStoreInfo* ret = NULL;
return *ret;
}

/**
 * This is part of Virtual Phonebook internal framefork and not
 * usable for clients. Clients use CVPbkContactManager 
 * for creating links from a stream.
 *
 * Creates a link array from a stream. Stream contains the internals
 * of the contact link. Internals are the contact store implementation
 * specific part of the package format.
 *
 * NOTE: this must work wheter the OpenL has been called or not. This
 *       means that a link can not contain any data that would need
 *       an open store before internalizing.
 *
 * @param aStream A stream containing the link internals.
 * @return A new contact link.
 * @internal
 */
MVPbkContactLink* CVPbkContactStoreStub::CreateLinkFromInternalsLC(
        RReadStream& /*aStream*/ ) const { return NULL; }

/**
* Returns own contact link asynchronously.
* 
* @params aObserver Operation observer
* @return operation handle
* @Asynchronous
*/
MVPbkContactOperationBase* CVPbkContactStoreStub::OwnContactLinkL(
	   MVPbkSingleContactLinkOperationObserver& /*aObserver*/) const { return NULL; }
    

/**
 * Returns the parent object of this object. For the root of the 
 * hierarchy returns self.
 * @return The parent object.
 */
MVPbkObjectHierarchy& CVPbkContactStoreStub::ParentObject() const 
{ 
MVPbkObjectHierarchy* ret=NULL;
return *ret;
}


/**
 * Creates an operation for contact retrieval.
 *
 * @param aLink A link to the contact
 * @param aObserver An observer for the operation.
 * @return A handle to the operation or NULL if the factory
 *         doesn't have reference to the store of the link.
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateContactRetrieverL(
        const MVPbkContactLink& /*aLink*/,
        MVPbkSingleContactOperationObserver& /*aObserver*/) { return NULL; }

/**
 * Creates an operation for deleting multiple contacts.
 *
 * @param aContactLinks An array of contact links that specify 
 *                      the contacts.
 * @param aObserver An observer for the operation.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateDeleteContactsOperationL(
        const MVPbkContactLinkArray& /*aContactLinks*/, 
        MVPbkBatchOperationObserver& /*aObserver*/ ) { return NULL; }

/**
 * Creates an operation for committing multiple contacts.
 *
 * @param aContacts An array of contact links that specify
 *                  the contacts
 * @param aObserver An observer for the operation.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateCommitContactsOperationL(
        const TArray<MVPbkStoreContact*>& /*aContacts*/,
        MVPbkBatchOperationObserver& /*aObserver*/) { return NULL; }

/**
 * Creates an operation for asynchronous phone number matching.
 * 
 * @param aPhoneNumber  A phone number to search for.
 * @param aMaxMatchDigits   Maximum number of digits to match from 
 *                          the end of the number.
 * @param aObserver Observer for the find process.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateMatchPhoneNumberOperationL(
        const TDesC& /*aPhoneNumber*/, 
        TInt /*aMaxMatchDigits*/,
        MVPbkContactFindObserver& /*aObserver*/) { return NULL; }

/**
 * Creates an operation for asynchronous contact finding.
 *
 * @param aSearchString String to search for.
 * @param aFieldTypes   List of field types that the search will include.
 * @param aObserver Observer for the find process.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateFindOperationL(
        const TDesC& /*aSearchString*/,
        const MVPbkFieldTypeList& /*aFieldTypes*/,
        MVPbkContactFindObserver& /*aObserver*/) { return NULL; }

/**
 * Creates an operation for asynchronous contact finding.
 * Finds a string containing text that is stored in one or more fields.
 * Client can give multiple find words. All the words must match to separated
 * data. E.g if there are two find words: "Jo" and "Jo" then field data
 * "John Johnson" matches but "John Doe" doesn't if the word parser uses
 * white space as a word separator.
 *
 * NOTE: The accuracy of the results depends on the ability of the store
 *       implementation to implement the find. The performance can vary
 *       too depending on the store. 
 *
 * @param aSearchStrings    Words that are compared to field data.
 * @param aFieldTypes   Types of the fields that are used.
 * @param aObserver An observer for asynchronous operation.
 * @param aWordParserCallBack a client implementation of word parser 
 *                            function that separates the field data 
 *                            into words. Parameter to function is 
 *                            TVPbkWordParserParam.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateFindOperationL(
        const MDesC16Array& /*aSearchStrings*/, 
        const MVPbkFieldTypeList& /*aFieldTypes*/, 
        MVPbkContactFindFromStoresObserver& /*aObserver*/, 
        const TCallBack& /*aWordParserCallBack*/ ) { return NULL ;}
    
/**
 * Creates an operation for compressing the store.
 *
 * @param aObserver An observer for the compress operation.
 * @return A handle to the operation or NULL
 */
MVPbkContactOperation* CVPbkContactStoreStub::CreateCompressStoresOperationL(
		MVPbkBatchOperationObserver& /*aObserver*/) { return NULL; }
 

/**
 * Returns the number of stores in the list.
 *
 * @return The number of stores in the list.
 */
TInt CVPbkContactStoreListStub::Count() const { return 0; }

/**
 * Returns the store in given index in the list.
 *
 * @param aIndex the index of the store in the list
 * @precond aIndex > && aIndex < Count()
 *              VPbkError::Panic(VPbkError::EInvalidStoreIndex)
 *              is raised if the precondition does not hold.
 * @return The store in the specified index.
 */
MVPbkContactStore& CVPbkContactStoreListStub::At(TInt /*aIndex*/) const 
{ 
MVPbkContactStore* ret =NULL;
return *ret;
}

/**
 * Finds the contact store corresponding to the aUri.
 *
 * @param aUri the store URI to search for.
 * @return The contact store corresponding to the aUri or NULL.
 */
MVPbkContactStore* CVPbkContactStoreListStub::Find(
    const TVPbkContactStoreUriPtr& /*aUri*/) const 
    { 
    return &gXSPStore;
    }

/**
 * Opens all stores in the list asynchronously.
 *
 * A client can not use stores before it has successfully opened
 * them. CloseAll must be always called by the same observer that has
 * opened stores.
 *
 * NOTE: There can be only one observer for one store list at a time.
 *
 * @param aObserver An observer for the stores.
 * @exception KErrInUse If one observer is currently opening this list.
 */
void CVPbkContactStoreListStub::OpenAllL(MVPbkContactStoreListObserver& /*aObserver*/) { }

/**
 * Closes all stores in the list.
 *
 * This is safe to call in all circumstances. Implementations
 * can not trust that OpenAllL has been called before.
 *
 * @param aObserver An observer for the close process.
 */
void CVPbkContactStoreListStub::CloseAll(MVPbkContactStoreListObserver& /*aObserver*/) { }



//End of file
