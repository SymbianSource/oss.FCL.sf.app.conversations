/*
* s_presencecachereader2.cpp :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencecachereader2.cpp
*
*/
#include "s_presencecachereader2.h"

TInt gAllBuddiesPresenceInServiceError = KErrNone;
   
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::CreateReaderL
//------------------------------------------------------------
MPresenceCacheReader2* MPresenceCacheReader2:: CreateReaderL()
    {
    return new(ELeave) MPresenceCacheReader2Stub;
    }

//------------------------------------------------------------
// MPresenceCacheReader2Stub::BuddyCountInAllServices
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub::BuddyCountInAllServices()
    {
    return 0;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::BuddyCountInService
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub:: BuddyCountInService(const TDesC& /*aServiceName*/)
    {
    return 0;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::ServicesCount
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub::ServicesCount()
    {
    return 0;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::NewStubL
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub::CancelRead()
    {
    return 0;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::SetIdentityL
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub::AllBuddiesPresenceInService(const TDesC& /*aServiceName*/, 
        MPresenceCacheReadHandler2* /*aHandler*/)
    {
    return gAllBuddiesPresenceInServiceError;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::SetAvailabilityL
//------------------------------------------------------------
TInt MPresenceCacheReader2Stub::SetObserverForSubscribedNotifications(
        MPresenceCacheReadHandler2* /*aHandler*/)
    {
    return 0;
    }


//------------------------------------------------------------
// MPresenceCacheReader2::SubscribePresenceBuddyChangeL
//------------------------------------------------------------
TInt  MPresenceCacheReader2Stub::SubscribePresenceBuddyChangeL(const TDesC& /*aIdentity*/)
    {
    return 0;
    }
//------------------------------------------------------------
// MPresenceCacheReader2::UnSubscribePresenceBuddyChangeL
//------------------------------------------------------------                                               

void  MPresenceCacheReader2Stub::UnSubscribePresenceBuddyChangeL(const TDesC& /*aIdentity*/) 
    {

    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::BuddyId
//------------------------------------------------------------

 MPresenceBuddyInfo2* MPresenceCacheReader2Stub::PresenceInfoLC(const TDesC& /*aIdentity*/)
    {
    MPresenceBuddyInfo2* info =NULL;
    return info;
    }
