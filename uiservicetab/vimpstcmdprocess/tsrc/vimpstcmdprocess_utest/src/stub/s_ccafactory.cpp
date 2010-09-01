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
#include <ccafactory.h>
#include "s_mccaparameter.h"

class MCCAConnection;
class MCCAParameter;

    /**
     * Create new CCA client parameter entity.
     * Ownership is transferred.
     * @since S60 5.0
     * @return MCCAParameter, ownership is transferred.
     */
    EXPORT_C  MCCAParameter* TCCAFactory::NewParameterL( )
    {
    MCCAParameter* parameter = MCCAParameterstub::NewL();
    return parameter;
    }

    /**
     * Create new CCA client connection entity.
     * Ownership is transferred.
     * @since S60 5.0
     * @return MCCAConnection, ownership is transferred.
     */
    EXPORT_C  MCCAConnection* TCCAFactory::NewConnectionL( )
    {
    }
// End of File
    
