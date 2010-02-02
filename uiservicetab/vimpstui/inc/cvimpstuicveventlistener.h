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
* Description:  Defines the corresponding TAknsItemIds for bitmaps
*  Description : Handles statuspane, context pane, navi pane
*
*/

#ifndef CVIMPSTUICVEVENTLISTENER_H_
#define CVIMPSTUICVEVENTLISTENER_H_

#include <e32base.h>
#include <e32property.h>

// FORWARD DECLARATION
class CVIMPSTUiTabbedView;

//CLASS DECLARATION
/**
 *  Conversation view event listner.
 *
 *  @lib vimpstui.dll
 *  @since 5.0
 */
class CVIMPSTUICvEventListener : public CActive
    {
public :
        // Constructor
        /**
         * Creates a new CVIMPSTUICvEventListener.
         * @param aTabbedView, reference to tabbedview
         * @return New instance of this class         
         */
        static CVIMPSTUICvEventListener* NewL(CVIMPSTUiTabbedView& aTabbedView );
        
          /**
           * Standard C++ destructor.
           */
         ~CVIMPSTUICvEventListener();
         
         /**
          * start listening
         */
         void StartListening();
         
private : // From CActive

        /**
         *  RunL
         */
        void RunL();  
        
        /**
         *  DoCancel
        */
        void DoCancel();

        
private: // Implementation
       /**
        * Standard C++ constructor
        * @param aTabbedView, reference to tabbedview
        */
        CVIMPSTUICvEventListener( CVIMPSTUiTabbedView& aTabbedView);

       /**
        * Performs the 2nd phase of construction.
        */
       void ConstructL();
       
private :
    
    // owns , property to use
    RProperty iProperty;
    // not owned,  reference to tabbedview
    CVIMPSTUiTabbedView& iTabbedView;
       
    };

#endif /*CVIMPSTUICVEVENTLISTENER_H_*/
