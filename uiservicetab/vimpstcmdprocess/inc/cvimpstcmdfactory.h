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
* Description:  Creates IM command objects.
*
*/


#ifndef CVIMPSTCMDFACTORY_H
#define CVIMPSTCMDFACTORY_H

//  INCLUDES
#include <e32base.h>
#include "mvimpstcmdfactory.h"
#include "tvimpstenums.h"
#include "mvimpstengineservicestateeventobserver.h"

// FORWARD DECLARATIONS
class MVIMPSTProcessArray;
class MVIMPSTEngine;

//  CLASS DECLARATION
/**
 * Factory to create IM command objects.
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
NONSHARABLE_CLASS(CVIMPSTCmdFactory) :
        public CBase,
        public MVIMPSTCmdFactory,
        public MVIMPSTEngineServiceStateEventObserver
    {
    public: // Constructor and destructor
        /**
         * Static constructor.
         * @param aEngine, reference to engine.
         * @return A new instance of this class.
         */
        static CVIMPSTCmdFactory* NewL(MVIMPSTEngine& aEngine);

        /**
         * Destructor.
         */
        ~CVIMPSTCmdFactory();

    public: // From MImCommandFactory
        /**
         * See MImCommandFactory
         */
		MVIMPSTCmd* CreateCommandForIdL(
		        TImCommandId aCommandId,TAny* aData = NULL) ;

        /**
         * See MImCommandFactory
         */
		void AddObserverL(MVIMPSTCmdObserver* aObserver) ;    

		/**
         * See MImCommandFactory
         */
		void RemoveObserver(MVIMPSTCmdObserver* aObserver);

		/**
		 * @returns reference to process interface.
		 */
		MVIMPSTProcessArray& GetProcessInterface() ;    
    
    private:// form MVIMPSTEngineServiceStateEventObserver
        
       /**
	    * Observer Event for Service State changes
	    * @param aState - Indicates the service state
	    * @parma aServiceError, error if any.
	    */       
	 	void HandleServiceEventL
	    			( TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError ) ;
            
    private: // Implementation
        /**
         * symbian default constructor.
         * @param aEngine, reference to engine.
         */
        CVIMPSTCmdFactory(MVIMPSTEngine& aEngine);
        
        /**
         * Second phase constructor.
         */
        void ConstructL();

    private: // Data
        // owned ,pointer to CIMArrayProcess
    	MVIMPSTProcessArray* iArrayProcess;
        
        //not owned        
        RPointerArray<MVIMPSTCmdObserver> iObservers;
        
        //Ref to the engine item for current service
        MVIMPSTEngine& iEngine;
        
    };

#endif // CVIMPSTCMDFACTORY_H

// End of File
