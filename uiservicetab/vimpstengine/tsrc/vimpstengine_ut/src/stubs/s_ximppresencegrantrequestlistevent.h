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
* Description: s_ximppresencegrantrequestlistevent.h
*
*/
#ifndef MXIMPFWPRESENCEGRANTREQUESTLISTEVENTSTUB_H
#define MXIMPFWPRESENCEGRANTREQUESTLISTEVENTSTUB_H

#include <presencegrantrequestlistevent.h>


class MPresenceGrantRequestListEventStub : public MPresenceGrantRequestListEvent
    {
    TInt NewRequestsCount() const;
    const MPresenceGrantRequestInfo& NewRequest( TInt aIndex ) const;
    TInt DisappearedRequestsCount() const;
    const MPresenceGrantRequestInfo& DisappearedRequest( TInt aIndex ) const;
    TInt CurrentRequestsCount() const;
    const MPresenceGrantRequestInfo& CurrentRequest( TInt aIndex ) const;
    const MXIMPDataSubscriptionState& DataSubscriptionState() const;
   
   
    const TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps ) const;

     TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps );

     TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWPRESENCEGRANTREQUESTLISTEVENTSTUB_H
