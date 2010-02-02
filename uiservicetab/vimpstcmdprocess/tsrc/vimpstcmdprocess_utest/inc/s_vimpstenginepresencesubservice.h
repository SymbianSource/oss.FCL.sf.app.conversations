/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#ifndef     S_VIMPSTENGINEPRESENCESUBSERVICE_H
#define     S_VIMPSTENGINEPRESENCESUBSERVICE_H

// INCLUDES
#include <e32base.h>
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginesubservice.h"

//FORWARD DECLARATIONS

class MVIMPSTEnginePresenceSubServiceEventObserver;
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


NONSHARABLE_CLASS( CVIMPSTEnginePresenceSubService_Stub ): public CBase, 
						  public MVIMPSTEnginePresenceSubService
                         
    {
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
        static CVIMPSTEnginePresenceSubService_Stub* NewL( );

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
        static CVIMPSTEnginePresenceSubService_Stub* NewLC();

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEnginePresenceSubService_Stub();        
    
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
		
    
    public: //From MVIMPSTEnginePresenceSubService
        void FetchPresenceOfSingleContactL(const TDesC& aContact);
        
         void FetchPresenceOfSinleContactL(const TDesC& aUserId);
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
        TInt FetchPresenceFromCache();
        
        
        /**
         * Subscribe each contact.
         * 
         * @since 5.0 
         * @param aWVID id of the contact to be subscribed for.
         */
        void SubscribeToPresenceCacheL(const TDesC& aWVID);
      
        /**
         * UnSubscribe each contact.
         * 
         * @since 5.0 
         * @param aWVID id of the contact to be unsubscribed for.
         */
        void UnSubscribeToPresenceCacheL(const TDesC& aWVID);
        
        /**
         * Requests the XIMPFw to terminate presence
         * grant request list subscription.
         */
        void  UnSubscribeForAuthorizationL();
      
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
          * @param aUriOfTheContact, URI to be added
          * @return error.
          */
         TInt  AddToBlockListL( const TDesC& aUriOfTheContact );
         /**
          * Returns the blocked list.
          * it returns the blocked list that it maintains locally in engine.
          * @since S60 5.0
          * @return blocked list.
          */
         RPointerArray<HBufC>* GetBlockedList();
         void FetchBlockedListFromServerL(MVIMPSTEngineBlockedListFetchEventObserver* aOb);
         void UpdatePresenceStateL();
         void UnsubscribeListsL();
             
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
        CVIMPSTEnginePresenceSubService_Stub(  );
        
        /**
         * Performs the 2nd phase of construction.
         */        
        void ConstructL();

    
    private: // Data
                                
        // Service identifier
        TUint32 iServiceId; 
        
       
    }; 

#endif //S_VIMPSTENGINEPRESENCESUBSERVICE_H

