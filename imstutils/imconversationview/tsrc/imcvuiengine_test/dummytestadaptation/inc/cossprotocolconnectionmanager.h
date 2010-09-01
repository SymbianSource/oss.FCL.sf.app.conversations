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

#ifndef __COSSPROTOCOLCONNECTIONMANAGER_H__
#define __COSSPROTOCOLCONNECTIONMANAGER_H__

//  INCLUDES
#include <e32base.h>
#include <flogger.h>
#include <ximpstatus.h>
#include "mossprotocolconnectionmanager.h"
#include "creceivemessagehandler.h"

#include <ximperrors.hrh>

// FORWARD DECLARATIONS

class MXIMPServiceInfo;

class MXIMPProtocolConnectionHost;

class MOSSProtocolConnectionManager;
class MProtocolImDataHost;

/**
 * COSSProtocolConnectionManager
 *
 * Every XIMPFW-OSS Connection has Connection Manager Object.
 *
 *
 *
 */
NONSHARABLE_CLASS ( COSSProtocolConnectionManager ) : public CBase,
		public MOSSProtocolConnectionManager
		// implement session status observer callback
	{

public:

	static COSSProtocolConnectionManager* NewL();
	virtual ~COSSProtocolConnectionManager();


private:

	COSSProtocolConnectionManager();
	void ConstructL();


public: // MOSSProtocolConnectionManager


	MXIMPProtocolConnectionHost& HandleToHost();

public: // new methods

	/**
	   * set protocol connection host
	   * @param aHost protocol connection host
	   */
	void SetHost ( MXIMPProtocolConnectionHost& aHost );
	
	void SetImHost(MProtocolImDataHost& aHost);
	
	MProtocolImDataHost& GetImHost() ;

private: // data


	MXIMPProtocolConnectionHost* iHostHandle;
	// not own
	MProtocolImDataHost *iHost ; 

	};


#endif // __COSSPROTOCOLCONNECTIONMANAGER_H__
