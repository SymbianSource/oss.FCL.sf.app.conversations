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



#ifndef MVIMPSTSTORAGECONTACTSOBSERVER_H
#define MVIMPSTSTORAGECONTACTSOBSERVER_H

#include <e32std.h>
#include "tvimpstenums.h"

class MVIMPSTStorageContact;
class MVIMPSTStorageContactList;

// CLASS DECLARATION

/**
 *  Interface for contact data container observer
 *
 *  @lib vimpststorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageContactsObserver
    {
    public: // New functions

         /**
         * Called when a contact or list is about to change, or has been changed.
         * @param aList List that is changed. Can be NULL if contact is changed.
         * @param aContact Contact that is changed. Can be NULL if list is changed
         * @param aEventType The type of event: EStorageEventPreChange before
         * the change (contact still has the old information), or
         * EStorageEventPostChange after the change took place 
         * (contact has new information)
         * @param aUserIdChanged, ETrue if userid has been changed else EFalse.
         */
        virtual void HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType, 
		        						   MVIMPSTStorageContactList* aList, 
		        						   MVIMPSTStorageContact* aContact ,
		        						   TInt aContactIndex ) = 0;

        /**
         * Virtual destructor
         */
        virtual ~MVIMPSTStorageContactsObserver() {};
    };

#endif      // MVIMPSTSTORAGECONTACTSOBSERVER_H

// End of File
