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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#ifndef S_MCCACONNECTION_H_
#define S_MCCACONNECTION_H_

#include <mccaconnection.h>

class MCCAParameter;

/** @file mccaconnection.h
 *
 *  MCCAObserver is used to get notifications from CCA application.
 *
 *  @lib ccaclient
 *  @since S60 v5.0
 */

class MCCAConnectionStub:public MCCAConnection
    {

public:
    static MCCAConnection* MCCAConnectionStub::NewL();
    /**
     * Destroy this entity
     *
     * @since S60 5.0
     */
     void Close() ;

    /**
     * DEPRECATED - USE LaunchAppL() instead!
     * Ownership of MCCAParameter is NOT transferred.
     */
     void LaunchApplicationL( MCCAParameter& aParameter ) ;

    /**
     * Launches CCA application with the given parameter.
     * See above for example instructions.
     *
     * Ownership of MCCAParameter is transferred.
     *
     * Leaves KErrArgument if there is no contact data in the
     *        given parameter.
     * Leaves KErrAlreadyExists if there is already CCApplication
     * running simultaneously.
     *
     * @see MCCAParameter::SetContactDataL
     * @since S60 5.0
     * @param aParameter to launch application
     * @param aObserver to get notifications (see MCCAObserver)
     */
     void LaunchAppL(
        MCCAParameter& aParameter,
        MCCAObserver* aObserver = NULL ) ;
    };





#endif /* S_MCCACONNECTION_H_ */
