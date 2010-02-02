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
* Description:  message container class
*
*/


#ifndef __CIMCVENGINEMESSAGECONTAINER_H__
#define __CIMCVENGINEMESSAGECONTAINER_H__

#include "mimcvenginemessagesreadinterface.h"
#include "mimcvenginemessageswriteinterface.h"
#include "mimcvenginemessagecontainerinfo.h"
#include "mimcvenginemessageobserver.h"

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineBufferMemoryHandler;
class MIMCVEngineNewMsgObserver;
class MCAMessage;

// CLASS DECLARATION

/**
 *  Container class for messages.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */


class CIMCVEngineMessageContainer : public CBase, 
                            public MIMCVEngineMessageReadInterface, 
                            public MIMCVEngineMessageWriteInterface,
                            public MIMCVEngineMessageContainerInfo
    {
    public: // Construction
		
		/**
		 * Construction
		 * @param aServiceId - ServiceId
		 * @param aTargetId, Identifier of container
		 * @param aMemoryHandler Handles memory
		 * @param aType Type of container. EChatContainer
		 * @return Pointer to new created instance of CIMCVEngineMessageContainer
		 */
		static CIMCVEngineMessageContainer* NewL( 
                                      TInt aServiceId, 
                                      const TDesC& aTargetId,
                                      const TDesC& aDisplayName,  
                                      const TDesC8& aContactLink,
	                                  MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
	                                  TContainerType aType );
		
		/**
		 * Construction
		 * @param aServiceId - ServiceId
		 * @param aTargetId, Identifier of container
		 * @param aMemoryHandler Handles memory
		 * @param aType Type of container. EChatContainer
		 * @return Pointer to new created instance of CIMCVEngineMessageContainer
		 */
		static CIMCVEngineMessageContainer* NewLC( 
                                       TInt aServiceId, 
                                       const TDesC& aTargetId, 
                                       const TDesC& aDisplayName, 
                                       const TDesC8& aContactLink,
	                                   MIMCVEngineBufferMemoryHandler& aMemoryHandler, 
	                                   TContainerType aType  );

		/**
		 * Destruction
		 */
		~CIMCVEngineMessageContainer();
		
	private: // Construction
	
		/**
		 * Constructor
		 * @param aMemoryHandler Handles memory
		 */
		CIMCVEngineMessageContainer( MIMCVEngineBufferMemoryHandler& aMemoryHandler );
		
		/**
		 * Constructor
		 * @param aServiceId - ServiceId
		 * @param aTargetId, Identifier of container
		 * @param aType Type of container. EChatContainer
		 */
		void ConstructL( 
                         TInt aServiceId, 
                         const TDesC& aTargetId,
                         const TDesC& aDisplayName,  
                         const TDesC8& aContactLink,
		                 TContainerType aType );

	private: // Methods from MIMCVEngineMessageReadInterface

		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		TInt UnreadCount( TUnreadFilter aUnreadFilter = EUnreadAll ) const;
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		TInt MessageCount() const;
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		void StoreEditerMessageL( const TDesC& aEditorMessage );
		      
		/**
	     * @see MIMCVEngineMessageReadInterface
		  */
		 const TDesC& GetEditerMessage();
		                
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		void Read( TReadType aReadType, TInt aReadAmount = 0 );
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		MIMCVEngineMessage& Message( TInt aIndex ) const;
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		MIMCVEngineMessage& ReadNextUnread();

        /**
		 * @see MIMCVEngineMessageReadInterface
		 */
        MIMCVEngineMessage& ReadUnreadFromIndex( TInt aIndex );
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */
		void SetObserver( MIMCVEngineMessageObserver* aObserver );
		
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */		
		TPtrC Name() const;
		
		/**
         * @see MIMCVEngineMessageReadInterface
         */ 
		TDesC DisplayName() const;

	
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */		
        void DeleteMessage( TInt aIndex );       
   
        
		/**
		 * @see MIMCVEngineMessageReadInterface
		 */		
        MIMCVEngineMessageContainerInfo& TargetId();
        
        /**
         * @see MIMCVEngineMessageReadInterface         
         */
        void Lock( TBool aLocked );   
        
        /**
         * @see MIMCVEngineMessageReadInterface         
         */
        void SetActive( TBool aActiveChat );   
        
        /**
         * @see MIMCVEngineMessageReadInterface         
         */
        TBool IsActive();  
        

        
	private: // Methods from MIMCVEngineMessageWriteInterface
	
		/**
		 * @see MIMCVEngineMessageWriteInterface
		 */
		void AppendL( MIMCVEngineMessage* aMessage, TBool aSharedOwnership );
		 
	


		/**
		 * @see MIMCVEngineMessageWriteInterface
		 */
		TTime Time();
		
		/**
         * sets the display name
         * @param aDispalyName. updated display name
         */
		void SetNameL( const TDesC& aDispalyName );
	public:       
	    /**
	     * sets the contactlink
	     * @param aContactLink. updated contactlink
	     */
	    void  SetLinkL(const TDesC8& aContactLink);

	    /**
	     * @see MIMCVEngineMessageReadInterface
	     */     
	    TPtrC8 ContactLink() const;

	private: // From MIMCVEngineMessageContainerInfo

        
        /**
         * @see MIMCVEngineMessageContainerInfo
         */
        const TDesC& Target() const;
        
        /**
         * Returns target id to be used for
         * comparisons.         
         * @return Target id.
         */
        const TDesC& ComparableTarget() const;
        
        
        
    
	public: // New methods
			
		/**
		 * Ordering function for ordering Message containers
		 * @param aSourceA First compare item.
		 * @param aSourceB Second compare item.
		 * @return -1, 0 or +1, like CompareC.
		 */
		static TInt OrderUid( const CIMCVEngineMessageContainer& aSourceA, 
		                      const CIMCVEngineMessageContainer& aSourceB );
		                      
		/**
		 * Type of container
		 * @return Type of container. EChatContainer
		 */
		TContainerType ContainerType() const;
		
		
		/**
		 * Set chat observer.
		 * @param aObserver. Observer to set.
		 */
		void SetChatObserver( MIMCVEngineNewMsgObserver* aObserver );
		
    private: // New helper methods
        
        /**
         * Resolve type of container by identifier.
		 * @param aType, new type. If ENoneContainer, Resolved automaticly.         
         */
        void ResolveType( TContainerType aType );   
        
        /**
         * display id it remove the domain part if any 
         * @param aId id to extarct 
         * @param aDomainHiding , hiding the domain part 
         */
     	static TPtrC DisplayId( const TDesC& aId, TBool aDomainHiding = ETrue );
     
        
        
    public:
    
        /**
         * Get count of all messages. Unread and read.
         * @return Count of messages
         */
        TInt AllMessagesCount() const;
            
        /** 
         * Access to any message. Also unread.
         * @param aIndex Index of message
         * @return Reference to message.
         */
        MIMCVEngineMessage& MessageFromAll( TInt aIndex ) const;
        
        /**
         * Delete from all messages. Unread and read messages available.
         * @param aIndex. Index of message.
         */
        void DeleteMessageFromAll( TInt aIndex );
        
        /**
         * Is container locked or not. ETrue locked, EFalse not.
         * @return Locked status.
         */
        TBool IsLocked() const;
        
        /**
         * Sizes of messages in container in bytes.         
         * @return Size in bytes.
         */
        TInt ContainerSizeInBytes() const;
        
    private: // Member data
    
    	/// conversation party id Owns    	    	
    	HBufC* iTargetId;
    	
    	// Owns , conversation party name    	    	
    	HBufC* iBuddyDisplayName;
    	
    	// Owns ,contact link info of chat.
    	HBufC8* iContactLink;

    	// owns 
        HBufC* iDecodedTargetId;
        // owns : reference to previous entered message
        HBufC* iEditerMessge;
        
    	/// Offset to last unread message
    	TInt iUnreadOffset;
    	
    	/// Array of messages.
        RPointerArray< MIMCVEngineMessage > iMessageContainer;
        
        /// Reference to memory handler
        MIMCVEngineBufferMemoryHandler& iMemoryHandler;
        
        /// Observer class - Doesnt Own
        MIMCVEngineMessageObserver* iObserver;
        
        /// Type of container
        TContainerType iContainerType;
        
        /// Not own. Chat observer 
        MIMCVEngineNewMsgObserver* iChatObserver;        
        
        // Locked status
        TBool iLocked;
    
        
        // Size of contained messages in bytes
        TInt iContainerSizeInBytes;
        
        //Active Chat
        TBool iActiveChat;
        
    };
    
    
#endif      // __CIMCVENGINEMESSAGECONTAINER_H__

// End of File
