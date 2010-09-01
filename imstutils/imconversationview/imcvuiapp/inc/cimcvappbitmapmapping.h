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
* Description:  Defines the corresponding TAknsItemIds for bitmaps
*  Description : Handles statuspane, context pane, navi pane
*
*/



#ifndef CIMCVAPPBITMAPMAPPING_H
#define CIMCVAPPBITMAPMAPPING_H


// INCLUDES
#include <avkon.mbg>
#include <AknsConstants.h>
#include <uniutils.mbg>

#include <conversations.mbg>
#include <imcvuiapp.mbg>


// STRUCTURES
struct TCABitmapMapping
    {
    TInt iBitmapId;
    // must be pointer since msvc wont compile this if it's reference
    const TAknsItemID* iItemId;
    TInt iColorGroupId;
    };


// CONSTANTS

static const TCABitmapMapping KCABitmaps[] =
    {
    // the color groups are from Avkon Skin LAF
    

    { EMbmImcvuiappQgn_prop_im_smileys_happy, 	     &KAknsIIDQgnPropImSmileysHappy,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_sad, 	     &KAknsIIDQgnPropImSmileysSad,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_laugh,	     &KAknsIIDQgnPropImSmileysLaugh,  0 },
    { EMbmImcvuiappQgn_prop_im_smileys_wink, 	     &KAknsIIDQgnPropImSmileysWink, 0 },
    { EMbmImcvuiappQgn_prop_im_smileys_kiss,  	     &KAknsIIDQgnPropImSmileysKiss      ,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_angry,  	     &KAknsIIDQgnPropImSmileysAngry,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_bored,   	 &KAknsIIDQgnPropImSmileysBored,    0 },
    { EMbmImcvuiappQgn_prop_im_smileys_crying,  	 &KAknsIIDQgnPropImSmileysCrying,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_glasses,  	 &KAknsIIDQgnPropImSmileysGlasses,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_indif,    	 &KAknsIIDQgnPropImSmileysIndif,     0 },
    { EMbmImcvuiappQgn_prop_im_smileys_robot,  	     &KAknsIIDQgnPropImSmileysRobot,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_shock,        &KAknsIIDQgnPropImSmileysShock,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_skeptical,    &KAknsIIDQgnPropImSmileysSkeptical,      0 },
    { EMbmImcvuiappQgn_prop_im_smileys_sleepy, 		 &KAknsIIDQgnPropImSmileysSleepy,  0 },
    { EMbmImcvuiappQgn_prop_im_smileys_sunglasses,   &KAknsIIDQgnPropImSmileysSunglasses,    0 },
    { EMbmImcvuiappQgn_prop_im_smileys_surprise,     &KAknsIIDQgnPropImSmileysSurprise   ,       0 },
    { EMbmImcvuiappQgn_prop_im_smileys_tired,  		 &KAknsIIDQgnPropImSmileysTired,   0 },
    { EMbmImcvuiappQgn_prop_im_smileys_veryhappy,    &KAknsIIDQgnPropImSmileysVeryhappy,     0 },
    { EMbmImcvuiappQgn_prop_im_smileys_verysad,		 &KAknsIIDQgnPropImSmileysVerysad, 0 },
    { EMbmImcvuiappQgn_prop_im_smileys_wickedsmile,       &KAknsIIDQgnPropImSmileysWickedsmile,        0 },
    { EMbmImcvuiappQgn_prop_im_from,               &KAknsIIDQgnPropImFrom,           EAknsCIQsnIconColorsCG13 },
    { EMbmImcvuiappQgn_prop_im_tome,                    &KAknsIIDQgnPropImTome,           EAknsCIQsnIconColorsCG13 },
    // other than Chat icons
    { EMbmAvkonQgn_prop_empty,                                  &KAknsIIDQgnPropEmpty, 0 },

    // end of list
    { KErrNotFound, &KAknsIIDNone, KErrNotFound }
    };

#endif      // CIMCVAPPBITMAPMAPPING_H

// End of File
