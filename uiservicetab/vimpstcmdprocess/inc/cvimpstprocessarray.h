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
* Description:  Array Process
*
*/


#ifndef _CVIMPSTPROCESSARRAY_H
#define _CVIMPSTPROCESSARRAY_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmdhandler.h"
#include "vimpstcmd.hrh"
#include "mvimpststoragecontactsobserver.h"
#include "mvimpstprocessarray.h"
#include "tvimpstenums.h"
#include "mvimpstengineimsubserviceeventobserver.h"
#include "mvimpstenginepresencesubserviceeventobserver.h"
#include <MVPbkStoreContact.h>

// FORWARD DECLARATION
class MVIMPSTProcessArrayItem;
class MVIMPSTStorageContact;
class MVIMPSTStorageContactList;
class MVIMPSTStorageServiceView;
class MVIMPSTProcessArrayObserver;
class MVIMPSTStorageItemModel;
class MVPbkContactLink;
class MVPbkStoreContact;
class MVIMPSTEngine;
class CVIMPSTProcessFriendRequestItem;
class CVIMPSTProcessUnknownContactItem;

//  CLASS DECLARATION

/**
 * Array Process.
 * Responsible for handling all contacts array
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class CVIMPSTProcessArray : public CBase,
	                        public MVIMPSTProcessArray,
	                        public MVIMPSTStorageContactsObserver,
	                        public MVIMPSTEngineIMSubServiceEventObserver,
	                        public MVIMPSTEnginePresenceSubServiceEventObserver
    {
    friend class Cvimpstcmdprocess_utest;
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
         static CVIMPSTProcessArray* NewL(
         					MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTProcessArray();
        
    public: //form mvimpstchatobserver
       /**
        * @see MVIMPSTEngineIMSubServiceEventObserver.h
        **/
       void  HandleChatMessageEventL(TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aSender );
       
            
   	public:	
		  /**
	       * Observer Event for new add request
	       * aRequester - requester for add request.
	       */ 
	       void  HandleAddRequestEventL(TVIMPSTEnums::TOperationType aType, const TDesC& aRequesterId,
                                           const TDesC& aRequestorDisplayName );

       
    public : //from MVIMPSTProcessArray
    	/**
        * register for array call back
        * @param aObserver:  to be added
        */
         void AddObserver(MVIMPSTProcessArrayObserver* aObserver)  ;
                
        /**
        * unregister for array call back
        * @param aObserver:  to be removed
        */
         void RemoveObserver() ;
      
       /**
        * Count: Called by UI components to get total count of array 
        * @return integer Count of array
        */
        virtual TInt Count() const;
        
        /**
        * @return ETrue if presence service is enabled
        */
        TBool IsSubServiceSupportedL( TVIMPSTEnums::SubServiceType aType  ) const ;
        
        /**
         * @return, ETrue if IM is supported
         */
        TBool IsIMSupported();
        
        /**
         * @return, ETrue if Presence is supported
         */
        TBool IsPresenceSupported();
        
       /**
        * GetType: Function to get the type of list item
        * @param aIndex: index of the list item
        * @return TEnumsPC::TItem: type of the list item as EContactItem, EOwnStatusItem etc
        */
	    virtual TVIMPSTEnums::TItem GetType(TInt aIndex) const;
	    
	    /**
         * ResetArray: Function to reset and fill the array
         */
        void ResetArray();    	
       
       /**
        * GetItemNameText: To get item name of each list item
        * @param aIndex: Index of the list item whose name is to be returned
        * @return TPtrC: Name of the list item
        */
	    virtual TPtrC GetItemNameText(TInt aIndex); 
       
       
       /**
        * SetLoginState: To set the loginstate.
        * @param aLoginState: state of login
        */
        virtual void SetLoginStateL(TVIMPSTEnums::TVIMPSTRegistrationState aLoginState);

		  
       /**
        * GetLoginState: To set the loginstate.
        * @return  TVIMPSTRegistrationState, state of login
        */
        virtual TVIMPSTEnums::TVIMPSTRegistrationState GetLoginState();

	   /**
    	* @returns the VPBK Contact Link
    	*/
		MVPbkContactLink* ContactLink(TInt aIndex);
		
		
	   /**
	    * GetOnlineStatusL gets the status of given index. 
	    * @param aIndex: Index of the item 
    	* @returns the onlinestatus of given valid index.
    	*/
		TVIMPSTEnums::TOnlineStatus GetOnlineStatusL(TInt aIndex);
		
		/**
         * Get the status text for the contact
         * @param aIndex: Index of contact item
		 * @return reference to the status text.
         */
		const TDesC& StatusTextL(TInt aIndex ) ;
		
		/**
		 * From MVIMPSTProcessArray
		 *
		 */
		void MapContactListPositions(RArray<TInt>& aPositionArray );		
   	    /**
         * IsMsgPending: To find if there are any pending messages to 
         *				decide the icons for display
         * @param aIndex: index of the list item
         * @return bool: True if there are pending messages else false.
         */
        TBool IsMsgPending(TInt aIndex);
        
        /**
         * ResetPendingMsg: To reset if there are any pending messages to 
         *				decide the icons for display
         * @param aIndex: index of the list item
         */
        void ResetPendingMsg(TInt aIndex);
        
          
        /**
         * Find any contact which id is aContactId
         * @param aContactId, user id.
         * @return ETrue if contact already existst.else EFlase.
         */
       TBool FindAnyContact( const TDesC& aContactId );
       
       /**
         * Find any contact which id is aContactId
         * @param aContactId, user id.
         * @return ETrue if contact already existst.else EFlase.
         */
       TBool FindAnyContactServiceField( const TDesC& aContactId );
       /**
        * Find any friend request which id is aRequesterId
        * @param aRequesterId, user id.
        * @return ETrue if request already existst.else EFlase.
        */
       TBool FindAnyAddRequest( const TDesC& aRequesterId );
       
       /**
        * @param aRequesterId, user id.
        * @return ETrue if request already existst.else EFlase.
        */
       TBool IsContactBlocked( const TDesC& aRequesterId );
           
         /**
         * check if already conversation opened
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        TBool IsConversationExistL(const TDesC& aRecipientId) ;
        /**
         * check if already conversation opened
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        TBool IsConversationExist(TInt aIndex);
        
         /**
         * Find the index of contact with contact id as aContactId
         * @param aContactId : id of contact whose index is to be found
         * @return index of contact else KErrNotFounf if not found
         */
		TInt FindContactIndexL( const TDesC& aContactId ) ;
     
        /**
        * GetItemNameText: To get item name of each list item
        * @param aIndex, Index of the list item whose name is to be returned
        * @return TPtrC, Name of the list item
        */
		TPtrC GetItemUserId(TInt aIndex);
		  /**
        * RemoveFriendRequest: To remove friend request from list.
        * @param aIndex, Index of the list item to be removed.        
        */
         void RemoveFriendRequestL(const TDesC& aRequesterId);
        /**
         * Login user name of the own user from settings
         * owener ship is transferred to the caller
         * @return allocated HBufC: Login user name of the own user from settings
         */
         const TDesC& LoginUserIdFromStoreL() const;
         
         /**
          * OwnAvatarIndex: Returns the  avatar index for the corresponding contact item
          * This index is used to append the avatar in the UI.
          * If  index is  ' 0 ', then it  means no avatar is set.
          * 
          * @return index of the own avatar
          */
         TInt OwnAvatarIndexL( ) ;
         /**
          * OwnAvatarContent: Returns the mime type of the avatar image for the conatct
          * 
          * If nothing  returns KNullDesC8
          * @return  TDesC8& own avatar content
          */
         const TDesC8& OwnAvatarContentL( ) const ;
        
         /**
          * SetOwnAvatarIndex: Sets the index for own avatar contact item
          * @param aAvatarIndex: Index to be set
          */
         void SetOwnAvatarIndexL( TInt aAvatarIndex );
         
         /**
          * AvatarMimeType: Returns the mime type of the avatar image for the conatct
          * 
          * If nothing  returns KNullDesC8
          * @param aContactIndex : Index of the conatct item
          * 
          * @return  TDesC8& mime type of the avatar
          */
         const TDesC8& AvatarContent(TInt aContactIndex) ;
		
		 /**
          * SetAvatarIndex: Sets the index for avatar for for the corresponding contact item
          * 
          * @param aContactIndex : Index of the conatct item
          * @param aAvatarIndex: Index to be set
          */
         void SetAvatarIndex( TInt aContactIndex, TInt aAvatarIndex );
         
         /**
          * AvatarIndex: Returns the  avatar index for the corresponding contact item
          * This index is used to append the avatar in the UI.
          * If  index is  ' 0 ', then it  means no avatar is set.
          * 
          * @param aContactIndex : Index of the conatct item
          * 
          * @return index of the own avatar
          */
         TInt AvatarIndex(TInt aContactIndex) ;
         
         /**
         * from MVIMPSTProcessArray
         * Return index of aContactId (considering friend request also.).
         * @param aContactId, contact id for which index to be found.
         * @return, Index of aContactId including friend request.
         */
         TInt GetSelectedItemIndex(const TDesC& aContactId); 
         
         /*
          *  find and return the array item
          */
         MVIMPSTProcessArrayItem* FindArrayItem( const TDesC& aSenderId );
         
         /*
          *  reset all pending and open flags to EFalse
          */
         void ResetOpenConversationPendingMsg();
         
         /*
          * @aIndex, index for the listbox item whose descriptor is needed
          * @return, the descriptor for the array item to be used by listbox
          */
         TPtrC16 GetFormattedString(TInt aIndex);

         
    public: // From MVIMPSTStorageContactsObserver
       
       
        /**
        * HandleChange: To handle the Chnage event from the MCAStoredContactsObserver
        * @see MCAStoredContactsObserver
        * @param aList: reference to the list at which a change has occured
        * @param aContact:  reference to the contact at which a change has occured
        * @param aEventType: TVIMPSTEnums::TCAObserverEventType,type of change event that has occured 
        * @param aUserIdChanged, ETrue if userid has changed, else EFalse.
        * @return void
        */
         void HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
         							MVIMPSTStorageContactList* aList, 
	        						MVIMPSTStorageContact* aContact,
	        						TInt aContactIndex );
        
        
    private: // Implementation
    
       /**
        * Default constructor
        * @param aEngine, reference to engine.
        */
      
       CVIMPSTProcessArray( MVIMPSTEngine& aEngine );
       
      /**
       * Second phase constructor
       */ 
       void ConstructL();
               
      /**
       * FillItemL: fill up the individual item in the array
       * @return void
       **/
       void FillItemL();
	  /**
       * FillOwnDataL: fill up fill the owndata item
       * @return void
       **/
       TBool  FillOwnDataL();
	   
      /**
       * FillArray: fill up the complete array
       * @return void
       **/
       void FillArrayL();
        
       /**
        * fill the unknown contact to main array
        * @return void
        **/
       void FillUnknownContactsL();
       /**
        * FillAddRequestDataL: fill up the add request to array
        * @return void
        **/
       void FillAddRequestDataL();
      
       /**
        * remove all the unknown contacts from main array iItemArray,
        * so that unknown contact can pesist till the session is entertainesd.
        * @return void
        **/
       void RemoveUnKnownContacts();
        /**
         * RemoveAddRequestData: removes data from iItemArray,
         * so that request can pesist till the reqest is entertainesd.
         * @return void
         **/
       void RemoveAddRequestData();
       
       /**
        * handlle the message which is from unknown buddy 
        * @param aSenderId , Sender Id 
        * @return void
        **/
       void DoHandleUnKnownContactMessageL( const TDesC& aSenderId ,TBool aIsMsgPending);
       
       /**
        * get the all open chat and append unknown contacts
        * @return void
        **/
       void GetAndCreateOpenChatListL();
       
       /**
        * remove the item from invitation or unknown array if found
        * @param aContactId :  conatct user Id
        * @return ETrue if contact is removed
        */
       TBool RemoveFromUnknonOrInvitationListL( const TDesC& aContactId , TBool aBlocked )  ;
       /**
        * play the incoming message tone.
        */
       void PlayMsgBeepL();
       
       /**
        * @return ETrue if unread message exist
        */
       TBool IsUnreadMessageExistsL(const TDesC& aRecipientId);
       /**
        * helper method for removing unknown contacts
        * @return ETrue if contact is removed
        */
       TBool RemoveFromUnknonListL( const TDesC& aAddedUserId );
       /**
        * helper method for removing invitation contacts
        * @return ETrue if contact is removed
        */
       TBool RemoveFromInvitationListL( const TDesC& aAddedUserId );
       /**
        * helper method for finding and marking as open chat
        * @return ETrue if contact is find
        */
       TBool FindAndMarkContactAsOpenChat( const TDesC& aContactId, TBool aMesssagePending  );

    private: // Data
      
      /**
       * starting index of Owndata	, it's 0 as per ui spec
       **/	
	   TInt  iOwnDataIndex; 
	   
	  /**
       * starting index of contacts	, it's 0 as per ui spec
       **/
	   TInt iContactListIndex;	
	   
       //Ref to the engine item for current service
       MVIMPSTEngine& iEngine;

      /**
       * Flag to check whether own data added or not
       **/
       TBool iOwnDataAdded;
       
      /**
	   * Owns. Buffer that hold's information about fetched item
	   **/
 	   HBufC* iData;
 	   	

      /**
       * reference to the contact list interface
       **/
       MVIMPSTStorageServiceView* iContactInterface;
       
       // owned : take RPointerArray if nedded
       MVIMPSTProcessArrayObserver* iProcessObservers;
       
      /**
       * list of items here
       **/
       RPointerArray<MVIMPSTProcessArrayItem> iItemArray; 
       
      
      /**
       * reference to the contact list model
       * not owned.
       **/
       MVIMPSTStorageItemModel* iContactListModel;
       
       //service id
       TUint32 iServiceId;
       
       TVIMPSTEnums::TVIMPSTRegistrationState iLoginState;

      /**
       *  reference list of friend request items here
       **/
       RPointerArray<CVIMPSTProcessFriendRequestItem> iAddRequestArray;
       
        /**
       *  reference list of friend request items here
       **/ 
       RPointerArray<CVIMPSTProcessUnknownContactItem> iUnKnownContactArray;

       // ETrue if fetch completed
       TBool iFetchCompleted;
       
       // ETrue if engine supports IM subservice
       TBool iIMSupported;
       
       // ETrue if engine supports Presence subservice
       TBool iPresenceSupported;
            
    };

#endif // _CVIMPSTPROCESSARRAY_H

// End of File
