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
* Description:  message handler observer
*
*/

#ifndef MIMCVENGINEMESSAGEHANDLEROBSERVER_H
#define MIMCVENGINEMESSAGEHANDLEROBSERVER_H

//  INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "imcvuiliterals.h"
#include "mimcvenginemessage.h"
//	FORWARD CLASS DECLERATIONS
    
     
// CLASS DECLARATION
/**
 *  presence handler interface
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageHandlerObserver
    {
    public: 
    
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
    
    protected: // For protection.
    
        /**
        * Destructor for protection
        */
        virtual ~MIMCVEngineMessageHandlerObserver(){}
        
};
#endif      // MIMCVENGINEHANDLEROBSERVER_H

// End of File
