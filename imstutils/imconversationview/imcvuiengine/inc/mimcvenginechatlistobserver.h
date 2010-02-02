/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer interface to handle chat container changes.
*
*/


#ifndef __MIMCVEngineChatListObserverOBSERVER_H__
#define __MIMCVEngineChatListObserverOBSERVER_H__

//  INCLUDES
#include <e32def.h>

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessageReadInterface;

// CLASS DECLARATION


enum TChatListEventType
    {
    EChatItemAdded,
    EChatItemDeleted,
    EChatAllItemDeleted,
    EEventNotSpecified
    };
/**
 *  Observer interface to handle messages container changes.
 *
 *  @lib imcvuiengine.dll
 *  @since 5.0
 */
class MIMCVEngineChatListObserver
	{
	
	public: // Interface

		/**
		 * Handle events.
		 * @param aEvent Event to handle.
		 * @param aContainerInfo, Container
		 */
		virtual void HandleChatListEvent( TInt aServiceId, TChatListEventType aEvent, 
								MIMCVEngineMessageReadInterface* aContainerInfo
								 ) = 0;

	protected: // For protection.

		/**
		 * Destructor
		 */
		virtual ~MIMCVEngineChatListObserver(){}
	};

#endif      // __MIMCVEngineChatListObserverOBSERVER_H__

// End of File
