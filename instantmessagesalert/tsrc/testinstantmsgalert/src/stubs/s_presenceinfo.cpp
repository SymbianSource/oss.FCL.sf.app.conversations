/*
* s_presenceinfo.cpp :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presenceinfo.cpp
*
*/

#include "s_presenceinfo.h"
#include "s_presencepersoninfo.h"

MPersonPresenceInfoStub gMPersonPresenceInfo;
TBool gPersonPresenceInfoAvailable = EFalse;

const MPersonPresenceInfo* MPresenceInfoStub::PersonPresence() const
    {
    if ( gPersonPresenceInfoAvailable )
        {
        return &gMPersonPresenceInfo;
        }
    MPersonPresenceInfo* info = NULL;
    return info;
    }
MPersonPresenceInfo* MPresenceInfoStub::PersonPresence()
    {
    MPersonPresenceInfo* info = NULL;
    return info;
    }

void MPresenceInfoStub::SetPersonPresenceL( MPersonPresenceInfo* /*aPersonPresence*/ )
    {
    }

TInt MPresenceInfoStub::ServicePresenceCount() const
    {
    return 0;
    }

const MServicePresenceInfo& MPresenceInfoStub::ServicePresenceAt( TInt /*aIndex*/ ) const
    {
    MServicePresenceInfo* info = NULL;
    return *info;
    }

void MPresenceInfoStub::LookupServicePresenceByServiceType(
            MServicePresenceInfo*& /*aServicePresence*/,
            const TDesC8& /*aMatch*/ ) const
    {
    }

void MPresenceInfoStub::AddServicePresenceL( MServicePresenceInfo* /*aServicePresence*/ )
    {
    }

TInt MPresenceInfoStub::DevicePresenceCount() const
    {
    return 0;
    }

const MDevicePresenceInfo& MPresenceInfoStub::DevicePresenceAt( TInt /*aIndex*/ ) const
    {
    MDevicePresenceInfo* info = NULL;
    return *info;
    }

void MPresenceInfoStub::AddDevicePresenceL( MDevicePresenceInfo* /*aDevicePresence*/ )
    {
    }

const TAny* MPresenceInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceInfoStub::GetInterfaceId() const
    {
    return 0;
    }



