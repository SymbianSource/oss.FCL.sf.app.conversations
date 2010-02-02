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
* Description:  Contact list container implementation
*
*/



// INCLUDE FILES
#include "s_vimpststoragecontactlist.h"

    
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::ListId
// -----------------------------------------------------------------------------
//
const TDesC& MyVIMPSTStorageContactListStub::ListId() const
    {
    return KNullDesC;     
    }
    
        
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::DisplayName
// -----------------------------------------------------------------------------
//
TPtrC MyVIMPSTStorageContactListStub::DisplayName() const
    {
    const TDesC& des ( KNullDesC ) ;
    return des;  
    }
    
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::SetDisplayName
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactListStub::SetDisplayNameL( const TDesC& /*aDisplayName*/ )
    {
   
    }
 

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::Sort
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactListStub::Sort()
    {
   
    }

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::ResortContact
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactListStub::ResortContact( MVIMPSTStorageContact* /*aContact*/ )
    {
    }

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::ContactFindById
// -----------------------------------------------------------------------------
//
void MyVIMPSTStorageContactListStub::ResortUnKnownContact( MVIMPSTStorageContact* /*aContact*/ )
    {
    }
    
   
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::FindIndexOfContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt MyVIMPSTStorageContactListStub::FindIndexOfContact( const MVIMPSTStorageContact* /*aContact*/,
                            TVIMPSTEnums::TFilterType /*aFilter*/  ) const
    {    
    return KErrNotFound;
    }
       
// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::Count
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
TInt MyVIMPSTStorageContactListStub::Count() const
    {
    return 0;
    }


// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::operator[]
// From MVIMPSTStorageContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& MyVIMPSTStorageContactListStub::operator[]( TInt /*aIndex */) const
    {
	MVIMPSTStorageContact* contact = NULL;
	return *contact;
    }


// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::FilteredContact
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
MVIMPSTStorageContact& MyVIMPSTStorageContactListStub::FilteredContact( 
                TInt /*aIndex*/, TVIMPSTEnums::TFilterType /*aFilter*/ ) const
	{
    MVIMPSTStorageContact* contact = NULL;
	return *contact;
	}

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::FilteredCount
// From MVIMPSTContactList
// -----------------------------------------------------------------------------
//
TInt MyVIMPSTStorageContactListStub::FilteredCount( 
                            TVIMPSTEnums::TFilterType /*aFilter*/ ) const
	{
    return 0;
	}

// -----------------------------------------------------------------------------
// MyVIMPSTStorageContactListStub::FilterAllowsContact
// -----------------------------------------------------------------------------
//
TBool MyVIMPSTStorageContactListStub::FilterAllowsContact( const MVIMPSTStorageContact* /*aContact*/,
                             TVIMPSTEnums::TFilterType /*aFilter*/ ) const
    {
    TBool showContact( EFalse );
    
    return showContact;   
    }
//  End of File
