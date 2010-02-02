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
* Description:  service manager callback implementation class
*
*/


#ifndef C_CICPSERVICEMANAGER_H
#define C_CICPSERVICEMANAGER_H


// INCLUDES

#include <e32base.h>
#include <cchservice.h>
#include <ximpbase.h>
#include <ximpcontext.h>
#include <ximpcontextobserver.h>
#include <ximpcontextstate.h>
#include <spdefinitions.h>

#include "timconnproviderenums.h"

#define MAX_LENGTH 512

class MXIMPContext; //for MXIMPContext
class MXIMPClient;
class CICPServiceRequest;
class CXmppSettingsApi; //protocol specific settings store
class CIMCacheFactory; // imcache factory
class MIMCacheUpdater; // cache updater
class CSPSettings; //SP Settings

/**
*  CIcpServiceManager class
*  @lib imconnectionprovider.dll
*/
class CIcpServiceManager : public CBase,
						   public MXIMPContextObserver
	{
	//for test case purpose
   	friend class Cimconnectionprovider_test;

public:	

    /**
    * Two-phased constructor
    */
	static CIcpServiceManager* NewL( MCchServiceObserver& aServiceObserver );

    /**
    * Destructor.
    */
	virtual ~CIcpServiceManager();

public: // New functions
    
    /**
     * Client enables service. CCHService implementation
     * should use MCCHServiceObserver for service state notifications.
     * @see MCCHServiceObserver
     * @param aServiceId
     */
    void EnableServiceL( TUint aServiceId,
                         TCCHSubserviceType aSubServiceType );
                                 
    /**
     * Client enables service. CCHService implementation
     * should use MCCHServiceObserver for service state notifications.
     * @see MCCHServiceObserver
     * @param aServiceId
     * @param aIapId
     */
    void EnableServiceL( TUint aServiceId,
                         TCCHSubserviceType aSubServiceType,
                         TUint aIapId );
    
    /**
     * Client disables service. 
     * @param aServiceId
     */
    void DisableServiceL( TUint aServiceId,
                          TCCHSubserviceType aSubServiceType );
    
    /**
     * Client checks is the service available by the IAP id array.
     * CCHService implementation should return ETrue if service is
     * registrable by the (some) IAP, array contains. 
     * @param aServiceId
     * @param aIapIdArray Id array of the currently available iaps.
     * @return ETrue if service is available.
     */
    TBool IsAvailableL( TUint aServiceId,
                        TCCHSubserviceType aSubServiceType,
                        const RArray<TUint32>& aIapIdArray ) const;
    
    /**
     * Get service state information.
     * @param aServiceId
     * @param aState Current service state.
     * @return General symbian error code.
     */
    TInt GetServiceState( TUint aServiceId,
                          TCCHSubserviceType aSubServiceType, 
                          TCCHSubserviceState& aState ) const;
    
    /**
     * Get service network information.
     * @param aServiceId
     * @param aSnapId Used Snap id (should be set as null if
     *               not in use)
     * @pamar aIapId Used IAP id (should be set as null if
     *               not in use)
     * @return General symbian error code.
     */
    TInt GetServiceNetworkInfoL( TUint aServiceId,
                                TCCHSubserviceType aSubServiceType,
                                TUint32& aSnapId, 
                                TUint32& aIapId,
                                TBool& aSnapLocked,
                                TBool& aPasswordSet) const;
                                        
    /**
     * Set Snap id for service.
     * @param aServiceId
     * @param aSnapId 
     * @return General symbian error code.
     */
    TInt SetSnapId( TUint aServiceId,
                    TCCHSubserviceType aSubServiceType,
                    TUint aSnapId );
                    
    /**
     * Set IAP id for service.
     * @param aServiceId
     * @param aSnapId 
     * @return General symbian error code.
     */
    TInt SetIapId( TUint aServiceId,
                   TCCHSubserviceType aSubServiceType,
                   TUint aIapId );                               
    
    /**
     * Get service specifics information about service and protocol
     * @param aServiceId
     * @param aSubservice
     * @param aBuffer  
     */
    void GetServiceInfoL( TUint aServiceId,
                          TCCHSubserviceType aSubServiceType,
                          RBuf& aBuffer ) const;

    /**
     * Sets service reserved or unreserved
     * @param aReserved Reserved
     * @param aServiceId
     * @param aSubServiceType Sub service type
     * @return Symbian error codes
     */
    TInt SetServiceReserved( TBool aReserved,
                             TUint aServiceId,
                             TCCHSubserviceType aSubServiceType );
                             
                             
      /**
     * Returns the connection parameters
     * @since S60 3.2.3
     * @param aServiceId Selected service id 
     * @param aServiceId Selected service type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt GetConnectionParameter( TUint aServiceId,
                         TCCHSubserviceType aSubServiceType,
                         TCchConnectionParameter aParameter,
                         TInt& aValue ) const;
    
    /**
     * Returns the connection parameters
     * @since S60 3.2.3
     * @param aServiceId Selected service id 
     * @param aServiceId Selected service type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
     void GetConnectionParameterL(TUint aServiceId,
                         TCCHSubserviceType aSubServiceType, 
                         TCchConnectionParameter aParameter,
                         RBuf& aValue ) const;
    
    /**
     * Sets the connection parameters
     * @since S60 3.2.3
     * @param aServiceId Selected service id 
     * @param aServiceId Selected service type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt SetConnectionParameter( TUint aServiceId,
                         TCCHSubserviceType aSubServiceType, 
                         TCchConnectionParameter aParameter,
                         TInt aValue );
    
    /**
      * Sets the connection parameters
      * @since S60 3.2.3
      * @param aServiceId Selected service id 
      * @param aServiceId Selected service type
      * @param aParameter Connection parameter of the service
      * @param aValue Value of the parameter
      * @return Symbian error code
      */
     void SetConnectionParameterL( TUint aServiceId,
                         TCCHSubserviceType aSubServiceType, 
                          TCchConnectionParameter aParameter,
                          const TDesC& aValue );

    /**
     * Returns service reserved info
     * @param aServiceId
     * @param aSubServiceType Sub service type
     * @return ETrue if reserved
     */
    TBool IsReserved( TUint aServiceId,
                      TCCHSubserviceType aSubServiceType ) const;                              
     
    /**
     * Profile event call back.
     * @since S60 3.2
     * @param aProfileId SIP profile id.
     * @param aEvent a new profile event. 
     */
    void ProfileEventOccurred( const TUint32 aProfileId,
                               TCCHSubserviceState aEvent, TInt aError );
                               
    /**
     * Service event call back.
     * @since S60 3.2
     * @param aServiceId Service id.
     * @param aEvent a new profile event. 
     */
    void ServiceEventOccurred( const TUint32 aServiceId,
                               const TCCHSubserviceType aSubService,
                               TCCHSubserviceState aEvent, TInt aError );
    
    
    /**
     * Handles presence context event
     * @param aContext
     * @param aEvent Event
     */                             
    void HandlePresenceContextEvent( const MXIMPContext& aContext,
                                     const MXIMPBase& aEvent );
                                     
                                     
    
    /**
     * Handles bind to the presence context
     * @param aServiceSettingId
     */                                 		
    void BindL(TInt aServiceSettingId);
    
    /**
     * Handles unbind to the presence context
     */
    void UnBindL();
                                               
private:

	TInt ConvertXIMPErrToCChErr(const TInt aXimpErr);
	
    /**
     * Default constructor
     * @param aStateObserver State Observer
     */
    CIcpServiceManager( MCchServiceObserver& aServiceObserver );
    
    /**
     * 2nd phase constructor
     */
    void ConstructL();
    
    
	/**
	* FindRequestId   
	* find the request id
	* @param aRequestId to find
	*/
	CICPServiceRequest* FindRequestId(TXIMPRequestId& aRequestId ) ;
	
	
	/**
	* RemoveRequestId 
	* remove from the array
	* @param aRequestId  ,to remove 
	*/
	CICPServiceRequest* RemoveRequestId(TXIMPRequestId& aRequestId) ;
	
	
	/** Create a Request
	* @Returns the Ptr to the new request created
	* @param - aRequestId
	* @param - aType
	*/	
	CICPServiceRequest* CreateRequestL(TXIMPRequestId& aRequestId,
							TIMConnProviderEnums::TRequestTypes aType );

	
    
      /**
       * SetIMDisabledL
       * @param aServiceId: ServiceId  
       */
      void SetIMDisabledL(TUint32 aServiceId);
      
      
	TBool ValidateServiceL( TUint32 aServiceId ) const;
	TInt GetSPSettingsIntPropertyL( TUint32 aServiceId, 
								TServicePropertyName aPropertyName ) const;


	TBool ValidateSubService( TCCHSubserviceType aSubServiceType ) const;
	
	
    
    /**
         * Gets the name of the service
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param aServiceName, service name is copied to this parameter
         *	 caller should allocate memory for this parameter
         * @return None
         */                                             
        void GetServiceNameL( 
            TUint32 aServiceId, TDes& aServiceName ); 

private:  

    /**
     * Observer for state changes
     */
    MCchServiceObserver& iServiceObserver; 
    
    /**
     * Event
     */
    TCCHSubserviceState          iOngoingState;    
    
    /**
     * Ongoing service id and subservice type
     */
     
    TUint                      iIapId;
    
    TUint                      iSnapId;
    
    TInt                       iServiceId;
    
    TInt                       iLastReportedError;   
    /** 
     * Ximp specific data handling 
     */
     //owns
    MXIMPClient* iPresClient;
    
    //owns
    MXIMPContext* iPresenceCtx;
        
    // Own
    RPointerArray<CICPServiceRequest> iReqIDArray;    
    
    // Owned , settings api to set and reset owndata.
    CXmppSettingsApi* iXmppParameters;  
    
	//Stores the XIMPFw EventTypes Subscribed for		
    RArray< TInt32 > iAcceptedEventTypes;
    
    //stores the userid
    HBufC* iOwnUserId;    
    
    // owned, imcache factory ,call release on it.
    CIMCacheFactory* iIMCacheFactory;
    
    // owned,by factory, cache updater 
    MIMCacheUpdater* iIMCacheUpdater ;
    
    HBufC* iServiceName;
    
    CSPSettings *iCSPSetting;
    
    #ifdef _DEBUG
	#endif 	 
        
	};	

#endif  // C_CICPSERVICEMANAGER_H

// End of file

