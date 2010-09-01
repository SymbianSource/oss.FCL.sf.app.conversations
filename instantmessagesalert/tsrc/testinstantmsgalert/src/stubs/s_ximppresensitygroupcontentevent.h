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
* Description: s_ximppresensitygroupcontentevent.h
*
*/
#ifndef MXIMPFWPRESENSITYGROUPCONTENTEVENTSTUB_H
#define MXIMPFWPRESENSITYGROUPCONTENTEVENTSTUB_H

#include <presentitygroupcontentevent.h>


class MPresentityGroupContentEventStub : public MPresentityGroupContentEvent
    {
    const MXIMPIdentity& GroupId() const;
    TInt NewMembersCount() const;
    const MPresentityGroupMemberInfo& NewMember( TInt aIndex ) const;
    TInt UpdatedMembersCount() const;
    const MPresentityGroupMemberInfo& UpdatedMember( TInt aIndex ) const;
    TInt DisappearedMembersCount() const;
    const MPresentityGroupMemberInfo& DisappearedMember( TInt aIndex ) const;  
    TInt CurrentMembersCount() const;
    const MPresentityGroupMemberInfo& CurrentMember( TInt aIndex ) const;
    const MXIMPDataSubscriptionState& DataSubscriptionState() const;
    
    
    const TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps ) const;

     TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps );

     TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWPRESENSITYGROUPCONTENTEVENTSTUB_H
