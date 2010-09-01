/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Key - value container.
*
*/

// INCLUDE FILES
#include	<utf.h> // for unicode character conversions
#include	<centralrepository.h>
#include    <e32std.h>
#include    "cvimpstsettingskeyvaluepair.h"
#include    "vimpstsettingsstoredefinitions.h"
#include    "vimpstsettingsstorecenrepuids.h"
//MACROS 

//Helper macro to check value type in data set
#define REQUIRE_SET_VALUE_TYPE( aType ) \
    { \
    TBool _settable = ( iValueType == EValueTypeNone ) || \
                      ( iValueType == aType ); \
    if( !_settable )\
        {\
        return KErrGeneral;\
        }\
    } \

//Helper macro to check value type in data get
#define REQUIRE_GET_VALUE_TYPE( aType ) \
    { \
    if( iValueType == EValueTypeNone ) \
        { \
        return KErrNotFound; \
        } \
    TBool _readable = ( iValueType == aType ); \
    if( !_readable ) \
        { \
        return KErrGeneral; \
        } \
    } \


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::New()
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePair* CVIMPSTSettingsKeyValuePair::New( const TDesC16& aKey )
    {
    //Non leaving two phased constructor!!!
    				 
    CVIMPSTSettingsKeyValuePair* self = new CVIMPSTSettingsKeyValuePair;	// CSI: 62 #
    if( self )
        {
        self->iKey = aKey.Alloc();
        if( !self->iKey )
            {
            delete self;
            self = NULL;
            }
        }
    
    return self;    
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::NewLC()
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePair* CVIMPSTSettingsKeyValuePair::NewLC( const TDesC& aKeyValuePairFlat )
    {
    CVIMPSTSettingsKeyValuePair* self = new (ELeave) CVIMPSTSettingsKeyValuePair;
    CleanupStack::PushL( self );
    self->ParseFlatKeyValuePairL( aKeyValuePairFlat );
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::~CVIMPSTSettingsKeyValuePair()
// Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePair::~CVIMPSTSettingsKeyValuePair()
    {
    Reset();
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::CVIMPSTSettingsKeyValuePair()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePair::CVIMPSTSettingsKeyValuePair()
    : iValueType( EValueTypeNone )
    {
    }



// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::ConstructL()
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsKeyValuePair::ConstructL( RReadStream& aStream )
    {
    
    //Default data    
    TInt keyLength = aStream.ReadInt32L();
    iKey = HBufC::NewL( keyLength );
    TPtr keyPtr( iKey->Des() );
    aStream.ReadL( keyPtr, keyLength );
    
    iValueType = static_cast< TValueType > ( aStream.ReadInt8L() );
    switch( iValueType )
        {        
        case EValueTypeDesC16: 
            {
            TInt descLength = aStream.ReadInt32L();
            iBuf16 = HBufC16::NewL( descLength );
            TPtr16 descPtr16( iBuf16->Des() );
            aStream.ReadL( descPtr16, descLength );
            break;
            }

        //FLOW THROUGH
        case EValueTypeNone:
        default:
            {
            break;
            }
        };
    }



// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::Reset()
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsKeyValuePair::Reset()
    {
    switch( iValueType )
        {
        case EValueTypeDesC16: 
            {
            delete iBuf16;
            iBuf16 = NULL;
            break;
            }
        
        //FLOW THROUGH       
        case EValueTypeNone:
        default:
            {
            break;
            }
        };
    
    iValueType = EValueTypeNone;

    delete iKey;
    iKey = NULL;
    }



// -----------------------------------------------------------------------------
// CVIMPSTUiSAPKeyValuePair::Key()
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTSettingsKeyValuePair::Key() const
    {
    return *iKey;
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::SetValue()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePair::SetValue( const TDesC16& aValue )
    {
    REQUIRE_SET_VALUE_TYPE( EValueTypeDesC16 )

	// Allocate memory for the new value
    HBufC16* tmp = aValue.Alloc();
    if( !tmp )
        {
        return KErrNoMemory;
        }   

 	// Swap pointers
    HBufC16* tmp2( NULL );
   	tmp2 = iBuf16;
    iBuf16 = tmp;
    iValueType = EValueTypeDesC16;
	
	TBool dataTooBig( IsDataTooBig() );
	TInt err( KErrNone );

	// Rollback if data too bog
	if( dataTooBig )
		{
		iBuf16 = tmp2;
		delete tmp;
		err = KErrTooBig;
		}
	// Otherwise delete the old value
	else
		{
		delete tmp2;
		}	 	    
    return err;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::GetValue()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePair::GetValue( TPtrC16& aValue ) const
    {
    REQUIRE_GET_VALUE_TYPE( EValueTypeDesC16 )

    aValue.Set( *iBuf16 );
    return KErrNone;
    }
	
// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::KeyValuePairFlatLC()
// -----------------------------------------------------------------------------
//

HBufC* CVIMPSTSettingsKeyValuePair::KeyValuePairFlatLC()
	{
	
	// reserve memory for 2 field separators
	HBufC* flatBuf = HBufC::NewLC( NCentralRepositoryConstants::KMaxUnicodeStringLength );  
	
	TPtr flatBufPtr( flatBuf->Des() );
   	
    flatBufPtr.Append( *iKey );
    flatBufPtr.Append( KKeyValuePairFieldSeparator );
	flatBufPtr.AppendNum( iValueType, EDecimal );
   	flatBufPtr.Append( KKeyValuePairFieldSeparator );
		
	switch( iValueType )
        {
        case EValueTypeDesC16: 
            {
          	flatBufPtr.Append( *iBuf16 );
            break;
            }

        //FLOW THROUGH
        case EValueTypeNone:
        default:
            {
            break;
            }
        };
	return flatBuf;
	}

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::IsDataTooBig()
// -----------------------------------------------------------------------------
//
TBool CVIMPSTSettingsKeyValuePair::IsDataTooBig() const
    {
    return ( DataSizeDec() > ( NCentralRepositoryConstants::KMaxUnicodeStringLength - 
                    2*KKeyValuePairFieldSeparator().Length() - 1) );
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::DataSizeDec()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePair::DataSizeDec() const
    {    
    TInt dataSize( iKey->Length() );
    
    switch( iValueType )
        {    
        case EValueTypeDesC16: 
            {
            dataSize += iBuf16->Length();
            break;
            }

        //FLOW THROUGH
        case EValueTypeNone:
        default:
            {
            break;
            }
        };

    return dataSize;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePair::ParseFlatKeyValuePairL()
// -----------------------------------------------------------------------------
//

void CVIMPSTSettingsKeyValuePair::ParseFlatKeyValuePairL( const TDesC& aKeyValuePairFlat )
    {
    TInt offset( aKeyValuePairFlat.Find( KKeyValuePairFieldSeparator ) );
    TInt offset2( aKeyValuePairFlat.Mid( offset + 
            KKeyValuePairFieldSeparator().Length() ).Find(
                    KKeyValuePairFieldSeparator ) 
    );

    if( ( offset == KErrNotFound ) || ( offset2 == KErrNotFound ) )
        {
        //Incorrect format, leave
        User::Leave( KErrNotFound );
        }

    TPtrC key( aKeyValuePairFlat.Left( offset ) );

    iKey = key.AllocL();

    TPtrC valueType( aKeyValuePairFlat.Mid( offset + 
            KKeyValuePairFieldSeparator().Length(), offset2 ) );
    TLex lexer( valueType );
    TInt valueTypeInt( 0 );
    User::LeaveIfError( lexer.Val( valueTypeInt ) ); 
    TPtrC value( aKeyValuePairFlat.Mid(offset + offset2 + 
            2*KKeyValuePairFieldSeparator().Length() ) ); // skip 2 separators
            switch( valueTypeInt )
                {               
                case EValueTypeDesC16:
                    {
                    User::LeaveIfError( SetValue( value ) );
                    break;
                    }

                    //FLOW THROUGH
                case EValueTypeNone:
                default:
                    {
                    break;
                    }
                }
    }
//  End of File

