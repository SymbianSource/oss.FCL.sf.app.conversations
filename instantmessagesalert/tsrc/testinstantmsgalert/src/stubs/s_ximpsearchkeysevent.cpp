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
* Description: s_ximpsearchkeysevent.cpp
*
*/


#include "s_ximpsearchkeysevent.h"

TInt32 gMXIMPSearchKeysEventStubInterfaceId = 0;


TInt MSearchKeysEventStub::InfoCount()const
    {
    return KErrNone;
    }
    
const MSearchKeyInfo& MSearchKeysEventStub::SearchKeyInfo( TInt /*aIndex*/ )const
    {
    MSearchKeyInfo* info = NULL;
    return *info;
    }


const TAny* MSearchKeysEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ ) const
    {
    return this;
    }

 TAny* MSearchKeysEventStub::GetInterface(
                        TInt32 /*aInterfaceId*/,
                        TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MSearchKeysEventStub::GetInterfaceId() const
    {
    return gMXIMPSearchKeysEventStubInterfaceId;
    }
