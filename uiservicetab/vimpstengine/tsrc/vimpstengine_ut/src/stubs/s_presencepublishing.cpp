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
* Description: s_presencepublishing.cpp
*
*/
#include "s_presencepublishing.h"

TXIMPRequestId gMPresencePublishingStubId;
TInt gPublishOwnPresenceLeaves = KErrNone;

TXIMPRequestId MPresencePublishingStub::SubscribeOwnPresenceL( const MPresenceInfoFilter& /*aPif*/ )
    {
    return gMPresencePublishingStubId;
    }

TXIMPRequestId MPresencePublishingStub::UnsubscribeOwnPresenceL()
    {
    return gMPresencePublishingStubId;
    }

TXIMPRequestId MPresencePublishingStub::PublishOwnPresenceL( const MPresenceInfo& /*aPresence*/ )
    {   
    User::LeaveIfError( gPublishOwnPresenceLeaves ); 
        
    return gMPresencePublishingStubId;
    }

TXIMPRequestId MPresencePublishingStub::SubscribePresenceWatcherListL()
    {
    return gMPresencePublishingStubId;
    }

TXIMPRequestId MPresencePublishingStub::UnsubscribePresenceWatcherListL()
    {
    return gMPresencePublishingStubId;
    }

const TAny* MPresencePublishingStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresencePublishingStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresencePublishingStub::GetInterfaceId() const
    {
    return 0;
    }
