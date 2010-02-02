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
* Description:  connection manager
*
*/

#include "cossprotocolconnectionmanager.h"

#include <e32std.h>
#include <e32base.h>
#include <ximpobjectfactory.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpstatus.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::COSSProtocolConnectionManager()
// ---------------------------------------------------------------------------
//
COSSProtocolConnectionManager::COSSProtocolConnectionManager()
	{
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnectionManager::ConstructL()
	{

	}


// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolConnectionManager* COSSProtocolConnectionManager::NewL()
	{

	COSSProtocolConnectionManager* self = new ( ELeave )
									      COSSProtocolConnectionManager();

	CleanupStack::PushL ( self );

	self->ConstructL();
	CleanupStack::Pop ( self );

	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::~COSSProtocolConnectionManager()
// ---------------------------------------------------------------------------
//
COSSProtocolConnectionManager::~COSSProtocolConnectionManager()
	{

	}


// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::HandleToHost()
// ---------------------------------------------------------------------------
//
MXIMPProtocolConnectionHost& COSSProtocolConnectionManager::HandleToHost()
	{
	return *iHostHandle;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::SetHost()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnectionManager::SetHost ( MXIMPProtocolConnectionHost& aHost )
	{
	iHostHandle = &aHost;
	}

// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::SetHost()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnectionManager::SetImHost(MProtocolImDataHost& aHost)
	{
	iHost = &aHost ;
	}	

// ---------------------------------------------------------------------------
// COSSProtocolConnectionManager::SetHost()
// ---------------------------------------------------------------------------
//
MProtocolImDataHost& COSSProtocolConnectionManager::GetImHost()
	{
	return *iHost;
	}

// End of File

