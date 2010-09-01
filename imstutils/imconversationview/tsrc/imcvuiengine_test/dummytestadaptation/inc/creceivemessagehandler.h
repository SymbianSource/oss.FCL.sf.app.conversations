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
* Description:  Implementation of reicive message handler
*
*/


#ifndef __CRECEIVEMESSAGEHANDLER_H__
#define __CRECEIVEMESSAGEHANDLER_H__


//  INCLUDES
#include <e32base.h>
#include <ximpbase.h>



// FORWARD DECLARATIONS

class MOSSProtocolConnectionManager;
/*class MPEngXMLParser;
class MPEngXmlSerializer;*/

/**
 * CReceiveMessageHandler
 *
 *
 *
 * @lib ?library
 */

class CReceiveMessageHandler: public CActive


	{

	public:

		static CReceiveMessageHandler* NewL (
		    MOSSProtocolConnectionManager& aConnMan
		);


		virtual ~CReceiveMessageHandler();

	private:

		CReceiveMessageHandler ( MOSSProtocolConnectionManager& aConnMan );
		void ConstructL();
	
	private: // from CActive

		void DoCancel();
		void RunL();
		TInt RunError ( TInt aError );

	public:
        
    	/**
		 * start listening recieved massages
		 */    
		void StartListeningL();
    	/**
		 * parse the data and update the data cache of ximp.send event back to ximp
		 */
		void ProcessIncomingDataL();

	private: // data

		/**
		 * Send data request to pure data handler generates the id
		 */
		TInt iSendId;
		
		/**
		 * Oss Protocol Plugin Connection Manager
		 * Not own.  *** Write "Not own" if some other class owns this object.
		 */
		MOSSProtocolConnectionManager& iConnMan;

	};


#endif // __CRECEIVEMESSAGEHANDLER_H__

