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



#ifndef __COSSPROTOCOLLOGOUTREQUEST_H_
#define __COSSPROTOCOLLOGOUTREQUEST_H_

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <ximpbase.h>
#include "ossprotocloliterals.h"
// FORWARD DECLARATIONS

class MOSSProtocolConnectionManager;

class COSSProtocolConnectionManager;


/**
 * COSSProtocolLogoutRequest
 *
 * Issue Login Request to the Network Server
 *
 * @lib ?library
 * @since S60 v4.0
 */
NONSHARABLE_CLASS (  COSSProtocolLogoutRequest ) : public CActive


	{

public:

	static COSSProtocolLogoutRequest* NewLC ( MOSSProtocolConnectionManager& aConnMan,
	TXIMPRequestId aRequestId );
	static COSSProtocolLogoutRequest* NewL ( MOSSProtocolConnectionManager& aConnMan,
	        TXIMPRequestId aRequestId );
	virtual ~COSSProtocolLogoutRequest();


private:

	COSSProtocolLogoutRequest ( MOSSProtocolConnectionManager& aConnMan,
	                            TXIMPRequestId aRequestId  );
	void ConstructL();


private: // from CActive

	void DoCancel();
	/*
	 * RunL() function also deletes the request object once the activity is done
	 *
	 */
	void RunL();
	TInt RunError ( TInt aError );

public:
	/**
	 * Issue logout request to the isolation server
	 */
	void IssueLogoutRequestL();


private: // data

	/**
	 * OWN: Request Id
	 */

	TXIMPRequestId iRequestId;
	/**
	 * OSS Protocol Plugin Connection Manager
	 * Not own.
	 */
	MOSSProtocolConnectionManager& iConnMan;

	/**
	 * Send data request to pure data handler generates the id
	 */
	TInt iSendId;

	};


#endif // __COSSPROTOCOLLOGOUTREQUEST_H_

