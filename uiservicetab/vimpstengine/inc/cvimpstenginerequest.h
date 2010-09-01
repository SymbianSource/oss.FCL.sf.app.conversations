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
* Description: implements the request handler for ximp fw.
*
*/



#ifndef _CVIMPSTENGINEREQUEST_H__
#define _CVIMPSTENGINEREQUEST_H__

// include files
#include <e32base.h>
#include <ximpbase.h>
#include "tvimpstenums.h"

/**
 *  This class implements the request handler for ximp fw.
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS(CVIMPSTEngineRequest) :public CBase
	{

	public:  // Two-phased constructors and destructor

       /**
		* Two-phased constructor.
		* @param aRequestId request id
		* @param aRequestType - Enum defined in tvimpstenums.h
		*/
		static CVIMPSTEngineRequest* NewL(TXIMPRequestId& aRequestId,
		        TXimpOperation aRequestType );


       /**
		* Two-phased constructor.
		* @param aRequestId request id
		*/
		static CVIMPSTEngineRequest* NewLC(TXIMPRequestId& aRequestId,
		        TXimpOperation aRequestType);

		/**
		* ~CVIMPSTEngineRequest 
		*/
		virtual ~CVIMPSTEngineRequest();

		/**
		* StartWait
		*/
		void StartWait();

		/**
		* StopWait method 
		*/
		void StopWait();

		/**
		* GetRequestId  
		* @return TXIMPRequestId the request id 
		*/
		const TXIMPRequestId& GetRequestId() const;

		/**
		* IsWaitStarted method from context obsrver  
		* check weather wait is started or not
		* @return TBool ETrue if wait is started, else EFalse.
		*/
		TBool IsWaitStarted() ;		
		
		/**
         * Set the Type of Request
         * @param aType, a request type - Enum defined in tvimpstenums.h
         */
        void SetRequestType(TXimpOperation aType) ;
        
        
        /**
         * Get the Type of Request
         * @return request type - Enum defined in tvimpstenums.h
         */
        TXimpOperation RequestType() const;
	
	private:


		/**
		* CVIMPSTEngineRequest 
		* @ param aRequestId ,request id 
		* @ param aType, request type.
		*/
		CVIMPSTEngineRequest(TXIMPRequestId& aRequestId,
		        TXimpOperation aRequestType);

	private:
		// own request id
		TXIMPRequestId  iRequestId;

		// own active sheduler 
		CActiveSchedulerWait    iWait;
		
		//request type
		TXimpOperation iReqType ;
		
};

#endif  //_CVIMPSTENGINEREQUEST_H__

// end of file
