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
* Description: s_ximpcontextstateevent.cpp
*
*/
#include "s_ximpcontextstateevent.h"


TInt32 gMXIMPContextStateEventStubInterfaceId = 0;

const MXIMPContextState& MXIMPContextStateEventStub::ContextState() const
    {
    MXIMPContextState* state = NULL;
    return *state;
    }

const MXIMPStatus* MXIMPContextStateEventStub::StateChangeReason() const
    {
    MXIMPStatus* status = NULL;
    return status;
    }

const TAny* MXIMPContextStateEventStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

TAny* MXIMPContextStateEventStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps */)
    {
    return NULL;
    }

TInt32 MXIMPContextStateEventStub::GetInterfaceId() const
    {
    return gMXIMPContextStateEventStubInterfaceId;
    }
