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
* Description:  imservice observer class
*
*/


#ifndef M_IMSERVICEOBSERVER_H
#define M_IMSERVICEOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CONSTANTS
// None

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// CLASS DECLARATION
    
/**
 *  MImServiceObserver declaration.
 *  @since S60 5.0
 */
class MImServiceObserver
    {

public: // New functions

    /**
     * Service state observer.
     * @since S60 5.0
     * @param aServiceId
     * @param aSubService
     * @param aState a new state of the service.
     */
    virtual void ServiceStateChanged( const TUint aServiceId,
                               const TCCHSubserviceType aSubService,
                               TCCHSubserviceState aState,
                               TInt aError ) = 0;
    

  
    };

#endif // M_IMSERVICEOBSERVER_H

// End of file
