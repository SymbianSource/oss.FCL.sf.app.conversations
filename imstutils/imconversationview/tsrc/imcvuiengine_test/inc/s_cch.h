/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_cch.h
*
*/

#ifndef MyCchServiceH
#define MyCchServiceH

//  EXTERNAL INCLUDES
#include <cch.h>
#define KMECOIMPLEMENTATIONUID 0x20012423
const TInt KTestServiceId = 255;

_LIT( KTestServiceName, "TEST_SERVICE" );
_LIT(KTestUserName, "testlm123@gmail.com");
_LIT(KOwnUserNameMatch,"gizmo:Sharath"); // dont change this value 
_LIT(KTestPassword, "password");
_LIT(KTestDescription, "error");

TInt CreateSpEntryL( TInt aServiceId );
class MyCchService : public CCchService
	{
    TInt Enable( TCCHSubserviceType aType );
    
    /**
     * Launches service disable. See MCchServiceObserver for status changes.
     *
     * @param aType Type of service
     * @return Symbian error
     */
    TInt Disable( TCCHSubserviceType aType );
    
    /**
     * Returns the current service state
     *
     * @param aType Type of service
     * @param aStatus Status of the service, return value
     * @return Symbian error code
     */
    TInt GetStatus( TCCHSubserviceType aType, 
    						TCchServiceStatus& aStatus ) const ;
    
    /**
     * Returns the connection parameters
     *
     * @param aType Type of service
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt GetConnectionParameter( TCCHSubserviceType aType,
    					TCchConnectionParameter aParameter, TInt& aValue ) const;
    
    /**
     * Returns the connection parameters
     *
     * @param aType Type of service
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt GetConnectionParameter( TCCHSubserviceType aType, 
    					TCchConnectionParameter aParameter, RBuf& aValue ) const;
    
    /**
     * Sets the connection parameters
     *
     * @param aType The type of service
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt SetConnectionParameter( TCCHSubserviceType aType, 
    					TCchConnectionParameter aParameter, TInt aValue );
    
    /**
     * Sets the connection parameters
     *
     * @param aType The type of service
     * @param aParameter Connection parameter of the service
     * @param aValue Value of the parameter
     * @return Symbian error code
     */
    TInt SetConnectionParameter( TCCHSubserviceType aType, 
    					TCchConnectionParameter aParameter, const TDesC& aValue );
    
    /**
     * Reserves the service for exclusive usage
     *
     * @param aType Type of service
     * @return Symbian error code
     */
    TInt Reserve( TCCHSubserviceType aType );
    
    /**
     * Frees the service of exclusive usage
     *
     * @param aType Type of service
     * @return Symbian error code
     */
    TInt Free( TCCHSubserviceType aType );
    
    /**
     * Is the service reserved
     *
     * @param aType Type of service
     * @param aReserved True if the service is reserved
     * @return Symbian error code
     */
    TInt IsReserved( TCCHSubserviceType aType, TBool& aReserved ) const;
    
    /**
     * Returns the service id
     *
     * @return The id of the service
     */
    TInt ServiceId() const;
    
    /**
     * @deprecated Do not use this anymore, change to AddObserver!
     *
     * Adds observer for listening service events
     *
     * @param aObserver Event observing class
     */
    void SetObserver( MCchServiceStatusObserver& aObserver );
    
    /**
     * @deprecated Do not use this anymore, change to 
     * RemoveObserver method with paramater
     * 
     * Removes the observer of service events
     */
    void RemoveObserver( );

    /**
     * Adds observer for listening service events
     *
     * @param aObserver Event observing class
     * @return KErrAlreadyExist Observer already added
     *         KErrNone Observer was added
     */
    TInt AddObserver( MCchServiceStatusObserver& aObserver );
    
    /**
     * Removes the observer of service events
     *
     * @param aObserver Event observing class
     * @return KErrNotFound Observer was not found
     *         KErrNone Observer was removed
     */
    TInt RemoveObserver( MCchServiceStatusObserver& aObserver );
    
    /**
     * Checks if the type is supported
     *
     * @param aType Type of service
     * @param aSupported True if the service type is supported
     * @return Symbian error code
     */
    TInt IsSupported( TCCHSubserviceType aType, TBool& aSupported ) const;
    
    private:    
    	MCchServiceStatusObserver* iObserver;
    	
	};    
	
	
#endif //MyCchService


	
  
