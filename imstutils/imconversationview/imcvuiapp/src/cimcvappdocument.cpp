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
* Description:  application document
*
*/


#include "cimcvappdocument.h"
#include "cimcvappui.h"
#include "mimcvappui.h"
// Standard Symbian OS construction sequence
CIMCVAppDocument* CIMCVAppDocument::NewL(CEikApplication& aApp)
    {
    CIMCVAppDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------
// CIMCVAppDocument::NewLC()
// two phase construction
// ---------------------------------------------------------
//
CIMCVAppDocument* CIMCVAppDocument::NewLC(CEikApplication& aApp)
    {
    CIMCVAppDocument* self = new (ELeave) CIMCVAppDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// ---------------------------------------------------------
// CIMCVAppDocument::ConstructL()
// ---------------------------------------------------------
//
void CIMCVAppDocument::ConstructL()
    {
	// no implementation required
    }    
// ---------------------------------------------------------
// CIMCVAppDocument::CIMCVAppDocument()
// ---------------------------------------------------------
//
CIMCVAppDocument::CIMCVAppDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
	// no implementation required
    }
// ---------------------------------------------------------
// CIMCVAppDocument::~CIMCVAppDocument()
// ---------------------------------------------------------
//
CIMCVAppDocument::~CIMCVAppDocument()
    {
	// no implementation required
    }
// ---------------------------------------------------------
// CIMCVAppDocument::CreateAppUiL()
// ---------------------------------------------------------
//
CEikAppUi* CIMCVAppDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CIMCVAppUi;
    return appUi;
    }

// ----------------------------------------------------
// CIMCVAppDocument::UpdateTaskNameL()
// Makes Startup-application hidden in menu shell and fastswap window
// ----------------------------------------------------
void CIMCVAppDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
    {
    CEikDocument::UpdateTaskNameL( aWgName );
    aWgName->SetHidden( ETrue );
    }
    
// end of file
