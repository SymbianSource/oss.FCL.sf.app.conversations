/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles the use of Precense Framework
 *
*/


// INCLUDE FILES
#include "s_CVIMPSTEngineSessionCntxtObserver.h"
//Presence Observer
//#include "mvimpstenginepresencesubserviceeventobserver.h"

// CONTANTS


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      CVIMPSTEnginePresenceSubService_Stub()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineSessionCntxtObserver_Stub* CVIMPSTEngineSessionCntxtObserver_Stub::NewL(TUint32 aServiceId) 
        {
    CVIMPSTEngineSessionCntxtObserver_Stub* self = NewLC(aServiceId);
    CleanupStack::Pop(self);
    return self;
        }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::ConstructL()
// ---------------------------------------------------------------------------
//
 CVIMPSTEngineSessionCntxtObserver_Stub* CVIMPSTEngineSessionCntxtObserver_Stub::NewLC(TUint32 aServiceId)
    {
   CVIMPSTEngineSessionCntxtObserver_Stub* self =
    new (ELeave) CVIMPSTEngineSessionCntxtObserver_Stub( );
    CleanupStack::PushL(self);
   // self->ConstructL();
    return self;

    }
    
  CVIMPSTEngineSessionCntxtObserver_Stub::~CVIMPSTEngineSessionCntxtObserver_Stub()
    {
   

    }  

// End of file
