
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
* Description:  class implementatio from vpbkstorage handler to store
*
*/


#ifndef MVIMPSTSTORAGEVPBKSTOREHANDLER_H
#define MVIMPSTSTORAGEVPBKSTOREHANDLER_H

#include <e32std.h>
#include <MVPbkContactStoreObserver.h>

class MVPbkContactViewBase;
class MVPbkContactLink;
class MVPbkContactStore;
class MVPbkStoreContact;

/**
 * A class for a contact store event.
 *
 * The contact store event contains the type of the event and
 * possibly a contact link to a changed contact.
 */
enum TVIMPSTVPbkStoreEventType
	{
	EVPbkUnknownEvent = -1,
	EVPbkUnknownChanges = 0,
	EVPbkContactAdded ,
	EVPbkContactDeleted,
	EVPbkContactChanged,
	EVPbkContactRetriving,
	EVPbkContactFetching,
	EVPbkContactReading,
	EVPbkUpdatingAvatar,
	EVPbkContactSynchronizing,
	EVPbkContactReadCompleted
	};	
				
 class TVIMPSTVPbkStoreEvent
	{
	  
   public: // Interface
   	
        /**
         * Constructor
         *
         * @param aEventType The type of the contact store event
         * @param aContactLink The identifier of the changed contact or NULL.
         *                     The link is not owned by this class.
         */
    	 TVIMPSTVPbkStoreEvent( TVIMPSTVPbkStoreEventType aEventType, 
				                MVPbkContactLink* aContactLink = NULL,
				                MVPbkStoreContact* aContact = NULL ,
				                const TDesC& aUserId = KNullDesC ,
				                const TDesC& aName = KNullDesC,
				                const TDesC8& aAvatarContent = KNullDesC8 );
				                
  public:               
		/// Own: Type of the event that has happened
		TVIMPSTVPbkStoreEventType iEventType;
		/// Ref: Link to the contact that is accociated with the event or NULL. 
		///      The link is valid only during HandleStoreEventL function call
		///      and client can not take the ownership of the instance.
		//       Client must clone the link if it needs it later.
		MVPbkContactLink* iContactLink;
		///Own: Spare for future extension
		MVPbkStoreContact* iVPbkContact;
		///Own: Spare for future extension
		TPtrC iUserId;
		///Own: Spare for future extension
		TPtrC iDisplayName;	
		// owns avatar content
		TPtrC8 iAvatarContent;
	};

inline TVIMPSTVPbkStoreEvent::TVIMPSTVPbkStoreEvent(
							TVIMPSTVPbkStoreEventType aEventType, 
			                MVPbkContactLink* aContactLink,
			                MVPbkStoreContact* aContact,
			                const TDesC& aUserId ,
			                const TDesC& aName ,
			                const TDesC8& aAvatarContent ) :
	iEventType(aEventType),
	iContactLink(aContactLink),
	iVPbkContact( aContact ),
	iUserId(aUserId ),
	iDisplayName( aName ),
	iAvatarContent( aAvatarContent )
	{
	}
/**
 *  Interface for contact data container observer
 *
 *  @lib vimpststorage.dll
 *  @since 5.0
 */
class MVIMPSTStorageVPbkStoreHandler
    {
    public: // New functions
		
	    /**
         * From MVPbkContactStoreObserver
         * Callback: Store event occured
         *
         * @since S60 5.0
         * @param aContactStore, contact store
         * @param aStoreEvent, event
         */
        virtual void HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent)  = 0 ; 
        
        
     		/**
		* Virtual destructor
		*/
		virtual ~MVIMPSTStorageVPbkStoreHandler() {};
    };

#endif      // MVIMPSTSTOREDCONTACTSOBSERVER_H

// End of File
