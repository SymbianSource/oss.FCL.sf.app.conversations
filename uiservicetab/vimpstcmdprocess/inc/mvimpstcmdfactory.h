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
* Description:  IM Command factory abstract API.
*
*/


#ifndef MVIMPSTCMDFACTORY_H
#define MVIMPSTCMDFACTORY_H

#include "vimpstcmd.hrh"

//  FORWARD DECLARATIONS
class MVIMPSTCmd;
class MVIMPSTCmdObserver;
class MVIMPSTProcessArray;

//  CLASS DECLARATION 

/**
 * Instant Messaging command factory API.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class MVIMPSTCmdFactory
    {
    public:  // Interface
	    /**
		 * Virtual destructor.
		 */
		virtual ~MVIMPSTCmdFactory() { }

		/**
         * Creates a command given command ID.
         *
         * @param aCommandId ID of the command that the command object is created for.
         * @param aData         Pass the data required for the command to process      
         * @return the created command object
         */
        virtual MVIMPSTCmd* CreateCommandForIdL(
                TImCommandId aCommandId,TAny* aData = NULL)  =0;
       
        /**
         * AddObserverL :add the observer from ui to get back the command events
         * @param: aObserver 
         */
       virtual void AddObserverL(MVIMPSTCmdObserver* aObserver) = 0;    
       
       
        /**
         * RemoveObserverL :remoces the observer from ui.
         * @param: aObserver 
         */
       virtual void RemoveObserver(MVIMPSTCmdObserver* aObserver ) = 0;
       
       /**
        * Return a reference to the precess array.
        * @return MVIMPSTProcessArray, reference of precess array.
        */      
       virtual MVIMPSTProcessArray& GetProcessInterface() = 0;       
		                
    };

#endif // MVIMPSTCMDFACTORY_H

// End of File
