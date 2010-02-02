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
* Description:  a single field with type
 *
*/


#include "cvimpstfieldpropertyarray.h"

#include "cvimpstfieldproperty.h"
#include "vimpstdetailsview.hrh"

// Virtual Phonebook
#include <MVPbkFieldType.h>
#include <MVPbkStoreContactField.h>
#include <MVPbkStoreContact.h>
#include <MVPbkContactFieldUriData.h>
// System includes
#include <barsread.h>
_LIT(KColon,":");

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::CVIMPSTFieldPropertyArray
// ---------------------------------------------------------------------------
//
inline CVIMPSTFieldPropertyArray::CVIMPSTFieldPropertyArray(MVPbkStoreContact& aContact ):
	iContact( aContact )
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::ConstructL
// ---------------------------------------------------------------------------
//
inline void CVIMPSTFieldPropertyArray::ConstructL(
        const MVPbkFieldTypeList& aSupportedFieldTypeList,TResourceReader& aReader,
        TBool aPresenceSupported,
        TDesC& aServiceName, TStoreType aStoreType)
    {
    ReadFieldPropertiesL( aReader, aSupportedFieldTypeList, aPresenceSupported , aServiceName, aStoreType);
    }

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::NewL
// ---------------------------------------------------------------------------
//
 CVIMPSTFieldPropertyArray* CVIMPSTFieldPropertyArray::NewL
        (const MVPbkFieldTypeList& aSupportedFieldTypeList, MVPbkStoreContact& aContact,
        TResourceReader& aReader,
        TBool aPresenceSupported,
        TDesC& aServiceName, TStoreType aStoreType )
    {
    CVIMPSTFieldPropertyArray* self = new(ELeave) CVIMPSTFieldPropertyArray( aContact );
    CleanupStack::PushL(self);
    self->ConstructL( aSupportedFieldTypeList ,aReader, aPresenceSupported, aServiceName, aStoreType);
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::~CVIMPSTFieldPropertyArray
// ---------------------------------------------------------------------------
//
CVIMPSTFieldPropertyArray::~CVIMPSTFieldPropertyArray()
    {
    iFieldProperties.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::Count
// ---------------------------------------------------------------------------
//
TInt CVIMPSTFieldPropertyArray::Count() const
    {
    return iFieldProperties.Count();
    }

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::At
// ---------------------------------------------------------------------------
//
 CVIMPSTFieldProperty& CVIMPSTFieldPropertyArray::At
        (TInt aIndex) const
    {
    return *iFieldProperties[aIndex];
    }

// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::GetFieldById
// ---------------------------------------------------------------------------
//
 CVIMPSTFieldProperty* CVIMPSTFieldPropertyArray::GetFieldById( TInt aFieldId ) 
    {
    CVIMPSTFieldProperty* retField = NULL;
    
    TInt count = Count();
    
    for( TInt i=0; i< count; i++ )
	    {
	    CVIMPSTFieldProperty* field = iFieldProperties[i];
	    if( field && field->FieldId() == aFieldId  )
		    {
		    retField = field;
		    break; 	
		    }
	    }
    return retField;
    }


// ---------------------------------------------------------------------------
// CVIMPSTFieldPropertyArray::ReadFieldPropertiesL
// ---------------------------------------------------------------------------
//
 void CVIMPSTFieldPropertyArray::ReadFieldPropertiesL(
         TResourceReader& aReader,
         const MVPbkFieldTypeList& aSupportedFieldTypeList,
         TBool aPresenceSupported,
         TDesC& aServiceName, TStoreType aStoreType)
     {
     // aSupportedFieldTypeList is a list of the supported field type of the used store
     const TInt count = aReader.ReadInt16();

     for (TInt i = 0; i < count; ++i)
         {
         // do not change the order of reading from aReader
         const TInt fieldTypeResId = aReader.ReadInt32();
         TInt fieldId = aReader.ReadInt8(); 
         TInt maxLength = aReader.ReadInt16();
         TInt iconId = aReader.ReadInt8();
         HBufC* defaultLabel = aReader.ReadHBufCL();
         if( !defaultLabel )
             {
             defaultLabel = KNullDesC().AllocL();
             }
         CleanupStack::PushL(defaultLabel);
         const MVPbkFieldType* fieldType = aSupportedFieldTypeList.Find( fieldTypeResId );

         if( fieldId == EVIMPSTFieldIdStatusMsg && aPresenceSupported ) // currently not supported
             {
             CVIMPSTFieldProperty* property = CVIMPSTFieldProperty::NewLC( fieldId, iconId, maxLength, *defaultLabel );
             iFieldProperties.AppendL( property );
             CleanupStack::Pop(property);// property
             }
         else if( fieldType )
             {
             MVPbkStoreContactFieldCollection& fieldCollection = iContact.Fields();
             TInt fieldCount = fieldCollection.FieldCount();
             for( TInt i=0; i< fieldCount; i++ )
                 {
                 MVPbkStoreContactField& field = fieldCollection.FieldAt( i );	
                 const MVPbkFieldType* type = field.BestMatchingFieldType() ;
                 if( fieldType == type )
                     {	
                     if( fieldId == EVIMPSTFieldIdImpp )
                         {
                         const MVPbkContactFieldData& fieldData = field.FieldData();
                         const MVPbkContactFieldUriData& uri = MVPbkContactFieldUriData::Cast(fieldData);
                         TPtrC xspId = uri.Uri();
                         TInt index = xspId.Find( KColon );
                         if( index>0 )
                             {
                             TPtrC servicename = xspId.Left(index);      
                             if(  servicename.Compare(aServiceName) == 0)
                                 {
                                 CVIMPSTFieldProperty* property = CVIMPSTFieldProperty::NewLC( fieldId, iconId, maxLength, *defaultLabel );
                                 property->SetStoredFieldL( field );
                                 iFieldProperties.AppendL( property );
                                 CleanupStack::Pop(property); //property 
                                 }
                             }
                          }
                     else if( fieldId == EVIMPSTFieldIdDisplayName || 
                             fieldId == EVIMPSTFieldIdFirstName || 
                             fieldId == EVIMPSTFieldIdLastName )
                         {
                         CVIMPSTFieldProperty* property = CVIMPSTFieldProperty::NewLC( fieldId, iconId, maxLength, *defaultLabel );
                         property->SetStoredFieldL( field );
                         iFieldProperties.AppendL( property );
                         CleanupStack::Pop(property); //property
                         }
                     else if( aStoreType == EStoreServer )  //append all fields to details view if it is server store
                         {
                         CVIMPSTFieldProperty* property = CVIMPSTFieldProperty::NewLC( fieldId, iconId, maxLength, *defaultLabel );
                         property->SetStoredFieldL( field );
                         iFieldProperties.AppendL( property );
                         CleanupStack::Pop(property); //property
                         }
                     }
                 } 
             }
         CleanupStack::PopAndDestroy(defaultLabel); 
         }

     }

// End of File
