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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <e32base.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "CVIMPSTStorageContactList_UT.h"
#include "vimpststorageutils.h"
#include "vimpstdebugassert.h"
#include "vimpststorageutils.h"
#include "tvimpststoragepanics.h"
#include "cvimpststoragecontact.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCVIMPSTStorageContactList_UT::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContactList_UT::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCVIMPSTStorageContactList_UT::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Test_NewL", CCVIMPSTStorageContactList_UT::Test_NewL ),
        ENTRY( "Test_ListId", CCVIMPSTStorageContactList_UT::Test_ListId ),
        ENTRY( "Test_DisplayName", CCVIMPSTStorageContactList_UT::Test_DisplayName ),
        ENTRY( "Test_AddContactToList", CCVIMPSTStorageContactList_UT::Test_AddContactToList ),
        ENTRY( "Test_RemoveContactFromList", CCVIMPSTStorageContactList_UT::Test_RemoveContactFromList ),
        ENTRY( "Test_Count", CCVIMPSTStorageContactList_UT::Test_Count ),
        ENTRY( "Test_FilteredContact", CCVIMPSTStorageContactList_UT::Test_FilteredContact ),
        ENTRY( "Test_FilterAllowsContact", CCVIMPSTStorageContactList_UT::Test_FilterAllowsContact ),
        ENTRY( "Test_FilteredCount", CCVIMPSTStorageContactList_UT::Test_FilteredCount ),
        ENTRY( "Test_FindIndexOfContact", CCVIMPSTStorageContactList_UT::Test_FindIndexOfContact ),
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
//-----------------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_NewL
// Function to test the NewL function of StorageContactList.
//-----------------------------------------------------------------
//
TInt CCVIMPSTStorageContactList_UT::Test_NewL( CStifItemParser& aItem )
    {
    
    TInt result = KErrGeneral;
    
    TPtrC listId;
    TPtrC displayName;
    
    aItem.GetNextString( listId );
    if( listId.Compare( _L("null") ) == 0 )
        {
        listId.Set( KNullDesC );
        }
    
    aItem.GetNextString( displayName );
    if( displayName.Compare( _L("null") ) == 0 )
        {
        displayName.Set( KNullDesC );
        }

    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL(); 
    
    CVIMPSTStorageContactList* contactList = 
            CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );
    
    if( contactList )
        {
        result = KErrNone;
        delete contactList;
        }
    
    return result;
    
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_ListId
// Function to test the ListId function of StorageContactList.
// -----------------------------------------------------------
//
TInt CCVIMPSTStorageContactList_UT::Test_ListId( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TPtrC listId;
    TPtrC displayName;
    
    aItem.GetNextString( listId );
    if( listId.Compare( _L("null") ) == 0 )
        {
        listId.Set( KNullDesC );
        }
    
    displayName.Set( KNullDesC );

    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL(); 
    
    CVIMPSTStorageContactList* contactList = 
            CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );

    TPtrC resultListId;
    resultListId.Set(contactList->ListId());
    
    if( listId.Compare( resultListId ) == 0 )
        {
        result = KErrNone;
        }
    
    if( contactList )
        delete contactList;
    
    return result;
    
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_DisplayName
// Function to test the ListId function of StorageContactList.
// -----------------------------------------------------------
//
TInt CCVIMPSTStorageContactList_UT::Test_DisplayName( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TPtrC listId;
    TPtrC displayName;
    
    aItem.GetNextString( displayName );
    if( displayName.Compare( _L("null") ) == 0 )
        {
        displayName.Set( KNullDesC );
        }
    aItem.GetNextString( listId );
    if( listId.Compare( _L("null") ) == 0 )
        {
        listId.Set( KNullDesC );
        }
    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL(); 
    
    CVIMPSTStorageContactList* contactList = 
            CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );

    TPtrC resultDisplayName;
    resultDisplayName.Set(contactList->DisplayName() );
    
    if( displayName.Compare( resultDisplayName ) == 0 )
        {
        result = KErrNone;
        }
    
    if( contactList )
        delete contactList;
    
    return result;
    
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_AddContactToList
// Function to test the AddStorageContactToCacheL function of StorageContactList.
// -----------------------------------------------------------
//

