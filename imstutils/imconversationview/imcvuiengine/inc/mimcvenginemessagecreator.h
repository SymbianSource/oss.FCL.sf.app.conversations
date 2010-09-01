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
* Description:  message creater interface
*
*/


#ifndef __MIMCVENGINEMESSAGECREATOR_H__
#define __MIMCVENGINEMESSAGECREATOR_H__

#include <e32base.h>
#include <bamdesca.h>

#include "mimcvenginemessage.h"
//	FORWARD CLASS DECLERATIONS

// CLASS DECLARATION

/**
 *  Interface for creating message
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageCreator
	{
	public: // Definitions
		
		/**
		 * SMessageData
		 * Struct to contain data of message
		 */	
		struct SMessageData
			{
			TInt iVersion;
			const TDesC& iSenderId;
			const TDesC& iTargetId;
			const TDesC& iText;
			const MIMCVEngineMessage::TContentType iContentType;
			MIMCVEngineMessage::TMessagerType iMessager;
			};

	public:

        /**
         * Create message.
         * @param aData, Structure to contain data for message.
         * @return New message.
         */
		virtual MIMCVEngineMessage* CreateMessageL( const SMessageData& aData ) const = 0;
	
		/**
		 * Create failed text message.
		 * @param MIMCVEngineMessage, which has been failed to deliver
		 * @return new MIMCVEngineMessage. Ownership is passed to caller
		 */
		virtual MIMCVEngineMessage* CreateFailedMessageL( 
                                     const MIMCVEngineMessage* pSource ) const = 0 ;
	       
		/**
		 * Virtual destructor
		 */
		virtual ~MIMCVEngineMessageCreator(){}
	}; 

/// Current version of message creator messages.
const TInt KMessageDataVersion = 0;

#endif      // __MIMCVENGINEMESSAGECREATOR_H__

// End of File
