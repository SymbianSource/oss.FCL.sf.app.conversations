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
* Description:  chat interface 
*
*/


#ifndef MIMCVENGINECHATINTERFACE_H
#define MIMCVENGINECHATINTERFACE_H

#include "mimcvenginemessagesreadinterface.h"

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessageWriteInterface;
class MIMCVEngineNewMsgObserver;
class MIMCVEngineMessageReadInterface;
class MIMCVEngineBufferMemoryHandler;
class MIMCVEngineChatListObserver;


/**
 * Internal buffer ids, these must be specified so that they can't appear in
 * wireless village ids. And they must also be non-matchable to KNullDesC when
 * comparing with neutral compare.
 *
 * From WV_CSP_v1.1.pdf:
 *   As per URI [RFC2396], certain reserved characters must be escaped if they 
 *   occur within the User-ID, Resource,. This includes the characters 
 *   ";", "?", ":", "&", "=", "+", "$" and ",".
 *
 * When using neutral compare, atleast ";", "?", ":", "&", "=", "+", "#" and "," 
 * are invalid.
 */
_LIT( KSendBufferId, "$" );

// CLASS DECLARATION

/**
 *  Interface for getting access to message containers.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineChatInterface
	{	    	
	public: // Interface
	
		/**
		 * Provide access to chat messages. If messageContainer does not exists
		 * creates one. 
		 * @param aServiceId Part of identifier to identify container.
		 * @param aTargetId Part of identifier to identify container.		 
		 * @param aType. Preferred type of container. If ENoneContainer, 
		 *               resolved by container.
		 * @return Read interface to container
		 */
		virtual MIMCVEngineMessageReadInterface& MessageReadInterfaceL(
            TInt aServiceId, 
            const TDesC& aTargetId = KSendBufferId,
            const TDesC& aDisplayName = KNullDesC ,
            const TDesC8& aContactLink = KNullDesC8,
            MIMCVEngineMessageReadInterface::TContainerType aType = MIMCVEngineMessageReadInterface::ENoneContainer
            ) = 0;

		/**
		 * Provide access to append chat messages. 
		 * If messageContainer does not exists, creates one. 
		 * @param aServiceId Part of identifier to identify container.
		 * @param aTargetId Part of identifier to identify container.		 
		 * @param aType. Preferred type of container. If ENoneContainer, 
		 *               resolved by container.		 
		 * @return Write interface to container
		 */
		virtual MIMCVEngineMessageWriteInterface& MessageWriteInterfaceL(
            TInt aServiceId, 
            const TDesC& aTargetId = KSendBufferId,
            const TDesC& aDisplayName = KNullDesC ,
            const TDesC8& aContactLink = KNullDesC8 ,
            MIMCVEngineMessageReadInterface::TContainerType aType = 
                        MIMCVEngineMessageReadInterface::ENoneContainer ) = 0;
		
                    
        /**
         * Delete identified chat container		 
		 * @param aServiceId Part of identifier to identify container.
		 * @param aTargetId Part of identifier to identify container.		 
         */		
		virtual void DeleteChatL( TInt aServiceId,
		                          const TDesC& aTargetId = KSendBufferId ) = 0;
		                          

			
		/**
		 * @see MIMCVEngineNewMsgObserver
		 */ 
		virtual TInt ChatCount() const = 0 ;
		
        /**
         * Close all open containers.
         */
        virtual void CloseAllContainers() = 0;
        
        
        /**
         * Returns the Memory Handler
         * @return Reference to MIMCVEngineBufferMemoryHandler
         */
        virtual MIMCVEngineBufferMemoryHandler& MemoryHandler() = 0;
        
        /**
		 * Register observer for chat events. Remember to unregister 
		 * when not needed
		 * @param aObserver Observer which is registered.
		 * @return KErrNone if succeed.
		 */
		virtual void RegisterNewMsgObserver( MIMCVEngineNewMsgObserver* aObserver ) = 0;

		/**
		 * Unregister observer for chat events.
		 * @param aObserver Observer which is unregistered
		 * @return KErrNone if found and unregistered.
		 */
		virtual void UnregisterNewMsgObserver() = 0;

		
		/**
		 * Register observer for conversations added/deleted events. Remember to unregister 
		 * when not needed
		 * @param aObserver Observer which is registered.
		 * @return KErrNone if succeed.
		 */
		virtual TInt RegisterChatListObserver( MIMCVEngineChatListObserver* aObserver ) = 0;

		/**
		 * Unregister observer for conversations added/deleted.
		 * @param aObserver Observer which is unregistered
		 * @return KErrNone if found and unregistered.
		 */
		virtual TInt UnregisterChatListObserver( MIMCVEngineChatListObserver* aObserver ) = 0;
			                     
        	/**
		 * @see MIMCVEngineChatInterface
		 */ 
		virtual ~MIMCVEngineChatInterface()
			{
				
			};

	};

#endif      // MIMCVENGINECHATINTERFACE_H

// End of File
