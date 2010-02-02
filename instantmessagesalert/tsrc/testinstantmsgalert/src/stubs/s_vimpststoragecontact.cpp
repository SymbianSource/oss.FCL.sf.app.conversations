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
#include "s_vimpststoragecontact.h"

#include "TVIMPSTEnums.h"
#include <MVPbkStoreContact.h>

// ============================ MEMBER FUNCTIONS ===============================

    
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::UserId
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& MyVIMPSTStorageContactStub::UserId() const
    {
    return KNullDesC;   
    }



// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::OnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TOnlineStatus MyVIMPSTStorageContactStub::OnlineStatus() const
    {
    return iPresenceStatus;        
    }


// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::SetOnlineStatus
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactStub::SetOnlineStatus( 
                        TVIMPSTEnums::TOnlineStatus aOnlineStatus )
    {
    
    iPresenceStatus = aOnlineStatus;
    }
        
     

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::StatusText
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
const TDesC& MyVIMPSTStorageContactStub::StatusText() const
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::SetStatusTextL
// From MVIMPSTStorageContact
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactStub::SetStatusTextL( const TDesC& /*aStatusText*/ )
    {
   
    }
  
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::SetUserIdL
// From MVIMPSTStorageExtendedStorageContact
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactStub::SetUserIdL( const TDesC& /*aUserId */ )
    {
    
    }
    

// ---------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::ContactLink()
// ---------------------------------------------------------------------------
// 
MVPbkContactLink* MyVIMPSTStorageContactStub::ContactLink() const
    {
	MVPbkContactLink* link = NULL;
	
    return link;
    }

// ---------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::VoIPNumbers()
// ---------------------------------------------------------------------------
// 
RPointerArray<HBufC>& MyVIMPSTStorageContactStub::ServiceDetails() 
    {
	RPointerArray<HBufC> array;
	
    return array;
    }

// ---------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::SetSeriveFields()
// ---------------------------------------------------------------------------
// 
void MyVIMPSTStorageContactStub::SetSeriveFields( 
    const CDesCArray& /*aArrayOfServices*/ )
    {
    
    }

// ---------------------------------------------------------------------------
// MyVIMPSTStorageContactStub::SetSeriveFields()
// ---------------------------------------------------------------------------
// 
void MyVIMPSTStorageContactStub::SignalChanges()
    {
    
    }
 //  End of File  

// MyMVIMPSTStorageViewIdStub::SetOwnData
// From MVIMPSTStorageContacts.
// 
// -----------------------------------------------------------------------------



