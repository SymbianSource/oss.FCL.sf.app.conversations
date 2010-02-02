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
* Description:  OSs adapter plugin panics
*
*/

#ifndef __OSSPROTOCOLPLUGINPANICS_H__
#define __OSSPROTOCOLPLUGINPANICS_H__

//  INCLUDES
#include    <e32std.h>


// CONSTANTS
/**
* OSS Adaptation Library panic category.
*/
_LIT( KOSSProtocolPluginPanic, "OSSProtocolPlugin" );

/**
* OSS Adaptation Library Auth interface
*/

_LIT(KOSSProtocolPresenceAuthorization, "COSSProtocolPresenceAuthorization");

/**
* OSS Adaptation Library Publishing interface
*/

_LIT(KOSSProtocolPresencePublishing, "COSSProtocolPresencePublishing");

/**
* OSS Adaptation Library watching interface.
*/

_LIT(KOSSProtocolPresenceWatching, "COSSProtocolPresenceWatching");

/**
* OSS Adaptation Library panic presentity groups interface.
*/

_LIT(KOSSProtocolPresentityGroups, "COSSProtocolPresentityGroups");

/**
* OSS Adaptation Library panic conversation interface.
*/

_LIT(KOSSProtocolConversation, "COSSProtocolImConversation");


/**
* Telepathy adapter plugin library panic reasons.
*
* Following panic reasons are used in
* OSS adapter plugin library panics.
*
* @since 3.0
*/
enum TOSSAdapterPluginPanics
    {
    ESesionConsistencyLost                      = 1,
    EOpIdConsistencyLost                        = 2,
    ENoActiveOperations                         = 3,
    EUnknownUnumeration                         = 4,
    EReferenceCountUnderflow                    = 5,
    EIlegalSingletonDeletion                    = 6,
    EAccessHandlerObserversUnregistered         = 7,
    EUnknowPureData                             = 8,
    EListenerAlreadyActive                      = 9,
    ELogInDataNotValid                          = 10

    };


#endif  //__OSSPROTOCOLPLUGINPANICS_H__

//  End of File

