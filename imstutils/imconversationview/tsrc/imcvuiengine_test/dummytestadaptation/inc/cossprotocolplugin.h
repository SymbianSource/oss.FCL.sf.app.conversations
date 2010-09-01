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


#ifndef __COSSPROTOCOLPLUGIN_H__
#define __COSSPROTOCOLPLUGIN_H__

//  INCLUDES
#include <e32base.h>
#include <ximpprotocolpluginbase.h>

// FORWARD DECLARATIONS

class MXIMPServiceInfo;

class MXIMPContextClientInfo;

class COSSProtocolConnection;

class CProcessLauncher;




/**
 * OSS protocol Plugin
 *
 * OSS plugin is ECOM plugin and shall be loaded by PrFw.
 * OSS has one or many connections.
 * @lib ?library
 * @since S60 v4.0
 */

class COSSProtocolPlugin : public CXIMPProtocolPluginBase
	{

	public:

		static COSSProtocolPlugin* NewL();
		static COSSProtocolPlugin* NewLC();

		~COSSProtocolPlugin();

	private:

		COSSProtocolPlugin();
		void ConstructL();

	public: // From MXIMPProtocolPlugin
		// For comments,see MXIMPProtocolPlugin
		void PrimeHost ( MXIMPProtocolPluginHost& aHost );

		MXIMPProtocolConnection& AcquireConnectionL (
		    const MXIMPServiceInfo& aServiceInfo,
		    const MXIMPContextClientInfo& aClientCtxInfo );
		void ReleaseConnection (
		    MXIMPProtocolConnection& aConnection );

	public: // from MXIMPBase

		TAny* GetInterface (
		    TInt32 aInterfaceId,
		    TIfGetOps  aOptions );

		const TAny* GetInterface (
		    TInt32 aInterfaceId,
		    TIfGetOps  aOptions ) const;

		TInt32 GetInterfaceId() const;

	private: // Data
		/**
		* Prime host
		* Not own.
		*/
		MXIMPProtocolPluginHost* iHost;
		/**
		* protocol connections
		* Own.
		*/
		RPointerArray< COSSProtocolConnection > iConnections;

		// owned pointer to imp
		CProcessLauncher *iIsoServerLauncher;


	};

#endif // __COSSPROTOCOLPLUGIN_H__
