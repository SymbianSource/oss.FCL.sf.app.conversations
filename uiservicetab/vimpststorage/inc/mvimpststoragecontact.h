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
* Description:  Stored contact data interface (getters/setters)
*
*/



#ifndef MVIMPSTSTORAGECONTACT_H
#define MVIMPSTSTORAGECONTACT_H

#include <e32base.h>
#include "tvimpstenums.h"
// CLASS DECLARATION
class MVPbkContactLink;
class MVIMPSTStorageVPbkContactStore;

/**
 *  Getter/Setter interface for contacts.
 *
 *  @lib vimpststorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageContact
    {
    public: // Definitions
    
        /**
         * An Enum
         * to define which sorting algorithm need to be used to sort contacts.
         */

        enum TSortAlgorithm
            {
            ECompareAlphabetically, /* sort alphabetically */
            ECompareByPresence     /* sort based on presence */
            };
        
    public: // Interface
        /**
         * Avatar data
         * this returns the avatar data of available else Knulldec8
         * 
         * @return TDesC8& avatar content of this contact or knulldec8
         */
        virtual const TDesC8& AvatarContent() const = 0;
        
         /**
         * User id
         * @return TDesC& User ID of this contact
         */
        virtual const TDesC& UserId() const = 0;
        
          /**
         * User id
         * @return TDesC& User ID of this contact
         */
        virtual const TDesC& Name() const = 0;
        
        /**
         * Online status
         * @return TOnlineStatus On-line status of this contact
         */
        virtual TVIMPSTEnums::TOnlineStatus OnlineStatus() const = 0;

        /**
         * Set online status
         * @param aOnlineStatus, online status to be set.
         */
        virtual void SetOnlineStatus( TVIMPSTEnums::TOnlineStatus aOnlineStatus ) = 0;
        
       	/**
         * Set User ID
         * @param aUserId, user id to be set.
         */                
        virtual void SetUserIdL( const TDesC& aUserId ) = 0;
        
        
        virtual void SetAvatarContentL( const TDesC8& aAvatarContent ,
                MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler) = 0 ;
        
        /**
         * Set User ID
         * @param aUserId, user id to be set.
         */                
        virtual void SetNameL( const TDesC& aName ) = 0;
        
        /**
         * Get the status text for the contact
	 	 * @return TDesC& reference to the status text.    
	     */
        virtual const TDesC& StatusText() const = 0;
        
        /**
         * Set Status Text
         * @param aStatusText, status text to be set.
         */
        virtual void SetStatusTextL( const TDesC& aStatusText ) = 0;		   
		
		/**
	     * Returns virtual phonebook contact link of this contact
	     *
	     * @since S60 5.0
	     * @return MVPbkContactLink*, virtual phonebook contact link. Can be NULL
	     */
		virtual MVPbkContactLink* ContactLink() const = 0;
		
		
		virtual void SetAvatarIndex(TInt aIndex ) = 0 ;
		
		
		virtual TInt AvatarIndex() const  = 0;
		
	    /**
         * Destruction.
         */
        virtual ~MVIMPSTStorageContact(){};
    };
    
#endif      // MVIMPSTSTORAGECONTACT_H

// End of File
