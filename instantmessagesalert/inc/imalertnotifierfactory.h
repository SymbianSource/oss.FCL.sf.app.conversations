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
* Description:  Publish and Subscribe or Shared data factory method implementation
*
*/

#ifndef IMALERTNOTIFIERFACTORY_H
#define IMALERTNOTIFIERFACTORY_H

//  INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class MIMAlertNotifierInterface;
class MIMAlertNotifierKeyObserver;

// CLASS DECLARATION

/**
*  Class for notifying key changes and changing the values of Pub&Sub- or shared data key changes,
*  depending which is used in the platform.
*
*  @since Series 60 Platform 3.0
*/
// This is not a C-class although it begins with a CA
class IMAlertNotifierFactory // CSI: 51 # see above
    {
    public: // New methods

         /** 
         * Instantiates a notifier object
         * @param aObserver Observer class for the key
		 * @param aUid 
         */
        IMPORT_C static MIMAlertNotifierInterface* CreateNotifierL( 
                MIMAlertNotifierKeyObserver* aObserver );
    };

#endif      // IMALERTNOTIFIERFACTORY_H

// End of File
