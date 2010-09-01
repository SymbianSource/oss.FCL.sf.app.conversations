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
* Description: s_ximpclient.cpp
*
*/
#include "s_ximpclient.h"
#include "s_ximpcontext.h"

void MXIMPClientStub::GetProtocolsL(
                RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& /*aProtocols*/ )
    {
    }

MXIMPFeatureInfo* MXIMPClientStub::GetFrameworkFeaturesLC()
    {
    return NULL;
    }

MXIMPContext* MXIMPClientStub::NewPresenceContextLC()
    {
    MXIMPContext* context = new ( ELeave ) MXIMPContextStub;
    CleanupStack::PushL( context );
    return context;
    }

const TAny* MXIMPClientStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MXIMPClientStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MXIMPClientStub::GetInterfaceId() const
    {
    return 0;
    }


MXIMPClient* MXIMPClientStub::NewClientL()
	{
	return new (ELeave) MXIMPClientStub();	
	}


    
MXIMPClientStub::~MXIMPClientStub()
	{
		
	}


    
    

    
