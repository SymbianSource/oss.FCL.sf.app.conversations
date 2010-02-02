/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: enum value for imconnection provider
*
*/
#ifndef TIMCONNPROVIDERENUMS_H
#define TIMCONNPROVIDERENUMS_H

#include <e32base.h>
#include <bamdesca.h>

/**
* enums of request types
*/
class TIMConnProviderEnums
    {

	public:	// Enumerations and typedefs
	
		enum TRequestTypes
			{
			ELoginRequest = 0,
			ELogoutRequest,
			EFetchRequest,
			EFetchContent,
			EGrantListRequest,
			EPublishOwnPresence,
			EInvalid
			};
	   	    };

#endif      // TIMCONNPROVIDERENUMS_H

