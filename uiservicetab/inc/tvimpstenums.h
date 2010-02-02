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
* Description: enum for service tab
*
*/

#ifndef TVIMPSTENUMSPC_H
#define TVIMPSTENUMSPC_H

#include <e32base.h>
#include <bamdesca.h>

/*
	This is main interface of arraywrapper on Process component side.
	UI Side array decorator has to use this array and get the details required....
	
	
*/
//FORWARD DECLARATION
class MCCAParameter;
class MCCAConnection;

/**
 * structure
 * encapsulating data required for search 
 */
struct TVIMPSTSearchData
	{
	TPtrC iFirstName; /*First name*/
	TPtrC iLastName; /*Last name*/
	TPtrC iEmailId;	/*Email Id*/
	};
	
/**
 * structure
 * TSendData for temp use,to be removed later 
 */
struct TSendData
    {
    TPtrC   msgText; /* actual message to be sent*/
    TPtrC   recipientID; /* Id of hte receiver of the msgText.*/
    
    };
 
/**
 * structure
 * TNewContact encpsulating data required for addaing a new contact.
 */
struct TNewContact
    {
    TPtrC   listId; /* list id*/
    TPtrC   displayName; /* display name*/
    TPtrC   userId; /* user id */
    
    };
 
/**
 * structure
 * TLaunchCCAData encpsulating data required for launching the details view
 */
struct TLaunchCCAData
    {
    MCCAParameter* iParameter;	 /* parameter */
	MCCAConnection* iConnection;/* connection */
	TInt iIndex; /* index of the contact */
    
    };
 
/**
 * structure
 * TAddFriendResponse encpsulating data required for sending 
 * accapted/ rejected reactive authorization response.
 */
struct TAddFriendResponse
    {
    const TDesC& iRequesterId ; /*  user id of the requestor*/
    TBool iResponse; // Accepted/Declined ETrue/EFalse
    };

/**
 * structure
 * TVoipData encpsulating data required for voip
 */
struct TVoipData
	{
	TInt iWindowGroup; /* window group*/
	TInt iIndex; /* index of the contact */
	TUint32 iServiceId; /* service id*/
	};
/**
 * TVPbkSelectedData encapsulating data required for
 * adding contact from phone book
 * iPackedLinks HBufC8 pointer to hold packed links from selection dialog.
 * iXSPList array of xsp id(s) retrived based on iPackedLinks.
 */
struct TVPbkSelectedData
    {
    HBufC8* iPackedLinks;
    RPointerArray<HBufC> iXSPList;    
    };
// CLASS DECLARATION

/**
 *  TVIMPSTEnums all the enms and structures used across the application are defined here.
 *  @lib vimpstengine.lib
 *  @since 5.0
 */		
