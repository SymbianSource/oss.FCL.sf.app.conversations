/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: t_vimpstengineimsubservice.h
*
*/
#ifndef _T_VimpstEngineImSubService_H_
#define _T_VimpstEngineImSubService_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include "mvimpstengineserviceconnectioneventobserver.h"
#include "mvimpstengineimsubserviceeventobserver.h" // MVIMPSTEngineIMSubServiceEventObserver
#include "mvimpstenginecchuieventobserver.h"

class CVIMPSTEngineIMSubService;
class CVIMPSTEngineCchHandler;
class CVIMPSTEngineServiceTableFetcher;
class T_VimpstEngineImSubService : 
    public CEUnitTestSuiteClass,
    public MVIMPSTEngineServiceConnectionEventObserver,
    public MVIMPSTEngineIMSubServiceEventObserver
    {
    public:

        static T_VimpstEngineImSubService* NewLC();
        ~T_VimpstEngineImSubService();

    public:     // From observer interface
           
        // From MVIMPSTEngineServiceConnectionEventObserver
        void HandleServceConnectionEventL( );
        
        // From MVIMPSTEngineIMSubServiceEventObserver
        void  HandleChatMessageEventL(TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aSender);
        
    private:

        void ConstructL();
        void SetupL();
        void Teardown();
        
        // Help methods
        TInt CreateSpEntryL( TInt aServiceId );

        // Test methods
        void T_NewLL();
        void T_RegisterChatObserverL();
        void T_UnRegisterChatObserverL();
        void T_HandleIMCacheEventLL();
        void T_GetUnreadCountL();
        void T_SubServiceStateL();
        void T_IsConversationExistLL();        
        void T_ResolveServiceStateLL();
        void T_TypeL();
        void T_CchEventOccuredLL();
        void T_DoHandleCchErrorLL();
        void T_CreateIMCacheAccessorLL();
        void T_ReleaseIMCacheAccessorL();        
        void T_IsConversationDoesntExistLL();
        
        void T_CloseConversationExisitingConversationLL();
		void T_CloseConversationNonExisitingConversationLL();
		
		
		void T_IsConversationExistNoIMCacheAccessorL();
		void T_CloseConversationNoIMCacheAccessorL();
		void T_GetUnreadCountNoIMCacheAccessorL();
		
		void T_RegisterNULLChatObserverL();
		void T_UnRegisterNULLChatObserverL();
		
		
		void T_RegisterChatObserverTwiceL();
		void T_UnRegisterChatObserverTwiceL();
		
		void T_HandleIMCacheEventWithRegisteredObserverL();
        

    private:

        T_VimpstEngineImSubService() {};

    private:

        CVIMPSTEngineCchHandler* iCchHandler;
        CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
        CVIMPSTEngineIMSubService* iImSubService;
        MVIMPSTEngineCchUiEventObserver* iCchUiEventObserver;
        
        TInt iTestServiceId;
        
        TBool iChatMsgReceived;
        TBool iAddRequestReceived;
         
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_VimpstEngineImSubService_H_

// end of file
