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
* Description: s_servicepresenceinfo.h
*
*/
#ifndef MXIMPFWSERVICEPRESENCEINFOSTUB_H
#define MXIMPFWSERVICEPRESENCEINFOSTUB_H

#include <servicepresenceinfo.h>

class MServicePresenceInfoStub : public MServicePresenceInfo
    {
    const TPtrC8 ServiceType() const;
    
    void SetServiceTypeL( const TDesC8& aServiceType );
    
    const MPresenceInfoFieldCollection& Fields() const;
    
    MPresenceInfoFieldCollection& Fields();

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };


#endif //MXIMPFWSERVICEPRESENCEINFO_H



