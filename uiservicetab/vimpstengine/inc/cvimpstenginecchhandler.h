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


#ifndef     C_CVIMPSTENGINECCHHANDLER_H
#define     C_CVIMPSTENGINECCHHANDLER_H

// INCLUDES
#include <e32base.h>
#include <cch.h>
#include <cchuiobserver.h>
#include "mvimpstenginecchhandlerobserver.h"
#include "tvimpstenums.h"
#include "mvimpstenginecchuieventobserver.h"

#include "cvimpstsettingsstore.h"
/**
 *  structure.
 */
struct TObserverStructure
	{
	MVIMPSTEngineCchHandlerObserver* iObserver; /* observer */
	TCCHSubserviceType iSubserviceType; /* subservice type(IM/VOIP/PRESENCE */
	}; 

// CLASS DECLARATION
/**
 *  CCH handler
 *  This class implements the handling of CCH
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */ 
NONSHARABLE_CLASS( CVIMPSTEngineCchHandler ) : public CBase,
							 public MCchServiceStatusObserver,
							 public MCchUiObserver
							 
    {
    //for test cases.
    friend class T_VimpstEngineCchHandler;
    public:

        /**
         * Two-phased constructor.
         * @param aServiceId, service id to monitor
         * @return a new instace of this class
         */
        static CVIMPSTEngineCchHandler* NewL(TUint aServiceId, MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver);

        /**
         * Two-phased constructor.
         * @param aServiceId, service id to monitor
         * @return a new instace of this class
         * 
         */
        static CVIMPSTEngineCchHandler* NewLC(TUint aServiceId,MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver );
        
        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEngineCchHandler();
        
        /**
         * Shuts down connection to CCH.
         *
         * @since S60 5.0
         * @return None
         */
        void ShutdownCch();
        
        /**
         * Gets service state
         *
         * @since S60 5.0
         * @param aServiceId service id
         * @param aSubServiceType subservice typ(IM/VOIP/PRESENCE)
         * @param aCCHState state of the cch.
         * @return TInt Standard Symbian error code
         */         
         TInt GetServiceState(TUint aServiceId,
									TInt aSubServiceType, 
									TCCHSubserviceState& aCCHState
									);

        /**
         * Enables service
         * @since S60 5.0
         * @return TInt, error code
         */
        TInt EnableService();

        /**
         * Disables service
         * @since S60 5.0
         * @return None
         */
        TInt DisableService();
                
    
       	/**
       	 * Register for CCH Events
       	 * @param aObserver
       	 * @param aSubServiceType type of subservice
       	 */
        void RegisterCchObserverL(MVIMPSTEngineCchHandlerObserver* aObserver,
                                                   TCCHSubserviceType aSubServiceType );
        
        /**
       	 * UnRegister for CCH Events
       	 * @param aSubServiceType - type of subservice
       	 */                                           
        void UnRegisterCchObserver( TCCHSubserviceType aSubServiceType );
        
        
        /**
         * Returns the connection parameters
         * @param aSubServiceType type of subservice
         * @return own data - Ownership Transferred to the caller
         */        
        HBufC* GetConParametersL(			
		    TCchConnectionParameter aConnParam );
        
        /**
         * Set cch connection parameter
         *
         * @since S60 v5.0
         * @param aServiceId service id
         * @param aConnParam cch connection parameter
         * @param aConnParamValue connection parameter value
         * @return Error code.
         */
        TInt SetConnectionParameter(
           TCchConnectionParameter aConnParam,
           const TDesC& aConnParamValue );
           
		
		/**
		 * Change Connection
		 * Shows CCHUI dialog for changing the Network Connection for this service
		 * ECchUiDialogTypeChangeConnection
		 * @return Error Code
		 */
		TInt ChangeConnectionL();
		
		/**
		* For getting cchui api.
		* 
		* @return reference to cchui api.
		*/
    	MCchUi& CchUiApi() const;
            
    	/**
        * For checking whether password available in settings or not
        * @param aConnection Param value 
        * @Return true if password is present in the settings else returns false    
        */
    	TBool IsPasswordAvailable( TCchConnectionParameter aConnParam );
    public: // methods from MCchServiceObserver
        
        /**
         * @see MCchServiceObserver
         */
		void ServiceStatusChanged(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus );

		
		// methods from MCchUiObserver
        /**
		   * @see MCchUiObserver
		 */

		void ConnectivityDialogsCompletedL(
		        TInt aServiceId, 
		        MCchUiObserver::TCchUiOperationResult aOperationResult );

		    
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceId service id.
         */    
        CVIMPSTEngineCchHandler(TUint aServiceId, MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver);
        
        /**
         * Performs the 2nd phase of construction.
         */        
        void ConstructL();

    private: // Data
    
        // does not own array of observers.
        RArray<TObserverStructure> iObserverStructure;
        MVIMPSTSettingsStore* iSettingsStore;
		// Own CCH Client
		CCch*	iCchClient;		
		
		// service id
		TUint iServiceId;
		MVIMPSTEngineCchUiEventObserver& iCchUiEventObserver;		
    };

#endif // C_CVIMPSTENGINECCHHANDLER_H

// End of file
