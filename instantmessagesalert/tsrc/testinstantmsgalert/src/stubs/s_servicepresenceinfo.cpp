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
* Description: s_servicepresenceinfo.cpp
*
*/

#include "s_servicepresenceinfo.h"
#include "s_presenceinfofieldcollection.h"

MPresenceInfoFieldCollectionStub gMPresenceInfoFieldCollection;

const TPtrC8 MServicePresenceInfoStub::ServiceType() const
    {
    return KNullDesC8();    
    }

void MServicePresenceInfoStub::SetServiceTypeL( const TDesC8& /*aServiceType*/ )
    {
    }

const MPresenceInfoFieldCollection& MServicePresenceInfoStub::Fields() const
    {
    MPresenceInfoFieldCollection* collection = NULL;
    return *collection;
    }

MPresenceInfoFieldCollection& MServicePresenceInfoStub::Fields()
    {
    return gMPresenceInfoFieldCollection;
    }

const TAny* MServicePresenceInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MServicePresenceInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MServicePresenceInfoStub::GetInterfaceId() const
    {
    return 0;
    }
