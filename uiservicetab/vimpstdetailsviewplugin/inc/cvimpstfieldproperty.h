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
* Description:  a single field with property 
 *
*/


#ifndef CVIMPSTFIELDPROPERTY_H
#define CVIMPSTFIELDPROPERTY_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class TResourceReader;
class MVPbkFieldTypeList;
class CVIMPSTFieldPropertyArray;
class MVPbkStoreContactField;
class MVPbkFieldType;
class MVPbkBaseContactField;

//class MVPbkContactFieldData;
/**
 * details view field property.
 * Field property for a im details field type.
 * Field property contains data for contact field presentation
 * in the UI.
 * @since s60 v5.0
 */
class CVIMPSTFieldProperty :  public CBase
    {
    friend class CVIMPSTFieldPropertyArray;
    
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aFieldId                   field id
	   	 * @param aIconId                   icon id
		 * @param aMaxLength                 max lable length
		 * @param aDefaultLabel             default label
         * @return  A new instance of this class.
         * @since s60 v5.0
         */
        static CVIMPSTFieldProperty* NewLC(TInt aFieldId, TInt aIconId, TInt aMaxLength, const TDesC& aDefaultLabel );

        /**
         * Destructor.
         * @since s60 v5.0
         */
        ~CVIMPSTFieldProperty();

    public: // Interface

         
        /**
         * set the MVPbkStoreContactField 
         * @param aField, to set 
         * @since s60 v5.0
         */
        void SetStoredFieldL(MVPbkStoreContactField& aField ) ;

    public: // From MPbk2FieldProperty
        
          
       	/**
         * @return icon id 
         * @since s60 v5.0
         */     
        TInt IconId() ;
        
        /**
         * @return default label if exist other wise field label
         * @since s60 v5.0
         */    
        const TDesC& DefaultLabelL() ;
        
        /**
         * @return field id 
         * @since s60 v5.0
         */
        TInt FieldId() ;
     
     	/**
         * @return field data 
         * @since s60 v5.0
         */
    	const TDesC& FieldDataL();
    	
       	/**
    	 * Set the field data 
         * @param  aText, to set
         * @since s60 v5.0
         */
    	void SetFieldTextL(const TDesC& aText ); 
	 
    private: // Implementation
    	/**
         * default constructor
         * @param aFieldId                   field id
	   	 * @param aIconId                   icon id
		 * @param aMaxLength                 max lable length
		 * @param aDefaultLabel             default label
         * @since s60 v5.0
         */
        CVIMPSTFieldProperty(TInt aFieldId, TInt aIconId, TInt aMaxLength );
        
        /**
         * provide two phase contructions
         * @param aReader                   Resource reader pointed to a
         *                                  VIMPST_FIELD_PROPERTY
         *                                  structure.
         * @param aSupportedFieldTypeList   List of supported field types.
         * @since s60 v5.0
         */
        void ConstructL(const TDesC&  aDefaultLabel);

    private: // Data
         // owns : field id
       	TInt iFieldId;
       	 /// Own: Index of an icon
        TInt iIconId;                    		// STRUCT iconId
        // owns : max lable length
       	TInt iMaxLength;
       	 // Own: Default label for the field
        HBufC* iDefaultLabel;                   // LTEXT defaultLabel
       	// owns : virtual pbk field
        MVPbkStoreContactField* iField; 
        /// Own:  field data
        HBufC* iFieldData;                   
       
	};

#endif // CVIMPSTFIELDPROPERTY_H

// End of File
