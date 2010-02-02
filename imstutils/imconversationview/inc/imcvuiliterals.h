/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  literal collection file
*
*/


#ifndef IMCVUILITERALS_H
#define IMCVUILITERALS_H


const TUid KUidconversationviewApp = {0x20012425};
const TUid KUidIMCViewSoftNoteActivationId = {42};
const TUid KUidConversationsView = {1};

/// Constants
_LIT8( KMimeJpeg, "image/jpeg" );
_LIT8( KMimeGif, "Image/gif" );

_LIT( KCVTab, "\t" );

_LIT( KIMHTTPPREFIX, "http://");
_LIT( KIMRTSP, "rtsp");

_LIT(KCVColon,":");

//Used In Chat View 
//Length of "http"
const TInt KHttpLength( 4 );

//constants
const TInt KCVProtocolUidTelepathy = 0x101FB3E7;//0x111fffff; // telepathy.

const TInt32 KCVAppId = 1;

_LIT( KGroupIdPart, "/" );

const TInt KSystemOperationCode = -1;

// "test character identity and accents, ignore case"
const TInt KCollationLevel = 1;

_LIT( KMsgId, "MessageID1");

// CONSTANTS
const TInt KMaxDateStringLength = 25; // max length of date message

const TUint16 KVersionNum( 1 );

// Panic definitions
_LIT( KIMCVUIEnginePanicCategory, "IMCV Engine " );

// Application UID
const TUid KUidConversationView = {1};

//maximum 100 character for text limit in editer
#define KMAXTEXTLIMIT 100

// maximum two line for message editor
#define KMAXEDITORLINE 2

#define KMAXUSERIDLENGTH 512

#define KMAX_STATUS_LENGTH 1024


const TUid KPhonebook2AppUid = { 0x101F4CCE };
const TUid KUidIMServiceTabViewUid = {10001};

const TInt KDefaultMsgLength = 400;

// constants declaration
const TInt KAMaxResourcePathLength  = 512;
_LIT( KADefaultIconFile, "\\resource\\apps\\imcvuiapp.mif" );
_LIT8(KAppIdForBrand,"xsp");
_LIT8(KDefaultBrandIdForBrand,"xsp");

const TInt KVIMPSTUISPSMaxPropertyLength = 512;

const TUid KIMCVPropertyUid = { 0x101FB3E7 };
const TUint KIMCVConnectedKey          = 0x00000001;
/**
 * used enum's for exiting application from conversation view
 */
enum TIMCVUiPropertyEvent
    {
    EIMCVUiEventNone      = 0,   
    EIMCVUiEventExitOnly,     
    EIMCVUiEventLogoutExit       
    };

enum TIMCVConnectionState
   {
   EUnKnown,
   ELoggedOut,        
   ELoggedIn,
   EForcedLoggedOut
   };

enum TIMCVOnlineStatus
    {
    EUnknown = -1,
    EOffline,
    EOnline,
    EInvisible,
    EAway,
    EBusy,
    EOnPhone,
    EDoNotDisturb,
    EPending,
    EBlock,
    EServiceOut,
    ECallForward
    };
#endif      // IMCVUILITERALS_H

