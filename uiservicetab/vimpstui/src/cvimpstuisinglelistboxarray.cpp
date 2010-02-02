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


// INCLUDE FILES
#include	"cvimpstuisinglelistboxarray.h"
#include    "tvimpstconsts.h"
#include    "vimpstutils.h"
#include    "mvimpstprocessarray.h"
#include 	<vimpstuires.rsg>
#include    "tvimpstenums.h"

// platform includes

#include    <AknUtils.h>
#include    <StringLoader.h>
#include    <AknIconUtils.h>
#include    <AknIconArray.h>
#include    <eikclbd.h>
#include    <AknsConstants.h>

#include    <vimpstui.mbg>

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CVIMPSTUiSingleListboxArray* CVIMPSTUiSingleListboxArray::NewL(MVIMPSTProcessArray& aItemModel,
							            CColumnListBoxData* aListboxData,
							            CVIMPSTUiSingleStyleListBox& aListBox
							            )
    {
    CVIMPSTUiSingleListboxArray* self = new (ELeave) CVIMPSTUiSingleListboxArray( aItemModel, 
    								aListboxData, aListBox);
    								
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CVIMPSTUiSingleListboxArray::~CVIMPSTUiSingleListboxArray()
    {
    delete iData;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CVIMPSTUiSingleListboxArray::CVIMPSTUiSingleListboxArray( MVIMPSTProcessArray& aItemModel,
        CColumnListBoxData* aListboxData, CVIMPSTUiSingleStyleListBox& aListBox /*= NULL*/)
    :
     iItemArray(aItemModel), 
     iListboxData( aListboxData ),
     iListBox(aListBox)
    {
    }

// Symbian OS default constructor can leave.
void CVIMPSTUiSingleListboxArray::ConstructL()
	{
	
	 // maximum length of icons appended to identification
    // There can be four icons (A+B and two on D-column)
    iMaxLengthOfIcons =
        KContactOnlineIconA().Length()+  // longest A-Column icon
        KEmptyIconC().Length() + // longest C-column icon
        KContactNewMsgIcon().Length(); // longest D-Column icon
    // Reserve enough memory to handle maximum size item
	iData = HBufC::NewL( KFriendsListMaxIdentificationLength +
	                     KStatusMsgMaxLength +
					     iMaxLengthOfIcons );

    }


// ---------------------------------------------------------
// CVIMPSTUiSingleListboxArray::MdcaCount
// Returns the number of descriptor elements in a descriptor array.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiSingleListboxArray::MdcaCount() const
	{
	return iItemArray.Count();
	}

// ---------------------------------------------------------
// CVIMPSTUiSingleListboxArray::MdcaCount
// Return pointer to descriptor data in given index of an array
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC16 CVIMPSTUiSingleListboxArray::MdcaPoint( TInt aIndex ) const
	{
	// Although this is a const method, we do change the member data.
	// This is done because of performance
	// -> no need to create new buffer every time.
    TPtr dataPtr( iData->Des() );
    dataPtr.Zero();

    // fetch the item and process correct type    
    TVIMPSTEnums::TItem type = iItemArray.GetType(aIndex);

    switch( type )
        {
        case TVIMPSTEnums::EOwnStatusItem:
	        {
	        TRAP_IGNORE( AppendOwnDataL(dataPtr,aIndex ) );
	        break;	
	        }
		
        case TVIMPSTEnums::EContactItem: 
			{
			// contact item
	    	TRAP_IGNORE( AppendContactL(dataPtr,aIndex ) );
			break;
			}
			
        } //end outer switch
    return dataPtr;
	}
	

// ---------------------------------------------------------
// CVIMPSTUiSingleListboxArray::AppendContact
// Append formatted contact identification to buffer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListboxArray::AppendContactL( TPtr& aBuffer,
                                      TInt aIndex
                                      ) const
    {
    // A-column
     aBuffer.Append( KEmptyIconA );
    // B-column
     TPtrC identification(iItemArray.GetItemNameText(aIndex) );
     aBuffer.Append( identification.Left(KFriendsListMaxIdentificationLength) );

     //C-Column
     aBuffer.Append( KEmptyIconC );

     // D-Column
     if( iItemArray.IsMsgPending( aIndex ) )
         {
         aBuffer.Append( KContactNewMsgIcon );   
         }
     else
         {
         aBuffer.Append( KEmptyIconC );  
         } 
    }

// ---------------------------------------------------------
// CVIMPSTUiSingleListboxArray::AppendContact
// Append formatted contact identification to buffer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListboxArray::AppendOwnDataL( TPtr& aBuffer,
                                      TInt aIndex
                                      ) const
	{    
	// no need to append any icon in the a coloum as there is no presence service.
	 // A-column
	 aBuffer.Append( KEmptyIconA );
	 
     // B-column
    TPtrC identification(iItemArray.GetItemNameText(aIndex));
    aBuffer.Append( identification.Left( KFriendsListMaxIdentificationLength ) );


    //C-Column
    aBuffer.Append( KEmptyIconA );
    
    //D-Column
    aBuffer.Append( KEmptyIconA );
   }

// ---------------------------------------------------------
// CVIMPSTUiSingleListboxArray::AppendContactListL
// Append formatted contact list identification to buffer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CVIMPSTUiSingleListboxArray::AppendContactListL( TPtr& /*aBuffer*/, 
        TInt /*aIndex */) const
        {
        // add functionality when contactlists  supported
        }


//  End of File