TInt CCVIMPSTStorageContactList_UT::Test_AddContactToList( CStifItemParser& aItem )
    {
    TInt result = KErrNotFound;
    
    TInt flag;
    
    aItem.GetNextInt( flag );
    
    CVIMPSTStorageContact* contact = NULL;
    
    //TVIMPSTEnums::TOnlineStatus status;
    TBuf<64> userId;
    TBuf<64> displayName;
    
    TBuf<64> listId;
    
    
    userId.Copy( _L("jbl@nokia.com") );
    displayName.Copy( _L("Jabber Limited") );
    listId.Copy( _L("mycontacts") );
    
    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL();
    
    displayName.Copy( _L("my list") );
    CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );
    
    TInt index = 0; // first element.
    TInt err = 0; // no error
    switch( flag )
        {
        case 1:
            {
            // this case tests addition of a normal contact to
            // the contactlist.
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            err = list->AddStorageContactToCacheL( contact, index );
            
            if( 0 == err )
                result = KErrNone;
            
            break;
            }
        case 2:
            {
            // this case tests addition of a null contact to the
            // contactlist.
            err = list->AddStorageContactToCacheL( contact, index );
            
            if( KErrArgument == err )
                result = KErrNone;
            
            break;
            }
        case 3:
            {
            // this case tests addition of a contact by passing 
            // positive index.
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            index = 2;
            
            err = list->AddStorageContactToCacheL( contact, index );
            
            if( 0 == err )
                result = KErrNone;
            
            break;
            }
        case 4:
            {
            // this case tests addition of a contact by passing negative
            // index.
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            index = -2;
            
            err = list->AddStorageContactToCacheL( contact, index );
            
            if( 0 == err )
                result = KErrNone;

            break;
            }
        case 5:
            {
            // this case tests addition of a contact by passing zero
            // index.
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            index = 0;
            
            err = list->AddStorageContactToCacheL( contact, index );
            
            if( 0 == err )
                result = KErrNone;

            break;
            }
        case 6:
            {
            // This case adds same contact twice. 
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            list->AddStorageContactToCacheL( contact, index );
            
            index++;
            
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            err = list->AddStorageContactToCacheL( contact, index );

            if( KErrAlreadyExists == err )
                result = KErrNone;
            
            break;
            }
        case 7:
            {
            
            TInt err1, err2, err3;
            
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            err1 = list->AddStorageContactToCacheL( contact, index );

            userId.Copy( _L("bose@nokia.com") );
            displayName.Copy( _L("Boss") );
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            err2 = list->AddStorageContactToCacheL( contact, index );

            userId.Copy( _L("logitect@nokia.com") );
            displayName.Copy( _L("Logis") );
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            err3 = list->AddStorageContactToCacheL( contact, index );

            if( !err1 && !err2 && !err3)
                result = KErrNone;
            
            break;
            }
            
        }
    return result;
    
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_RemoveContactFromList
// Function to test the RemoveContactFromList function of StorageContactList.
// -----------------------------------------------------------
//

TInt CCVIMPSTStorageContactList_UT::Test_RemoveContactFromList( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    /* Currently no test cases related to this function
     * because we phonebook is stubbed and contactlink 
     * is always returned as null.
     * Once corrected, test cases can be written.
     */
    
    return result;
    }
