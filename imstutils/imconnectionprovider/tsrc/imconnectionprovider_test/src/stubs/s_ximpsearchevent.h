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
* Description: s_ximpsearchevent.h
*
*/
#ifndef MXIMPFWSEARCHEVENTSTUB_H
#define MXIMPFWSEARCHEVENTSTUB_H

#include <searchevent.h>


class MSearchEventStub : public MSearchEvent
    {
    TInt SearchResultCount() const;
    const MSearchInfo& SearchResult( TInt aIndex ) const;
    const MXIMPIdentity& SearchId() const;
    
    const TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps ) const;

     TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps );

     TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWSEARCHEVENTSTUB_H
