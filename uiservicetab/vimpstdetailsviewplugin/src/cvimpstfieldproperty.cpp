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


#include "cvimpstfieldproperty.h"

#include "cvimpstfieldpropertyarray.h"
// From Virtual Phonebook
#include <MVPbkFieldType.h>
#include <MVPbkStoreContactField.h>
#include "vimpstdetailsview.hrh"
#include <MVPbkBaseContactField.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldUriData.h>
#include <MVPbkContactFieldDateTimeData.h>
// System includes
#include <barsread.h>
#include "uiservicetabtracer.h"

_LIT(KColon,":");
// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::CVIMPSTFieldProperty
// --------------------------------------------------------------------------
//    
CVIMPSTFieldProperty::CVIMPSTFieldProperty( TInt aFieldId, TInt aIconId, TInt aMaxLength )
     :iFieldId( aFieldId ),
     iIconId( aIconId ),
     iMaxLength( aMaxLength)
     {
     }

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::~CVIMPSTFieldProperty
// --------------------------------------------------------------------------
//    
CVIMPSTFieldProperty::~CVIMPSTFieldProperty()
    {
    delete iDefaultLabel;
    delete iFieldData;
    delete iField;
    }

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::ConstructL
// --------------------------------------------------------------------------
//    
inline void CVIMPSTFieldProperty::ConstructL(const TDesC&  aDefaultLabel)
    {
    if( aDefaultLabel.Length() )
        {
        iDefaultLabel = aDefaultLabel.AllocL();
        }
    }
// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::NewLC
// --------------------------------------------------------------------------
//    
CVIMPSTFieldProperty* CVIMPSTFieldProperty::NewLC
        ( TInt aFieldId, TInt aIconId, TInt aMaxLength,  const TDesC&  aDefaultLabel )
    {
    CVIMPSTFieldProperty* self = new( ELeave ) CVIMPSTFieldProperty( aFieldId, aIconId, aMaxLength);
    CleanupStack::PushL( self );
    self->ConstructL(aDefaultLabel);
    return self;
    }

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::IconId
// --------------------------------------------------------------------------
//
TInt CVIMPSTFieldProperty::IconId() 
    {
    return iIconId;
    }

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::FieldId
// --------------------------------------------------------------------------
//
TInt CVIMPSTFieldProperty::FieldId() 
    {
    if( iFieldId == EVIMPSTFieldIdImpp || 
    	iFieldId ==  EVIMPSTFieldIdEmailAddress|| 
    	iFieldId == EVIMPSTFieldIdEmailGen )
	    {
	    // these fields are map to one field
	    return EVIMPSTFieldIdImpp;
	    }
    return iFieldId;
    }
// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::DefaultLabelL
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTFieldProperty::DefaultLabelL() 
    {
   	if( iField && !iDefaultLabel )
		{
		iDefaultLabel = iField->FieldLabel().Left( iMaxLength ).AllocL();	
		}
		
    if( iDefaultLabel )
	    {
	    return 	*iDefaultLabel;
	    }

    return  KNullDesC();
    }

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::SetFieldTextL
// --------------------------------------------------------------------------
//
void CVIMPSTFieldProperty::SetFieldTextL(const TDesC& aText ) 
    {
    if( iFieldData )
	    {
	    delete iFieldData;
	    iFieldData = NULL;	
	    }
	iFieldData = aText.AllocL();
    }
// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::SetStoredFieldL
// --------------------------------------------------------------------------
//
void CVIMPSTFieldProperty::SetStoredFieldL(MVPbkStoreContactField& aField ) 
    {
    iField = aField.CloneLC();
    CleanupStack::Pop();
	}

// --------------------------------------------------------------------------
// CVIMPSTFieldProperty::FieldDataL
// --------------------------------------------------------------------------
//
const TDesC& CVIMPSTFieldProperty::FieldDataL() 
    {
	TRACER_AUTO;
    if( iField && !iFieldData )
        {
        const MVPbkContactFieldData& fieldData = iField->FieldData();
        switch( fieldData.DataType() )
            {
            case EVPbkFieldStorageTypeText:
                {
                TPtrC text = MVPbkContactFieldTextData::Cast( fieldData ).Text();
                iFieldData = text.AllocL();
                break;
                }
             case EVPbkFieldStorageTypeUri:
                {
                const MVPbkContactFieldUriData& uri = MVPbkContactFieldUriData::Cast(fieldData);

                HBufC* user = uri.Uri().AllocLC();
                TPtr userPtr( user->Des() );
                TInt index = userPtr.Find( KColon );
                if( index != KErrNotFound )
                    {
                    TInt length = userPtr.Length();
                    iFieldData = userPtr.Right( length - index - 1 ).AllocL();
                    CleanupStack::PopAndDestroy(); //user   
                    }
                else
                    {
                    CleanupStack::Pop();// user
                    iFieldData = user;      
                    }
            
                break;
                }
             case EVPbkFieldStorageTypeDateTime:
                 {
                 const MVPbkContactFieldDateTimeData& dateData =
                 MVPbkContactFieldDateTimeData::Cast( fieldData );
                 TTime date = dateData.DateTime();
                 _LIT( KDateFormat, "%D%M%Y%/0%1%/1%2%/2%3%/3" );
                 const TInt KDateLength( 12 );
                 HBufC* dateStr = HBufC::NewLC( KDateLength );
                 TPtr datePtr( dateStr->Des() );
                 date.FormatL( datePtr, KDateFormat, TLocale() );
                 CleanupStack::Pop( dateStr );
                 iFieldData = dateStr;
                 break;
                 }
             default :
                {
                break;  
                }  
            }
        }
    return iFieldData ? *iFieldData : KNullDesC();
    }

	 
// End of file
