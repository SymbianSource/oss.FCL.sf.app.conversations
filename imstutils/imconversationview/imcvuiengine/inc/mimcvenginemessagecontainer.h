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
* Description:  message container interface
*
*/


#ifndef MIMCVENGINEMESSAGECONTAINER_H
#define MIMCVENGINEMESSAGECONTAINER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MIMCVEngineMessage;

// CLASS DECLARATION

/**
*  UI side message container interface
*
*  imcvuiapp.exe
*  @since Series 60 3.0
*/
class MIMCVEngineMessageContainer
    {
    public: // New functions

        /**
         * Add new message to container
         * @param aMessage Message
         */
        virtual void AddMessageL( MIMCVEngineMessage& aMessage ) = 0;

               
                       
         /**
         * Insert message to container start.
         * @since S60 v3.2
         * @param aMessage Message to be inserted.
         */
         virtual void InsertMessageL( MIMCVEngineMessage& aMessage ) = 0;
         
    protected:
        
        /**
		 * Destructor.
		 */
        virtual ~MIMCVEngineMessageContainer() {};
    
    };

#endif      // MIMCVENGINEMESSAGECONTAINER_H

// End of File
