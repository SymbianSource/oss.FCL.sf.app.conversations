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
* Description:  create document class
*
*/


#include "cimcvappapplication.h"
#include "cimcvappdocument.h"
#include "imcvuiliterals.h"
// ---------------------------------------------------------
// CIMCVAppApplication::CreateDocumentL()
// Creates CIMCVAppDocument object
// ---------------------------------------------------------
//
CApaDocument* CIMCVAppApplication::CreateDocumentL()
    {  
    // Create an conversationview document, and return a pointer to it
    CApaDocument* document = CIMCVAppDocument::NewL(*this);
    return document;
    }
// ---------------------------------------------------------
// CIMCVAppApplication::AppDllUid()
// ---------------------------------------------------------
//
TUid CIMCVAppApplication::AppDllUid() const
    {
    // Return the UID for the conversationview application
    return KUidconversationviewApp;
    }

