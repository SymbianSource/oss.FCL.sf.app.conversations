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
* Description:  Class handling connection to precense framework
*
*/


#ifndef     CVIMPSTENGINEPRESENCESUBSERVICE_H
#define     CVIMPSTENGINEPRESENCESUBSERVICE_H

// INCLUDES
#include <e32base.h>
#include <presencecachereadhandler2.h> // handling the presence cache events 
#include <mpresencebuddyinfo2.h> // buddy info class
#include <presenceinfo.h> //Presence info test

#include "mvimpstenginesubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginecchhandlerobserver.h"
#include "mvimpstenginesessioncntxtobserver.h"
#include "tvimpstenums.h"
#include "cvimpstengineimagehandler.h"
//FORWARD DECLARATIONS
class CVIMPSTEngineServiceTableFetcher;
class CVIMPSTEngineCchHandler;
class CVIMPSTEngineSessionCntxtObserver;
class MPresenceCacheReader2;
class CIMCacheFactory;
class MIMCacheUpdater;
class MPresentityGroupContentEvent;
class CVIMPSTBlockedListManager;
class MVIMPSTEngineServiceConnectionEventObserver;

// CLASS DECLARATION
/**
 *  Presence sub service
 *  This class implements the presence subservice
 *  The problem in VOIP case would be that this "sip" must be hardcoded. Can this be avoided
 *
 *
 *  The Presence Information can be fetched either thro PresenceCache or
 *  thro XIMP's MPresentityPresenceEvent. So somehow we should be able to differentiate 
 *  which service uses PresenceCache or XIMP's MPresentityPresenceEvent during the creation of this object
 *  One of the proposal could be is to use the CCH, if it could provide us this information or SP Settings
 *  
 *  All the Block/Friend Request/Presence information of a contact should be passed 
 *  to the VIMPSTStorage which will update the list on the UI.
 *  
 */


