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
* Description:  CVIMPSTProcessContactItem, CVIMPSTProcessContactListItem, CVIMPSTProcessOwnDataItem, 
*				 CVIMPSTProcessConversationItem
*                are the different types of array items stored in the MCAMainViewArrayPC
*
*/



#ifndef CVIMPSTPROCESSARRAYITEM_H
#define CVIMPSTPROCESSARRAYITEM_H
#include <e32base.h>

#include "tvimpstenums.h"
#include "cvimpstprocessarray.h"
//  FORWARD DECLARATION
class MVPbkContactLink;

// CLASS DECLARATION

/**
* Base class for cimcontactitem, cimcontactlistitem, cimowndataitem
* to be stored into an array of <CCAArrayItemPC> on the process component side.
* The UI components will query for each item in this heterogeneous array to display 
* information like contactlist, contact, conversation, groups etc in the listbox of 
* the main view class
* @lib vimpstcmdprocess.dll
* @since 5.0
*/
class MVIMPSTProcessArrayItem
    {
     
    public:
    
    /**
    * Returns the default type of the listbox item 
    * @return TEnumsPC::TItem: type of the listbox item as either EContactitem, EContactListItem etc
    */
    virtual TVIMPSTEnums::TItem Type() = 0;
    
    /**
    * Returns the name of the listbox item 
    * @return TPtrC: name of the listbox item to be displayed
    */
    virtual TPtrC GetItemNameText() = 0 ;
    
    /**
    * Sets the name of the listbox item 
    * @param aName: Name of the listbox item to be set
    * @return void
    */
    
    virtual void SetMsgPending(TBool aMsgPending) = 0;
    
    /**
     * gets the messages pending status of the contact item 
     * @return iSMsgPending: true if the contact has pending messages 
     * else false
     */
    virtual TBool IsMsgPending() = 0 ;
    
    /**
     * Sets the name of the listbox item 
     * @param aName: Name of the listbox item to be set
     * @return void
     */

    virtual void SetConversationOpen(TBool aConvOpen ) = 0;

    /**
     * gets the messages pending status of the contact item 
     * @return iSMsgPending: true if the contact has pending messages 
     * else false
     */
    virtual TBool IsConversationOpen() = 0 ;
        
    /**
    * Returns the array index of the listbox item on engine/storage side
    * @return TInt: index of the listbox item on engine/storage side
    */
    virtual TInt GetItemIndex() = 0;
    
    /**
    * return the VPBK Contact Link
    */
    virtual MVPbkContactLink* ContactLink() = 0;  
    
    
    /**
    * Sets the on engine/storage side index of the listbox item 
    * @param aIndex: index of the listbox item on engine/storage side
    * @return void
    */
    virtual void SetItemIndex(TInt aIndex) = 0;
    
    /**
    * Returns the user id of the listbox item 
    * @return TPtrC: user id of the listbox item.
    */
    virtual TPtrC GetItemUserId() = 0 ;
    /**
     * Returns the avatar index of the listbox item 
     * @return TInt: avatar index of the listbox item.
     */
    virtual TInt  AvatarIndex() = 0;
    
    /**
     * sets the avatar index of the listbox item 
     * @param aAvatarIndex: avatar index to be set 
     */
    virtual void  SetAvatarIndex(TInt aAvatarIndex ) = 0; 
    
    
    /**
     * returns status text of the contact
     */
    virtual const TDesC& StatusText() = 0;
    
    /**
     * returns online status of the contact
     */
    virtual TVIMPSTEnums::TOnlineStatus OnlineStatus() = 0;
    
    
    /**
     * return formatted string needed for listbox for this item
     */
    virtual TPtrC16 FormattedItemString() = 0;
    
   
    /**
     * virtual destructor.
     */
    virtual ~MVIMPSTProcessArrayItem(){};    
   
    
    };
    
