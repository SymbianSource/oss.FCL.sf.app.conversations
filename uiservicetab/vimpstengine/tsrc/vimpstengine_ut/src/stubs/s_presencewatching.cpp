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
* Description: s_presencewatching.cpp
*
*/
#include "s_presencewatching.h"


TXIMPRequestId MPresenceWatchingStub::SubscribePresentityPresenceL(
    const MXIMPIdentity& /*aPresentityId*/,
    const MPresenceInfoFilter& /*aPif*/ )
    {
    TXIMPRequestId id;
    return id;
    }

TXIMPRequestId MPresenceWatchingStub::UnsubscribePresentityPresenceL(
    const MXIMPIdentity& /*aPresentityId*/ )
    {
    TXIMPRequestId id;
    return id;
    }
    
TXIMPRequestId MPresenceWatchingStub::SubscribePresentityGroupMembersPresenceL(
    const MXIMPIdentity& /*aGroupId*/,
    const MPresenceInfoFilter& /*aPif*/ )
    {
    TXIMPRequestId id;
    return id;
    }

TXIMPRequestId MPresenceWatchingStub::UnsubscribePresentityGroupMembersPresenceL(
    const MXIMPIdentity& /*aGroupId*/ )
    {
    TXIMPRequestId id;
    return id;
    }

const TAny* MPresenceWatchingStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceWatchingStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceWatchingStub::GetInterfaceId() const
    {
    return 0;
    }
