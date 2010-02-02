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
* Description:  Interface for XIMP Framework object factory.
*
*/

#include "s_ximpobjectfactory.h"
#include "s_ximpidentity.h"

TBool gCreateXIMPIdentity = EFalse;
/**
 * Instantiates new status object.
 *
 * @return New status object instance.
 *         Object ownership is returned to caller.
 *
 * @leave KErrNoMemory if failed to allocate memory.
 */
MXIMPStatus* MyMXIMPObjectFactoryStub::NewStatusLC()
    {
    return NULL;
    }


/**
 * Instantiates new identity object.
 *
 * @return New identity object instance.
 *         Object ownership is returned to caller.
 *
 * @leave KErrNoMemory if failed to allocate memory.
 */
MXIMPIdentity* MyMXIMPObjectFactoryStub::NewIdentityLC() 
	{
	if (gCreateXIMPIdentity)
		{
		MyMXIMPIdentityStub* ret = new (ELeave) MyMXIMPIdentityStub();
		CleanupStack::PushL(ret);
		return ret;
		}

	return NULL;
	}


/**
 * Instantiates new restricted object collection object.
 *
 * @return New restricted object collection instance.
 *         Object ownership is returned to caller.
 *
 * @leave KErrNoMemory if failed to allocate memory.
 */
MXIMPRestrictedObjectCollection* MyMXIMPObjectFactoryStub::NewRestrictedObjectCollectionLC() { return NULL;}


/**
 * Instantiates new object collection object.
 *
 * @return New object collection instance.
 *         Object ownership is returned to caller.
 *
 * @leave KErrNoMemory if failed to allocate memory.
 */
MXIMPObjectCollection* MyMXIMPObjectFactoryStub::NewObjectCollectionLC() { return NULL; }


/**
 * Instantiates new data subscription state object.
 *
 * @return New data subscription state instance.
 *         Object ownership is returned to caller.
 *
 * @leave KErrNoMemory if failed to allocate memory.
 */
MXIMPDataSubscriptionState* MyMXIMPObjectFactoryStub::NewDataSubscriptionStateLC() { return NULL; }


const TAny* MyMXIMPObjectFactoryStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
                    {
                    	return NULL;
                    }

TAny* MyMXIMPObjectFactoryStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
{
	return NULL;
}

TInt32 MyMXIMPObjectFactoryStub::GetInterfaceId() const
{
	return KErrNone;
}


