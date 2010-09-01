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
* Description:  This interface describes structure of notify services
*
*/


#ifndef MIMALERTNOTIFIERINTERFACE_H
#define MIMALERTNOTIFIERINTERFACE_H

//  INCLUDES
#include <e32base.h>
#include <coreapplicationuisdomainpskeys.h>
#include <ctsydomainpskeys.h>


// CLASS DECLARATION

/**
*  An interface for listening PS key changes
*
*  @since Series 60 2.6
*/
class MIMAlertNotifierInterface
    {
	public:
		enum TNotifyKey
			{
			EMSGToneQuitKey			
			};
	public:
		
		virtual ~MIMAlertNotifierInterface(){};

    public: // New functions
        
		virtual TInt SetInt( TNotifyKey aKey, TInt aValue ) = 0;
		virtual TInt GetInt( TNotifyKey aKey, TInt& aValue ) = 0;
		virtual TInt ListenKeyChanges( TNotifyKey aKey ) = 0;
    };

#endif      // MIMALERTNOTIFIERINTERFACE_H   
            
// End of File

