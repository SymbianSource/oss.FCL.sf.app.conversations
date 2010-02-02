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
* Description:  Publish and Subscribe factory method implementation
*
*/ 

// INCLUDE FILES
#include    "imalertnotifierfactory.h"
#include    "mimalertnotifierinterface.h"
#include    "cimalertnotifierimpl.h"


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// IMAlertNotifierFactory::CreateNotifierL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C MIMAlertNotifierInterface* IMAlertNotifierFactory::CreateNotifierL( MIMAlertNotifierKeyObserver* aObserver )
	{
	return CIMAlertNotifierImpl::NewL( aObserver );
	}
	
//  End of File

