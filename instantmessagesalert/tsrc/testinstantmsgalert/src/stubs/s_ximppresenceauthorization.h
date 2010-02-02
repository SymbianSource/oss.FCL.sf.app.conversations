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
* Description: s_ximppresenceauthorization.h
*
*/
#ifndef MXIMPFWPRESENCEAUTHORIZATIONSTUB_H
#define MXIMPFWPRESENCEAUTHORIZATIONSTUB_H

#include <presenceauthorization.h>


class MPresenceAuthorizationStub : public MPresenceAuthorization
    {
    TXIMPRequestId SubscribePresenceGrantRequestListL();
    
    TXIMPRequestId UnsubscribePresenceGrantRequestListL();
    
    TXIMPRequestId GrantPresenceForPresentityL(
                   const MXIMPIdentity& aPresentityId,
                   const MPresenceInfoFilter& aPif );
    
    TXIMPRequestId WithdrawPresenceGrantFromPresentityL(
                   const MXIMPIdentity& aPresentityId );

    TXIMPRequestId GrantPresenceForPresentityGroupMembersL(
                  const MXIMPIdentity& aGroupId,
                  const MPresenceInfoFilter& aPif );

    TXIMPRequestId WithdrawPresenceGrantFromPresentityGroupMembersL(
                  const MXIMPIdentity& aGroupId );
  
    TXIMPRequestId GrantPresenceForEveryoneL(
                  const MPresenceInfoFilter& aPif );

    TXIMPRequestId WithdrawPresenceGrantFromEveryoneL();
  
    TXIMPRequestId SubscribePresenceBlockListL();

    TXIMPRequestId UnsubscribePresenceBlockListL();
  
    TXIMPRequestId BlockPresenceForPresentityL(
                 const MXIMPIdentity& aPresentityId );
    
    TXIMPRequestId CancelPresenceBlockFromPresentityL(
                 const MXIMPIdentity& aPresentityId );

   
    const TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps ) const;

     TAny* GetInterface(
                         TInt32 aInterfaceId,
                         TIfGetOps aOps );

     TInt32 GetInterfaceId() const;
    };



#endif // MXIMPFWPRESENCEAUTHORIZATIONSTUB_H
