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
* Description:  Presentity group handling interface for clients use.
*
*/


#include "s_presentitygroups.h"

/**
 * Protected destructor.
 * Object instancies can't be deleted via this interface.
 */
MyPresentityGroupsStub::~MyPresentityGroupsStub() {}

//MyPresentityGroupsStub::MyPresentityGroupsStub {}

/**
 * Subscribes presentity group list notifications.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::SubscribePresentityGroupListL() 
    {
    TXIMPRequestId id;
    return id;
    }



/**
 * Unsubscribes presentity group list notifications.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::UnsubscribePresentityGroupListL() 
    {
    TXIMPRequestId id;
    return id;
    }



/**
 * Creates presentity group.
 *
 * Note: Remote service may alter given presentity group ID and
 *       display name. Altered ID and display names are signalled back
 *       to client through MXIMPRequestCompleteEvent.
 *
 * @param [in] aGroupId
 *        Identification for presentity group.
 *
 * @param [in] aDisplayName
 *        Presentity group display name.
 *
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::CreatePresentityGroupL(
                const MXIMPIdentity& /*aGroupId*/,
                const TDesC16& /*aDisplayName*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }


/**
 * Deletes presentity group.
 *
 * @param [in] aGroupId
 *        Identification for presentity group.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::DeletePresentityGroupL(
                const MXIMPIdentity& /*aGroupId*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }



/**
 * Updates presentity group display name.
 *
 * Note: Remote service may alter given presentity group display name.
 *       Altered display name is signalled back to client through
 *       MXIMPRequestCompleteEvent.
 *
 * @param [in] aGroupId
 *        Identification for presentity group.
 *
 * @param [in] aDisplayName
 *        New display name for the presentity group.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::UpdatePresentityGroupDisplayNameL(
                const MXIMPIdentity& /*aGroupId*/,
                const TDesC16& /*aDisplayName*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }


/**
 * Subscribes presentity group content notifications.
 *
 * @param [in] aGroupId
 *        Identification for presentity group for which
 *        to subscribe content notifications.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::SubscribePresentityGroupContentL(
                const MXIMPIdentity& /*aGroupId*/ )
    {
    TXIMPRequestId id;
       return id;
    }


/**
 * Unsubscribes presentity group content notifications.
 *
 * @param [in] aGroupId
 *        Identification for presentity group for which
 *        content notification subscription to terminate.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::UnsubscribePresentityGroupContentL(
                const MXIMPIdentity& /*aGroupId*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }


/**
 * Requests the protocol to add new member to
 * identified presentity group.
 *
 *
 * Note: Remote service may alter given member and
 *       display name. Altered ID and display names
 *       are signalled back to client through MXIMPRequestCompleteEvent.
 *
 * @param [in] aGroupId
 *        Identification of the presentity group where to
 *        add new member.
 *
 * @param [in] aMemberId
 *        Identification of the new member.
 *
 * @param [in] aMemberDisplayName
 *        Display name for the added member.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::AddPresentityGroupMemberL(
                const MXIMPIdentity& /*aGroupId*/,
                const MXIMPIdentity& /*aMemberId*/,
                const TDesC16& /*aMemberDisplayName*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }


/**
 * Requests the XIMPFw to remove member from
 * identified presentity group.
 *
 * @param [in] aGroupId
 *        Identification of the presentity group from
 *        where to remove member.
 *
 * @param [in] aMemberId
 *        Identification of the member to remove.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::RemovePresentityGroupMemberL(
                const MXIMPIdentity& /*aGroupId*/,
                const MXIMPIdentity& /*aMemberId*/ )
    {
    TXIMPRequestId id;
       return id;
    }



/**
 * Requests the XIMPFw to update presentity group
 * member display name.
 *
 * Note: Remote service may alter given presentity group member
 *       display name. Altered display name is signalled back
 *       to client through MXIMPRequestCompleteEvent.
 *
 * @param [in] aGroupId
 *        Identification for presentity group.
 *
 * @param [in] aMemberId
 *        Identification of the member which display
 *        name to update.
 *
 * @param [in] aMemberDisplayName
 *        New display name for the member.
 *
 * @return The request ID identifying the issued request.
 */
TXIMPRequestId MyPresentityGroupsStub::UpdatePresentityGroupMemberDisplayNameL(
                const MXIMPIdentity& /*aGroupId*/,
                const MXIMPIdentity& /*aMemberId*/,
                const TDesC16& /*aMemberDisplayName*/ ) 
    {
    TXIMPRequestId id;
       return id;
    }

    

const TAny* MyPresentityGroupsStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MyPresentityGroupsStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MyPresentityGroupsStub::GetInterfaceId() const
    {
    return 0;
    }
