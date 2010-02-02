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
* Description:  IM AddContact command object
*
*/


#ifndef CVIMPSTCMDADDTOPBK_H_
#define CVIMPSTCMDADDTOPBK_H_


// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"

// FORWARD DECLARATION

class MVIMPSTEngine;
class MVIMPSTStorageContact;

/**
 * IM AddContact command object.
 */
NONSHARABLE_CLASS(CVIMPSTCmdAddToPbk) : public CBase,
                                        public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aCommandId        Command id.
         * @param aStoreContact contact to be added to phone book.
         * @param aEngine instance of engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdAddToPbk* NewL( const TInt aCommandId ,
                                         const MVIMPSTStorageContact& aStoreContact,
                                         MVIMPSTEngine& aEngine );

        /**
         * Destructor.
         */
        ~CVIMPSTCmdAddToPbk();

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
        * @param aCommandId        Command id.
        * @param aContactId id of the contact to be aded..
        * @param aEngine instance of engine.
        */
        CVIMPSTCmdAddToPbk( const TInt aCommandId, 
                            const MVIMPSTStorageContact& aStoreContact, 
                            MVIMPSTEngine& aEngine );
                
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //Vpbk store contact, not owned.
        const MVIMPSTStorageContact& iStoreContact;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code.
        TInt iError;
    };

#endif /*CVIMPSTCMDADDTOPBK_H_*/

// End of File
