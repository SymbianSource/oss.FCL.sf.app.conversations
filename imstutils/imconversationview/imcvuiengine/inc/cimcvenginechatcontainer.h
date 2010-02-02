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
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/

#ifndef CIMCVENGINECHATCONTAINER_H
#define CIMCVENGINECHATCONTAINER_H

#include "mimcvenginebuffermemoryhandler.h"
#include "mimcvenginechatinterface.h"
#include "mimcvenginenewmsgobserver.h"
#include "mimcvenginechatlistobserver.h"


//	FORWARD CLASS DECLERATIONS
class CIMCVEngineMessageContainer;


// CLASS DECLARATION

/**
 *  Container for message containers
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class CIMCVEngineChatContainer : public CBase, 
                         public MIMCVEngineChatInterface,
                         public MIMCVEngineBufferMemoryHandler, 
                         public MIMCVEngineNewMsgObserver
	{
    public: // Construction
		
		/**
		 * Construction
		 */
		static CIMCVEngineChatContainer* NewL(TInt aServiceId);
		
		/**
		 * Destruction
		 */
		~CIMCVEngineChatContainer();
		
		
		
		
	private: // Construction
	
		/**
		 * Constructor
		 */
		CIMCVEngineChatContainer( TInt aServiceId );
		
	private: // Methods from MIMCVEngineChatInterface				
		
		
		/**
		 * @see MIMCVEngineChatInterface
		 */
		MIMCVEngineBufferMemoryHandler& MemoryHandler();
		
		
		/**
		 * @see MIMCVEngineChatInterface
		 */
		MIMCVEngineMessageReadInterface& MessageReadInterfaceL(
                        TInt aServiceId, 
                        const TDesC& aTargetId,
                        const TDesC& aDisplayName,
                        const TDesC8& aContactLink,
                        MIMCVEngineMessageReadInterface::TContainerType aType );

		/**
		 * @see MIMCVEngineChatInterface
		 */
		MIMCVEngineMessageWriteInterface& MessageWriteInterfaceL( 
                        TInt aServiceId, 
                        const TDesC& aTargetId,
                        const TDesC& aDisplayName,
                        const TDesC8& aContactLink,
                        MIMCVEngineMessageReadInterface::TContainerType aType );
		
		/**
		 * @see MIMCVEngineChatInterface
		 */
		void DeleteChatL( 
		                  TInt aServiceId,
		                  const TDesC& aTargetId );
		                  
		/**
		 * @see MIMCVEngineChatInterface
		 */
        void CloseAllContainers();
					
		
		
		/**
		 * @see MIMCVEngineChatInterface
		 */ 
		TInt ChatCount() const;
		
		
		/**
		 * @see MIMCVEngineChatInterface
		 */ 
		void RegisterNewMsgObserver( MIMCVEngineNewMsgObserver* aObserver );

		/**
		 * @see MIMCVEngineChatInterface
		 */ 
		void UnregisterNewMsgObserver();
		
		
		/**
		 * @see MIMCVEngineNewMsgObserver
		 */ 
		TInt RegisterChatListObserver( MIMCVEngineChatListObserver* aObserver );

		/**
		 * @see MIMCVEngineNewMsgObserver
		 */ 
		TInt UnregisterChatListObserver( MIMCVEngineChatListObserver* aObserver );
		
	
		/**
		 * @see MIMCVEngineNewMsgObserver
		 */ 
        TInt MessagesPendingCount( TInt& aCountOfChats,
            MIMCVEngineMessageReadInterface::TUnreadFilter aUnreadFilter =
            MIMCVEngineMessageReadInterface::EUnreadAll ) const;
       
        
			
	private: // Methods from MIMCVEngineBufferMemoryHandler

		/**
		 * @see MIMCVEngineBufferMemoryHandler
		 */
		 TBool FreeMemoryIfNeededL( TInt aSize );
		 
		
			
		/**
		 * return the chat interface at given index
		 */ 
	 	MIMCVEngineMessageReadInterface& ChatAt( TInt aIndex ) const;
	 	
    private: // Methods from MIMCVEngineNewMsgObserver
    
        /**
         * @see MIMCVEngineNewMsgObserver
         */
        void HandleChatEventL( TChatEventType aEvent, 
                              MIMCVEngineMessage* aMessage = NULL );
        		 
    
		
	private: // New helper methods
		
		/**
		 * Return index to chat. Try to create if does not exists.
		 * Leave if not possible. e.g. KErrNoMemory.
		 * @param aServiceId Part of identifier to identify container.
		 * @param aTargetId Part of identifier to identify container.		 
		 * @return Index of container.
		 */
		TInt ChatL( TInt aServiceId, 
                    const TDesC& aTargetId,
                    const TDesC& aDisplayName,
                    const TDesC8& aContactLink,
                    MIMCVEngineMessageReadInterface::TContainerType aType = 
                        MIMCVEngineMessageReadInterface::ENoneContainer );
		            
		/**
		 * Returns latest found item by index. 
		 * FindChat or ChatL( id, type ) must be called before this one.
		 * @param aIndex, Index of needed item.
		 * @param aResetLatest. Reset type of latest found container. 
		 *                      ETrue, Reset, EFalse, No reseting.
		 * @return Pointer to right container.
		 */ 
        CIMCVEngineMessageContainer* ChatL( TInt aIndex, TBool aResetLatest = ETrue );
		
		/**
		 * FindChat or ChatL( id, type ) must be called before this one.
		 * @param aIndex, Index of needed item.
		 * @param aResetLatest. Reset type of latest found container. 
		 *                      ETrue, Reset, EFalse, No reset.
		 */
		void RemoveChat( TInt aIndex, TBool aResetLatest = ETrue );

		/**
		 * Finds if container exists and returns index.
		 * @param aChat Reference chat.
		 * @return Index of container. If container is not found, 
		 *                             KErrNotFound is returned.
		 */
		TInt FindChat( const CIMCVEngineMessageContainer& aChat );
		
		/**
		 * Create new container. userid  
		 * current ones, if KNullDesC used.
		 * @see CIMCVEngineMessageContainer
		 */
		 CIMCVEngineMessageContainer* CreateContainerLC(                               
                              TInt aServiceId, 
                              const TDesC& aTargetId,
                              const TDesC& aDisplayName, 
                              const TDesC8& aContactLink,
                              MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
                              MIMCVEngineMessageReadInterface::TContainerType aType );

	private:
	    /**
		 * helper method
		 */                           
		 void HandleChatListEvent( TChatListEventType aEvent, 
                                   MIMCVEngineMessageReadInterface& aContainerInfo );
		
		                      
	private: // Member data
	    // service Id
	    TInt iServiceId;
		/// Container of chats. Owns items.
		RPointerArray< CIMCVEngineMessageContainer > iChatContainer;

		// Container of send buffers. Owns items.
		RPointerArray< CIMCVEngineMessageContainer > iSendContainer;
		
		/// Container of observers. Does not own items.
		MIMCVEngineNewMsgObserver* iNewMsgObserver;
		
		/// External memory handler if set.
		MIMCVEngineBufferMemoryHandler* iExternalMemoryHandler;
		
			/// Last found item type
	    MIMCVEngineMessageReadInterface::TContainerType iLatestType;
	    
	    /// Container of observers. Does not own items.
		RPointerArray< MIMCVEngineChatListObserver > iChatListObservers;
	    
    	};


#endif      // CIMCVENGINECHATCONTAINER_H

// End of File
