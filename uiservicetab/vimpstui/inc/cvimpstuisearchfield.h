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
* Description:  search field class declaration.
*
*/


#ifndef CVIMPSTUISEARCHFIELD_H
#define CVIMPSTUISEARCHFIELD_H

// INCLUDES
#include <e32base.h>

#include "mvimpstuisearchfield.h"
#include "tvimpstenums.h"

// FORWARD DECLARATIONS
class CEikCaptionedControl;

// CLASS DECLARATION
/**
 * search editor field class declaration .
 *  @lib vimpstui.lib
 *  @since S60 v5.0
 */ 
 
class  CVIMPSTUiSearchField  : public CBase,
							   public MVIMPSTUiSearchField
    {
    
    public:  // Constructors and destructor
        
        /**
         * Creates a new instance of this class.
         * Creates editor fields 
         * append the field into array from last
         * @param aLebal, label of field
         * @param aFieldDataType, field data type
         * @param aFieldType, field type basic or advanced
         * @param aSearchKey, search key
         * @return  A new instance of this class.
         */    
        static CVIMPSTUiSearchField* NewL(const TDesC& aLebal, 
        			TVIMPSTSearchDataFieldType aFieldDataType,
        			TIMPSTSearchFieldTypes aFieldType, 
        			TVIMPSTEnums::TVIMPSTSearchKey aSearchKey  );

		 /**
         * Destructor.
         */
        ~CVIMPSTUiSearchField();
        
    public: // from MVIMPSTUiSearchField
    	
		/**
		* @see MVIMPSTUiSearchField
		*/
    	TInt ControlId() const;
        
        /**
		* @see MVIMPSTUiSearchField
		*/
        HBufC* ControlTextL() const;
        
        /**
		* @see MVIMPSTUiSearchField
		*/
        CEikEdwin* Control() const;
       
       /**
		* @see MVIMPSTUiSearchField
		*/
        void SetControl(CEikEdwin* aControl, CEikCaptionedControl* aCaptionedCtrl) ;
       
       /**
		* @see MVIMPSTUiSearchField
		*/
        void ActivateL() ; 
        
        /**
		 * @see MVIMPSTUiSearchField
		 */
        void SetFieldDataL( const TDesC& aData );
       
       /**
		* @see MVIMPSTUiSearchField
		*/
        TPtrC FieldLabel() ;    	
		
		/**
		 * @see MVIMPSTUiSearchField
		 */
		TVIMPSTSearchDataFieldType FieldDataType() const ;
		
		/**
		 * @see MVIMPSTUiSearchField
		 */
		TPtrC FieldData() ;
		
		/**
		 * @see MVIMPSTUiSearchField
		 */
		void ResetFieldData() ;
		
		/**
		 * @see MVIMPSTUiSearchField
		 */
		TBool IsBasicField() const;
		
		/**
		 * @see MVIMPSTUiSearchField
		 */
		TVIMPSTEnums::TVIMPSTSearchKey GetSearchKey() const ;
		 		
    private :
    
    	/**
         * C++ constructor.
         * @param aFieldDataType , field data type eg  text ,number 
         * @param aFieldType,  field type eg basic or advanced
         * @param aSearchKey, a search key
         */
        CVIMPSTUiSearchField(TVIMPSTSearchDataFieldType aFieldDataType, 
        					TIMPSTSearchFieldTypes aFieldType ,
        					TVIMPSTEnums::TVIMPSTSearchKey aSearchKey );
        
        /**
         * ConstructL
         * @param aLebal, a field label
         */
        void ConstructL(const TDesC& aLebal );
     
		
 	private :
 	
 		// owns : a field data type
 		TVIMPSTSearchDataFieldType iFieldDataType;
 	
 		// owns : field type
 		HBufC* iFieldLabel;
 	
 		// owns : field saved data
 		HBufC* iFieldData;
 		
 		// owns : field type
 		TIMPSTSearchFieldTypes iFieldType;
 		
 		// owns : search key
 		TVIMPSTEnums::TVIMPSTSearchKey iSearchKey;
 		
 	protected: // Data
        // Ref: Editor control
        CEikEdwin* iControl;
        // Ref: Fields captioned control
        CEikCaptionedControl* iCaptionedCtrl; 	
 	      
    };

#endif  // CVIMPSTUISEARCHFIELD_H
            
// End of File

