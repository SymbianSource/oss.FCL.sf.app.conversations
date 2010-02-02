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
* Description:  class for cch service handling
 *                
 *
*/


#ifndef CSERVICEWIDGETCCHHANDLER_H
#define CSERVICEWIDGETCCHHANDLER_H

#include <cch.h>
#include <cchclient.h>
#include <cchtypes.h>
#include <cchuiobserver.h>
#include "servicewidgetcpglobals.h"
#include "servicewidgetenums.hrh"

// farword declarations
class MServiceWidgetServiceStateObserver;
/**
 * @class cch handler class implementation
 * @since 5.0
 */
struct TSubServiceState
	{
	TCCHSubserviceType iSubServiceType; /* subservice type(IM/VOIP/PRESENCE */
	TSWSTRegistrationState iSubServiceState; /* observer */
	}; 

class CServiceWidgetCchHandler  : public CBase,
                             	  public MCchServiceStatusObserver
	{

	public:

		/**
		* Two-phased constructor.
		* @param aServiceId, service id
		* @param aContentPrioritizer, service state observer
		* @return the instance of this class
		*/
		static CServiceWidgetCchHandler* NewL( TUint32 aServiceId,
												MServiceWidgetServiceStateObserver& aContentPrioritizer );

		/**
		* Two-phased constructor.
		* @param aServiceId, service id
		* @param aContentPrioritizer, service state observer
		* @return the instance of this class
		*/
		static CServiceWidgetCchHandler* NewLC( TUint32 aServiceId,
												MServiceWidgetServiceStateObserver& aContentPrioritizer );

		/**
		* Standard C++ destructor
		*/    
		~CServiceWidgetCchHandler();

	public: // new methods

		/**
		* @return own user id
		*/ 
		const TDesC& OwnUserIdL();

		/**
		* @return own user id
		*/ 
		TInt XimpAdaptationUidL();

		/**
		* @param aSubServiceType, a sub service type
		* @return ETrue if service supported 
		*/
		TBool IsSubServiceSupported(TCCHSubserviceType aSubServiceType );

	
		/**
		* read the supported sub service of selected service 
		*/
		void ReadSupportedSubServices(); 
		
		
		/**
		* mapped the cch state to local state 
		* @param aState ,cch state
		* @return mapped state
		*/ 
		TSWSTRegistrationState ParseGetServiceState();

		/**
		 *  @return tabview id if service installed else NULL
		 */ 
		TInt GetServiceViewIdL( TInt& aTabUid );
		
		/**
		 *  @returns true if service logged in successfully else KErrNotFound
		 */ 
		TInt EnableServiceL( );

	public: // method from MCchServiceObserver

		/**
		* Signaled when service status or error changes
		*
		* @param aServiceId Id of the service
		* @param aType Service type
		* @param aServiceStatus Service status
		*/
		void ServiceStatusChanged(TInt aServiceId,
								const TCCHSubserviceType aType,
								const TCchServiceStatus& aServiceStatus );
	
    private: // Implementation

		/**
		* Standard C++ constructor
		* @param aServiceId, service id
		* @param aContentPrioritizer, service state observer
	    */
		CServiceWidgetCchHandler(TUint32 aServiceId,
								 MServiceWidgetServiceStateObserver& aContentPrioritizer );

		/**
		* Performs the 2nd phase of construction.
		*/        
		void ConstructL();

		/**
		* read the own status from the xmmp
		* @return the own status
		*/ 
		HBufC* GetConParametersL( TCchConnectionParameter aConnParam ); 
		
		/**
		* mapped the cch state to local state 
		* @param aState ,cch state
		* @return mapped state
		*/ 
		TSWSTRegistrationState GetLocalServiceState(TCCHSubserviceState aState );
		
		/**
		* @return service state
		*/
		TSWSTRegistrationState GetServiceState( TCCHSubserviceType aServiceType );
		
		/**
		* check from cch service  
		* @param aSubServiceType ,a sub service type
		* @return ETrue if supported
		*/ 
		TBool IsServiceSupported(TCCHSubserviceType aSubServiceType );

	private: // Data
		//owns : serviceid       
		TInt iServiceId;
		// not owns : reference to state observer
		MServiceWidgetServiceStateObserver& iStateObserver; 
		//owns : reference to cch 
		CCch*   iCchClient;     
		//owns : own user id
		HBufC* iOwnData;
		// owns : service name 
		HBufC* iServiceName;
		// owns : xmpp parameters
		TSWSTRegistrationState iServiceState;
		 // does not own array of observers.
        RArray<TSubServiceState> iSupportedSubServices;
	};

#endif /*CSERVICEWIDGETCCHHANDLER_H*/
