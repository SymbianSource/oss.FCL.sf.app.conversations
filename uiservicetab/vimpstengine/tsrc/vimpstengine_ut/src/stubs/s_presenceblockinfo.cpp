/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence block info object.
*
*/

#include "s_presenceblockinfo.h"
#include "s_ximpidentity.h"

extern MyMXIMPIdentityStub gMXIMPIdentity; // already defined in MPresentityPresenceEventStub

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// CPresenceBlockInfoImp::BlockedEntityId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& MPresenceBlockInfoStub::BlockedEntityId() const
    {
    return gMXIMPIdentity; ;
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// MPresenceBlockInfoStub::BlockedEntityDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& MPresenceBlockInfoStub::BlockedEntityDisplayName() const
    {
    return KNullDesC();
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// MPresenceBlockInfoStub::SetBlockedEntityIdL()
// ---------------------------------------------------------------------------
//
void MPresenceBlockInfoStub::SetBlockedEntityIdL( 
    MXIMPIdentity* /*aIdentity*/ )
    {
    /*CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; */
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// MPresenceBlockInfoStub::SetBlockedEntityDisplayNameL()
// ---------------------------------------------------------------------------
//
void MPresenceBlockInfoStub::SetBlockedEntityDisplayNameL( 
    const TDesC16& /*aDisplayName*/ )
    {
    /*HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );*/
    }
    

const TAny* MPresenceBlockInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceBlockInfoStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceBlockInfoStub::GetInterfaceId() const
    {
    return 0;
    }


// End of file
