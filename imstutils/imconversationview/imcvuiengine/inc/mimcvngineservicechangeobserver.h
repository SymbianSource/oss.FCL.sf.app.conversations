/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  service change observer
*
*/


#ifndef MIMCVENGINESERVICECHANGEOBSERVER_H
#define MIMCVENGINESERVICECHANGEOBSERVER_H

#include "mimcvenginechatlistobserver.h"  

class MIMCVEngineMessageReadInterface;

/**
 *  Observer class for getting service status change 
 *
 *  @lib imcvengine.lib
 */
class MIMCVEngineServiceChangeObserver 
    {

public:

    /*
     * new service has been installed
     */
    virtual void HandleNewServiceL( TInt aServiceId ) = 0 ;
    /*
     *  service has been deleted
     */
    virtual void HandleServiceDeletedL( TInt aServiceId )= 0 ;
        
    /**
     * @see handle the presence change notification
     * to handle the connection lost
     */
    virtual void HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId ) = 0 ;

    /**
     * Handle message send operation completion.
     * @param aResult result  to handle.
     */
    virtual void HandleOperationCompletdL(TInt aserviceId, MIMCVEngineMessage::TMessagerType aType , TInt aResult) = 0 ;

    /**
     * @see HandleConnectionTerminatedL
     * to handle the connection lost
     * @param aReason, reason report
     */
    virtual void HandleConnectionEventL(TInt aServiceId, TIMCVConnectionState aState, TInt aReason )  = 0  ;

    
    /**
     * Handle events.
     * @param aEvent Event to handle.
     * @param aContainerInfo, Container
     */
    virtual void HandleChatListEvent(TInt aServiceId, TChatListEventType aEvent, 
                                     MIMCVEngineMessageReadInterface* aContainerInfo
                                     ) = 0;


protected:

    virtual ~MIMCVEngineServiceChangeObserver()
        {

        }

    };



#endif // MIMCVENGINESERVICECHANGEOBSERVER_H
