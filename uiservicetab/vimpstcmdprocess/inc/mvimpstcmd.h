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
* Description:  IM command object interface
*
*/


#ifndef _MVIMPSTCMD_H_
#define _MVIMPSTCMD_H_

// INCLUDES
#include <e32std.h>
#include "vimpstcmd.hrh"

// FORWARD DECLARATIONS
class MVIMPSTCmdObserver;


// CLASS DECLARATION

/**
 * IM command object interface. 
 *
 * Every Instant Messaging command must implement this interface.
 * Application command handler can then create these command objects
 * and execute them. Commands can report about their execution via
 * MImCommandObserver interface. 
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class MVIMPSTCmd
    {
    public: // Interface

        /**
         * Destructor.
         */
        virtual ~MVIMPSTCmd()
                {}

        /**
         * Executes the command and destroys this command object when done.
         * The command is also destroyed if this function leaves.
         */
        virtual void ExecuteLD() = 0;
        
        /**
         * Adds a command observer for this command.
         * @param aObserver     Command observer to add.
         */
        virtual void AddObserver(
                MVIMPSTCmdObserver& aObserver ) = 0;
        
		
		/**
         * Returns the command id.
         * @return TImCommandId     Command ID.
         */                
		virtual TInt CommandId() const = 0 ;		
		
		
		/**
         * Returns the result data/error code.
         * The return value will have a valid data, only if this function
         * is called in CommandFinished() observer notification[MImCommandObserver]
         * @return TAny     pointer to the result data. 
         *     Ownership is not transferred to the caller
         */                
		virtual TInt Result() const = 0 ;             

    };

#endif // _MVIMPSTCMd_H_

// End of File
