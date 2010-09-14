/*
*  dllmain.cpp  Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: dllmain.cpp
*
*/
#include <platform/digia/eunit/CEUnitTestSuite.h>

#include "t_vimpstengine.h"
#include "t_vimpstenginevoipsubservice.h"
#include "t_vimpstengineimsubservice.h"
#include "t_vimpstenginecchhandler.h"
#include "t_vimpstengineservicetablefetcher.h"
#include "t_vimpstenginesessioncntxtobserver.h"
#include "t_vimpstenginerequest.h"
#include "t_vimpstenginerequestmapper.h"
#include "t_vimpstenginecontactmgmtextention.h"
#include "t_vimpstenginesearchmgrextention.h"
#include "t_vimpstenginepresencesubservice.h"

EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* suite = CEUnitTestSuite::NewLC( _L("vimpstengine_ut") );
    
   suite->AddL( T_VimpstEngine::NewLC() );
    CleanupStack::Pop();
     
    suite->AddL( T_VimpstEngineVoipSubService::NewLC() );
    CleanupStack::Pop();
    
    suite->AddL( T_VimpstEngineImSubService::NewLC() );
    CleanupStack::Pop();
    
    suite->AddL ( T_VimpstEngineCchHandler::NewLC() );
    CleanupStack::Pop();

	suite->AddL ( T_VIMPSTEngineServiceTableFetcher::NewLC() );
    CleanupStack::Pop();
    
    suite->AddL ( T_VIMPSTEngineRequest::NewLC() );
    CleanupStack::Pop();
    
    suite->AddL ( T_VIMPSTEngineRequestMapper::NewLC() );
    CleanupStack::Pop();
    
    suite->AddL ( T_VIMPSTEngineContactMgmtExtention::NewLC() );
    CleanupStack::Pop(); 
              
    suite->AddL ( T_VimpstEngineSessionCntxtObserver::NewLC() );
    CleanupStack::Pop();
    
    /*suite->AddL ( T_VIMPSTEngineSearchMgrExtention::NewLC() );
    CleanupStack::Pop();*/
   
    suite->AddL ( T_VimpstEnginePresenceSubService::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop( suite );

    return suite;
    }

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
#endif

//  END OF FILE
