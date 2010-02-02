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
* Description:  contact store class implementaion to virtual phonebook store type
*
*/


#ifndef MVIMPSTSTORAGECONTACTSTORE_H_
#define MVIMPSTSTORAGECONTACTSTORE_H_

class MVPbkContactLink;
class MVPbkStoreContact;
class MVIMPSTStorageContact;

/*
* structure to hold the fetch contact information
*/
struct TVIMPSTContactInfo
	{
	///Own: user Id
	HBufC* iUserId;
	///Own: display name
	HBufC* iDisplayName;	
	};
	
class MVIMPSTStorageVPbkContactStore
                                  
      {
    public:
      
      enum TVIMPSTFetchingSteps
			{
			EFetchingUnknown = -1,
			EContactReadComplete,
			EFetchingCalled,
			EFetchingOn,
			EFetchStart
			};  
        /**
	     * 
	     * return ETrue if local store
	     **/
		virtual TBool  LocalStore() const = 0 ;
		
		virtual TInt CreateVPbkContactL( const TDesC& aUserId, 
        							    const TDesC& aDisplayName,
        							     TBool aInvitationAutoAccept = EFalse)=0 ;
		
		virtual TInt CreateRetriveVPbkContactL( TInt aIndezToUse ) = 0;
		
		virtual TInt deleteRetriveVPbkContactL( TInt aIndezToUse ) = 0;
		
		virtual TInt RemoveVPbkContactL(const MVPbkContactLink& aContactLink) = 0 ;
		
		virtual TInt RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks )  = 0 ;
		
		virtual TInt UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
								    const TDesC8& aAvatartData ) = 0 ;
		
		virtual TInt RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact) = 0 ;
		
		virtual void AddVPbkFetchContactsL( RArray <TPtrC> &aFirstNameList, 
		                            		RArray <TPtrC> &aServiceField ) = 0 ;
		
		virtual TInt RemoveAllVPbkContactsL() = 0 ;	
		
		virtual const TDesC& GetRetrieveVPbkXSPIdL(TInt aIndex )  = 0 ;
			
		virtual void DeleteDatabaseL() = 0 ;	
		/*
		* virtual destructor
		*/
	   	virtual ~MVIMPSTStorageVPbkContactStore()
		   	{
		   		
		   	}
      };

#endif /*MVIMPSTSTORAGECONTACTSTORE_H_*/
