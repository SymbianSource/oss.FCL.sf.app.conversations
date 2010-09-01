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



#ifndef SCVPBKSTORECONTACTFIELD_H
#define SCVPBKSTORECONTACTFIELD_H


// INCLUDES
#include <e32base.h>
#include <MVPbkStoreContactField.h>
#include <MVPbkFieldType.h>

// CONSTANTS


// CLASS DECLARATIONS

/**
 * An interface for the field of the store contact.
 *
 * Client can access the contact field using this interface. The field
 * can have a label and a data.
 *
 * A new field can be created using the MVPbkStoreContact interface.
 * For looping the contact fields see MVPbkStoreContactFieldCollection.
 * The parent of the field in the object hierarchy is the store contact.
 *
 * @see CVPbkContactFieldTypeIterator
 * @see CVPbkFieldTypeSelector
 */
class CVPbkStoreContactFieldStub : public CBase,
                               public MVPbkStoreContactField,
                               public MVPbkFieldType
    {
    public:  // ctor
    	CVPbkStoreContactFieldStub(TInt aIndex);
        /**
         * Destructor
         */
        ~CVPbkStoreContactFieldStub();

    public: // From MVPbkBaseContactField
        const MVPbkContactFieldData& FieldData() const ;

    public:  // New functions
        /**
         * Returns ETrue if the field supports label. 
         * 
         * Overwrites the global setting in MVPbkContactStoreProperties. 
         * This must be confirmed before using SetFieldLabelL or 
         * MaxLabelLength.
         * 
         * @return ETrue if the field supports label. Otherwise EFalse.
         */
        TBool SupportsLabel() const ;
        
        /**
         * Returns the label of the field or KNullDesC if there is no field 
         * label.
         *
         * @return The label of the field or KNullDesC
         */
        TPtrC FieldLabel() const ;

        /**
         * Sets this field's label.
         *
         * SupportsLabel must be true for using this.
         *
         * @param aText The label for the field.
         */
        void SetFieldLabelL( const TDesC& aText ) ;
        
        /**
         * Gets the maximum length of the label. 
         *
         * SupportsLabel must be true for using this.
         * 
         * @return The maximum length of the label or KVPbkUnlimitedLabelLength
         *         if the store has no limits. Zero should be returned in other
         *         cases though this shouldn't be called if labels are 
         *         not supported.
         */
        TInt MaxLabelLength() const ;

        /**
         * Returns the data storage (read-write) of the field. 
         *
         * The data type depends on the field and it's client's responsibility
         * to check the type before casting the type.
         *
         * @return The data storage of the field.
         * @see MVPbkContactFieldTextData::Cast
         * @see MVPbkContactFieldDateTimeData::Cast
         * @see MVPbkContactFieldBinaryData::Cast
         */
        MVPbkContactFieldData& FieldData() ;

        /**
         * Clones the field. 
         * 
         * Pushes the created copy to the cleanup stack.
         *
         * @return A copy of the field.
         * @see MVPbkStoreContactFieldCollection::FieldAt
         */
        MVPbkStoreContactField* CloneLC() const ;
        
        /**
         * Creates a link representing the contact and the field.
         *
         * The field can be later retrieved using the RetrieveField of
         * MVPbkStoreContactFieldCollection interface.
         *
         * NOTE: implementations of stores are possibly using an index
         *       of the field as an identifier so clients should prefer not
         *       to save field links permanently. E.g modifying the contact
         *       can invalidate the link in some store implementations.
         *       A field link is practical in use cases where the link is
         *       created and immediately given to another component.
         *
         * @return A link representing the contact and the field or NULL
         *         if the contact doesn't exist in the store. E.g a new
         *         contact that hasn't been committed has no unique
         *         identifier yet.
         *         NULL is not put into the CleanupStack.
         */
        MVPbkContactLink* CreateLinkLC() const ;


    public:  // Interface
        /**
         * Returns the parent contact where this field is from.
         *
         * @return The parent contact where this field is from.
         */
        MVPbkBaseContact& ParentContact() const ;

        /**
         * Returns this field's type or NULL if no mapping exists
         * between the native type and phonebook type.
         *
         * @param aMatchPriority    matching priority to use.
         * @return The field type or NULL
         * @postcond !FieldType(list) || list.ContainsSame(*FieldType(list))
         */
        const MVPbkFieldType* MatchFieldType(
                TInt aMatchPriority) const ;
        
        /**
         * Returns the best matching type of the field or NULL 
         * if no mapping exists between the native type and 
         * phonebook type. This is the same as looping 
         * MatchFieldType from priority 0 and getting the first 
         * matched type.
         *
         * @return The field type or NULL
         * @postcond !FieldType(list) || list.ContainsSame(*FieldType(list))
         */
        const MVPbkFieldType* BestMatchingFieldType() const ;

        

        /**
         * Returns true if this field is the same as another field in the
         * contact instance. 
         * Doesn't work for fields from different contact instances.
         * Always use this method instead of direct pointer comparison.
         *
         * @param aOther another field inside the contact
         * @return ETrue if contact is same
         */
        TBool IsSame(const MVPbkBaseContactField& aOther) const ;

        
public:  // New functions
        /**
         * Returns the parent object of this object. For the root of the 
         * hierarchy returns self.
         * @return The parent object.
         */
        MVPbkObjectHierarchy& ParentObject() const ;
        
		/**
         * Returns the Versit properties mapped to this field type.
         *
         * @return Versit properties.
         */
        TArray<TVPbkFieldVersitProperty> VersitProperties() const ;

        /**
         * Returns the Versit parameters fields of this type should not have.
         *
         * @return Excluded parameters.
         */
        const TVPbkFieldTypeParameters& ExcludedParameters() const ;

        /**
         * Type name for field types not supported by the Versit 2.1 standard.
         *
         * @return A non-Versit type.
         */
        TVPbkNonVersitFieldType NonVersitType() const ;

        /**
         * Returns true if this field type is the same as aOtherType.
         *
         * @param aOtherType The type to compare.
         * @return ETrue if the field types are the same.
         */
        TBool IsSame(
                const MVPbkFieldType& aOtherType ) const ;

        /**
         * Returns true if this field type matches a Versit property.
         *
         * @param aMatchProperty A versit property to match against.
         * @param aMatchPriority A matching priority. Priorities start
         *                       from zero which is the highest priority.
         *                       Priority is also a direct index to
         *                       VersitProperties() array.
         * @return ETrue if this field type matches aMatchProperty at
         *         aMatchPriority. Returns always false if 
         *         aMatchPriority >= VersitProperties().Count().
         */
        TBool Matches(
                const TVPbkFieldVersitProperty& aMatchProperty,
                TInt aMatchPriority ) const ;

        /**
         * Returns the field type resource id from VPbkEng.rsg
         * that be used to identify a type.
         *
         * @return A field type resource id.
         */
        TInt FieldTypeResId() const ;   
        
	private:
		TInt iIndex ;
        
    };

#endif  // SCVPBKSTORECONTACTFIELD_H
        
// End of file
