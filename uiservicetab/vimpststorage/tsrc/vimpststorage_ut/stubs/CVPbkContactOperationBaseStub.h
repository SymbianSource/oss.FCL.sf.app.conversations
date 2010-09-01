/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/



#ifndef CVPbkContactOperationBaseStub_H
#define CVPbkContactOperationBaseStub_H

// INCLUDES
#include <MVPbkContactOperationBase.h>

// CLASS DECLARATIONS

/**
 * Virtual Phonebook abtract asynchronous contact operation.
 * This interface is used as a handle to asynchronous contact operations.
 * Clients should keep a reference to the handle during the asynchronous 
 * operation to be able to delete and cancel the operation before it
 * completes.
 */
class CVPbkContactOperationBaseStub : public CBase ,  
									public MVPbkContactOperationBase
    {
    public:  // Interface
        /**
         * Destructor. Cancels this operation and releases any 
         * resources held by it.
         */
        ~CVPbkContactOperationBaseStub() 
        {
        	
        }
        
    };

#endif // CVPbkContactOperationBaseStub_H

// End of File
