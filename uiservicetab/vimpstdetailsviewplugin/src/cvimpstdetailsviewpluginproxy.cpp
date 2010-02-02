/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View plugin base class of ECE's CCA-application
 *
*/


#include <e32std.h>
#include <implementationproxy.h>

#include "cvimpstdetailsviewpluginfactory.h"
#include "vimpstdetailsviewpluginuids.hrh"

const TImplementationProxy ImplementationTable[] =
{
    IMPLEMENTATION_PROXY_ENTRY(KVIMPSTDetailsViewPluginImplmentationUid,
            CVIMPSTDetailsViewPluginFactory::NewL)
};

// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
{
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
}

// End of File
