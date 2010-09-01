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
* Description: s_presenceobjectfactory.h
*
*/
#ifndef MPRESENCEOBJECTFACTORYSTUB_H
#define MPRESENCEOBJECTFACTORYSTUB_H

#include <presenceobjectfactory.h>

class MPresenceObjectFactoryStub : public MPresenceObjectFactory
    {
public:

    MPresentityGroupInfo* NewPresentityGroupInfoLC();

    MPresentityGroupMemberInfo* NewPresentityGroupMemberInfoLC();

    MPresenceWatcherInfo* NewPresenceWatcherInfoLC();

    MPresenceGrantRequestInfo* NewPresenceGrantRequestInfoLC();

    MPresenceInfo* NewPresenceInfoLC();

    MPresenceInfoFilter* NewPresenceInfoFilterLC();

    MPersonPresenceInfo* NewPersonPresenceInfoLC();

    MServicePresenceInfo* NewServicePresenceInfoLC();

    MDevicePresenceInfo* NewDevicePresenceInfoLC();

    MPresenceInfoField* NewInfoFieldLC();

    MPresenceInfoFieldValueEnum* NewEnumInfoFieldLC();

    MPresenceInfoFieldValueText* NewTextInfoFieldLC();

    MPresenceInfoFieldValueBinary* NewBinaryInfoFieldLC();

    MPresenceBlockInfo* NewPresenceBlockInfoLC();

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };


#endif // MPRESENCEOBJECTFACTORY_H



