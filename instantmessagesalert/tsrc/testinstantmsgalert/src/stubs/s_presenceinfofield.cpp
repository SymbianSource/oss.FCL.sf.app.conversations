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
* Description : s_presenceinfofield.cpp
*
*/

#include "s_presenceinfofield.h"

const TDesC8& MPresenceInfoFieldStub::FieldType() const
    {
    return KNullDesC8;
    }

void MPresenceInfoFieldStub::SetFieldTypeL( const TDesC8& /*aFieldType*/ )  
    {
    }

const MXIMPBase& MPresenceInfoFieldStub::FieldValue() const
    {
    MXIMPBase* base = NULL;
    return *base;
    }

MXIMPBase& MPresenceInfoFieldStub::FieldValue()
    {
    MXIMPBase* base = NULL;
    return *base;
    }

void MPresenceInfoFieldStub::SetFieldValue( MXIMPBase* /*aFieldValue*/ )
    {
    }

const TAny* MPresenceInfoFieldStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceInfoFieldStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceInfoFieldStub::GetInterfaceId() const
    {
    return 0;
    }





    
