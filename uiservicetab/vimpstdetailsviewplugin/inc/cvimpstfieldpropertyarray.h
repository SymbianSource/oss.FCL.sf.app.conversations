/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Field property array for service details view field types.
 *
*/

 
#ifndef CVIMPSTFIELDPROPERTYARRAY_H
#define CVIMPSTFIELDPROPERTYARRAY_H

// INCLUDE FILES
#include <e32base.h>
#include "cvimpstdetailscontacthandler.h"

// FORWARD DECLARATIONS
class TResourceReader;
class MVPbkFieldTypeList;
class CVIMPSTFieldProperty;
class MVPbkStoreContact;
// CLASS DECLARATION

/**
 * Field property array for service details view field types.
 * part of vimpstdetailsviewplugin.dll
 * @since s60 v5.0 
 */
class CVIMPSTFieldPropertyArray : public CBase
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aSupportedFieldTypeList  , A list of field types.
         *                                  If this is the master fieldtype
         *                                  list then all the properties
         *                                  are loaded, otherwise only those
         *                                  properties that have the field
         *                                  type in the list are loaded.
         * @param aContact ,  store contact 
         * @param aReader, resource reader reference
         * @aParam aPresenceSupported, check for presence supported or not
         * @return  A new instance of this class.
         * @since s60 v5.0 
         */
        static CVIMPSTFieldPropertyArray* NewL(
                const MVPbkFieldTypeList& aSupportedFieldTypeList,MVPbkStoreContact& aContact,
                TResourceReader& aReader ,
                TBool aPresenceSupported ,
                TDesC& aServiceName, TStoreType aStoreType);

        
        /**
         * Destructor.
         * @since s60 v5.0 
         */
        ~CVIMPSTFieldPropertyArray();

    public: 
    
    	/**
         * @return the field count.
         * @since s60 v5.0 
         */
        TInt Count() const;
        
        /**
         * @param aIndex , index of field
         * @return the field located at aIndex .
         * @since s60 v5.0 
         */
        CVIMPSTFieldProperty& At(TInt aIndex ) const;
        
        /**
         * @param aFieldId , index of field
         * @return the field find field by id
         * @since s60 v5.0 
         */
        CVIMPSTFieldProperty* GetFieldById( TInt aFieldId ) ;
        

    private: // Implementation
    	
    	/**
         * @param aFieldId , index of field
         * @return the field find field by id
         * @since s60 v5.0 
         */
        CVIMPSTFieldPropertyArray(MVPbkStoreContact& aContact );
        
        /**
         * provide two phase contructions
         * @param aSupportedFieldTypeList  , A list of field types.
         *                                  If this is the master fieldtype
         *                                  list then all the properties
         *                                  are loaded, otherwise only those
         *                                  properties that have the field
         *                                  type in the list are loaded.
         * @param aReader, resource reader reference
         * @aParam aPresenceSupported, check for presence supported or not
         * @since s60 v5.0 
         */
        void ConstructL(
                const MVPbkFieldTypeList& aSupportedFieldTypeList,
                TResourceReader& aReader,
                TBool aPresenceSupported,
                TDesC& aServiceName, TStoreType aStoreType);
                
        /**
         * helper function to read the feld from resource
         * @param aSupportedFieldTypeList  , A list of field types.
         *                                  If this is the master fieldtype
         *                                  list then all the properties
         *                                  are loaded, otherwise only those
         *                                  properties that have the field
         *                                  type in the list are loaded.
         * @param aReader, resource reader reference
         * @aParam aPresenceSupported, check for presence supported or not
         * @since s60 v5.0 
         */       
        void ReadFieldPropertiesL( TResourceReader& aReader,
                const MVPbkFieldTypeList& aSupportedFieldTypeList,
                TBool aPresenceSupported,
                TDesC& aServiceName, TStoreType aStoreType);
              
    private: // Data
        /// Own: Field properties
        RPointerArray<CVIMPSTFieldProperty> iFieldProperties;
        
        // Not Owns : reference to StoreContact
        MVPbkStoreContact& iContact;
        
    };

#endif // CVIMPSTFIELDPROPERTYARRAY_H

// End of File
