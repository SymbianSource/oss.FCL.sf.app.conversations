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
* Description:  contact handler class implementaion to virtual phonebook observers
 *
*/


#ifndef CVIMPSTCONTACTHANDLER_H
#define CVIMPSTCONTACTHANDLER_H

#include <e32base.h>
#include <badesca.h>
#include <s32file.h>
#include <d32dbms.h> 

#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactFindObserver.h>
#include <MVPbkContactStoreListObserver.h>
#include <CVPbkContactIdConverter.h>

// forward declaration
class CVPbkContactManager;
class MVPbkContactLinkArray;
class MVPbkContactLink;
class MVPbkStoreContact;
class MVPbkContactOperationBase;
class MVPbkContactStoreList;
class MVIMPSTDetailsViewContactObserver;
class MCCAParameter;

enum TStoreType
    {
    EStoreUnkwon = -1,
    EStoreLocal,
    EStoreServer
    };
/**
 *  contact handler class implementaion to virtual phoneboo observers
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
class CVIMPSTDetailsContactHandler :  public CActive,
							   public MVPbkSingleContactOperationObserver,
							   public MVPbkContactFindObserver,
							   public MVPbkContactStoreListObserver
	{

	public:

	    /**
	     * Two-phased constructor.
	     * @param aParameter , reference to plugin parameter
	     * @param aObserver , reference to contact view class
	     * @param aServiceStoreUri , service store name
	     * @return instance of same class 
	     * @since S60 v5.0
	     * 
	     */
	    static CVIMPSTDetailsContactHandler* NewL( MCCAParameter& aParameter,
	    									MVIMPSTDetailsViewContactObserver& aObserver,
	    									const TDesC& aServiceStoreUri,
	    									const TDesC& aServiceName,
	    									TUint aserviceId);

	    /**
	     * Destructor.
	     * @since S60 v5.0
	     */
	    ~CVIMPSTDetailsContactHandler();
	    
	    /**
	     * set the specified link ,it take the ownership of links.
	     * @param aLinks , links to set
	     * @param aReadXspId , need to read xsp id from contacts.cdb or not
	     * @since S60 v5.0
	     */
	    void SetLinks( MVPbkContactLinkArray* aLinks,TBool aReadXspId );
	    
	    /**
	     * @return XSPuserId.
	     * @since S60 v5.0
	     */
	    const TDesC& GetXSPUserId();
	    
	    /**
	     * @return List of all open store.
	     * @since S60 v5.0
	     */
	    MVPbkContactStoreList& ContactStoresL();

	    /**
        * @return store type
        * @since S60 v5.0
        */
	    TStoreType StoreType();
	    
	    /**
	     * @return current iCurrentLink.
	     * @since S60 v5.0
	     */
	    MVPbkContactLinkArray* Links() const;
	    
	     /**
	     * cancel the ongoing request.
	     * @since S60 v5.0
	     */
	    void CancelOngoingRequest();

	public: // From CActive
		/**
	     * from CActive.
	     * @since S60 v5.0
	     */
	    void RunL();

		/**
	     * from CActive.
	     * @since S60 v5.0
	     */
	    TInt RunError(TInt aError);

		/**
	     * from CActive.
	     * @since S60 v5.0
	     */
	    void DoCancel();

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
	
	public: // From MVPbkContactFindObserver
		/**
         * Called when find is complete. Caller takes ownership of the results
         * In case of an error during find, the aResults may contain only 
         * partial results of the find
         *
         * @param aResults Array of contact links that matched the find
         *                 Callee must take ownership of this object in
         *                 the start of the function, ie. in case the 
         *                 function leaves the results must be destroyed. 
         *                 The find operation can be destroyed at the end 
         *                  of this callback.
         */
        void FindCompleteL( MVPbkContactLinkArray* aResults ) ;

        /**
         * Called in case the find fails for some reason. The find operation
         * can be destroyed at the end of this callback.
         * 
         * @param aError One of the system wide error codes.
         *        KErrNotReady if store is not ready (not open or unavailable)
         */
        void FindFailed( TInt aError ) ;
		
		/* 
		* get the display name of the contact to show in the status pane
		*/
        
        HBufC* GetDisplayNameLC();
  
  private: // From MVPbkContactStoreListObserver

		/**
		 * from MVPbkContactStoreListObserver
	     * Called when the opening process is complete. 
         *
         * Before this all stores have sent StoreReady or StoreUnavailable 
         * event. The client can not trust that all stores are available
         * for use when this is called.
         * @since S60 v5.0
	     */
	    void OpenComplete();

		/**
		 * from MVPbkContactStoreListObserver
	     * Called when a contact store is ready to use.
         *
         * @param aContactStore The store that is ready.
	     * @since S60 v5.0
	     */
	    void StoreReady(MVPbkContactStore& aContactStore );

		/**
		 * from MVPbkContactStoreListObserver
	     * Called when a contact store becomes unavailable.
         *
         * Client may inspect the reason of the unavailability and decide
         * whether or not it will keep the store opened (ie. listen to 
         * the store events).
         *
         * @param aContactStore The store that became unavailable.
         * @param aReason The reason why the store is unavailable.
         *                This is one of the system wide error codes.
	     * @since S60 v5.0
	     */
	    void StoreUnavailable(MVPbkContactStore& aContactStore, TInt aReason );

		/**
		 * from MVPbkContactStoreListObserver
	     * Called when changes occur in the contact store.
         *
         * @see TVPbkContactStoreEvent
         * @param aContactStore A store whose event it is.
         * @param aStoreEvent The event that has occurred.
	     * @since S60 v5.0
	     */
	    void HandleStoreEventL(MVPbkContactStore& aContactStore, TVPbkContactStoreEvent aStoreEvent );

     
	private:

		/**
	     * default constructor
	     * @param aObserver , contact observer
	     * @since S60 v5.0
	     */
	    CVIMPSTDetailsContactHandler( MVIMPSTDetailsViewContactObserver& aObserver, TUint aserviceId );

		/**
	     * provides two phase constrcution.
	     * @param aPluginParameter , plugin parameter
	     * @param aServiceStoreUri , service store name
	     * @since S60 v5.0
	     */
	    void ConstructL( MCCAParameter& aParameter, const TDesC& aServiceStoreUri, const TDesC& aServiceName );
	    
	   	/**
	     * helper method for issue pending request.
	     * @param aError , error if any 
	     * @since S60 v5.0
	     */
	    void IssueRequest(TInt aError = KErrNone);
	    
	    /* API to be used for local store only
	     * to check whether the phonebook contact is a buddy the service installed
	     * @ param aCurrentLink of the phonebook contact
	     * return ETrue if the link is a buddy of that service
	     * @since S60 v5.0
	     */
	    TBool CheckWhetherValidLocalLinkL(
	            const MVPbkContactLink& aCurrentLink );
	    
	    /* API to be used for local store service only
	     * to check whether the local %service_name%.db (which has contact identifiers) 
	     * exsists already
	     */
	    TBool DbExists();
	    /* API to be used for local store only
	     * open's the local %service_name%.db file
	     */
	    void OpenDbL();
	    
	     /* API to be used for local store only
	     * close's the local %service_name%.db file
	     */
	    void CloseDb();
	    
	    /* API to be used for local store only
	     * search the identifier aIdentifier passed in the ColNo aColNo of the db file
	     * return ETrue if the identifier is found in the db col
	     * @since S60 v5.0
	     */
	    TBool SeekRowL( TDbColNo aColNo, TInt32& aIdentifier );
	    
	    /* API to be used for local store only
	     * open db table
	     * @since S60 v5.0
	     */	     
	    void OpenTableL();

	   
	private:    // data
	    
		/**
		* Not own : Reference to details view plugin
		*/
		MVIMPSTDetailsViewContactObserver& iObserver;

		/**
		* own :Reference to contact manager.
		*/
		CVPbkContactManager* iContactManager;

		/**
		* owns : reference to link array
		*/ 
		MVPbkContactLinkArray* iLinks;

		/**
		* not owns : reference to contact link which is owned by iLinks
		*/
		const MVPbkContactLink* iCurrentLink;

		/**
		* owns : reference to operation base contact
		*/
		MVPbkContactOperationBase* iRetrieveOperation;

		/**
		* owns : extracted user id from pbk xsp id 
		*/
		HBufC* iXSPUserId;

		/**
		* owns : if ETrue , xsp id need to read
		*/
		TBool iReadXspId;

		/**
		* owns : list of open store
		*/
		MVPbkContactStoreList* iStoreList;

		/**
		*  owns : reference to active chedular
		*/
		CActiveSchedulerWait iWait;

		/**
		* owns : service store uri
		*/
		HBufC* iServiceStoreUri;
		
				
		/**
		 * Owned : Pointer to MVPbkStoreContact
		 */
		MVPbkStoreContact* iStoreContact; 
	 
		// owns : store type
		TStoreType iStoreType;
		
		//  owns, service name 
		HBufC* iServiceName;
		

        //Ownns - DB Store
        CPermanentFileStore*    iFileStore;

        //File Stream 
        RFs     iFs;
        
        //DataBase Object for Creating/Opening/Closing DB
        RDbStoreDatabase        iDb;
        
        // current column set
        CDbColSet*          iColset;        // owned

        
        RDbTable            iTable;         // current table
        
        // own , contact id converter to and from link
        CVPbkContactIdConverter* iIdConverter;
        
        TUint iServiceId;
      
	  	};

#endif // CVIMPSTCONTACTHANDLER_H

// End of File
