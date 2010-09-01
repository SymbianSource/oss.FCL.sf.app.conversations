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

#ifndef MyPresentityGroupsStub_H
#define MyPresentityGroupsStub_H

#include <e32std.h>
#include <ximpbase.h>
#include <presentitygroups.h>

class MXIMPIdentity;


/**
 * Presentity group handling interface for clients use.
 * Interface is implemented by the XIMP Framework.
 *
 * MPresentityGroups provides services for
 * XIMP Framework clients to manage presentity groups
 * and presentity group members. It provides also services
 * to subscribe notifications from presentity group list changes
 * and from individual presentity group changes.
 *
 * Client can retrieve MPresentityGroups interface from
 * MXIMPContext interface.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MyPresentityGroupsStub : public MPresentityGroups, public CBase
    {

public:		
    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    ~MyPresentityGroupsStub();


    /**
     * Subscribes presentity group list notifications.
     *
     * @return The request ID identifying the issued request.
     */
    TXIMPRequestId SubscribePresentityGroupListL() ;



    /**
     * Unsubscribes presentity group list notifications.
     *
     * @return The request ID identifying the issued request.
     */
    TXIMPRequestId UnsubscribePresentityGroupListL() ;



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
    TXIMPRequestId CreatePresentityGroupL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName ) ;


    /**
     * Deletes presentity group.
     *
     * @param [in] aGroupId
     *        Identification for presentity group.
     *
     * @return The request ID identifying the issued request.
     */
    TXIMPRequestId DeletePresentityGroupL(
                    const MXIMPIdentity& aGroupId ) ;



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
    TXIMPRequestId UpdatePresentityGroupDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName ) ;



public: //Prototypes for presentity group member management requests


    /**
     * Subscribes presentity group content notifications.
     *
     * @param [in] aGroupId
     *        Identification for presentity group for which
     *        to subscribe content notifications.
     *
     * @return The request ID identifying the issued request.
     */
    TXIMPRequestId SubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId ) ;


    /**
     * Unsubscribes presentity group content notifications.
     *
     * @param [in] aGroupId
     *        Identification for presentity group for which
     *        content notification subscription to terminate.
     *
     * @return The request ID identifying the issued request.
     */
    TXIMPRequestId UnsubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId ) ;


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
    TXIMPRequestId AddPresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName ) ;


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
    TXIMPRequestId RemovePresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId ) ;



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
    TXIMPRequestId UpdatePresentityGroupMemberDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName ) ;
                    
	const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;                    

    };



#endif // MyPresentityGroupsStub_H



