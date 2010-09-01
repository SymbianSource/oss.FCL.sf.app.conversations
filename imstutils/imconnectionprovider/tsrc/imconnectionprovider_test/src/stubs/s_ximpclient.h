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
* Description: s_ximpclient.h
*
*/
#ifndef _MXIMPCLIENTSTUB_
#define _MXIMPCLIENTSTUB_

#include <ximpclient.h>

class MXIMPProtocolInfo;
class MXIMPFeatureInfo;
class MXIMPContext;

class MXIMPClientStub : public CBase, public MXIMPClient
    {

public:	
	/**
     * Factory method to instantiate MXIMPClient.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default presence framework isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new presence client object. Object
     *         ownership is returned to caller.
     */
    static MXIMPClient* NewClientL();


    /**
     * Public destructor.
     * Object can be deleted through this interface.
     */
    ~MXIMPClientStub();
    
    void GetProtocolsL(
                RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocols );

    MXIMPFeatureInfo* GetFrameworkFeaturesLC();

    MXIMPContext* NewPresenceContextLC();

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };

#endif // MXIMPFWCLIENT_H



