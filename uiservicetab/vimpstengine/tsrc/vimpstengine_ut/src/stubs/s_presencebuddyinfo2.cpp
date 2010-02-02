/*
* s_presencebuddyinfo2.cpp : Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencebuddyinfo2.cpp
*
*/
#include "s_presencebuddyinfo2.h"
#include <avabilitytext.h>

#define KAvailability  _L("online");
#define KMsg  _L("hello");
#define KBuddyId  _L("gizmo:Sharath");
#define KAvatar _L8("gshhshshhshshhsjskssiisisisisiisks");

//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
 MPresenceBuddyInfo2Stub* MPresenceBuddyInfo2Stub:: NewStubL()
    {
    
    return new(ELeave) MPresenceBuddyInfo2Stub;
    }

//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::BuddyId
//------------------------------------------------------------
TPtrC MPresenceBuddyInfo2Stub::BuddyId( ) const 
    {
    return KBuddyId;
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::Availability
//------------------------------------------------------------
MPresenceBuddyInfo2::TAvailabilityValues  MPresenceBuddyInfo2Stub::Availability( ) 
    {
    return EUnknownAvailability;
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::AvailabilityText
//------------------------------------------------------------
TPtrC MPresenceBuddyInfo2Stub::AvailabilityText( )
    {
    return KAvailability;
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::Avatar
//------------------------------------------------------------
TPtrC8 MPresenceBuddyInfo2Stub::Avatar( ) 
    {
    return KAvatar;
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
TPtrC MPresenceBuddyInfo2Stub::StatusMessage( ) 
    {
    return KMsg;
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::SetIdentityL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::SetIdentityL( const TDesC& /*aBuddyId*/ )
    {

    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::SetAvailabilityL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::SetAvailabilityL( 
        MPresenceBuddyInfo2::TAvailabilityValues /*aAvailability*/,
        const TDesC& /*aAvailabilityText*/ ) 
    {

    }


//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::SetAvatarL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::SetAvatarL( const TDesC8& /*aAvatar*/ ) 
    {

    }

//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::SetStatusMessageL( 
        const TDesC& /*aStatusMessage*/ ) 
    {

    }

//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::SetAnyFieldL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::SetAnyFieldL( 
        const TDesC16& aKey,
        const TDesC8& aValue ) 
    {
    HBufC8* valueBuffer = aValue.AllocLC();  
    HBufC* keyBuffer = aKey.AllocLC();  
       
   TInt insertPos = iIds.Find(0);
   if ( insertPos < 0 )
       {
       insertPos = iIds.Count();
       iIds.Append( keyBuffer );
       iValues.Append( valueBuffer );
       }
   else
      {
      iIds[insertPos] = keyBuffer;
      iValues[insertPos] = valueBuffer;
      }
    CleanupStack::Pop( keyBuffer );
    CleanupStack::Pop( valueBuffer );  
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
TPtrC8 MPresenceBuddyInfo2Stub::GetAnyField( 
        const TDesC16& aKey )
    {
    HBufC* key = aKey.AllocLC();
    HBufC8* val = NULL; 
    TInt count = iIds.Count();
    for(TInt  i = 0; i < count; i++)
        {
         if(iIds[i]->Des().Compare(KExtensionKey) == 0)  
            {       
            val = iValues[i];  
            break;
            } 
        }
    CleanupStack::PopAndDestroy(key);
    // since there is no destructor for this test class, the member variable are destroyed here.
    // as they are not needed any further, if these member variables are required further then appropriate destructor 
    // should be added.
    // iIds.ResetAndDestroy();
    // iIds.Close();
    // iValues.ResetAndDestroy();
    // iValues.Close();
    return val ? val->Des() : TPtrC8();  
    
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::GetFieldKeysL( 
        CDesCArrayFlat& aKeys ) 
    {
    aKeys.Reset();
    aKeys.AppendL(KExtensionKey );
     
    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::RemoveField(const TDesC& /*aKey*/ ) 
    {

    }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
TBool MPresenceBuddyInfo2Stub::EqualsIdentity(
        const MPresenceBuddyInfo2& /*aOtherInstance*/ ) const 
        {
        return ETrue;
        }
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::ExternalizeL( RWriteStream& /*aStream*/ ) const 
{

}
//------------------------------------------------------------
// MPresenceBuddyInfo2Stub::NewStubL
//------------------------------------------------------------
void MPresenceBuddyInfo2Stub::InternalizeL( RReadStream& /*aStream*/ ) 
    {

    }




