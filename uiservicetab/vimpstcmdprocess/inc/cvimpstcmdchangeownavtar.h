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
* Description: IM Change Avtar command object 
*
*/


#ifndef CVIMPSTCMDCHANGEOWNAVTAR_H
#define CVIMPSTCMDCHANGEOWNAVTAR_H

// INCLUDES
#include <e32base.h>
#include "mvimpstcmd.h"
#include "tvimpstconsts.h"
#include "tvimpstenums.h"

// FORWARD DECLARATION

class MVIMPSTEngine;

/**
 * IM Change Avtar command object.
 */
NONSHARABLE_CLASS(CVIMPSTCmdChangeOwnAvtar) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aFileName         Avtar file name
         * @param aEngine           Engine 
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdChangeOwnAvtar* NewL(
                const TInt aCommandId ,const TAvatarData& aData,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdChangeOwnAvtar();

    public: // From mvimpstcmd
		   /**
		    * @see mvimpstcmd
		    */
		    void ExecuteLD();        
		   /**
		    * @see mvimpstcmd
		    */
		    void AddObserver(
		            MVIMPSTCmdObserver& aObserver );
		   /**
		    * @see mvimpstcmd
		    */
				TInt CommandId() const;		
		 		/**
		    * @see mvimpstcmd
		    */	
				TInt Result() const;
		       

    private: // Implementation
    
       /**
        * symbian default constructor
        */
        CVIMPSTCmdChangeOwnAvtar(
                const TInt aCommandId, 
                const TAvatarData& aData, 
                MVIMPSTEngine& aEngine);
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        /** 
         * Command id
         */
        const TInt iCommandId;  
        
        /**
         * avatar data.file name and mimetype
         */
        const TAvatarData& iData;
        
        /**
         * observer 
         * doesnt own
         */
        MVIMPSTCmdObserver* iObserver;  
        
        /** 
         * doesnt own
         * engine for this service
         */
        MVIMPSTEngine& iEngine;
        
        /**
         * error code.
         */
        TInt iError;

    };

#endif // CVIMPSTCMDCHANGEOWNAVTAR_H

// End of File
