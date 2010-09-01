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

#ifndef MyMXIMPObjectFactoryStub_H
#define MyMXIMPObjectFactoryStub_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>
#include <ximpobjectfactory.h>


class MXIMPStatus;
class MXIMPIdentity;
class MXIMPObjectCollection;
class MXIMPRestrictedObjectCollection;
class MXIMPDataSubscriptionState;


/**
 * Interface for XIMP Framework object factory.
 *
 * XIMP object factory is used to instantiate
 * data objects declared in Data Model API.
 *
 * MXimpFwObjectFactory interface can be obtained from
 * MXIMPContext or MXIMPProtocolConnectionHost
 * interfaces.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MyMXIMPObjectFactoryStub : public CBase, public MXIMPObjectFactory
    {
public:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    ~MyMXIMPObjectFactoryStub() {};
    
    MyMXIMPObjectFactoryStub() 
    {
    	
    };


    /**
     * Instantiates new status object.
     *
     * @return New status object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    MXIMPStatus* NewStatusLC() ;


    /**
     * Instantiates new identity object.
     *
     * @return New identity object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    MXIMPIdentity* NewIdentityLC() ;


    /**
     * Instantiates new restricted object collection object.
     *
     * @return New restricted object collection instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    MXIMPRestrictedObjectCollection* NewRestrictedObjectCollectionLC() ;


    /**
     * Instantiates new object collection object.
     *
     * @return New object collection instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    MXIMPObjectCollection* NewObjectCollectionLC() ;


    /**
     * Instantiates new data subscription state object.
     *
     * @return New data subscription state instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    MXIMPDataSubscriptionState* NewDataSubscriptionStateLC() ;
    
    const TAny* GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps ) const;
                    

	TAny* GetInterface(
	                    TInt32 aInterfaceId,
	                    TIfGetOps aOps );

	TInt32 GetInterfaceId() const;



    };

#endif // MyMXIMPObjectFactoryStub_H



