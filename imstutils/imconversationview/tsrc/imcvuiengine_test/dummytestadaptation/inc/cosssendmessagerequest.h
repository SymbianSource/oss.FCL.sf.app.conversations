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
* Description:  Oss Protocol send massage request
*
*/


#ifndef __COSSSENDMESSAGEREQUEST_H__
#define __COSSSENDMESSAGEREQUEST_H__


//  INCLUDES
#include <e32base.h>
#include <ximpbase.h>
#include "ossprotocloliterals.h"

// FORWARD DECLARATIONS

class MOSSProtocolConnectionManager;

class MImConversationInfo;
/**
 * COSSSendMessageRequest
 *
 * Issue Send Message Request to Network Server
 *
 * @lib ?library
 * @since S60 v4.0
 */

class COSSSendMessageRequest: public CActive


	{

	public:

		static COSSSendMessageRequest* NewL (
		    MOSSProtocolConnectionManager& aConnMan,
		    TXIMPRequestId aRequestId );

		static COSSSendMessageRequest* NewLC (
		    MOSSProtocolConnectionManager& aConnMan,
		    TXIMPRequestId aRequestId );

		virtual ~COSSSendMessageRequest();

	private:

		COSSSendMessageRequest (
		    MOSSProtocolConnectionManager& aConnMan,
		    TXIMPRequestId aRequestId  );
		void ConstructL();

	private: // from CActive

		void DoCancel();
		/*
		 * RunL() function also deletes the request object once the activity is done
		 *
		 */
		void RunL();
		TInt RunError ( TInt aError );

	public:
		/**
		  * Send message to the recipients
		  * @param aImMessage stucture stores message,recipients
		*/
		void SendMessageL ( const MImConversationInfo& aImMessage );

	private: // data

		/**
		 * Request Id from PrFw
		 */
		TXIMPRequestId iRequestId;
		/**
		 * Send data request to pure data handler generates the id
		 */
		TInt iSendId;
	
		HBufC16* iListId;
		/**
		 * Oss Protocol Plugin Connection Manager
		 * Not own.  *** Write "Not own" if some other class owns this object.
		 */
		MOSSProtocolConnectionManager& iConnMan;

	};


#endif // __COSSSENDMESSAGEREQUEST_H__

