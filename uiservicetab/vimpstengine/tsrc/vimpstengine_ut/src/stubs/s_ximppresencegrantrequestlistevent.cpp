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
* Description: s_ximppresencegrantrequestlistevent.cpp
*
*/

#include "s_ximppresencegrantrequestlistevent.h"
#include "s_presencegrantrequestinfo.h"

TInt32 gMXIMPPresenceGrantRequestListEventStubInterfaceId = 0;
MPresenceGrantRequestInfoStub gMPresenceGrantRequestInfo;
TInt gNewRequestsCountValue = KErrNone;
TInt gCurrentRequestsCountValue = KErrNone;

TInt MPresenceGrantRequestListEventStub::NewRequestsCount() const
    {
    return gNewRequestsCountValue;
    }

const MPresenceGrantRequestInfo& MPresenceGrantRequestListEventStub::NewRequest( 
    TInt /*aIndex*/ ) const
    {
    MPresenceGrantRequestInfo* info = MPresenceGrantRequestInfoStub::NewL();
    //return *gMPresenceGrantRequestInfo;
    return *info;
    }

TInt MPresenceGrantRequestListEventStub::DisappearedRequestsCount() const
    {
    return KErrNone;
    }

const MPresenceGrantRequestInfo& MPresenceGrantRequestListEventStub::DisappearedRequest( 
    TInt /*aIndex*/ ) const
    {
    MPresenceGrantRequestInfo* info = NULL;
    return *info;
    }

TInt MPresenceGrantRequestListEventStub::CurrentRequestsCount() const
    {
    return gCurrentRequestsCountValue;
    }

const MPresenceGrantRequestInfo& MPresenceGrantRequestListEventStub::CurrentRequest( 
    TInt /*aIndex*/ ) const
    {
    return gMPresenceGrantRequestInfo;
    }

const MXIMPDataSubscriptionState& MPresenceGrantRequestListEventStub::DataSubscriptionState() const
    {
    MXIMPDataSubscriptionState* state = NULL;
    return *state;
    }


const TAny* MPresenceGrantRequestListEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

 TAny* MPresenceGrantRequestListEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceGrantRequestListEventStub::GetInterfaceId() const
    {
    return gMXIMPPresenceGrantRequestListEventStubInterfaceId;
    }
