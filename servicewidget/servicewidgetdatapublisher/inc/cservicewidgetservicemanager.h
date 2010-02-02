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
* Description:  service manager for a specific service
 *                
 *
*/

 

#ifndef CSERVICEWIDGETSERVICEMANAGER_H
#define CSERVICEWIDGETSERVICEMANAGER_H

#include <vwsdef.h>
#include <e32base.h>
#include "mservicewidgetobservers.h"
#include "servicewidgetcpglobals.h"

class CServiceWidgetContentPublisher;
class CServiceWidgetPresenceHandler;
class CServiceWidgetCchHandler;
class CServiceWidgetMessageHandler;
class CServiceWidgetTimer;
class CCoeEnv;
class CServiceWidgetBrandHandler;
class MVIMPSTSettingsStore;
/**
 *  
 * content harvester plugin	
 * layout Description of servicewidgetplugin and numbering of boxes
 *
 *  -------------------------------------
 *  | 1	A	| User ( Offline )	|offline| Text Row 1
 *  |   V	|	2				|icon 3	|
 *  |   A	|----------------------------
 
 *  |	T	| n new message  			| if nothing display "click to online"
 *  |	AR	| or n online friends	4	| Text Row 2
 *	-------------------------------------
 *  new message has higher priority than friends invite
 *  friends invite has higher priority than n online user
 *  any fried come online show for x second and go to back again
 *
 *  @lib servicewidgetplugin
 *  @since S60 3.2
 */
 
