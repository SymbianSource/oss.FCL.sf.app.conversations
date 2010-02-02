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
* Description:  Interface for observer
*
*/


#ifndef _MVIMPSTPROCESSARRAY_H_
#define _MVIMPSTPROCESSARRAY_H_

#include <e32std.h>
#include "tvimpstenums.h"

// FORWARD DECLARATION
class MVIMPSTProcessArrayObserver;
class MVPbkContactLink;
class MVPbkStoreContact;

// CLASS DECLARATION
/**
 *  Interface for observer
 *
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
 class MVIMPSTProcessArray 
 {
  public:
 	   /**
        * register for array call back
        * @param aObserver, observer to be registered for 
        * @return void
        */
        virtual void AddObserver(MVIMPSTProcessArrayObserver* aObserver) = 0 ;
                
       /**
        * unregister for array call back
        * @param aObserver:  to be removed
        * @return void
        */
        virtual void RemoveObserver() = 0;
        	
       /**
        * Count: Called by UI components to get total count of array elements.
        * @return integer: Count of items on the array.
        */
        virtual TInt Count() const = 0; // total count of array 
        
        
         /**
          * @return ETrue if voip presence service is enabled,else EFalse.
          */
        virtual TBool IsSubServiceSupportedL( TVIMPSTEnums::SubServiceType aType ) const = 0 ;
   
	   /**
        * GetType: Function to get the type of list item
        * @param aIndex: index of the list item
        * @return TEnumsPC::TItem: type of the list item as EContactItem, 
        * EOwnStatusItem etc
        */
		virtual TVIMPSTEnums::TItem GetType(TInt aIndex) const = 0;
        
       /**
        * GetItemNameText: To get item name of each list item
        * @param aIndex: Index of the list item whose name is to be returned
        * @return TPtrC: Name of the list item
        */
        virtual TPtrC GetItemNameText(TInt aIndex)=0; 
        
        
       /**
    	* @returns the VPBK Contact Link
    	*/
        virtual MVPbkContactLink* ContactLink(TInt aIndex) = 0;        
       
        
	   /**
        * SetLoginState: To set the loginstate.
        * @param aLoginState: state of login
        */
        virtual void SetLoginStateL(TVIMPSTEnums::TVIMPSTRegistrationState aLoginState) = 0;

		  
       /**
        * SetLoginState: To set the loginstate.
        * @return TVIMPSTRegistrationState, state of login
        */
        virtual TVIMPSTEnums::TVIMPSTRegistrationState GetLoginState() = 0;
        
       /**
    	* @returns the TOnlineStatus, current status.
    	*/
		virtual TVIMPSTEnums::TOnlineStatus GetOnlineStatusL(TInt aIndex) = 0;
        
       	/**
         * Get the status text for the contact
         * @param aIndex: Index of contact item
		 * @return reference to the status text.
         */
		virtual const TDesC& StatusTextL(TInt aIndex ) = 0 ;
		
		 /**
         * Construct an array of contact list positions in model.
         * aPositionArray is cleared before appending positions!
         * @param aPositionArray array in which to store positions
         * @return none
         */
		 virtual void MapContactListPositions(RArray<TInt>& aPositionArray ) = 0;		
      	/**  
      	 * IsMsgPending: To find if there are any pending messages to 
         * 			decide the icons for display
         * @param aIndex: index of the list item
         * @return TBool: ETrue if there are pending messages else EFalse.
         */
        virtual TBool IsMsgPending(TInt aIndex) = 0 ;
        
        /**
         * ResetPendingMsg: To reset if there are any pending messages to 
         *				decide the icons for display
         * @param aIndex: index of the list item
         */
        virtual void ResetPendingMsg(TInt aIndex) = 0;

           
          /**
         * Find any contact which id is aContactId
         * @param ETrue if contact already existst.
         * else EFlase.
         */
        virtual TBool FindAnyContact( const TDesC& aContactId ) = 0;
        
         
         /**
         * check if already conversation opened
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        virtual TBool IsConversationExistL(const TDesC& aRecipientId) = 0;
        
        
        /**
         * check if already conversation opened
         * @param aRecipientId, the selected user id with whom conversation is started
         * @return ETrue if already exist else EFlase.
         */
        virtual TBool IsConversationExist(TInt aIndex) = 0 ;
        
         /**
         * Find the index of contact with contact id as aContactId
         * @param aContactId : id of contact whose index is to be found
         * @return index of contact else KErrNotFounf if not found
         */
		virtual TInt FindContactIndexL( const TDesC& aContactId ) = 0;
        
 		/**
        * GetItemNameText: To get item name of each list item
        * @param aIndex: Index of the list item whose name is to be returned
        * @return TPtrC: Name of the list item
        */
	    virtual TPtrC GetItemUserId(TInt aIndex) =0;
	     
	    /**
         * LoginUserNameFromSettingsL: Login user name of the own user from settings
         * owener ship is transferred to the caller
         * @return allocated HBufC: Login user name of the own user from settings
         */
        virtual const TDesC&  LoginUserIdFromStoreL() const =0;

       
        /**
         * 
         * Return index of aContactId (considering friend request also.).
         * @param aContactId, contact id for which index to be found.
         * @return, Index of aContactId including friend request.
         */
        virtual TInt GetSelectedItemIndex(const TDesC& aContactId) = 0;
        
        /**
         * OwnAvatarIndex: Returns the  avatar index for the own item
         * This index is used to append the avatar(ICON) in the UI.
         * If  index is  ' 0 ', then it  means no avatar is set.
         * 
         * @return index of the avatar
         */
        virtual TInt OwnAvatarIndexL( ) = 0;
        /**
         * SetOwnAvatarIndex: Sets the index for avatar for for the own item
         * 
         * @param aAvatarIndex: Index to be set
         */
        virtual void SetOwnAvatarIndexL( TInt aAvatarIndex ) = 0;
        
        /**
         * OwnAvatarContent: Returns the content of the avatar image for the own item
         * 
         * If nothing  returns KNullDesC8
         * 
         * @return  TDesC8& content of the avatar
         */
        virtual const TDesC8& OwnAvatarContentL( ) const = 0;
        /**
         * AvatarContent: Returns the content of the avatar image for the conatct
         * 
         * If nothing  returns KNullDesC8
         * @param aContactIndex : Index of the conatct item
         * 
         * @return  TDesC8& content of the avatar
         */
        virtual const TDesC8& AvatarContent(TInt aContactIndex) = 0;
         
         /**
         * SetAvatarIndex: Sets the index for avatar for for the corresponding contact item
         * 
         * @param aContactIndex : Index of the conatct item
         * @param aAvatarIndex: Index to be set
         */
        virtual void SetAvatarIndex( TInt aContactIndex, TInt aAvatarIndex )= 0;
       
        /**
         * AvatarIndex: Returns the  avatar index for the corresponding contact item
         * This index is used to append the avatar(ICON) in the UI.
         * If  index is  ' 0 ', then it  means no avatar is set.
         * 
         * @param aContactIndex : Index of the conatct item
         * 
         * @return index of the avatar
         */
        virtual TInt AvatarIndex(TInt aContactIndex) = 0;
        
        /**
         * remove the item from invitation or unknown array if found
         * @param aContactId :  conatct user Id
         * @return ETrue if contact is removed
         */
        virtual TBool RemoveFromUnknonOrInvitationListL( const TDesC& aContactId , TBool aBlocked ) = 0 ;
                
	   	/**
		* @return, formatted string for the listbox item
		*/
        virtual TPtrC16 GetFormattedString(TInt aIndex) = 0;
        
        /**
         * Virtual destructor
         */
        virtual ~MVIMPSTProcessArray() {};
        
 };
#endif // _MVIMPSTPROCESSARRAY_H_

// End of File
