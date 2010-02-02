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
* Description: execute command for blocking contact 
*
*/


#ifndef CVIMPSTCMDBLOCKCONTACT_H
#define CVIMPSTCMDBLOCKCONTACT_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"

// FORWARD DECLARATION

class MVIMPSTEngine;
class MVPbkContactLink;
class MVIMPSTProcessArray;
// CLASS DECLARATION
/**
 * IM Deletecontact command object.
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdBlockContact) : public CBase,
                                            public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aCommandId        Command id.
         * @param aContactId id of the contact to be blocked..
         * @param aEngine instance of engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdBlockContact* NewL(
                const TInt aCommandId ,const TDesC& aContactId,
                MVIMPSTProcessArray& mArrayProcess,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdBlockContact();

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
        * @param aContactId id of the contact to be deleted..
        * @param aEngine instance of engine.
        */
        CVIMPSTCmdBlockContact(
                const TInt aCommandId, 
                const TDesC& aContactId, 
                MVIMPSTProcessArray& mArrayProcess,
                MVIMPSTEngine& aEngine);
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //contact id
        const TDesC& iContactId;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        // not owns
        MVIMPSTProcessArray& iProcessArray;
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code
        TInt iError;

    };

#endif // CVIMPSTCmdBlockContact_H

// End of File
