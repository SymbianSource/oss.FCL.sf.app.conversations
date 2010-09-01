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

 
#include "cvimpstdetailsviewlistboxmodel.h"

#include "cvimpstfieldproperty.h"
#include "cvimpstfieldpropertyarray.h"
#include "vimpstdetailsview.hrh"
#include "tvimpstenums.h"
#include <vimpstdetailsviewpluginrsc.rsg>
// virtual phonebook 
#include <MVPbkFieldType.h>
#include <MVPbkStoreContact.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>
// system includes
#include <aknlists.h>
#include <avkon.rsg>
#include <VPbkEng.rsg>
#include <AknIconArray.h>
#include <barsc.h>
#include <barsread.h>
#include <StringLoader.h>

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewListBoxModel* CVIMPSTDetailsViewListBoxModel::NewL(
    CEikListBox& aListBox,
    MVIMPSTDetailsPresenceHandler& aPresenceHandler,
    HBufC8* aFieldBuffer)
	{
	CVIMPSTDetailsViewListBoxModel* self =
	    new(ELeave) CVIMPSTDetailsViewListBoxModel(aListBox, aPresenceHandler,
	    aFieldBuffer );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::~CVIMPSTDetailsViewListBoxModel
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewListBoxModel::~CVIMPSTDetailsViewListBoxModel()
	{
	delete iFieldArray;
	delete iTextCache;
	delete iFieldBuffer;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::CVIMPSTDetailsViewListBoxModel
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewListBoxModel::CVIMPSTDetailsViewListBoxModel(
	    CEikListBox& aListBox,
	    MVIMPSTDetailsPresenceHandler& aPresenceHandler,
	    HBufC8* aFieldBuffer )
	: iListBox(aListBox),
    iPresenceHandler( aPresenceHandler )
	{
	// owns
    iFieldBuffer = aFieldBuffer ;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewListBoxModel::ConstructL()
	{
	iTextCache = new(ELeave) CDesCArrayFlat(KCacheGranularity);
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewListBoxModel::FocusedFieldIndex()
	{
	return iListBox.CurrentItemIndex();
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewListBoxModel::SetFocusedListIndex(TInt aIndex)
	{
	if( !iFieldArray )
		{
		return;
		}
	TInt count = iFieldArray->Count();
	if (  aIndex < 0 || aIndex >= count  )
		{
		aIndex = 0;
		}
	if( count )
		{
		iListBox.SetCurrentItemIndexAndDraw(aIndex); 
		}
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::
// ---------------------------------------------------------------------------
//
 CVIMPSTFieldProperty& CVIMPSTDetailsViewListBoxModel::FocusField()
	{
	return ( iFieldArray->At( FocusedFieldIndex() ) );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::FieldsArray
// ---------------------------------------------------------------------------
//
 CVIMPSTFieldPropertyArray& CVIMPSTDetailsViewListBoxModel::FieldsArray()
	{
	return *iFieldArray;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::FieldsCount
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewListBoxModel::FieldsCount()
	{
	if( !iFieldArray )
		{
		return 0;	
		}
	return iFieldArray->Count();
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::CreateListPresentationL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewListBoxModel::CreateListPresentationL( MVPbkStoreContact& aContact,
													   TInt aFocusFieldIndex,TDesC& aServiceName,  TStoreType aStoreType )
	{
	const MVPbkContactStoreProperties& storeProperties =  aContact.ParentStore().StoreProperties();
	const MVPbkFieldTypeList& supportedFieldTypes = storeProperties.SupportedFields();
	
	if( iFieldBuffer )
	    {
	    TResourceReader reader;
        HBufC8* buf = iFieldBuffer->AllocL();
        CleanupStack::PushL( buf );
        reader.SetBuffer( buf );        
        iFieldArray = CVIMPSTFieldPropertyArray::NewL( supportedFieldTypes, 
                                                    aContact,
                                                    reader , 
                                                    iPresenceHandler.IsSupported(),
                                                    aServiceName,
                                                    aStoreType);
        CleanupStack::PopAndDestroy( buf );
        } 	

	AddFieldsDataL();
	if( iTextCache->Count() ) 
		{
	    CVIMPSTFieldProperty* field = iFieldArray->GetFieldById( EVIMPSTFieldIdImpp  );
		if( field )
			{
	 		iPresenceHandler.SubscribePresenceL( field->FieldDataL() );	
			}
		SetFocusedListIndex( aFocusFieldIndex );	
		}
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::SetEmptyTextsToListboxL()
// See header for details. 
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewListBoxModel::SetEmptyTextsToListboxL()
    { 
    // update the empty string now to avoid flickering
    HBufC* string = StringLoader::LoadLC( R_SERVDETAIL_BACKGROUNDTEXT_HEADER );
    if( string )
		{
		iListBox.View()->SetListEmptyTextL( *string );
		CleanupStack::PopAndDestroy( string );
		iListBox.DrawNow(); 	
		}
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::MdcaCount
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewListBoxModel::MdcaCount() const
	{
	return iTextCache->Count();
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::MdcaPoint
// ---------------------------------------------------------------------------
//
TPtrC CVIMPSTDetailsViewListBoxModel::MdcaPoint(TInt aIndex) const
	{
	return iTextCache->MdcaPoint(aIndex);
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::AddFieldsDataL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewListBoxModel::AddFieldsDataL( )
	{
	iTextCache->Reset();
	
	TInt fieldCount = iFieldArray->Count();

	for (TInt i = 0; i < fieldCount; i++)
		{
		TBuf<KBufferSize> buffer;
		buffer.Zero();
		
		CVIMPSTFieldProperty& field = iFieldArray->At(i);
		TInt iconId = field.IconId();
		if( field.FieldId() == EVIMPSTFieldIdStatusMsg )
			{
			TVIMPSTEnums::TOnlineStatus status = iPresenceHandler.GetPresenceStatus();
			switch( status )
				{
				case TVIMPSTEnums::EOnline:
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconOn);
					break;	
					}
				case TVIMPSTEnums::EInvisible:
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconInvi);
					break;	
					}
				case TVIMPSTEnums::EAway:
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconAway);
					break;	
					}
				case TVIMPSTEnums::EBusy:
				case TVIMPSTEnums::EDoNotDisturb:    
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconBusy);
					break;	
					}
				case TVIMPSTEnums::EOnPhone:
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconOnMobile);
					break;	
					}
				case TVIMPSTEnums::EOffline:
					{
					iconId = static_cast<TInt>(EVIMPSTServiceIconOff);
					break;	
					}
				case TVIMPSTEnums::ECallForward:
				    {
				    iconId = static_cast<TInt>(EVIMPSTServiceIconCallForward);
				    break;  
				    }				                    
				case TVIMPSTEnums::EPending:
                case TVIMPSTEnums::EBlocked:
                case TVIMPSTEnums::EUnknown:
                case TVIMPSTEnums::EServiceOut:
                default:
                    {
                    iconId = static_cast<TInt>(EVIMPSTServiceIconEmpty);
                    break;  
                    }   
                        }
                    field.SetFieldTextL( iPresenceHandler.GetStatusMessageL() )	;
                    }
					
	    if( iconId == static_cast<TInt>(EVIMPSTServiceIconEmpty ) )
			{
			buffer.Append(KTabLit);	//0
			}
		else
			{
			buffer.AppendNum(iconId); //0
			buffer.Append(KTabLit);	
			}
			
		TPtrC labelPtr = field.DefaultLabelL();
		buffer.Append( labelPtr );
		buffer.Append(KTabLit);
		TPtrC fieldText = field.FieldDataL();
		buffer.Append( fieldText );
		iTextCache->AppendL(buffer);
		buffer.Zero();
		}
	iListBox.HandleItemAdditionL();
	}

// -----------------------------------------------------------------------------
// CVIMPSTDetailsViewListBoxModel::DisplayId
// -----------------------------------------------------------------------------
//
TPtrC CVIMPSTDetailsViewListBoxModel::DisplayId( const TDesC& aId, TBool aListHiding )
    {
    TPtrC ret( aId );
    // locate ":" for userid, groupid.
    // locate "/" for list id.
    TInt pos = aId.FindC( aListHiding ? KSlash : KColon );

    if ( ( pos != KErrNotFound) && ( pos != aId.Length()-1) )
        {
        // contains the special character, and it is not the last char
        // remove everything before the special char (including the char)
        ret.Set( aId.Mid( pos + 1 ) );
        }
   // remove also the domain part
    TInt domainPos = ret.FindC( KAt );
    if ( ( domainPos != KErrNotFound ) && ( domainPos != 0 ) )
        {
        ret.Set( ret.Mid( 0, domainPos ) );
        }
    return ret;
    }    
   
// End of File
