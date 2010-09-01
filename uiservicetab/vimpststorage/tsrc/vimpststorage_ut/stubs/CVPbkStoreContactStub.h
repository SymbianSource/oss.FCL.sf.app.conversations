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



#ifndef CVPbkStoreContactStub_H
#define CVPbkStoreContactStub_H


// INCLUDES
#include <MVPbkStoreContact.h>
#include <MVPbkViewContact.h>

// CLASS DECLARATIONS

/**
 * An interface for store contacts.
 *
 * A store contact is a contact that includes all the fields of
 * the contact. For this reason it usually contains more data compared
 * to the corresponding view contact. It can contain all types of fields that
 * are supported by the its parent store.
 *
 * The store contact can be edited if it's not read-only. The client must
 * first lock the existing contact then edit it and finally commit the changes.
 *
 * @see MVPbkContactStore
 * @see MVPbkViewContact
 */
class CVPbkStoreContactStub : public CBase,
                          public MVPbkStoreContact,
                          public MVPbkViewContact
    {
    public:  // Destructor
    	CVPbkStoreContactStub() ;
    
        ~CVPbkStoreContactStub() { }

    public: // From MVPbkBaseContact (covariant return types)
        const MVPbkStoreContactFieldCollection& Fields() const ;
     
     
     
    	/**
         * Returns the parent view of the contact.
         *
         * @return The parent view of the contact.
         */
        MVPbkContactViewBase& ParentView() const ;

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
        void ReadL( MVPbkContactObserver& aObserver ) const ;

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
        void ReadAndLockL( MVPbkContactObserver& aObserver ) const ;

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
        MVPbkExpandable* Expandable() const ;

        /**
         * Returns ETrue if two contacts are same.
         *
         * @param aOtherContact A contact to be compared.
         * @param aContactStore The store of the aOtherContact.
         * @return ETrue if two contacts are same.
         */
        TBool IsSame( const MVPbkViewContact& aOtherContact, 
                              const MVPbkContactStore* aContactStore ) const ;
        
        /**
         * Returns ETrue if two contacts are same.
         *
         * @param aOtherContact A contact to be compared.
         * @param aContactStore The store of the aOtherContact.
         * @return ETrue if two contacts are same.
         */                    
        TBool IsSame( const MVPbkStoreContact& aOtherContact, 
                              const MVPbkContactStore* aContactStore ) const ;    

    public:  // New functions
        

        /**
         * Returns this contact's parent store.
         *
         * @return The parent store of the contact.
         */
        MVPbkContactStore& ParentStore() const 
        { 
        MVPbkContactStore* ret=NULL;
        return *ret;
        };

        /**
         * Returns this contact's fields (read-write).
         *
         * @return A collection of contact fields.
         */
        MVPbkStoreContactFieldCollection& Fields();

        /**
         * Creates a new field for this contact.
         *
         * The new field must be added to the contact using AddFieldL.
         *
         * @param aFieldType  A type of the field to create. Must be found in
         *                    ParentStore().SupportedFieldTypes().
         * @return A new field object. The returned object is left on the
         *         cleanup stack.
         * @exception KErrNotSupported if the field type is not supported.
         * @exception KErrAccessDenied if the contact can not be modified.
         */
        MVPbkStoreContactField* CreateFieldLC(
                const MVPbkFieldType& /*aFieldType*/) const { return NULL; };

        /**
         * Adds a new field to the contact.
         * 
         * The field must be previously created with CreateFieldLC and
         * it must NOT be used after adding.
         *
         * If the client needs the field after adding it must be retrieved
         * using Fields().
         *
         * @param aField A new field that was created using CreateFieldLC.
         *               This object takes ownership of the field.
         * @precond aField must not be NULL or 
         *          VPbkError::Panic(VPbkError::ENullContactField) is raised.
         * @precond aField must be returned from this->CreateFieldLC or
         *          VPbkError::Panic(VPbkError::EInvalidContactField) panic is raised.
         * @postcond this->Fields().FieldCount() == 
         *           old(this->Fields().FieldCount()) + 1
         * @return The index of the new field in the field collection.
         * @exception KErrAccessDenied if the contact can not be modified.
         */
        TInt AddFieldL(MVPbkStoreContactField* /*aField*/) { return 0; };

        /**
         * Removes a field from the contact.
         *
         * @param aIndex A zero-based index of the field to remove.
         * @precond aIndex >= 0 && aIndex < FieldCount().
         *          Panics with VPbkError::EInvalidFieldIndex.
         * @precond The contact is not read-only otherwise panics with 
         *          VPbkError::EInvalidAccessToReadOnlyContact.
         * @postcond this->Fields().FieldCount() == 
         *           old(this->Fields().FieldCount()) - 1
         */
        void RemoveField(TInt /*aIndex*/) {};

        /**
         * Removes all the fields from the contact.
         *
         * @precond The contact is not read-only otherwise panics with 
         *          VPbkError::EInvalidAccessToReadOnlyContact.
         * @postcond this->Fields().FieldCount() =
         */
        void RemoveAllFields() {};

        /**
         * Locks this contact for modification asynchronously.
         *
         * Once the observer is notified this contact is locked and cab
         * be modified.
         *
         * @param aObserver The observer to call back when the operation
         *                  completes. The observer will not be called if this
         *                  function leaves.
         * @exception KErrInUse If another asynchronous operation is 
         *            already in progress.
         * @exception KErrAccessDenied if the contact can not be modified.
         */
        void LockL(MVPbkContactObserver& /*aObserver*/) const {};

        /**
         * Saves the contact to its associated store asynchronously.
         *
         * LockL must have been called before commit if this is
         * an existing contact. Otherwise ContactOperationFailed is called
         * with KErrAccessDenied.
         *
         * @param aObserver The observer to call back when this operation
         *                  completes. The observer will not be called if this
         *                  function leaves.
         * @exception KErrInUse If another asynchronous operation is already
         *            in progress.
         * @exception KErrAccessDenied if the contact can not be modified.
         */
        void CommitL(MVPbkContactObserver& /*aObserver*/) const {};

        /**
         * Returns the identifiers of the groups that the contact 
         * belongs to.
         *
         * @return The groups that this contact belongs to.
         */
        MVPbkContactLinkArray* GroupsJoinedLC() const { return NULL; };

        /**
         * Returns the group interface of the store contact if this contact
         * is a group.
         * If this contact is not a group, NULL is returned.
         *
         * @return The group interface or NULL.
         */
        MVPbkContactGroup* Group() { return NULL; };
        
        /**
         * Returns the maximum amount of fields of given type that can be
         * inserted to the contact.
         *
         * E.g. A USIM ADN contact can have 1 or more phone numbers but there
         * is a limit that the store in USIM defines.
         * On the other hand the contact in the Contacts Model data base
         * doesn't have limits.
         *
         * @param aType The field type of the field
         * @return The maximum amount fields of given type in the contact
         *         or KVPbkStoreContactUnlimitedNumber it there is no limit
         *         set by the store contact
         */
        TInt MaxNumberOfFieldL(const MVPbkFieldType& /*aType*/) const { return 0; };

        /**
         * Returns auxiliary properties.
         * Can return NULL if underlying store doesn't support any 
         * of MVPbkStoreContactProperties functions.
         *
         * @return A properties object
         */
        MVPbkStoreContactProperties* PropertiesL() const { return NULL; };
        
        /**
         * Set the contact as the store's current own contact asynchronously. 
         *
         * @param aObserver The observer to call back when this operation
         *                  completes. The observer will not be called if this
         *                  function leaves.
         * @exception KErrInUse If another asynchronous operation is already
         *            in progress.
         * @exception KErrNotSupported If store does not support own contact
         */
        void SetAsOwnL(MVPbkContactObserver& /*aObserver*/) const {};
        
        
        
        /**
         * Returns this contact's fields (read only).
         * @return This contact's fields in read only mode.
         */
       // const MVPbkBaseContactFieldCollection& Fields() const ;

        /**
         * Returns true if this a representation of the same contact.
         *
         * @param aOtherContact a contact this contact is compared against.
         * @return ETrue if this and aOtherContact represent the same contact.
         */
        TBool IsSame(const MVPbkBaseContact& aOtherContact) const ;

        /**
         * Returns true if this a representation of the same contact.
         *
         * @param aOtherContact a contact this contact is compared against.
         * @return ETrue if this and aOtherContact represent the same contact.
         */
        TBool IsSame(const MVPbkStoreContact& aOtherContact) const ;

        /**
         * Returns true if this a representation of the same contact.
         *
         * @param aOtherContact a contact this contact is compared against.
         * @return ETrue if this and aOtherContact represent the same contact.
         */
        TBool IsSame(const MVPbkViewContact& aOtherContact) const ;

        /**
         * Creates link representing this contact.
         *
         * @return A link or NULL if the contact doesn't exist in the store, 
         *         e.g. a folding contact in the view could be that kind or
         *         a new contact that hasn't been committed to the store.
         *         NULL is not put into the CleanupStack.
         */
        MVPbkContactLink* CreateLinkLC() const ;

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
        void DeleteL(MVPbkContactObserver& aObserver) const ;
        
        /**
         * Returns ETrue if this view contact is from given store
         *
         * @param aContactStoreUri the URI of the store to compare
         * @return ETrue if the view contact was from the given store
         */
        TBool MatchContactStore(
                const TDesC& aContactStoreUri) const ;
        
        /**
         * Returns ETrue if this contact is from the same store 
         * domain as the given one.
         *
         * @param aContactStoreDomain the the store domain
         * @return ETrue if the contact was from the same store domain.
         */
        TBool MatchContactStoreDomain(
                const TDesC& aContactStoreDomain) const ;
        
        /**
         * Creates and returns a bookmark that points to the contact.
         * Bookmark can be used to retrieve an index of the contact in 
         * the view.
         *
         * @return A new bookmark to the contact
         */
        MVPbkContactBookmark* CreateBookmarkLC() const ;
        
        /**
         * Returns the parent object of this object. For the root of the 
         * hierarchy returns self.
         * @return The parent object.
         */
        MVPbkObjectHierarchy& ParentObject() const;
        
    };


#endif  // CVPbkStoreContactStub

//End of file
