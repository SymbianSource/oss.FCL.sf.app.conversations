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
* Description:   IM Settings command object
*
*/


#ifndef CVIMPSTCMDCALL_H
#define CVIMPSTCMDCALL_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"

// CLASS DECLARATION

class MVIMPSTProcessArray;

/**
 * IM Settings command object.
 */
NONSHARABLE_CLASS(CVIMPSTCmdCall) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdCall* NewL(
                const TInt aCommandId, TInt& aIndex, 
                MVIMPSTProcessArray& mArrayProcess,
                TInt aWindowGroup,
                TUint32 aServiceId);
        /**
         * Destructor.
         */
        ~CVIMPSTCmdCall();

    public: // From MImCommand
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
        */
        CVIMPSTCmdCall(const TInt aCommandId, TInt& aIndex, 
		        MVIMPSTProcessArray& mArrayProcess,
		        TInt aWindowGroup,
		        TUint32 aServiceId);
       /**
        * symbian second phase constructor
        */        
        void ConstructL();
        
       
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //Doesnt own
        MVIMPSTCmdObserver* iObserver;
        
        //Index of the list item chosen
        TInt& iIndex;
        
        //Doesnt own
        MVIMPSTProcessArray& iArrayProcess;
        
        //window group
        TInt iWindowGroup;
        
        //Service Id
        TUint32 iServiceId;
        

    };

#endif // CVIMPSTCMDCALL_H

// End of File
