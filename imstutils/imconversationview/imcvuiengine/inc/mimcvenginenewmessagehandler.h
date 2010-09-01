/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  interface for message handling 
*
*/

#ifndef MIMCVENGINESERVICESESSIONHANDLER_H
#define MIMCVENGINESERVICESESSIONHANDLER_H

//  INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <bamdesca.h>

#include "imcvuiliterals.h"

// CLASS DECLARATION
class TXIMPRequestId;
/**
 *  Observer interface to handle message container changes.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
//  FORWARD CLASS DECLERATIONS

class MIMCVEngineServiceSessionHandler
    {

public: 

    /**
     * @see HandleDeliveryReportL
     * to handle the new message delivery report
     * @param aResultError, delivery report
     * @param aText, msg to send
     */
    virtual void HandleDeliveryReportL(TXIMPRequestId aOperatioCode, TInt aResult ) = 0 ;

    /**
     * @see HandleConnectionTerminatedL
     * to handle the connection lost
     * @param aReason, reason report
     */
    virtual void HandleConnectionEventL( TIMCVConnectionState aState, TInt aReason ) = 0 ;

public: // For protection.

    /**
     * Destructor for protection
     */
    virtual ~MIMCVEngineServiceSessionHandler(){}
    };

/**
 *  Observer interface to handle message container changes.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineServiceSessionObserver
	{

	public: 
			 /**
			 * register MIMCVEngineServiceSessionHandler class to listen the event
			 */
			virtual void RegisterObserver(MIMCVEngineServiceSessionHandler* aObserver) = 0 ;
			/**
			 * unregister MIMCVEngineServiceSessionHandler class to avoid the event
			 */
			virtual void UnRegisterObserver(MIMCVEngineServiceSessionHandler* aObserver) = 0 ;
        
	public: // For protection.

		/**
		 * Destructor for protection
		 */
		virtual ~MIMCVEngineServiceSessionObserver(){}
	};
#endif      // MIMCVENGINESERVICESESSIONHANDLER_H

// End of File
