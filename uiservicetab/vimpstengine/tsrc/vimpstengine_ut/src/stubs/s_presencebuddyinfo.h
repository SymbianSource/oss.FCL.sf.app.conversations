/*
* s_presencebuddyinfo.h :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencebuddyinfo.h
*
*/
#ifndef MXIMPFWBUDDYPRESENCEINFOSTUB_H
#define MXIMPFWBUDDYPRESENCEINFOSTUB_H

#include <presencebuddyinfo.h>

class MPresenceBuddyInfoStub : public MPresenceBuddyInfo
    {
    public:
        static MPresenceBuddyInfoStub* NewStubL();
        
        TInt Set( MXIMPIdentity* aIdentity, 
                  MPresenceInfo* aPresenceInfo);
        TInt SetPresenceInfo(MPresenceInfo* aPresenceInfo);
        TInt SetBuddyId(MXIMPIdentity* aIdentity);
        TInt SetPersonPresenceL(MPersonPresenceInfo* aPersonPresenceInfo);                                                        
        const MPresenceInfo* PresenceInfo() const;   
        const MXIMPIdentity* BuddyId() const;
        TBool EqualsIdentity(const MPresenceBuddyInfo* aOtherInstance ) const;
        
        const TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps ) const;
    
        TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps );
    
        TInt32 GetInterfaceId() const;
    };



#endif //MXIMPFWBUDDYPRESENCEINFOSTUB_H



