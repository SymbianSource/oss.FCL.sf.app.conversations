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
* Description:  Interface for observer
*
*/


#ifndef MVIMPSTPROCESSARRAYOBSERVER_H
#define MVIMPSTPROCESSARRAYOBSERVER_H

#include <e32std.h>
#include "tvimpstenums.h"

// CLASS DECLARATION

/**
 *  Interface for observer
 *
 *  @lib vimpstcmdprocess.dll
 *  @since 5.0
 */
  
class MVIMPSTProcessArrayObserver
    {
    public: // New functions
	   
	    
        /**
         * Called when a contact is added.
         * @param type. type of the item deleted(contact/contactlist).
         * @param aIndex The index which was added
         */
        virtual void HandleAdditionL(TVIMPSTEnums::TItem type, 
        							TInt aIndex) = 0;
        /**
         * Called when a contact is deleted.
         * @param type. type of the item deleted(contact/contactlist).
         * @param aIndex The aIndex which was deleted.
         */        							
        virtual void HandleDeletionL(TVIMPSTEnums::TItem type, TInt aIndex) = 0;
        virtual void HandleAvatarChangeL( const TDesC& aUserId ) = 0;

        /**
         * Virtual destructor
         */
        virtual ~MVIMPSTProcessArrayObserver() {};
    };


#endif // MVIMPSTPROCESSARRAYOBSERVER_H

// End of File