// -----------------------------------------------------------------------------
//
TInt CCVIMPSTStorageContactList_UT::Test_Count( CStifItemParser& aItem )
    {
    
    TInt result = KErrGeneral;
    
    TInt flag;
    
    aItem.GetNextInt( flag );
    
    TBuf<64> userId;
    TBuf<64> listId;
    TBuf<64> displayName;
    CVIMPSTStorageContact* contact = NULL;
    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL();
    listId.Copy( _L("mycontacts") );
    displayName.Copy( _L("my list") );
    CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );

    TInt count = 0; // contacts count
    TInt index = 0;
    
    switch( flag )
        {
        case 1:
            {
            userId.Copy( _L("logitect@nokia.com") );
            displayName.Copy( _L("Logis") );
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            if( !list->AddStorageContactToCacheL( contact, index ) )
                {
                count++;
                }
            
            userId.Copy( _L("bose@nokia.com") );
            displayName.Copy( _L("Boss") );
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            if( !list->AddStorageContactToCacheL( contact, index ) )
                {
                count++;
                }

            // adding same contact, won't be added to list.
            userId.Copy( _L("logitect@nokia.com") );
            displayName.Copy( _L("argen") );
            contact = CVIMPSTStorageContact::NewL( userId, displayName );
            if( !list->AddStorageContactToCacheL( contact, index ) )
                {
                count++;
                }
            
            break;
            }
        case 2:
        default:
            {
            // No contacts added
            break;
            }
        }
    
    TInt listCount = list->Count();
    
    if( listCount == count )
        {
        result = KErrNone;
        }
    
    delete list;
    delete sorter;
    
    return result;
    
    }
// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_Count
// Function to test the Count function of StorageContactList.
// -----------------------------------------------------------
//

