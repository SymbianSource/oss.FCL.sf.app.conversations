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
* Description:  Class that provides handling of vpbk store
*
*/


#ifndef CVIMPSTSTOTRAGEVPBKSTOREHANDLER_H
#define CVIMPSTSTOTRAGEVPBKSTOREHANDLER_H

// INCLUDES
#include <e32base.h>
#include <cntdef.h>

#include "mvimpststoragevpbkstorehandler.h"

#include <MVPbkContactViewObserver.h>
#include <MVPbkContactStoreObserver.h>
#include <MVPbkContactStoreListObserver.h>
#include <MVPbkContactObserver.h>
#include <MVPbkBatchOperationObserver.h>
#include <MVPbkSingleContactOperationObserver.h>

#include <badesca.h>

// FORWARD DECLARATIONS
class MVPbkContactViewBase;
class MVPbkContactLink;
class CVPbkContactManager;
class MVPbkContactStoreList;
class MVPbkContactView;
class MVPbkStoreContact;
class MVPbkViewContact;
class CVPbkContactLinkArray;
class MVIMPSTStorageVPbkStoreHandler;
class CPbk2SortOrderManager;
class MVPbkContactOperationBase;
class MVPbkBaseContact;
class CVPbkContactLinkArray;

// CLASS DECLARATION
/**
 *  vpbk store Handler
 *  This class implements the use of Virtual Phonebook for XSP Stores
 *
 *  @lib vimpststorage.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTStorageVPbkStoreHandler ) : public CActive,
													  public MVPbkContactViewObserver, 
								                      public MVPbkContactStoreObserver,
								                      public MVPbkContactStoreListObserver,                                          
								                      public MVPbkContactObserver,
								                      public MVPbkBatchOperationObserver,
								                      public MVPbkSingleContactOperationObserver
    {
    
      struct TVIMPSTAvatarData
          {
          // owns
          MVPbkContactLink* iContactLink;
          // owns
          HBufC8* iAvatarContent;
          };
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
        static CVIMPSTStorageVPbkStoreHandler* NewL(const TDesC& aContactDb,
        											const TDesC& aServiceName, 
        											MVIMPSTStorageVPbkStoreHandler& aStoreHandler,
        											TBool aLocalStore );

        /**
         * Two-phased constructor.
         *
         * @param aContactDb, contact database
         * @param aListId, Default List ID 
         * @param aServiceId - Service Id
         * @param aServiceName - Service Name
         * @param aStorageView, Ref to Storage (CVIMPSTStorageViewId)
         */
        static CVIMPSTStorageVPbkStoreHandler* NewLC(const TDesC& aContactDb,
        										 	 const TDesC& aServiceName,
        											 MVIMPSTStorageVPbkStoreHandler& aStoreHandler,
        											 TBool aLocalStore );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTStorageVPbkStoreHandler();
	public : // 
		/**
         * return vpbk store
         * @param aUri uri of store
         */  
		MVPbkContactStore* GetDefaultStoreL( const TDesC& aUri );
		
		TInt CreateVPbkContactL( const TDesC& aUserId, 
        				          const TDesC& aDisplayName  )  ;
		
		TInt RemoveVPbkContactL(const CVPbkContactLinkArray& aContactLinkArray ) ;
		
		TInt RetrieveVPbkContactL(const TDesC8& aPackedLinks );
		
		TInt UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
								    const TDesC8& aAvatartData );
		
		TInt RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact);
		
		TInt RemoveAllVPbkContactsL();
	
    private: // From MVPbkContactViewObserver
    
        /**
         * From MVPbkContactViewObserver
         * Callback: Contact view is ready
         *
         * @since S60 5.0
         * @param aView, view that is ready
         */
        void ContactViewReady( MVPbkContactViewBase& aView );
        
        /**
         * From MVPbkContactViewObserver
         * Callback: Contact view is unavailable
         *
         * @since S60 5.0
         * @param aView, view that is unavailable
         */
        void ContactViewUnavailable( MVPbkContactViewBase& aView );

        /**
         * From MVPbkContactViewObserver
         * Callback: Contact is added to view
         *
         * @since S60 5.0
         * @param aView, view that contact was added to
         * @param aIndex, position to where the contact was added to
         * @param MVPbkContactLink, contact link of the added contact
         */
        void ContactAddedToView( MVPbkContactViewBase& aView, 
                TInt aIndex, const MVPbkContactLink& aContactLink );
        
        /**
         * From MVPbkContactViewObserver
         * Callback: Contact was removed from the view
         *
         * @since S60 5.0
         * @param aView, view that contact was removed from
         * @param aIndex, position from where the contact was removed from
         * @param MVPbkContactLink, contact link of the removed contact
         */
        void ContactRemovedFromView( MVPbkContactViewBase& aView, 
                TInt aIndex, const MVPbkContactLink& aContactLink );
        
        /**
         * From MVPbkContactViewObserver
         * Callback: Contact view error
         *
         * @since S60 5.0
         * @param aView, view that caused the error
         * @param aError, error that occured
         * @param aErrorNotified, was it notified
         */
        void ContactViewError( MVPbkContactViewBase& aView, 
                TInt aError, TBool aErrorNotified );
    
    private: // From MVPbkContactStoreObserver
        
        /**
         * From MVPbkContactStoreObserver
         * Callback: Contact store is ready
         *
         * @since S60 5.0
         * @param aContactStore, contact store
         */
        void StoreReady( MVPbkContactStore& aContactStore);

        /**
         * From MVPbkContactStoreObserver
         * Callback: Store is unavailable
         *
         * @since S60 5.0
         * @param aContactStore, contact store
         * @param aReason, error code
         */
        void StoreUnavailable( MVPbkContactStore& aContactStore, TInt aReason);

        /**
         * From MVPbkContactStoreObserver
         * Callback: Store event occured
         *
         * @since S60 5.0
         * @param aContactStore, contact store
         * @param aStoreEvent, event
         */
        void HandleStoreEventL( MVPbkContactStore& aContactStore , 
								    TVPbkContactStoreEvent aStoreEvent  );
    
    private: // From MVPbkContactStoreListObserver

        /**
         * From MVPbkContactStoreListObserver
         * Callback: List open is completed
         *
         * @since S60 5.0
         */
        void OpenComplete();
    
 	private:				
        /**
         * From  MVPbkContactObserver
         */
        void ContactOperationCompleted(TContactOpResult aResult);

        /**
         * From MVPbkContactObserver
         */
        void ContactOperationFailed( TContactOp aOpCode, 
        							TInt aErrorCode, 
        							TBool aErrorNotified);
   
    private: // From MVPbkBatchOperationObserver
        void StepComplete( MVPbkContactOperationBase& aOperation,
                TInt aStepSize );
        TBool StepFailed( MVPbkContactOperationBase& aOperation,
                TInt aStepSize, TInt aError );
        void OperationComplete( MVPbkContactOperationBase& aOperation );
    
	public: // From MVPbkSingleContactOperationObserver

		/**
		* from MVPbkSingleContactOperationObserver.
		* Called when the operation is completed.
		*
		* A client has the operation as a member and it can delete the operation
		* instance in this function call. If the implementation of the store
		* calls the function from the operation instance it must not handle
		* any member data after calling it.
		*
		* @param aOperation The completed operation.
		* @param aContact The contact returned by the operation.
		*                 A client must take the ownership immediately.
		*
		*                  NOTE:
		*                  If you use CleanupStack for MVPbkStoreContact
		*                  Use MVPbkStoreContact::PushL or
		*                  CleanupDeletePushL from e32base.h.
		*                  (Do Not Use CleanupStack::PushL(TAny*) because
		*                  then the virtual destructor of the M-class
		*                  won't be called when the object is deleted).
		* @since S60 v5.0
		*/
		void VPbkSingleContactOperationComplete(
					        MVPbkContactOperationBase& aOperation,
					        MVPbkStoreContact* aContact);

		/**
		* from MVPbkSingleContactOperationObserver.
		* Called if the operation fails.
		*
		* A client has the operation as a member and it can delete the operation
		* instance in this function call. If the implementation of the store
		* calls the function from the operation instance it must not handle
		* any member data after calling it.
		*
		* @param aOperation The failed operation.
		* @param aError An error code of the failure.
		* @since S60 v5.0
		*/
		void VPbkSingleContactOperationFailed(
					        MVPbkContactOperationBase& aOperation,
					        TInt aError);    
                
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
        
        void HandleContactViewReadyL( MVPbkContactViewBase& aView  );
      	
    	void DoHnadleSingleContactOperationCompleteL( MVPbkContactOperationBase& aOperation,
                                            		MVPbkStoreContact* aContact );
         
        void DoHnadleOpenCompleteL();
        
        void RetriveFieldDataAndSendEventL(const MVPbkStoreContact& aContact );
        
        /**
         * destroys the content of RArray.
         */
        void ResetAndDestroyRArray();
                                            		
    private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CVIMPSTStorageVPbkStoreHandler( MVIMPSTStorageVPbkStoreHandler& aStoreHandler,TBool aLocalStore );
        
        /**
         * Performs the 2nd phase of construction.
         *
         * @param aContactDb, contact database address
         * @param aServiceName, service Name
         */        
        void ConstructL( const TDesC& aContactDb,const TDesC& aServiceName );
        
      
      	HBufC* GetContactFieldDataLC(const MVPbkStoreContact& aContact,
      								 TInt aFieldResId ) ;
      	 
        /**
         * To get the avatar data
         *
         * @param aContact, contact
         * @param aFieldResId, field id
         */ 
      	HBufC8* GetContactBinaryFieldDataLC(const MVPbkStoreContact& aContact,
      	                                        TInt aFieldResId ); 
      	
      	/**
      	 * check if any pending add contacts 
      	 * commit if pending
      	 */ 
      	void CommitNextPendingContactsL() ;
      	
    private: // Data
        // Not owns reference to contactStore
        MVIMPSTStorageVPbkStoreHandler& iStoreHandler;
        // Own: Virtual phonebook   
        CVPbkContactManager* iContactManager;
        // Not own, Contact store list owned by the contact 
        // manager, do not delete
        MVPbkContactStoreList* iStores;
        // Not own, Contact store owned by the contact 
        // manager, do not delete
        MVPbkContactStore* iStore;
        // Own, contact view containing contacts in store
        MVPbkContactView* iContactView;
        //OWN: Active scheduler wait for waiting the completion
        CActiveSchedulerWait    iWait;
        // owns : Service store name
        HBufC* iStoreName;
        // Own, sort order manager
        CPbk2SortOrderManager* iSortOrderManager;
        // owns : service name
        HBufC* iServiceName;
        // owns : a vpbk store contact
        MVPbkStoreContact* iStoreContact;
        // owns : operation contact
        MVPbkContactOperationBase* iContactOperation;
        // owns : a contact Link;
        MVPbkContactLink* iContactLink;
        // owns : results 
        TInt iOperationError;
        // owns : Store event type
        TVIMPSTVPbkStoreEventType iStoreEventType;
        // owns : fetching index
      	TInt iFetcingIndex;   
    	// owns : adding of contact is in progress
      	HBufC* iAddIdInProgress;
      	// ETrue if local store used
      	TBool iLocalStore;
      	// is contact operation is in use
      	TBool iCommitOperationInProgress;
      	// owns : synch contact which are overlapping or pending contacts
      	RPointerArray<MVPbkStoreContact> iUnCommittedVPbkContacts;
      	
      	// owns : synch avatars which are overlapping or pending avatars
      	RArray<TVIMPSTAvatarData> iPendingAvatars;
    
     };

#endif // CVIMPSTSTOTRAGEVPBKSTOREHANDLER_H

// End of file
