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
* Description:  vimpstdebugassert.h
*
*/


#ifndef __VIMPSTDEBUGASSERT_H__
#define __VIMPSTDEBUGASSERT_H__

#include "vimpstdebugmacros.h"

#ifdef __WVUI_TEST__
    // for test code, define empty assert (otherwise causes decisions)
    struct TCHATEmptyDebug { };
    
    #define CHAT_EMPTY( s ) TCHATEmptyDebug()
        
    #define __CHAT_ASSERT_DEBUG( c ) CHAT_EMPTY( s )
    
#else
    // debug assert that panics with filename and line number if c is not true
    #define __CHAT_ASSERT_DEBUG( c ) \
            __ASSERT_DEBUG( c, User::Panic( TPtrC((const TText *)__WFILE__), __LINE__ ) );
            
#endif

#endif  // __VIMPSTIMDEBUGASSERT_H__

//  End of File
