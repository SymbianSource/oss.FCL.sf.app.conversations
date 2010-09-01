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
* Description: IM Settings command object 
*
*/


#ifndef CVIMPSTCMDCCA_H
#define CVIMPSTCMDCCA_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"
#include <mccaparameter.h>
#include <ccafactory.h>
#include <mccaconnection.h> 

// FORWARD DECLARATION

class MVIMPSTProcessArray;
class MVIMPSTEngine;
//CLASS DECLARATION
/**
 * IM Settings command object.
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdCCA) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aParameter, parammeter.
         * @param aConnection, ccaconnection.
         * @param aIndex, index.
         * @param mArrayProcess, reference to MVIMPSTProcessArray
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdCCA* NewL(
                const TInt aCommandId, TInt& aIndex, 
                MCCAConnection* aConnection,
                MVIMPSTProcessArray& mArrayProcess,
                MVIMPSTEngine& aEngine );
        /**
         * Destructor.
         */
        ~CVIMPSTCmdCCA();

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
        * @param aParameter, parammeter.
        * @param aConnection, ccaconnection.
        * @param aIndex, index.
        * @param mArrayProcess, reference to MVIMPSTProcessArray
        */
        CVIMPSTCmdCCA( const TInt aCommandId, 
                		TInt& aIndex, 
                		 MCCAConnection* aConnection,
                		MVIMPSTProcessArray& mArrayProcess,
                		MVIMPSTEngine& aEngine  );
         
       /**
        * symbian second constructor
        */       
        void ConstructL();
        
        /**
        * helper method to launch cca
        */ 
        void LaunchCcaL();
        
       
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //doesnt own
        MVIMPSTCmdObserver* iObserver;
        
        //index of the item in the list
        TInt& iIndex;
        
        //does not own : connection to CCA
         MCCAConnection* iConnection;
        // doesnt own:array process item 
        MVIMPSTProcessArray& iArrayProcess;
        
        MVIMPSTEngine& iEngine;
        
        //  own:parameter to pass to the CCA connection
        MCCAParameter* iParameter;
        

    };

#endif // CVIMPSTCMDCCA_H

// End of File
