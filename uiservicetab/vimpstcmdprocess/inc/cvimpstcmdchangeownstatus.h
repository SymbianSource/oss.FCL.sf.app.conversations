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
* Description:  IM Login command object
*
*/


#ifndef CVIMPSTCMDCHANGEOWNSTATUS_H
#define CVIMPSTCMDCHANGEOWNSTATUS_H

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
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdChangeOwnStatus) : public CBase,
                                  public MVIMPSTCmd
    {
    public: // Construction and destruction

        /**
         * Creates a new instance of this class.
         *
         * @param aCommandId        Command id.
         * @param aStatus, structure containing status and status message.
         * @param aEngine, reference to engine.
         * @return  A new instance of this class.
         */
        static CVIMPSTCmdChangeOwnStatus* NewL(
                const TInt aCommandId ,TStatusAndStatusText aStatus,
                MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdChangeOwnStatus();

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
        * @param aStatus, structure containing status and status message.
        * @param aEngine, reference to engine.
        */
        CVIMPSTCmdChangeOwnStatus(
                const TInt aCommandId, 
                TStatusAndStatusText aStatus, 
                MVIMPSTEngine& aEngine);
       /**
        * symbian second phase constructor
        */         
        void ConstructL();
        
        
    private: // Data
        
        //Command id
        const TInt iCommandId;  
        
        //contact id
        TBuf<50> iStatusText;
        
        // online status.
        TVIMPSTEnums::TOnlineStatus iStatus;
        
        //observer - doesnt own
        MVIMPSTCmdObserver* iObserver;  
        
        //doesnt own - engine for this service
        MVIMPSTEngine& iEngine;
        
        //error code.
        TInt iError;

    };

#endif // CVIMPSTCMDCHANGEOWNSTATUS_H

// End of File
