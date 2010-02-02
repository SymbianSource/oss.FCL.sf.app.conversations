/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This interface is used to observer refresh timer lauch
*
*/


#ifndef __MVIMPSTSTORAGEREFRESHTIMEROBSERVER_H
#define __MVIMPSTSTORAGEREFRESHTIMEROBSERVER_H

#include <e32std.h>
#include "tvimpstenums.h"

class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;

class MVIMPSTStorageActiveObserver
	{
	public:

		/**
		* This call is made when the waiting completes
		* @param aError error code for waiting error, KErrNone if completes without error
		*
		*/
		virtual void HandleDelayedNotificationL(TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
											   MVIMPSTStorageContactList *aList ,
					                           MVIMPSTStorageContact* aContact,
					                           TInt aContactIndex) = 0;

		/**
		* Virtual destructor
		*/
		virtual ~MVIMPSTStorageActiveObserver() {};
	};

#endif // __MVIMPSTSTORAGEREFRESHTIMEROBSERVER_H
