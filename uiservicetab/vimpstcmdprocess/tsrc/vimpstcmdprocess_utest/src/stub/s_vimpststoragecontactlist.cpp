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
#include "s_vimpststoragecontactlist.h"

// ============================ MEMBER FUNCTIONS ===============================    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::ListId
// -----------------------------------------------------------------------------
//
const TDesC& vimpststoragecontactlist_stub::ListId() const
    {
    return KNullDesC;    
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::DisplayName
// -----------------------------------------------------------------------------
//
TPtrC vimpststoragecontactlist_stub::DisplayName() const
    {
    return NULL;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::AddStorageContactToCacheL
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::AddStorageContactToCacheL( MVIMPSTStorageContact* aContact, TInt& aIndex )
    {
    
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::RemoveContactFromCacheL
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::RemoveContactFromCacheL( const TDesC& aContactId, TInt& aIndex )
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::RemoveContactFromCacheL
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::RemoveContactFromCacheL( const MVPbkContactLink& aContactLink, TInt& index )
    {
    
    }    
 // -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContact
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::ContactCount( TBool aSkipOfflineContacts,
						   TBool aSkipBlocekedContacts ) const
    {
    
    }
            
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactIndex
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::FindContactIndex( const TDesC& aUserId,
                                       TInt& aOrderedIndex ) const
    {
     
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactEntry
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::FindContactEntry( const MVIMPSTStorageContact* aContact ) const
    {
    
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::Sort
// -----------------------------------------------------------------------------
//
void vimpststoragecontactlist_stub::Sort()
    {
    
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::ResortContact
// -----------------------------------------------------------------------------
//
void vimpststoragecontactlist_stub::ResortContact( MVIMPSTStorageContact* aContact )
    {
    
    }
   
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindIndexOfContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::FindIndexOfContact( const MVIMPSTStorageContact* aContact,
                            TVIMPSTEnums::TFilterType aFilter  ) const
    {    
    
    }
       
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::Count
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::Count() const
    {
    return 1;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::OnlineCount
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::OnlineCount() const
    {
    
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::operator[]
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& vimpststoragecontactlist_stub::operator[]( TInt aIndex ) const
    {
    
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::OnlineContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& vimpststoragecontactlist_stub::OnlineContact( TInt aIndex ) const
    {
    
    }
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilteredContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& vimpststoragecontactlist_stub::FilteredContact( 
                TInt aIndex, TVIMPSTEnums::TFilterType aFilter ) const
	{
    
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilteredCount
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::FilteredCount( 
                            TVIMPSTEnums::TFilterType aFilter ) const
	{
    return 1;
  	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FilterAllowsContact
// -----------------------------------------------------------------------------
//
TBool vimpststoragecontactlist_stub::FilterAllowsContact( const MVIMPSTStorageContact* aContact,
                             TVIMPSTEnums::TFilterType aFilter ) const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContact
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* vimpststoragecontactlist_stub::FindContact( const TDesC& aContactId )
    {
    //return NULL;
    }
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactByContactLinkL
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact* vimpststoragecontactlist_stub::FindContactByContactLink( const MVPbkContactLink& aContactLink )
    {
    //return NULL;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageContactList::FindContactByLinkL
// -----------------------------------------------------------------------------
//
TInt vimpststoragecontactlist_stub::FindContactByLinkL( const MVPbkContactLink& aContactLink,
                                          TInt& aIndexOrderedArray ) const
    {
    return NULL;
    }        
    
//  End of File
