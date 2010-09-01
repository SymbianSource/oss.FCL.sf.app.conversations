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
* Description:  All CSP errors, symbolically
 *
*/


#ifndef _VIMPSTALLERRORS_H
#define _VIMPSTALLERRORS_H


#define KPREQUESTERRSUCCESSFUL 200 // successfull

/**
 *  A Enum
 * TEngineErrors list of engine errors
 */ 
enum TEngineErrors
    {
    EVIMPSTWrongUserIdPassword               = 409,	/* wrong userid password error*/	
    EVIMPSTUnknownUserId               = 531, /* unknown user id */
    
    // 6xx - SESSION
    EVIMPSTSessionExpired              = 600, /* session expired*/
    EVIMPSTForcedLogout                = 601, /* Forced Logout */
    EVIMPSTNotLoggedIn                 = 604, /* not logged in */
    EVIMPSCancelAccesspoint            = 605 , /*cancel accespoint*/

    // 7xx - PRESENCE AND CONTACT LIST
    EVIMPSTInvalidPresenceAttribute    = 750, /* invalid presence attribute */
    EVIMPSTInvalidPresenceValue        = 751, /* Invalid presence value */

       // 9xx - GENERAL ERRORS
    EVIMPSTMultipleErrors              = 900, /* Multiple errors */
    EVIMPSTNotSupported /* feature not supported*/
    };

#endif // _IMPSCSPALLERRORS_H
