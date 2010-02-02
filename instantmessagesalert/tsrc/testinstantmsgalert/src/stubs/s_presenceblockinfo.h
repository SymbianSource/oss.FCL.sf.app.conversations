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

#ifndef MXIMPFWPRESENCEBLOCKINFOSTUB_H
#define MXIMPFWPRESENCEBLOCKINFOSTUB_H

#include <presenceblockinfo.h> 



/**
 * Interface for presence block info object.
 *
 * Presence block info object presents single
 * presence block entry in the presence block list.
 * Presence block info object holds the identity
 * and display name information for the presence
 * block entry.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceBlockInfoStub :public MPresenceBlockInfo 
    {

public:

    /**
     * Returns identity of the blocked entity.
     *
     * @return The identity of the blocked entity.
     */
    const MXIMPIdentity& BlockedEntityId() const;



    /**
     * Returns displayname of the blocked entity.
     *
     * @return The displayname of the blocked entity.
     */
     const TDesC16& BlockedEntityDisplayName() const;



    /**
     * Sets identity of the blocked entity.
     *
     * @param [in] aIdentity
     *        Identity of the blocked entity.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
     void SetBlockedEntityIdL(
                MXIMPIdentity* aIdentity );


    /**
     * Sets displayname of the blocked entity.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    void SetBlockedEntityDisplayNameL(
                const TDesC16& aDisplayName );
    
    const TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps ) const;

        TAny* GetInterface(
                            TInt32 aInterfaceId,
                            TIfGetOps aOps );

        TInt32 GetInterfaceId() const;

    };


#endif // MXIMPFWPRESENCEBLOCKINFOSTUB_H



