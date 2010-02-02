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
* Description:  Interface for identity object.
*
*/


#include "s_xmppsettingsapi.h"
TBool setpassword = EFalse;

CXmppSettingsApi* CXmppSettingsApi::NewL()
    {
//   if (!myCSPSettings)
//	    {
//	    myCSPSettings = new ( ELeave ) CXmppSettingsApi;
//	    }
//    return myCSPSettings;
		return new ( ELeave ) CXmppSettingsApi;
    }

CXmppSettingsApi* CXmppSettingsApi::NewLC()
    {
    CXmppSettingsApi* settings = CXmppSettingsApi::NewL();
    CleanupStack::PushL( settings );
    return settings;
    }

CXmppSettingsApi::~CXmppSettingsApi()
		{
			
		}

void CXmppSettingsApi::CreateSettingsRecordL( TUint32& aSettingsRecordId )
		{
			
		}

void CXmppSettingsApi::RemoveSettingsRecordL( TUint32 aSettingsRecordId )
		{
			
		}

void CXmppSettingsApi::DefaultSettingsRecordL( TUint32& aSettingsRecordId )
		{
			
		}
		
void CXmppSettingsApi::GetSettingsRecordIdArrayL( RArray<TUint32>& aArray )
		{
			
		}

void CXmppSettingsApi::SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             const TDesC& aAttrValue )
		{
			
		}

void CXmppSettingsApi::SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TInt aAttrValue )
		{
			
		}

void CXmppSettingsApi::GetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TDes& aAttrValue )
         {
         if(setpassword)
             {
             _LIT(KText,"password");
             aAttrValue.Copy(KText);
             }
         else
             {
             _LIT(KText,"");
             aAttrValue.Copy(KText);
             }
         }

void CXmppSettingsApi::GetParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName,
                                TInt& aAttrValue )
		{
			
		}

void CXmppSettingsApi::RemoveParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName )
		{
			
		}

	
void CXmppSettingsApi::GetRepositoryKeyL( TUint32 aSettingsRecordId,
                       const TDesC& aAttrName,
                       TBool aCreateColumnIfNotExist,
                       TUint32& aTheKey )
		{
			
		}
