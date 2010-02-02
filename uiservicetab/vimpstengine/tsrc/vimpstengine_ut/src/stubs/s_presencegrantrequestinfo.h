/*
* s_presencegrantrequestinfo.h :Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencegrantrequestinfo.h
*
*/
#ifndef MPRESENCEGRANTREQUESTINFOSTUB
#define MPRESENCEGRANTREQUESTINFOSTUB

#include <presencegrantrequestinfo.h>

/**
 * Interface for presence grant request info object.
 *
 * Presence grant request info object presents single
 * presence grant request entry in the presence
 * grant request list. Presence grant request info
 * object holds the identity and display name information
 * for the presence grant request.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceGrantRequestInfoStub : public MPresenceGrantRequestInfo
    {
public:
    static MPresenceGrantRequestInfo* NewL();
    const MXIMPIdentity& RequestorId() const;

    const TDesC16& RequestorDisplayName() const;

    void SetRequestorIdL( MXIMPIdentity* aIdentity );

    void SetRequestorDisplayNameL( const TDesC16& aDisplayName );
    
    const TAny* GetInterface( TInt32 aInterfaceId, TIfGetOps aOps ) const;

    TAny* GetInterface( TInt32 aInterfaceId, TIfGetOps aOps );
 
    TInt32 GetInterfaceId() const;
     
    };

#endif
