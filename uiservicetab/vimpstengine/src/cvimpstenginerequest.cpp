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
* Description: REquest for VIMPSTEngine
*
*/

// INCLUDE FILES
#include "cvimpstenginerequest.h"
#include "vimpstdebugtrace.h"

#include <e32base.h>

// ---------------------------------------------------------
// CVIMPSTEngineRequest::NewL
// ---------------------------------------------------------

CVIMPSTEngineRequest* CVIMPSTEngineRequest::NewL(TXIMPRequestId& aRequestId,
        TXimpOperation aType)
	{
	CVIMPSTEngineRequest* self = CVIMPSTEngineRequest::NewLC(aRequestId,aType);
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequest::NewLC
// ---------------------------------------------------------
CVIMPSTEngineRequest* CVIMPSTEngineRequest::NewLC(TXIMPRequestId& aRequestId,
        TXimpOperation aType )
	{
	CVIMPSTEngineRequest* self = new (ELeave) CVIMPSTEngineRequest(aRequestId,aType);
	CleanupStack::PushL( self );
	return self;	
	}


// ---------------------------------------------------------
// CVIMPSTEngineRequest::~CVIMPSTEngineRequest
// 
// ---------------------------------------------------------
CVIMPSTEngineRequest::~CVIMPSTEngineRequest()
	{
	if(iWait.IsStarted() ) 
        {
        iWait.AsyncStop();  
        }
	 
	}



// ---------------------------------------------------------
// CVIMPSTEngineRequest::CVIMPSTEngineRequest
// 
// ---------------------------------------------------------
CVIMPSTEngineRequest::CVIMPSTEngineRequest(TXIMPRequestId& aRequestId,
        TXimpOperation aType)
:iRequestId(aRequestId),
iReqType(aType)
	{
	
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequest::StartWait
// 
// ---------------------------------------------------------	
void CVIMPSTEngineRequest::StartWait()
	{
	if( ! iWait.IsStarted() )
		{
		// codescanner warning can be ignored, we are not starting an active object but 
		// CActiveSchedulerWait.
		iWait.Start();
		}
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequest::StopWait
// 
// ---------------------------------------------------------	
void CVIMPSTEngineRequest::StopWait()
	{
	if(iWait.IsStarted() )	
		{
		iWait.AsyncStop();	
		}
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequest::GetRequestId
// 
// ---------------------------------------------------------	
const TXIMPRequestId& CVIMPSTEngineRequest::GetRequestId() const
	{
	return iRequestId ;
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequest::IsWaitStarted
// 
// ---------------------------------------------------------		
TBool CVIMPSTEngineRequest::IsWaitStarted()
	{
	return iWait.IsStarted() ;	
	}
// ---------------------------------------------------------
// CVIMPSTEngineRequest::SetRequestType
// 
// ---------------------------------------------------------
void CVIMPSTEngineRequest::SetRequestType(TXimpOperation aType) 
    {
    iReqType = aType ;
    }


// ---------------------------------------------------------
// CVIMPSTEngineRequest::RequestType
// 
// ---------------------------------------------------------
TXimpOperation CVIMPSTEngineRequest::RequestType() const
    {
    return iReqType ;
    }

// end of file 