class CServiceWidgetServiceManager 	: public CBase,
									  public MServiceWidgetNewMessageObserver,
									  public MServiceWidgetPresenceObserver,
									  public MServiceWidgetServiceStateObserver,
									  public MServiceWidgetTimerObserver
	{
	public:

		// to change priority , rearange this
		enum TIMWidgetContentPriority
			{
			EIMUknown = -1,  // no priority
			EIMNewMessage = 0, // highest priority
			EIMFriendInvite,
			EIMPresence,	 // lowest priority
			};

		
		/**
		* Two-phased constructor.
		* @param aServiceId, a service id
		* @param aContentPublisher, content publisher reference
	    * @param aSettingStore, reference to settings store
		* @return the instance of same class
		*/
		static CServiceWidgetServiceManager* NewL(TUint32 aServiceId,
											      CServiceWidgetContentPublisher& aContentPublisher,
											      MVIMPSTSettingsStore& aSettingStore );

		
		/**
		* Standard C++ destructor
		*/    
		~CServiceWidgetServiceManager();
		
		
public : // new method


    /**
     * update the content in widgets
     */
    void UpdateContentsL();
    /**
     * Returns the service ID for which Service Manager is created
     */
    TInt ServiceId();

    /**
     * Unregisters widget when service is deleted
     */
    void UnregisterWidgetL();

    /**
     * Registers widget when service is installed
     */
    void RegisterWidgetL();

    /**
     * Returns the service Name for which Service Manager is created
     */
    const TDesC& ServiceName();

    /**
     * Intialize all objects once widget is Activated on Home Screen
     */
    void ActivateWidgetL(TDesC& aInstId);
    /**
     * destroy all objects once widget is deactivated from Home Screen
     */
    void DeactivateWidgetL(TDesC& aInstId);
    /**
     *Executes action based on iExecuteOperation
     */
	
    void ExecuteActionL();
   

public : // MServiceWidgetContentPrioritizer
    /**
     * from MServiceWidgetNewMessageObserver
     * Gets called when data update is available as new messgae recieved.
     * Client can then decide wether to use the old one
     * or the updated one.
     * @param aSenderId, sender name
     * @param aMessageText, new message details
     */
    void HandleNewMessageL(const TDesC& aSenderId ,
            const TDesC& aMessageText ) ;

    /**
     * from MServiceWidgetNewMessageObserver
     * im cache call back 
     * @param aState service state
     */   
    void HandleCountChangedL();

    /**
     * from MServiceWidgetPresenceObserver
     * Handler for own status change notification
     */    
    void HandleOwnStatusChangeL(TWSPresenceStatus aStatus) ;

    /**
     * from MServiceWidgetPresenceObserver
     * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
     * @param aErrorCode 
     * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
     * Ownership to callee. Can be null if not found.
     */    
    void HandleChangeL() ;

    /**
     * from MServiceWidgetPresenceObserver
     * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
     * @param aErrorCode 
     * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
     * Ownership to callee. Can be null if not found.
     */    
    void HandleBuddyPresenceChangeL( const TDesC& aBuddyId, 
                                     TWSPresenceStatus aStatus,
                                     const TDesC& aStatusText );

    /**
     * from MServiceWidgetPresenceObserver
     * Gets called when data update is available as new messgae recieved.
     * Client can then decide wether to use the old one
     * or the updated one.
     * @param aSenderId, sender name
     * @param aMessageText, new message details
     */
    void HandleAddRequestL(const TDesC& aRequesterId )   ;


    /**
     * from MServiceWidgetServiceStateObserver
     * Handler cch server 
     * @param aState service state
     */   
    void HandleServiceStatusChangedL(TSWSTRegistrationState aState) ;

    /**
     * from MServiceWidgetTimerObserver
     * timer expiration call back
     * @param aState service state
     */   
    void HandleTimerExpiredL();
    
    /**
         * WidgetSuspended
         * @param aIsWidgetSuspended True - widget is suspended on Homescreen
         *                           False - Widget is active
         */   
    void WidgetSuspended( TBool aIsWidgetSuspended );


	private:
		/**
		* Constructor
		* @param aServiceId, a service id
		* @param aContentPublisher, content publisher reference
		* @param aSettingStore, reference to settings store
		*/   
		CServiceWidgetServiceManager(TUint32 aServiceId,
								     CServiceWidgetContentPublisher& aContentPublisher,
								     MVIMPSTSettingsStore& aSettingStore );
		
		/**
		* provides the two phase construction
		*/  
		void ConstructL();
	
		/**
		* load the resource file in memory for use
		*/  
		void LoadResourceFileL();
		

    /**
     * read and set the current data
     */   
    void SetCurrentTextDataL();

    /**
     * create the second row data
     * @return created text : ownership transfer to caller
     */   
    HBufC* GetSecondRowDataLC() ;

		/**
		* create all handlers if service enable 
		* delete all handlers if service loggedoff
		* @param aState service state
		*/   
		void CreateHandlersL();


    /**
     * reset the priority to zero
     */   
    inline void ResetPriority()
        {
        iPriority = 0 ;	
        }
    

	private :
		// owns : service id 
          TInt iServiceId;
		//not owns : Publisher client
		CServiceWidgetContentPublisher& iContentPublisher;
		//not owns : settings store
		MVIMPSTSettingsStore& iSettingStore;
		// owns : cch handler	
		CServiceWidgetCchHandler* iCchHandler;
    	// owns : presence handler
		CServiceWidgetPresenceHandler* iPresenceHandler;
    	// owns : message handler
		CServiceWidgetMessageHandler* iMessageHandler;
    	// owns : current priority  for second row box 
		TInt iPriority;
    	// owns : service state
		TSWSTRegistrationState iServiceState;
    	// owns : timer
		CServiceWidgetTimer* iWidgetTimer;
		// owns : 
        TSWAExecuteOperation iExecuteOperation;
	     //owns : brand icons 
		CServiceWidgetBrandHandler* iBrandHandler;
		// Not owns : A static CEikonEnv
	    CCoeEnv* iEnv;
	    //tabview id for the service
	    TInt iServiceviewId;
       //owns : service Name
       HBufC* iServiceName;
       // ETrue if widget is suspended
       TBool iIsWidgetSuspended;
       
       //Widget InstanceId
       HBufC* iInstId;
			   // Service tab UID from spsettings
        TInt iServiceTabUid;
	    };

#endif  // CSERVICEWIDGETSERVICEMANAGER_H


// End of File
