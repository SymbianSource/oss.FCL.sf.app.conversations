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
* Description: s_ximpsearchevent.cpp
*
*/

#include "s_ximpsearchevent.h"

TInt32 gMXIMPSearchEventStubInterfaceId = 0;


TInt MSearchEventStub::SearchResultCount()const
    {
    return KErrNone;
    }
    
const MSearchInfo& MSearchEventStub::SearchResult( TInt /*aIndex*/ )const
    {
    MSearchInfo* info = NULL;
    return *info;
    }

const MXIMPIdentity& MSearchEventStub::SearchId()const
    {
    MXIMPIdentity* identity = NULL;
    return *identity;
    }


const TAny* MSearchEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

 TAny* MSearchEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MSearchEventStub::GetInterfaceId() const
    {
    return gMXIMPSearchEventStubInterfaceId;
    }
