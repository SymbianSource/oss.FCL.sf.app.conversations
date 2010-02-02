/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "CVIMPSTStorageContactSorter_UT.h"
#include "cvimpststoragecontactsorter.h"
#include "vimpstdebugtrace.h"
#include <flogger.h>
#include "mvimpststoragecontact.h"
#include "cvimpststoragecontact.h"
#include "tvimpstenums.h"
#include "tvimpststoragepanics.h"

_LIT( KUserId1, "vidic@mu.com");
_LIT( KUserId2, "carr@mu.com");
_LIT( KUserId3, "terry@mu.com");
_LIT( KUserId4, "gibbs@mu.com");

_LIT( KDisplayName1, "vidic");
_LIT( KDisplayName2, "carr");
_LIT( KDisplayName3, "terry");
_LIT( KDisplayName4, "gibbs");
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContactSorter_UT::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCVIMPSTStorageContactSorter_UT::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContactSorter_UT::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreateSorter", CCVIMPSTStorageContactSorter_UT::Test_NewL ),
        ENTRY( "DoSort", CCVIMPSTStorageContactSorter_UT::Test_SortL ),
        ENTRY( "SelectAlgorithm", CCVIMPSTStorageContactSorter_UT::SetSortAlgorithm ),
        ENTRY( "CompareContacts", CCVIMPSTStorageContactSorter_UT::Test_CompareL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::Test_NewL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    CVIMPSTStorageContactSorter* sorter = NULL;
    
    sorter = CVIMPSTStorageContactSorter::NewL();
    
    if( sorter )
        {
        result = KErrNone;
        }
    if( sorter )
        {
        delete sorter;
        }
    return result;
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::Test_SortL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TPtrC param;
    
    MVIMPSTStorageContact::TSortAlgorithm sortAlgorithm;
    
    aItem.GetNextString( param );
    
    CVIMPSTStorageContactSorter* sorter = NULL;
    
    sorter = CVIMPSTStorageContactSorter::NewL();

    // setting the algorithm to be used for sorting. 
    if( param.Compare( _L("alpha") ) == 0 )
        {
        sortAlgorithm = MVIMPSTStorageContact::ECompareAlphabetically;
        }
    else if( param.Compare( _L("presence") ) == 0 )
        {
        sortAlgorithm = MVIMPSTStorageContact::ECompareByPresence;
        }
    
    // comparing the sorted array and result array.
    // setting the result for the test case.
    TInt compareResult = -1;
    
    CreateContactArray();
    
    switch( sortAlgorithm )
        {
        case MVIMPSTStorageContact::ECompareByPresence:
            {
            // sorting the contact array
            sorter->SetSortAlgorithm( sortAlgorithm );
            CreatePresenceSortArray();
            sorter->Sort( iContactArray );
            compareResult = CompareArray( iContactArray, iPresenceSortArray );
            ResetArray( iPresenceSortArray );
            break;
            }
        case MVIMPSTStorageContact::ECompareAlphabetically:
        default:
            {
            // sorting the contact array
            sorter->SetSortAlgorithm( sortAlgorithm );
            CreateAlphabeticalSortArray();
            sorter->Sort( iContactArray );
            compareResult = CompareArray( iContactArray, iAlphabeticalSortArray );
            ResetArray( iAlphabeticalSortArray );
            break;
            }
        }
    ResetArray( iContactArray );
    
    if( -1 == compareResult )
        {
        result = KErrNone;
        }
    
    if( sorter )
        delete sorter;
    
    return result;
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::Test_CompareL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;

    CVIMPSTStorageContactSorter* sorter = NULL;
    
    sorter = CVIMPSTStorageContactSorter::NewL();
    
    CVIMPSTStorageContact* contact1 = NULL;
    CVIMPSTStorageContact* contact2 = NULL;
    TBuf<64> userId;
    TBuf<64> displayName;
    
    TPtrC param;
    aItem.GetNextString( param );
    TInt compareResult;
    
    if( param.Compare( _L("2validcontacts") ) == 0 )
        {
        // first contact
        userId.Copy(KUserId1);
        displayName.Copy( KDisplayName1 );
        contact1 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        // second contact
        userId.Copy(KUserId2);
        displayName.Copy( KDisplayName2 );
        contact2 = CVIMPSTStorageContact::NewL( userId, displayName );
        
       compareResult = sorter->Compare( *contact1, *contact2 );
       
       if( compareResult )
           {
           result = KErrNone;
           }
       
        }
    else if( param.Compare( _L("1validcontact") ) == 0 )
        {
        // first contact
        userId.Copy(KUserId1);
        displayName.Copy( KDisplayName1 );
        contact1 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        // null contact

        userId.Copy( KNullDesC );
        displayName.Copy( KNullDesC );
        contact2 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        compareResult = sorter->Compare( *contact1, *contact2 );
        
        if( compareResult ) // Not Equal
            {
            result = KErrNone;
            }

        }
    else if( param.Compare( _L("1nullcontact") ) == 0 )
        {

        // nullcontact
        userId.Copy( KNullDesC );
        displayName.Copy( KNullDesC );
        contact1 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        // second contact
        userId.Copy(KUserId2);
        displayName.Copy( KDisplayName2 );
        contact2 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        compareResult = sorter->Compare( *contact1, *contact2 );
        
        if( compareResult ) // Not Equal
            {
            result = KErrNone;
            }
        
        }
    else if( param.Compare( _L( "2nullcontacts") ) == 0 )
        {
        // first contact, null
        userId.Copy( KNullDesC );
        displayName.Copy( KNullDesC );
        contact1 = CVIMPSTStorageContact::NewL( userId, displayName );

        // second contact, null
        userId.Copy( KNullDesC );
        displayName.Copy( KNullDesC );
        contact2 = CVIMPSTStorageContact::NewL( userId, displayName );

        
        // no contact
        compareResult = sorter->Compare( *contact1, *contact2 );
        
        if( compareResult == 0 )
            {
            result = KErrNone;
            }
        
        }
    else if( param.Compare( _L( "samecontacts") ) == 0 )
        {
        // first contact
        userId.Copy(KUserId1);
        displayName.Copy( KDisplayName1 );
        contact1 = CVIMPSTStorageContact::NewL( userId, displayName );
        // second contact
        userId.Copy(KUserId1);
        displayName.Copy( KDisplayName1 );
        contact2 = CVIMPSTStorageContact::NewL( userId, displayName );
        
        compareResult = sorter->Compare( *contact1, *contact2 );
        
        if( compareResult == 0 )
            {
            result = KErrNone;
            }
        
        }
    if( contact1 )
        delete contact1;
    if( contact2 )
        delete contact2;
    if( sorter )
        delete sorter;
    return result;
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::SetSortAlgorithm( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TInt input;
    
    aItem.GetNextInt( input );
    
    CVIMPSTStorageContactSorter* sorter = NULL;
    
    sorter = CVIMPSTStorageContactSorter::NewL();
    
    switch( input )
        {
        case 1:
            {
            sorter->SetSortAlgorithm( MVIMPSTStorageContact::ECompareByPresence );
            result = KErrNone;
            }
            break;
        case 2:
            {
            sorter->SetSortAlgorithm( MVIMPSTStorageContact::ECompareAlphabetically );
            result = KErrNone;
            }
            break;
        }
    
    delete sorter;
    
    return result;
    
    }


// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
void CCVIMPSTStorageContactSorter_UT::CreateContactArray()
    {
    
    CVIMPSTStorageContact* contact = NULL;

    TBuf<64> userId;
    TBuf<64> displayName;
    
    // first contact
    userId.Copy(KUserId1);
    displayName.Copy( KDisplayName1 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EBusy );
    iContactArray.AppendL( contact );
    
    // second contact
    userId.Copy(KUserId2);
    displayName.Copy( KDisplayName2 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnline );
    iContactArray.AppendL( contact );
    
    // third contact
    userId.Copy(KUserId3);
    displayName.Copy( KDisplayName3 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EPending );
    
    iContactArray.AppendL( contact );

    // fourth contact
    userId.Copy(KUserId4);
    displayName.Copy( KDisplayName4 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnPhone );
    iContactArray.AppendL( contact );
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
void CCVIMPSTStorageContactSorter_UT::CreateAlphabeticalSortArray()
    {
    // order of sort : contact2, contact4, contact3, contact1
    CVIMPSTStorageContact* contact = NULL;

    TBuf<64> userId;
    TBuf<64> displayName;
    
    // first contact
    userId.Copy(KUserId2);
    displayName.Copy( KDisplayName2 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EBusy );
    iAlphabeticalSortArray.AppendL( contact );
    
    // second contact
    userId.Copy( KUserId4 );
    displayName.Copy( KDisplayName4 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnline );
    iAlphabeticalSortArray.AppendL( contact );
    
    // third contact
    userId.Copy(KUserId3);
    displayName.Copy( KDisplayName3 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EPending );
    iAlphabeticalSortArray.AppendL( contact );

    // fourth contact
    userId.Copy(KUserId1);
    displayName.Copy( KDisplayName1 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnPhone );
    iAlphabeticalSortArray.AppendL( contact );

    }
// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
void CCVIMPSTStorageContactSorter_UT::CreatePresenceSortArray()
    {
    // order of sort : contact2, contact1, contact4, contact 3 
    
    CVIMPSTStorageContact* contact = NULL;

    TBuf<64> userId;
    TBuf<64> displayName;
    
    // first contact
    userId.Copy(KUserId2);
    displayName.Copy( KDisplayName2 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EBusy );
    iPresenceSortArray.AppendL( contact );
    
    // second contact
    userId.Copy( KUserId1 );
    displayName.Copy( KDisplayName1 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnline );
    iPresenceSortArray.AppendL( contact );
    
    // third contact
    userId.Copy( KUserId4 );
    displayName.Copy( KDisplayName4 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EPending );
    iPresenceSortArray.AppendL( contact );

    // fourth contact
    userId.Copy( KUserId3 );
    displayName.Copy( KDisplayName3 );
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( TVIMPSTEnums::EOnPhone );
    iPresenceSortArray.AppendL( contact );
    
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContactSorter_UT::CompareArray(RPointerArray< MVIMPSTStorageContact > & aFirstArray, RPointerArray< MVIMPSTStorageContact >& aSecondArray )
    {
    // Number of contacts to be compared
    TInt count = 4;
    TInt result = -1;
    
    
    TBuf<64> userId1,userId2;
    
    
    // loop index
    TInt i = -1;
    for( i=0; i<count; i++ )
        {
        userId1.Copy( aFirstArray[i]->UserId() );
        userId2.Copy( aSecondArray[i]->UserId() );
        
        if( userId1.Compare( userId2 ) != 0 )
            {
            result = i;
            break;
            }
        }
    
    return result;
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageSorter_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
void CCVIMPSTStorageContactSorter_UT::ResetArray( RPointerArray< MVIMPSTStorageContact > & aArray )
    {
    aArray.ResetAndDestroy();
    }

//----------------------------
// To fix build break, please refer task no. in synergy.
//----------------------------
//
void Panic( TVIMPSTStoragePanics aPanic )
    {
    User::Panic( KPanicCategory, aPanic );
    }
//  [End of File] - Do not remove
