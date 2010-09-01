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
* Description:  CVIMPSTUiArray-array decorator
*
*/



#ifndef _CVIMPSTUISINGLELISTBOXARRAY_H
#define _CVIMPSTUISINGLELISTBOXARRAY_H

//  INCLUDES
#include    <e32base.h>
#include    <bamdesca.h> // MDesCArray
#include    <AknIconArray.h>
#include    <gulicon.h>

// FORWARD DECLARATIONS
class CColumnListBoxData;
class CVIMPSTUiListBoxModel;
class CVIMPSTUiSingleStyleListBox;
class MVIMPSTProcessArray;

// CLASS DECLARATION

/**
 *  Friends-array decorator.
 *  Decorates the array for listbox
 *
 *  @lib vimpstui.dll
 *  @since 5.0
 */
class CVIMPSTUiSingleListboxArray : public CBase, public MDesCArray
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.		 
		 * @param aItemModel that contains the data.
		 * @param aListBoxData Listbox data for setting row properties,
		 *                   can be NULL.
		 * @param aListBox Listbox 
         */
        static CVIMPSTUiSingleListboxArray* NewL(
            MVIMPSTProcessArray& aItemModel,
            CColumnListBoxData* aListboxData,
            CVIMPSTUiSingleStyleListBox& aListBox  );

        /**
         * Destructor.
         */
        virtual ~CVIMPSTUiSingleListboxArray();
     

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
    
        /**
         * Append formatted contact identification to buffer
         * @param aBuffer Buffer where identification will be appended
         * @param aContact Contact
         * @param aContactList Contact list
         */
        void AppendContactL( TPtr& aBuffer, TInt aIndex ) const;
        
        /**
         * Append formatted contact list identification to buffer
         * @param aBuffer Buffer where identification will be appended
         * @param aContactList Contact list
         */
        void AppendContactListL( TPtr& aBuffer, 
                                 TInt aIndex ) const;
                                 
        /**
         * Append formatted owndata identification to buffer
         * @param aBuffer Buffer where identification will be appended
         * @param aContactList Contact list
         */                        
        void AppendOwnDataL( TPtr& aBuffer,
                            TInt aIndex ) const;

	private:
	
        /**
         * C++ default constructor.
		 * @param aItemModel that contains the data.
         * @param aListBoxData Listbox data for setting row properties, can be NULL.
         * @param aListBox Listbox 
         */
        CVIMPSTUiSingleListboxArray( MVIMPSTProcessArray& aItemModel,
            CColumnListBoxData* aListboxData ,CVIMPSTUiSingleStyleListBox& aListBox );

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL();

    private:    // Data        
        
        // wrapper array on engine side arrays
        MVIMPSTProcessArray& iItemArray;
        
		// Doesn't own. Pointer to listbox data for setting row properties
		CColumnListBoxData* iListboxData;
		
		//Ref. to Contact List Box
		CVIMPSTUiSingleStyleListBox& iListBox;
		
		// Owns. Buffer that hold's information about fetched item
		HBufC* iData;
	
		// maxmimun length of icon strings added to formatted text
		TInt iMaxLengthOfIcons;
	
		
        // Not own. Model to check the real location of items.
        CVIMPSTUiListBoxModel* iContactListBoxModel;
        
	};

#endif      // _CVIMPSTUISINGLELISTBOXARRAY_H

// End of File
