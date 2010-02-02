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
* Description:  all observer classes
*
*/


#ifndef MSERVICEWIDGETOBSERVERS_H
#define MSERVICEWIDGETOBSERVERS_H

#include <cchtypes.h>
#include "servicewidgetenums.hrh"
/**
 *  
 *  Observer interface for messages handler	
 *  @dll servicewidgetplugin
 *  @since S60 5.0
 */
 
class  MServiceWidgetNewMessageObserver 
	{
	public:
			/**
	    	* Gets called when data update is available as new messgae recieved.
	    	* Client can then decide wether to use the old one
	    	* or the updated one.
	    	* @param aSenderId, sender id
	    	* @param aMessageText, new message details
	       	*/
	        virtual void HandleNewMessageL(const TDesC& aSenderId ,const TDesC& aMessageText )  = 0 ;
	        
	        /**
	    	* unread message change notification
	    	* Client can then decide wether to use the old one
	       	*/      
	        virtual void HandleCountChangedL() = 0 ;
	      	
		                
	protected :
			/**
			* virtual destructor
			*/
			virtual ~MServiceWidgetNewMessageObserver()
				{
					
				}
	    
			               
	};

/**
 *  
 *  Observer interface for presence handler	
 *  @dll servicewidgetplugin
 *  @since S60 5.0
 */
class  MServiceWidgetPresenceObserver 
	{
	public:
		             
	    /**
         * Handler for own status change notification
         */    
        virtual void HandleOwnStatusChangeL(TWSPresenceStatus aStatus ) = 0;
	
		/**
		* Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
		*
		* @param aErrorCode 
		* @param aPresenceBuddyInfoList presence infos for all buddies requested. 
		* Ownership to callee. Can be null if not found.
		*/    
		virtual void HandleChangeL( ) = 0 ;
	    
        /**
        * from MServiceWidgetPresenceObserver
        * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
        * @param aErrorCode 
        * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
        * Ownership to callee. Can be null if not found.
        */    
		virtual void HandleBuddyPresenceChangeL(const TDesC& aBuddyId, 
                                        TWSPresenceStatus aStatus,
                                        const TDesC& aStatusText ) = 0 ;
	    /**
		* Gets called when data update is available as new messgae recieved.
		* Client can then decide wether to use the old one
		* or the updated one.
		* @param aSenderId, sender name
		* @param aMessageText, new message details
		*/
		virtual void HandleAddRequestL(const TDesC& aRequesterId )  = 0  ;
	
			              
	protected :
			/**
			* virtual destructor
			*/
			virtual ~MServiceWidgetPresenceObserver()
				{
					
				}
	    
			               
};
 
 /**
 *  
 *  Observer interface for session state handler	
 *  @dll servicewidgetplugin
 *  @since S60 5.0
 */
class  MServiceWidgetServiceStateObserver 
	{
	public:
			
	       /**
	         * Handler for service state changes
	         * param aState, service state
	         */    
	        virtual void HandleServiceStatusChangedL( TSWSTRegistrationState aState) = 0;
	                 
	protected :
			/**
			* virtual destructor
			*/
			virtual ~MServiceWidgetServiceStateObserver()
				{
					
				}
		               
	};

/**
 *  
 *  Observer interface for timer 	
 *  @dll servicewidgetplugin
 *  @since S60 5.0
 */
class  MServiceWidgetTimerObserver 
	{
	public:
			/**
	         * Handler for timer expiration
	         */  
	        virtual void HandleTimerExpiredL() = 0 ;
	        
	                
	protected :
			/**
			* virtual destructor
			*/
			virtual ~MServiceWidgetTimerObserver()
				{
					
				}
	    
			               
	};

/**
 *  
 *  Observer interface for handling ai foreground event	
 *  @dll servicewidgetplugin
 *  @since S60 5.0
 */

class MServiceWidgetEventObserver
	{
	public:
		  
	        /**
	         * Handler for timer ai foreground event 
	         */  
	        virtual void HandleServiceEventL(const TDesC& aPublisherId, const TDesC8& aEvent,TDesC& aInstId ) = 0 ;
	                    
	                
	protected :
			/**
			* virtual destructor
			*/
			virtual ~MServiceWidgetEventObserver()
				{
					
				}	
		
	};
#endif  // MSERVICEWIDGETOBSERVERS_H


// End of File
