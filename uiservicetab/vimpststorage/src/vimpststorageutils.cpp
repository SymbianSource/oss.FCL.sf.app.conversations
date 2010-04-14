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
* Description:  Utils for storage modules.
*
*/


#include "vimpststorageutils.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"

#include "uiservicetabtracer.h"
#include "vimpstbuilddefinitions.h"

#include <escapeutils.h>
#include <collate.h>

// needed to skip prefixes in domain-neutral comparison (TUint version)
const TUint KColonUInt( ':' );

// needed to skip domains in domain-neutral comparison (TUint version)
const TUint KAtUInt( '@' );

// "test character identity and accents, ignore case"
const TInt KCollationLevel = 1;



// defines how far we are going to search for the protocol part in user id
const TInt KMaxLengthSearchProtPart = 4;

// -----------------------------------------------------------------------------
// VIMPSTStorageUtils::NeutralCompare
// -----------------------------------------------------------------------------
//
TInt VIMPSTStorageUtils::NeutralCompare( const TDesC& aId1,
        const TDesC& aId2, TBool aDomainNeutral )
    {
	TRACER_AUTO;
    // points to user part of id
    TPtrC ptrId1( aId1 );
    TPtrC ptrId2( aId2 );

	// Reduce looking for protocol part only to beginning of the WVID and
    // skip protocol part ("anything:") in the beginning of the WVID
    TInt colonPos1 = aId1.Left( KMaxLengthSearchProtPart ).Locate( KColonUInt );

    // first id
    if ( ( KErrNotFound != colonPos1 ) && ( aId1.Length() -1 != colonPos1 ) )
        {
        // contains ":", and it is not the last char
        ptrId1.Set( aId1.Mid( colonPos1 + 1 ) );
        }

    TInt colonPos2 = aId2.Left( KMaxLengthSearchProtPart ).Locate( KColonUInt );

    // second id
    if ( ( KErrNotFound != colonPos2 ) && ( aId2.Length() -1 != colonPos2 ) )
        {
        // contains ":", and it is not the last char
        ptrId2.Set( aId2.Mid( colonPos2 + 1 ) );
        }

    // find out if we have domains in the ids
    TInt domainPos1( ptrId1.Locate( KAtUInt ) );
    TInt domainPos2( ptrId2.Locate( KAtUInt ) );

    TBool domainIn1( KErrNotFound != domainPos1 );
    TBool domainIn2( KErrNotFound != domainPos2 );

    // points to domains in the neutral id
    TPtrC ptrDom1( KNullDesC );
    TPtrC ptrDom2( KNullDesC );

    // points to user parts in the neutral id
    TPtrC ptrUid1( ptrId1 );
    TPtrC ptrUid2( ptrId2 );

    // separate user id parts and domain parts
    if ( domainIn1 )
        {
        ptrDom1.Set( ptrId1.Mid( domainPos1 + 1) );
        ptrUid1.Set( ptrId1.Mid( 0, domainPos1 ) );
        }

    if ( domainIn2 )
        {
        ptrDom2.Set( ptrId2.Mid( domainPos2 + 1) );
        ptrUid2.Set( ptrId2.Mid( 0, domainPos2 ) );
        }

    // Create custom collation method to ignore punctuations
    // index 0 gets the default method
    TCollationMethod collation =
        *Mem::CollationMethodByIndex( 0 );
    collation.iFlags |= TCollationMethod::EIgnoreNone;

    // domains are compared only when it is really needed
    // check if userid part is the same in both ids   
   
    TInt idResult = ptrUid1.Compare( ptrUid2);
    
	if( idResult != 0 )
		{
		return idResult;
		}

	// id part is same, we have to compare domain

    // If domain comparison is neutral and one id is without domain
    // -> Domains are same. Other situation domainResult stays valid.
    if( aDomainNeutral && ( domainIn1 ^ domainIn2 ) )
        {
        return 0;
        }
    else
    	{
    	return ptrDom1.CompareC( ptrDom2, KCollationLevel, &collation );
    	}
    }

// -----------------------------------------------------------------------------
// VIMPSTStorageUtils::DisplayId
// -----------------------------------------------------------------------------
//
TPtrC VIMPSTStorageUtils::DisplayId( const TDesC& aId, TBool /*aListHiding*/ )
    {
	TRACER_AUTO;
    TPtrC ret( aId );

    return ret;
    }


// End of file
