/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Open Conversations
*
*/



#ifndef MIMCVENGINEOPENCHATS_H_H
#define MIMCVENGINEOPENCHATS_H_H


//forward declarations


enum TOpenChatsEventType
    {
   	EUnReadCountChanged,
   	ENewChat,
   	EChatClosed,
   	EChatAllClosed,
   	EReadCountChanged  
    };
     

//Observer Class
class MIMCVEngineOpenChatsObserver 
	{
	public:
		
		/**
		 * Handle Chnage events for Open Conversations
		 * @param aIndex Index of the Item Change.
		 * @param aChangeType, Change Type listed above 
		 */
		virtual void HandleChangeL(TInt aServiceId, TInt aIndex, TOpenChatsEventType aChangeType) = 0;
		
      	/*set the status msg.When uses is in conversation one user and receive
      	*msg from some other user.
      	*@param aIndex Index of the Item which sent message.
		*/
      	virtual void SetStatusMsgL(TInt aServiceId, TInt aIndex,const TDesC & aMsg)= 0;
      	
		/**
        * Virtual destructor
        */      		
		virtual ~MIMCVEngineOpenChatsObserver() 
		{
			
		};
	};



// CLASS DECLARATION

/**
 *  Interface for Open Conversations
 *
 *  @lib imcvuiengine.dll
 *  @since 5.0
 */
class MIMCVEngineOpenChats 
    {    
    
    public:           
       
	   /**
        * GetDisplayNameL: returns the Identification       
        * @param aIndex - Index of the item for which Identification 
        *                 is required
        * @return TPtrC - Identification
        */
		virtual TPtrC GetDisplayNameL(TInt aIndex) const = 0;


	   /**
        * GetTargetIdL: returns the Conversation Party ID      
        * @param aIndex - Index of the item 
        * @return Conversation Party ID
        */
		virtual const TDesC& GetTargetIdL(TInt aIndex) const = 0;
	   
	   
	   /**
        * Count: Returns the number of items              
        * @return TInt - Total number of openchats item
        */
		virtual TInt Count() const = 0;
		
		
	   /**
        * SetActiveItemL: Sets the item as the current active one       
        * @param aIndex - Index of the item
        * @return void
        */		
		virtual void SetActiveItemL( const TInt aIndex ) = 0;
		
		
		/**
        * GetActiveItem: Gets the current active conversation        
        * @return Active Conversation Party Id
        */
		virtual const TDesC& GetActiveItem() const = 0;
		
		/**
	    * Returns the pending messages status of the contact item 
	    * @param aIndex - Index of the item
	    * @return bool: true if the conversation item has pending messages
	    * else return false
	    */	    
	    virtual TBool IsMsgPendingL( const TInt aIndex ) const = 0;
	   
	   	/**
	    * Find the index of the item in the Conversation List 
	    * @param aItem - Conversation Party Id	    
	    * @return aIndex - Index of the item	    
	    */
	    virtual TInt FindIndexL(const TDesC& aItem) = 0;
	    
	    /**
		 * Register observer for chat events. Remember to unregister 
		 * when not needed
		 * @param aObserver Observer which is registered.		 
		 */
	    virtual void RegisterObserver(MIMCVEngineOpenChatsObserver* aChatObserver) = 0;
	    
	    
	    /**
		 * Unregister observer for chat events.		 
		 *
		 */		 
	    virtual void UnRegisterObserver() = 0;
	    
	    virtual void ResetL() = 0;
      	
      	
	   /**
        * Virtual destructor
        */      		
		virtual ~MIMCVEngineOpenChats() 
		{			
		};

		
    };

#endif      // MIMCVENGINEOPENCHATS_H_H

// End of File
