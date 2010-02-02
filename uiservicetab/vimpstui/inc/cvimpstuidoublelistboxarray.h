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
* Description:  CVIMPSTUiDoubleListboxArray-array decorator
*
*/



#ifndef _CVIMPSTUIDIUBLELISTBOXARRAY_H
#define _CVIMPSTUIDIUBLELISTBOXARRAY_H

//  INCLUDES
#include    <e32base.h>
#include    <bamdesca.h> // MDesCArray
#include    <AknIconArray.h>
#include    <gulicon.h>

// FORWARD DECLARATIONS
class CFormattedCellListBoxData;
class CColumnListBoxData;
class CVIMPSTUiListBoxModel;
class CVIMPSTUiDoubleStyleListBox;
class MVIMPSTProcessArray;
class CFont;
class CVIMPSTUiDoubleListBoxTabViewControl;

// CLASS DECLARATION

/**
 *  Friends-array decorator.
 *  Decorates the array for listbox
 *
 *  @lib vimpstui.dll
 *  @since 5.0
 */
class CVIMPSTUiDoubleListboxArray : public CBase, public MDesCArray
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.		 
		 * @param aItemModel that contains the data.
		 * @param aListBoxData Listbox data for setting row properties,
		 *                   can be NULL.
		 * @param aListBox Listbox 
         */
        static CVIMPSTUiDoubleListboxArray* NewL(
            MVIMPSTProcessArray& aItemModel,
            CFormattedCellListBoxData* aListboxData,
            CVIMPSTUiDoubleStyleListBox& aListBox,
            CVIMPSTUiDoubleListBoxTabViewControl& aTabbedviewControl);

        /**
         * Destructor.
         */
        virtual ~CVIMPSTUiDoubleListboxArray();
     

	public: // Functions from MDesCArray

        /**
         * From MDesCArray, Returns the number of descriptor elements in 
		 * a descriptor array.
		 * @see MDesCArray
         */
		TInt MdcaCount() const;

        /**
         * From MDesCArray, Indexes into a descriptor array.
		 * @see MDesCArray
         */
		TPtrC16 MdcaPoint( TInt aIndex ) const;
		
	
    private: // new functions
                    
        void AppendOwnDataL( TPtr& aBuffer,
                            TInt aIndex ) const;
        
                                    

	private:
	
        /**
         * C++ default constructor.
		 * @param aItemModel that contains the data.
         * @param aListBoxData Listbox data for setting row properties, can be NULL.
         * @param aListBox Listbox 
         */
        CVIMPSTUiDoubleListboxArray( MVIMPSTProcessArray& aItemModel,
                CFormattedCellListBoxData* aListboxData ,CVIMPSTUiDoubleStyleListBox& aListBox,
                CVIMPSTUiDoubleListBoxTabViewControl& aTabbedviewControl);

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL();

    private:    // Data        
        
        // wrapper array on engine side arrays
        MVIMPSTProcessArray& iItemArray;
        
		// Doesn't own. Pointer to listbox data for setting row properties
        CFormattedCellListBoxData* iListboxData;
		
		//Ref. to Contact List Box
        CVIMPSTUiDoubleStyleListBox& iListBox;
		
		// Owns. Buffer that hold's information about fetched item
		HBufC* iData;
	
		// maxmimun length of icon strings added to formatted text
		TInt iMaxLengthOfIcons;
				
		// pointers to listbox fonts, contained objects are not owned
		RPointerArray<CFont> iFonts;
        // Not own. Model to check the real location of items.
        CVIMPSTUiListBoxModel* iContactListBoxModel;
        CVIMPSTUiDoubleListBoxTabViewControl& iTabbedviewControl;
        
        // to hold the value whether presence is supported or not for the service
        TBool iPresenceSupported;
         // to hold the value whether im is supported  or not for the service
        TBool iIMSupported;
        
	};

#endif      // _CVIMPSTUIDIUBLELISTBOXARRAY_H

// End of File
