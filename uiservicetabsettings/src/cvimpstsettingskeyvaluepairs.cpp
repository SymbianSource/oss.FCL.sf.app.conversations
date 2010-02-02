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
* Description:  Container for individual key-value pairs
*
*/

// INCLUDE FILES
#include    "cvimpstsettingskeyvaluepairs.h"
#include    "cvimpstsettingskeyvaluepair.h"
#include    "cvimpstsettingslookupkeyvaluepair.h"

#include    <e32std.h>

//CONSTANTS
const TInt KKeyValuePairGranurality = 5;


// LOCAL CONSTANTS AND MACROS
#define KEYWORD_ORDER TLinearOrder< CVIMPSTSettingsKeyValuePair > ( CompareKeys )


// ============================ LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CompareKeys()
// Comparison function for TLinearOrder
//
// @param aLhs The left hand side in comparison.
// @param aRhs The right hand side in comparison.
// @return Result from compare
// -----------------------------------------------------------------------------
//
TInt CompareKeys( const CVIMPSTSettingsKeyValuePair& aLhs, const CVIMPSTSettingsKeyValuePair& aRhs )
    {
    return aLhs.Key().Compare( aRhs.Key() );
    }
 
// -----------------------------------------------------------------------------
// SetValueToPairs()
// Templated algorithm to set or update new value to list of key values.
//
// @param aPairs The list of key-value pairs.
// @param aLookupKey The lookup key to use in searching.
// @param aKey The key for which to assign the value.
// @param aValue Templated data type to assign to key.
// @return Error code.
// -----------------------------------------------------------------------------
//
template< typename T >
TInt SetValueToPairs( RPointerArray< CVIMPSTSettingsKeyValuePair >& aPairs,
                      CVIMPSTSettingsLookupKeyValuePair& aLookupKey,
                      const TDesC& aKey, 
                      const T& aValue )
    {
    aLookupKey.SetLookupKey( aKey );
    

    TInt err = KErrNone;
    TInt index = aPairs.FindInOrder( &aLookupKey, KEYWORD_ORDER );
    
    if( index != KErrNotFound )
        {
        //Update existing
        err = aPairs[ index ]->SetValue( aValue ); 
        }

    else
        {
        //create new
        CVIMPSTSettingsKeyValuePair* newPair = CVIMPSTSettingsKeyValuePair::New( aKey ); 
        if( !newPair )
            {
            err = KErrNoMemory;
            }
        else
            {
            //Insert new 
            
            err = newPair->SetValue( aValue );
			if( err == KErrNone )
   				{
   				err = aPairs.InsertInOrderAllowRepeats( newPair, KEYWORD_ORDER ); 
  				 }

			if( err != KErrNone )
   				{
   				delete newPair;
   				}

        	}
        }
    aLookupKey.ResetLookupKey();
    return err;
    }




// -----------------------------------------------------------------------------
// GetValueFromPairs()
// 
// Templated algorithm to get value from list of key values.
//
// @param aPairs The list of key-value pairs.
// @param aLookupKey The lookup key to use in searching.
// @param aKey The key for which to get the value.
// @param aValue Templated data type to get.
// @return Error code.
// -----------------------------------------------------------------------------
//
template< typename T >
TInt GetValueFromPairs( const RPointerArray< CVIMPSTSettingsKeyValuePair >& aPairs,
                        CVIMPSTSettingsLookupKeyValuePair& aLookupKey,
                        const TDesC& aKey, 
                        T& aValue ) 
    {
    aLookupKey.SetLookupKey( aKey );


    TInt index = aPairs.FindInOrder( &aLookupKey, KEYWORD_ORDER );
    if( index != KErrNotFound )
        {
        index = aPairs[ index ]->GetValue( aValue ); 
        }

    
    aLookupKey.ResetLookupKey();
    return index;
    }

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CVIMPSTUiSAPKeyValuePairs::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePairs* CVIMPSTSettingsKeyValuePairs::NewL()
    {
    CVIMPSTSettingsKeyValuePairs* self = new (ELeave) CVIMPSTSettingsKeyValuePairs;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }



// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::~CVIMPSTSettingsKeyValuePairs()
// Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePairs::~CVIMPSTSettingsKeyValuePairs()
    {
    iPairs.ResetAndDestroy();
    delete iLookupKey;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::CVIMPSTSettingsKeyValuePairs()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsKeyValuePairs::CVIMPSTSettingsKeyValuePairs()
    : iPairs( KKeyValuePairGranurality )
    {
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::ConstructL)
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsKeyValuePairs::ConstructL()
    {
    iLookupKey = CVIMPSTSettingsLookupKeyValuePair::NewL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::SetValueDesC16()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePairs::SetValueDesC16( const TDesC& aKey, const TDesC16& aValue )
    {
    return SetValueToPairs( iPairs, *iLookupKey, aKey, aValue );
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::GetValueDesC16()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePairs::GetValueDesC16( const TDesC& aKey, TPtrC16& aValue )
    {
    return GetValueFromPairs( iPairs, *iLookupKey, aKey, aValue );
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::DeletePair()
// -----------------------------------------------------------------------------
//
TInt CVIMPSTSettingsKeyValuePairs::DeletePair( const TDesC& aKey )
    {    
    iLookupKey->SetLookupKey( aKey );
    
    TInt index = iPairs.FindInOrder( iLookupKey, KEYWORD_ORDER );
    if( index != KErrNotFound )
        {
        delete iPairs[ index ]; 
        iPairs.Remove( index );
        index = KErrNone;
        }
     
    iLookupKey->ResetLookupKey();
    
    return index;
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsKeyValuePairs::Pairs()
// -----------------------------------------------------------------------------
//
RPointerArray< CVIMPSTSettingsKeyValuePair >& CVIMPSTSettingsKeyValuePairs::Pairs() 
    {
    return iPairs;
    }

//  End of File

