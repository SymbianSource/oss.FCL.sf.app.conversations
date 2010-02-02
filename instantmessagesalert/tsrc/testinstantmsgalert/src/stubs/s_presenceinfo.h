/*
* s_presenceinfo.h :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presenceinfo.h
*
*/
#ifndef MPRESENCEINFOSTUB
#define MPRESENCEINFOSTUB

#include <presenceinfo.h>

/**
 * Interface for presence info object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoStub : public MPresenceInfo
    {
public:

    const MPersonPresenceInfo* PersonPresence() const;
    MPersonPresenceInfo* PersonPresence();

    void SetPersonPresenceL( MPersonPresenceInfo* aPersonPresence );

    TInt ServicePresenceCount() const;

    const MServicePresenceInfo& ServicePresenceAt( TInt aIndex ) const;

    void LookupServicePresenceByServiceType(
                MServicePresenceInfo*& aServicePresence,
                const TDesC8& aMatch ) const;

    void AddServicePresenceL( MServicePresenceInfo* aServicePresence );

    TInt DevicePresenceCount() const;

    const MDevicePresenceInfo& DevicePresenceAt( TInt aIndex ) const;

    void AddDevicePresenceL( MDevicePresenceInfo* aDevicePresence );

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };

#endif
