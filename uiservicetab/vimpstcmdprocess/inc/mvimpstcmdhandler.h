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
* Description:  IM command handler interface
*
*/


#ifndef MVIMPSTCMDHANDLER_H
#define MVIMPSTCMDHANDLER_H

// INCLUDES
#include <e32def.h>
#include "vimpstcmd.hrh"
#include "mvimpstprocessarray.h"

// FORWARD DECLARATIONS
class MVIMPSTCmd;
class MVIMPSTCmdObserver;
class MVIMPSTProcessArray;

// CLASS DECLARATION

/**
 * IM command handler interface.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class MVIMPSTCmdHandler
    {
    public: // Interface

        /**
         * Handles IM commands.
         *
         * @param aCommandId    The id of the command to handle. 
         * @param aObserver		The observer
         * @param aData         Pass the data required for the command to process      
         * @return  ETrue if the command was handled, EFalse otherwise.
         */
        virtual TBool HandleCommandL(
                const TImCommandId aCommandId, MVIMPSTCmdObserver* aObserver,
                TAny* aData ) = 0;
        /**
         * RegisterEventObserverL :register the observer from ui to get back the events
         * @param: aObserver 
         */
        virtual void RegisterEventObserverL( MVIMPSTCmdObserver* aObserver ) = 0;
        
        /**
         * UnRegisterEventObserver :Unregister the observer from ui to stop getting the events
         * @param: aObserver 
         */
        virtual void UnRegisterEventObserver( MVIMPSTCmdObserver* aObserver ) = 0;
        
        
        /**
         * GetProcessInterface :Returns the Process interface
         * @return MVIMPSTProcessArray, reference to process array.
         */
        virtual MVIMPSTProcessArray& GetProcessInterface() = 0 ;
        

    
        /**
         * destructor.
         */    
         virtual ~MVIMPSTCmdHandler() {}
    };

#endif // MVIMPSTCMDHANDLER_H

// End of File
