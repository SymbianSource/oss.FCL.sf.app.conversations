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
* Description: t_vimpstenginevoipsubservice.h
*
*/
#ifndef _T_VimpstEngineVoipSubService_H_
#define _T_VimpstEngineVoipSubService_H_

//  EXTERNAL INCLUDES
#include <platform/digia/eunit/ceunittestsuiteclass.h>
#include "mvimpstenginecchuieventobserver.h"
#include "mvimpstengineserviceconnectioneventobserver.h"

class CVIMPSTEngineVOIPSubService;
class CVIMPSTEngineCchHandler;
class CVIMPSTEngineServiceTableFetcher;
//class CVIMPSTEngineCchUiEventObserver;

class T_VimpstEngineVoipSubService : 
    public CEUnitTestSuiteClass,
    public MVIMPSTEngineServiceConnectionEventObserver
    {
    public:

        static T_VimpstEngineVoipSubService* NewLC();
        ~T_VimpstEngineVoipSubService();

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
        void T_CchEventOccuredLL();
        void T_DoHandleCchErrorLL();
        void T_ResolveServiceStateL();
        void T_SubServiceStateL();
        void T_TypeL();

    private:

        T_VimpstEngineVoipSubService() {};

    private:

        CVIMPSTEngineCchHandler* iCchHandler;
        MVIMPSTEngineCchUiEventObserver* iCchUiEventObserver;
        CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
        CVIMPSTEngineVOIPSubService* iVoIPSubService;
        
        TInt iTestServiceId;
         
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
