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

#ifndef S_VIMPSTSTORECONTACT_H
#define S_VIMPSTSTORECONTACT_H

#include "mvimpststoragecontact.h"

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

_LIT(KDummy,"dummy");
class MVPbkStoreContact;
class MVPbkContactObserver;
class MVIMPSTContactOperationCompleteObserver;
class MVIMPSTStorageVPbkContactStore;

class vimpststorecontact_stub : public MVIMPSTStorageContact
	{
	public :
	    
	    void SetNameL( const TDesC& aName );
	    void SetAvatarContentL( const TDesC8& aAvatarContent ,
	                  MVIMPSTStorageVPbkContactStore& aVPPkStoreHandler);
	    
         static MVIMPSTStorageContact* NewL();
         
         const TDesC8& AvatarContent() const ;
        
         TInt AvatarIndex() const ;
         
         void SetAvatarIndex( TInt avatarIndex);

         const TDesC& UserId() const ;
        
         TVIMPSTEnums::TOnlineStatus OnlineStatus() const ;

         void SetOnlineStatus( TVIMPSTEnums::TOnlineStatus aOnlineStatus ) ;
        
         void SignalChanges() ;
         
         const TDesC& Name() const;

         void SignalAvatarChange() ;

         void SetUserIdL( const TDesC& aUserId ) ;
        
         const TDesC& StatusText() const ;
        
         void SetStatusTextL( const TDesC& aStatusText ) ;		   
		
		 MVPbkContactLink* ContactLink() const;
		
	     MVPbkStoreContact* VPbkStoreContact() const ;
		
	     RPointerArray<HBufC>& ServiceDetails() ;
	     /**
	      * Get the impp field for the contact
	      * @return TDesC& reference to the impp field.    
	      */
	     const TDesC&  ImppField();

	     void Commit(MVPbkContactObserver* aObserver = NULL,
	    					MVIMPSTContactOperationCompleteObserver* aContactOperationObserver = NULL) ;
	    
	     void ResetAvatarContent() ;  
	     
	     void SetContactOperationObserver( MVIMPSTContactOperationCompleteObserver* 
	    								aContactOperationObserver );
	    								
	    ~vimpststorecontact_stub();
	    
	    HBufC* GetContactFieldDataLC( const TDesC& aDefaultDomain /*= KNullDesC */,
	                                                        TInt aFieldResId /*= R_VPBK_FIELD_TYPE_IMPP*/);
	        
		private:
		RPointerArray<HBufC> iServiceDetailArray ;
	};

#endif      // S_VIMPSTSTORECONTACT_H

// End of File
