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
* Description: s_presencewatching.h
*
*/
#ifndef MXIMPFWPRESENCEWATCHINGSTUB_H
#define MXIMPFWPRESENCEWATCHINGSTUB_H

#include <presencewatching.h>

class MPresenceWatchingStub : public MPresenceWatching
    {
    TXIMPRequestId SubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif );

    TXIMPRequestId UnsubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId );
    
    TXIMPRequestId SubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif );

    TXIMPRequestId UnsubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId );
    

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWPRESENCEWATCHINGSTUB_H



