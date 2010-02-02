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
* Description:  M-Class that capsulates IM sub service API's
*
*/


#ifndef MVIMPSTENGINEIMSUBSERVICE_H
#define MVIMPSTENGINEIMSUBSERVICE_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"
#include "mvimpstenginesubservice.h"
#include <imcachedefs.h>

//FORWARD DECLARATION
class MVIMPSTEngineIMSubServiceEventObserver;

//CLASS DECLARATION
/**
 *  
 *  This class list the M-Class for IM subservices
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
//Class declaration
class MVIMPSTEngineIMSubService: public MVIMPSTEngineSubService
	{
	
	public:
	
		/**
         * Casts MVIMPSTEngineIMSubService to this interface.
         *
         * @precond aSubService.Type() == TVIMPSTEnums::EIM
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aSubService    Base Class .
         * @return Casted inteface for the IM SubService
         */
        IMPORT_C static MVIMPSTEngineIMSubService& Cast(
                MVIMPSTEngineSubService& aSubService);

        
        /**
         * Casts constMVIMPSTEngineIMSubService to this interface.
         *
         * @precond aSubService.Type() == TVIMPSTEnums::EIM
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aSubService    Base Class .
         * @return Casted inteface for the IM SubService
         */        
        IMPORT_C static const MVIMPSTEngineIMSubService& Cast(
                const MVIMPSTEngineSubService& aSubService);
	
	
		/**
		* RegisterChatObserver
		* The observers that have implemented MVIMPSTEngineIMSubServiceEventObserver can use this API
		* to register themselves to receive IM SubService (Conversation) related events
		* @param aObserver - Object that implements the observer class MVIMPSTEngineIMSubServiceEventObserver that 
		* needs to be added.
		*/
	  	virtual void RegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver) = 0;
	  	
		/**
		* UnRegisterChatObserver
		* The observers that have implemented MVIMPSTEngineIMSubServiceEventObserver can use this API
		* to unregister themselves from IM SubService (Conversation) related events
		* @param aObserver - Object that implements the observer class MVIMPSTEngineIMSubServiceEventObserver that
		* needs to be removed.
		*/
	  	virtual void UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver) = 0;
	  	
		/**
		* GetUnreadCountL
		* API used to retreive the Unread Msg Count for a Buddy
		* @param aBuddyId, id of the conversation party. 
		*/
	  	virtual TInt GetUnreadCountL(const TDesC& aBuddyId) = 0;
	
	
		/**
         * check if already conversation opened
         * @param aBuddyId, the selected user id with whom conversation is started
         * @return TBool ETrue if already exist else EFlase.
         */
        virtual TBool IsConversationExistL(const TDesC& aBuddyId) const = 0;
        
        /**
        * Closes conversation with the selected user        
        * @param aBuddyId, id of the conversation party.           
        */
        virtual void CloseConversationL( const TDesC& aBuddyId ) = 0;
	    						   
        /**
        * get all conversation with the selected service id        
        * @param aBuddyId, id of the conversation party.           
        */
      
        virtual RArray<SIMCacheChatItem> GetOpenChatListL() = 0 ;
        
	   /**
		* Destructor
		*/		
		virtual ~MVIMPSTEngineIMSubService() 
		{}
		
	};

#endif //MVIMPSTENGINEIMSUBSERVICE_H