/**
* CCAContactItemPC is a structure that will encapsualte information on a contact 
* item in the single array on the process component side.
* @lib vimpstcmdprocess.dll
* @since 5.0
*/    
class CVIMPSTProcessContactItem : public CBase,
                                  public MVIMPSTProcessArrayItem
    {
    
    public: // symbain constructor , destructor
		
		/**
		 *  Creates a new instance of this class.
		 * @param aContactId, reference to contactid.
		 * @param aContactLink, pointer to Contactlink.		 
		 * @return the ContactitemPC instance
		 */
    	static CVIMPSTProcessContactItem* NewL( CVIMPSTProcessArray& aParent, const TDesC& aContactName,
					    						const TDesC& aContactId, 
					    						MVPbkContactLink* aContactLink,
					    						const TDesC& aStatusText,
					    						TVIMPSTEnums::TOnlineStatus aOnlineStatus
					    						);
    	/**
    	* symbain way of construction
    	*/
    	void ConstructL(  const TDesC& aContactId, 
                          const TDesC& aContactName,
                          const TDesC& aStatusText,
                          TVIMPSTEnums::TOnlineStatus aOnlineStatus 
                          );
    	
    	/**
    	* destructor
    	*/
    	virtual ~CVIMPSTProcessContactItem();
    	
    private:

		/**
		* C++ default constructor
	    * @param aContactId, reference to contactid.
        * @param aContactLink, pointer to Contactlink.        
		*/
		CVIMPSTProcessContactItem( CVIMPSTProcessArray& aParent, MVPbkContactLink* aContactLink);

    public:
     //to be used later.
	    /**
	    * Returns the blocked status of the contact item 
	    * @return bool: true if the contact item is blocked else false
	    */
	    //TBool IsBlocked();
		   
		/**
		* Sets the blocked status of the contact item 
		* @param aBlockedStatus: true if the contact is blocked else false
		*/
	    //void SetBlockedStatus(TBool aBlockedStatus);

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
	    
    public: //From MImArrayItem
    
	    /**
	    * Returns the name of the contact item 
	    * @return TPtrC: name of the contact item to be displayed
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
	    * @return TInt, index
	    */
    	inline TInt GetItemIndex()   
    		{ 	
    		return iIndex;    
    		}
		
		/**
	    * Set the index of the contact item from engine side model
	    * @param TInt, aIndex
	    */
	    inline void SetItemIndex(TInt aIndex) 	
			{  
			iIndex  = aIndex;
			}
		
	   /**
    	* @returns the VPBK Contact Link
    	*/
		inline MVPbkContactLink* ContactLink()
			{
			return iContactLink;
			}
			
		
	    /**
	    * Returns the user id of the listbox item 
	    * @return TPtrC: user id of the listbox item.
	    */
	    TPtrC GetItemUserId();
	      
	    /**
	     * Returns the avatar index of the listbox item 
	     * @return TInt: avatar index of the listbox item.
	     */
	     inline TInt  AvatarIndex() 
	         {
	         return iAvatarIndex;
	         }
	    
	    /**
	     * sets the avatar index of the listbox item 
	     * @param aAvatarIndex: avatar index to be set 
	     */
	    inline void  SetAvatarIndex(TInt aAvatarIndex ) 
	        {
	        iAvatarIndex = aAvatarIndex;
	        
	        TRAP_IGNORE(DoFormatStringL());
	        }
	    
	    /**
	     * returns status text of the contact
	     */
	    const TDesC& StatusText();
	    
	    /**
	     * returns online status of the contact
	     */
	    TVIMPSTEnums::TOnlineStatus OnlineStatus();
	    
	    /*
	     * Format and Reformat the string for listbox
	     */
	    void DoFormatStringL();

	    /**
	     * return formatted string needed for listbox for this item
	     */
	    TPtrC16 FormattedItemString();	    
    			
    private:
	    
	    // Process Array
	    CVIMPSTProcessArray& iParent;
	    
  	   /**
	    * The name of the listbox item
	    */
	    HBufC* iItemNameTextContact;
	    
	    
	    /**
	    * The name of the listbox item
	    */
	    HBufC* iContactUserId;

	   /**
  	 	* Index of the item in engines model class
  	 	*/
  	 	TInt iIndex;
  	 	
  	 	//Not owned
  	 	MVPbkContactLink* iContactLink; 	 	
  	 	
  	 	// ETrue if it has a pending message, else EFalse.
  	 	TBool iSMsgPending;
  	 	
  	 	TBool iIsConversationOpen;
  	 	
 	 	// avatar index
  	 	TInt iAvatarIndex;
  	 	
  	 	// Status text Message
  	 	HBufC* iStatusText;
  	 	
  	 	// Stores the status of the contact
  	 	TVIMPSTEnums::TOnlineStatus iOnlineStatus;
  	 	
  	 	HBufC* iFormattedString;
	        
    };