class TVIMPSTEnums
    {

    public:	// Enumerations and typedefs

        /**
         * A enum
         * to identify which type of service.
         */
        enum TServiceIds
            {
            EXmppService = 0, /* xmmp service */
            EXizmoService /* gizmo service */
            };
    
    
    
        enum SubServiceType
            {
            EVoip,
            EPresence,
            EIM,
            ELast		
            };
    
        enum ExtentionType
            {
            ESearch,
            EBlocking,
            EContactManagement,
            EExtentionLast	
            };	
    
        enum FeatureSupport
            {
            ESupportUnKnown,
            ESupported,
            ENotSupported
            };			
    
        /**
         * A enum
         * Registration states to control views
         */
        enum TVIMPSTRegistrationState
            {
            ESVCENotRegistered = 0,/* Service Not registered*/
            ESVCERegistered = 0x8, /* Service registered */
            ESVCENetworkConnecting = 0x4, /* service connecting */		    
            ESVCEWaitingForNetwork = 0x2, /* service waiting for connection*/
            ESVCENetworkDisConnecting = 0x1, /* service disconnecting */
            ESVCEUpdatingContacts = 0x10
            };
    
    
        /**		 
         * A enum
         * to identify Type of operation
         */
        enum TOperationType
            {
            EAddItem, /* Add item */
            ERemoveItem, /* Remove Item */
            EUpdateItem /* Update Item */
            };
    
        /**
         * A enum
         * to identify errors.
         */
        enum TImpsCSPAllErrors
            {
            EImpsErrNone = 0, /* no error */
            ECSPInsufficientGroupPrivileges, /* Insufficient group privileges error */
            ECSPNotSupported, /* Not supported error */
            ECSPInvalidParameter, /* Invalid parameter error */
            ECSPMultipleErrors /* general error */
            };
    
        /**
         * A enum
         * to identify Type of item
         */
        enum TItem
            {
            EInvalid = -1, /* unknown Item */
            EOwnStatusItem, /* Own item */
            EContactItem, /* Contact Item */
            EContactListItem, /* ContactList Item */
            EFriendRequestItem,/* Friend request item */
            EUnknonContactItem /* contact item but not in friend list */
            };
    
        /**
         * A enum
         * to identify the onlne status
         */
        enum TOnlineStatus
            {
            EUnknown = -1, /* unknown status */
            EOffline, /* Offline status */
            EOnline, /* Online status */
            EInvisible, /* Invisible status */
            EAway, /* Away status */
            EBusy, /* Busy status */
            EOnPhone,
            EDoNotDisturb,
            EPending,
            EBlocked,
            EServiceOut,
            ECallForward
            };
    
        /**
         * A enum
         * ximp context states.
         */
        enum TLoginState
            {
            EOffLine = 0, /* Context is inactive */
            ELogging,          /* Context is binding to service */
            ELoggedIn, /* Context is active and binded to service */
            ELoggingOut,           /* Context is unbinding from service */
            ERequestComplete, /* user cancel login */
            ELoggingCancel /* user cancel ongoing login */
            };
    
        /**
         * A enum
         * Type of note
         */
        enum TNoteType
            {
            EUnKnown = -1, /* unknown note type*/
            EWaiteNote, /* Wait note */
            EInfoNote, /* Info note */
            EWaiteNoteWithRSK, /* wait note with RSK */
            EConfirmationNote, /* Confirmation Note */
            EInformationNote /* Information note.*/
            };
        /**
         * A enum
         * request presence
         */
        enum TVIMPSTPresenceRequestStatus
            {
            ESVCEPresenceRequestStatusUnKnown = -1, //unknown presence statusRequest Status might be NotFound
            ESVCEPresenceRequestStatusAlwaysAsk = 0, /* ask when ever there si a request for presence.*/
            ESVCEPresenceRequestStatusAutoAccept /* auto accept when ever there is a request for presence.*/
            };    
    
    
    
        /**
         * A enum
         * Presence event
         */
        enum TVIMPSTPresenceEvent
            {
            ESVCENoEvent = 0, /* No presence Event */
            ESVCERemotePartyAcceptedPresenceRequest, /* Remote contact accepted the presence request. */
            ESVCEExtensionRemotePartyBlockedYou, /* Remote contact blocked you */
            ESVCERemotePartyWantsToWatchYou /* remote contact requested for the your presence. */
            };
    
        /**
         * A enum
         * Filter type for getting right contacts.
         * Tells us what to show; for example EFilterAll
         *  does not filter out any statuses: all contacts are shown
         */
        enum TFilterType
            {
            EFilterNotDefined = -1, /* Filter Not defined */
            EFilterAll = 0, /* Filter All*/
            EFilterAndOperation = 1, /* Filter based on AND operation*/
            EFilterOnline = 2, /* Fileter online */
            EFilterNonBlocked = 4, /*  Filter non blocked */
            EFilterAway = 8, /*  Filter Away */
            EFilterNonOffline = 16 /*  this means online and away */ 
            };
    
        /**
         * A enum
         * to identify client type
         */
        enum TClientType
            {
            EUnknownClient = 0, /*  Unknown Client*/
            EMobile, /*  Mobile Client*/
            EPC /*  PC Client*/
            };       
    
        /**
         * A enum
         * observer event types
         */
        enum TVIMPSTStorgaeEventType
            {
            EStorageEventUnkown = -1,
            EStorageEventListAddition, /* Storege ListAddition event*/
            EStorageEventListDelete, /* Storege ListDelete event*/
            EStorageEventListChanged, /* Storege ListChanged event*/
            EStorageEventContactAddition, /* Storege ContactAddition event*/
            EStorageEventContactDelete, /* Storege ContactDeletion event*/
            EStorageEventContactChange, /* storage contact information chnaged*/
            EStorageAvatarChange, 	// avatar change signal,
            EStoragePresenceChange, // Contact presence chnages
            EStorageMultiplePresenceChange, // Contact presence chnages
            EStorageOwnPresenceChange,
            EStorageAllContactRemoved, // all storage contact removed 
            EStorageContactFetchComplete,
            EStorageEventDeleteFromPbk,
            EStorageEventOwnUserChanged,
            EStorageEventUserIdPreChange, // in pre change engin emust unsubscribe and close conversatio  if exist
            EStorageEventUserIdPostChange,// engine must subscribe here
            EStorageContactReadComplete,
            EStorageContactReading,
            EStorageContactFetching,
            EStorageContactFetchExistInStore,
            EStorageContactSynchronizing
            };
    
        /**
         * A enum 
         * Search Key Types
         */
        enum TVIMPSTSearchKey
            {
            EVIMPSTSearchKeyUknown = -1, /* Unknown search key */
            EVIMPSTFirstName = 600, /* First Name search key */
            EVIMPSTMiddleName, /* Middle Name search key */
            EVIMPSTLastName, /* Last NAme search key */
            EVIMPSTFullName, /* Full Name search key */
            EVIMPSTNickName, /* Nick Name search key */
            EVIMPSTBirthday, /* BirthDay search key */
            EVIMPSTCountry, /* Country search key */
            EVIMPSTCity, /* City search key */
            EVIMPSTState, /* State search key */
            EVIMPSTEmailAddress, /* Email Address search key */
            EVIMPSTUser, /* User search key */
            EVIMPSTOrganisation, /* Organization search key */
            EVIMPSTOrganisationUnit /* Organization Unit search key */
            };	
    
    
    
        enum TVIMPSTBindStatus
            {         
            EVIMPSTBindNotDone = 0,
            EVIMPSTBindDone,
            EVIMPSTUnBinding,
            EVIMPSTUnBindWaiting,
            EVIMPSTBindFailureMode
            };
    
        enum TPresenceStatus
            {
            EVIMPSTPresenceNotSubscribed = 0, // default values:
            EVIMPSTPresenceOnline,    // subscribed
            EVIMPSTPresenceOffline,   // subscribed
            EVIMPSTPresenceBlocked,   // blocked
            EVIMPSTPresencePending,   // subscribed
            EVIMPSTPresencePendingAuthorization,
            EVIMPSTPresencePendingBlock,
            EVIMPSTPresencePendingUnBlock,
            EVIMPSTPresenceAutoSubscribe,
            EVIMPSTPresenceSubscribeWaitingForContactCreation,
            EVIMPSTPresenceSubscribeWhenContactCreated
            };
    
        enum TPresenceEvent
            {
            EVIMPSTNoEvent = 0,
            EVIMPSTPresenceDataChanged,
            EVIMPSTRemotePartyAcceptedPresenceRequest,
            EVIMPSTRemotePartyBlockedYou,
            EVIMPSTRemotePartyWantsToWatchYou,
            EVIMPSTSubcribeListReceived
            };
        
        enum TIMEventType
            {
            EIMRequestCompleted = 0,
            EIMNewMessage ,
            EIMUnreadMessage,
            EIMUnreadChange,
            EIMNewChat,
            EIMChatStarted,
            EIMChatClosed,
            EIMAllChatClosed
            };
    };

