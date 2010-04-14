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
* Description: Request mapper for VIMSTEngine
*
*/

// INCLUDE FILES

#include "cvimpstenginerequestmapper.h"

#include "cvimpstenginerequest.h"

#include "uiservicetabtracer.h"

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::two phase constructor
// 
// ---------------------------------------------------------	
CVIMPSTEngineRequestMapper* CVIMPSTEngineRequestMapper:: NewL()
	{
	CVIMPSTEngineRequestMapper* self = CVIMPSTEngineRequestMapper::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::NewLC
// 
// ---------------------------------------------------------	
CVIMPSTEngineRequestMapper* CVIMPSTEngineRequestMapper::NewLC()
	{
	CVIMPSTEngineRequestMapper* self = new (ELeave) CVIMPSTEngineRequestMapper();
	CleanupStack::PushL( self );
	return self;	
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::~CVIMPSTEngineRequestMapper
// 
// ---------------------------------------------------------	
CVIMPSTEngineRequestMapper::~CVIMPSTEngineRequestMapper()
	{
	iRequestArray.ResetAndDestroy();	
	}


// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::CVIMPSTEngineRequestMapper
// 
// ---------------------------------------------------------	
CVIMPSTEngineRequestMapper::CVIMPSTEngineRequestMapper()
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::CreateRequestL
// 
// ---------------------------------------------------------	
CVIMPSTEngineRequest* CVIMPSTEngineRequestMapper::CreateRequestL
											(TXIMPRequestId& aRequestId, 
											TBool aIsWait, TXimpOperation aType)
	{
	TRACER_AUTO;
	CVIMPSTEngineRequest* req = CVIMPSTEngineRequest::NewL(aRequestId,aType );
	CleanupStack::PushL(req);	
	TInt status( iRequestArray.Append( req ) );
	User::LeaveIfError( status );
		
	if(aIsWait)
		{
		req->StartWait();	
		}


	CleanupStack::Pop(req);
	return req;
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::FindRequestId
// 
// ---------------------------------------------------------		
CVIMPSTEngineRequest* CVIMPSTEngineRequestMapper::FindRequestId(const TXIMPRequestId& aRequestId )
	{
	TRACER_AUTO;
	TInt count = iRequestArray.Count() ;
	TXIMPRequestId reqId ;
	CVIMPSTEngineRequest *req = NULL;
	for (TInt i=0; i < count; i++)
		{
		req = iRequestArray[i] ;
		
		reqId = req->GetRequestId() ;
		
		if( aRequestId == reqId )			
			{
			break ;
			}
		}
	return req ;
	}

// ---------------------------------------------------------
// CVIMPSTEngineRequestMapper::RemoveRequestId
// 
// ---------------------------------------------------------		
void CVIMPSTEngineRequestMapper::RemoveRequestId(const TXIMPRequestId& aRequestId )
    {
	TRACER_AUTO;
    TInt count = iRequestArray.Count() ;
    for (TInt i=0; i < count; i++)
        {
        if( iRequestArray[i]->GetRequestId() == aRequestId)         
            {
            delete iRequestArray[i];
            iRequestArray[i] = NULL;
            iRequestArray.Remove( i );
            iRequestArray.Compress();
            break ;
            }
        }
    }

	
// end of file 
