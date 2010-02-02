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



#include <cvpbkcontactmanager.h>

#include "CVPbkContactOperationBaseStub.h"
#include "s_CVPbkContactStoreList.h"

// CLASS DECLARATIONS

/**
 * Virtual Phonebook Contact Manager. Contact Manager is the root access point
 * to Virtual Phonebook engine functionality. The client can specify the list
 * of stores in the constructor he/she is interested in or load additional 
 * plug-ins later on. In addition to loading the implementing plug-ins the 
 * client must open the stores. Clients should share the contact manager 
 * instance within their context, otherwise e.g. field types from two 
 * different instances will not produce the matches as the matching is
 * based on pointer comparison.
 *
 * Examples of absolute URI stores:
 * Default contact database: "cntdb://contacts.cdb"
 * LDAP store: "ldap://www.telnumbers.com/"
 * The URI scheme name is used to resolve a store capable of supporting 
 * this type of store. The actual store implementations are ECom plugins.
 *
 *  @lib VPbkEng.lib
 */

TSecurityInfo gTSecurityInfo;
CVPbkContactOperationBaseStub* gOperationStub;
CVPbkContactStoreListStub gStoreList;

/**
 * Creates a contact manager instance and loads the plug-ins that can 
 * handle the stores corresponding to the aURIList.
 * @param aURIList  Universal Resource Identifier list of the stores 
 *                  to be loaded initially. 
 * @param aFs       File system handle. NULL opens a new connection.
 * @return	Contact manager instance.
 */
EXPORT_C CVPbkContactManager* CVPbkContactManager::NewL(
        const CVPbkContactStoreUriArray& /*aURIList*/,
        RFs* aFs /*= NULL*/)
		{ 
		return new( ELeave ) CVPbkContactManager( gTSecurityInfo, aFs);
		}
        
/**
 * Creates a contact manager instance and loads the plug-ins that can 
 * handle the stores corresponding to the aURIList.
 * @param aSecurityInfo Security info from caller to be passed for stores.
 *                      Stores can check security info and deside
 *                      if client can use service.
 * @param aURIList      Universal Resource Identifier list of the stores 
 *                      to be loaded initially. 
 * @param aFs           File system handle. NULL opens a new connection.
 * @return              Contact manager instance.
 */
EXPORT_C CVPbkContactManager* CVPbkContactManager::NewL(
        TSecurityInfo aSecurityInfo,
        const CVPbkContactStoreUriArray& /*aURIList*/,
        RFs* aFs /*= NULL*/)
	{ 
	return new( ELeave ) CVPbkContactManager( aSecurityInfo, aFs);
	}

/**
 * Destructor.
 */
CVPbkContactManager::~CVPbkContactManager(){ }


/**
 * The contact manager owns a master list of field types that clients
 * use. The store implementation then maps their native types
 * to Virtual Phonebook types. Clients don't create field type objects
 * themselves but use always references to types offered 
 * by this function.
 *
 * @return The global list of field types.
 */
EXPORT_C const MVPbkFieldTypeList& CVPbkContactManager::FieldTypes() const
	{ 
	MVPbkFieldTypeList* retValue = NULL;
	return *retValue;

	}

/**
 * The contact store list is used to handle a set of stores.
 * After creation of manager the client can use the list to open
 * all stores.
 *
 * @return The list of available contact stores.
 */
EXPORT_C MVPbkContactStoreList& CVPbkContactManager::ContactStoresL(){ return gStoreList; }

/**
 * Loads a store plug-in for given URI and adds aURI 
 * to the list of stores handled by this manager.
 * If there is no store plug-in for the aURI then nothing changes.
 *
 * @param aURI the store URI to be loaded.
 */
EXPORT_C void CVPbkContactManager::LoadContactStoreL(const TVPbkContactStoreUriPtr& /*aURI*/){ }

/**
 * Attribute manager is used for handling contact attributes like
 * speed dialing or defaults.
 *
 * @returns The contact attribute manager.
 */
EXPORT_C MVPbkContactAttributeManager& CVPbkContactManager::ContactAttributeManagerL()
	{ 
	MVPbkContactAttributeManager* ret = NULL;
	return *ret;
	}

