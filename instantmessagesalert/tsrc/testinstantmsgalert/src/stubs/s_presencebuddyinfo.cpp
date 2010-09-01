/*
* s_presencebuddyinfo.cpp Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:s_presencebuddyinfo.cpp
*
*/

#include "s_presencebuddyinfo.h"
#include "s_presenceinfo.h"



MPresenceBuddyInfoStub* MPresenceBuddyInfoStub::NewStubL()
    {
    return new ( ELeave ) MPresenceBuddyInfoStub();
    }

 TInt MPresenceBuddyInfoStub::Set( 
     MXIMPIdentity* /*aIdentity*/, 
     MPresenceInfo* /*aPresenceInfo*/)
     {
     return KErrNone;
     }
 
TInt MPresenceBuddyInfoStub::SetPresenceInfo(MPresenceInfo* /*aPresenceInfo*/)
    {
    return KErrNone;
    }
    
TInt MPresenceBuddyInfoStub::SetBuddyId(MXIMPIdentity* /*aIdentity*/)
    {
    return KErrNone;
    }

TInt MPresenceBuddyInfoStub::SetPersonPresenceL(
    MPersonPresenceInfo* /*aPersonPresenceInfo*/)
    {
    return KErrNone;
    }

const MPresenceInfo* MPresenceBuddyInfoStub::PresenceInfo() const
    {
    if ( !gMPresenceInfoStub )
        {
        gMPresenceInfoStub = new ( ELeave ) MPresenceInfoStub();
        }
    
    return gMPresenceInfoStub;
    }

const MXIMPIdentity* MPresenceBuddyInfoStub::BuddyId() const
    {
    MXIMPIdentity* identity = NULL;
    return identity;
    }

TBool MPresenceBuddyInfoStub::EqualsIdentity(
    const MPresenceBuddyInfo* /*aOtherInstance*/ ) const
    {
    return ETrue;
    }

const TAny* MPresenceBuddyInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceBuddyInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceBuddyInfoStub::GetInterfaceId() const
    {
    return 0;
    }



    
