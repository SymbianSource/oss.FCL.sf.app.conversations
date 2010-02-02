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
* Description:  vimpstsettings store implementation.
*
*/


// INCLUDE FILES

#include 	<e32std.h>
#include    <centralrepository.h>

#include    "cvimpstsettingscenrep.h"
#include	"cvimpstsettings.h"
#include	"cvimpstsettingskeyvaluepair.h"
#include    "cvimpstsettingskeyvaluepairs.h"
#include	"vimpstsettingsstorecenrepuids.h"
#include    "vimpstsettingsstoredefinitions.h"
#include    "rvimpstsettingsreleasearray.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::CVIMPSTUiSAPCenRep
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsCenRep::CVIMPSTSettingsCenRep()
    {
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsCenRep::ConstructL( )
    {
    iRepository = CRepository::NewL( KCRUidIMStore ); 
    }


// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsCenRep* CVIMPSTSettingsCenRep::NewL( )
    {
    CVIMPSTSettingsCenRep* self = CVIMPSTSettingsCenRep::NewLC( );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsCenRep* CVIMPSTSettingsCenRep::NewLC( )
    {
    CVIMPSTSettingsCenRep* self = new( ELeave ) CVIMPSTSettingsCenRep();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::~CVIMPSTSettingsCenRep
// Destructor
// -----------------------------------------------------------------------------
//
CVIMPSTSettingsCenRep::~CVIMPSTSettingsCenRep()
    {   
    delete iRepository;
    }



// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::StoreNewSAPL()
// Stores new SAP in central repository
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTSettingsCenRep::UpdateOldSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings )
    {  

    // no documentation for StartTransaction() return values, leave if any error
    User::LeaveIfError( iRepository->StartTransaction( CRepository::EReadWriteTransaction ) );

    iRepository->CleanupCancelTransactionPushL();	

    const RPointerArray< CVIMPSTSettingsKeyValuePair >& pairs( aSettings->KeyValuePairs().Pairs() );


    //First delete old key-value pairs 	
    RArray<TUint32> foundPairs;
    CleanupClosePushL( foundPairs );
    iRepository->FindL( EKeyValuePairBase + aServiceId, KSAPPairsMask, foundPairs );
    TInt oldCount( foundPairs.Count() );
    for( TInt i( 0 ); i < oldCount; ++i )
        {
        User::LeaveIfError( iRepository->Delete( foundPairs[i] ) );
        }

    TInt count( pairs.Count() );
    RVIMPSTSettingsReleaseArray tempArray;
    CleanupClosePushL( tempArray );
    for( TInt i( 0 );i < count; ++i )
        {
        //HBufC ownership transfers here
        HBufC* valuePairFlat = pairs[i]->KeyValuePairFlatLC();
        User::LeaveIfError( iRepository->Create( EKeyValuePairBase + i + aServiceId,
                *valuePairFlat ) );
        // CRepository->Create() is not copying the pointed data during transaction
        // so these must be stored until the transaction has completed
        tempArray.AppendL( valuePairFlat );
        CleanupStack::Pop( valuePairFlat );
        }

    TUint32 err( KErrNone );
    //passed err will be silently consumed because the value is of no use to client
    User::LeaveIfError( iRepository->CommitTransaction( err ) );	
    CleanupStack::PopAndDestroy( 3 ); //temparray, foundpairs, transaction
	
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::GetSAPL()
// Gets SAP from central repository
// (other items were commented in a header).
// -----------------------------------------------------------------------------

void CVIMPSTSettingsCenRep::GetSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings )
	{
	TRAPD( err, DoGetSettingsL( aServiceId, aSettings ) );
	if( err )
		{
		User::Leave( err );
		}
	}

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::DoGetSAPL()
// Does the actual get operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------

void CVIMPSTSettingsCenRep::DoGetSettingsL( TUint32& aServiceId, CVIMPSTSettings* aSettings )
	{
	TBool transaction( StartOwnTransaction( CRepository::EReadTransaction ) );
	if( transaction )
	    {
	    iRepository->CleanupCancelTransactionPushL();
	    }

	HBufC* tmpBuffer = HBufC::NewLC( NCentralRepositoryConstants::KMaxUnicodeStringLength );
	TPtr tmpPtr( tmpBuffer->Des() );

	RPointerArray< CVIMPSTSettingsKeyValuePair >& pairs( aSettings->KeyValuePairs().Pairs() );
	pairs.ResetAndDestroy();

	RArray<TUint32> foundPairs;
	CleanupClosePushL( foundPairs );
	iRepository->FindL( EKeyValuePairBase + aServiceId, KSAPPairsMask, foundPairs );
	TInt count( foundPairs.Count() );

	for( TInt i( 0 );i < count; ++i )
	    {
	    TPtr valuePairFlatPtr( tmpBuffer->Des() );

	    User::LeaveIfError(
	            iRepository->Get( foundPairs[ i ],
	                    valuePairFlatPtr ) );

	    // parse the key-value pair descriptor

	    CVIMPSTSettingsKeyValuePair* pair = CVIMPSTSettingsKeyValuePair::NewLC( valuePairFlatPtr );

	    pairs.AppendL( pair );

	    CleanupStack::Pop( pair );
	    }

	if( transaction )
	    {
	    TUint32 err( KErrNone );
	    User::LeaveIfError( iRepository->CommitTransaction( err ) );
	    CleanupStack::PopAndDestroy(); // transaction
	    }
	CleanupStack::PopAndDestroy( 2 ); // foundPairs, tmpBuffer	
	}


// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::StartOwnTransaction()
// Starts new transaction if there is no ongoing transaction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CVIMPSTSettingsCenRep::StartOwnTransaction( TInt aMode )
    {
     /*currently the API TransactionState() has been removed from 9.2
     * the API TransactionState was added for the synchronization purpose
     * from meco point of view, as of now we are no where, same key value is not updated across processes or exe's
     * this can be considered as a temporary fix and more analysis has to done on this
     * to provide a complete fix.
     */
    //TInt transaction( iRepository->TransactionState() );

    // if( !transaction )
    //  {
    iRepository->StartTransaction( static_cast<CRepository::TTransactionMode>( aMode ) );
    return ETrue;
    //  }
    //return EFalse;
    }

// -----------------------------------------------------------------------------
// CVIMPSTSettingsCenRep::Repository()
// Starts new transaction if there is no ongoing transaction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRepository* CVIMPSTSettingsCenRep::Repository( )const
    {
    return iRepository;
    }

//  End of File
