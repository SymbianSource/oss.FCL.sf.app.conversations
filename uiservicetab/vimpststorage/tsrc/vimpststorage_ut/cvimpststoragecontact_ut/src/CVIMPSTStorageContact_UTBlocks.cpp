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
#include "CVIMPSTStorageContact_UT.h"
#include "mvimpststoragecontact.h"
#include "cvimpststoragecontact.h"
//#include <MVPbkContactLink.h>
#include "s_MVPbkContactLinkstub.h"

_LIT( KUserName, "nokia@nokia.com" );
_LIT( KDisplayName, "connectingpeople" );
_LIT( KUserName1, "sonyericsson@sony.com" );

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContact_UT::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCVIMPSTStorageContact_UT::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCVIMPSTStorageContact_UT::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        //ENTRY( "Example", CCVIMPSTStorageContact_UT::ExampleL ),
        ENTRY( "CreateStorageContact", CCVIMPSTStorageContact_UT::Test_NewL ),
        ENTRY( "SetGetUserId", CCVIMPSTStorageContact_UT::Test_SetUserId_UserIdL ),
        ENTRY( "SetAvatarContent", CCVIMPSTStorageContact_UT::Test_AvatarContentL ),
        ENTRY( "SetGetDisplayName", CCVIMPSTStorageContact_UT::Test_DisplayNameL ),
        ENTRY( "SetGetPresenceStatus", CCVIMPSTStorageContact_UT::Test_PresenceStatusL ),
        ENTRY( "SetGetStatusText", CCVIMPSTStorageContact_UT::Test_StatusTextL ),
        ENTRY( "SetGetAvatarIndex", CCVIMPSTStorageContact_UT::Test_AvatarIndexL ),
        ENTRY( "ContactLink", CCVIMPSTStorageContact_UT::Test_ContactLinkL ),
        //ADD NEW ENTRY HERE SetGetAvatarIndex
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_NewL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    TPtrC userId;
    aItem.GetNextString( userId );
    
    if( userId.Compare( _L("null") ) == 0 )
        {
        userId.Set( KNullDesC );
        }
    
    TPtrC displayName;
    aItem.GetNextString( displayName );
    if( displayName.Compare( _L("null") ) == 0 )
        {
        displayName.Set( KNullDesC );
        }
    
    TPtrC contactLinkFlag;
    aItem.GetNextString( contactLinkFlag );
    CVIMPSTStorageContact* contact = NULL;
    MVPbkContactLink* contactLink = NULL;
    HBufC8* aAvatarContent = NULL;
    if( contactLinkFlag.Compare( _L("CLINK")) == 0)
        {
        contact = CVIMPSTStorageContact::NewL( userId, displayName, *contactLink,*aAvatarContent );
        }
    else
        {
        contact = CVIMPSTStorageContact::NewL( userId, displayName );
        }
    if( contact )
        {
        result = KErrNone;
        }
    
    return result;
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_SetUserId_UserIdL( CStifItemParser& aItem )
    {
    
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TPtrC input;
    TPtrC output;
    
    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    
    if( contact )
        {

        aItem.GetNextString( input );
        
        if( input.Compare(_L("null")) == 0 )
            {
            input.Set( KNullDesC );
            contact->SetUserIdL( KNullDesC );
            }
        else
            {
            contact->SetUserIdL( input );
            }

        output.Set(contact->UserId());

        if( input.Compare( output ) == 0 )
            {
            result = KErrNone;
            }

        }
    
    return result;
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_AvatarContentL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TPtrC tempInput;
    
    TBuf8<256> input;
    
    TPtrC8 output;
    
    CVIMPSTStorageContact* contact = NULL;
    MVIMPSTStorageVPbkContactStore* VPbkContactStore = NULL;
    contact = CVIMPSTStorageContact::NewL( userId, displayName );

    if( contact )
        {
        aItem.GetNextString( tempInput );
        
        input.Copy( tempInput );
        
        if( input.Compare(_L8("null")) == 0 )
            {
            input.Copy( KNullDesC );
            }
        else
            {
            contact->SetAvatarContentL( input, *VPbkContactStore );
            }

        output.Set( contact->AvatarContent() );

        if( input.Compare( output ) == 0 )
            {
            result = KErrNone;
            }
        
        }
    return result;
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_DisplayNameL( CStifItemParser& aItem)
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TPtrC input;
    TPtrC output;
    
    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    
    if( contact )
        {

        aItem.GetNextString( input );
        
        if( input.Compare(_L("null")) == 0 )
            {
            input.Set( KNullDesC );
            }
        contact->SetNameL( input );
        output.Set(contact->Name());

        if( input.Compare( output ) == 0 )
            {
            result = KErrNone;
            }

        }
    
    return result;
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_PresenceStatusL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TPtrC input;
    TVIMPSTEnums::TOnlineStatus inputState,outputState;

    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    
    if( contact )
        {
        aItem.GetNextString( input );

        if( input.Compare( _L("EOnPhone") ) == 0 )
            {
            inputState = TVIMPSTEnums::EOnPhone;
            contact->SetOnlineStatus( inputState );
            }
        else
            {
            inputState = TVIMPSTEnums::EUnknown;
            }
        outputState = contact->OnlineStatus();

        if( inputState == outputState )
            {
            result = KErrNone;
            }
        }
    return result;
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_StatusTextL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TPtrC input;
    TPtrC output;
    
    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );
    
    if( contact )
        {

        aItem.GetNextString( input );
        
        if( input.Compare(_L("null")) == 0 )
            {
            input.Set( KNullDesC );
            }
        else
            {
            contact->SetStatusTextL( input );
            }
        
        output.Set(contact->StatusText());

        if( input.Compare( output ) == 0 )
            {
            result = KErrNone;
            }

        }
    
    return result;
    
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_AvatarIndexL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TInt input;
    TInt output;
    
    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );

    
    if( contact )
        {

        aItem.GetNextInt( input );
        
        if( input != -1 )
        contact->SetAvatarIndex( input );
        
        output = contact->AvatarIndex();

        if( input == output )
            {
            result = KErrNone;
            }

        }
    
    return result;
    
    }

// -- -- -- -- -------------------------------------------------
// @ see CVIMPSTStorageContact_UT.h for description.
// -- -- -- -- -------------------------------------------------
//
TInt CCVIMPSTStorageContact_UT::Test_ContactLinkL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    
    TBuf<64> userId;
    userId.Copy( KUserName );
    
    TBuf<64> displayName;
    displayName.Copy( KDisplayName );
    
    TInt output;
    
    CVIMPSTStorageContact* contact = NULL;
    
    contact = CVIMPSTStorageContact::NewL( userId, displayName );

    
    if( contact )
        {
        
        MVPbkContactLink * link = contact->ContactLink();
        
        if( link == NULL )
        result = KErrNone;
        
        }    
    
    return result;
    
    }


//  [End of File] - Do not remove
