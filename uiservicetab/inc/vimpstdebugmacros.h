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
* Description:   debug macro for uiservicetab
*
*/


#ifndef __VIMPSTIMDEBUGMACROS_H__
#define __VIMPSTIMDEBUGMACROS_H__

// widen macros for unicode text
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#endif // __VIMPSTIMDEBUGMACROS_H__