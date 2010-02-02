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
* Description:  Implementation of detailsview plugin control data model
 *
*/


#ifndef CVIMPSTDETAILSVIEWLISTBOXMODEL_H
#define CVIMPSTDETAILSVIEWLISTBOXMODEL_H

#include <e32base.h>
#include <badesca.h>

#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactObserver.h>
#include "tvimpstenums.h"
#include "mvimpstdetailspresencehandler.h"
#include "cvimpstdetailscontacthandler.h"
// forward declaration
class CEikListBox;
class MVPbkStoreContact;
class CVIMPSTFieldPropertyArray;
class CVIMPSTFieldProperty;
class MVIMPSTDetailsPresenceHandler;

// constants declaraion
const TInt KCacheGranularity = 4;
const TInt KBufferSize = 256;
_LIT(KTabLit, "\t");
_LIT(KSlash, "/");
_LIT(KAt, "@");
_LIT(KColon, ":");
_LIT(KSpace," ");

/**
 *  list box model class implemenation for list box items
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
class CVIMPSTDetailsViewListBoxModel :  public CBase,
										public MDesCArray
	{

	public:

	    /**
	     * Two-phased constructor.
	     * @param aListBox, reference to the list box
	     * @param aPresenceHandler, reference to presence handler
	     * @param aFieldBuffer, pointer to a stream buffer
	     * @return Pointer of CVIMPSTDetailsViewListBoxModel class
	     * @since s60 v5.0
	     */
	    static CVIMPSTDetailsViewListBoxModel* NewL( CEikListBox& aListBox,
											        MVIMPSTDetailsPresenceHandler& aPresenceHandler,
											        HBufC8* aFieldBuffer );
	    /**
	     * Destructor.
	     * @since s60 v5.0
	     */
	    ~CVIMPSTDetailsViewListBoxModel();

	   
	     /**
	     * Get field index of focused list item
	     * @return focus field index.
	     * @since s60 v5.0
	     */
	    TInt FocusedFieldIndex();
   
	     /**
	     * set field index focused list item
	     * @param aIndex, field index.
	     * @since s60 v5.0
	     */
	    void SetFocusedListIndex(TInt aIndex);
	    
	    /**
	     * @return the focus field 
	     * @since s60 v5.0
	     */
	    CVIMPSTFieldProperty& FocusField();
	    
	    /**
	     * return the  field array
	     * @since s60 v5.0
	     */
	    CVIMPSTFieldPropertyArray& FieldsArray();
	    
	    
         /**
	     * From MDesCArray
	     * (see details from header)
	     *
	     * @since S60 v5.0
	     */
	    TInt MdcaCount() const;
	
	  
	    /**
	     * function for creating the presentation contact 
	     * @param aContactStore the contact store
	     * @param aFocusFieldIndex focus field index
	     * @since S60 v5.0
	     */
	     void CreateListPresentationL( MVPbkStoreContact& aContact,
							    TInt aFocusFieldIndex,TDesC& aServiceName, TStoreType aStoreType);
		
		/**
	     * Helper function for adding contact field items to
	     * listbox data model.
	     *
	     * @since S60 v5.0
	     */
	    void AddFieldsDataL();
	    
  		
  		 /**
	     * set the empty text to list box
	     * @since S60 v5.0
	     */
	    void SetEmptyTextsToListboxL();
	    
	    	   
	    /**
	     * helper removing domain part
	     * @param aId, id to use
	     * @param aListHiding,ETrue if hide domain part
	     * @since S60 v5.0
	     */
	    TPtrC DisplayId( const TDesC& aId, TBool aListHiding = EFalse  );
	    
	    
	    /**
	     * return the field count
	     * @since S60 v5.0
	     */
	    TInt FieldsCount();

  
	private:
	   
	    /**
	     * From MDesCArray
	     * (see details from header)
	     * @param aIndex, index of item 
	     * @since S60 v5.0
	     */
	    TPtrC MdcaPoint(TInt aIndex) const;

	private:

		/**
	     *  default constructor
	     * @param aListBox, reference to the list box
	     * @param aPresenceHandler, reference to presence handler
	     * @param aFieldBuffer, pointer to a stream buffer
	     * @since S60 v5.0
	     */
	    CVIMPSTDetailsViewListBoxModel(
					        CEikListBox& aListBox,
					        MVIMPSTDetailsPresenceHandler& aPresenceHandler,
					        HBufC8* aFieldBuffer );
		/**
	     *  provide two phase  construction
	     * @since S60 v5.0
	     */
	    void ConstructL();
	    
	   

	private:
	    // data
		
         /**
	     * not owns : Reference to listbox.
	     */
	    CEikListBox& iListBox;
		
		/**
		* Not owned : reference to presence handler
		*/  
		MVIMPSTDetailsPresenceHandler& iPresenceHandler;  

	    /**
	     * Owns : Pointer to listbox data.
	     */
	    CDesCArray* iTextCache;
 
	     /**
	     * Owns : Pointer to field array
	     */
	    CVIMPSTFieldPropertyArray* iFieldArray;
	    
 	   
	     /**
	     * Owns : Pointer to stream buffer
	     */
	    HBufC8* iFieldBuffer;
	   
	  
		};

#endif // CVIMPSTDETAILSVIEWLISTBOXMODEL_H

// End of File
