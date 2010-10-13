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
* Description:  Class handling the use of Cch
*
*/


#ifndef     C_CIMCVENGINECCHHANDLER_H
#define     C_CIMCVENGINECCHHANDLER_H

// INCLUDES
#include <e32base.h>
#include <cch.h>

#include "cimcvengine.h"
#include "mimcvenginecchhandler.h"
#include "mimcvenginecchobserver.h"
class CSPSettings;
// CLASS DECLARATION
/**
 *  CCH handler
 *  This class implements the handling of CCH
 *
 *  @lib imcvengine.lib
 *  @since S60 5.0
 */
 
 
NONSHARABLE_CLASS( CIMCVEngineCchHandler ) : public CBase,
										public MIMCVEngineCchHandler,
										public MCchServiceStatusObserver
    {
    public:

        /**
         * Two-phased constructor.
         * @param aServiceId, service id to monitor
         */
        static CIMCVEngineCchHandler* NewL(TUint aServiceId, 
        								CIMCVEngine& aEngine);

        /**
         * Two-phased constructor.
         * @param aServiceId, service id to monitor
         */
        static CIMCVEngineCchHandler* NewLC(TUint aServiceId, 
        									CIMCVEngine& aEngine);
        
        /**
         * Standard C++ destructor
         */    
        ~CIMCVEngineCchHandler();
        
	/**
         * Register for CCh Events
	 * @param aObserver - Observer
         */
        void RegisterObserver(MIMCVEngineCCHObserver* aObserver);
	
	/**
         * UnRegister for CCh Events
	 * 
         */
	void UnRegisterObserver();

        /**
         * Gets service state
         *
         * @since S60 5.0         
         * @param aCCHState service state (result)
         * @return TInt Standard Symbian error code
         */         
         TInt GetServiceState(TCCHSubserviceState& aCCHState);

        /**
         * Enables service
         *
         * @since S60 5.0         
         * @return TInt, error code
         */
        TInt EnableService();
            
        
        /**
         * Returns the User Id for this service
         * Returns HbufC* and the caller of this function should deallocate the same
         * @since S60 5.0         
         * @return HBufC*
         */
        HBufC* GetUserIdL();
        
        /**
         * Returns the XimpFw Protocol UID for this service
         * Returns TInt 
         * @since S60 5.0         
         * @return TInt , error code
         */
        TInt GetAdapterUidL(); 
        
        /**
         * Disables service
         *
         * @since S60 5.0         
         * @return TInt, error code
         */
        TInt DisableService();        
        
    
    	/**
         * IsServiceLoggedIn
         *
         * @since S60 5.0         
         * 
         * @return ETrue - if Service Logged In, EFalse - Service Not Logged In
         */         
         TBool IsServiceLoggedIn();
    
    private:
    	// methods from MCchServiceStatusObserver
        
        /**
         * @see MCchServiceStatusObserver
         */
		void ServiceStatusChanged(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus );    
				
		void DoHandleServiceStatusChangedL(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus ); 				 
    private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CIMCVEngineCchHandler(TUint aServiceId, 
        						CIMCVEngine& aEngine);
        
        /**
         * Performs the 2nd phase of construction.
         *
         */        
        void ConstructL();

   				
    private: // Data    

		// Own: CCH Client
		CCch*	iCchClient;		
		
		TUint iServiceId;	//Service Id
		
		CIMCVEngine& iEngine; //engine
		
		//doesnt own
		MIMCVEngineCCHObserver* iObserver;
        
    };

#endif // C_CIMCVENGINECCHHANDLER_H

// End of file
