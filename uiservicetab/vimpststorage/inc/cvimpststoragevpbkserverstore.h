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
* Description:  Class that provides handling of vpbk server store
*
*/


#ifndef C_CVIMPSTSTORAGESERVERSTORE_H
#define C_CVIMPSTSTORAGESERVERSTORE_H

#include <e32base.h>
#include "mvimpststoragevpbkcontactstore.h"
#include "mvimpststoragevpbkstorehandler.h"

class CVIMPSTStorageVPbkStoreHandler;
class MVIMPSTStorageServiceCacheWriter;
class MVPbkContactLink;
class MVPbkStoreContact;
class MVIMPSTStorageContact;

// CLASS DECLARATION
/**
 *  vpbk local store 
 *  This class implements the use of Virtual Phonebook for Local Store(Contacts.cdb)
 *
 *  @lib vimpststorage.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTStorageVPbkServerStore ) : public CActive,                     
							                     	 public MVIMPSTStorageVPbkContactStore,
							                     	 public MVIMPSTStorageVPbkStoreHandler
    {
        
    public:

		
        /**
         * Two-phased constructor.
         *
         * @param aContactDb, contact database
         * @param aListId, Default List ID 
         * @param aServiceId - Service Id
         * @param aServiceName - Service Name
         * @param aStorageView, Ref to Storage (CVIMPSTStorageViewId)
         */
        static CVIMPSTStorageVPbkServerStore* NewL(const TDesC& aContactDb,
        										   const TDesC& aServiceName, 
        										  MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter );

        /**
         * Two-phased constructor.
         *
         * @param aContactDb, contact database
         * @param aListId, Default List ID 
         * @param aServiceId - Service Id
         * @param aServiceName - Service Name
         * @param aStorageView, Ref to Storage (CVIMPSTStorageViewId)
         */
        static CVIMPSTStorageVPbkServerStore* NewLC( const TDesC& aContactDb, 
        											 const TDesC& aServiceName,
        											MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTStorageVPbkServerStore();

   public: // from MVIMPSTStorageVPbkContactStore
	
		/**
	     * See @MVIMPSTStorageVPbkContactStore
	     * @ return ETrue
	     **/
		TBool  LocalStore() const ;
		
		TInt CreateVPbkContactL(const TDesC& aUserId, 
        					   const TDesC& aDisplayName,
        					   TBool aInvitationAutoAccept = EFalse )  ;
		
		TInt RemoveVPbkContactL(const MVPbkContactLink& aContactLink) ;
		
		TInt RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks )  ;
		
		TInt CreateRetriveVPbkContactL(TInt aIndexToUse ) ;
		
		TInt deleteRetriveVPbkContactL(TInt aIndexToUse ) ;
		
		TInt UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
								    const TDesC8& aAvatartData );
		
		TInt RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact);
		
			
		void AddVPbkFetchContactsL( RArray <TPtrC> &aFirstNameList, 
		                              RArray <TPtrC> &aServiceField )  ;
		                              
		const TDesC& GetRetrieveVPbkXSPIdL(TInt aIndex )  ;
	
		TInt RemoveAllVPbkContactsL();	
		
		
		void DeleteDatabaseL();
   public :
        /**
         * From MVPbkContactStoreObserver
         * Callback: Store event occured
         *
         * @since S60 5.0
         * @param aContactStore, contact store
         * @param aStoreEvent, event
         */
        void HandleVPbkStoreEventL(TVIMPSTVPbkStoreEvent aVPbkStoreEvent) ;		
   
   private: // From CActive
           void RunL();
           void DoCancel();
           TInt RunError(TInt aError );
                   
     private: 
              
           /**
            * Issues Request
            *                  
            */
           void IssueRequest();    

    private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CVIMPSTStorageVPbkServerStore(MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter);
        
        /**
         * Performs the 2nd phase of construction.
         *
         * @param aContactDb, contact database address
         * @param aServiceName, service name         
         */        
        void ConstructL(const TDesC& aContactDb,const TDesC& aServiceName );
        
        /*
        * helper method for deleing the etry from iFetchContactsToBeAdded
        */
        void ResetAndDestroyLocalArrayL();
        
 
    private: // Data
        MVIMPSTStorageServiceCacheWriter& iServiceCacheWriter;
        // Own: pointer to a class which interact with Virtual phonebook   
        CVIMPSTStorageVPbkStoreHandler* iVPbkStoreHandler;
        // owns : fetched contacts
        RArray<TVIMPSTContactInfo> iFetchContactsToBeAdded;
        // owns 
        TVIMPSTFetchingSteps iFetchStep;
        // owns : list of xsp id 
     	RPointerArray<HBufC> iRetrivedXSPIdArray;
     	
     	
    };

#endif // C_CVIMPSTSTORAGELOCALSTORE_H

// End of file
