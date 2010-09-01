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
* Description:  adaptation for open source
*
*/

#include "cossprotocolplugin.h"

#include "cossprotocolconnection.h"

#include <badesca.h>
#include <ecom/implementationproxy.h>
#include <ximpserviceinfo.h>
#include <ximpbase.h>
#include <ximpidentity.h>
#include <presenceinfo.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpcontextclientinfo.h>
#include <featmgr.h>
#include <bldvariant.hrh>

 
// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Key value pair table to identify correct constructor
// function for the requested interface.
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY ( 0x20007B6D, COSSProtocolPlugin::NewL )
	};


// ---------------------------------------------------------------------------
// Exported function to return the implementation proxy table
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy ( TInt& aTableCount )
	{

	aTableCount = sizeof ( ImplementationTable ) / sizeof ( TImplementationProxy );

	return ImplementationTable;
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::COSSProtocolPlugin()
// ---------------------------------------------------------------------------
//
COSSProtocolPlugin::COSSProtocolPlugin()
	{

	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::ConstructL()
// ---------------------------------------------------------------------------
//
void COSSProtocolPlugin::ConstructL()
	{

	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::NewLC()
// ---------------------------------------------------------------------------
//
COSSProtocolPlugin* COSSProtocolPlugin::NewLC()
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::NewLC() Start" ) );

	COSSProtocolPlugin* self = COSSProtocolPlugin::NewL();
	CleanupStack::PushL ( self );

	//LOGGER ( TXT ( "COSSProtocolPlugin::NewLC() End" ) );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::NewL()
// ---------------------------------------------------------------------------
//
COSSProtocolPlugin* COSSProtocolPlugin::NewL()
	{
	//LOGGER ( TXT ( "=============================START=========================================" ) );
	//LOGGER ( TXT ( "COSSProtocolPlugin::NewL() Start" ) );

	COSSProtocolPlugin* self = new ( ELeave ) COSSProtocolPlugin();
	CleanupStack::PushL ( self );
	self->ConstructL();
	CleanupStack::Pop ( self );

	//LOGGER ( TXT ( "COSSProtocolPlugin::NewL() End :Address of %d" ), &self );
	return self;
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::~COSSProtocolPlugin()
// ---------------------------------------------------------------------------
//
COSSProtocolPlugin::~COSSProtocolPlugin()
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::~COSSProtocolPlugin() Start" ) );
	//delete iIsoServerLauncher;

	iConnections.ResetAndDestroy();

	//LOGGER ( TXT ( "COSSProtocolPlugin::~COSSProtocolPlugin() End" ) );
	//LOGGER ( TXT ( "=============================END=========================================" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::PrimeHost()
// ---------------------------------------------------------------------------
//
void COSSProtocolPlugin::PrimeHost ( MXIMPProtocolPluginHost& aHost )
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::PrimeHost() Start" ) );

	iHost = &aHost;

	//LOGGER ( TXT ( "COSSProtocolPlugin::PrimeHost() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::AcquireConnectionL()
// ---------------------------------------------------------------------------
//
MXIMPProtocolConnection& COSSProtocolPlugin::AcquireConnectionL (
    const MXIMPServiceInfo& aServiceInfo,
    const MXIMPContextClientInfo& aClientCtxInfo )
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::AcquireConnectionL() Start" ) );

	TInt count = iConnections.Count();
	//COSSProtocolConnection* connection = NULL;
	// count how many connections are there in plugin
	// create new connection if array is empty
	// create existing connection based on check if it
	if ( count == 0 )
		{
		COSSProtocolConnection* connection = COSSProtocolConnection::NewL ( aServiceInfo, aClientCtxInfo );
		CleanupStack::PushL ( connection );
		iConnections.AppendL ( connection );
		CleanupStack::Pop ( connection );
	//	iIsoServerLauncher->LaunchProcessL();	
		//LOGGER ( TXT ( "COSSProtocolPlugin::isoserver launched" ) );
		//LOGGER ( TXT ( "COSSProtocolPlugin::AcquireConnectionL() End" ) );
		return *connection;

		}
	else if ( count == 1 )
		{
		//LOGGER(TXT("COSSProtocolPlugin::AcquireConnectionL() End") );
		return *iConnections[0];
		}
	else
		{
		// even if there are many connection objects, we just return
		// the reference to the first connection object which is 
		// initiated by CCH
		return *iConnections[0];
		//LOGGER(TXT("COSSProtocolPlugin::AcquireConnectionL() End") );
		}
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::ReleaseConnection()
// ---------------------------------------------------------------------------
//
void COSSProtocolPlugin::ReleaseConnection ( MXIMPProtocolConnection& aConnection )
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::ReleaseConnection() Start" ) );

	TInt connectionsCount = iConnections.Count();

	for ( TInt i ( connectionsCount - 1 ); i >= 0; i-- )
		{
		MXIMPProtocolConnection* tmp = iConnections[i];

		if ( tmp == &aConnection )
			{
			TRAP_IGNORE( iConnections[i]->ReleaseConnectionL() );
			delete iConnections[i];
			iConnections.Remove ( i );
			}
		}

	//LOGGER ( TXT ( "COSSProtocolPlugin::ReleaseConnection() End" ) );
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* COSSProtocolPlugin::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions  )
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::GetInterface() Start" ) );

	if ( aInterfaceId == MXIMPProtocolPlugin::KInterfaceId )
		{
		MXIMPProtocolPlugin* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
	//	User::Panic ( KOSSProtocolPlugin, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolPlugin::GetInterface() End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::GetInterface()
// ---------------------------------------------------------------------------
//
const TAny* COSSProtocolPlugin::GetInterface ( TInt32 aInterfaceId,
        TIfGetOps aOptions ) const
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::GetInterface() const Start" ) );

	if ( aInterfaceId == MXIMPProtocolPlugin::KInterfaceId )
		{
		const MXIMPProtocolPlugin* self = this;
		return self;
		}

	if ( aOptions == MXIMPBase::EPanicIfUnknown )

		{
	//	User::Panic ( KOSSProtocolPlugin, KErrExtensionNotSupported );
		}

	//LOGGER ( TXT ( "COSSProtocolPlugin::GetInterface() const End" ) );

	return NULL;
	}


// ---------------------------------------------------------------------------
// COSSProtocolPlugin::GetInterfaceId() const
// ---------------------------------------------------------------------------
//
TInt32 COSSProtocolPlugin::GetInterfaceId() const
	{
	//LOGGER ( TXT ( "COSSProtocolPlugin::GetInterface() without parameters Start-End" ) );
	return MXIMPProtocolPlugin::KInterfaceId;
	}


// End of file

