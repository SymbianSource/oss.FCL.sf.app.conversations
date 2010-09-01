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
* Description:  search fields interface.
*
*/


#ifndef MVIMPSTUISEARCHFIELD_H
#define MVIMPSTUISEARCHFIELD_H

// INCLUDES
#include <w32std.h>
#include "tvimpstenums.h"

// FORWARD DECLARATIONS
class CEikEdwin;
class CEikCaptionedControl;

enum TVIMPSTSearchDataFieldType
	{
	EVIMPSTEdwinText,
	EVIMPSTEdwinNumber,
	EVIMPSTEdwinEmail,
	EVIMPSTEdwinMobile	
	};

enum TIMPSTSearchFieldTypes
	{
	EIMBasicSearch = 0 ,
	EIMAdvancedSearch	
	};
 

// CLASS DECLARATION
/**
 * search field interface.
 */
 
class MVIMPSTUiSearchField
    {
   	    	
    public: // Interface

        /**
         * virtual Destructor.
         */
        virtual ~MVIMPSTUiSearchField()
            {
            }
    
        /**
         * control  id
         * @return  Dialog control id.
         */
        virtual TInt ControlId() const = 0;
 
 	
         /**
         * Returns the CEikEdwin.
         * @return Field label.
         */
 		
		 virtual CEikEdwin* Control() const = 0 ;
		 
	    /**
         * SetControl and captured control 
         * @param aControl, a control to set
         * @param aCaptionedCtrl, a captured control to set 
         */
		 virtual void SetControl(CEikEdwin* aControl, 
		 					CEikCaptionedControl* aCaptionedCtrl)  = 0 ;
		 
		 
         /**
         * activate the field 
         * before calling must call SetControl
         */
		 virtual void ActivateL()  = 0 ;
		 
		 
         /**
         * Returns the Control Text.
         * @return Field data.
         */
		 virtual HBufC* ControlTextL() const = 0 ;
		
         /**
         * Returns the field label.
         * @return field label.
         */
        virtual TPtrC FieldLabel()  = 0;

        
		/**
         * Returns the field data.
         * @return field data.
         */
        virtual TPtrC FieldData()  = 0;
        
       
        /**
         * Sets given data as the field 
         * @param aData, data to set 
         */
        virtual void SetFieldDataL(
                const TDesC& aData ) = 0;

	
         /**
         * @return the field data type
         */		
		virtual TVIMPSTSearchDataFieldType FieldDataType() const = 0 ;
		
		/**
         * @return ETrue if basic field
         */
	    virtual TBool IsBasicField() const = 0 ;
	    
	    /**
         * @return field search key
         */
	    virtual	TVIMPSTEnums::TVIMPSTSearchKey GetSearchKey() const  = 0 ;
	   
	  	/**
         * reset the field data.
         */
        virtual void ResetFieldData() = 0;	
   
    };

#endif // MVIMPSTUISEARCHFIELD_H

// End of File

