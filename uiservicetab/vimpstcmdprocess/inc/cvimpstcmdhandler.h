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
* Description:  IM command handler
*
*/


#ifndef CVIMPSTCMDHANDLER_H
#define CVIMPSTCMDHANDLER_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmdhandler.h"

// FORWARDS
class MVIMPSTCmdFactory;
class MVIMPSTEngine;

//  CLASS DECLARATION

/**
 * IM command handler.
 * Responsible for handling all IM commands 
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
class CVIMPSTCmdHandler : public CBase,
                            public MVIMPSTCmdHandler
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        IMPORT_C static CVIMPSTCmdHandler* NewL(MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdHandler();

    public: // From MVIMPSTCmdHandler
        
        /**
         * see MVIMPSTCmdHandler
         */
        TBool HandleCommandL(
                const TImCommandId aCommandId, MVIMPSTCmdObserver* aObserver,
                TAny* aData );
        
        /**
         * see MVIMPSTCmdHandler
         */
        void RegisterEventObserverL( MVIMPSTCmdObserver* aObserver ) ;
        
        /**
         * see MVIMPSTCmdHandler
         */
        void UnRegisterEventObserver( MVIMPSTCmdObserver* aObserver ) ;
        
        /**
         * see MVIMPSTCmdHandler
         */
        MVIMPSTProcessArray& GetProcessInterface() ;
        
    private: // Implementation
       /**
        * symbian default constructor
        */
        CVIMPSTCmdHandler();
       /**
        * second phase constructor
        */
        void ConstructL(MVIMPSTEngine& aEngine);
        

    private: // Data
        /// Own: Command factory
        MVIMPSTCmdFactory* iCommandFactory;        
    };

#endif // CVIMPSTCMDHANDLER_H

// End of File
