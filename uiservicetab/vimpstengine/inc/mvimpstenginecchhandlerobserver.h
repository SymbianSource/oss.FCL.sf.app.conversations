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
* Description:  CCH Handler Observer
*
*/


#ifndef     __MVIMPSTENGINECCHHANDLEROBSERVER_H
#define     __MVIMPSTENGINECCHHANDLEROBSERVER_H

// INCLUDES
#include <e32base.h>
#include <cchclientserver.h>

// CLASS DECLARATION

/**
 *  CCH Handler Observer
 *  This class defines CCH observing methods
 *
 *  @lib  vimpstengine.lib
 *  @since S60 5.0
 */
class MVIMPSTEngineCchHandlerObserver
    {
    public:	
    
        /**
         * Informs observers that cch event has occured.
         *
         * @since S60 5.0
         * @param aServiceId, service id related to event
         * @param aState, service state
         * @param aServiceError, service error
         * @return None         
         */    
        virtual void CchEventOccuredL( 
            TUint aServiceId, 
            TCCHSubserviceState aState, 
            TInt aServiceError ) = 0;
    };

#endif // __MVIMPSTENGINECCHHANDLEROBSERVER_H

// End of file
