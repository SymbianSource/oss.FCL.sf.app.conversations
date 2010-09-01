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
* Description:  
*
*/


#ifndef CVIMPSTCMDADDFROMPBK_H_
#define CVIMPSTCMDADDFROMPBK_H_


// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "mvimpstcmd.h"

// FORWARD DECLARATION

class MVIMPSTEngine;
class MVPbkStoreContact;

/**
 * CVIMPSTCmdAddFromPbk command object.
 */
NONSHARABLE_CLASS(CVIMPSTCmdAddFromPbk) : public CBase,
                                          public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aCommandId Command id.
         * @param aStoreContact phone book contact to be added.
         * @param aEngine instance of engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdAddFromPbk* NewL( const TInt aCommandId ,
                                           const MVPbkStoreContact& aStoreContact,
                                           MVIMPSTEngine& aEngine );

        /**
         * Destructor.
         */
        ~CVIMPSTCmdAddFromPbk();

    public: // From mvimpstcmd
       /**
        * see mvimpstcmd
        */
        void ExecuteLD();        
       /**
        * see mvimpstcmd
        */
        void AddObserver( MVIMPSTCmdObserver& aObserver );
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
        * @param aCommandId Command id.
        * @param aStoreContact phone book contact to be aded..
        * @param aEngine instance of engine.
        */
        CVIMPSTCmdAddFromPbk( const TInt aCommandId, 
                              const MVPbkStoreContact& aStoreContact, 
                              MVIMPSTEngine& aEngine );
                
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        /**
         * symbian second phase constructor
         */
        void GetServiceFieldsL( CDesCArray& aArrayForServiceFields );
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //phone book store contact, not owned.
        const MVPbkStoreContact& iStoreContact;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code.
        TInt iError;
    };

#endif /*CVIMPSTCMDADDFROMPBK_H_*/

// End of File
