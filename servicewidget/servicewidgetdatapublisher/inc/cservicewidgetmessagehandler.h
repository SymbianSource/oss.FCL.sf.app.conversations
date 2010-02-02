/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message handler notification
 *                
 *
*/

 

#ifndef CSERVICEWIDGETMESSAGEHANDLER_H
#define CSERVICEWIDGETMESSAGEHANDLER_H

#include <e32base.h>
// imcache
#include <mimcacheeventhandler.h>
#include <imcachedefs.h>

// farword declaration
class MIMCacheAccessor;
class CIMCacheFactory;
class MServiceWidgetNewMessageObserver;

const TUid KConversationViewAppUid = {0x20012425};
const TUid KIMPhoneBookUid = {0x101f4cce};

/**
 * @class im cache message handler class implementation
 * @since 5.0
 */
 
class CServiceWidgetMessageHandler 	 : public CBase,
                               	   public MIMCacheEventHandler
{

public:
    
    /**
      * Two-phased constructor.
      * @param aMessageObserver, refernce to content handler
      * @param aServiceId, service id to monitor
      * @param aOwnUserId, own user id
      */
     static CServiceWidgetMessageHandler* NewL(MServiceWidgetNewMessageObserver& aMessageObserver,
     								  TInt aServiceId, 
     								  const TDesC& aOwnUserId );

      /**
      * Two-phased constructor.
      * @param aMessageObserver, refernce to content handler
      * @param aServiceId, service id to monitor
      * @param aOwnUserId, own user id
      */
     static CServiceWidgetMessageHandler* NewLC(MServiceWidgetNewMessageObserver& aMessageObserver,
     									 TInt aServiceId, 
     									 const TDesC& aOwnUserId  );
     
     /**
      * Standard C++ destructor
      */    
     ~CServiceWidgetMessageHandler();
     
public : // new methods 
	
		/**
		* get the unread counts
		* @return count
		*/
		TInt GetUnreadMessageCountL();

		/**
		* get the unread sender
		* @return sender
		*/
		const TDesC& GetUnreadSenderId() ;
		
		/** check if only one sender or multiple 
		* @return ETrue or False
		*/
		TBool IsSingleMessageSender(); 


public:   // from MIMCacheEventHandler
    
    		/**
    	* Gets called when data update is available.
    	* Client can then decide wether to use the old one
    	* or the updated one.
    	* @param aNewMessage, new message details
       	*/
        void HandleIMCacheEventL(TIMCacheEventType aEvent, TAny* aChatMessage = NULL ) ;
     
        
              
private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CServiceWidgetMessageHandler(MServiceWidgetNewMessageObserver& aMessageObserver );
        
        /**
         * Performs the 2nd phase of construction.
         * @param aOwnUserId, user id
         */        
        void ConstructL( const TDesC& aOwnUserId, TInt aServiceId  );
        
        /**
         * serach the given id in array 
         * @param aNewSenderId, sender user id
         * @return ETrue if found
         */    
        TBool IsExistInSendersArrayL( const TDesC& aNewSenderId ) ;
        
        /**
         * synchronized the senders array by deleting the sender
         * which is not having  any unread message
         */    
        void SynchronizeSendersArrayL() ;
        
        /*
         * plays the msg tone for incoming messages
         * 
         */
        void PlayMsgAlertToneL();

private: // Data
		// not owns , reference to contententpritizer
		MServiceWidgetNewMessageObserver& iMessageObserver;
		// owns : im cache accessor
		MIMCacheAccessor* iAccessor;
		//owns : im cache factory
		CIMCacheFactory* iIMCacheFactory;
		// owns : sender is unique or multiple sender
		RPointerArray<HBufC> iSendersArray;
};

#endif /*CSERVICEWIDGETMESSAGEHANDLER_H*/

