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
* Description: s_ximppresenceauthorization.cpp
*
*/
#include "s_ximppresenceauthorization.h"

TInt32 gMXIMPPresenceAuthorizationInterfaceId = 0;



TXIMPRequestId MPresenceAuthorizationStub::SubscribePresenceGrantRequestListL()
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub::UnsubscribePresenceGrantRequestListL()
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: GrantPresenceForPresentityL(
    const MXIMPIdentity& /*aPresentityId*/,
    const MPresenceInfoFilter& /*aPif*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: WithdrawPresenceGrantFromPresentityL(
    const MXIMPIdentity& /*aPresentityId*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: GrantPresenceForPresentityGroupMembersL(
    const MXIMPIdentity& /*aPresentityId*/,
    const MPresenceInfoFilter& /*aPif*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: WithdrawPresenceGrantFromPresentityGroupMembersL(
    const MXIMPIdentity& /*aPresentityId*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: GrantPresenceForEveryoneL(
    const MPresenceInfoFilter& /*aPif*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: WithdrawPresenceGrantFromEveryoneL()
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: SubscribePresenceBlockListL()
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: UnsubscribePresenceBlockListL()
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: BlockPresenceForPresentityL(
    const MXIMPIdentity& /*aPresentityId*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }

TXIMPRequestId MPresenceAuthorizationStub:: CancelPresenceBlockFromPresentityL(
    const MXIMPIdentity& /*aPresentityId*/ )
    {
    TXIMPRequestId requestId;
    return requestId;
    }


const TAny* MPresenceAuthorizationStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

 TAny* MPresenceAuthorizationStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceAuthorizationStub::GetInterfaceId() const
    {
    return gMXIMPPresenceAuthorizationInterfaceId;
    }
