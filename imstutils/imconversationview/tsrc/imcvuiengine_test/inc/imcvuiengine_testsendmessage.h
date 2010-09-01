/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  imcvuiengine_test test module.
*
*/


#ifndef _IMCVENGINESENDMESSAGE_H_
#define _IMCVENGINESENDMESSAGE_H_

// INCLUDE FILES
#include <e32math.h>
#include <e32base.h>
#include "cimcvengine.h"
#include "mimcvenginemessagehandlerobserver.h"

class MIMCVEngineFactory;

class CSPSettings;

NONSHARABLE_CLASS(CIMCVUiEngine_TestSendMessage) : public CBase,
                                                   public MIMCVEngineMessageHandlerObserver
									   
    { 
    public:  // Constructors and destructor


        /**
        * Two-phased constructor.
        */
        static CIMCVUiEngine_TestSendMessage* NewL();

        /**
        * Destructor.
        */
        virtual ~CIMCVUiEngine_TestSendMessage();
        
        void CreateConnectionL();
        
        void ReleaseConnectionL();

    public: // MImCommandObserver
    
    	
        /**
        * StartNewConvesationL test method.
        * @return Symbian OS error code.
        */
        TInt StartNewConvesationL();
        
         /**
        * CloseConvesationL test method.
        * @return Symbian OS error code.
        */
        TInt CloseConvesationL();
        
        
        /**
        * SendNullMessageL test method.
        * @return Symbian OS error code.
        */
        TInt SendNullMessageL();
        
         /**
        * SendTextMessageL test method.
        * @return Symbian OS error code.
        */
        TInt SendTextMessageL();

        /**
        * SendSplCharMessageL test method.
        * @return Symbian OS error code.
        */
        TInt SendSplCharMessageL();
        
         /**
        * SendCosMessageL test method.
        * @return Symbian OS error code.
        */
        TInt SendCosMessageL();
        
        /**
        * DeactivateConvesationL test method.
        * @return Symbian OS error code.
        */
        TInt DeactivateConvesationL();
        
         /**
        * SendToInvaliedRecipientL test method.
        * @return Symbian OS error code.
        */
        TInt SendToInvaliedRecipientL();
        
        /**
        * TestRecievedMessageL test method.
        * @return Symbian OS error code.
        */
        TInt TestMultipleSendMessageL( );
        
         /**
        * TestSplCharRecieveMsgL test method.
        * @return Symbian OS error code.
        */
        TInt TestSendRecieveMsgL();

        /**
        * TestCosRecieveMsgL test method.
        */
        TInt TestSendFetchSendMsgL();
        
        TInt TestConnectionTerminationL();    
        
        TInt TestLoginStateL();
        
        TInt TestLoginUserIdL();
        
        TInt TestGetLanguageIdL();
        
        TInt TestGetBrandIdL();
        
        TInt TestChatInterfaceL();
            	
		TInt GetResult() const ;
		
		TInt TestMessageCoundL();
		
		TInt TestMessageUnreadCoundL();
		
		TInt TestSubscribePresenceL();
		
		TInt TestGetPresenceStatusL();
		
		TInt TestAppendInfoMessageL();
		
		
		TInt DeleteContextL();
		
		TInt CreateContextL();
		
		TInt TestCCHHandlerL();


		TInt ReadyForShutdownTestL();
		TInt ServiceNameTestL();
		TInt OpenChatsTestL();
		TInt GetServiceIdTestL();
		
		TInt TestClosAllContainerL();
		TInt TestDeleteChatL();
		TInt TestEditorMessageL();
		
		TInt TestConvertPresCacheEnum();

			
	public :
		
	    /**
	     * @see handle the presence change notification
	     * to handle the connection lost
	     */
	    void HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId ) ;

	    /**
	     * Handle message send operation completion.
	     * @param aResult result  to handle.
	     */
	    void HandleOperationCompletdL(TInt aserviceId, MIMCVEngineMessage::TMessagerType aType , TInt aResult)  ;

	    /**
	     * @see HandleConnectionTerminatedL
	     * to handle the connection lost
	     * @param aReason, reason report
	     */
	    void HandleConnectionEventL(TInt aServiceId, TIMCVConnectionState aState, TInt aReason )    ;
		
   	
    private:

        /**
        * C++ default constructor.
        */
        CIMCVUiEngine_TestSendMessage();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * read the service id
        */
        void ReadServiceDetalsL();
        

    private:   
    
        //OWN: Active scheduler wait for waiting the completion
        MIMCVEngineFactory* iEngineFactory;
        
        //not owns
    	CIMCVEngine* iEngine ;
    		
    	CActiveSchedulerWait iWait;
    	
		TInt iResult;
		
		TInt iServiceId;
		
			//owns : reference to setting
        CSPSettings* iSettings;
    };

#endif      // _IMCVENGINESENDMESSAGE_H_

// End of File
