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
* Description:  Oss Protocol im conversation implementation
*
*/

#include <ximpbase.h>
#include <ximpidentity.h>
#include <ximpprotocolconnectionhost.h>
#include "cossprotocolimconversation.h"
#include "cossprotocolconnectionmanager.h"
#include "cosssendmessagerequest.h"
#include "mossprotocolconnectionmanager.h"
#include "ossprotocolpluginpanics.h"
#include <imconversationinfo.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* COSSProtocolImConversation::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions )
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::GetInterface() Start" ) );

	if ( aInterfaceId == MProtocolImConversation::KInterfaceId )
		{
		MProtocolImConversation* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
		User::Panic ( KOSSProtocolConversation, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolImConversation::GetInterface() End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::GetInterface()
// ---------------------------------------------------------------------------
//
const TAny* COSSProtocolImConversation::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions ) const
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::GetInterface() Start" ) );

	if ( aInterfaceId == MProtocolImConversation::KInterfaceId )
		{
		const MProtocolImConversation* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
		User::Panic ( KOSSProtocolConversation, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolImConversation::GetInterface() End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::GetInterfaceId()
// ---------------------------------------------------------------------------
//
TInt32 COSSProtocolImConversation::GetInterfaceId() const
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::GetInterfaceId() Start-End" ) );
	return MProtocolImConversation::KInterfaceId;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::COSSProtocolImConversation()
// ---------------------------------------------------------------------------
//
COSSProtocolImConversation::COSSProtocolImConversation (
    MOSSProtocolConnectionManager& aConnMan )
		: iConnMan ( aConnMan )
	{
	//LOGGER ( TXT ( "Groups::COSSProtocolImConversation() Start-End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::ConstructL()
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::ConstructL() Start" ) );
	//LOGGER ( TXT ( "COSSProtocolImConversation::ConstructL() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolImConversation* COSSProtocolImConversation::NewL (
    MOSSProtocolConnectionManager& aConnMan )
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::NewL() Start" ) );

	COSSProtocolImConversation* self =
	    new ( ELeave ) COSSProtocolImConversation ( aConnMan );
	CleanupStack::PushL ( self );
	self->ConstructL( );
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSProtocolImConversation::NewL() End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImConversation::~COSSProtocolImConversation()
// ---------------------------------------------------------------------------
//
COSSProtocolImConversation::~COSSProtocolImConversation()
	{
	//LOGGER ( TXT ( "Groups::~COSSProtocolImConversation() Start" ) );

	//LOGGER ( TXT ( "Groups::~COSSProtocolImConversation() End" ) );
	}



// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoSendMessageL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::DoSendMessageL (
    const MImConversationInfo& aImMessage,
    TXIMPRequestId aReqId )
	{
	_LIT(KInvaliedReciepient,"Invalied Recipient");	//String to be sent
	//LOGGER ( TXT ( "Groups::DoSendMessageL() Start" ) );
	TPtrC16 message = aImMessage.TextMessage();
	const MDesCArray& recipients = aImMessage.RecipientL();
	if(message == KInvaliedReciepient)
		{
		iConnMan.HandleToHost().HandleRequestCompleted ( aReqId, KErrArgument );
		return;	
		}
		
	iConnMan.HandleToHost().HandleRequestCompleted ( aReqId, KErrNone );


	//LOGGER ( TXT ( "Groups::DoSendMessageL() End" ) );
	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoForwardMessageL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::DoForwardMessageL (
    const MImConversationInfo& /*aImMessage*/,
    TXIMPRequestId /*aReqId*/ )
	{

	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoSendMessageToGroupL()
// ---------------------------------------------------------------------------
//
void  COSSProtocolImConversation::DoSendMessageToGroupL (
    const MImConversationInfo& /*aImMessage*/,
    const TDesC* /*aGroupId*/, TXIMPRequestId /*aReqId*/ )
	{

	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoBlockUsersL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId COSSProtocolImConversation::DoBlockUsersL (
    const MDesCArray* /*aUserIds*/,
    TXIMPRequestId aReqId )
	{
    return aReqId;
	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoUnBlockUsersL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId COSSProtocolImConversation::DoUnBlockUsersL (
    const MDesCArray* /*aUserIds*/,
    TXIMPRequestId aReqId )
	{
    return aReqId;
	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoGetBlockedListRequestL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::DoGetBlockedListRequestL (
    const MXIMPIdentity& /*aImMessageId*/,
    TXIMPRequestId /*aReqId*/ )
	{
   
	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoGetMessageListL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::DoGetMessageListL (
    const MXIMPIdentity& /*aImMessageId*/,
    const TDesC* /*aGroupId*/,
    const TInt /*aMessageCount*/,
    TBool /*aDeliveryReportWanted*/,
    TXIMPRequestId /*aReqId*/ )
	{

	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::DoRejectMessageL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::DoRejectMessageL (
    const MXIMPIdentity& /*aImMessageId*/,
    const TDesC& /*aMessageID*/,
    TXIMPRequestId /*aReqId*/ )
	{

	}

// ---------------------------------------------------------------------------
// COSSProtocolImConversation::CreateDataHandlerL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImConversation::CreateDataHandlerL()
	{
	//LOGGER ( TXT ( "COSSProtocolImConversation::CreateDataHandler() Start" ) );

	//LOGGER ( TXT ( "COSSProtocolImConversation::CreateDataHandler() End" ) );
	}

// End of file

