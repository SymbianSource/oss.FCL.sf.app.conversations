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
* Description:  Class that capsulates single service
*
*/


#ifndef     C_CVIMPSTENGINEIMSERVICESTATE_H
#define     C_CVIMPSTENGINEIMSERVICESTATE_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "tvimpstenums.h"

#include "mvimpstcchhandlerobserver.h"

// FORWARD DECLARATINS
class CVIMPSTEngineServiceTableFetcher;

class MXIMPContext;
class CVIMPSTEngineSessionCntxtObserver;
class MVIMPSTServiceSessionObserver;
class CXmppSettingsApi;
class MPresenceFeatures;
class CVIMPSTEngineCchHandler;
class CVIMPSTServiceStateListener;
class MVIMPSTEngineFetchCompleteObserver;

// CLASS DECLARATION

/**
 *  
 *  This class represents IM sub service
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTEngineImServiceState ) : public CBase,
                  								    public MVIMPSTEngineCchHandlerObserver
    {
    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @param aServiceId, service id
         * @param aPresenceContext, presence context
         * @param aEventObserver, presence context observer
         * @param aTableFetcher, service table fetcher object
         * @param aXmppParameters, accessor to settings api
         * @param aSettingId, settings id
         * @param aCchHandler, reference to cchhandler
         * @param aObserver,fetch complete observer
         * @return ptr to CVIMPSTEngineServiceState   
         */
        static CVIMPSTEngineImServiceState* NewL( TUint aServiceId, 
        						MXIMPContext& aPresenceContext,
        						CVIMPSTEngineSessionCntxtObserver& aEventObserver,
        						CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        						CXmppSettingsApi& aXmppParameters,
    							TUint aSettingId,
    							CVIMPSTEngineCchHandler& aCchHandler,
    							MVIMPSTEngineFetchCompleteObserver& aObserver );

        /**
         * Two-phased constructor.
         *
         * @param aServiceId, service id
         * @param aPresenceContext, presence context
         * @param aEventObserver, presence context observer
         * @param aTableFetcher, service table fetcher object
         * @param aXmppParameters, accessor to settings api
         * @param aSettingId, settings id
         * @param aCchHandler, reference to cchhandler
         * @param aObserver,fetch complete observer
         * @return ptr to CVIMPSTEngineServiceState
         */
        static CVIMPSTEngineImServiceState* NewLC( TUint aServiceId, 
        						MXIMPContext& aPresenceContext,
        						CVIMPSTEngineSessionCntxtObserver& aEventObserver,
        						CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        						CXmppSettingsApi& aXmppParameters,
    							TUint aSettingId,
    							CVIMPSTEngineCchHandler& aCchHandler,
    							MVIMPSTEngineFetchCompleteObserver& aObserver );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEngineImServiceState();
        
    private: // From MVIMPSTEngineCchHandlerObserver
        
        /**
         * From MVIMPSTEngineCchHandlerObserver 
         * Observer callback for CCH event
         *
         * @param aServiceId, service id
         * @param aState, service state
         * @param aServiceError, error
         * @since S60 5.0
         */
        void CchEventOccuredL( 
            TUint aServiceId, 
            TCCHSubserviceState aState, 
            TInt aServiceError );
        
        /**
         * Resolves IM service state for given service id.
         *
         * @since S60 3.2
         * @param aServiceId, service id
         * @param aCchErr, service error is stored here
         * @return TVIMPSTRegistrationState,
         * service state to use
         */  
        TVIMPSTEnums::TVIMPSTRegistrationState ResolveImServiceState( 
		        				TUint aServiceId, TInt& aCchErr ) ;
	public: 
        /**
         * Returns service ID
         *
         * @return TUint32
         */                                    

		TUint32 ServiceId();
               
        /**
         * Sets service state.
         *
         * @since S60 5.0
         * @param aNewState, service state to set
         */                                                 
        void SetServiceState( 
            TVIMPSTEnums::TVIMPSTRegistrationState aNewState );
		
      	/**
		* does unbinding of the context
		*/
		TInt UnBindL() ;
		
		/**
		* binds the context.
		*/
		TInt BindL( TUid aProtocolImpUid,
				       TInt32 aIapId  ) ;

		
		/**
		* returns the state of the this service
		* @return TVIMPSTRegistrationState.
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState ServiceState();
		
		/**
		* cotacts updation completed
		*/
		void ContactsUpdateCompletedL();
            
    public:
    	/**
    	 * Register Session Observer
    	 * @param aObserver observer to be registered
    	 */
    	 void RegisterServiceSessionObserver
    	 					(MVIMPSTServiceSessionObserver* aObserver);
    	 
    	/**
    	 * UnRegisters Session Observer
    	 * @param aObserver observer to be unregistered.
    	 */
    	 void UnRegisterServiceSessionObserver
    	 					(MVIMPSTServiceSessionObserver* aObserver);
    		
    	 
     
    private:
    
        /**
         * Handles cch error events.
         *
         * @since S60 5.0
         * @param aServiceError, service error code
         */                                          
        void DoHandleCchErrorL( TInt aServiceError );
        
        /**
         * NotifyObserver of the change in Service State
         * @param aServiceError service error
         */
         void NotifyObserversL(TInt aServiceError);
         
         
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceId, service id
         * @param aPresenceContext, presence context
         * @param aEventObserver, presence context observer
         * @param aTableFetcher, service table fetcher object
         * @param aXmppParameters, accessor to settings api
         * @param aSettingId, settings id
         * @param aCchHandler, reference to cchhandler
         */    
        CVIMPSTEngineImServiceState( TUint aServiceId, 
        						MXIMPContext& aPresenceContext,
        						CVIMPSTEngineSessionCntxtObserver& aEventObserver,
        						CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        						CXmppSettingsApi& aXmppParameters,
    							TUint aSettingId,
    							CVIMPSTEngineCchHandler& aCchHandler);

        /**
         * Performs the 2nd phase of construction.
         * @param aObserver,fetch complete observer
         */             
        void ConstructL( MVIMPSTEngineFetchCompleteObserver& aObserver );
        
    private: // Data
    
        // Service id of this service
        TUint32   iServiceId;
    
               
        // Stores services current state
        TVIMPSTEnums::TVIMPSTRegistrationState  iServiceState;
        
        //doen not own reference to presence context
        MXIMPContext& iPresenceContext;
		
		//does not own reference to context observer			
		CVIMPSTEngineSessionCntxtObserver& iEventObserver;
		
		//Ref to the service table getter functions
		CVIMPSTEngineServiceTableFetcher& iTableFetcher;
		
		// reference to cenrep api.
		CXmppSettingsApi& iXmppParameters;
        
        //settings id
        TUint iSettingId;
        
        // owned  pointer to presence plugin
        MPresenceFeatures* iPresFeatures;

		//Doesn't Own
		RPointerArray <MVIMPSTServiceSessionObserver> iObservers;
		
		// will be ETrue if Logged in to the service
		TBool iLoggedIn;
		
		//handler to the CCh, doesnt own
		CVIMPSTEngineCchHandler& iCchHandler;
		
		// owns : state change lsitener
       CVIMPSTServiceStateListener* iStateListener;
       
       //says whether a bind operation was initiated from here
       TBool iBindOperationInitiated;
	
    };

#endif // C_CVIMPSTENGINEIMSERVICESTATE_H

// End of file
