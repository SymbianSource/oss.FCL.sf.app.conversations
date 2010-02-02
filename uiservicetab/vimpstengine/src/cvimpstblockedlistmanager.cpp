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
* Description:  manages the locally maintained blocked list
*
*/
// INCLUDE FILES
#include "cvimpstblockedlistmanager.h"
#include "vimpstdebugtrace.h"


// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTServiceListManager::NewL
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTBlockedListManager* CVIMPSTBlockedListManager::NewL()
    {
    TRACE( T_LIT("CVIMPSTBlockedListManager::NewL start"));
    CVIMPSTBlockedListManager* self = 
    			CVIMPSTBlockedListManager::NewLC();
    CleanupStack::Pop( self );
   	TRACE( T_LIT("CVIMPSTBlockedListManager::NewL end"));
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTBlockedListManager::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTBlockedListManager* CVIMPSTBlockedListManager::NewLC()
	{
	TRACE( T_LIT("CVIMPSTBlockedListManager::NewLC start"));
	CVIMPSTBlockedListManager* self = new (ELeave) CVIMPSTBlockedListManager;
	CleanupStack::PushL( self );
	self->ConstructL();
	TRACE( T_LIT("CVIMPSTBlockedListManager::NewLC end"));
	return self;
	}
    
// ---------------------------------------------------------
// CVIMPSTBlockedListManager::ConstructL
// ---------------------------------------------------------
void CVIMPSTBlockedListManager::ConstructL()
	{
	TRACE( T_LIT("CVIMPSTBlockedListManager::ConstructL start"));
    TRACE( T_LIT("CVIMPSTBlockedListManager::ConstructL end"));    	
    }
	
// ---------------------------------------------------------
// CVIMPSTBlockedListManager::CVIMPSTBlockedListManager
// ---------------------------------------------------------	
CVIMPSTBlockedListManager::CVIMPSTBlockedListManager()
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTBlockedListManager::~CVIMPSTBlockedListManager
// ---------------------------------------------------------
CVIMPSTBlockedListManager::~CVIMPSTBlockedListManager()
    {
	iBlockedList.ResetAndDestroy();	
	iBlockedList.Close();
    }

// ---------------------------------------------------------
// CVIMPSTBlockedListManager::AddToBlockedListL
// ---------------------------------------------------------
void CVIMPSTBlockedListManager::AddToBlockedListL(const TDesC& aContact)
	{
	HBufC* contact = aContact.AllocL();
	iBlockedList.Append(contact);//owner ship tranfered.
	}

// ---------------------------------------------------------
// CVIMPSTBlockedListManager::AddToBlockedList
// ---------------------------------------------------------
void CVIMPSTBlockedListManager::RemoveFromBlockListL(const TDesC& aContact)
	{
	for(TInt i=0; i<iBlockedList.Count(); ++i)
		{
		if(0 == aContact.CompareC(iBlockedList[i]->Des()))
			{
			HBufC* unblocked = iBlockedList[i];
			iBlockedList.Remove(i);
			delete unblocked ;
			}
		}
	}


// ---------------------------------------------------------
// CVIMPSTBlockedListManager::BlockedList
// ---------------------------------------------------------
RPointerArray<HBufC>* CVIMPSTBlockedListManager::BlockedList()
	{
	return &iBlockedList;
	}
	
// ---------------------------------------------------------
// CVIMPSTBlockedListManager::ResetL
// ---------------------------------------------------------
void CVIMPSTBlockedListManager::ResetL()
	{
	iBlockedList.ResetAndDestroy();	
	}

	

// end of file
