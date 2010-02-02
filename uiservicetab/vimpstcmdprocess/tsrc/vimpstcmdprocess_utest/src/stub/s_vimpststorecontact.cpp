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

//  INCLUDES
#include "s_vimpststorecontact.h"

MVIMPSTStorageContact* vimpststorecontact_stub::NewL()
	{
	MVIMPSTStorageContact* contact = new(ELeave)vimpststorecontact_stub;
	return contact;
	}

 void vimpststorecontact_stub::SetNameL( const TDesC& aName ) 
    {
    
    }

 void vimpststorecontact_stub::SetAvatarContentL( const TDesC8& aAvatarContent ,
               MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler)
    {
    
    }

const TDesC& vimpststorecontact_stub::Name()const 
    {
    return KNullDesC();
    }
         
TInt vimpststorecontact_stub::AvatarIndex() const 
	{
		return 0;
	}

void vimpststorecontact_stub::SetAvatarIndex( TInt avatarIndex)
	{
		
	}

const TDesC8& vimpststorecontact_stub::AvatarContent() const 
	{
	return KNullDesC8();
	}

const TDesC& vimpststorecontact_stub::UserId() const 
	{
		return KDummy();
	}

TVIMPSTEnums::TOnlineStatus vimpststorecontact_stub::OnlineStatus() const 
	{
		TVIMPSTEnums::TOnlineStatus status;
		return status;
	}

void vimpststorecontact_stub::SetOnlineStatus( TVIMPSTEnums::TOnlineStatus aOnlineStatus ) 
	{
		
	}

void vimpststorecontact_stub::SignalChanges() 
	{
		
	}

void vimpststorecontact_stub::SignalAvatarChange() 
	{
		
	}

void vimpststorecontact_stub::SetUserIdL( const TDesC& aUserId ) 
	{
		
	}

const TDesC& vimpststorecontact_stub::StatusText() const 
	{
	return KDummy();
	}

void vimpststorecontact_stub::SetStatusTextL( const TDesC& aStatusText )
	{
		
	}

MVPbkContactLink* vimpststorecontact_stub::ContactLink() const
	{
		return NULL;
	}

MVPbkStoreContact* vimpststorecontact_stub::VPbkStoreContact() const
	{
		return NULL;
	}

RPointerArray<HBufC>& vimpststorecontact_stub::ServiceDetails() 
	{
		_LIT(KText1, "xyz");
		TBufC16<5> buf11(KText1);
		HBufC* buf1;
		TRAP_IGNORE(buf1 = buf11.AllocL());
		iServiceDetailArray.Append(buf1);
		TInt count  = iServiceDetailArray.Count();
		return iServiceDetailArray;
	}

const TDesC& vimpststorecontact_stub::ImppField() 
    {
    return KDummy();
    }
void vimpststorecontact_stub::Commit(MVPbkContactObserver* aObserver /*= NULL*/,
			MVIMPSTContactOperationCompleteObserver* aContactOperationObserver /*= NULL*/) 
	{
		
	}

void vimpststorecontact_stub::ResetAvatarContent() 
	{
		
	}
void vimpststorecontact_stub::SetContactOperationObserver( MVIMPSTContactOperationCompleteObserver* 
	    								aContactOperationObserver )
	{
		
	}
	
vimpststorecontact_stub::~vimpststorecontact_stub()
	{
	//buf1.	
	}

HBufC* vimpststorecontact_stub::GetContactFieldDataLC( const TDesC& aDefaultDomain /*= KNullDesC */,TInt aFieldResId /*= R_VPBK_FIELD_TYPE_IMPP*/)
    {
    HBufC* retVal= NULL;
    return retVal;
    }
// End of File
