/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/


// INCLUDE FILES
#include "s_vimpststoragecontact.h"
//#include <MVPBKSTORECONTACT.H>
 #include "s_MVPbkContactLinkstub.h"
 
// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* vimpststoragecontact_stub::NewL()
    {
    MVIMPSTStorageContact* contact = new(ELeave)vimpststoragecontact_stub;
        return contact;
    }

// Destructor
vimpststoragecontact_stub::~vimpststoragecontact_stub()
    {
    
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::UserId
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& vimpststoragecontact_stub::UserId() const
    {
    return KDummy();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::UserId
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& vimpststoragecontact_stub::Name() const
{
return KDummy();
}
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::OnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus vimpststoragecontact_stub::OnlineStatus() const
{
TVIMPSTEnums::TOnlineStatus status = TVIMPSTEnums::EOffline;
return status;       
}


// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetOnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontact_stub::SetOnlineStatus( 
                        TVIMPSTEnums::TOnlineStatus /*aOnlineStatus*/ )
    {
      //iPresenceStatus = aOnlineStatus;
    }
        

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::StatusText
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& vimpststoragecontact_stub::StatusText() const
    {
    return KDummy();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetStatusTextL
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontact_stub::SetStatusTextL( const TDesC& /*aStatusText*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetAvatarContentL
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontact_stub::SetAvatarContentL( const TDesC8& /*aAvatarContent*/, MVIMPSTStorageVPbkContactStore& /*aVPPkStoreHandler*/ )
    {
    
    }

   
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetUserIdL
// From MVIMPSTStorageExtendedStorageContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontact_stub::SetUserIdL( const TDesC& /*aUserId*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetNameL
// From MVIMPSTStorageExtendedStorageContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontact_stub::SetNameL( const TDesC& /*aName*/ )
    {
    
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::ContactLink()
// ---------------------------------------------------------------------------
// 
MVPbkContactLink* vimpststoragecontact_stub::ContactLink() const
    {
    MVPbkContactLink* link = CVPbkContactLinkStub::NewL();
    return link;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::AvatarContent
// ---------------------------------------------------------------------------
//  
const TDesC8& vimpststoragecontact_stub::AvatarContent() const
    {
    _LIT8(Kavatar,"avatar");
    return Kavatar();
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::SetAvatarIndex()
// ---------------------------------------------------------------------------
// 
void vimpststoragecontact_stub::SetAvatarIndex(TInt aIndex ) 
    {
   iAvatarIndex = aIndex;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::AvatarIndex
// ---------------------------------------------------------------------------
//  
TInt vimpststoragecontact_stub::AvatarIndex() const
    {
    return 1;
    }

//  End of File  
