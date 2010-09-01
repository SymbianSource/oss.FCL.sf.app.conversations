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
* Description:  presene status handler 
 *                
 *
*/


#ifndef CSERVICEWIDGETPRESENCEHANDLER_H
#define CSERVICEWIDGETPRESENCEHANDLER_H

#include <presencecachereader2.h>
#include <mpresencebuddyinfo2.h> // buddy info class
#include <presenceinfo.h> //Presence info test
#include <presencecachereadhandler2.h> // handling the presence cache events 
#include <ximpcontextobserver.h>
#include "servicewidgetenums.hrh"

// farword declarations
class MXIMPContext;
class MXIMPClient;
class MPresenceCacheReadHandler2;
class MServiceWidgetPresenceObserver;
class CActiveSchedulerWait;
class MXIMPBase;
class MPresenceFeatures;
class CServiceWidgetBuddyPresence;
class CFbsBitmap;

// class defnitions
class CServiceWidgetPresenceHandler  : public CBase,
								  	   public MXIMPContextObserver,
							      	   public MPresenceCacheReadHandler2
							      	   
{
public:
   	 
    /**
      * Two-phased constructor.
      * @param aPresenceObserver, prsence observer
      * @param aServiceName, service name
      */
     static CServiceWidgetPresenceHandler* NewL( MServiceWidgetPresenceObserver& aPresenceObserver,
     										 const TDesC& aServiceName,
     										 const TDesC& aOwnId );

     /**
      * Two-phased constructor.
      * @param aPresenceObserver, prsence observer
      * @param aServiceName, service name
      */
     static CServiceWidgetPresenceHandler* NewLC( MServiceWidgetPresenceObserver& aPresenceObserver,
     										 const TDesC& aServiceName,
     										 const TDesC& aOwnId );
     
     /**
      * Standard C++ destructor
      */    
     ~CServiceWidgetPresenceHandler();
      
 public:
		
		/**
		* @return online counts
		*/
		TInt OnlineBuddyCount();

		/**
		* @return  all friend count
		*/
		TInt AllBuddyCount();
		
		/**
		* @return  all inviations count
		*/
		TInt GetInviteCounts();
		
		/**
		* @return on going operation id
		*/
		TWServiceOperationId GetOngoingOperationId();

		  /**
         * get the connected session
         * @param aProtocolUid, protocal to use
         * @param aServiceId, service id to use
          */ 
        void GetConnectedSessionL(TInt aProtocolUid,
        						  TInt aServiceId );
        
         /**
         * release the connected session
         */ 
        void ReleaseConnectionL();
        
        /**
         * release the connected session
         */ 
       	TWSPresenceStatus GetOwnStatus();
       

 private : // from MXIMPContextObserver
		/**
	     * HandleximpContextEvent method from context obsrver  
	     * ximp framework
	     * @param aContext
	     * @param aEvent, event  
	     */
	     void HandlePresenceContextEvent(const MXIMPContext& aContext,
	                                     const MXIMPBase& aEvent );      	
		
 private: // from prescachereadhandler
		/**
	     * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
	     *
	     * @param aErrorCode 
	     * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
	     *         Ownership of elements to callee. Can be empty if not found.
	     */   
	    void HandlePresenceReadL(TInt aErrorCode,
	         			RPointerArray<MPresenceBuddyInfo2>& aPresenceBuddyInfoList) ;
	                        
	    /**
	     * Handler for Buddy presence change notifications.
	     *
	     * @param aErrorCode 
	     * @param aPresenceBuddyInfo presence info for buddy. 
	     *         Ownership to callee. Can be NULL if aErrorCode exist.
	     */    
	    void HandlePresenceNotificationL(TInt aErrorCode,
	        					MPresenceBuddyInfo2* aPresenceBuddyInfo);
       
private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CServiceWidgetPresenceHandler( MServiceWidgetPresenceObserver& aPresenceObserver );
        
        /**
         * Performs the 2nd phase of construction.
         * @param aServiceName, service name
          */        
        void ConstructL(const TDesC& aServiceName, const TDesC& aOwnId );
        
        
         /**
         * subsribe the presence change notification
          */ 
        void RetrieveSubscribedListL();
        
         /**
         * subsribe the presence change notification
         */ 
        void  SubscribeForAuthorizationL();
        
         /**
         * subsribe the presence change notification
         * @param aUserId, user  to use
          */ 
        void SubscribePresenceL( const TDesC& aUserId );
        
         /**
         * read buddy presence from cache
         */    
        void  ReadPresenceFromCacheL();
      
      	/**
         * helper method for bind complete 
         */ 
      	void DoHandleBindCompleteL();
      	
      	/**
         * helper method  buddy fetch event
         * @param aEvent, group list event
         */ 
      	void DoHandlePresentityGroupContentEventL( const MXIMPBase& aEvent );
      	
      	/**
         * helper method  friend request fetch event
         * @param aEvent, grand list event
         */ 
      	void DoHandlePresenceGrantRequestListEventL( const MXIMPBase& aEvent );
      	
      	/**
         * helper method  for converting ximpstatus to local status
         * @param aAvailabilityEnum, availability of contact
         */ 
      	TWSPresenceStatus ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum,TPtrC aAvabilityText);
      	
      	/**
         * method for maitain the unique information about status
         * @param aUserId, a user id of contact
         * @param aStatus, status of contact
         */ 
      	void CheckAndCacheBuddyAvailabilityL( const TDesC& aUserId, TWSPresenceStatus aStatus );

private: // Data

		 // not owns : reference to presence observer for notification
        MServiceWidgetPresenceObserver& iPresenceObserver;
	     // owns : service name 
        HBufC* iServiceName;
        // owns :  own user id 
        HBufC* iOwnUserId;
        // not own
        TInt iServiceId;
	     // own , client for presence framework
        MXIMPClient*   iClient;
        // own ,presence context to prsence framework
        MXIMPContext* iPresenceCtx;
	      //Stores the XIMPFw EventTypes Subscribed for      
	    RArray< TInt32 > iAcceptedEventTypes;
	    //owns : reference to presence cache reader
		MPresenceCacheReader2* iPresenceCacheReader;
	    // owns : total budy  count 
        TInt iTotalCount;
         // owned  pointer to presence plugin
	    MPresenceFeatures* iFeatures;
	 	// own : current ongoing operation
  		TWServiceOperationId iOperationId;
  		// owns : list of presence buddy
  		RPointerArray<CServiceWidgetBuddyPresence> iOnlineBuddyArray;
  		// owns : own status
  		TWSPresenceStatus iOwnStatus;
  		// owns : invitation count
  		TInt iInvitationCount;
 
};

#endif /*CSERVICEWIDGETPRESENCEHANDLER_H*/
