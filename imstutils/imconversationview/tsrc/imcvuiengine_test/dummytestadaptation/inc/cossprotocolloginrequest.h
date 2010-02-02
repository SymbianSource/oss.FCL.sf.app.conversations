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



#ifndef _COSSPROTOCOLLOGINREQUEST_H_
#define _COSSPROTOCOLLOGINREQUEST_H_

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <ximpbase.h>

#include "ossprotocloliterals.h"
// FORWARD DECLARATIONS

class MOSSProtocolConnectionManager;

class COSSProtocolConnectionManager;


const TInt KImpsMaxResourceIdLen 	= 64;  // maximum Resource ID, this is not sure!

/**
 * COSSProtocolLoginRequest
 *
 * Issue Login Request to the Network Server
 *
 * @lib ?library
 * @since S60 v4.0
 */
NONSHARABLE_CLASS (  COSSProtocolLoginRequest ) : public CActive


	{

public:

	static COSSProtocolLoginRequest* NewLC ( MOSSProtocolConnectionManager& aConnMan,
	TXIMPRequestId aRequestId );
	static COSSProtocolLoginRequest* NewL ( MOSSProtocolConnectionManager& aConnMan,
	                                        TXIMPRequestId aRequestId );
	virtual ~COSSProtocolLoginRequest();


private:

	COSSProtocolLoginRequest ( MOSSProtocolConnectionManager& aConnMan,
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

private:  // new methods

	   /**
		* Resource Id  Generator
		* Leaves with KErrNotFound if no value
		* @return ResourceId 
		*/
	TPtrC GenResourceId( const TDesC& aResourceId );
public:

	/**
	 * Issue login request to the isolation server
	 */
	void IssueLoginRequestL();


private: // data

	/**
	* OWN:Request Id from PrFw
	*/
	TXIMPRequestId iRequestId;
	/**
	 * Send data request to pure data handler generates the id
	 */
	TInt iSendId;
	/* OSS Protocol Plugin Connection Manager
	 * Not own.
	 */
	MOSSProtocolConnectionManager& iConnMan;

	
	TBuf<KImpsMaxResourceIdLen>          iResourceId;  //  Resource ID Implementation
	};


#endif // _COSSPROTOCOLLOGINREQUEST_H_

