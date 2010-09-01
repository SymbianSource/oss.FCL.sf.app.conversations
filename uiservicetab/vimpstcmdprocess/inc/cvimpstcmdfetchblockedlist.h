/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Fetch the blocked list from server
*
*/

#ifndef CVIMPSTCMDFETCHBLOCKEDLIST_H
#define CVIMPSTCMDFETCHBLOCKEDLIST_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"


// FORWARD DECLARATION
class MVIMPSTEngine;

// CLASS DECLARATION
/**
 * IM Login command object.
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdFetchBlockedContactList) : public CBase,
                                  					public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aServiceID, service id.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdFetchBlockedContactList* NewL(
                const TInt aCommandId , MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdFetchBlockedContactList();

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
        * @param aCommandId        Command id.
        * @param aServiceID, service id.
        * @param aEngine, reference to engine.
        */
        CVIMPSTCmdFetchBlockedContactList(
                const TInt aCommandId, MVIMPSTEngine& aEngine);
        
       /**
        * second phase constructor
        */       
        void ConstructL();
        
        
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

#endif // CVIMPSTCMDFETCHBLOCKEDLIST_H

// End of File
