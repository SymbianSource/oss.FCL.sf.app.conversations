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
* Description:  Contact list container implementation
 *
*/


#ifndef TVIMPSTSTORAGEPANICS_H
#define TVIMPSTSTORAGEPANICS_H

// CONSTANTS
_LIT( KPanicCategory, "vimpststorage" );
_LIT( KViewUnInitialised, "vimpststorage - View Uninitialised" );


#define 	KLIBNOTINITIALIZED		KErrBadLibraryEntryPoint
#define     KVIEWNOTREADY			KErrNotReady

// DATA TYPES
enum TVIMPSTStoragePanics
    {
    ENoSuitableIdentification,  // for some reason contact identification is NULL
    EObserverIsNull,            // Storage manager panics if given observer is NULL
    EStreamLengthIsZero,        // if read stream length is zero during internalizing
    ENoSuitableUserId,          // for some reason contact userID is NULL
    ESortingCorupted,			// Sorting of the contact has failed
    EContactsArrayOutOfSync     // Two arrays of the contact list are out of synchronization 
    };

// FUNCTION PROTOTYPES

// ----------------------------------------------------
// Panic
// Panic handler
// ----------------------------------------------------
//
GLDEF_C void Panic( TVIMPSTStoragePanics aPanic );

#endif // TVIMPSTSTORAGEPANICS_H