/**
* CVIMPSTProcessContactListItem is a structure that will encapsualte information on a contact 
* list in the single array on the process component side.
* @lib vimpstcmdprocess.dll
* @since 5.0
*/ 
class CVIMPSTProcessContactListItem : public CBase,
                                      public MVIMPSTProcessArrayItem 
    {
    
    public:
       /**
        * Creates a new instance of this class.
		* @return the CVIMPSTProcessContactListItem instance
		*/
    	static CVIMPSTProcessContactListItem* NewL(CVIMPSTProcessArray& aParent);
    	
       /**
    	* second phase constructor
    	*/
    	void ConstructL();
    	
       /**
    	* destructor
    	*/
    	virtual ~CVIMPSTProcessContactListItem();

	private:

	   /**
		* C++ default constructor
		*/	
   		CVIMPSTProcessContactListItem(CVIMPSTProcessArray& aParent);
   		
    public:
    
	    /**
	    * Returns the expanded or collapsed state information of the contact list 
	    * @return bool, true if the contact list is collapsed else return false
	    */
	    TBool IsCollapsed();
	    
	   	    
	    /**
		* Sets the collapsed/expanded state of the contact list
		* @param aCollapsed, true if the contact list item is collapsed else false
		*/
		void SetCollapsed(TBool aCollapsed);
	   	    
	public: //From CImArrayItem

		/**
		* Returns the name of the contact list item 
		* @return TPtrC, name of the contact item to be displayed
		*/
		TPtrC GetItemNameText();

		/**
		* Sets the name of the contact list item 
		* @param aName: Name of the contact list item to be set
		* @return void
		*/
		void SetItemNameText(const TDesC&);    

		/**
		* Returns the default type of the contact list item 
		* @return TEnumsPC::TItem: type of the contact list item as either 
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
			return 0;
			}
		
       /**
        * Returns the user id of the listbox item 
        * @return TPtrC: user id of the listbox item.
        */
        inline TPtrC GetItemUserId()
            {
            return KNullDesC();
            }
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
        void SetMsgPending(TBool /*aMsgPending*/)
            {
            // NA
            }
        /**
         * Sets the messages pending status of the contact item 
         * @param aPendingMessageStatus: true if the contact has pending messages 
         * else false
         */
        TBool IsMsgPending()
            {
            return EFalse;
            }
        /**
         * Sets the name of the listbox item 
         * @param aName: Name of the listbox item to be set
         * @return void
         */

        void SetConversationOpen(TBool /*aConvOpen*/ )
            {
            // NA
            }

        /**
         * gets the messages pending status of the contact item 
         * @return iSMsgPending: true if the contact has pending messages 
         * else false
         */
        TBool IsConversationOpen() 
            {
            return EFalse;
            }
        
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
        
    private:
        
        // Process Array
        CVIMPSTProcessArray& iParent;
    
    	/**
	 	* collapsed/expanded status of the contact list
	    */
		TBool iCollapsedStatus;
		
			
		/**
	 	* Total no. of contacts in the contact list
	    */
	    TInt  iTotalContacts;

		/**
	 	* name of the contact list followed by 
	 	* the (No. of Online Contacts/ Total contacts) in that contact list
	    */
	    HBufC* iText;
	    
       /**
  	 	* Index of the item in engines model class
  	 	*/
  	 	TInt	iIndex;
     
                
        /**
        * The name of the listbox item
        * Till the contalist is supported in futuer, 
        * iItemNameTextList is a pointer.
        */
        HBufC* iItemNameTextList; 
    };
    
