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
* Description:  oss protocol plugin im features
*
*/

#include "cossprotocolimfeatures.h"
#include <protocolimconversation.h>
#include <badesca.h>
#include "cossprotocolimconversation.h"

_LIT ( KOSSProtocolImFeatures, "COSSProtocolImFeatures" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolConnection::COSSProtocolImFeatures()
// ---------------------------------------------------------------------------
//
COSSProtocolImFeatures::COSSProtocolImFeatures(COSSProtocolConnectionManager& aConnMan) : iConnMan (aConnMan)
	{
	}

// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolImFeatures::ConstructL (
    COSSProtocolConnectionManager& aConnMan )
	{
	iImConversation = COSSProtocolImConversation::NewL ( aConnMan );
	}

// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolImFeatures* COSSProtocolImFeatures::NewL (
    COSSProtocolConnectionManager& aConnMan )
	{

	COSSProtocolImFeatures* self = new ( ELeave ) COSSProtocolImFeatures(aConnMan);
	CleanupStack::PushL ( self );
	self->ConstructL ( aConnMan );
	CleanupStack::Pop ( self );

	return self;
	}

// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::~COSSProtocolImFeatures()
// ---------------------------------------------------------------------------
//
COSSProtocolImFeatures::~COSSProtocolImFeatures()
	{
	delete iImConversation;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* COSSProtocolImFeatures::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions )
	{
	if ( aInterfaceId == MProtocolImFeatures::KInterfaceId )
		{
		MProtocolImFeatures* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
		User::Panic ( KOSSProtocolImFeatures, KErrExtensionNotSupported );
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::GetInterface()
// ---------------------------------------------------------------------------
//
const TAny* COSSProtocolImFeatures::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions ) const
	{

	if ( aInterfaceId == MProtocolImFeatures::KInterfaceId )
		{
		const MProtocolImFeatures* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )
		{
		User::Panic ( KOSSProtocolImFeatures, KErrExtensionNotSupported );
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::GetInterfaceId() const
// ---------------------------------------------------------------------------
//
TInt32 COSSProtocolImFeatures::GetInterfaceId() const
	{
	return MProtocolImFeatures::KInterfaceId;
	}


// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::ImConversation()
// ---------------------------------------------------------------------------
//
MProtocolImConversation& COSSProtocolImFeatures::ImConversation()
	{
	return *iImConversation;
	}

MProtocolImGroup& COSSProtocolImFeatures::ImGroup()
    {
    MProtocolImGroup* group = NULL;
    return *group;
    }
// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::ImInvitation()
// ---------------------------------------------------------------------------
//
MProtocolImInvitation& COSSProtocolImFeatures::ImInvitation()
	{
	MProtocolImInvitation* invitation = NULL;
	return *invitation;
	}

// ---------------------------------------------------------------------------
// COSSProtocolImFeatures::ImSearch()
// ---------------------------------------------------------------------------
//
MProtocolImSearch& COSSProtocolImFeatures::ImSearch()
	{
	MProtocolImSearch* search = NULL;
	return *search;
	}
	
void COSSProtocolImFeatures::SetHost(MProtocolImDataHost& aHost )
	{
		iHost = &aHost ;
		iConnMan.SetImHost(aHost) ;
	}
	
// end of file
