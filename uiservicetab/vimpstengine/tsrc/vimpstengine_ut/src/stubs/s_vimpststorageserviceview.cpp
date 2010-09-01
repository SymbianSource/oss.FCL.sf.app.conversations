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
* Description:  
*
*/


#include "s_vimpststorageserviceview.h"

MVIMPSTStorageServiceView* vimpstserviceview_stub::NewL()
    {
    MVIMPSTStorageServiceView* ob = new(ELeave)vimpstserviceview_stub;
        return ob;
    }

void vimpstserviceview_stub::AddObserverL( MVIMPSTStorageContactsObserver* aObserver ) 
    {

    }

void vimpstserviceview_stub::RemoveObserver( MVIMPSTStorageContactsObserver* aObserver )
    {
    
    }


MVIMPSTStorageContact* vimpstserviceview_stub::FindContactByUserId( const TDesC& aUserId )
    {
    return NULL;
    }


TInt vimpstserviceview_stub::ListCount() const
{
return 1;
}

MVIMPSTStorageContactList& vimpstserviceview_stub::ListAt( TInt aIndex ) const 
{
MVIMPSTStorageContactList *abc = NULL;
    
return *abc;
}


MVIMPSTStorageContactList* vimpstserviceview_stub::FindContactList( const TDesC& aListId )
    {
    return NULL;
    }

MVIMPSTStorageContactList* vimpstserviceview_stub::CreateContactListL( const TDesC& aContactListId, 
        const TDesC& aDisplayName ) 
    {
    MVIMPSTStorageContactList *abc = NULL;
        
    return abc;
    }

void vimpstserviceview_stub::RemoveContactList( const TDesC& aContactListId )
    {
    
    }


TInt vimpstserviceview_stub::ContactCount(TBool aSkipOfflineContacts ) const
{
return 0;
}



MVIMPSTStorageContact* vimpstserviceview_stub::UpdatePresenceL(const TDesC& aContactId, 
        TVIMPSTEnums::TOnlineStatus aStatus, 
        const TDesC& aStatusText,
        const TDesC8& aAvatarData,
        TBool aIsClearingAvatar ) 
    {
    return NULL;
    }

MVIMPSTStorageContact* vimpstserviceview_stub::UpdateAvatarL(const TDesC& aContactId,
        const TDesC8& aAvatarData )
    {
    return NULL;
    }

MVIMPSTStorageContact* vimpstserviceview_stub::CreateNewContactL(const TDesC& aUserId, 
        const TDesC& aDisplayName,
        TBool aIsInvitationItem ,
        TBool aInvitationAutoAccept)
    {
    MVIMPSTStorageContact* contact = NULL;
    return  contact;
    }

TInt vimpstserviceview_stub::CreateNewContactFromRetrivedIdL(  TInt aIndexToUse  )
    {
    return 0;
    }

TInt vimpstserviceview_stub::DeleteNewContactFromRetrivedIdL(  TInt aIndexToUse  )
    {
    return 0;
    }

TInt vimpstserviceview_stub::RemoveContactL( MVIMPSTStorageContact* aContact  )
    {
    return 0;
    }


void vimpstserviceview_stub::CreateNewFetchContactsL( RArray <TPtrC> &aFirstNameList, 
        RArray <TPtrC> &aServiceField )
    {
    
    }


TBool  vimpstserviceview_stub::IsLocalStore() const
{
return ETrue;
}


TInt vimpstserviceview_stub::RetriveLinkXSPIdsL(const TDesC8& aContactPackLink )
    {
    return 0;
    }
const TDesC& vimpstserviceview_stub::GetRetrieveXSPIdL(TInt aIndex )
    {
    return KNullDesC();
    }

MVIMPSTStorageContact* vimpstserviceview_stub::FindContactByLink(const MVPbkContactLink& aContactLink )
    {
    return NULL;
    }


void vimpstserviceview_stub::Sort( const TDesC& aContactListId)
    {
    
    }

MVIMPSTStorageContact& vimpstserviceview_stub::OwnContactL() 
    {
    MVIMPSTStorageContact *abc = NULL;
        
    return *abc;
    }

void vimpstserviceview_stub::DeleteDatabaseL() 
    {
    
    }

void vimpstserviceview_stub::SetOwnUserIdL(const TDesC& aUserId ) 
    {
    
    }

void vimpstserviceview_stub::SetUnnamedTextL(HBufC* aUnnamedText )
    {
    
    }



// End of File
