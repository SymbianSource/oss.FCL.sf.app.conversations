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
* Description:  message handler class declaration
*
*/


#ifndef __CIMCVENGINEMESSAGEHANDLER_H__
#define __CIMCVENGINEMESSAGEHANDLER_H__

#include 	"mimcvenginemessage.h"
#include    "mimcvenginemessagecreator.h"           // For SMessageData
#include 	"mimcvenginemessagehandler.h"
#include 	"mimcvenginenewmessagehandler.h"

#include    <mimcacheeventhandler.h> // im cache 
#include    <cimcachefactory.h>
#include 	<imcachedefs.h>

#include 	<ximpcontext.h>
#include    <presencecachereadhandler2.h> // handling the presence cache events 
#include    <mpresencebuddyinfo2.h> // buddy info class
#include    "cimcvenginestoragelistener.h"

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineChatInterface;
class MIMCVEngineMessageUtils;
class MIMCVEngineMessageReadInterface;
class MIMCVEngineMessageHandler;
class MIMCVEngineNewMessageHandler;
class MIMCVEngineMessageHandlerObserver;

// ximp im
class MImFeatures;
class MPresenceCacheReader2;
class MXIMPIdentity;
// im cache
class MIMCacheUpdater;
class CIMCacheFactory;

class CIMCVEngineOpenChats;
class CIMCVEngineStorageListener;
class MIMCVEngineStorageObserver;
// CLASS DECLARATION

/**
 *  Handler class for incoming and outgoing messages
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */

