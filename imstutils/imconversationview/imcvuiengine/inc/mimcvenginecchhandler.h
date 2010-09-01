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
* Description:  M-Class handling the use of Cch
*
*/


#ifndef     M_CIMCVENGINECCHHANDLER_H
#define     M_CIMCVENGINECCHHANDLER_H

// INCLUDES
#include <e32base.h>
#include <cch.h>


// CLASS DECLARATION
/**
 *  CCH handler
 *  This class implements the handling of CCH
 *
 *  @lib imcvengine.lib
 *  @since S60 5.0
 */ 

 
NONSHARABLE_CLASS( MIMCVEngineCchHandler ) 
    {
    public:

        
        /**
         * Standard C++ destructor
         */    
        virtual ~MIMCVEngineCchHandler()
        {
        	
        }
        
        /**
         * Gets service state
         *
         * @since S60 5.0         
         * @param aCCHState service state (result)
         * @return TInt Standard Symbian error code
         */         
         virtual TInt GetServiceState( 
									TCCHSubserviceState& aCCHState
									) = 0;

        /**
         * Enables service
         *
         * @since S60 5.0         
         * @return TInt, error code
         */
        virtual TInt EnableService() = 0;
            
        
        /**
         * Returns the User Id for this service
         * Returns HbufC* and the caller of this function should deallocate the same
         * @since S60 5.0         
         * @return HBufC*
         */
        virtual HBufC* GetUserIdL() = 0 ; 
        
        /**
         * Returns the XimpFw Protocol UID for this service
         * Returns TInt 
         * @since S60 5.0         
         * @return TInt , error code
         */
        virtual TInt GetAdapterUidL()= 0 ; 
        /**
         * Disables service
         *
         * @since S60 5.0         
         * @return TInt, error code
         */
        virtual TInt DisableService() = 0;  
        
       
       	/**
         * IsServiceLoggedIn
         *
         * @since S60 5.0         
         * 
         * @return ETrue - if Service Logged In, EFalse - Service Not Logged In
         */         
         virtual TBool IsServiceLoggedIn() = 0;
              
    };

#endif // M_CIMCVENGINECCHHANDLER_H

// End of file
