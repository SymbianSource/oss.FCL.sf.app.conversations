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
* Description: s_ximpsearchkeysevent.h
*
*/
#ifndef MXIMPFWSEARCHKEYSEVENTSTUB_H
#define MXIMPFWSEARCHKEYSEVENTSTUB_H

#include <searchkeysevent.h>


class MSearchKeysEventStub : public MSearchKeysEvent
    {
    TInt InfoCount() const;
    const MSearchKeyInfo& SearchKeyInfo( TInt aIndex ) const;
    
    const TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps ) const;

     TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps );

     TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWSEARCHKEYSEVENTSTUB_H