/**
* CVIMPSTProcessOwnDataItem is a structure that will encapsualte information on Own data 
* item in the single array on the process component side.
*/   
 class CVIMPSTProcessOwnDataItem : public CBase,
                                   public MVIMPSTProcessArrayItem
    {
    	
    public:
	   /**
	    * Creates a new instance of this class.
		* @param aContactId, id of the contact		
		*/
    	static CVIMPSTProcessOwnDataItem* NewL(CVIMPSTProcessArray& aParent, TDesC& aContactId);
    	
       /**
    	* second phase constructor
    	*/
    	void ConstructL();
    	
       /**
    	* destructor
    	*/
    	virtual ~CVIMPSTProcessOwnDataItem();

   private:

       /**
        * symbian default constructor
        * @param aContactId, id of the contact        
        */
	
        CVIMPSTProcessOwnDataItem(CVIMPSTProcessArray& aParent,TDesC& aContactId);
		    
	public: //From CImArrayItem

		/**
		* Returns the name of the contact list item 
		* @return TPtrC, name of the contact item to be displayed
		**/
		TPtrC GetItemUserId();

	
		/**
		* Returns the default type of the contact list item 
		* @return TEnumsPC::TItem, type of the contact list item as either 
		* EContactitem, EContactListItem etc
		*/
		TVIMPSTEnums::TItem Type();

     	/**
	    * Get the index of the contact item from engine side model
	    * @return TInt, index
	    */
	    inline TInt GetItemIndex()   
    		{ 	
    		return iIndex;    
    		}
		
		/**
	    * Set the index of the contact item from engine side model
	    * @param TInt, aIndex
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
			return 0;
			}
    	
        /**
        * Returns the user id of the listbox item 
        * @return TPtrC: user id of the listbox item.
        */
        inline TPtrC GetItemNameText()
            {
            return KNullDesC();
            }
        /**
         * Returns the avatar index of the listbox item 
         * @return TInt: avatar index of the listbox item.
         */
        inline TInt  AvatarIndex() 
            {
            return iAvatarIndex;
            }

        /**
         * sets the avatar index of the listbox item 
         * @param aAvatarIndex: avatar index to be set 
         */
        inline void  SetAvatarIndex(TInt aAvatarIndex ) 
            {
            iAvatarIndex = aAvatarIndex;
            
            TRAP_IGNORE(DoFormatStringL());
            }
        /**
         * returns the mimetype
         * @return returns the mimetype
         */
        inline const TDesC8& MimeType()
            {
            if( iMimeType )
                {
                return *iMimeType;
                }
            else
                {
                return KNullDesC8();
                }    
            }
        /**
         * sets the mimetype
         * @param  the mimetype to be set
         */
        inline void SetMimeType( const TDesC8& aMime)
            {
            delete iMimeType; 
            iMimeType = NULL;
            iMimeType = aMime.Alloc();
            } 
        void SetMsgPending(TBool /*aMsgPending*/)
            {
            // NA
            }
        /**
         * Sets the messages pending status of the contact item 
         * @param aPendingMessageStatus: true if the contact has pending messages 
         * else false
         */
        TBool IsMsgPending()
            {
            return EFalse;
            }
        /**
         * Sets the name of the listbox item 
         * @param aName: Name of the listbox item to be set
         * @return void
         */

        void SetConversationOpen(TBool /*aConvOpen*/ )
            {
            // NA
            }

        /**
         * gets the messages pending status of the contact item 
         * @return iSMsgPending: true if the contact has pending messages 
         * else false
         */
        TBool IsConversationOpen() 
            {
            return EFalse;
            }
        
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
        
    private:
       	    
        //Process Array
        CVIMPSTProcessArray& iParent;
        
	    /**
  	 	* Index of the item in engines model class
  	 	*/
  	 	TInt	iIndex;
     
        /**
        * The name of the listbox item
        */
        TDesC& iOwnDataId;
        
        
        // avatar index	
    	TInt iAvatarIndex;
    	// owned mimetype
    	HBufC8* iMimeType;
    };



/**
 * unknown contact item
 * Responsible for handling feriend requests.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class CVIMPSTProcessUnknownContactItem :  public CBase,
                                          public MVIMPSTProcessArrayItem
    {
    public: // symbain constructor , destructor
        
        /**
        * Returns the instance of tis class.
        * @param aBuddyId, buddyId id.
        * @return CVIMPSTProcessFriendRequestItem, instance of this class.
        */
        static CVIMPSTProcessUnknownContactItem* NewL(CVIMPSTProcessArray& aParent, const TDesC& aBuddyId);
        
        /**
         * symbain way of construction
         * @param aBuddyId, requestor id.
         */
        void ConstructL(const TDesC& aBuddyId);
        
        /**
         * destructor
         */
        virtual ~CVIMPSTProcessUnknownContactItem();
        
    private:

         /**
          * C++ default constructor
          */
        CVIMPSTProcessUnknownContactItem(CVIMPSTProcessArray& aParent);
        
    public : // new method
        
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
         * Sets the name of the listbox item 
         * @param aName: Name of the listbox item to be set
         * @return void
         */

        void SetConversationOpen(TBool aConvOpen );
        
        /**
         * gets the messages pending status of the contact item 
         * @return iSMsgPending: true if the contact has pending messages 
         * else false
         */
        TBool IsConversationOpen(); 
        
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
        
        // Process Array
        CVIMPSTProcessArray& iParent;
        
        /**
         * The name of the listbox item (owned).
         */
         HBufC* iItemNameText;          
             
        /**
         * Index of the item in engines model class
         */
         TInt iIndex;
         
         // Owns, user id of item.
         HBufC* iUesrId;
         
         // if any message is pending
         
         TBool iSMsgPending;
         
         TBool iIsConversationOpen;
         
         HBufC* iFormattedString;
    };
    
#endif  //CVIMPSTPROCESSARRAYITEM_H
