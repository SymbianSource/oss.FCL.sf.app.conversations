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
* Description:  M-Class that capsulates presence sub service API's
*
*/


#ifndef MVIMPSTENGINEPRESENCESUBSERVICE_H
#define MVIMPSTENGINEPRESENCESUBSERVICE_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"
#include "mvimpstenginesubservice.h"


//FORWARD DECLARATION
class MVIMPSTEnginePresenceSubServiceEventObserver;
class MVIMPSTEngineBlockedListFetchEventObserver;
//CLASS DECLARATION
/**
 *  
 *  This class list the M-Class for subservices
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
//Class declaration
class MVIMPSTEnginePresenceSubService: public MVIMPSTEngineSubService
	{
	
	public:
	
		/**
         * Casts MVIMPSTEnginePresenceSubService to this interface.
         *
         * @precond aSubService.Type() == TVIMPSTEnums::EPresence
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aSubService    Base Class .
         * @return Casted inteface for the Presence SubService
         */
        IMPORT_C static MVIMPSTEnginePresenceSubService& Cast(
                MVIMPSTEngineSubService& aSubService);

        
        /**
         * Casts constMVIMPSTEnginePresenceSubService to this interface.
         *
         * @precond aSubService.Type() == TVIMPSTEnums::EPresence
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aSubService    Base Class .
         * @return Casted inteface for the Presence SubService
         */        
        IMPORT_C static const MVIMPSTEnginePresenceSubService& Cast(
                const MVIMPSTEngineSubService& aSubService);
	
	
		/**
		* RegisterPresenceEventObserver
		* The observers that have implemented MVIMPSTEnginePresenceSubServiceEventObserver can use this API
		* to register themselves to receive Presence SubService related events
		* @param aObserver - Object that implements the observer class 
		*   MVIMPSTEnginePresenceSubServiceEventObserver that needs to be added.
		*/
	  	virtual void RegisterPresenceEventObserverL(
	  								MVIMPSTEnginePresenceSubServiceEventObserver* aObserver) = 0;
	  	
		/**
		* UnRegisterChatObserver
		* The observers that have implemented MVIMPSTEnginePresenceSubServiceEventObserver can use this API
		* to unregister themselves from Presence SubService related events
		* @param aObserver - Object that implements the observer class 
		*    MVIMPSTEnginePresenceSubServiceEventObserver that needs to be removed.
		*/
	  	virtual void UnRegisterPresenceEventObserver(
	  								MVIMPSTEnginePresenceSubServiceEventObserver* aObserver) = 0;
	  	
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
        virtual  void SubscribePresenceOfSingleContactL( const TDesC& aUriOfTheContact ) = 0;  
        
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
        virtual void UnSubscribePresenceOfSingleContactL( const TDesC& aUriOfTheContact ) = 0;                          
        /**
         * Fetches presence from presencecache.
         * This is called by the service tab whenever it comes up,
         * so that it has uptodate presence information of the contacts
         * that are present in the service tab.
         * 
         * @since S60 5.0   
         **/
        virtual TInt FetchPresenceFromCache() = 0;
        
       
       /**
        * Requests the XIMPFw to subscribe for presence
        * grant request list subscription.
        */  
        virtual void  SubscribeForAuthorizationL() = 0;
       
       /**
        * sends PresenceGrantForPresentity  response to server        
        * 
        * @since S60 5.0   
        * @param aContactId, id of the requesting party..
        * @param aResponse , Etrue if accpeted else rejected. 
        * @return TInt error if any 
        *      
        */ 
        virtual TInt SendPresenceGrantPresentityL( const TDesC& aContactId , TBool aResponse ) = 0;

        /**
         * IsChangeOwnStatusSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
        virtual TBool IsChangeOwnStatusSupported() = 0;
        
        /**
         * IsStatusMsgSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
        virtual TBool IsStatusMsgSupported() = 0;
        
         /**
         * IsAvatarSupported by the protocol
         * 
         * @since S60 5.0   
         * @return TBool ETrue if supported else EFalse.
         */ 
        virtual TBool IsAvatarSupported() = 0;
        
        /**
         * SetAvatarSupported by the protocol
         * 
         * @since S60 5.0   
         * @param aSupported, TBool ETrue if supported else EFalse.
         */ 
        virtual void SetAvatarSupported(TBool aSupported ) = 0;

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
         virtual TInt PublishOwnPresenceL(TVIMPSTEnums::TOnlineStatus aStatus,
         						  const TDesC& aValue,
         						  const TDesC& aFilename = KNullDesC, 
         						  const TDesC8& aMimetype = KNullDesC8,
         						  TBool aIsAvatar = EFalse  ) = 0 ;
        
         /**
          * Removes URI from block list
          *
          * @since S60 5.0
          * @param aUriOfTheContact, URI to be removed
          * @return error.
          */
         virtual TInt  RemoveFromBlockListL( const TDesC& aUriOfTheContact ) = 0;

         /**
          * Adds URI to block list
          *
          * @since S60 5.0
          * @param aContactId, URI to be added
          * @return error.
          */
         virtual TInt  AddToBlockListL( const TDesC& aContactId ) = 0;
         
         /**
          * Returns the blocked list.
          * it returns the blocked list that it maintains locally in engine.
          * @since S60 5.0
          * @return blocked list.
          */
         virtual RPointerArray<HBufC>* GetBlockedList() = 0;
         
		/**
		* fetch blocked list from server to get the freshed blocked list.
		*@param aOb,pointer of MVIMPSTEngineBlockedListFetchEventObserver,
		*		   Notify that fetch of blocked list has been completed..
		*/
		virtual void FetchBlockedListFromServerL(MVIMPSTEngineBlockedListFetchEventObserver* aOb) = 0;

         /**
          * function call from the engine on the completion of contact fetch operation
          */
         virtual void UpdatePresenceStateL() = 0;
         
     /**
      * Fetches the presence of singlecontact form presence cache
      * @param aContact, user id of the ocntact who's presence 
      * needs to eb fetched.
      */
     virtual void FetchPresenceOfSingleContactL(const TDesC& aContact) = 0; 
     
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
    virtual void UnsubscribeListsL() = 0;
       /**
		* virtual Destructor
		*/		
		virtual ~MVIMPSTEnginePresenceSubService() 
		{}
		
	};

#endif //MVIMPSTENGINEPRESENCESUBSERVICE_H


