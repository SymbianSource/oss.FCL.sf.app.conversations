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
* Description:  oss plugin implementation for XIMP Framework
*
*/


#ifndef __COSSPROTOCOLCONNECTION_H__
#define __COSSPROTOCOLCONNECTION_H__

//#include "OSSProtocloLiterals.h"

#include <e32base.h>
#include <e32def.h>
#include <e32std.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpprotocolconnection.h>


// FORWARD DECLARATIONS

class MXIMPServiceInfo;

class MXIMPContextClientInfo;

class MXIMPProtocolConnection;

class COSSProtocolConnectionManager;

class COSSProtocolImFeatures;

/**
 * COSSProtocolConnection
 *
 * OSS Protocol Connection Object. Protocol Connection Object has connection Manager.
 *
 *
 *
 */

class COSSProtocolConnection :
			public CBase,
			public MXIMPProtocolConnection
	{

	public:

		static COSSProtocolConnection* NewL ( const MXIMPServiceInfo& aServiceInfo,
		                                      const MXIMPContextClientInfo& aClientCtxInfo );


		~COSSProtocolConnection();
		
		
			
		void ReleaseConnectionL();


	private:

		COSSProtocolConnection();


		void ConstructL ( const MXIMPServiceInfo& aServiceInfo,
		                  const MXIMPContextClientInfo& aClientCtxInfo );


	public: // from MXIMPBase

		TAny* GetInterface ( TInt32 aInterfaceId,
		                     TIfGetOps  aOptions );

		const TAny* GetInterface ( TInt32 aInterfaceId,
		                           TIfGetOps  aOptions ) const;


		TInt32 GetInterfaceId() const;
		
	
	public: // from CPrFwProtocolConnection

		// Common
		void PrimeHost ( MXIMPProtocolConnectionHost& aHost );


		// session management.for comments see MXIMPProtocolConnection
		void OpenSessionL ( const MXIMPContextClientInfo& aClientCtxInfo,
		                    TXIMPRequestId aOpId );

		void OpenSessionL ( const TInt& aSettingsId,
		                    TXIMPRequestId aReqId );
		void OpenSessionL (
		    const MXIMPContextClientInfo& aContextClient,
		    const TInt& aSettingsId,
		    TXIMPRequestId aReqId );

		void CloseSession ( const MXIMPContextClientInfo& aClientCtxInfo,
		                    TXIMPRequestId aOpId );
		void LogoutRequestL(TXIMPRequestId aOpId);

		void ReOpenSessionsL ( TXIMPRequestId aReqId );

		void GetSupportedFeaturesL ( CDesC8Array& aFeatures ) const;

		MProtocolPresenceFeatures& ProtocolPresenceFeatures();
		
		//MProtocolImFeatures* ProtocolImFeatures();
		TAny* GetProtocolInterface(TInt aInterfaceId);


	private:    // data members
		/**
		    * OWN:
		    */
		COSSProtocolConnectionManager* iConnMan;
	
		/**
		 * OWN:
		 */
		COSSProtocolImFeatures* iImFeatures;
		/**
		 * OWN:
		 */
		TXIMPRequestId iRequestId;
    
    
    	TInt iConntectionCreated;
    	
    	MProtocolPresenceFeatures* iPresenceFeature;


	};


#endif // __COSSPROTOCOLCONNECTION_H__
