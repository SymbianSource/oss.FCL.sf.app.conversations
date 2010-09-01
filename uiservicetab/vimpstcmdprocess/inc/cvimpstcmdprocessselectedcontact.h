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
* Description:  CVIMPSTCmdProcessSelectedContact command object
*
*/




#ifndef CVIMPSTCMDPROCESSSELECTEDCONTACT_H_
#define CVIMPSTCMDPROCESSSELECTEDCONTACT_H_

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "mvimpstcmd.h"

// FORWARD DECLARATION

class MVIMPSTEngine;
class MVPbkContactLinkArray;
struct TVPbkSelectedData;
/**
 * CVIMPSTCmdProcessSelectedContact command object.
 */
class CVIMPSTCmdProcessSelectedContact : public CBase,
                                         public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         * @param aCommandId Command id.
         * @param aSelectedEntries structure to hold packed links of selected phone book contact(s).
         * @param aEngine instance of engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdProcessSelectedContact* NewL( const TInt aCommandId ,
                                                       TVPbkSelectedData& aSelectedEntries,
                                                       MVIMPSTEngine& aEngine );

        /**
         * Destructor.
         */
        ~CVIMPSTCmdProcessSelectedContact();

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
        * @param aSelectedEntries structure to hold packed links of selected phone book contact(s).
        * @param aEngine instance of engine.
        */
        CVIMPSTCmdProcessSelectedContact( const TInt aCommandId, 
                                          TVPbkSelectedData& aSelectedEntries, 
                                          MVIMPSTEngine& aEngine );
                
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        /**
        * return ETrue if exist in block list
        */    
        TBool IsContactBlocked(const TDesC& aUserId);
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        // TVPbkSelectedData to holde packed links of selected contacts.
        TVPbkSelectedData& iSelectedEntries;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code.
        TInt iError;
    };

#endif /*CVIMPSTCMDPROCESSSELECTEDCONTACT_H_*/

// End of File

