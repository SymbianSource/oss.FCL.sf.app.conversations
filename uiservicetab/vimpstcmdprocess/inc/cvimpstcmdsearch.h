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
* Description:  Search Command header
*
*/


#ifndef CVIMPSTCMDSEARCH_H
#define CVIMPSTCMDSEARCH_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"
#include "tvimpstconsts.h"
#include "tvimpstenums.h"


// FORWARD DECLARATION
class MVIMPSTEngine;

// CLASS DECLARATION
/**
 * IM Login command object.
 * @lib vimpstcmdprocess.dll
 * @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdSearch) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aKeyDataArray, array of keys for search.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdSearch* NewL(
                const TInt aCommandId ,
                RArray<TVIMPSTSearchKeyData>& aKeyDataArray,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdSearch();

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
        * @param aKeyDataArray, array of keys for search.
        * @param aEngine, reference to engine.
        */
        CVIMPSTCmdSearch(
                const TInt aCommandId, 
                RArray<TVIMPSTSearchKeyData>& aKeyDataArray, 
                MVIMPSTEngine& aEngine);
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
         //own array of search ids and values
        RArray<TVIMPSTSearchKeyData>& iKeyDataArray;
             
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code.
        TInt iError;

    };

#endif // CVIMPSTCMDSEARCH_H

// End of File
