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
* Description:  implementation of single request to fw
*
*/



#ifndef _CICPSERVICEREQUEST_H__
#define _CICPSERVICEREQUEST_H__

// include files
#include <e32base.h>
#include <ximpbase.h>

#include "timconnproviderenums.h"

/**
* class declaration
* @imconnectionprovider.dll
*/

class CICPServiceRequest :public CBase
	{

	public:  // Two-phased constructors and destructor

		/**
		* create the instance of this class
		* @param aRequestId, request id to use
		* @param aType, a request type
		* @return instance of this class
		*/
		static CICPServiceRequest* NewL(TXIMPRequestId& aRequestId,
								TIMConnProviderEnums::TRequestTypes aType);

		/**
		* create the instance of this class
		* @param aRequestId, request id to use
		* @param aType, a request type
		* @return instance of this class
		*/
		static CICPServiceRequest* NewLC(TXIMPRequestId& aRequestId,
								TIMConnProviderEnums::TRequestTypes aType);

		/**
		* ~CICPServiceRequest 
		*/
		virtual ~CICPServiceRequest();

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
		* @return the request id 
		*/
		const TXIMPRequestId& GetRequestId() const;

		/**
		* IsWaitStarted method from context obsrver  
		* check weather wait is started or not
		* @return flage
		*/
		TBool IsWaitStarted() ;
		
		/**
		* Set the Type of Request
		* @param aIsLoginReq, a request type
		*/
		void SetRequestType(TIMConnProviderEnums::TRequestTypes aIsLoginReq) ;
		
		
		/**
		* Get the Type of Request
		* @return request type
		*/
		TIMConnProviderEnums::TRequestTypes RequestType() const;

	private:

		/**
		* ConstructL   
		*/
		void ConstructL();

		/**
		* CICPServiceRequest 
		* @param aRequestId ,request id 
		* @param aType ,request type 
		*/
		CICPServiceRequest(TXIMPRequestId& aRequestId,
						TIMConnProviderEnums::TRequestTypes aType);

	private: //data
	
		// own request id
		TXIMPRequestId  iRequestId;

		// own active sheduler 
		CActiveSchedulerWait    iWait;
		
		//request type
		TIMConnProviderEnums::TRequestTypes iReqType ;
};

#endif  //_CICPSERVICEREQUEST_H__

// end of file
