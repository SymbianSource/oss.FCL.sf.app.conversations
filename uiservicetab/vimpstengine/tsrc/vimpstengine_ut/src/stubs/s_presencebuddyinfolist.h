/*
* s_presencebuddyinfolist.h : Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencebuddyinfolist.h
*
*/
#ifndef MXIMPFWBUDDYPRESENCEINFOLISTSTUB_H
#define MXIMPFWBUDDYPRESENCEINFOLISTSTUB_H

#include <presencebuddyinfolist.h>
#include <presencebuddyinfo.h>

class MPresenceBuddyInfoListStub : public MPresenceBuddyInfoList
    {
    public:
        static MPresenceBuddyInfoListStub* NewStubL();
        
        RPointerArray<MPresenceBuddyInfo>& GetObjectCollection();
        TPtrC ServiceName() const;
        TInt Count() const;
        TInt AddOrReplace(MPresenceBuddyInfo* aBuddyPresenceInfo);
        TInt FindAndRemove(const MXIMPIdentity& aIdentity);
        MPresenceBuddyInfo* FindAndGet(const MXIMPIdentity& aIdentity) const;
        
        const TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps ) const;
    
        TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps );
    
        TInt32 GetInterfaceId() const;
        
        ~MPresenceBuddyInfoListStub()
	        {
	        iBuddyInfo.ResetAndDestroy();	
	        }
        
    private :
    
    		RPointerArray<MPresenceBuddyInfo> 	iBuddyInfo;
    };



#endif //MXIMPFWBUDDYPRESENCEINFOLISTSTUB_H



