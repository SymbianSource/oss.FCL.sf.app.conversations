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
* Description:  search fields array declaration.
*
*/


#ifndef CVIMPSTUISEARCHFIELDARAY_H
#define CVIMPSTUISEARCHFIELDARAY_H

// INCLUDES
#include <e32base.h>

#include "mvimpstuisearchfield.h"

// FORWARD DECLARATIONS
class MVIMPSTSearchUiBuilder;
class CEikCaptionedControl;

// CLASS DECLARATION
/**
 *  An array of search editor fields.
 *  @lib vimpstui.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CVIMPSTUiSearchFieldArray) :  public CBase
    {
    public:  // Constructors and destructor
        
        /**
         * Creates a new instance of this class.
         * @return  A new instance of this class.
         */
        static CVIMPSTUiSearchFieldArray* NewL();
        
        /**
         * Destructor.
         */
        ~CVIMPSTUiSearchFieldArray();

    public: // Interface

        /**
         * Creates editor fields 
         * append the field into array from last
         * @param aLebal, label of field
         * @param aFieldDataType, field data type
         * @param aFieldType, field type basic or advanced
         * @param aSearchKey, search key
         */
        TInt CreateFieldsL(const TDesC& aLebal, TVIMPSTSearchDataFieldType aFieldDataType ,
        				 TIMPSTSearchFieldTypes aFieldType = EIMAdvancedSearch,
        				 TVIMPSTEnums::TVIMPSTSearchKey aSearchKey = TVIMPSTEnums::EVIMPSTSearchKeyUknown  );
        
        /**
         * Creates editor fields 
         * insert into array at given index
         * @param aIndex, index where field to inserted
         * @param aLebal, label of field
         * @param aFieldDataType, field data type
         * @param aFieldType, field type basic or advanced
         * @param aSearchKey, search key
         */
        TInt InsertFieldsL(TInt aIndex, const TDesC& aLebal, 
							TVIMPSTSearchDataFieldType aFieldDataType ,
							TIMPSTSearchFieldTypes aFieldType = EIMAdvancedSearch,
							TVIMPSTEnums::TVIMPSTSearchKey aSearchKey = TVIMPSTEnums::EVIMPSTSearchKeyUknown ) ;
											
        /**
         * Creates editor fields from the array
         * @param aUiBuilder, reference to builder class
         */
        void CreateUiFieldsFromArrayL(MVIMPSTSearchUiBuilder& aUiBuilder  );
        
      
    public:  // new implementations
    
    	
		/**
		 * count no of fields in array
		 * @return no of fields in array
		 */  
		inline TInt Count() const;


		/**
		 * find and return the field store at given index
		 * @param aIndex ,index of field
		 * @return Base class reference of field
		 */  
		inline MVIMPSTUiSearchField& At( TInt aIndex );


		/**
		 * AreAllUiFieldsEmpty check the all field are empty 
		 * @return ETrue if empty else EFalse
		 */    
		TBool AreAllUiFieldsEmpty() const;

		/**
		 * AreAllControlsEmptyL check the all control text are empty 
		 * @return ETrue if empty else EFalse
		 */ 
		TBool AreAllUiControlsEmptyL() const;

		/**
		 * set the focus  to selected field 
		 * @param aFieldIndex, index of field
		 */         
		void SetFocusL(TInt aFieldIndex );

		/**
		 * get the first field data which is entered by user 
		 * @return text of field
		 */ 
		TPtrC GetFirstEnteredFieldDataL();

		/**
		 * GetSearchKeyDataL fill the data into array for lower layer to use
		 */ 
		void GetSearchKeyDataL(RArray<TVIMPSTSearchKeyData>& aKeyDataArray );

		/**
		 * reset field data text
		 */ 
		void ResetFieldData();

		/**
		 * RemoveField remove field from array  
		 * @param aIndex, index to use
		 */ 
		void RemoveField(TInt aIndex );

		/**
		 * insert a field at given location into array
		 * @param aField, field to insert
		 * @param aIndex, index to use
		 */   
		void InsertField(MVIMPSTUiSearchField*  aField , TInt aIndex );
		
		
		 /**
		 * @return selected search type
		 */ 
		TIMPSTSearchFieldTypes GetSearchType();

		/**
		 * insert a field at given location into array
		 * @param aType, selected search type
			 */   
		void SetSearchType(TIMPSTSearchFieldTypes aType);

                       
    private: // new Implementation
    
    	/**
		 * CVIMPSTUiSearchFieldArray default constructor
		 */   
        CVIMPSTUiSearchFieldArray();
        
        /**
		 * ConstructL
		 */   
        void ConstructL();
        
        /**
		 * create the field control of a field
		 * set the control id to given field
		 * @param aField, field to insert
		 */   
        void CreateUiFieldsL( MVIMPSTUiSearchField& aField );
                    
    private: // Data
        
		// Own: An array of dialog fields
		RPointerArray<MVIMPSTUiSearchField> iFieldArray;
		
		// not owns, reference to ui builder class ,dialog
        MVIMPSTSearchUiBuilder* iUiBuilder;
		
		// Ref: Editor control
        CEikEdwin* iControl;
        
        // Ref: Fields captioned control
        CEikCaptionedControl* iCaptionedCtrl; 
        
        // owns : selelected search type
        TIMPSTSearchFieldTypes iSelectedSearchType;
        
        
    };

// INLINE FUNCTIONS

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::Count
// --------------------------------------------------------------------------
//
inline TInt CVIMPSTUiSearchFieldArray::Count() const
    {
    return iFieldArray.Count();
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::At
// --------------------------------------------------------------------------
//    
inline MVIMPSTUiSearchField& CVIMPSTUiSearchFieldArray::At
        ( TInt aIndex )
    {
    return *iFieldArray[aIndex];
    }

#endif // CVIMPSTUISEARCHFIELDARAY_H
            
// End of File

