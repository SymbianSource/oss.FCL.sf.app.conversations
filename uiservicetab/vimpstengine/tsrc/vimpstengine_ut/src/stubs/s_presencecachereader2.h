/*
* s_presencecachereader2.h :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencecachereader2.h
*
*/
#ifndef MPRESENCECACHEREADER2STUB_H
#define MPRESENCECACHEREADER2STUB_H

#include <presencecachereader2.h>

class  MPresenceCacheReader2Stub : public MPresenceCacheReader2
    {
   
public: //Sub interfaces for presence cache reader, Synchonous methods

    MPresenceBuddyInfo2* PresenceInfoLC(const TDesC& aIdentity);

    TInt BuddyCountInAllServices() ;

    TInt BuddyCountInService(const TDesC& aServiceName) ;

    TInt ServicesCount();

    TInt CancelRead() ;

public: //Sub interfaces for presence cache reader, Asynchronous methods
    TInt AllBuddiesPresenceInService(const TDesC& aServiceName, 
            MPresenceCacheReadHandler2* aHandler);

public: //Sub interfaces for presence cache reader, Asynchronous methods

    TInt SetObserverForSubscribedNotifications(
            MPresenceCacheReadHandler2* aHandler) ;


    TInt SubscribePresenceBuddyChangeL(const TDesC& aIdentity);

    void UnSubscribePresenceBuddyChangeL(const TDesC& aIdentity) ;
    };



#endif //MPRESENCECACHEREADER2STUB_H



