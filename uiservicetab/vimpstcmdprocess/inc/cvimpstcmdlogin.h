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
* Description: IM Login command object 
*
*/


#ifndef CVIMPSTCMDLOGIN_H
#define CVIMPSTCMDLOGIN_H

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
NONSHARABLE_CLASS(CVIMPSTCmdLogin) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aServiceId, service id.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdLogin* NewL(
                const TInt aCommandId ,TUint32 aServiceID,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdLogin();

    public: // From mvimpstcmd
       /**
        * see mvimpstcmd
        */
        void ExecuteLD();        
       /**
        * see mvimpstcmd
        */
        void AddObserver(
                MVIMPSTCmdObserver& aObserver );
       /**
        * see mvimpstcmd
        */
		TInt CommandId() const;		
	   /**
        * see mvimpstcmd
        */	
		TInt Result() const;  
		
	         

    private: // Implementation
    
       /**
        * symbian default constructor
        * @param aCommandId        Command id.
        * @param aServiceId, service id.
        * @param aEngine, reference to engine.
        */
        CVIMPSTCmdLogin(
                const TInt aCommandId, 
                TUint32 aServiceID, 
                MVIMPSTEngine& aEngine);
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //Service Id
        TUint32 iServiceId;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;

    };

#endif // CVIMPSTCMDLOGIN_H

// End of File