TInt CCVIMPSTStorageContactList_UT::Test_FilteredContact( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    MVIMPSTStorageContactList* list = GetList();
    
    TInt flag;
    aItem.GetNextInt( flag );
    
    switch( flag )
        {
        case 1:
            {
            // Variant 1.
            MVIMPSTStorageContact& filteredContact =
                    list->FilteredContact( 0, TVIMPSTEnums::EFilterNonOffline);
            //MVIMPSTStorageContact& filteredContact = (*list[0]
            TInt compare = ( (*list)[0].UserId() ).Compare( filteredContact.UserId() );
            if( compare == 0 )
                result = KErrNone;
            break;
            }
        case 2:
            {
            // Variant 2.
            MVIMPSTStorageContact& filteredContact =
                    list->FilteredContact( 5, TVIMPSTEnums::EFilterNonOffline);
            TInt compare = ( (*list)[5].UserId() ).Compare( filteredContact.UserId() );
            if( compare == 0 )
                result = KErrNone;
            break;
            }
        case 3:
            {
            // Variant 3.
            delete (CVIMPSTStorageContactSorter *) list;
            CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL();
            list = CVIMPSTStorageContactList::NewL( *sorter, _L("MyList"), _L("display") );
            MVIMPSTStorageContact& filteredContact =
                    list->FilteredContact( 1, TVIMPSTEnums::EFilterAway);
            TInt compare = ( (*list)[6].UserId() ).Compare( filteredContact.UserId() );
            if( compare == 0 )
                result = KErrNone;
            break;
            }
        }
    
    delete (CVIMPSTStorageContactSorter *) list;
    
    return result;
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::Test_Count
// Function to test the Count function of StorageContactList.
// -----------------------------------------------------------
//

TInt CCVIMPSTStorageContactList_UT::Test_FilterAllowsContact( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TInt inputFilter;
    TInt testCaseNumber;
    TPtrC inputString;
    TVIMPSTEnums::TFilterType filter;
    
    aItem.GetNextString( inputString );
    aItem.GetNextInt( inputFilter );
    aItem.GetNextInt( testCaseNumber );
    
    CVIMPSTStorageContact* contact = NULL;
    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL();

    // Creating a contactList
    TBuf<64> listId;
    TBuf<64> displayName;
    
    listId.Copy( _L("contacts.manu") );
    displayName.Copy( _L("Personal Contacts") );
    CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );
    
    TVIMPSTEnums::TOnlineStatus status;
    // Create a list of 8 contacts
    
    filter = GetFilter( inputFilter );
    
    if( inputString.Compare( _L("null") ) != 0 )
        {
        TBuf<64> userId;
        TInt index = 0;
        // contact number 1
        userId.Copy( _L("prashanth.ds@infosys.com") );
        if( 31 != testCaseNumber )
            {
            status = TVIMPSTEnums::EOnline;
            }
        contact = CVIMPSTStorageContact::NewL( userId, displayName );
        contact->SetOnlineStatus( status );
        list->AddStorageContactToCacheL( contact, index );
        }
    
    TBool funcValue;
    funcValue = list->FilterAllowsContact( contact, filter );
    
    if( !funcValue && testCaseNumber == 28 )
        result = KErrNone;
    
    if(  funcValue && testCaseNumber == 29 )
        result = KErrNone;
    
    if( !funcValue && testCaseNumber == 30 )
        result = KErrNone;
    
    if( funcValue && testCaseNumber == 31 )
        result = KErrNone;
        
    delete sorter;
    delete list;
    
    return result;
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::FilteredCount
// -----------------------------------------------------------
//

TInt CCVIMPSTStorageContactList_UT::Test_FilteredCount( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    MVIMPSTStorageContactList* list = GetList();
    
    TInt inputFilter;
    
    aItem.GetNextInt( inputFilter );
    
    TVIMPSTEnums::TFilterType filter;
    
    switch( inputFilter )
        {
        case 1:
            {
            filter = TVIMPSTEnums::EFilterNotDefined;
            if( 0 == list->FilteredCount( filter ) )
                {
                result = KErrNone;
                }
            }
            break;
        case 2:
            {
            filter = TVIMPSTEnums::EFilterOnline;
            if( 4 == list->FilteredCount( filter ) )
                {
                result = KErrNone;
                }
            }
            break;
        case 3:
            {
            if( list->FilteredCount( filter ) )
                {
                result = KErrNone;
                }
            }
            break;
        }
    delete (CVIMPSTStorageContactSorter *)list;
    return result;
    }


TInt CCVIMPSTStorageContactList_UT::Test_FindIndexOfContact( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    CVIMPSTStorageContactList* list = (CVIMPSTStorageContactList *) GetList();
    
    MVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( KNullDesC, KNullDesC );
    
    TInt inputFilter;
    
    aItem.GetNextInt( inputFilter );
    
    TVIMPSTEnums::TFilterType filter;

    filter = TVIMPSTEnums::EFilterOnline;
    
    switch( inputFilter )
        {
        case 1:
            {
            MVIMPSTStorageContact& contact1 = (*list)[0];
            
            if( 0 == list->FindIndexOfContact( &contact1 ) )
                {
                result = KErrNone;
                }
            }
            break;
        case 2:
            {
            MVIMPSTStorageContact& contact1 = (*list)[2];
            
            if( 2 == list->FindIndexOfContact( &contact1, filter ) )
                {
                result = KErrNone;
                }
            }
            break;
        case 3:
            {
            MVIMPSTStorageContact& contact1 = (*list)[5];
            
            if( KErrNotFound == list->FindIndexOfContact( &contact1, filter ) )
                {
                result = KErrNone;
                }
            }
            break;
        }

    delete (CVIMPSTStorageContact *) contact;
    delete (CVIMPSTStorageContactSorter *)list;

    return result;
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::GetFilter
// -----------------------------------------------------------
//

TVIMPSTEnums::TFilterType CCVIMPSTStorageContactList_UT::GetFilter( TInt aFilter )
    {
    TVIMPSTEnums::TFilterType filter;
    switch( aFilter )
        {
        case -1:
            {
            filter = TVIMPSTEnums::EFilterNotDefined;
            }
        break;
        case 0:
            {
            filter = TVIMPSTEnums::EFilterAll;
            }
        break;
        case 1:
            {
            filter = TVIMPSTEnums::EFilterAndOperation;
            }
        break;
        case 2:
            {
            filter = TVIMPSTEnums::EFilterOnline;
            }
        break;
        case 4:
            {
            filter = TVIMPSTEnums::EFilterNonBlocked;
            }
        break;
        case 8:
            {
            filter = TVIMPSTEnums::EFilterAway;
            }
        break;
        case 16:
            {
            filter = TVIMPSTEnums::EFilterNonOffline;
            }
        break;

        }
    
    return filter;
    
    }

// ----------------------------------------------------------
// CCVIMPSTStorageContactList_UT::GetStatus
// -----------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus CCVIMPSTStorageContactList_UT::GetStatus( TInt aStatus )
    {
    TVIMPSTEnums::TOnlineStatus status;
    switch( aStatus )
        {
        case 0:
            {
            status = TVIMPSTEnums::EUnknown;
            }
        break;
        case 1:
            {
            status = TVIMPSTEnums::EOffline;
            }
        break;
        case 2:
            {
            status = TVIMPSTEnums::EOnline;
            }
        break;
        case 3:
            {
            status = TVIMPSTEnums::EInvisible;
            }
        break;
        case 4:
            {
            status = TVIMPSTEnums::EAway;
            }
        break;
        case 5:
            {
            status = TVIMPSTEnums::EBusy;
            }
        break;
        case 6:
            {
            status = TVIMPSTEnums::EOnPhone;
            }
        break;
        case 7:
            {
            status = TVIMPSTEnums::EDoNotDisturb;
            }
        break;
        case 8:
            {
            status = TVIMPSTEnums::EPending;
            }
        break;
        case 9:
            {
            status = TVIMPSTEnums::EBlocked;
            }
        break;
        }
    return status;
    
    }

MVIMPSTStorageContactList* CCVIMPSTStorageContactList_UT::GetList( )
    {
    CVIMPSTStorageContact* contact = NULL;
    CVIMPSTStorageContactSorter* sorter = CVIMPSTStorageContactSorter::NewL();

    // Creating a contactList
    TBuf<64> listId;
    TBuf<64> displayName;
    
    listId.Copy( _L("contacts.manu") );
    displayName.Copy( _L("Personal Contacts") );
    
    CVIMPSTStorageContactList* list = CVIMPSTStorageContactList::NewL( *sorter, listId, displayName );
    
    // Create a list of 8 contacts
    
    TBuf<64> userId;
    TVIMPSTEnums::TOnlineStatus status;
    TInt index = 0;
        // contact number 1
    userId.Copy( _L("prashanth.ds@infosys.com") );
    displayName.Copy( _L("prashanth") );
    status = TVIMPSTEnums::EOnline;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 2
    userId.Copy( _L("dovin.fern@tcs.com") );
    displayName.Copy( _L("dovin") );
    status = TVIMPSTEnums::EOnline;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );
    
    // contact number 3
    userId.Copy( _L("basavaraj.hn@accenture.com") );
    displayName.Copy( _L("basava") );
    status = TVIMPSTEnums::EOnline;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 4
    userId.Copy( _L("nandan.av@tcs.com") );
    displayName.Copy( _L("nandan") );
    status = TVIMPSTEnums::EOnline;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 5
    userId.Copy( _L("mithun.shettigara@tataelxsi.com") );
    displayName.Copy( _L("Mithun") );
    status = TVIMPSTEnums::EOffline;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 6
    userId.Copy( _L("santosh.wagle@bel.com") );
    displayName.Copy( _L("Santhosh") );
    status = TVIMPSTEnums::EAway;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 7
    userId.Copy( _L("sandeep.hebbar@tcs.com") );
    displayName.Copy( _L("Sandeep") );
    status = TVIMPSTEnums::EPending;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    // contact number 8
    userId.Copy( _L("niranjan.murthy@mindtree.com") );
    displayName.Copy( _L("Niranjan") );
    status = TVIMPSTEnums::EBusy;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    contact->SetOnlineStatus( status );
    list->AddStorageContactToCacheL( contact, index );

    
    return list;
    
    }
//  [End of File] - Do not remove
