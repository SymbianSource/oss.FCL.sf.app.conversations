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
* Description:  search field implemetation
*
*/


#include "cvimpstuisearchfield.h"

// System includes
#include <eikedwin.h>
#include <eikcapc.h>
#include <eikedwin.h>
#include <AknUtils.h>

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::CVIMPSTUiSearchField
// -----------------------------------------------------------------------------
//
CVIMPSTUiSearchField::CVIMPSTUiSearchField( TVIMPSTSearchDataFieldType aFieldDataType, 
											TIMPSTSearchFieldTypes aFieldType, 
											TVIMPSTEnums::TVIMPSTSearchKey aSearchKey 
											/* =EVIMPSTSearchKeyUknown*/)
	:iFieldDataType( aFieldDataType ),
	iFieldType(aFieldType ),
	iSearchKey( aSearchKey )
    {
    iCaptionedCtrl  = NULL;
    iControl = NULL;
    }
   
// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchField* CVIMPSTUiSearchField::NewL( const TDesC& aLebal, 
							TVIMPSTSearchDataFieldType aFieldDataType,
	 						TIMPSTSearchFieldTypes aFieldType ,
	 						TVIMPSTEnums::TVIMPSTSearchKey aSearchKey )
    {
    CVIMPSTUiSearchField* self = 
        new ( ELeave ) CVIMPSTUiSearchField( aFieldDataType , aFieldType, aSearchKey );
    CleanupStack::PushL( self );
    self->ConstructL(aLebal);
    CleanupStack::Pop( self );
    return self;
    } 
// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchField::ConstructL( const TDesC& aLabel )   
	{
	iFieldLabel = aLabel.AllocL();
	}
// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::~CVIMPSTUiSearchField
// -----------------------------------------------------------------------------
//
CVIMPSTUiSearchField::~CVIMPSTUiSearchField()
    {
    delete iFieldLabel;
    delete iFieldData;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::ControlId
// -----------------------------------------------------------------------------
//
TInt CVIMPSTUiSearchField::ControlId() const
    {
    return ( TInt ) this;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::FieldDataType
// -----------------------------------------------------------------------------
//
TVIMPSTSearchDataFieldType CVIMPSTUiSearchField::FieldDataType() const
    {
    return iFieldDataType;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::FieldLabel
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTUiSearchField::FieldLabel() 
    {
    if( iFieldLabel )
	    {
	     return iFieldLabel->Des();	
	    }
    return KNullDesC();
    }
  
// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::FieldData
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTUiSearchField::FieldData() 
    {
    if( iFieldData )
		{
		return iFieldData->Des();	
		}
    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::ResetFieldData
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchField::ResetFieldData() 
    {
    if( iFieldData )
		{
		delete iFieldData;
		iFieldData = NULL;	
		}
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::GetSearchKey
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TVIMPSTSearchKey CVIMPSTUiSearchField::GetSearchKey() const
    {
    return iSearchKey;
    }
 // --------------------------------------------------------------------------
// CVIMPSTUiSearchField::IsBasicField
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchField::IsBasicField() const
    {
    return ( iFieldType == EIMBasicSearch );
    } 
// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::SetFieldDataL
// -----------------------------------------------------------------------------
//  
void CVIMPSTUiSearchField::SetFieldDataL( const TDesC& aFieldData )
    {
    if( iFieldData )
	    {
	    delete iFieldData;
	    iFieldData = NULL;	
	    }
	iFieldData = aFieldData.AllocL();
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchField::SetControl
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchField::SetControl(CEikEdwin* aControl, CEikCaptionedControl* aCaptionedCtrl ) 
    {
    iControl = NULL;
    iControl = aControl ;
    
    iCaptionedCtrl = NULL;
    iCaptionedCtrl = aCaptionedCtrl;
    }
 // --------------------------------------------------------------------------
// CVIMPSTUiSearchField::Control
// --------------------------------------------------------------------------
//
CEikEdwin* CVIMPSTUiSearchField::Control() const
    {
    return iControl;
    }
// --------------------------------------------------------------------------
// CVIMPSTUiSearchField::ActivateL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchField::ActivateL() 
    {
    if( !iCaptionedCtrl )
	    {
	    User::Leave( KErrNotReady );	
	    }
    iCaptionedCtrl->ActivateL();
    }
// -----------------------------------------------------------------------------
// CVIMPSTUiSearchField::ControlTextL
// -----------------------------------------------------------------------------
//  
HBufC* CVIMPSTUiSearchField::ControlTextL() const
    {
    HBufC* textBuf = NULL;

   	switch( iFieldDataType )
			{

			case EVIMPSTEdwinNumber :
				{
				// number field
				break;
				}
			case EVIMPSTEdwinEmail :
				{
				// email field
				break;
				}
				
			case EVIMPSTEdwinMobile:
				{
				// mobile field
				break;
				}
			case EVIMPSTEdwinText :
				{
    			if ( iControl )
	        		{
	        		textBuf = iControl->GetTextInHBufL();
	        		}
				break;
				}
			}
    return textBuf;
    } 


// End of File  