class CIMCVEngineMessageHandler : 	public CBase, 
									public MIMCVEngineMessageHandler,
									public MIMCVEngineServiceSessionHandler,
								    public MIMCacheEventHandler,
								    public MPresenceCacheReadHandler2,
								    public MIMCVEngineStorageObserver
    {
    friend class CIMCVUiEngine_TestSendMessage;
    
    public: // Construction

        /**
         * Construction
         * @param aMessageUtils Access to message utils ( Message creator, 
		 * @param aContext, Access to Context
		 * @param aServiceId , Service Id
		 * @param aOwnId - OwnUserId
         * @return Pointer to new created instance of CIMCVEngineMessageCreator
         */
        static CIMCVEngineMessageHandler* NewL( MIMCVEngineChatInterface& aChatInterface, 
                                                MIMCVEngineMessageCreator& aMessageCreator,
                                             	MXIMPContext* aContext, 
	                							TInt aServiceId ,
	                							const TDesC& aOwnId);

        /**
         * two phase Construction
         * @param aMessageUtils Access to message utils ( Message creator, 
		 * @param aContext, Access to Context
		 * @param aServiceId , Service Id
		 * @param aOwnId - OwnUserId
         * @return Pointer to new created instance of CIMCVEngineMessageCreator
         */
        static CIMCVEngineMessageHandler* NewLC( MIMCVEngineChatInterface& aChatInterface, 
                                                MIMCVEngineMessageCreator& aMessageCreator,
                                             	MXIMPContext* aContext,
												TInt aServiceId ,
												const TDesC& aOwnId);

        /**
         * Destruction
         */
        ~CIMCVEngineMessageHandler();
        
        
        /**
         * ResetContextL
         * @param aContext, new context 
         */
        void ResetContextL(MXIMPContext* aContext);
        

    private: // Construction

        /**
         * Constructor
         * @see CIMCVEngineMessageHandler::NewL
         * @param aChatInterface, reference to chat interface
         * @param aMessageUtils, reference to the message utils
         * @param aContext, reference to ximpfw context 
         */
        CIMCVEngineMessageHandler( MIMCVEngineChatInterface& aChatInterface,
                                    MIMCVEngineMessageCreator& aMessageCreator,
                                    MXIMPContext* aContext ,
					                TInt aServiceId);

        /**
         * Constructor
         */
        void ConstructL(const TDesC& aOwnId );
		
		/**
         * read service name from csp settings
         * @param aServiceId, service id.
        */
		void ReadServiceNameL(TInt aServiceId );
		
			
		/**
         * handle the failed messages
         * @param aMessage, message 
         */
		void HandleMessageSentFailedL( MIMCVEngineMessage& aMessage ) ;
		
		
    public: // From MIMCVEngineServiceSessionHandler

               
        /**
		* @see HandleDeliveryReportL
		* to handle the new message delivery report
		* @param aResultError, delivery report
		* @param aText, msg to send
		*/
		void HandleDeliveryReportL( TXIMPRequestId aOperatioCode,
									 TInt aResultError )  ;
       
        
		
		/**
		 * @see HandleConnectionTerminatedL
		 * to handle the connection lost
		 * @param aReason, reason report
		 */
		void HandleConnectionEventL( TIMCVConnectionState aState, TInt aReason ) ;
		
   public :	//MIMCVEngineMessageHandler
   		
        MIMCVEngineMessageCreator& MessageCreator() const ;
       
	   /** SendMessageL 
		*  api to send the message to below layer
		* @param aMessage, message to be send
		*/	
        void SendMessageL(MIMCVEngineMessage& message);
        
        /** append to cache 
		*  api to send the message to below layer
		* @param aBuddyId, a buddyId 
		* @param aTextMessage, message to be saved
		*/	
        void AppendToCacheL(const TDesC& aBuddyId, const TDesC& aTextMessage );
        
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		void StartNewConversationL(const TDesC& aRecipientId );
		
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		TInt CloseConversationL(const TDesC& aBuddyId);
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		void CloseAllConversationL();
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		TInt DeactivateConversationL();	


		void UnSubscribePresenceL(const TDesC& aRecipientId);
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		void SubscribePresenceL(const TDesC& aWVID);
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		void SetObserver( MIMCVEngineMessageHandlerObserver* aObserver ) ;
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		void RemoveObserver();
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		TIMCVOnlineStatus GetPresenceStatus();
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		RArray<SIMCacheChatItem> GetChatListL( TInt aServiceId );		
		
		/**
		 * @see MIMCVEngineMessageHandler
		 */
		CIMCVEngineOpenChats& OpenChats() ;
		
		TInt ServiceId() const ;
		
		
	public:     // MIMCacheMessageChangeObserver
    
    	/**
    	* Gets called when data update is available.
    	* Client can then decide wether to use the old one
    	* or the updated one.
    	* @param aNewMessage, new message details
       	*/
        void HandleIMCacheEventL(TIMCacheEventType aEvent, TAny* aChatMessage = NULL ) ;
        
   public: // prescachereadhandler
	
	     /**
          * Handler for Asynchronous methods of MPresenceCacheReader.
          * @param aErrorCode 
          * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
          *         Ownership to callee. Can be null if not found.
          */    
		void HandlePresenceReadL(TInt aErrorCode,
		         RPointerArray<MPresenceBuddyInfo2>& aPresenceBuddyInfoList);
		
		/**
         * Handler for Buddy presence change notifications.
         * @param aErrorCode 
         * @param aPresenceBuddyInfo presence info for buddy. 
         *         Ownership to callee. Can be NULL if aErrorCode exist.
         */  
		void HandlePresenceNotificationL(TInt aErrorCode,
		        MPresenceBuddyInfo2* aPresenceBuddyInfo);  
  
  
      

                            	
    private :
    
    	/** NotifyOperationCompletdL 
	    * @param aResult, aResult to be send
		*/	
    	void NotifyOperationCompletdL(MIMCVEngineMessage::TMessagerType aType , TInt aResult  );        
    	
    	/** Helper Functions
	    * 
		*/
    	MIMCVEngineMessage::TMessageType  GetEngineMessageType(TIMCacheMessageType aType );
    	MIMCVEngineMessage::TMessagerType GetEngineMessagerType(TIMCacheMessagerType aType );
    	MIMCVEngineMessage::TContentType  GetEngineContentType(TIMCacheContentType aType ); 
    	
    	/**
         * ReadPresenceStatusL,
         * read the presence status and text from presence cache
         * @param aIdentity ,   identity  in the XSP format
         **/
    	void ReadPresenceStatusL(const TDesC& aIdentity );
    	/**
         * ConvertStringToStatus,converts the string to enums for presence status
         * aAvailabilityEnum enum value indicating the presence status.
         * aAvailabilityText, availability text to support multiple presence.
         * @return staus.
         **/
    	TIMCVOnlineStatus ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues  aAvailabilityEnum, TPtrC aAvailabilityText); 
       	
    	 /**
          * Converts the  presence cache extension keys 
          * user for block and pending states  to client supported presence status.
          * 
          * @since 5.0
          * @param aPresenceBuddyInfo presence buddy info class.
          */
        void GetKeyFiledsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo);

		/*
		* call back after fetching the display name and link
		*/
         void HandleServiceBuddyDetailsFetchedL(TInt aServiceId, 
                        const TDesC& aXspId,
                        const TDesC8 & aContactLink,
                        const TDesC & aDisplayName )  ;
    	
    private: // Member data
    
        // Owns , reference to message creator
        MIMCVEngineMessageCreator&  iMessageCreator;
   	
    	/// Does not own. Access to message containers
    	MIMCVEngineChatInterface& iChatInterface;
    	   
        //  owns : storage change listener
        CIMCVEngineStorageListener* iStorageListener;
    
    	// not woned
    	MXIMPContext* iContext;	
    	
       	// woned ,delet it 
        MImFeatures* iImFeatures;
        
        /// Messages which are send to the server
        // removed once delivered successfully to server
    	RPointerArray< MIMCVEngineMessage > iHoldingMessages;
    	
    	// not owned , send observers
    	MIMCVEngineMessageHandlerObserver* iObserver;
    	
    	// owned, access to accesss/update interface  
    	// call realease of factory
    	CIMCacheFactory* iIMCacheFactory;
    	
    	// owned, access/update to imcache
   		MIMCacheUpdater* iIMCacheUpdater;
   	
   		//owns
       	MPresenceCacheReader2* iPresenceCacheReader;
            	
       	// owns , presence status
       	TIMCVOnlineStatus iPresenceStatus;
       	
       	// owns , service name
       	HBufC* iServiceName;
       	
       	// owns
       	CIMCVEngineOpenChats* iOpenChats;
       	
       	TInt iServiceId;
      
   	    	  
    };
    
#endif      // __CIMCVENGINEMESSAGEHANDLER_H__

// End of File
