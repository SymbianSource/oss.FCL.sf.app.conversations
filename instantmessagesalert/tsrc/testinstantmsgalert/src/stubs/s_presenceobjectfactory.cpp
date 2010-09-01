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
* Description: s_presenceobjectfactory.cpp
*
*/

#include "s_presenceobjectfactory.h"
#include "s_servicepresenceinfo.h"
#include "s_presenceinfofield.h"
#include "s_presenceinfofieldvalueenum.h"
#include "s_presencepersoninfo.h"
#include "s_presenceinfofieldvaluetext.h"
#include "s_presenceinfo.h"

MPresenceInfoStub* gMPresenceInfoStub = NULL; 
MServicePresenceInfoStub gMPresenceObjectFactoryStubServicePresenceInfo;
MPresenceInfoFieldStub gMPresenceObjectFactoryStubPresenceInfoField;
MPresenceInfoFieldValueEnumStub gMPresenceObjectFactoryStubPresenceInfoFieldValueEnum;
MPersonPresenceInfoStub gMPresenceObjectFactoryStubPersonPresenceInfo;
MPresenceInfoFieldValueTextStub gMPresenceObjectFactoryStubPresenceInfoFieldValueText;

MPresentityGroupInfo* MPresenceObjectFactoryStub::NewPresentityGroupInfoLC()
    {
    return NULL;
    }

MPresentityGroupMemberInfo* MPresenceObjectFactoryStub::NewPresentityGroupMemberInfoLC()
    {
    return NULL;
    }

MPresenceWatcherInfo* MPresenceObjectFactoryStub::NewPresenceWatcherInfoLC()
    {
    return NULL;
    }

MPresenceGrantRequestInfo* MPresenceObjectFactoryStub::NewPresenceGrantRequestInfoLC()
    {
    return NULL;
    }

MPresenceInfo* MPresenceObjectFactoryStub::NewPresenceInfoLC()
    {
    if ( !gMPresenceInfoStub )
        {
        gMPresenceInfoStub = new ( ELeave )MPresenceInfoStub();
        }
    
    CleanupStack::PushL( gMPresenceInfoStub );
    return gMPresenceInfoStub;
    }

MPresenceInfoFilter* MPresenceObjectFactoryStub::NewPresenceInfoFilterLC()
    {
    return NULL;
    }

MPersonPresenceInfo* MPresenceObjectFactoryStub::NewPersonPresenceInfoLC()
    {
    CleanupStack::PushL( &gMPresenceObjectFactoryStubPersonPresenceInfo );
    return &gMPresenceObjectFactoryStubPersonPresenceInfo;
    }

MServicePresenceInfo* MPresenceObjectFactoryStub::NewServicePresenceInfoLC()
    {
    CleanupStack::PushL( &gMPresenceObjectFactoryStubServicePresenceInfo ); 
    return &gMPresenceObjectFactoryStubServicePresenceInfo;
    }

MDevicePresenceInfo* MPresenceObjectFactoryStub::NewDevicePresenceInfoLC()
    {
    return NULL;
    }

MPresenceInfoField* MPresenceObjectFactoryStub::NewInfoFieldLC()
    {
    CleanupStack::PushL( &gMPresenceObjectFactoryStubPresenceInfoField );
    return &gMPresenceObjectFactoryStubPresenceInfoField;
    }

MPresenceInfoFieldValueEnum* MPresenceObjectFactoryStub::NewEnumInfoFieldLC()
    {
    CleanupStack::PushL( &gMPresenceObjectFactoryStubPresenceInfoFieldValueEnum );
    return &gMPresenceObjectFactoryStubPresenceInfoFieldValueEnum;
    }

MPresenceInfoFieldValueText* MPresenceObjectFactoryStub::NewTextInfoFieldLC()
    {
    CleanupStack::PushL( &gMPresenceObjectFactoryStubPresenceInfoFieldValueText );
    return &gMPresenceObjectFactoryStubPresenceInfoFieldValueText;
    }

MPresenceInfoFieldValueBinary* MPresenceObjectFactoryStub::NewBinaryInfoFieldLC()
    {
    return NULL;
    }

MPresenceBlockInfo* MPresenceObjectFactoryStub::NewPresenceBlockInfoLC()
    {
    return NULL;
    }

const TAny* MPresenceObjectFactoryStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceObjectFactoryStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceObjectFactoryStub::GetInterfaceId() const
    {
    return 0;
    }
