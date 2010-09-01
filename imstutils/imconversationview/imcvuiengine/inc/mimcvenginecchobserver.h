/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer class
*
*/



#ifndef MIMCVENGINE_CCH_OBSERVERH
#define MIMCVENGINE_CCH_OBSERVERH


    
/**
 *  Observer class for getting service status change messages
 *
 *  @lib imcvengine.lib
 */
class MIMCVEngineCCHObserver 
    {

public:
	/// Process states
        enum TServiceState
            {
            ELogin,     
            EConnecting,                   
            EDisconnecting,
            ENotLoggedIn
            };
            
    /**
     * Signaled when service status or error changes
     *
     * @param aServiceId Id of the service     
     * @param aServiceStatus Service status
     */
    virtual void ServiceStatusChanged( TInt aServiceId,     								   
                                       TServiceState aServiceStatus ) = 0;

    };



#endif // MIMCVENGINE_CCH_OBSERVERH
