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

// INCLUDE FILES
#include "cicpservicerequest.h"

#include <e32base.h>
#include <ximpbase.h>

// ---------------------------------------------------------
// CICPServiceRequest::two phase construction
// ---------------------------------------------------------

CICPServiceRequest* CICPServiceRequest::NewL(TXIMPRequestId& aRequestId,
							TIMConnProviderEnums::TRequestTypes aType)
	{
	CICPServiceRequest* self = CICPServiceRequest::NewLC(aRequestId, aType);
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------
// CICPServiceRequest::two phase construction
// ---------------------------------------------------------
CICPServiceRequest* CICPServiceRequest::NewLC(TXIMPRequestId& aRequestId,
							TIMConnProviderEnums::TRequestTypes aType)
	{
	CICPServiceRequest* self = new (ELeave) CICPServiceRequest(aRequestId, aType);
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;	
	}


// ---------------------------------------------------------
// CICPServiceRequest::~CICPServiceRequest
// 
// ---------------------------------------------------------
CICPServiceRequest::~CICPServiceRequest()
	{
	 
	}

// ---------------------------------------------------------
// CICPServiceRequest::ConstructL
// 
// ---------------------------------------------------------
void CICPServiceRequest::ConstructL()
	{
	
	}

// ---------------------------------------------------------
// CICPServiceRequest::CICPServiceRequest
// 
// ---------------------------------------------------------
CICPServiceRequest::CICPServiceRequest(TXIMPRequestId& aRequestId,
								TIMConnProviderEnums::TRequestTypes aType):
iRequestId(aRequestId),
iReqType(aType)
	{
	
	}

// ---------------------------------------------------------
// CICPServiceRequest::StartWait
// 
// ---------------------------------------------------------	
void CICPServiceRequest::StartWait()
	{
	if( ! iWait.IsStarted() )
		{
		// codescanner warning can be ignored, we are not starting an active object but 
		// CActiveSchedulerWait.
		iWait.Start();
		}
		
	}

// ---------------------------------------------------------
// CICPServiceRequest::StopWait
// 
// ---------------------------------------------------------	
void CICPServiceRequest::StopWait()
	{
	 if(iWait.IsStarted() )	
		 {
		 iWait.AsyncStop();	
		 }
	}

// ---------------------------------------------------------
// CICPServiceRequest::GetRequestId
// 
// ---------------------------------------------------------	
const TXIMPRequestId& CICPServiceRequest::GetRequestId() const
	{
	return iRequestId ;
	}

// ---------------------------------------------------------
// CICPServiceRequest::IsWaitStarted
// 
// ---------------------------------------------------------		
TBool CICPServiceRequest::IsWaitStarted()
	{
	 return iWait.IsStarted() ;	
	}


// ---------------------------------------------------------
// CICPServiceRequest::SetRequestType
// 
// ---------------------------------------------------------
void CICPServiceRequest::SetRequestType(TIMConnProviderEnums::TRequestTypes aType) 
	{
	iReqType = aType ;
	}


// ---------------------------------------------------------
// CICPServiceRequest::RequestType
// 
// ---------------------------------------------------------
TIMConnProviderEnums::TRequestTypes CICPServiceRequest::RequestType() const
	{
	return iReqType ;
	}
// end of file 
