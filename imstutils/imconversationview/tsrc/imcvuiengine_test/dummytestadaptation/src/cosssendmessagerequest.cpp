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
* Description:  Send message Request to the Network Server
*
*/

#include "cosssendmessagerequest.h"
#include <ximpprotocolconnectionhost.h>
#include "cossprotocolconnectionmanager.h"
#include <ximpobjectfactory.h>
#include <ximpobjectcollection.h>
#include <protocolpresentitygroupsdatahost.h>
#include <presenceobjectfactory.h>
#include <presentitygroupinfo.h>
#include <presentitygroupmemberinfo.h>
#include <ximpidentity.h>
#include <ximpdatasubscriptionstate.h>
#include <protocolpresencedatahost.h>
#include <badesca.h>

#include<imconversationinfo.h>
#include<imerrors.hrh>

/**
* operations state enums variables 
*/

enum ImStates
	{
	RecipientOffline = 1,
	SenderBlocked = 3,
	NotSupported = 5,
	  
	// add if  required
	};
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSSendMessageRequest::COSSSendMessageRequest()
// ---------------------------------------------------------------------------
//
COSSSendMessageRequest::COSSSendMessageRequest (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId ) :

		CActive ( EPriorityNormal ),
		iRequestId ( aRequestId ),
		iConnMan ( aConnMan )
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::COSSSendMessageRequest Start" ) );
	CActiveScheduler::Add ( this );

	//LOGGER ( TXT ( "COSSSendMessageRequest::COSSSendMessageRequest End" ) );
	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSSendMessageRequest::ConstructL()
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::ConstructL Start-End" ) );


	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::NewL()
// ---------------------------------------------------------------------------
//
COSSSendMessageRequest* COSSSendMessageRequest::NewL (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::NewL Start" ) );

	COSSSendMessageRequest* self = new ( ELeave )
	COSSSendMessageRequest ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );
	self->ConstructL();
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSSendMessageRequest::NewL End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::NewLC()
// ---------------------------------------------------------------------------
//
COSSSendMessageRequest* COSSSendMessageRequest::NewLC (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::NewLC Start" ) );

	COSSSendMessageRequest* self =
	    COSSSendMessageRequest::NewL ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );

	//LOGGER ( TXT ( "COSSSendMessageRequest::NewLC End" ) );
	return self;
	}

// ---------------------------------------------------------------------------
// COSSSendMessageRequest::~COSSSendMessageRequest()
// ---------------------------------------------------------------------------
//
COSSSendMessageRequest::~COSSSendMessageRequest()
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::~COSSSendMessageRequest Start" ) );
	//LOGGER ( TXT ( "COSSSendMessageRequest::~COSSSendMessageRequest End" ) );
	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::DoCancel()
// ---------------------------------------------------------------------------
//
void COSSSendMessageRequest::DoCancel()
	{
	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::RunL()
// ---------------------------------------------------------------------------
//
void COSSSendMessageRequest::RunL()
	{

	 iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, KErrNone );

	//LOGGER ( TXT ( "COSSSendMessageRequest::RunL End" ) );
	}


// ---------------------------------------------------------------------------
// COSSSendMessageRequest::RunError()
// ---------------------------------------------------------------------------
//
TInt COSSSendMessageRequest::RunError ( TInt  aError )
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::RunError Start" ) );

	iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, aError );

	delete this;
	//LOGGER ( TXT ( "COSSSendMessageRequest::RunError End" ) );
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// COSSSendMessageRequest::SendMessageL()
// ---------------------------------------------------------------------------
//
void COSSSendMessageRequest::SendMessageL ( const MImConversationInfo& aImMessage )
	{
	//LOGGER ( TXT ( "COSSSendMessageRequest::SendMessageL Start" ) );
	SetActive();
	
	User::RequestComplete( iStatus, KErrNone);
	
	//LOGGER ( TXT ( "COSSSendMessageRequest::SendMessageL End" ) );
	}

// End of file
