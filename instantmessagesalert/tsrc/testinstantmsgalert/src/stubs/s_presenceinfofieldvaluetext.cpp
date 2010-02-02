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
* Description: s_presenceinfofieldvaluetext.cpp
*
*/

#include "s_presenceinfofieldvaluetext.h"

TInt MPresenceInfoFieldValueTextStub::MaxLength() const
    {
    return 0;
    }

TPtrC MPresenceInfoFieldValueTextStub::TextValue() const
    {
    return KNullDesC();
    }

void MPresenceInfoFieldValueTextStub::SetTextValueL( const TDesC& /*aValue*/ )
    {
    }

const TAny* MPresenceInfoFieldValueTextStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceInfoFieldValueTextStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceInfoFieldValueTextStub::GetInterfaceId() const
    {
    return 0;
    }
