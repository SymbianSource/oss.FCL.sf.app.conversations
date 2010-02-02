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
* Description:  message read interface
*
*/

#ifndef __MIMCVENGINEMESSAGESREADINTERFACE_H__
#define __MIMCVENGINEMESSAGESREADINTERFACE_H__

#include <e32base.h>

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessage;
class MIMCVEngineMessageObserver;
class MIMCVEngineMessageContainerInfo;

// CLASS DECLARATION

/**
 *  Interface for getting access to messages.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageReadInterface
	{

	public: // Definitions
		
		/**
		 * How many messages we want to change from unread to read.
		 */
		enum TReadType
			{
			EReadAll,
			EReadAmount,
			EReadOne,
			EReadInvalid
			};	
	
        /**
         * Type of container
         */
        enum TContainerType
            {
            ENoneContainer,            
            EChatContainer,
            ESendContainer
            };
            
         /**
          *
          */
         enum TUnreadFilter
            {
            EUnreadAll,
            EUnreadReceived
            };	
	
	public: // Interface
	
		/**
		 * Get count of unread messages.
		 * @param aUnreadFilter Show only received messages as unread or all.
		 * @return Amount of unread messages.
		 */
		virtual TInt UnreadCount( TUnreadFilter aUnreadFilter = EUnreadAll ) const = 0;
		
		/**
		 * Get count of readable messages. 
		 * @return Amount of readable messages. Does not include unread messages.
		 */		 
		virtual TInt MessageCount() const = 0;
		
		/**
		 * Get stores the previously typed message before switching the tab 
		 * 
		 */      
		virtual void StoreEditerMessageL(const TDesC& aEditorMessage ) = 0;
		        
		/**
		 * Gets the previously stored message
		 * @return the message
		  */   
		virtual const TDesC& GetEditerMessage() = 0;
		                        
		
		/**
		 * Move unread messages to read.
		 * @param aReadType Type of read. EReadAll, EReadAmount, EReadOne
		 * @param aReadAmount Amount of messages to move from unread to read. 
		 * 		  Default 0, Needed if TReadAmount is used.
		 */
		virtual void Read( TReadType aReadType, TInt aReadAmount = 0 ) = 0;
		
		/**
		 * Get message.
		 * @param aIndex, Index of message in messagearray.
		 * @return Reference to message.
		 */
		virtual MIMCVEngineMessage& Message( TInt aIndex ) const = 0;
		
		/**
		 * Get next unread message
		 * @return Reference to next unread message.
		 */
		virtual MIMCVEngineMessage& ReadNextUnread() = 0;
		
		/**
		 * Read one unread message from index aIndex.
		 * @since S60 v3.2
		 * @param aIndex Index to wanted message.
		 * @return Reference to last unread message.
		 */
		virtual MIMCVEngineMessage& ReadUnreadFromIndex( TInt aIndex ) = 0;
		
		/**
		 * Set observer for observing changes in messages container.
		 * Observer can handle different situations like e.g.
		 * containers destruction, messages arrival etc.
		 * @param aObserver Observer class to get notifications.
		 * 		  Uninstall observer with passing NULL as parameter.
		 */
		virtual void SetObserver( MIMCVEngineMessageObserver* aObserver ) = 0;
		
		/**
		 * Get name of chat.
		 * @return Name of chat.
		 */
		virtual TPtrC Name() const = 0;
		
		/**
		 * Delete message in index.
		 * @param aIndex, Index of message to be deleted.
		 * @param aFreeMemory, ETrue if message should be deleted. EFalse if ownership of message
		 * is transferred to caller. Remember to achieve 
		 * @return Pointer to message if not ownership is tranferred to caller. NULL if not.
		 */
		virtual void DeleteMessage( TInt aIndex ) = 0;
        /**
         * Id information of message container.
         * @return Container information.
         */
        virtual MIMCVEngineMessageContainerInfo& TargetId() = 0;
        
        /**
         * Lock/unlock messages from deletion
         * @param aLocked, ETrue locks, EFalse unlocks.
         */
        virtual void Lock( TBool aLocked ) = 0;
        
        /**
         * Sets whether the Conversation is Currently Active in UI
         * @param aActiveChat, ETrue ActiveConversation, EFalse InActiveConversation.
         */
        virtual void SetActive( TBool aActiveChat ) = 0;
        
         /**
         * Gets whether the Conversation is Currently Active in UI
         * @return ETrue ActiveConversation in UI, EFalse InActiveConversation.
         */
        virtual TBool IsActive() = 0;
		
        /*
         * return contact link.
         */
        virtual TPtrC8 ContactLink() const = 0;
        
        /**
         * returns displayname
         */ 
        virtual TDesC DisplayName() const = 0;
    
    protected: // For protection
		/**
		 * Destructor for protection
		 */
		virtual ~MIMCVEngineMessageReadInterface(){}
	};

#endif      // __MIMCVENGINEMESSAGESREADINTERFACE_H__

// End of File
