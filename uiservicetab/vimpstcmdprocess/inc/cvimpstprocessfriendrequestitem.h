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
* Description: Friend request item, stores all the information regarding *the ERactive authorization 
*
*/



#ifndef CVIMPSTPROCESSFRIENDREQUESTITEM_H_
#define CVIMPSTPROCESSFRIENDREQUESTITEM_H_

#include <e32base.h>
#include "tvimpstenums.h"
#include "cvimpstprocessarrayitem.h"

//  CLASS DECLARATION

/**
 * Friend request item, stores all the information regarding the ERactive authorization.
 * Responsible for handling feriend requests.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class CVIMPSTProcessFriendRequestItem:public CBase,
                                      public MVIMPSTProcessArrayItem
    {
    public: // symbain constructor , destructor
        
        /**
        * Returns the instance of tis class.
        * @param aRequesterId, requestor id.
        * @return CVIMPSTProcessFriendRequestItem, instance of this class.
        */
        static CVIMPSTProcessFriendRequestItem* NewL(CVIMPSTProcessArray& aParent, const TDesC& aRequesterId, const TDesC& aRequestorDisplayName);
        
        /**
         * symbain way of construction
         * @param aRequesterId, requestor id.
         */
        void ConstructL(const TDesC& aRequesterId, const TDesC& aRequestorDisplayName);
        
        /**
         * destructor
         */
        virtual ~CVIMPSTProcessFriendRequestItem();
        
    private:

         /**
          * C++ default constructor
          */
        CVIMPSTProcessFriendRequestItem(CVIMPSTProcessArray& aParent);
        
    public: //From MImArrayItem
    
        /**
         * Returns the name of the contact item 
         * @return TPtrC, name of the contact item to be displayed
         */
        TPtrC GetItemNameText();
        
       /**
        * Returns the default type of the contact item 
        * @return TEnumsPC::TItem: type of the contact item as either 
        * EContactitem, EContactListItem etc
        */
        TVIMPSTEnums::TItem Type();
    
        
        /**
         * Get the index of the contact item from engine side model
         * @return TInt: index
         */
        inline TInt GetItemIndex()   
            {   
            return iIndex;    
            }
        
        /**
         * Set the index of the contact item from engine side model
         * @param TInt: aIndex
         */
        inline void SetItemIndex(TInt aIndex)   
            {  
            iIndex  = aIndex;
            }
        
        /**
         * @return the VPBK Contact Link
         */
        inline MVPbkContactLink* ContactLink()
            {
            return NULL;
            }
            
        
        
        /**
         * Returns the user id of the listbox item 
         * @return TPtrC, user id of the listbox item.
         */
        inline TPtrC GetItemUserId();

        /**
         * Returns the avatar index of the listbox item 
         * @return TInt: avatar index of the listbox item.
         */
        inline TInt  AvatarIndex() 
            {
            return KErrNone; // always return zero
            }

        /**
         * sets the avatar index of the listbox item 
         * @param aAvatarIndex: avatar index to be set 
         */
        inline void  SetAvatarIndex(TInt /*aAvatarIndex*/ ) 
            {
            // dummy not used
            }
       
        /**
         * Sets the messages pending status of the contact item 
         * @param aPendingMessageStatus: true if the contact has pending messages 
         * else false
         */
        void SetMsgPending(TBool aPendingMessageStatus);

        /**
         * gets the messages pending status of the contact item 
         * @return iSMsgPending: true if the contact has pending messages 
         * else false
         */
        TBool IsMsgPending();

        /**
         * Sets the name of the listbox item 
         * @param aName: Name of the listbox item to be set
         * @return void
         */

        void SetConversationOpen(TBool aConvOpen ) ;

        /**
         * gets the messages pending status of the contact item 
         * @return iSMsgPending: true if the contact has pending messages 
         * else false
         */
        TBool IsConversationOpen() ;
        
        /**
         * returns status text of the contact
         */
        const TDesC& StatusText()
            {
            //NA
            return KNullDesC;
            }
        
        /**
         * returns online status of the contact
         */
        TVIMPSTEnums::TOnlineStatus OnlineStatus()
            {
            //NA
            return TVIMPSTEnums::EUnknown;
            }
        
        /*
         * Format and Reformat the string for listbox
         */
        void DoFormatStringL();
        
        /**
         * return formatted string needed for listbox for this item
         */
        TPtrC16 FormattedItemString();
        
    private://member variables.
        
        //Process Array
        CVIMPSTProcessArray& iParent;
        
        /**
         * Index of the item in engines model class
         */
         TInt iIndex;
         
         // Owns, user id of item.
         HBufC* iUesrId;
         
         // Owns, user id of item.
         HBufC* iDisplayName;
         
         // ETrue if it has a pending message, else EFalse.
         TBool iSMsgPending;
         // is conversation open
         TBool iIsConversationOpen;
         
         // for listbox
         HBufC* iFormattedString;
    };

#endif /*CVIMPSTPROCESSFRIENDREQUESTITEM_H_*/

