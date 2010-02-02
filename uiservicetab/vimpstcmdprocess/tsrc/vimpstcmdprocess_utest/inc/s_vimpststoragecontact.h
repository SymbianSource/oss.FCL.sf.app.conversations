/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#ifndef S_VIMPSTSTORAGECONTACT_H
#define S_VIMPSTSTORAGECONTACT_H

#include "mvimpststoragecontact.h"

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

_LIT(KDummy,"dummy@presence");

class vimpststoragecontact_stub : public MVIMPSTStorageContact
	{
	public :
	    ~vimpststoragecontact_stub();
	    
         static MVIMPSTStorageContact* NewL();
         
	    /**
         * @see MVIMPSTStorageContact
         */
        void SetUserIdL( const TDesC& aUserId );
        
        void SetAvatarContentL( const TDesC8& aAvatarContent ,
                        MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler) ;
        //void SetAvatarContentL( const TDesC8& aAvatarContent );
        
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
        //CVIMPSTStorageContact();
       
       /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL(const TDesC& aUserId,
                          const TDesC& aDisplayName,
                          const MVPbkContactLink& aContactLink
                          );
        
         /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL(const TDesC& aUserId,
                        const TDesC& aDisplayName );    
		private:
		RPointerArray<HBufC> iServiceDetailArray ;
		TInt iAvatarIndex;
	};

#endif      // S_VIMPSTSTORECONTACT_H

// End of File
