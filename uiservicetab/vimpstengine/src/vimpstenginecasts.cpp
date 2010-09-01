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
* Description:  Provides the Casr Functions
*
*/


// INCLUDES
#include "mvimpstengineimsubservice.h"
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginecontactmgmtextention.h"

#include "mvimpstenginesearchmgrextention.h"

enum TPanicCode
	{
	EPanicTypeMismatch = 1
	};
#ifdef _DEBUG	
void Panic( TPanicCode aPanic )
	{	
	_LIT( KPanicCat, "VIMPSTEngine::Cast" );
	User::Panic( KPanicCat, aPanic );
	}
#else
void Panic( TPanicCode /*aPanic*/ )
    {   

    }
#endif // _DEBUG


EXPORT_C MVIMPSTEngineIMSubService& MVIMPSTEngineIMSubService::Cast
        (MVIMPSTEngineSubService& aSubService)
    {
    __ASSERT_ALWAYS( aSubService.Type() == TVIMPSTEnums::EIM,
        Panic(EPanicTypeMismatch) );
    return static_cast<MVIMPSTEngineIMSubService&>(aSubService);
    }

EXPORT_C const MVIMPSTEngineIMSubService& MVIMPSTEngineIMSubService::Cast
        (const MVIMPSTEngineSubService& aSubService)
    {
    __ASSERT_ALWAYS( aSubService.Type() == TVIMPSTEnums::EIM,
        Panic(EPanicTypeMismatch) );
    return static_cast<const MVIMPSTEngineIMSubService&>(aSubService);
    }


EXPORT_C MVIMPSTEnginePresenceSubService& MVIMPSTEnginePresenceSubService::Cast
        (MVIMPSTEngineSubService& aSubService)
    {
    __ASSERT_ALWAYS( aSubService.Type() == TVIMPSTEnums::EPresence,
        Panic(EPanicTypeMismatch) );
    return static_cast<MVIMPSTEnginePresenceSubService&>(aSubService);
    }

EXPORT_C const MVIMPSTEnginePresenceSubService& MVIMPSTEnginePresenceSubService::Cast
        (const MVIMPSTEngineSubService& aSubService)
    {
    __ASSERT_ALWAYS( aSubService.Type() == TVIMPSTEnums::EPresence,
        Panic(EPanicTypeMismatch) );
    return static_cast<const MVIMPSTEnginePresenceSubService&>(aSubService);
    }

EXPORT_C MVIMPSTEngineContactMgmtExtention& MVIMPSTEngineContactMgmtExtention::Cast
        (MVIMPSTEngineExtentionFeatures& aExtentionFeature)
    {
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::EContactManagement,
        Panic(EPanicTypeMismatch) );
    return static_cast<MVIMPSTEngineContactMgmtExtention&>(aExtentionFeature);
    }

EXPORT_C const MVIMPSTEngineContactMgmtExtention& MVIMPSTEngineContactMgmtExtention::Cast
        (const MVIMPSTEngineExtentionFeatures& aExtentionFeature)
    {
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::EContactManagement,
        Panic(EPanicTypeMismatch) );
    return static_cast<const MVIMPSTEngineContactMgmtExtention&>(aExtentionFeature);
    }

EXPORT_C MVIMPSTEngineSearchMgrExtention& MVIMPSTEngineSearchMgrExtention::Cast
        (MVIMPSTEngineExtentionFeatures& aExtentionFeature)
    {
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::ESearch,
        Panic(EPanicTypeMismatch) );
    return static_cast<MVIMPSTEngineSearchMgrExtention&>(aExtentionFeature);
    }

EXPORT_C const MVIMPSTEngineSearchMgrExtention& MVIMPSTEngineSearchMgrExtention::Cast
        (const MVIMPSTEngineExtentionFeatures& aExtentionFeature)
    {
    __ASSERT_ALWAYS( aExtentionFeature.Type() == TVIMPSTEnums::ESearch,
        Panic(EPanicTypeMismatch) );
    return static_cast<const MVIMPSTEngineSearchMgrExtention&>(aExtentionFeature);
    }





