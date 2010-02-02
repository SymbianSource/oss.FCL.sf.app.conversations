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


#include "COSSProtocolLoginRequest.h"
#include <ximpprotocolconnectionhost.h>
#include "mossprotocolconnectionmanager.h"
#include "COSSProtocolConnection.h"
#include "cossprotocolconnectionmanager.h"
#include <string.h>

#include <e32math.h>

#include <ximperrors.hrh>

/**
* operations state enums variables 
*/

enum LoginStates
	{
	EAuthFailed = 3,
	ETpConnError = -6997,
	// add if  required
	};
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::COSSProtocolLoginRequest()
// ---------------------------------------------------------------------------
//
COSSProtocolLoginRequest::COSSProtocolLoginRequest (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId ) :
		CActive ( CActive::EPriorityStandard  ),
		iRequestId ( aRequestId ),
		iConnMan ( aConnMan )
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::COSSProtocolLoginRequest Start" ) );
	CActiveScheduler::Add ( this );
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::COSSProtocolLoginRequest End" ) );
	}

// COSSProtocolLoginRequest::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLoginRequest::ConstructL()
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::COSSProtocolLoginRequest Start-End" ) );
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::COSSProtocolLoginRequest End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolLoginRequest* COSSProtocolLoginRequest::NewL (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::NewL Start" ) );

	COSSProtocolLoginRequest* self =
	    new ( ELeave ) COSSProtocolLoginRequest ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );
	self->ConstructL();
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSProtocolLoginRequest::NewL End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::NewLC()
// ---------------------------------------------------------------------------
//
COSSProtocolLoginRequest* COSSProtocolLoginRequest::NewLC (
    MOSSProtocolConnectionManager& aConnMan,
    TXIMPRequestId aRequestId )
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::NewLC Start" ) );

	COSSProtocolLoginRequest* self =
	    COSSProtocolLoginRequest::NewL ( aConnMan, aRequestId );
	CleanupStack::PushL ( self );

	//LOGGER ( TXT ( "COSSProtocolLoginRequest::NewLC End" ) );
	return self;
	}

// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::~COSSProtocolLoginRequest()
// ---------------------------------------------------------------------------
//
COSSProtocolLoginRequest::~COSSProtocolLoginRequest()
	{
	//LOGGER ( TXT ( "::~COSSProtocolLoginRequest Start-End" ) );
	Cancel();
	//LOGGER ( TXT ( "::~COSSProtocolLoginRequest End\n" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::DoCancel()
// ---------------------------------------------------------------------------
//
void COSSProtocolLoginRequest::DoCancel()
	{
	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::RunL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLoginRequest::RunL()
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::RunL Start" ) );

	  iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, KErrNone );

	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::RunError()
// ---------------------------------------------------------------------------
//
TInt COSSProtocolLoginRequest::RunError ( TInt  aError )
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::RunError Start" ) );

	iConnMan.HandleToHost().HandleRequestCompleted ( iRequestId, aError );

	delete this;
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::RunError End" ) );
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::IssueLoginRequestL()
// ---------------------------------------------------------------------------
//
void COSSProtocolLoginRequest::IssueLoginRequestL()
	{
	//LOGGER ( TXT ( "COSSProtocolLoginRequest::IssueLoginRequestL Start" ) );

	//LOGGER ( TXT ( "COSSProtocolConnection::OpenSessionL() End" ) );
	}
// ---------------------------------------------------------------------------
// COSSProtocolLoginRequest::GenResourceId()
// ---------------------------------------------------------------------------
//
TPtrC COSSProtocolLoginRequest::GenResourceId( const TDesC& aResourceId )
    {
    
     }  

// End of file
