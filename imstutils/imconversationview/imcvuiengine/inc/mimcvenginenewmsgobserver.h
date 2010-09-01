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
* Description:  chat observer interface
*
*/


#ifndef __MIMCVENGINECHATOBSERVER_H__
#define __MIMCVENGINECHATOBSERVER_H__

//  INCLUDES
#include <e32def.h>

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessage;

// CLASS DECLARATION

/**
 *  Observer interface to handle messages container changes.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineNewMsgObserver
	{

	public: // Definitions

		/**
		 * Events for observer to handle.
		 */
		enum TChatEventType
			{			
			EUnreadCountChanged, // Happens with new message and when messages are read.
			EMemoryLow,
            EEventNotSpecified
			};

	public: // Interface

		/**
		 * Handle events.
		 * @param aEvent Event to handle.
		 * @param aMessage, Event occurred because of message
		 */
		virtual void HandleChatEventL( TChatEventType aEvent, MIMCVEngineMessage* aMessage = NULL ) = 0;

	protected: // For protection.

		/**
		 * Destructor
		 */
		virtual ~MIMCVEngineNewMsgObserver(){}
	};

#endif      // __MIMCVENGINECHATOBSERVER_H__

// End of File