/**
 * Creates a new contact view specified by the view definition.
 * The view can not be used before it has notified the observer 
 * that it is ready.
 *
 * @param aObserver the observer for view events.
 * @param aViewDefinition Definition of the view to create
 * @param aSortOrder a list of field types that defines fields
 *        that are used in sorting. View contacts
 *        have these fields. However, the store can have restrictions
 *        for types that can be used in the sort order.
 *        The sort order has no effect for shared views that have
 *        already been created.
 *
 * @see CVPbkSortOrder
 * @return Newly created view. 
 *         If NULL CleanupStack pop is still needed
 */
EXPORT_C MVPbkContactViewBase* CVPbkContactManager::CreateContactViewLC(
        MVPbkContactViewObserver& /*aObserver*/,
        const CVPbkContactViewDefinition& /*aViewDefinition*/, 
        const MVPbkFieldTypeList& /*aSortOrder*/) const
        { 
        return NULL;
        }

/**
 * Retrieves asynchronously a contact that is individualized by 
 * a contact link. Delete the operation for canceling.
 *
 * @param aLink a link to the contact
 * @param aObserver operation observer
 * @return New handle to the operation
 * @Asynchronous
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::RetrieveContactL(
        const MVPbkContactLink& /*aLink*/,
        MVPbkSingleContactOperationObserver& /*aObserver*/) const
        { 
        gOperationStub = new( ELeave ) CVPbkContactOperationBaseStub();
        return gOperationStub;
        }

/**
 * Creates an array of links corresponding to a packaged link or links.
 * Packed links have been obtained by packing a link or a link array
 * and they can be used for example for IPC but not for saving links. 
 * Loads the store plug-ins that are capable of creating the 
 * appropriate links.
 *
 * @param aPackedLinks packed links
 * @return A link array.
 * @see MVPbkContactLink::PackLC
 * @see MVPbkContactLinkArray::PackLC
 */
EXPORT_C MVPbkContactLinkArray* CVPbkContactManager::CreateLinksLC(
    const TDesC8& /*aPackedLinks*/) const{ return NULL; }

/**
 * Internalizes a permanent link or links. An externalized link
 * has been created using MVPbkStreamable interface from
 * MVPbkContactLink.
 * Loads the store plug-ins that are capable of creating the 
 * appropriate links.
 *
 * @param aStream a stream that contains a link or links.
 * @return A link array
 * @see MVPbkContactLink::Streamable
 */
EXPORT_C MVPbkContactLinkArray* CVPbkContactManager::CreateLinksLC(
    RReadStream& /*aStream*/) const{ return NULL; }
    
/**
 * Deletes contacts defined in link array asynchronously.
 *
 * @param aContactLinks Contacts to delete
 * @param aObserver Operation observer
 * @return New handle to the operation
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::DeleteContactsL(
        const MVPbkContactLinkArray& /*aContactLinks*/, 
        MVPbkBatchOperationObserver& /*aObserver*/){ return NULL; }

/**
 * Commits all contacts in aContacts asynchronously.
 *
 * @see MVPbkStoreContact::CommitL
 * @param aContacts Contacts to commit.
 * @param aObserver Operation observer.
 * @return New handle to the operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::CommitContactsL(
        const TArray<MVPbkStoreContact*>& /*aContacts*/, 
        MVPbkBatchOperationObserver& /*aObserver*/){ return NULL; }

/**
 * Copies all contacts in aContactLinks to aTargetStore asynchronously. 
 * If aTargetStore is NULL, contacts are copied to the store where 
 * they already are, ie. the contacts are duplicated.
 *
 * @param aContactLinks Array of contact links to duplicate.
 * @param aTargetStore Target store to copy contacts to.
 *                     If NULL this behaves like duplicate.
 * @param aObserver Observer for the copying process.
 * @return New handle to the operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::CopyContactsL(
        const MVPbkContactLinkArray& /*aContactLinks*/,
        MVPbkContactStore* /*aTargetStore*/,
        MVPbkBatchOperationObserver& /*aObserver*/){ return NULL; }

/**
 * Searches the contact stores for a phone number using 
 * defined amount of digits from the end of the number asynchronously.
 * The store implementations determine, which field types are included 
 * in the search. 
 * 
 * @param aPhoneNumber Phone number to search for.
 * @param aMaxMatchDigits Maximum number of digits to match 
 * 						  from the end of the number.
 * @param aObserver Observer for the find process.
 * @return New handle to the find operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::MatchPhoneNumberL(
        const TDesC& /*aPhoneNumber*/, 
        TInt /*aMaxMatchDigits*/,
        MVPbkContactFindObserver& /*aObserver*/){ return NULL; }

