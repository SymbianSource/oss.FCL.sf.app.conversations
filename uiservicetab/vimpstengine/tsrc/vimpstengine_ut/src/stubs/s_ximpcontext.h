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
* Description: s_ximpcontext.h
*
*/
#ifndef MXIMPFWPRESENCECONTEXTSTUB_H
#define MXIMPFWPRESENCECONTEXTSTUB_H

#include <ximpcontext.h>

class MyMXIMPObjectFactoryStub;

class MXIMPContextStub : public CBase, public MXIMPContext
    {
public:
	MXIMPContextStub();
	~MXIMPContextStub();
    void RegisterObserverL(
            MXIMPContextObserver& aObserver,
            const TArray<TInt32>* aEventFilter = NULL );

    void UnregisterObserver(
            MXIMPContextObserver& aObserver );

     TXIMPRequestId BindToL(
                TUid aProtocolImpUid,
                const TDesC16& aServiceAddress,
                const TDesC16& aUsername,
                const TDesC16& aPassword,
                TInt32 aIapId );
     TXIMPRequestId BindToL(
                TUid aProtocolImpUid,
                const TDesC16& aServiceAddress,
                const TDesC16& aUsername,
                const TDesC16& aPassword,
                const TDesC16& aClientId,
                TInt32 aIapId );
     TXIMPRequestId BindToL( TUid aProtocolImpUid,
                            TInt aSettingsId );

     TXIMPRequestId BindToL(
                 TUid aProtocolImpUid,
                 const TDesC16& aUsername,
                 const TDesC16& aPassword,
                 TInt aSettingsId );
     
    TXIMPRequestId UnbindL();
    MXIMPFeatureInfo* GetContextFeaturesLC() const;
    MXIMPObjectFactory& ObjectFactory() const;

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    
    private:
    MyMXIMPObjectFactoryStub* iObjFactory;
    
    };


#endif // MXIMPFWPRESENCECONTEXT_H