NONSHARABLE_CLASS( CVIMPSTEnginePresenceSubService ): public CBase, 
						  public MVIMPSTEnginePresenceSubService,
                          public MPresenceCacheReadHandler2,                          
                          public MVIMPSTEngineCchHandlerObserver,
                          public MVIMPSTEngineSessionCntxtObserver
    {
    //for testcases
    friend class T_VimpstEnginePresenceSubService;

    public:

        /**
         * Two-phased constructor.
         *
         * @since s60 5.0
         * @param aServiceId, service id
         * @param aCchHandler, reference to cchhandler.
         * @param aTableFetcher, reference to settingstable.
         * @param aXimpEventObserver, reference to ximpevent observer.
         * @param aServiceStateListener, reference to servicestatelistener.
         */
        static CVIMPSTEnginePresenceSubService* NewL( TUint32 aServiceId, 
                                                    CVIMPSTEngineCchHandler& aCchHandler,
                                                    CVIMPSTEngineServiceTableFetcher& aTableFetcher,
                                                    CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
                                                    MVIMPSTEngineServiceConnectionEventObserver& aObserver);

        /**
         * Two-phased constructor.
         *
         * @since s60 5.0
         * @param aServiceId, service id
         * @param aCchHandler, reference to cchhandler.
         * @param aTableFetcher, reference to settingstable.
         * @param aXimpEventObserver, reference to ximpevent observer.
         * @param aServiceStateListener, reference to servicestatelistener.
         */
        static CVIMPSTEnginePresenceSubService* NewLC(TUint32 aServiceId, 
                CVIMPSTEngineCchHandler& aCchHandler,
                CVIMPSTEngineServiceTableFetcher& aTableFetcher,
                CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
                MVIMPSTEngineServiceConnectionEventObserver& aObserver);

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEnginePresenceSubService();        
    
    public: //From MVIMPSTSubService     
    	
        /**
         * @return TVIMPSTRegistrationState Service State of this service
         */
        TVIMPSTEnums::TVIMPSTRegistrationState SubServiceState() const;     
        
        
        TVIMPSTEnums::SubServiceType Type() const;                         

		
		/**
         * @see MVIMPSTSubService
         */
		TBool Enabled();
		
		/**
		* Set the blocked list manager.
		@param aBlockedListMgr blocked list instance pointer.
		*/
		void SetBlockedListManager(CVIMPSTBlockedListManager* aBlockedListMgr);

		//Reset the blocked list to zero blocked contact	
    	void ResetBlockedListManagerL();
    public: //From MVIMPSTEnginePresenceSubService
        
        /**
		* @see MVIMPSTEnginePresenceSubService
		*/
		void RegisterPresenceEventObserverL(
							MVIMPSTEnginePresenceSubServiceEventObserver* aObserver);

		/**
		* @see MVIMPSTEnginePresenceSubService
		*/
		void UnRegisterPresenceEventObserver(
							MVIMPSTEnginePresenceSubServiceEventObserver* aObserver);
		
        /**
         * Subsribes presence of a single contact
         * Can be done for the contacts who are already subscribed(Required atleast in XMPP Case)
         * This function can be used on FriendRequests also wherein it performs 2 operation
         * 1. Accepts the invitation if Friend Request is Pending(GrantPresenceForPresentityL)
         * 2. Subscribes for the presence either to PresenceCache 
         * or XIMP (MPresentityPresenceEvent)
         * 
         *
         * @since S60 5.0
         * @param aUriOfTheContact, Presence URI to be subscribed         
         */
         void SubscribePresenceOfSingleContactL( const TDesC& aUriOfTheContact );            
        
           
        /**
         * Unsubsribes presence of a single contact
         * Used for those contact whose presence has been subscribed
         * Can be used for Friend Requests also
         * 1. Denies the invitation for the contact (WithdrawPresenceGrantFromPresentityL)
         * 2. Sends a unsubscribe to PresenceCache 
         * or XIMP (UnsubscribePresentityPresenceL)
         *
         * @since S60 5.0
         * @param aUriOfTheContact, Presence URI to be subscribed         
         */
         void UnSubscribePresenceOfSingleContactL( const TDesC& aUriOfTheContact );				
		
        /**
         * Fetches presence from presencecache.
         * 
         * @since 5.0
         **/
        TInt  FetchPresenceFromCache();
        
        /**
         * Unsubscribes ximp group contents.
         * 
         * Buddy list and authorization lists are always unsubscibed.
         * Block list is unsubscribed if it is supported.
         *
         * @since S60 5.0
         * @param None
         * @leave Standard Symbian error codes.
         * @return None
         */                 
        void UnsubscribeListsL();
      
       /**
        * Requests the XIMPFw to subscribe for presence
        * grant request list subscription.
        */  
       void  SubscribeForAuthorizationL();
       
       /**
        * sends PresenceGrantForPresentity  response to server        
        * @param aContactId, id of the requesting party..
        * @param aResponse , Etrue if accpeted else rejected. 
        * @return TInt error if any       
        */ 
        TInt SendPresenceGrantPresentityL( const TDesC& aContactId , TBool aResponse );
        
        /**
         * IsChangeOwnStatusSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
         TBool IsChangeOwnStatusSupported() ;
         
        /**
         * IsStatusMsgSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
         TBool IsStatusMsgSupported();
         
          /**
         * IsAvatarSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
        TBool IsAvatarSupported() ;
        
        /**
         * SetAvatarSupported by the protocol
         * 
         * @since S60 5.0   
         * @param aSupported, TBool ETrue if supported else EFalse.
         */ 
        void SetAvatarSupported(TBool aSupported ) ;
      

         /**
          * PublishOwnPresenceL
          * to update the  ownstatus to server.
          * and publishes the own status.
          * 
          * @param aStatus ,status to be updated to the server.
          * @param aValue , text for the status message
 		  * @param aFilename , avatar file name
  		  * @param aMimetype , a mime type
  		  * @param aIsAvatar , a avatar field included
          * @return TInt error if any
          */                                
         TInt PublishOwnPresenceL(TVIMPSTEnums::TOnlineStatus aStatus,
         						  const TDesC& aValue,
         						  const TDesC& aFilename = KNullDesC, 
         						  const TDesC8& aMimetype = KNullDesC8,
         						  TBool aIsAvatar = EFalse  );

      
         /**
          * Removes URI from block list
          *
          * @since S60 5.0
          * @param aUriOfTheContact, URI to be removed
          * @return error.
          */
         TInt  RemoveFromBlockListL( const TDesC& aUriOfTheContact );

         /**
          * Adds URI to block list
          *
          * @since S60 5.0
          * @param aContactId, URI to be added
          * @return error.
          */
         TInt  AddToBlockListL( const TDesC& aContactId );
         
         /**
         * Get list of blocked contact.
         */
         RPointerArray<HBufC>* GetBlockedList();
			
         /**
         * fetch blocked list from server to get the freshed blocked list.
         */
		 void FetchBlockedListFromServerL(MVIMPSTEngineBlockedListFetchEventObserver* aOb);

         /**
         * function called from the engine once contact fetching completes
         */
         void UpdatePresenceStateL();
         
         /**
          * Fetches the presence of singlecontact form presence cache
          * @param aContact, user id of the ocntact who's presence 
          * needs to eb fetched.
          */
         void FetchPresenceOfSingleContactL(const TDesC& aContact);
    
   
        	             
	private: // prescachereadhandler
	
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
    	
    
    private: // From MVIMPSTEngineCchHandlerObserver
        
        /**
         * From MVIMPSTEngineCchHandlerObserver 
         * Observer callback for CCH event
         *
         * @param aServiceId, service id
         * @param aState, service state
         * @param aServiceError, error
         * @since S60 5.0
         */
        void CchEventOccuredL( 
            TUint aServiceId, 
            TCCHSubserviceState aState, 
            TInt aServiceError );
        /**
         * Handles any error that occured during login.
         
         * @since S60 5.0
         * @param aServiceError, error
         */
        void DoHandleCchErrorL( TInt aServiceError );
        
    public:
        /**
         * Observer Event for forwarding the ximp events.
         * @param aContext - ximpfw context
         * @param aEvent - ximpfw event
         */ 
        void HandleSessionContextEventL( const MXIMPContext& aContext, 
                                             const MXIMPBase& aEvent,
                                             TXimpOperation aXimpOperation = EVIMPSTXimpOperationNoOperation);                        

    private:
          
        /**
         * Resolves service state for given service id.
         *
         * @since S60 5.0
         * @param aState, state of the subservice
         * @param aServiceError, service error is stored here
         * @return TVIMPSTRegistrationState,
         * service state to use
         */      
        TVIMPSTEnums::TVIMPSTRegistrationState  ResolveServiceStateL( TCCHSubserviceState aState, 
                TInt aServiceError );
        
        /**
         * Gets a list of all presence subscribtions
         *
         * @since S60 5.0
         */
        void RetrieveSubscribedListL();
     
        /**
         * Handles presentity group context events.
         *
         * @since S60 5.0
         * @param aContext, presence context
         * @param aEvent, presence event
         */        
        void DoHandlePresentityGroupContentEventL(
            const MXIMPContext& aContext,
            const MXIMPBase& aEvent );

        /**
         * Handles presentity grant request list events.
         *
         * @since S60 5.0
         * @param aContext, presence context
         * @param aEvent, presence event
         */   
        void DoHandlePresenceGrantRequestListEventL(
            const MXIMPContext& aContext,
            const MXIMPBase& aEvent );
        
        /**
         * Handles presentity block list events.
         *
         * @since S60 5.0
         * @param aContext, presence context
         * @param aEvent, presence event
         */   
       void DoHandlePresenceBlockListEventL(
           const MXIMPContext& aContext,
           const MXIMPBase& aEvent );       
      
        /**
         * Handles completed ximp event. 
         *
         * @since S60 5.0
         * @param aType, completed event. 
         * @param aCompleteCode, error code of complete
         * @param aEvent, MXIMBase for the clients to use information
         */                
        void HandleXimpRequestCompleteL( TXimpOperation aType,
                                             TInt aCompleteCode,
                                             const MXIMPBase& aEvent);
        
        /**
         * Handles completed list/bind/unbind events. 
         *
         * @since S60 5.0
         * @param aType, completed event. 
         * @param aCompleteCode, error code of complete
         * @param aEvent, MXIMBase for the clients to use information
         */                
        void HandleListEventCompleteL(TXimpOperation aType,
                                         TInt aCompleteCode,
                                         const MXIMPBase& aEvent );
        /**
         * ConvertPresenceStatus 
         * @param aStatus presence status to be converted
         * @return  TAvailabilityValues
         */
        NPresenceInfo::TAvailabilityValues ConvertPresenceStatus(TVIMPSTEnums::TOnlineStatus aStatus);
        

        /**
         * Stores all the contact into virtual store
         * need to chec how thi has to be handled for sip:as 
         * sip does not fetch the contacts from the server.
         * 
         * @since 5.0
         * @param aListEvent list containing all the group members.
         */
        void StoreToVirtualStoreL( const MPresentityGroupContentEvent& aListEvent );
        
        
        
        /**
         * Converts the  presence cache status to client supported presence status.
         * 
         * @since 5.0
         * @param aAvailabilityEnum presence cache status value.
         * @param aAvabilityText avability text, To support Multiple presence states.
         * @return TVIMPSTEnums::TOnlineStatus client  presence status.
         */
        TVIMPSTEnums::TOnlineStatus ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum,TPtrC aAvabilityText);
        /**
         * Initiates downloading of block list
         * @since 5.0
         */
        void RetrieveBlockListL();
        
        /**
         * Converts the  presence cache extension keys 
         * user for block and pending states  to client supported presence status.
         * 
         * @since 5.0
         * @param aPresenceBuddyInfo presence buddy info class.
         * @param TVIMPSTEnums::TOnlineStatus client  presence status.
         */
        void GetKeyFieldsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo,TVIMPSTEnums::TOnlineStatus &aStatus);
        
		  /**
           * Handler for Buddy presence change notifications.
           *
           * @param aPresenceBuddyInfo presence info for buddy. 
           */
        void DoHandlePresenceNotificationL(MPresenceBuddyInfo2& aPresenceBuddyInfo);

        
    private: // Implementation

        /**
         * Standard C++ constructor
         * 
         * @since s60 5.0
         * @param aServiceId, service id
         * @param aCchHandler, reference to cchhandler.
         * @param aTableFetcher, reference to settingstable.
         * @param aXimpEventObserver, reference to ximpevent observer.
         * @param aServiceStateListener, reference to servicestatelistener.
         */   
        CVIMPSTEnginePresenceSubService( TUint32 aServiceId, 
                CVIMPSTEngineCchHandler& aCchHandler,
                CVIMPSTEngineServiceTableFetcher& aTableFetcher,
                CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
                MVIMPSTEngineServiceConnectionEventObserver& aObserver );
        
        /**
         * Performs the 2nd phase of construction.
         */        
        void ConstructL();
    
    private: // Data
                                
        // Service identifier
        TUint32 iServiceId; 
        
        //Ref to the CCH Handler
        //thro this interface Presence sub service listens to the
        //ECCHPrsenceSub connectivity events.
        CVIMPSTEngineCchHandler& iCchHandler; 
        
        //doesnt own, reference to settings table.
        CVIMPSTEngineServiceTableFetcher& iSettingsTableFetcher;
        
        //doesnot own, reference to ximpeventobserver.
        CVIMPSTEngineSessionCntxtObserver& iXimpEventObserver;
        
        //does not own, reference to servicestatelistener.
        MVIMPSTEngineServiceConnectionEventObserver& iObserver;
        
          
        //owns
        MPresenceCacheReader2* iPresenceCacheReader;    
        
        //Owns, service name
        HBufC* iServiceName;
        

        // Stores services current state
        TVIMPSTEnums::TVIMPSTRegistrationState  iServiceState;        
        
        TBool iChangeStatusSupported;
        
        TBool iChangeStatusMsgSupported;
        
        // ETrue if supported
        TBool iAvatarSupported;
        
        //Doesnt Own
		MVIMPSTEnginePresenceSubServiceEventObserver* iSubServiceObserver;
		
		 // Etrue if clearing avatar in progress
		TBool iIsClearingAvatar;
		
		//stores the default MBM path
	    TFileName iDefaultMbmPath;
		
		//Blocked list manager instance pointer.
		CVIMPSTBlockedListManager* iBlockedListMgr;

		//Not owned;Notify that blocked list has been fetched.
		MVIMPSTEngineBlockedListFetchEventObserver* iBlockedListObserver;
		
		//flag indicate if request for fetching blocked list is pending.
		TBool iBlockListFetchReqPending;
		
		//flag indicates pending Logout request
		TBool iLogoutRequest;
		// owns : ETrue if fetching is ongoing
		TBool iIsFetchingContact;
		
		TBool iAutoAccept;
		
		// owns, tells weather it is subscribe to authorization list or not.
		TBool iSubscribeToAuthList;
	
   }; 

#endif //CVIMPSTENGINEPRESENCESUBSERVICE_H

