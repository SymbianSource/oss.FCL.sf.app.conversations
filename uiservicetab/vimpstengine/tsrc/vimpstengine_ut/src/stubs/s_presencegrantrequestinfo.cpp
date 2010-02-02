/*
* s_presencegrantrequestinfo.cpp Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencegrantrequestinfo.cpp
*
*/

#include "s_presencegrantrequestinfo.h"
#include "s_ximpidentity.h"

extern MyMXIMPIdentityStub gMXIMPIdentity; // already defined in MPresentityPresenceEventStub


MPresenceGrantRequestInfo* MPresenceGrantRequestInfoStub::NewL()
    {
    MPresenceGrantRequestInfo*   self = new (ELeave) MPresenceGrantRequestInfoStub;
    return self;
    }
const MXIMPIdentity& MPresenceGrantRequestInfoStub::RequestorId() const
    {
    gMXIMPIdentity.SetIdentityL(_L("identity"));
    return gMXIMPIdentity;
    }

const TDesC16& MPresenceGrantRequestInfoStub::RequestorDisplayName() const
    {
    return KNullDesC();
    }

void MPresenceGrantRequestInfoStub::SetRequestorIdL( MXIMPIdentity* /*aIdentity*/ )
    {   
    }   

void MPresenceGrantRequestInfoStub::SetRequestorDisplayNameL( const TDesC16& /*aDisplayName*/ )
    {   
    }

const TAny* MPresenceGrantRequestInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

TAny* MPresenceGrantRequestInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceGrantRequestInfoStub::GetInterfaceId() const
    {
    return 0;
    }

