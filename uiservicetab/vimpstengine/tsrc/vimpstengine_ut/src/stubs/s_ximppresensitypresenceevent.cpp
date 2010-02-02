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
* Description: s_ximppresensitypresenceevent.cpp
*
*/
#include "s_ximppresensitypresenceevent.h"
#include "s_ximpidentity.h"
#include "s_presenceinfo.h"

MyMXIMPIdentityStub gMXIMPIdentity;
MPresenceInfoStub gMPresenceInfo;
//MXIMPDataSubscriptionState gMXIMPDataSubscriptionState;

TInt32 gMXIMPPresentityPresenceEventStubInterfaceId = 0;

const MXIMPIdentity& MPresentityPresenceEventStub::PresentityId() const
    {
    return gMXIMPIdentity;
    }
    
const MPresenceInfo& MPresentityPresenceEventStub::SubscribedPresence() const
    {
    return gMPresenceInfo;
    }
        
const MXIMPDataSubscriptionState& MPresentityPresenceEventStub::DataSubscriptionState() const
    {
    MXIMPDataSubscriptionState* state = NULL;
    return *state;
    }
    
    
const TAny* MPresentityPresenceEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

 TAny* MPresentityPresenceEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresentityPresenceEventStub::GetInterfaceId() const
    {
    return gMXIMPPresentityPresenceEventStubInterfaceId;
    }


