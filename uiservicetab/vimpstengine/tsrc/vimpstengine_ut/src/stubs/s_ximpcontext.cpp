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
* Description: s_ximpcontext.cpp
*
*/
#include "s_ximpcontext.h"
#include "s_ximpobjectfactory.h"

TInt gUnBindingLeaves = KErrNone;

void MXIMPContextStub::RegisterObserverL(
        MXIMPContextObserver& /*aObserver*/,
        const TArray<TInt32>* /*aEventFilter*/ )
    {
    }

void MXIMPContextStub::UnregisterObserver(
        MXIMPContextObserver& /*aObserver*/ )
    {
    }

TXIMPRequestId MXIMPContextStub::BindToL(
            TUid /*aProtocolImpUid*/,
            const TDesC16& /*aServiceAddress*/,
            const TDesC16& /*aUsername*/,
            const TDesC16& /*aPassword*/,
            TInt32 /*aIapId*/ )
    {
    TXIMPRequestId id;
    return id;
    }

TXIMPRequestId MXIMPContextStub::BindToL(
            TUid /*aProtocolImpUid*/,
            const TDesC16& /*aServiceAddress*/,
            const TDesC16& /*aUsername*/,
            const TDesC16& /*aPassword*/,
            const TDesC16& /*aClientId*/,
            TInt32 /*aIapId*/ )
    {
    TXIMPRequestId id;
        return id;
    }

TXIMPRequestId MXIMPContextStub::BindToL(
            TUid /*aProtocolImpUid*/,
            const TDesC16& /*aUsername*/,
            const TDesC16& /*aPassword*/,
            TInt /*aSettingsId*/ )
    {
    TXIMPRequestId id;
        return id;
    }

TXIMPRequestId MXIMPContextStub::BindToL( TUid /*aProtocolImpUid*/,
                        TInt /*aSettingsId*/ )
    {
    TXIMPRequestId id;
        return id;
    }

TXIMPRequestId MXIMPContextStub::UnbindL()
    {
    User::LeaveIfError( gUnBindingLeaves );
    TXIMPRequestId id;
        return id;
    }

MXIMPFeatureInfo* MXIMPContextStub::GetContextFeaturesLC() const
    {
    return NULL;
    }

MXIMPObjectFactory& MXIMPContextStub::ObjectFactory() const
    {
    
    
    MXIMPContextStub* thisObject = const_cast <MXIMPContextStub*> (this);
    if (!thisObject->iObjFactory)
	    {
	    thisObject->iObjFactory = new (ELeave) MyMXIMPObjectFactoryStub();	
	    }
    return *iObjFactory;
    }

const TAny* MXIMPContextStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MXIMPContextStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MXIMPContextStub::GetInterfaceId() const
    {
    return 0;
    }
        
MXIMPContextStub::MXIMPContextStub() 
{
	
}

MXIMPContextStub::~MXIMPContextStub()
	{
	delete 	iObjFactory;
	}
