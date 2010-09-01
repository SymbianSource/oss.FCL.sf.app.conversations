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
* Description:  IM command observer interface
*
*/


#ifndef MVIMPSTCMDOBSERVER_H
#define MVIMPSTCMDOBSERVER_H
#include "tvimpstenums.h"

// FORWARD DECLARATIONS
class MVIMPSTCmd;

// CLASS DECLARATION
/**
 * IM command observer interface.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
class MVIMPSTCmdObserver
    {
    public: // Interface

        /**
         * Notifies the command observer that the command has finished.
         * @param aCommand  The command that was finished.
         */
        virtual void CommandFinishedL(
                const MVIMPSTCmd& aCommand ) = 0;
        /**
         * Notifies the the ui on events from service manager.
         * @param aState  the state of login.
         * @param aServiceError, service error if any.
         */       
        virtual void HandleCommandEventL
        		(TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError) = 0;

    protected: // Disabled functions
        /**
         * virtual destructor.
         */  
        virtual ~MVIMPSTCmdObserver()
        {}
    };

#endif // MVIMPSTCMDOBSERVER_H

// End of File
