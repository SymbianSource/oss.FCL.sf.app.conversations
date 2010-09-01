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
* Description:  messge observer interface 
*
*/


#ifndef __MIMCVENGINEMESSAGEOBSERVER_H__
#define __MIMCVENGINEMESSAGEOBSERVER_H__

//  INCLUDES
#include <e32def.h>

//	FORWARD CLASS DECLERATIONS

// CLASS DECLARATION

/**
 *  Observer interface to handle message container changes.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageObserver
	{

	public: // Definitions

		/**
		 * Events for observer to handle.
		 */
		enum TMessageEventType
			{
			ENewMessage,
			EMessageChanged,
			EMessageDeleted,
			EChatDeleted
			};

	public: // Interface

		/**
		 * Handle events.
		 * @param aEvent Event to handle.
		 * @param aIndex, Index of message which is affected by event.
		 */
		virtual void HandleMessageEvent( TMessageEventType aEvent, TInt aIndex = -1 ) = 0;
		
		
	
	protected: // For protection.

		/**
		 * Destructor for protection
		 */
		virtual ~MIMCVEngineMessageObserver(){}
	};

#endif      // __MIMCVENGINEMESSAGEOBSERVER_H__

// End of File
