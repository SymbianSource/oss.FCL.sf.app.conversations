/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  active object class for interacting with conversation view
*
*/

#ifndef CIMCVAPPLOGINEVENTLISTENER_H_
#define CIMCVAPPLOGINEVENTLISTENER_H_

#include <e32base.h>
#include <e32property.h>
#include <AknWaitDialog.h>
#include <e32std.h>
#include "cimcvkeys.h"

class CVIMPSTEngineServiceTableFetcher;

_LIT(KDelimiter, ":");
/**
 * constant to define the size of the contact link
 */
#define KMaxPackedContactLinkLength 300
/**
 * constant to define the size of the contact link
 */
#define KMaxServiceIdLength 3
#define KMaxSerIdXspIdLen 512

class CVIMPSTEngineCVListener : public CActive
    {
public :
        // Constructor
        /**
         * Creates a new CVIMPSTEngineCVListener.
         * @param aServiceTableFetcher, service table handler
         * @return New instance of this class         
         */
        static CVIMPSTEngineCVListener* NewL(
                CVIMPSTEngineServiceTableFetcher& aServiceTableFetcher );
        
          /**
           * Standard C++ destructor.
           */
         ~CVIMPSTEngineCVListener();
      
private : // From CActive

        /**
        *  RunL
        */
        void RunL();  
        
        /**
        *  DoCancel
        */
        void DoCancel();
        
        /**
         * RunError.
         */
        TInt RunError( TInt  aError);

private: // Implementation
       /**
        * Standard C++ constructor
        * @param aServiceTableFetcher, reference to service table handler
        */
        CVIMPSTEngineCVListener(
                CVIMPSTEngineServiceTableFetcher& aServiceTableFetcher );

       /**
        * Performs the 2nd phase of construction.
        */
       void ConstructL();
       
private :
    
    // owns , property to use
    RProperty iProperty;    
    
    // Not own, handle to service table
    CVIMPSTEngineServiceTableFetcher& iServiceTableFetcher;
    
    };

#endif /*CIMCVAPPLOGINEVENTLISTENER_H_*/
