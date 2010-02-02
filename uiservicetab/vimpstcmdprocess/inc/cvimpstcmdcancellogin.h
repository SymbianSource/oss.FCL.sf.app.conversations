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
* Description:  Cancel login command class header
*
*/


#ifndef CVIMPSTCMDCANCELLOGIN_H
#define CVIMPSTCMDCANCELLOGIN_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"


// FORWARD DECLARATION
class MVIMPSTEngine;

// CLASS DECLARATION
/**
 *  Login cancelling command object.
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdCancelLogin) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdCancelLogin* NewL(
                const TInt aCommandId ,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdCancelLogin();

    public: // From mvimpstcmd
       /**
        * See mvimpstcmd
        */
        void ExecuteLD();        
       /**
        * See mvimpstcmd
        */       
        void AddObserver(
                MVIMPSTCmdObserver& aObserver );
       /**
        * See mvimpstcmd
        */                
		TInt CommandId() const;
				
	   /**
        * See mvimpstcmd
        */	
		TInt Result() const;  
		
	         

    private: // Implementation
       /**
        * symbian default constructor
        * @param aCommandId Command id.
        * @param aEngine, reference to engine.
        */
        CVIMPSTCmdCancelLogin(
                const TInt aCommandId,
                MVIMPSTEngine& aEngine);

        
    private: // Data
        //Command id
        const TInt iCommandId;         
        
        //Service Id
        TUint32 iServiceId;
        
        //observer - Doesnt own
        MVIMPSTCmdObserver* iObserver;   
        
        //Doesnt own - engine interface
        MVIMPSTEngine& iEngine;

    };

#endif // CVIMPSTCmdCANCELLOGIN_H

// End of File
