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
* Description:  Openchats 
*
*/


#ifndef CIMCVENGINEOPENCHATS_H
#define CIMCVENGINEOPENCHATS_H

//includes
#include <e32std.h>
#include <e32base.h>


#include "mimcvengineopenchats.h"
#include "mimcvenginenewmsgobserver.h"
#include "mimcvenginechatlistobserver.h"


//forward declarations
class CCAConversationItemPC;
class CIMCVEngineMessageHandler;
class MIMCVEngineChatInterface;
class MIMCVEngineMessageReadInterface;


// CLASS DECLARATION

/**
 *  CIMCVEngineOpenChats
 *
 *  @lib imcvuiengine.dll
 *  @since 5.0
 */
class CIMCVEngineOpenChats : public CBase,
								 public MIMCVEngineOpenChats,
								 public MIMCVEngineNewMsgObserver,
								 public MIMCVEngineChatListObserver
								 
    {    

	public: // From MIMCVEngineOpenChats
		
		/**
         * @see MIMCVEngineOpenChats
         */
		TPtrC GetDisplayNameL(TInt aIndex) const;

		
		/**
         * @see MIMCVEngineOpenChats
         */
		const TDesC& GetTargetIdL(TInt aIndex) const;
		
		
		/**
         * @see MIMCVEngineOpenChats
         */
		TInt Count() const;
		
		/**
         * @see MIMCVEngineOpenChats
         */		
		void SetActiveItemL( const TInt aIndex );
		
		/**
         * @see MIMCVEngineOpenChats
         */		
		const TDesC& GetActiveItem() const;
		
		/**
         * @see MIMCVEngineOpenChats
         */	    
	    TBool IsMsgPendingL( const TInt aIndex ) const;
	    
	    /**
         * @see MIMCVEngineOpenChats
         */	    
	    TInt FindIndexL(const TDesC& aItem);
	    
	    /**
         * @see MIMCVEngineOpenChats
         */
	    void RegisterObserver(MIMCVEngineOpenChatsObserver* aChatObserver);
	    
	    
	    /**
         * @see MIMCVEngineOpenChats
         */
	    void UnRegisterObserver();
	    
	    void ResetL();
	    
	private:
		
	    /** 
		 * Inserts the chat container into the Conversations List
		 * @param aContainerInfo - Conversation/Chat Container Info
		 */	    
	    void Insert( MIMCVEngineMessageReadInterface* aContainerInfo );
	    
	    /** 
		 * Delets the chat container from the Conversations List
		 * @param aContainerInfo - Conversation/Chat Container Info
		 */
	   	void RemoveL( MIMCVEngineMessageReadInterface& aContainerInfo );	    
	    
	        
	private:
	
		/**
         * @see MIMCVEngineChatListObserver
         */	
		void HandleChatListEvent( TInt aServiceId , TChatListEventType aEvent, 
						MIMCVEngineMessageReadInterface* aContainerInfo
						) ;
		    
    public:                     
		
		/**
		* NewL - returns the instance to CIMCVEngineOpenChats
		*/
		static CIMCVEngineOpenChats* NewL(CIMCVEngineMessageHandler& aMessageHandler, 
										MIMCVEngineChatInterface& aChatInterface,
										TInt aServiceId );
										
		
		/**
		* Destructor
		*/
		virtual ~CIMCVEngineOpenChats();


	private:
		/**
         * @see MIMCVEngineNewMsgObserver
         */	
		void HandleChatEventL( TChatEventType aEvent, MIMCVEngineMessage* aMessage = NULL ) ;
		
	private: //Two phased ctor
		
		/**
		*CIMCVEngineOpenChats - ctor
		*/		
		CIMCVEngineOpenChats(CIMCVEngineMessageHandler& aMessageHandler,
							MIMCVEngineChatInterface& aChatInterface);

		
		/**
		*ConstructL 
		*/
		void ConstructL( TInt aServiceId );
    
    private: //Data 
	
		//Doesn't own
		RPointerArray <MIMCVEngineMessageReadInterface> iOpenChatsArray;
		
		//holds the index of the active tab item
		TInt iActiveItem;		
		
		CIMCVEngineMessageHandler& iMessageHandler;	
		
		//Doesnt Own
		MIMCVEngineOpenChatsObserver* iChatObserver;
		
		//Doesnt Own- Chat Interface
		MIMCVEngineChatInterface& iChatInterface;			
		
    };

#endif      // CIMCVENGINEOPENCHATS_H

// End of File
