/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for identity object.
*
*/

#ifndef MyMXIMPIdentityStub_H
#define MyMXIMPIdentityStub_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include <ximpidentity.h>

/**
 * Interface for identity object.
 *
 * Identity objects are used to identify
 * presentities, presentity groups and
 * presentity group members, XIMP framework
 * client applications etc. entities in
 * XIMP Framework.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MyMXIMPIdentityStub : public CBase, public MXIMPIdentity
    {

public:
	
	MyMXIMPIdentityStub();
	
    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    ~MyMXIMPIdentityStub() ;


public:


    /**
     * Returns identity string.
     *
     * @return The identity string.
     */
    const TDesC16& Identity() const ;



    /**
     * Sets identity.
     *
     * @param [in] aIdentity
     *        Identity string.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    void SetIdentityL(
                const TDesC16& aIdentity ) ;

    
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
private:
	HBufC*  iIdentity;
	
};


#endif // MyMXIMPIdentityStub_H
