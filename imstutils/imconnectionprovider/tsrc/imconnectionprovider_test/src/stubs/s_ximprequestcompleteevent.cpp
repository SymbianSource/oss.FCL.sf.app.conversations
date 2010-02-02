/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_ximprequestcompleteevent.cpp
*
*/

#include "s_ximprequestcompleteevent.h"

TInt32 gMXIMPRequestCompleteEventStubInterfaceId = 0;
TXIMPRequestId gMXIMPRequestCompleteEventStubRequestId;
TBool gRequestCompleteEventResultCodeKerrNotSupported = 0;
MyXIMPStatusStub gXIMPStatus;

const TXIMPRequestId& MXIMPRequestCompleteEventStub::RequestId() const
    {
    return gMXIMPRequestCompleteEventStubRequestId;
    }

const MXIMPStatus& MXIMPRequestCompleteEventStub::CompletionResult() const
    {    
    if (gRequestCompleteEventResultCodeKerrNotSupported)
	    {
	    gXIMPStatus.SetResultCode(KErrNotSupported);
	    return gXIMPStatus;	
	    }    
    
    gXIMPStatus.SetResultCode(KErrNone);
    
    return gXIMPStatus;
    }

const MXIMPBase* MXIMPRequestCompleteEventStub::LookupCompletionDataByType(
            TInt32 aInterfaceId ) const
    {
    MXIMPBase* base = NULL;
    return base;
    }

const TAny* MXIMPRequestCompleteEventStub::GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps ) const
    {
    return this;
    }

TAny* MXIMPRequestCompleteEventStub::GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps )
    {
    return NULL;
    }

TInt32 MXIMPRequestCompleteEventStub::GetInterfaceId() const
    {
    return gMXIMPRequestCompleteEventStubInterfaceId;
    }
