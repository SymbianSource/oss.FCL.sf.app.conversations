/*
* s_presencebuddyinfolist.cpp Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:s_presencebuddyinfolist.cpp
*
*/
#include "s_presencebuddyinfolist.h"


MPresenceBuddyInfoListStub* MPresenceBuddyInfoListStub::NewStubL()
    {
    return new ( ELeave ) MPresenceBuddyInfoListStub();
    }

 
RPointerArray<MPresenceBuddyInfo>& MPresenceBuddyInfoListStub::GetObjectCollection()
    {
    //RPointerArray<MPresenceBuddyInfo> array;
    return iBuddyInfo ; // *gPresenceArry;
    }

TPtrC MPresenceBuddyInfoListStub::ServiceName() const
    {
    TPtrC ptr;
    return ptr;
    }

TInt MPresenceBuddyInfoListStub::Count() const
    {
    return iBuddyInfo.Count();
    }

TInt MPresenceBuddyInfoListStub::AddOrReplace(
    MPresenceBuddyInfo* aBuddyPresenceInfo)
    {
    iBuddyInfo.Append( aBuddyPresenceInfo );
    return KErrNone;      
    }

TInt MPresenceBuddyInfoListStub::FindAndRemove(
    const MXIMPIdentity& /*aIdentity*/)
    {
    return KErrNone;
    }

MPresenceBuddyInfo* MPresenceBuddyInfoListStub::FindAndGet(
    const MXIMPIdentity& /*aIdentity*/) const
    {
    MPresenceBuddyInfo* info = NULL;
    return info;
    }


const TAny* MPresenceBuddyInfoListStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceBuddyInfoListStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceBuddyInfoListStub::GetInterfaceId() const
    {
    return 0;
    }



    
