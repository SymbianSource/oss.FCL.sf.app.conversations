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
* Description:  Tabbed view declaration
*
*/

#ifndef CVIMPSTUISTATUSPANEHANDLER_H
#define CVIMPSTUISTATUSPANEHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <aknview.h>
#include <akncontext.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <akntitle.h>
#include <AknIconUtils.h>
#include <avkon.hrh>
#include <eikenv.h>
#include <eikappui.h>
#include <fbs.h>
#include <eikspane.h>
// CLASS DECLARATION

/**
 *  CCVIMPSTUiStatusPaneHandler
 * 
 */
class CCVIMPSTUiStatusPaneHandler : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CCVIMPSTUiStatusPaneHandler();

    /**
     * Two-phased constructor.
     */
    static CCVIMPSTUiStatusPaneHandler* NewL();

    /**
     * Two-phased constructor.
     */
    static CCVIMPSTUiStatusPaneHandler* NewLC();
    
    /**
     * Function to set picture on the status pane   
     */
    void SetPictureL( CFbsBitmap *aBitmap, CFbsBitmap *aMask, TBool aNaviPush = EFalse);
    
    /**
     * Function to set text on the status pane
     */
    void SetTextL( const TDesC& aText );
    
    /**
     * Function to clear the icon on the Status Pane.
     */
    void ClearPaneL();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CCVIMPSTUiStatusPaneHandler();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();



private:
    
    /**
     * not owned. Handle to the environment
     */
    CEikonEnv* iEnv;

    /**
     * not owned, Handle to the environment
     */
    CEikStatusPane* iStatusPane;
    
    /**
     * not owned Handle to the environment
     */
    CAknTitlePane* iTitlePane;

    /**
     * not owned. Handle to the environment
     */
    CAknContextPane* iContextPane;

    /**
     * not owned. Handle to the environment
     */
    CAknNavigationControlContainer* iNaviPane;

    /**
     * Decorator pointer to decorate the navi pane.
     */
    CAknNavigationDecorator* iNaviDeco;
    
    /** 
     *  Variable to know if the iNaviDeco is pushed on the navi pane.
     */
    TBool iNaviPush;
    };
#endif // CVIMPSTUISTATUSPANEHANDLER_H

//END OF FILE.

