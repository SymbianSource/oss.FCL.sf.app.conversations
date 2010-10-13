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
* Description: t_vimpstenginepresencesubservice.h
*
*/
#ifndef _T_VimpstEnginePresenceSubService_H_
#define _T_VimpstEnginePresenceSubService_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include "mvimpstengineserviceconnectioneventobserver.h"
#include "cvimpstenginesessioncntxtobserver.h" // MVIMPSTEngineSessionCntxtObserver
#include "mvimpstenginecchuieventobserver.h"
class CustomCleanupResetAndDestroy;
class CVIMPSTEnginePresenceSubService;
class CVIMPSTEngineServiceTableFetcher;
class CVIMPSTEngineCchHandler;
class CVIMPSTEngineSessionCntxtObserver;
class MVIMPSTEngineSessionCntxtObserver;
class MPresenceCacheReader2;
class CIMCacheFactory;
class MIMCacheUpdater;
class CContactIdArray;
class CContactDatabase;
class MPresentityGroupContentEvent;
class CVIMPSTEngineSubServiceStateListener;
class CustomCleanupResetAndDestroy;

class T_VimpstEnginePresenceSubService : 
    public CEUnitTestSuiteClass,
    public MVIMPSTEngineServiceConnectionEventObserver
    {
    public:

        static T_VimpstEnginePresenceSubService* NewLC();
        ~T_VimpstEnginePresenceSubService();

    public:     // From observer interface
           
        // From MVIMPSTEngineServiceConnectionEventObserver
        void HandleServceConnectionEventL();
        
    private:

        void ConstructL();
        void SetupL();
        void Teardown();
        
        // Help methods
        TInt CreateSpEntryL( TInt aServiceId );

        // Test methods
        void T_NewLL();
        void T_SubServiceStateL();
        void T_TypeL();
        void T_Enabled();
        void T_RetrieveSubscribedListLL();      
        void T_SubscribePresenceOfSingleContactL();  
        void T_UnSubscribePresenceOfSingleContactLL();   
        void T_DoHandlePresentityPresenceEventLL();     
        void T_DoHandlePresentityGroupContentEventLL();
        void T_DoHandlePresenceGrantRequestListEventLL();
        void T_FormatPresenceUriLCL();
        void T_ResolveServiceStateLL();
        void T_CchEventOccuredLL();
        void T_DoHandleCchErrorLL();
        void T_PublishOwnPresenceLL();
        void T_ConvertPresenceStatus();
    //    void T_FetchPresenceFormCacheL();
        void T_HandlePresenceReadLL();
    //    void T_SubscribeToPresenceCacheLL();
        void T_HandlePresenceNotificationLL();
        void T_UnSubscribeForAuthorizationLL();
        void T_SubscribeForAuthorizationLL();
        void T_SendPresenceGrantPresentityLL();
        void T_HandleSessionContextEventLL();
        void T_HandleListEventCompleteLL();
        void T_HandleXimpRequestCompleteLL();
        void T_StoreToVirtualStoreLL();
        void T_IsChangeOwnStatusSupportedL() ;
        void T_IsStatusMsgSupportedL();
        void T_SubscribePresenceForAllLL();        
        void T_UnSubscribePresenceForAllLL();
        void T_ConvertXimpToClientPresenceStatusL();
      //  void T_RegisterPresenceEventObserverL();
        void T_UnRegisterPresenceEventObserverL();
        void T_DoHandlePresenceBlockListEventL();
        void T_AddToBlockListL();
        void T_AddToBlockListL_NoUserid();
        void T_AddToBlockListL_NoDomainNameL();
        void T_RemoveFromBlockListL();
        void T_RemoveFromBlockList_NoUseridL();
        void T_RemoveFromBlockList_NoDomainNameL();
        void T_RemoveFromBlockListMgr();
        void T_GetKeyFiledsAndValuesStatusPendingL();
        void T_GetKeyFiledsAndValuesStatusUnknownL();
        void T_GetKeyFiledsAndValuesStatusBlockedL();
		void T_FetchBlockedListFromServer();
    private:

        T_VimpstEnginePresenceSubService() {};

    private:

        CVIMPSTEngineCchHandler* iCchHandler;
        CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
        MVIMPSTEngineCchUiEventObserver* iCchUiEventObserver;
        CVIMPSTEngineSessionCntxtObserver* iSessionCntxtObserver;
        CVIMPSTEnginePresenceSubService* iPresenceSubService;
        
        TInt iTestServiceId;
         
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
