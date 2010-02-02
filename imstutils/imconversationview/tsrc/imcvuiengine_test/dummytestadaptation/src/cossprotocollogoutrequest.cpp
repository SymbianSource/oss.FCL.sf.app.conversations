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
* Description:  Login Request to the Network Server.
*
*/


#include "COSSProtocolLogoutRequest.h"
#include <ximpprotocolconnectionhost.h>
#include "mossprotocolconnectionmanager.h"
#include "COSSProtocolConnection.h"
#include "cossprotocolconnectionmanager.h"
#include <ximpobjectfactory.h>



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::COSSProtocolLogoutRequest()
// ---------------------------------------------------------------------------
//
COSSProtocolLogoutRequest::COSSProtocolLogoutRequest (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId ) :
		CActive ( CActive::EPriorityStandard  ),
		iRequestId ( aRequestId ),
		iConnMan ( aConnMan )
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::COSSProtocolLogoutRequest Start" ) );
	CActiveScheduler::Add ( this );
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::COSSProtocolLogoutRequest End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLogoutRequest::ConstructL()
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::COSSProtocolLogoutRequest Start-End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolLogoutRequest* COSSProtocolLogoutRequest::NewL (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::NewL Start" ) );

	COSSProtocolLogoutRequest* self =
	    new ( ELeave ) COSSProtocolLogoutRequest ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );
	self->ConstructL();
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::NewL End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::NewLC()
// ---------------------------------------------------------------------------
//
COSSProtocolLogoutRequest* COSSProtocolLogoutRequest::NewLC (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::NewLC Start" ) );

	COSSProtocolLogoutRequest* self =
	    COSSProtocolLogoutRequest::NewL ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );

	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::NewLC End" ) );
	return self;
	}

// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::~COSSProtocolLogoutRequest()
// ---------------------------------------------------------------------------
//
COSSProtocolLogoutRequest::~COSSProtocolLogoutRequest()
	{
	//LOGGER ( TXT ( "::~COSSProtocolLogoutRequest Start" ) );
	Cancel();
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::DoCancel()
// ---------------------------------------------------------------------------
//
void COSSProtocolLogoutRequest::DoCancel()
	{
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::RunL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLogoutRequest::RunL()
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::RunL Start" ) );

	iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, KErrNone );
	

 	delete this;

	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::RunL End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::RunError()
// ---------------------------------------------------------------------------
//
TInt COSSProtocolLogoutRequest::RunError ( TInt  aError )
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::RunError Start" ) );

	iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, aError );

	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::RunError - id:  %d" ), aError );

	delete this;
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::RunError End" ) );
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// COSSProtocolLogoutRequest::IssueLogoutRequestL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLogoutRequest::IssueLogoutRequestL()
	{
	//LOGGER ( TXT ( "COSSProtocolLogoutRequest::IssueLogoutRequestL Start" ) );

	//LOGGER ( TXT ( "COSSProtocolConnection::IssueLogoutRequestL() End" ) );
	}


// End of file
