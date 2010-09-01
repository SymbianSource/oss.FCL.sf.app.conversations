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

#include "tvimpstenums.h"
//  INCLUDES
#include "mvimpststoragecontact.h"

// FORWARD DECLARATIONS
class MVPbkContactLink;
class MVIMPSTStorageContact;

// CLASS DECLARATION

/**
 *  Contact data implementation
 * @lib vimpststorage.dll
 * @since 5.0
 */
class MyVIMPSTStorageContactStub : public MVIMPSTStorageContact
    {
    public:  // Constructors and destructor

        MyVIMPSTStorageContactStub()
	       {
	       	
	       }
        /**
         * C++ Destructor.
         */
        ~MyVIMPSTStorageContactStub()
	        {
	        	
	        }
               
        
    public: // Functions from base classes MVIMPSTStorageContact

		/**
         * @see MVIMPSTStorageContact
         */
        void SetUserIdL( const TDesC& aUserId );
        
        /**
         * @see MVIMPSTStorageContact
         */
	    MVPbkContactLink* ContactLink() const;
	    
	    /**
         * @see MVIMPSTStorageContact
         */
	    RPointerArray<HBufC>& ServiceDetails();

    
    	/**
         * @see MVIMPSTStorageContact
         */              
	   // void SetSeriveFields( const CDesCArray& aArrayOfServiceNumbers );
        
		
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC& UserId() const;

       
        /**
         * @see MVIMPSTStorageContact
         */
        TVIMPSTEnums::TOnlineStatus OnlineStatus() const;

        /**
         * @see MVIMPSTStorageContact
         */
        void SetOnlineStatus( TVIMPSTEnums::TOnlineStatus aOnlineStatus );
          
        /**
         * @see MVIMPSTStorageContact
         */
        void SignalChanges();
        
               
        /**
         * @see MVIMPSTStorageContact
         */
        const TDesC& StatusText() const;
        
        /**
         * @see MVIMPSTStorageContact
         */
        void SetStatusTextL( const TDesC& aStatusText );   

        
      
  private :
  
  			TVIMPSTEnums::TOnlineStatus iPresenceStatus;
  			
  		 };

#endif      // CVIMPSTSTORAGECONTACT_H

// End of File
