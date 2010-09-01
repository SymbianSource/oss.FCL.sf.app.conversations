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
* Description:  oss protocol plugin connection
*
*/

#include "cossprotocolconnection.h"

#include <badesca.h>
#include <ximpcontextclientinfo.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpprotocolconnection.h>
#include <protocolpresencefeatures.h>
#include <ximpbase.h>
#include <ximpidentity.h>
#include <presenceinfo.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpserviceinfo.h>

#include "cossprotocolconnectionmanager.h"
#include "cossprotocolimfeatures.h"

#include "cossprotocolloginrequest.h"
#include "mossprotocolconnectionmanager.h"

#include "cossprotocollogoutrequest.h"
#include <ximperrors.hrh>


// ======== COSSProtocolConnection - MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COSSProtocolConnection::COSSProtocolConnection()
// ---------------------------------------------------------------------------
//
COSSProtocolConnection::COSSProtocolConnection()
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::COSSProtocolConnection() Start-End" ) );
	}

// ---------------------------------------------------------------------------
// COSSProtocolConnection::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::ConstructL (
    const MXIMPServiceInfo&  /*aServiceInfo*/ ,
    const MXIMPContextClientInfo& /*aClientCtxInfo*/ )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::ConstructL() Start" ) );

//	PLUGIN_UNUSED_PARAM ( aClientCtxInfo );
	iConnMan = COSSProtocolConnectionManager::NewL();

//	iPresFeatures = COSSProtocolPresenceFeatures::NewL ( *iConnMan );
	iImFeatures = COSSProtocolImFeatures::NewL ( *iConnMan );
	//LOGGER ( TXT ( "COSSProtocolConnection::ConstructL() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolConnection* COSSProtocolConnection::NewL (
    const MXIMPServiceInfo& aServiceInfo,
    const MXIMPContextClientInfo& aClientCtxInfo )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::NewL() Start" ) );

	COSSProtocolConnection* self = new ( ELeave ) COSSProtocolConnection;
	CleanupStack::PushL ( self );
	self->ConstructL ( aServiceInfo, aClientCtxInfo );
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSProtocolConnection::NewL() End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::~COSSProtocolConnection()
// ---------------------------------------------------------------------------
//
COSSProtocolConnection::~COSSProtocolConnection()
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::~COSSProtocolConnection() Start" ) );

	delete iConnMan;
	delete iImFeatures;
	//LOGGER ( TXT ( "COSSProtocolConnection::~COSSProtocolConnection() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* COSSProtocolConnection::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::GetInterface() Start" ) );

	if ( aInterfaceId == MXIMPProtocolConnection::KInterfaceId )
		{
		MXIMPProtocolConnection* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
		User::Panic ( KOSSProtocolConnection, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolConnection::GetInterface() End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::GetInterface() const
// ---------------------------------------------------------------------------
//
const TAny* COSSProtocolConnection::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions ) const
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::GetInterface() const Start" ) );

	if ( aInterfaceId == MXIMPProtocolConnection::KInterfaceId )
		{
		const MXIMPProtocolConnection* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )
		{
		User::Panic ( KOSSProtocolConnection, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolConnection::GetInterface() const End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::GetInterfaceId() const
// ---------------------------------------------------------------------------
//
TInt32 COSSProtocolConnection::GetInterfaceId() const
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::GetInterfaceId() Start-End" ) );
	return MXIMPProtocolConnection::KInterfaceId;
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::PrimeHost()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::PrimeHost ( MXIMPProtocolConnectionHost& aHost )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::PrimeHost() Start" ) );

	iConnMan->SetHost ( aHost );

	//LOGGER ( TXT ( "COSSProtocolConnection::PrimeHost() Start" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::OpenSessionL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::OpenSessionL (
    const MXIMPContextClientInfo& /*aClientCtxInfo*/,
    TXIMPRequestId aOpId )
	{
	
	iConnMan->HandleToHost().HandleRequestCompleted(aOpId,KErrNone);
	
	//LOGGER ( TXT ( "COSSProtocolConnection::OpenSessionL() End" ) );
	}

// ---------------------------------------------------------------------------
// COSSProtocolConnection::OpenSessionL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::OpenSessionL ( const TInt& /*aSettingsId*/,
        TXIMPRequestId aReqId )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::OpenSessionLsettingsid \n" ) );

	iConnMan->HandleToHost().HandleRequestCompleted(aReqId,KErrNone);
	

	//LOGGER ( TXT ( "COSSProtocolConnection::OpenSessionLsettingsid() End" ) );
	}

// ---------------------------------------------------------------------------
// COSSProtocolConnection::OpenSessionL()
// ---------------------------------------------------------------------------
//

void COSSProtocolConnection::OpenSessionL (
    const MXIMPContextClientInfo& /*aContextClient*/,
    const TInt& /*aSettingsId*/,
    TXIMPRequestId /*aReqId*/ )
	{

	}

// ---------------------------------------------------------------------------
// COSSProtocolConnection::CloseSession()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::CloseSession (
    const MXIMPContextClientInfo& /*aClientCtxInfo*/,
    TXIMPRequestId aOpId )
	{
	iConnMan->HandleToHost().HandleRequestCompleted(aOpId,KErrNone);
	//LOGGER ( TXT ( "COSSProtocolConnection::CloseSession() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::ReOpenSessionsL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::ReOpenSessionsL ( TXIMPRequestId /*aOpId*/ )
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::ReOpenSessionsL() Start" ) );

	//LOGGER ( TXT ( "COSSProtocolConnection::ReOpenSessionsL() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolConnection::GetSupportedFeaturesL()
// ---------------------------------------------------------------------------
//
void COSSProtocolConnection::GetSupportedFeaturesL ( CDesC8Array& aFeatures ) const
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::GetSupportedFeaturesL() Start End" ) );
	// first empty the whole array
	aFeatures.Reset();

	// then add our features there
	aFeatures.AppendL ( KConnectionFeature );
	//LOGGER ( TXT ( "COSSProtocolConnection::GetSupportedFeaturesL" ) );
	}

// ---------------------------------------------------------------------------
// COSSProtocolConnection::ProtocolPresenceFeatures()
// ---------------------------------------------------------------------------
//
MProtocolPresenceFeatures& COSSProtocolConnection::ProtocolPresenceFeatures()
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::ProtocolPresenceFeatures() Start " ) );
	//LOGGER ( TXT ( "COSSProtocolConnection::ProtocolPresenceFeatures() End " ) );
	return *iPresenceFeature;
	}
// ---------------------------------------------------------------------------
// COSSProtocolConnection::ProtocolImFeatures()
// ---------------------------------------------------------------------------
//
/*
MProtocolImFeatures* COSSProtocolConnection::ProtocolImFeatures()
	{
	LOGGER ( TXT ( "COSSProtocolConnection::ProtocolImFeatures() Start " ) );
	return iImFeatures;
	}*/

// ---------------------------------------------------------------------------
// COSSProtocolConnection::ReleaseConnectionL()
// ---------------------------------------------------------------------------
//

void COSSProtocolConnection::ReleaseConnectionL()
	{
	//LOGGER ( TXT ( "COSSProtocolConnection::ReleaseConnectionL() Start " ) );

	//LOGGER ( TXT ( "COSSProtocolConnection::ReleaseConnectionL() End" ) );
	}
TAny* COSSProtocolConnection::GetProtocolInterface(TInt aInterfaceId)
	{
	if(aInterfaceId == MProtocolImFeatures::KInterfaceId)
		{
		return iImFeatures->GetInterface(aInterfaceId, MXIMPBase::EPanicIfUnknown ); 
		}
	return NULL;
	}

