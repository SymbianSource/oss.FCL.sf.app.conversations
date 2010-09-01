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
* Description:  conecction provider plugin implementation class
*
*/


#ifndef C_ICPPLUGIN_H
#define C_ICPPLUGIN_H


// INCLUDES
#include <cchservice.h>
#include "imserviceobserver.h"


class CIcpServiceManager;

/**
*  CIcpPlugin class
*  @lib imconnectionprovider.dll
*/
class CIcpPlugin : public CCchService,
				   public MImServiceObserver
    {
    
public:	// Constructors and destructor

    /**
    * Two-phased constructor
    */
    static CIcpPlugin* NewL( TAny* aParam );  

    /**
    * Destructor.
    */
    virtual ~CIcpPlugin();
 
 public : // from base class CCHService
 	
    /**
     * Client enables service. CCHService implementation
     * should use MCCHServiceObserver for service state notifications.
     * @see MCCHServiceObserver
     * @param aServiceSelection Service selection
     */
    void EnableServiceL( const TServiceSelection& aServiceSelection );
                                 
    /**
     * Client enables service. CCHService implementation
     * should use MCCHServiceObserver for service state notifications.
     * @see MCCHServiceObserver
     * @param aServiceSelection Service selection
     * @param aIapId Iap id
     */
    void EnableServiceL( const TServiceSelection& aServiceSelection,
                         TUint aIapId );
    
    /**
     * Client disables service. 
     * @param aServiceId
     */
    void DisableServiceL( const TServiceSelection& aServiceSelection );
    
    /**
     * Client checks is the service available by the IAP id array.
     * CCHService implementation should return ETrue if service is
     * registrable by the (some) IAP, array contains. 
     * @param aServiceSelection Service selection
     * @param aIapIdArray Id array of the currently available iaps.
     * @return ETrue if service is available.
     */
    TBool IsAvailableL( const TServiceSelection& aServiceSelection,
                        const RArray<TUint32>& aIapIdArray ) const;
    
    /**
     * Get service state information.
     * @param aServiceSelection Service selection
     * @param aState Current service state.
     * @return General symbian error code.
     */
    TInt GetServiceState( const TServiceSelection& aServiceSelection, 
                          TCCHSubserviceState& aState ) const;
    
    /**
     * Get service network information.
     * @param aConnectionInfo Connection info
     * @return General symbian error code.
     */
    TInt GetServiceNetworkInfo( TServiceConnectionInfo& aConnectionInfo ) const;
                                        
    /**
     * Set Snap id for service.
     * @param aServiceSelection Service selection
     * @return General symbian error code.
     */
    TInt SetSnapId( const TServiceSelection& aServiceSelection,
                    TUint aSnapId );
                    
    /**
     * Set IAP id for service.
     * @param aServiceSelection Service selection
     * @param aIapId 
     * @return General symbian error code.
     */
    TInt SetIapId( const TServiceSelection& aServiceSelection,
                   TUint aIapId );                               
    
    /**
     * Get service specifics information about service and protocol
     * @param aServiceSelection Service selection
     * @param aBuffer Info will be inserted to this buffer  
     */
    void  GetServiceInfoL( const TServiceSelection& aServiceSelection,
                           RBuf& aBuffer ) const;

    /**
     * Reserves the service for exclusive use
     * @param aServiceSelection Selected service, ID and type
     * @return General symbian error code.
     */
    TInt ReserveService( const TServiceSelection& aServiceSelection );

    /**
     * Frees the exclusive service reservation
     * @param aServiceSelection Selected service, ID and type
     * @return General symbian error code.
     */
    TInt FreeService( const TServiceSelection& aServiceSelection );
    
    /**
     * Is the service reserved
     * @param aServiceSelection Selected service, ID and type
     * @return TBool 
     */
    TBool IsReserved( const TServiceSelection& aServiceSelection ) const;
 
 public :  // from base class MSCPServiceObserver

    /**
     * Service state observer.
     * @since S60 5.0
     * @param aServiceId
     * @param aState a new state of the service.
     */
    void ServiceStateChanged( 
	    const TUint aServiceId,
	    const TCCHSubserviceType aSubService,
	    TCCHSubserviceState aState,
	    TInt aError );
    
    /**
     * Returns the connection parameters
     * @since S60 3.2.3
     * @param aServiceSelection Selected service, ID and type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt GetConnectionParameter( 
                         const TServiceSelection& aServiceSelection,
                         TCchConnectionParameter aParameter,
                         TInt& aValue ) const;
    
    /**
     * Returns the connection parameters
     * @since S60 3.2.3
     * @param aServiceSelection Selected service, ID and type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
     TInt GetConnectionParameter(  
                         const TServiceSelection& aServiceSelection, 
                         TCchConnectionParameter aParameter,
                         RBuf& aValue ) const;
    
    /**
     * Sets the connection parameters
     * @since S60 3.2.3
     * @param aServiceSelection Selected service, ID and type
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt SetConnectionParameter(  
                         const TServiceSelection& aServiceSelection, 
                         TCchConnectionParameter aParameter,
                         TInt aValue );
    
    /**
      * Sets the connection parameters
      * @since S60 3.2.3
      * @param aServiceSelection Selected service, ID and type
      * @param aParameter Connection parameter of the service
      * @param aValue Value of the parameter
      * @return Symbian error code
      */
     TInt SetConnectionParameter(  
                          const TServiceSelection& aServiceSelection, 
                          TCchConnectionParameter aParameter,
                          const TDesC& aValue );
        
       
private:

    /**
     * Constructor is hidden
     */
    CIcpPlugin( MCchServiceObserver& aServiceObserver );

    /**
     * 2nd phase constructor
     */
    void ConstructL();
    
private:

    /**
     * Service manager. Own
     */
    CIcpServiceManager* iServiceManager;
    
    /**
     * not owns : CCH service observer.
     */ 
    MCchServiceObserver& iServiceObserver;
   
	};	

#endif  // C_SCPPLUGIN_H

// End of file