/**
 * structure
 * TVIMPSTSearchKeyData  encapsulates the data required for search key
 */
struct TVIMPSTSearchKeyData
	{
	TVIMPSTEnums::TVIMPSTSearchKey iSearchKey; /* search key */
	TPtrC iSearchLabel; /* Search lable */
	TPtrC iSearchKeyData; /* isearchkeydata */	
	};

enum TErrorType
    {
    EProtected = 1, 
    ECorrupted,
    EErrExistInBlockList,
    EErrExistInContactList
    };
    
//enums
enum TXimpOperation
    {
    EVIMPSTXimpOperationNoOperation = 0,
    EVIMPSTXimpOperationBind,
    EVIMPSTXimpOperationUnBind,
    EVIMPSTXimpOperationGetSubscribedList,
    EVIMPSTXimpOperationGetBlockList,
    EVIMPSTXimpOperationGetPresenceGrantList,
    EVIMPSTXimpOperationGetWatcherList,
    EVIMPSTXimpOperationUnsubcribeWatcherList,
    EVIMPSTXimpOperationUnsubcribeGrantRequestList,
    EVIMPSTXimpOperationSubcribeGrantRequestList,
    EVIMPSTXimpOperationWithdrawPresenceGrant,
    EVIMPSTXimpOperationGrantPresenceForPresentity,
    EVIMPSTXimpOperationCancelPresenceBlockFromPresentity,
    EVIMPSTXimpOperationBlockPresenceForPresentity,
    EVIMPSTXimpOperationPublisOwnPresence,
    EVIMPSTXimpOperationSubscribe,
    EVIMPSTXimpOperationUnsubscribe,
    
    //Start of ContactMgmt Operations
    EVIMPSTXimpOperationAddContact,
    EVIMPSTXimpOperationDeleteContact,        
    //End of ContactMgmt Operations
    
    //Start of Search Feature Operations
    EVIMPSTXimpOperationSearchContact,
    EVIMPSTXimpOperationSubscribeSearchKeys
    //End of Search Feature Operations
    
  
    }; 

enum TVIMPSTFeatureSupported //multiples of 2
	{
	EVIMPSTFeatureUnknown 		= 0,   
	EVIMPSTFeaturePublish 		= 0x1,
	EVIMPSTFeatureFetch   		= 0x2,
	EVIMPSTFeatureSubscribe 	= 0x4,
	EVIMPSTFeatureUnsubscribe 	= 0x8,
	EVIMPSTFeatureAddContact 	= 0x10,
	EVIMPSTFeatureDeleteContact = 0x20,
	EVIMPSTFeatureBlock 		= 0x40,
	EVIMPSTFeatureUnBlock 		= 0x80,
	EVIMPSTFeatureAvatar 		= 0x100,
	EVIMPSTFeatureCreateGroup 	= 0x200,
	EVIMPSTFeatureDeleteGroup 	= 0x400,
	EVIMPSTFeatureInstantMessage = 0x800,
	EVIMPSTFeatureSearch 		= 0x1000,
	EVIMPSTFeatureLast			= 0x2000
	};
	

struct TStatusAndStatusText
	{
	TVIMPSTEnums::TOnlineStatus iStatus;
	TBuf<50> iStatusText;
	};

/**
 * structure
 * encapsulating data required for avatar, filename and mimetype
 */
struct TAvatarData
    {
    TVIMPSTEnums::TOnlineStatus iStatus;
	TBuf<50> iStatusText;
    TPtrC iFileName;
    TPtrC8 iMimeType;
    };
    
#endif      // TVIMPSTENUMS_H

