/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  oss protocol plugin connection manager interface
*
*/

#ifndef __MOSSCONNNECTIONMANAGERCONTROL_H__
#define __MOSSCONNNECTIONMANAGERCONTROL_H__


// FORWARD DECLARATIONS

class MXIMPProtocolConnectionHost;

/**
 * MTelepathyPrtPluginConnectionManager
 *
 * Telepathy Plugin Connection Manager's Interface
 *
 */
class MOSSProtocolConnectionManager
    {

public:

    
    /**
     * @since S60 ?S60_version
     * access to protocol connection host
     * @return protocol connection host
     */
    virtual MXIMPProtocolConnectionHost& HandleToHost() = 0;
        
protected:

    /**
     * Protected destructor.
     * Instancies can't be destroyed via this interface.
     */
    virtual ~MOSSProtocolConnectionManager() {}

    };

#endif // __MOSSCONNNECTIONMANAGERCONTROL_H__


