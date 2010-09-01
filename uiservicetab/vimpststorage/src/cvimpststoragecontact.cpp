/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of contact data container
*
*/



// INCLUDE FILES
#include "cvimpststoragecontact.h"
#include "tvimpstenums.h"
#include "uiservicetabtracer.h"
#include "cvimpststoragedefs.h"

#include <MVPbkStoreContact.h>
#include <MVPbkContactLink.h>
#include <e32base.h>
#include "mvimpststoragevpbkcontactstore.h"
 
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::CVIMPSTStorageContact
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContact::CVIMPSTStorageContact() 
    {
    iPresenceStatus = TVIMPSTEnums::EOffline;  
    iAvatarIndex = 0;  
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::ConstructL(const TDesC& aUserId,
    								  const TDesC& aDisplayName,
    								  const MVPbkContactLink& aContactLink,
    								  const TDesC8& aAvatarContent )
    {
	TRACER_AUTO;
    iUserId = aUserId.AllocL();
    iDisplayName = aDisplayName.AllocL();
    MVPbkContactLink* link = aContactLink.CloneLC();
    iContactLink = link;
    CleanupStack::Pop(); // link
    iAvatarContent = aAvatarContent.AllocL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::ConstructL(const TDesC& aUserId,
    								  const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    iUserId = aUserId.AllocL();
    iDisplayName = aDisplayName.AllocL();
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContact* CVIMPSTStorageContact::NewL(const TDesC& aUserId,
		        								  const TDesC& aDisplayName,
		        								  const MVPbkContactLink& aContactLink,
		        								  const TDesC8& aAvatarContent)
    {
	TRACER_AUTO;
    CVIMPSTStorageContact* self = CVIMPSTStorageContact::NewLC( aUserId,aDisplayName,aContactLink,aAvatarContent );
    CleanupStack::Pop( self );
	return self;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContact* CVIMPSTStorageContact::NewL(const TDesC& aUserId,
		        								  const TDesC& aDisplayName )
    {
	TRACER_AUTO;
    CVIMPSTStorageContact* self = new (ELeave) CVIMPSTStorageContact();
    CleanupStack::PushL( self );
    self->ConstructL( aUserId,aDisplayName );
    CleanupStack::Pop( self );
	return self;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageContact* CVIMPSTStorageContact::NewLC(const TDesC& aUserId,
		        								  const TDesC& aDisplayName,
		        								  const MVPbkContactLink& aContactLink,
		        								  const TDesC8& aAvatarContent)
    {
	TRACER_AUTO;
    CVIMPSTStorageContact* self = new( ELeave ) CVIMPSTStorageContact();
    CleanupStack::PushL( self );
    self->ConstructL( aUserId,aDisplayName,aContactLink ,aAvatarContent);
	return self;
    }

// Destructor
CVIMPSTStorageContact::~CVIMPSTStorageContact()
    {

    if(iContactLink)
        {
 
        delete iContactLink;
        iContactLink = NULL;
        }
    
    delete iUserId;
    delete iDisplayName;
    delete iStatusText;
    delete iAvatarContent;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::UserId
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTStorageContact::UserId() const
    {
	TRACER_AUTO;
    if( iUserId )
        {
        return *iUserId;
        }
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::UserId
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTStorageContact::Name() const
    {
	TRACER_AUTO;
    if( iDisplayName )
        {
        return *iDisplayName;
        }
    return KNullDesC;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::OnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus CVIMPSTStorageContact::OnlineStatus() const
    {
    return iPresenceStatus;        
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetOnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::SetOnlineStatus( 
                        TVIMPSTEnums::TOnlineStatus aOnlineStatus )
    {
    iPresenceStatus = aOnlineStatus;
    }
        

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::StatusText
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& CVIMPSTStorageContact::StatusText() const
    {
	TRACER_AUTO;
    if( iStatusText )
        {
        return *iStatusText;
        }
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetStatusTextL
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::SetStatusTextL( const TDesC& aStatusText )
    {
    if( iStatusText )
	    {
	    delete iStatusText;
	    iStatusText = NULL;	
	    }
    iStatusText= aStatusText.AllocL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetAvatarContentL
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::SetAvatarContentL( const TDesC8& aAvatarContent ,MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler )
    {
	TRACER_AUTO;
    if( iAvatarContent )
        {
        if( iAvatarContent->Compare( aAvatarContent ) == 0 )
            {
            // no need to set 
            return;
            }
        delete iAvatarContent;
        iAvatarContent = NULL;  
        }
    iAvatarContent = aAvatarContent.AllocL();
    if( iContactLink )
        {
        aVPPkStoreHandler.UpdateAvatarFieldDataL(*iContactLink, *iAvatarContent);
        }
    }

   
// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetUserIdL
// From MVIMPSTStorageExtendedStorageContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::SetUserIdL( const TDesC& aUserId )
    {
    if( iUserId )
	    {
	    delete iUserId;
	    iUserId = NULL;	
	    }
    iUserId = aUserId.AllocL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContact::SetNameL
// From MVIMPSTStorageExtendedStorageContact
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageContact::SetNameL( const TDesC& aName )
    {
	TRACER_AUTO;
    if( iDisplayName )
	    {
	    delete iDisplayName;
	    iDisplayName = NULL;	
	    }
    iDisplayName= aName.AllocL();
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::ContactLink()
// ---------------------------------------------------------------------------
// 
MVPbkContactLink* CVIMPSTStorageContact::ContactLink() const
    {
    return iContactLink;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::AvatarContent
// ---------------------------------------------------------------------------
//  
const TDesC8& CVIMPSTStorageContact::AvatarContent() const
    {
    if( iAvatarContent )
        {
        return *iAvatarContent;
        }
    return KNullDesC8();
    }
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::SetAvatarIndex()
// ---------------------------------------------------------------------------
// 
void CVIMPSTStorageContact::SetAvatarIndex(TInt aIndex ) 
    {
    iAvatarIndex = aIndex;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTStorageContact::AvatarIndex
// ---------------------------------------------------------------------------
//  
TInt CVIMPSTStorageContact::AvatarIndex() const
    {
    return iAvatarIndex;
    }

//  End of File  