/**
 * Searches the contact stores for a contact that contains
 * the given string in one of the field defined by given 
 * field type list asynchronously.
 * NOTE: In some cases the find matches also other fields than 
 *       those specified in aFieldTypes. Always loop through the
 *       results to check match in the required fields.
 *
 * @param aSearchString String to search for.
 * @param aFieldTypes List of field types that the search will include.
 *                    a field type list: use CVPbkFieldTypeRefsList and
 *                    append the types that are needed from 
 *                    the master field type list (FieldTypes()) 
 *                    to the reference list. Selection can be done
 *                    in a dynamic way using CVPbkFieldTypeSelector or
 *                    static way using resource ids of the field types.
 * @param aObserver Observer for the find process.
 * @return New handle to the find operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::FindL(
        const TDesC& /*aSearchString*/,
        const MVPbkFieldTypeList& /*aFieldTypes*/,
        MVPbkContactFindObserver& /*aObserver*/){ return NULL; }

/**
 * Finds a string containing text that is stored in one or more 
 * fields asynchronously. Client can give multiple find words. 
 * All the words must match to separated data. E.g. if there are 
 * two find words: "Jo" and "Jo" then field data "John Johnson" 
 * matches but "John Doe" doesn't if the word parser uses
 * white space as a word separator.
 *
 * NOTE: The accuracy of the results depends on the implementation 
 * of the store to carry out the find operation. The performance 
 * can also vary depending on the store. 
 *
 * @param aSearchStrings strings that are compared to field data.
 * @param aFieldTypes types of the fields that are used. Constructing
 *                    a fieldtype list: use CVPbkFieldTypeRefsList and
 *                    append the types that are needed from 
 *                    the master fieldtype list (FieldTypes()) 
 *                    to the reference list. Selection can be done
 *                    in a dynamic way using CVPbkFieldTypeSelector or
 *                    static way using resource ids of the fieldtypes.
 * 
 * @param aObserver an observer for asynchronous operation.
 * @param aWordParserCallBack a client implementation of word parser 
 *                            function that separates the field data 
 *                            into words. Parameter to function is 
 *                            TVPbkWordParserParam.
 * @return New handle to the find operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::FindL(
        const MDesC16Array& /*aSearchStrings*/, 
        const MVPbkFieldTypeList& /*aFieldTypes*/, 
        MVPbkContactFindFromStoresObserver& /*aObserver*/, 
        const TCallBack& /*aWordParserCallBack*/ ){ return NULL; }

/**
 * Compresses all stores asynchronously.
 * NOTE: Not all stores are able to implement compression to their
 * repositories.
 * 
 * @param aObserver Operation observer.
 * @return New handle to the operation.
 */
EXPORT_C MVPbkContactOperationBase* CVPbkContactManager::CompressStoresL(
		MVPbkBatchOperationObserver& /*aObserver*/){ return NULL; }

/**
 * Returns the file server session of the contact manager.
 * 
 * @return File server session of the contact manager.
 */
EXPORT_C RFs& CVPbkContactManager::FsSession()
	{ 
	RFs* rfs = NULL;
	return *rfs;
	}

    
CVPbkContactManager::CVPbkContactManager( TSecurityInfo /*aSecurityInfo*/, RFs* /*aFs*/ )
	{
		
	}


void CVPbkContactManager::ConstructL(const CVPbkContactStoreUriArray& /*aURIList*/)
	{
		
	}


MVPbkContactStore* CVPbkContactManager::FindStore(const TVPbkContactStoreUriPtr& /*aURI*/) const
	{
	return NULL;	
	}


MVPbkContactViewBase* CVPbkContactManager::CreateOptimizedCompositeViewLC(
						MVPbkContactViewObserver& /*aObserver*/,
						const CVPbkContactViewDefinition& /*aViewDefinition*/, 
						const MVPbkFieldTypeList& /*aSortOrder*/) const
	{
	return NULL;	
	}

MVPbkContactViewBase* CVPbkContactManager::CreateCompositeViewLC(
						MVPbkContactViewObserver& /*aObserver*/,
						const CVPbkContactViewDefinition& /*aViewDefinition*/, 
						const MVPbkFieldTypeList& /*aSortOrder*/) const
{
return NULL;	
}

// End of file
