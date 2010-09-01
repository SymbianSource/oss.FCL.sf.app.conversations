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
* Description:  Contact data container observer interface
*
*/



#ifndef _MVIMPSTENGINESERVICESTATEEVENTOBSERVER_H
#define _MVIMPSTENGINESERVICESTATEEVENTOBSERVER_H

#include "tvimpstenums.h"


// CLASS DECLARATION

/**
 *  Interface for observing connection events.
 *
 *  @lib vimpstengine.dll
 *  @since 5.0
 */
	
class MVIMPSTEngineServiceStateEventObserver
{
	
	public:
	    
	    /**
	    * Observer Event for Service State changes
	    * @param aState - Indicates the service state
	    * @param aServiceError, error if any.
	    */ 
	    virtual void HandleServiceEventL
	    			( TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError) = 0;
	    
	    
};

#endif //_MVIMPSTENGINESERVICESTATEEVENTOBSERVER_H