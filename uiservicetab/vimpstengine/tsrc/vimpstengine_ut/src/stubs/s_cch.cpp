/*
* s_cch.cpp : Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_cch.cpp
*
*/
//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <cch.h>

#include "s_cch.h"

MyCchService* myService = NULL;
TBool createService = EFalse;
TBool tryAllSubserviceAvailable = EFalse;
TBool tryVoipSubServiceAvailable = EFalse;
TBool tryPresenceSubServiceAvailable = EFalse;
TBool tryIMSubServiceAvailable = EFalse;
TBool tryConnParamLeaveWithNotSupported = EFalse;
TBool tryConnParamWithSuccess = EFalse;
TBool sendObserverEvent = EFalse;
TBool getUserName = EFalse;
TBool getPassword = EFalse;
TBool tryParamTomatchOwnId =EFalse;
TBool tryConnParamWithFailure = EFalse;



/*************** Stubs ****************/
CCchService* CCch::GetService( TInt /*aServiceId*/ )
	{
	if( createService && !myService )
		{
		myService = new ( ELeave ) MyCchService();
		return myService;
		}
		
	return myService;
	}


/************* End of Stubs ************/



TInt MyCchService::Enable( TCCHSubserviceType aType )
	{ 

	if (sendObserverEvent)
		{
		if (iObserver)
			{
			TCchServiceStatus aStatus;
			
			aStatus.SetState(ECCHEnabled);	
			aStatus.SetError(KErrNone);	
			
			if (tryPresenceSubServiceAvailable)
				{		
				iObserver->ServiceStatusChanged( 10, ECCHPresenceSub, aStatus );		
				}
			else
				{
				iObserver->ServiceStatusChanged( 10, aType, aStatus );			
				}				
			}
		}
	
	return KErrNone; 
	}
    
/**
 * Launches service disable. See MCchServiceObserver for status changes.
 *
 * @param aType Type of service
 * @return Symbian error
 */
 TInt MyCchService::Disable( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Returns the current service state
 *
 * @param aType Type of service
 * @param aStatus Status of the service, return value
 * @return Symbian error code
 */
 TInt MyCchService::GetStatus( TCCHSubserviceType aType, 
						TCchServiceStatus& aStatus ) const 
						
	{ 

	if ( (ECCHVoIPSub == aType) && (tryVoipSubServiceAvailable || tryAllSubserviceAvailable) )
		{		
		aStatus.SetState(ECCHUninitialized);	
		aStatus.SetError(KErrNone);	
		return KErrNone; 		
		}
	
	else if ( (ECCHPresenceSub == aType) && (tryPresenceSubServiceAvailable || tryAllSubserviceAvailable) )
		{		
		aStatus.SetState(ECCHUninitialized);	
		aStatus.SetError(KErrNone);			
		return KErrNone; 
		}
		
	else if ( (ECCHIMSub == aType) && (tryIMSubServiceAvailable || tryAllSubserviceAvailable) )
		{		
		aStatus.SetState(ECCHUninitialized);	
		aStatus.SetError(KErrNone);			
		return KErrNone; 
		}	
					
	return KErrNotFound	; 

	}

/**
 * Returns the connection parameters
 *
 * @param aType Type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
 TInt MyCchService::GetConnectionParameter( TCCHSubserviceType /*aType*/,
					TCchConnectionParameter /*aParameter*/, TInt& /*aValue*/ ) const{ return KErrNone;}

/**
 * Returns the connection parameters
 *
 * @param aType Type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
 TInt MyCchService::GetConnectionParameter( TCCHSubserviceType /*aType*/, 
					TCchConnectionParameter /*aParameter*/, RBuf& aValue ) const
					
	{
	if (tryConnParamLeaveWithNotSupported)
		{
		return KErrNotSupported;
		}

	if (tryConnParamWithSuccess)
		{
		aValue.Copy(KTestUserName);		
		}
	if (tryParamTomatchOwnId)
	    {
	    aValue.Copy(KOwnUserNameMatch);
	    }
	if ( getUserName )
	    {
	    aValue.Copy(KTestUserName);    
	    }
	if ( getPassword )
	    {
	    aValue.Copy(KTestPassword);    
	    }
	if(tryConnParamWithFailure)
	    {
	    return KErrNotFound;
	    }

	return KErrNone;
	}

/**
 * Sets the connection parameters
 *
 * @param aType The type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
 TInt MyCchService::SetConnectionParameter( TCCHSubserviceType /*aType*/, 
					TCchConnectionParameter /*aParameter*/, TInt /*aValue*/ )
					
	{
	return KErrNone;
	}

/**
 * Sets the connection parameters
 *
 * @param aType The type of service
 * @param aParameter Connection parameter of the service
 * @param aValue Value of the parameter
 * @return Symbian error code
 */
 TInt MyCchService::SetConnectionParameter( TCCHSubserviceType /*aType*/, 
					TCchConnectionParameter /*aParameter*/, const TDesC& /*aValue*/ )
	{
	if (tryConnParamLeaveWithNotSupported)
		{
		return KErrNotSupported;
		}

	return KErrNone;	
	}

/**
 * Reserves the service for exclusive usage
 *
 * @param aType Type of service
 * @return Symbian error code
 */
 TInt MyCchService::Reserve( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Frees the service of exclusive usage
 *
 * @param aType Type of service
 * @return Symbian error code
 */
 TInt MyCchService::Free( TCCHSubserviceType /*aType*/ ){ return KErrNone; }

/**
 * Is the service reserved
 *
 * @param aType Type of service
 * @param aReserved True if the service is reserved
 * @return Symbian error code
 */
 TInt MyCchService::IsReserved( TCCHSubserviceType /*aType*/, TBool& /*aReserved*/ ) const{ return KErrNone; }

/**
 * Returns the service id
 *
 * @return The id of the service
 */
 TInt MyCchService::ServiceId() const{ return KErrNone; }

/**
 * @deprecated Do not use this anymore, change to AddObserver!
 *
 * Adds observer for listening service events
 *
 * @param aObserver Event observing class
 */
 void MyCchService::SetObserver( MCchServiceStatusObserver& aObserver )
	 {
	 iObserver = &aObserver;
	 }

/**
 * @deprecated Do not use this anymore, change to 
 * RemoveObserver method with paramater
 * 
 * Removes the observer of service events
 */
 void MyCchService::RemoveObserver( )
 	{
	 iObserver = NULL;
	 }

 
/**
 * Adds observer for listening service events
 *
 * @param aObserver Event observing class
 * @return KErrAlreadyExist Observer already added
 *         KErrNone Observer was added
 */
 TInt MyCchService::AddObserver( MCchServiceStatusObserver& /*aObserver*/ ){ return KErrNone; }

/**
 * Removes the observer of service events
 *
 * @param aObserver Event observing class
 * @return KErrNotFound Observer was not found
 *         KErrNone Observer was removed
 */
 TInt MyCchService::RemoveObserver( MCchServiceStatusObserver& /*aObserver*/ ){ return KErrNone; }

/**
 * Checks if the type is supported
 *
 * @param aType Type of service
 * @param aSupported True if the service type is supported
 * @return Symbian error code
 */
 TInt MyCchService::IsSupported( TCCHSubserviceType /*aType*/, TBool& /*aSupported*/ ) const{ return KErrNone; }
 
 
 //end of file
 
 
 
