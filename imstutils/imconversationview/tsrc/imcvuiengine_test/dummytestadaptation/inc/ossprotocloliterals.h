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
* Description:  OSS adapter plugin 
*
*/

#ifndef __OSSPROTOCOLLITERALS_H__
#define __OSSPROTOCOLLITERALS_H__

#include <e32base.h>

#define CONNMGR_BUS 	"org.freedesktop.Telepathy.ConnectionManager.gabble"
#define CONNMGR_PATH 	"/org/freedesktop/Telepathy/ConnectionManager/gabble"
#define PROTOCOL 		"jabber"
#define TP_IFACE_CONN_MGR_INTERFACE \
        				"org.freedesktop.Telepathy.ConnectionManager"
#define TestId 			"mytest" 
#define ServerStr 		"server"
#define ResourceStr 	"resource"
#define AccountStr 		"account"
#define PasswdStr 		"password"
#define PortStr 		"port"
#define OldsslStr 		"old-ssl"
#define PORT 443

_LIT(KOSSProtocolConnection, "COSSProtocolConnection");
_LIT8(KConnectionFeature, "telepathy/connection ");
_LIT( KZeroId, "0" );
_LIT(KSlash,"/");
_LIT(KEnd,"\0");

enum TRegRespState
	{
	ERequestDelivered = 0,
	EResponseRecieved	
	};


#endif  //__OSSPROTOCOLLITERALS_H__

//  End of File

