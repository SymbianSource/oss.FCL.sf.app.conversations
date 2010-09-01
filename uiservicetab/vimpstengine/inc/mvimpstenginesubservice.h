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
* Description:  M-Class that capsulates sub service API's
*
*/


#ifndef MVIMPSTENGINESUBSERVICE_H
#define MVIMPSTENGINESUBSERVICE_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"


//FORWARD DECLARATION

//CLASS DECLARATION
/**
 *  
 *  This class list the M-Class for subservices
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
//Class declaration
class MVIMPSTEngineSubService
	{
		
	public:		
	   
	    /**
	     * SubServiceState
	     * This API is used to get the service state of the SubServices
	     * @return TVIMPSTRegistrationState Service State of this service
	     */
		virtual TVIMPSTEnums::TVIMPSTRegistrationState SubServiceState() const = 0;		
        
        
        /**
         * Type
         * This API is used to get the Type of the SubServices
         * @return the type of subservice - TVIMPSTEnums::SubServiceType
         */
        virtual TVIMPSTEnums::SubServiceType Type() const = 0;	
        
                
	   /**
		* Destructor
		*/		
		virtual ~MVIMPSTEngineSubService() 
		{}
		
	};

#endif //MVIMPSTENGINESUBSERVICE_H


