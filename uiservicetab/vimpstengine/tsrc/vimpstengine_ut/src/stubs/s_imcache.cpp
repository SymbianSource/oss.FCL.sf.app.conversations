/*
* s_imcache.cpp :    Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:s_imcache.cpp 
*
*/
// imcache  related headers
#include "cimcachefactory.h"
#include "mimcacheaccessor.h"
#include "mimcacheupdater.h"

TBool gConversationExist = EFalse;
TBool gConversationUnreadCount = EFalse;

class MyMIMCacheAccessor : public MIMCacheAccessor
     {
     public:
     
 		/**
    	* Register to observer conversation data updates
    	* @param aObserver the observer interface
    	*/
        void RegisterObserverL( MIMCacheEventHandler& /*aObserver*/ ) {}

    	/**
    	* Unregister a conversation data update observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterObserver( MIMCacheEventHandler& /*aObserver*/ ) {}
		
       	/**
    	* to get the unread message count from server
    	* @param aOwnUserId , logged in user id
    	* @return number of unread msg count
    	*/
		TInt GetUnreadMessageCountL( const TDesC& /*aBuddyId*/ ) 
			{
			if (gConversationUnreadCount)
				{
				return 1;
				}

			return 0;
	        }
		
	   /**
    	* to get the all unread message count from server
    	* @param , aOwnUserId logged in user id
    	* @return number of all unread msg count
    	*/
        TInt GetAllUnreadMessageCountL() { return KErrNone; }
        						
		 /**
         * check if already conversation opened
         * @param aServiceId, serviceid to use
         * @param aOwnUserId, own user id 
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        TBool IsConversationExistL( const TDesC& /*aBuddyId*/ ) 
	        {
			if (gConversationExist)
				{
				return ETrue;
				}

			return EFalse;
	        }	
        RArray<SIMCacheChatItem> GetChatListL( const TInt aServiceId  ){}
		/**
		* closes conversation and commit operation
		* @param aServiceId service id.
		* @param aOwnId own user id.
		* @param aRecipientId recipient's user id.
		*/
		void CloseConversationL( const TDesC& /*aBuddyId*/ ) {}
		                           
         /**
    	* to get the friend request count from server
    	* @param , aOwnUserId logged in user id
    	* @return number of all request
    	*/
        TInt GetFriendRequestCountL( ) { return KErrNone; }
        
        
        /**
		* Remove friend request from cahce and commit operation
		* @param aServiceId service id.
		* @param aOwnId own user id.
		* @param aRequesterId ,requester user id.
		*/
		void RemoveFriendRequestL( const TDesC& /*aBuddyId*/  ) {}
		 

     	/**
     	* Destructor
     	*/
     	~MyMIMCacheAccessor() {};
     	
     	MyMIMCacheAccessor() {};
     };

class MyMIMCacheUpdater : public MIMCacheUpdater
     {
     public:

        void RegisterObserverL( MIMCacheEventHandler& /*aObserver*/ ) {}

        void UnRegisterObserver( MIMCacheEventHandler& /*aObserver*/ ) {}

        void StartNewConversationL(const TDesC& /*aBuddyId*/ ) {};
       
        void AppendReceiveMessageL(const TDesC& /*aBuddyId*/,
                                   const TDesC& /*aText*/ ){};
        
        void AppendSendMessageL(const TDesC& /*aText*/ ){};
        
        void AppendMessageL(const TDesC& /*aBuddyId*/, const TDesC& /*aText*/){};
        
        void CloseConversationL( const TDesC& /*aBuddyId*/ ){};
        
        TInt DeactivateConversationL()
            {
            return KErrNone;
            }
        
        void AppendAddRequestL( const TDesC& /*aBuddyId*/, 
                                TInt /*aServiceId*/ ){};
        
        RArray<SIMCacheChatItem> GetChatListL( const TInt& /*aServiceId*/  )
            {
            RArray<SIMCacheChatItem> array;
            return array;
            }

        ~MyMIMCacheUpdater() {};
         
         MyMIMCacheUpdater() {};
     };

CIMCacheFactory* gIMCacheFactory = NULL;
MyMIMCacheAccessor* gMyMIMCacheAccessor = NULL;
MyMIMCacheUpdater* gMyMIMCacheUpdater = NULL;

CIMCacheFactory* CIMCacheFactory::InstanceL() 
	{
	if (!gIMCacheFactory)
		{
		gIMCacheFactory = new ( ELeave ) CIMCacheFactory();
		}
	return gIMCacheFactory;
	}
    	
void  CIMCacheFactory::Release()
	{
	
	delete gMyMIMCacheAccessor;
	gMyMIMCacheAccessor = NULL;
	
	delete gMyMIMCacheUpdater;
	gMyMIMCacheUpdater = NULL;
	
	delete gIMCacheFactory;
	gIMCacheFactory = NULL;
	
		
	}
    	
MIMCacheAccessor* CIMCacheFactory::CreateAccessorL(TInt /*aServiceId*/,
									const TDesC& /*aUserId*/  )
	{
	if (!gMyMIMCacheAccessor)
		{
 	gMyMIMCacheAccessor = new (ELeave) 	MyMIMCacheAccessor();
		}
	return gMyMIMCacheAccessor;		
	}

	
CIMCacheFactory::~CIMCacheFactory()
	{
		
	}

CIMCacheFactory::CIMCacheFactory()
	{
		
	}

void CIMCacheFactory::ConstructL()
	{
		
	}
	
MIMCacheUpdater* CIMCacheFactory::CreateUpdaterL(TInt /*aServiceId*/,
												const TDesC& /*aUserId*/ ,
												TBool /*aRegistrationNeeded*/ )
	{
	if (!gMyMIMCacheUpdater)
	    {
	//    gMyMIMCacheUpdater = new (ELeave)  MyMIMCacheUpdater();
	    }
	return gMyMIMCacheUpdater;     
	}



 
