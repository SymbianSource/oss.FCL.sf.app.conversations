/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message flow handling
*
*/



#ifndef _CIMCVAPPMESSAGEFLOWHANDLER_H
#define _CIMCVAPPMESSAGEFLOWHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "mimcvenginemessageobserver.h"
#include "mimcvenginechatlistobserver.h"

class MIMCVEngineMessageReadInterface;
class MIMCVEngineMessageContainer;
class CIMCVEngineContextObserver;
class CIMCVEngine;
// CLASS DECLARATION

/**
*  Flow controller for messages
*
*  @imcvuiapp.exe
*  @since Series 60 5.0
*/
class CIMCVAppMessageFlowHandler : public CTimer, 
								public MIMCVEngineMessageObserver,
								public MIMCVEngineChatListObserver
    {
    
    
    
    public:
    
        /**
         * Should we fetch new messages or not
         * @param aFetch ETrue if this chat is in foreground and should 
         *               receive messages, EFalse otherwise.
         */
        void FetchMessages( TBool aFetch );        
        
        
        
    protected: // Functions from MIMCVEngineMessageObserver

        /**
		 * Handle events.
		 * @param aEvent Event to handle.
		 * @param aIndex, Index of message which is affected by event.
		 */
		void HandleMessageEvent( TMessageEventType aEvent, TInt aIndex );
		
    private:
    	/*
    	* See MIMCVEngineChatListObserver
    	*/
    	void HandleChatListEvent(TInt aServiceId, TChatListEventType aEvent, 
								MIMCVEngineMessageReadInterface* aContainerInfo  ) ;
    	
    protected:  // Functions from CTimer

		/**
		 * @see CTimer
		 */
        void RunL();
        
		/**
		 * @see CActive
		 */
        TInt RunError( TInt aError );
		
	

    private:  // New functions

        /**
         * Get flow control value from settings
         */ 
        void UpdateTimeIntervalL();

        /**
         * Is opening on going or not.         
         * @return ETrue if opening on going, EFalse otherwise.
         */
        TBool IsOpening() const;
        
     
        
    private:

        /**
        * C++ default constructor.
        */
        CIMCVAppMessageFlowHandler(MIMCVEngineMessageContainer& aMessageContainer, 
                				   MIMCVEngineMessageReadInterface& aReadInterface,
                				   CIMCVEngine& aActiveEngine );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TBool aScrollOver );
        
	public:  // Constructors and destructor        
	
		/**
         * Two-phased constructor.         
         * @param aMessageContainer Interface to UI side message container
         * @param aReadInterface Message read interface        
         * @param aScrollOver Is scroll over required 
         *                             or not.
         */
        static CIMCVAppMessageFlowHandler* NewL( 
                            MIMCVEngineMessageContainer& aMessageContainer, 
                            MIMCVEngineMessageReadInterface& aReadInterface,
                            CIMCVEngine& aActiveEngine,
                            TBool aScrollOver );
                            
        /**
         * Destructor.
         */
        virtual ~CIMCVAppMessageFlowHandler();
                

    private:    // Data
    
        // Interface for UI side message container
        // Not owned
        MIMCVEngineMessageContainer& iMessages;
        
        // Interface for fetching messages
        // Not owned
        MIMCVEngineMessageReadInterface& iReadInterface;    
        
        CIMCVEngine& iActiveEngine;
        
        // time interval for receiving messages
        TTimeIntervalMicroSeconds32 iTimeInterval;
        
        // Is chat deleted already
        TBool iChatDeleted;
        
        // Should we fetch new messages
        TBool iFetchMessages;
        
        // Index for fetching messages
        TInt iMsgIndex;
        
        // Initial count for messages
        TInt iInitialMsgCount;
        
        // Is new message received while opening the chat
        TBool iNewMsgWhileOpening;
        
        // Flag is read from CR variation in class construction
        // ETrue in releases >= 3.2 
        // EFalse in releases < 3.2
        // Set to EFalse if this handler is used to handle
        // recorded chat
        TBool iBgOpeningMode;
        
        TInt iAddedUnreadMsgs;
        
        
    };


#endif      // _CIMCVAPPMESSAGEFLOWHANDLER_H

// End of File
