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
* Description:  constant for VIMST ui service tab
*
*/


#ifndef TVIMPSTCONSTS_H
#define TVIMPSTCONSTS_H

//includes 
#include "tvimpstenums.h"
//TODO:: uncomment this once the phonebook exprots this header file.
//#include <extensionmanager.hrh>

//constants

_LIT( KResourceDir, "\\resource\\" );
_LIT( KResourceFileName, "vimpstuires.rsc" );
_LIT( KDefaultIconFile, "\\resource\\apps\\vimpstui.mif" );

const TInt KMaxResourcePathLength  = 512;

//used to show info not for the fetaures which are not supported yet
_LIT(KNotSupportedMessage,"Selected feature not yet supported !");

_LIT(KColon,":");	// TODO: get this service name from service tables

const TInt KProtocolUidTelepathy = 0x101FB3E7;//0x111fffff; // telepathy.

const TInt KProtocolUid12 = 0x10282EE9 ;    // imps 1.2 new uid

const TInt KProtocolUid13 = 0x10282EEA  ;    // imps 1.3 id

const TInt32 KAppId = 11;

const TInt KFriendsListMaxIdentificationLength = 100;
const TInt KUsernameMaxLength = 100;
const TInt KPasswordMaxLength = 50;
const TInt KOwnDataIndex = 0;
const TInt KStatusMsgMaxLength = 75;

_LIT(KUserId ,"testui123@gmail.com") ;
_LIT(KPassword,"test_ui123") ;
_LIT(KAddress,"talk.google.com") ;

 
_LIT(Kview1,"View1");

// xmpp use signle default list
_LIT(KFriendList ,"buddyList");
_LIT(KIMContactListId,"buddyList");
_LIT(KVPContactListId,"VPContacts");

//Used for iStores->Find
_LIT(KPbkDefaultContactDb,"contacts.cdb"); 

//Used for iStores->Find
_LIT(KVIMDefaultContactDb,"vimpststore.cdb");


// used for constructing the DB path
_LIT( KCntDbURIDomain, "cntdb://" ); 
_LIT( KCntDbPrivateDrive, "c:" ); 

_LIT( KPhoneBookDbURI, "cntdb://c:contacts.cdb");

//used for constructing DB name.
_LIT(KUnderscore, "_");

const TInt KPropertyMaxLength = 512;
const TInt KMaxSearchLimit = 10;
const TInt KTelInternetFieldLength = 512;
const TInt KLisBoxItem = 160;
const TUint KIconIdMaxLength( 128 );
_LIT( KListItemFormatRegistered, "%d\t%S\t\t%d" );
_LIT( KListItemFormatBlockList, "%d\t%S" );
_LIT( KListItemFormatSelectContact, "%d\t%S" );

_LIT(KImppPresencePrefix, "impp:");
_LIT(KVoipPresencePrefix, "voip:");  

 
const TInt KMaxLengthOfAddress = 512; 

// CONSTANTS
const TInt KNumberGranularity( 12 );

const TInt KMediumAmountOfContacts = 64;
const TInt KLargeAmountOfContacts = 200;

_LIT(KTab, "\t");
_LIT(KSpace, " ");


// CONSTANTS
const TInt KCColumn = 2;
//const TInt KPrimaryFont = 0;
const TInt KSecondaryFont = 1;

// A Column Icons
_LIT( KContactOfflineIconA,     "0\t" );
_LIT( KContactOnlineIconA,      "1\t" );
_LIT( KListAwayIcon,        "2\t" );
_LIT( KListBusyIcon,        "3\t" );
_LIT( KListInvisibleIcon,        "4\t" );
_LIT( KEmptyIconA,              "\t" );
_LIT( KEmptyIconC,              "\t" );
_LIT( KContactOpenConvIcon,   "5\t" );
_LIT( KContactNewMsgIcon,   "6\t" );
_LIT( KDefaultAvatar,   "7\t" );
_LIT( KLocalPendingRequest,   "8\t" );
_LIT( KRemotePendingRequest,   "9\t" );
_LIT( KLocalPendingRequestOffline,   "10\t" );
_LIT( KRemotePendingRequestOffline,   "11\t" );
_LIT( KFrndOnMobileIcon,   "12\t" );
_LIT( KFrndBlockedIcon,   "13\t" );
_LIT( KCallForward,				"14\t" );

const TInt KDataArraySize = 5;


const TInt KVIMPSTUISPSMaxPropertyLength = 512;

const TInt KVIMPSTTabbedViewIdBase = 10000;

const TInt KVIMPSTSearchViewIdBase = 10101;

const TInt KVIMPSTBlockedViewIdBase = 10150;
// own status
// For own status
const TInt KAmountOfOwnStatusStates = 3;
const TInt KOnlineIconIndex = 0;
//Tab format for listbox item with an icon
_LIT( KTabFormat, "%d\t");
const TInt KTabFormatLenght = 4;

const TInt KStatusTextLength = 40;

const TInt KMaxStatusMessageCount = 4;

const TInt KStatusMessageHeaderLength = 3; // length of status message header.
const TInt KSearchStringLength = 50;

const TInt KArrayGranularity = 500; 
// KStatusMessageHeaderLength must be used as amount of characters for format.
_LIT( KStatusMessageHeaderFormat, "%3d" );
_LIT( KStatusMessageHeaderLengthFormat, "%d" ); // This is length of header.

// StatusText attribute value tags
_LIT8( KStatusTextfield, "StatusText");

_LIT( KAvailable, "available");
_LIT( KBusy, "dnd");
_LIT( KAway, "away");
_LIT( KXa, "xa");
_LIT( KHidden, "hidden");
_LIT( KOffline, "offline");
_LIT (KNetworkError, "Waiting for Connection");
_LIT(KFileName, "c:\\FILENAME.CPP");


// CONSTANTS
#define  KJPegType _L("image/jpeg")
#define  KGifType  _L("image/gif")
#define  KPngType  _L("image/png")

// From ICL documentation:
// "all decoder plugins also support thumbnail decodes with ratios of 1:2, 1:4 and 1:8."
const TInt KDecodeScaleRatioMin = 2;
const TInt KDecodeScaleRatioMax = 8;
const TInt KMaxListItemLength = 100;


_LIT(KAt,"@");
//TODO:: to be enabled once the phonebook exports ExtensionManager.hrh
//#define KVIMPSTTabbedViewIdBase 10001 //EExtensionManagerTabViewStartId
//#define KVIMPSTSearchViewIdBase  10101 //EExtensionManagerViewStartId 

// Database overhead
const TInt KEmptyDbSize = 1024;
_LIT( KContactId, "ContactId" );
_LIT( KContactTable, "Contacts" );
// Name for database.
_LIT( KStorageExtn, ".db");
//hardcoded path for the local service db file. 
_LIT(KDbPath, "c:\\MeCo\\");

#endif      // TVIMPSTCONSTS_H

