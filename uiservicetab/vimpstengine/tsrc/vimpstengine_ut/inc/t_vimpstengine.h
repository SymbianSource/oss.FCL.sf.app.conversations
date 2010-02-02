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
* Description: t_vimpstengine.h
*
*/
#ifndef _T_VimpstEngine_H_
#define _T_VimpstEngine_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

#include "mvimpstengineservicestateeventobserver.h"

//Forward declarations
class CVIMPSTEngine;
class CVIMPSTEngineServiceTableFetcher;

class T_VimpstEngine : public CEUnitTestSuiteClass, public MVIMPSTEngineServiceStateEventObserver
    {
    public:

        static T_VimpstEngine* NewLC();
        ~T_VimpstEngine();

    private:

        void ConstructL();
        void SetupL();        
        void Teardown();

        // From observers
        void HandleServiceEventL( 
            TVIMPSTEnums::TVIMPSTRegistrationState aState, 
            TInt aServiceError);
        
        // Help methods
        TInt CreateSpEntryL( TInt aServiceId );

        // Test methods
        void T_NewLL();
        void T_LoginL();
        void T_LogoutL();
        void T_ServiceIdL();
        void T_ServiceNameL();
        void T_ServiceStateL();
        void T_GetBrandInfoLL();
        void T_IsSubServiceSupportedLL();
        void T_IsSubServiceEnabledL();
        void T_ContactStoreIdLL();
        void T_RegisterServiceSessionObserverLL();
        void T_UnRegisterServiceSessionObserverLL();
        void T_IntializeStorageLL();
        void T_UnIntializeStorageLL();
        //void T_SetUserNameL();
        //void T_SetPasswordL();
    //    void T_UserNameLL();
    //    void T_PasswordLL();
        void T_SubServiceL();
        void T_ExtentionFeaturesL();
        void T_AddExtentionFeaturesLL();
        void T_RemoveExtentionFeaturesL();
        void T_HandleServceConnectionEventLL();
        void T_ParseGetServiceStateL();
        void T_HandleContactFetchedLL();
      //  void T_HandleContactAddedLL();
      //  void T_HandleContactDeletedLL();
        void T_CreateExtentionFeaturesLL();
        void T_SetPassword();
        void T_EmptySetPassword();
       // void T_SetUserNameL();
     //   void T_EmptySetUserNameL();
     //   void T_WithDomainNameSetUserNameL();
        void T_BlockNotSupported();
        void T_BlockSupported();
        void T_FetchBlockedListL();
        void T_RegisterBlockedListObserver();
		
    private:

        T_VimpstEngine() {};

    private:
        CVIMPSTEngine* iEngine;
        CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
        
        TInt iTestServiceId;
        
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_VimpstEngine_H_

// end of file
