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
* Description:  message handler interface
*
*/

#ifndef MIMCVENGINEMESSAGEHANDLER_H
#define MIMCVENGINEMESSAGEHANDLER_H

//  INCLUDES
#include <e32def.h>
#include "mimcvenginemessagehandlerobserver.h"
//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessage;
class MIMCVEngineMessageCreator;

// CLASS DECLARATION
			
/**
 *  Observer interface to handle message container changes.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageHandler
	{

	public: // Definitions
		
	    /**
        * @see message creator class
        */
	    virtual MIMCVEngineMessageCreator& MessageCreator() const = 0 ; 
       
	   /** SendMessageL 
		*  api to send the message to below layer
		* @param aMessage, message to be send
		*/
        virtual void SendMessageL(MIMCVEngineMessage& aMessage) = 0 ;
        
        /** append to cache 
		*  api to send the message to below layer
		* @param aBuddyId, aBuddyId to be saved
		* @param aTextMessage, message to be saved
		*/	
        virtual void AppendToCacheL(const TDesC& aBuddyId , const TDesC& aTextMessage ) = 0 ;
        
     	
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual void StartNewConversationL(const TDesC& aRecipientId )= 0;
		
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual TInt CloseConversationL(const TDesC& aBuddyId) = 0 ;
		
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual void CloseAllConversationL() = 0 ;
		
	
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual TInt DeactivateConversationL() = 0;

		virtual void UnSubscribePresenceL(const TDesC& aRecipientId) = 0;
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual void SubscribePresenceL(const TDesC& aWVID) = 0 ;
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual void SetObserver( MIMCVEngineMessageHandlerObserver* aObserver ) = 0;
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual void RemoveObserver() = 0;	
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		virtual TIMCVOnlineStatus GetPresenceStatus() = 0 ;
		
		virtual TInt ServiceId() const = 0  ;
        	
	public: // For protection.

		/**
		 * Destructor for protection
		 */
		virtual ~MIMCVEngineMessageHandler(){}
	};

#endif      // MIMCVENGINEMESSAGEHANDLER_H

// End of File
