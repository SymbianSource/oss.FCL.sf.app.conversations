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
* Description:  Common definitions for vimpstsettings store creation.
*
*/


#ifndef __VIMPSTSETTINGSSTOREDEFINITIONS_H
#define __VIMPSTSETTINGSSTOREDEFINITIONS_H

// INCLUDES
#include <e32base.h>


// Key-Value pairs
enum TKeyValuePairs
	{
	EKeyValuePairBase = 256
	};
	
// Masks for bitwise operations
//
//Mask for finding Key-value pairs
//bits: 111111 1111111111 1111111100000000
const TUint32 KSAPPairsMask = 0xFFFFFF00;

//Mask for checking SAP existence
const TUint32 KSAPExistsMask = 0xFFFFFFFF;

//Mask for finding SAP Uids
//bits: 1111111111111111 0000000000000000
const TUint32 KSAPUidMask = 0xFFFF0000;

// Offsets for bitwise operations
const TUint32 KServiceIDOffset = 16; //service id offset

// field separator for flat key-valuepair structure
_LIT( KKeyValuePairFieldSeparator, "||" );

// Maximum length of TInt textual representation
const TInt KTIntMaxLengthTextual = 10;

#endif	// __VIMPSTSETTINGSSTOREDEFINITIONS_H

