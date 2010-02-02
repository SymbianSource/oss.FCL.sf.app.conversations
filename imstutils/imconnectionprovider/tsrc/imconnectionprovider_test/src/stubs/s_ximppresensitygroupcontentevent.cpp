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
* Description: s_ximppresensitygroupcontentevent.cpp
*
*/

#include "s_ximppresensitygroupcontentevent.h"
#include "s_ximpidentity.h"

TInt32 gMXIMPPresentityGroupEventStubInterfaceId = 0;
extern MyMXIMPIdentityStub gMXIMPIdentity; // already define in MPresentityPresenceEventStub

const MXIMPIdentity& MPresentityGroupContentEventStub::GroupId() const
    {
    return gMXIMPIdentity;
    }
    
TInt MPresentityGroupContentEventStub::NewMembersCount() const
    {
    return KErrNone;
    }

const MPresentityGroupMemberInfo& MPresentityGroupContentEventStub::NewMember( 
    TInt /*aIndex*/ ) const
    {
    MPresentityGroupMemberInfo* info;
    return *info;
    }

TInt MPresentityGroupContentEventStub::UpdatedMembersCount() const
    {
    return KErrNone;
    }

const MPresentityGroupMemberInfo& MPresentityGroupContentEventStub::UpdatedMember( 
    TInt /*aIndex*/ ) const
    {
    MPresentityGroupMemberInfo* info;
    return *info;
    }

TInt MPresentityGroupContentEventStub::DisappearedMembersCount() const
    {
    return KErrNone;
    }

const MPresentityGroupMemberInfo& MPresentityGroupContentEventStub::DisappearedMember( 
    TInt /*aIndex*/ ) const
    {
    MPresentityGroupMemberInfo* info;
    return *info;
    }

TInt MPresentityGroupContentEventStub::CurrentMembersCount() const
    {
    return KErrNone;
    }

const MPresentityGroupMemberInfo& MPresentityGroupContentEventStub::CurrentMember( 
    TInt /*aIndex*/ ) const
    {
    MPresentityGroupMemberInfo* info;
    return *info;
    }

const MXIMPDataSubscriptionState& MPresentityGroupContentEventStub::DataSubscriptionState() const
    {
    MXIMPDataSubscriptionState* state;
    return *state;
    }


const TAny* MPresentityGroupContentEventStub::GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const
    {
    return this;
    }

 TAny* MPresentityGroupContentEventStub::GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps )
    {
    return NULL;
    }

TInt32 MPresentityGroupContentEventStub::GetInterfaceId() const
    {
    return gMXIMPPresentityGroupEventStubInterfaceId;
    }
