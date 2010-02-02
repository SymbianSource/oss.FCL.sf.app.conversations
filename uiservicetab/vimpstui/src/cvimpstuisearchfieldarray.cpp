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
* Description:  search fields array implementation.
*
*/


#include "cvimpstuisearchfieldaray.h"

#include "mvimpstsearchuibuilder.h"
#include "cvimpstuisearchfield.h"

// System includes
#include <barsread.h>
#include <coemain.h>
#include <eikcapc.h>
#include <eikedwin.h>
#include <AknUtils.h>

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::CVIMPSTUiSearchFieldArray
// --------------------------------------------------------------------------
//
inline CVIMPSTUiSearchFieldArray::CVIMPSTUiSearchFieldArray():
	 iSelectedSearchType( EIMBasicSearch )
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::~CVIMPSTUiSearchFieldArray
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchFieldArray::~CVIMPSTUiSearchFieldArray()
    {
    iFieldArray.ResetAndDestroy();
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiSearchFieldArray* CVIMPSTUiSearchFieldArray::NewL()
    {
    CVIMPSTUiSearchFieldArray* self = 
        new ( ELeave ) CVIMPSTUiSearchFieldArray( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::ConstructL
// --------------------------------------------------------------------------
//
inline void CVIMPSTUiSearchFieldArray::ConstructL()
    {
    
    }
    
// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::CreateFieldsFromContactL
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiSearchFieldArray::CreateFieldsL( const TDesC& aLebal, 
											TVIMPSTSearchDataFieldType aFieldDataType ,
											TIMPSTSearchFieldTypes aFieldType /*= EIMAdvancedSearch*/, 
											TVIMPSTEnums::TVIMPSTSearchKey aSearchKey/* = EVIMPSTSearchKeyUknown*/  )
    {
    MVIMPSTUiSearchField* newField = CVIMPSTUiSearchField::NewL( aLebal, aFieldDataType, aFieldType , aSearchKey);
    
    iFieldArray.Append( newField ); // ownership to array
    
	return newField->ControlId();
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::InsertFieldsL
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiSearchFieldArray::InsertFieldsL(TInt aIndex, const TDesC& aLebal, 
											TVIMPSTSearchDataFieldType aFieldDataType ,
											TIMPSTSearchFieldTypes aFieldType /*= EIMAdvancedSearch*/, 
											TVIMPSTEnums::TVIMPSTSearchKey aSearchKey/* = EVIMPSTSearchKeyUknown*/ )
    {
    MVIMPSTUiSearchField* newField = CVIMPSTUiSearchField::NewL( aLebal, aFieldDataType, aFieldType, aSearchKey );
    
    iFieldArray.InsertL( newField ,aIndex); // ownership to array
    
	return newField->ControlId();
    }
// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::AreAllUiFieldsEmpty
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchFieldArray::AreAllUiFieldsEmpty() const
    {
    const TInt count = iFieldArray.Count();
    
    for(TInt i = 0; i < count; ++i)
        {        
        TPtrC text = iFieldArray[i]->FieldData() ;
        if ( text.Length() > 0)
            {
            return EFalse;
            }
        
        }
    return ETrue;
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::AreAllUiControlsEmptyL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiSearchFieldArray::AreAllUiControlsEmptyL() const
    {
    const TInt count = iFieldArray.Count();
    
    for(TInt i = 0; i < count; ++i)
        {        
        HBufC* text = iFieldArray[i]->ControlTextL() ;
        if ( text && text->Length() )
            {
            return EFalse;
            }
        
        }
    return ETrue;
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::ResetFieldData
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::ResetFieldData() 
    {
    const TInt count = iFieldArray.Count();
   
    for(TInt i = 0; i < count; ++i)
        {        
        TInt len = iFieldArray[i]->FieldData().Length() ;
        if ( len > 0)
            {
            iFieldArray[i]->ResetFieldData();
            }
        
        }
    
    }

// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::CreateUiFieldsFromArrayL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::CreateUiFieldsFromArrayL(MVIMPSTSearchUiBuilder& aUiBuilder )
	{
	iUiBuilder = NULL; 
	iUiBuilder = &aUiBuilder;
	
	TInt count = Count();
	
	for( TInt i = 0 ; i< count; i++ )
		{
		MVIMPSTUiSearchField* field = iFieldArray[i];
	
		CreateUiFieldsL( *field );

		}
	}
// --------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::CreateUiFieldsFromArrayL
// --------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::CreateUiFieldsL(MVIMPSTUiSearchField& aField )
	{
	switch( aField.FieldDataType() )
		{

		case EVIMPSTEdwinNumber :
			{
			// if number field
			break;
			}
		case EVIMPSTEdwinEmail :
			{
			// if email field
			break;
			}
			
		case EVIMPSTEdwinMobile:
			{
			// if mobile field
			break;
			}
		case EVIMPSTEdwinText :
		default:
			{
			// default field type is text 
			// Create and insert a line in the dialog
			iControl = NULL;
			iCaptionedCtrl = NULL;
			
			iControl = static_cast<CEikEdwin*>(iUiBuilder->CreateLineL( aField.FieldLabel(), aField.ControlId(), EEikCtEdwin) );

			// Control is now owned by the dialog
			TInt maxFieldLength = 100 ;
			AknEditUtils::ConstructEditingL(iControl, maxFieldLength,
			maxFieldLength, EAknEditorTextCase | EAknEditorCharactersUpperCase
			| EAknEditorCharactersLowerCase, EAknEditorAlignLeft,
			ETrue, ETrue, EFalse);
			
			iControl->SetAknEditorCase(EAknEditorTextCase);
		
			// Place cursor to the end of the line
			iControl->AddFlagToUserFlags(CEikEdwin::EJustAutoCurEnd);
			
			HBufC* previousdata = aField.FieldData().AllocLC();
			iControl->SetTextL( previousdata );
			CleanupStack::PopAndDestroy();// previousdata
			aField.ResetFieldData();
		
			
			// CreateTextViewL() is flagged as deprecated but if it is not 
			// called here the ActivateL() below crashes sometimes.
			iControl->CreateTextViewL();
			iCaptionedCtrl = iUiBuilder->LineControl( aField.ControlId() );
			iCaptionedCtrl->SetTakesEnterKey(ETrue);
			aField.SetControl( iControl, iCaptionedCtrl ); // onerhip transferd here to field
		
			aField.ActivateL();
			break;	
			}
		}
	}

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::SetFocusL
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::SetFocusL(TInt aFieldIndex )
    {
    if (aFieldIndex >= 0 && aFieldIndex < iFieldArray.Count() && iUiBuilder )
        {
        MVIMPSTUiSearchField& field = At( aFieldIndex);
        
        iUiBuilder->TryChangeFocusL( field.ControlId() );
        }
        
    }   

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::GetFirstEnteredFieldDataL
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTUiSearchFieldArray::GetFirstEnteredFieldDataL()
    {
    const TInt count = iFieldArray.Count();
    
    for(TInt i = 0; i < count; ++i)
        {        
        TPtrC text = iFieldArray[i]->FieldData() ;
        if ( text.Length() > 0)
            {
            return iFieldArray[i]->FieldData();
            }
        
        }
    return KNullDesC();
    }   


// -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::GetSearchKeyDataL
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::GetSearchKeyDataL(RArray<TVIMPSTSearchKeyData>& aKeyDataArray )
    {
    const TInt count = iFieldArray.Count();
    TPtrC dataPtr(KNullDesC );
    for(TInt i = 0; i < count; ++i)
        {    
        MVIMPSTUiSearchField*  field = iFieldArray[i] ;
        TPtrC text = field->FieldData() ;
        if ( text.Length() > 0)
            {
			if(field->GetSearchKey() != TVIMPSTEnums::EVIMPSTSearchKeyUknown )
				{
				 TVIMPSTSearchKeyData data = 	{
								field->GetSearchKey(),
								KNullDesC(),
								text	
								};
				 
				aKeyDataArray.Append(data);	
				}
			else
				{
				TPtrC label = field->FieldLabel() ;
				TVIMPSTSearchKeyData data =	{
								field->GetSearchKey(),
								label,
								text	
								};
			 	 aKeyDataArray.Append(data );
			 				
				}
            }
        
        }
    
    }   
 
  // -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::RemoveField
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::RemoveField(TInt aIndex )
    {
   	iFieldArray.Remove(aIndex);
   	iFieldArray.Compress();
    }     
 
 // -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::InsertField
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::InsertField(MVIMPSTUiSearchField*  aField , TInt aIndex )
    {
   	iFieldArray.Insert(aField ,aIndex);
    }     
  
// -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::GetSearchType
// -----------------------------------------------------------------------------
//
TIMPSTSearchFieldTypes CVIMPSTUiSearchFieldArray::GetSearchType()
	{
	return iSelectedSearchType;
	}

// -----------------------------------------------------------------------------
// CVIMPSTUiSearchFieldArray::SetSearchType
// -----------------------------------------------------------------------------
//
void CVIMPSTUiSearchFieldArray::SetSearchType(TIMPSTSearchFieldTypes aType)
	{
	iSelectedSearchType = aType;
	}
  
//  End of File  
