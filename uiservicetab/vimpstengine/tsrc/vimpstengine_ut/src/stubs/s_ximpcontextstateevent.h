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
* Description: s_ximpcontextstateevent.h
*
*/
#ifndef MXIMPFWCONTEXTSTATEEVENTSTUB_H
#define MXIMPFWCONTEXTSTATEEVENTSTUB_H

#include <ximpcontextstateevent.h>


class MXIMPContextStateEventStub : public MXIMPContextStateEvent
    {
    const MXIMPContextState& ContextState() const;
    const MXIMPStatus* StateChangeReason() const;
        
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;

    };

#endif // MXIMPFWCONTEXTSTATEEVENTSTUB_H



