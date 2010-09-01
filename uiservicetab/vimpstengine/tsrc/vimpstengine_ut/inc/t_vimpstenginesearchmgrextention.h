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
* Description: t_vimpstenginesearchmgrextention.h
*
*/
#ifndef _T_VIMPSTEngineSearchMgrExtention_H_
#define _T_VIMPSTEngineSearchMgrExtention_H_

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

#include "mvimpstenginesearchextentioneventobserver.h"
#include "cvimpstenginesearchmgrextention.h"

//Forward declarations
class CVIMPSTEngineSearchMgrExtention;
class MXIMPClient;
class MXIMPContext;
class MPresenceFeatures;
class CVIMPSTEngineRequestMapper;


class T_VIMPSTEngineSearchMgrExtention : public CEUnitTestSuiteClass,
                                        public MVIMPSTEngineSearchExtentionEventObserver
    {
    public:

        static T_VIMPSTEngineSearchMgrExtention* NewLC();
        virtual ~T_VIMPSTEngineSearchMgrExtention();
    public:
        
        // From MVIMPSTEngineSearchExtentionEventObserver
        void HandleSearchResultL(RArray<TVIMPSTSearchData> aSerachData);
        void HandleSearchFinishedL(TInt aError);
        void HandleSearchKeysEventL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray );

    private:

        void ConstructL();
        void SetupL();        
        void Teardown();
		
        /*
         * To be tested: 
         * - SearchContactsL
         * - SubscribeForSearchKeysL
         * - IsSupported
         * - Type
         * - HandleSessionContextEventL
          */
        
        void TestSearchContactsL();
        void TestSubscribeForSearchKeysL();
        void TestIsSupported();
        void TestType();
        void TestHandleSessionContextEventL();
        
        void TestAllFunctionsL();
        
        
    private:

        T_VIMPSTEngineSearchMgrExtention() {};

    
    private:
        CVIMPSTEngineSearchMgrExtention* iSearcMgrExtention;
                
        MXIMPClient* iClient;               
        //Create new sink to receive ximp context events
        MXIMPContext* iPresenceCtx;
                
        MPresenceFeatures* iFeatures;
                
        CVIMPSTEngineRequestMapper* iRequestMapper;
    	
    	EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  _T_CSCPUtility_H_

// end of file
