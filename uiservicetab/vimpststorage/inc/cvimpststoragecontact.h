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
* Description:  Contact data container implementation
*
*/



#ifndef CVIMPSTSTORAGECONTACT_H
#define CVIMPSTSTORAGECONTACT_H


//  INCLUDES

#include <e32base.h>
#include <badesca.h>
#include "mvimpststoragecontact.h"

// FORWARD DECLARATIONS
class Cvimpststorage_test; //Unit testClass
class MVIMPSTStorageVPbkContactStore;
// CLASS DECLARATION

/**
 *  Contact data implementation
 * @lib vimpststorage.dll
 * @since 5.0
 */
NONSHARABLE_CLASS (CVIMPSTStorageContact) : public CBase,
   											public MVIMPSTStorageContact
   {
   public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aObserver contact data container observer
         * @param aContactLink pointer to contact link
         * @param aStoreContact pointer to the stored contact
         * @param aContactManager reference to  virtual pbk manager
         * @param aContactManagementObserver - Notified of ContactOperation Add/Deleted
         * @return CVIMPSTStorageContact* a new instance of this class
         */
        static CVIMPSTStorageContact* NewL(const TDesC& aUserId,
        								  const TDesC& aDisplayName,
        								  const MVPbkContactLink& aContactLink,
        								  const TDesC8& aAvatarContent
        								  );
        								  
        /**
         * Two-phased constructor.
         * @param aObserver contact data container observer
         * @param aContactLink pointer to contact link
         * @param aStoreContact pointer to the stored contact
         * @param aContactManager reference to  virtual pbk manager
         * @param aContactManagementObserver - Notified of ContactOperation Add/Deleted
         * @return CVIMPSTStorageContact* a new instance of this class
         */
        static CVIMPSTStorageContact* NewL(const TDesC& aUserId,
        								  const TDesC& aDisplayName );
		
		 /**
         * Two-phased constructor.
         * @param aObserver contact data container observer
         * @param aContactLink pointer to contact link
         * @param aStoreContact pointer to the stored contact
         * @param aContactManager reference to  virtual pbk manager
         * @param aContactManagementObserver - Notified of ContactOperation Add/Deleted
         * @return CVIMPSTStorageContact* a new instance of this class
         */
        static CVIMPSTStorageContact* NewLC(const TDesC& aUserId,
        								  const TDesC& aDisplayName,
        								  const MVPbkContactLink& aContactLink,
        								  const TDesC8& aAvatarContent
        								  );

        /**
         * C++ Destructor.
         */
        ~CVIMPSTStorageContact();
               
        
    public: // Functions from base classes MVIMPSTStorageContact

		/**
         * @see MVIMPSTStorageContact
         */
        void SetUserIdL( const TDesC& aUserId );
        
        
        void SetAvatarContentL( const TDesC8& aAvatarContent ,
                MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler);
        
        /**
         * Set User ID
         * @param aUserId, user id to be set.
         */                
        void SetNameL( const TDesC& aName ) ;
        
        /**
         * @see MVIMPSTStorageContact
         */
	    MVPbkContactLink* ContactLink() const;

	    
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC8& AvatarContent() const;
        
     
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC& UserId() const;
        
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC& Name() const;

       
        /**
         * @see MVIMPSTStorageContact
         */
        TVIMPSTEnums::TOnlineStatus OnlineStatus() const;

        
        /**
         * @see MVIMPSTStorageContact
         */
        void SetOnlineStatus( 
                TVIMPSTEnums::TOnlineStatus aOnlineStatus );
          
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC& StatusText() const;
        
        /**
         * @see MVIMPSTStorageContact
         */
        void SetStatusTextL( const TDesC& aStatusText );  
        
        
        void SetAvatarIndex(TInt aIndex )  ;
		
		
		TInt AvatarIndex() const;      
        
   private:

        /**
         * C++ default constructor.
         * @param aObserver contact data container observer         
         * @param aContactManager reference to  virtual pbk manager
         * @param aStoreContact pointer to the stored contact         
         * @param aContactManagementObserver - Notified of ContactOperation Add/Deleted
         */
        CVIMPSTStorageContact();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL(const TDesC& aUserId,
						  const TDesC& aDisplayName,
						  const MVPbkContactLink& aContactLink,
						  const TDesC8& aAvatarContent);
        
   		 /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL(const TDesC& aUserId,
					    const TDesC& aDisplayName );
    private:    // Data

        // user ID 
        HBufC* iUserId;     // owns
        
         // user ID 
        HBufC* iDisplayName; 

        // presence status (online, offline, etc)
        TVIMPSTEnums::TOnlineStatus iPresenceStatus;
        
        /// Status text. Owns.
        HBufC* iStatusText;
        
	    // Own, virtual phonebook contact link
	    MVPbkContactLink*       iContactLink;    
	  
	    // avatar content Owns        
	    HBufC8* iAvatarContent;
	    
	    TInt iAvatarIndex;
	    
	    //for test cases
	    friend class Cvimpststorage_test;
        
    };

#endif      // CVIMPSTSTORAGECONTACT_H

// End of File
