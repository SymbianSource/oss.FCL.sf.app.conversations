/*
* s_presencebuddyinfo2.h :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencebuddyinfo2.h
*
*/
#ifndef MPRESENCEBUDDYINFO2STUB_H
#define MPRESENCEBUDDYINFO2STUB_H

#include <mpresencebuddyinfo2.h>

class MPresenceBuddyInfo2Stub : public MPresenceBuddyInfo2,
                                public CBase
    {
public:
    static MPresenceBuddyInfo2Stub* NewStubL();

    void SetIdentityL( const TDesC& aBuddyId );

    TPtrC BuddyId( ) const ;        

    void SetAvailabilityL( 
            TAvailabilityValues aAvailability,
            const TDesC& aAvailabilityText ) ;

    TAvailabilityValues Availability( ) ;         

    TPtrC AvailabilityText( );           


    void SetAvatarL( const TDesC8& aAvatar ) ;

    TPtrC8 Avatar( ) ;             

    void SetStatusMessageL( 
            const TDesC& aStatusMessage ) ; 

    TPtrC StatusMessage( ) ;     

    void SetAnyFieldL( 
            const TDesC16& aKey,
            const TDesC8& aValue ) ;

    TPtrC8 GetAnyField( 
            const TDesC16& aKey ); 

    void GetFieldKeysL( 
            CDesCArrayFlat& aKeys ) ;

    void RemoveField(const TDesC& aKey );      

    TBool EqualsIdentity(
            const MPresenceBuddyInfo2& aOtherInstance ) const ;

    void ExternalizeL( RWriteStream& aStream ) const ;

    void InternalizeL( RReadStream& aStream ) ;   
    ~MPresenceBuddyInfo2Stub()
                {
                iIds.ResetAndDestroy();
                iIds.Close();
                iValues.ResetAndDestroy();
                iValues.Close();
                }
    
private:
    RPointerArray<HBufC> iIds;  
    RPointerArray<HBufC8> iValues; 
    };



#endif //MPRESENCEBUDDYINFO2STUB_H



