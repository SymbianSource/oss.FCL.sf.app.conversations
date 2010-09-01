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
* Description:  Class that provides handling of vpbk local store
*
*/


#ifndef C_CVIMPSTSTORAGELOCALSTORE_H
#define C_CVIMPSTSTORAGELOCALSTORE_H

#include <e32base.h>
#include <d32dbms.h>
#include <s32file.h>

#include "mvimpststoragevpbkcontactstore.h"
#include "mvimpststoragevpbkstorehandler.h"

//forward declaration
class CVIMPSTStorageVPbkStoreHandler;
class MVPbkViewContact;
class CVPbkContactIdConverter;
class MVIMPSTStorageVPbkStoreHandler;
class MVIMPSTStorageServiceCacheWriter;
class MVPbkContactLink;
class MVPbkStoreContact;
class CVPbkContactLinkArray;
class MVIMPSTStorageContact;

// CLASS DECLARATION
/**
 *  vpbk local store 
 *  This class implements the use of Virtual Phonebook for Local Store(Contacts.cdb)
 *
 *  @lib vimpststorage.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTStorageVPbkLocalStore ) : public CActive,                     
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
        static CVIMPSTStorageVPbkLocalStore* NewL(const TDesC& aContactDb, const TDesC& aServiceName,
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
        static CVIMPSTStorageVPbkLocalStore* NewLC( const TDesC& aContactDb, const TDesC& aServiceName,
        											MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTStorageVPbkLocalStore();

	public: // from MVIMPSTStorageVPbkContactStore
	
		/**
	     * See @MVIMPSTStorageVPbkContactStore
	     * @ return ETrue
	     **/
		TBool LocalStore() const ;
		
		TInt CreateVPbkContactL( const TDesC& aUserId, 
        						 const TDesC& aDisplayName,
        						 TBool aInvitationAutoAccept = EFalse)  ;
		
		TInt RemoveVPbkContactL(const MVPbkContactLink& aContactLink) ;
		
		TInt RemoveVPbkContactsL(const MVPbkContactLink& aContactLink) ;
		
		TInt RetrieveVPbkXSPIdL(const TDesC8& aPackedLinks )  ;
		
		TInt CreateRetriveVPbkContactL(TInt aIndexToUse ) ;
		
		TInt deleteRetriveVPbkContactL(TInt aIndexToUse ) ;
		
		TInt UpdateAvatarFieldDataL(const MVPbkContactLink& aContactLink,
								    const TDesC8& aAvatartData );
		
		TInt RemoveAvatarFieldL(MVPbkStoreContact& aStoreContact);
		
		
		void AddVPbkFetchContactsL( RArray <TPtrC> &aFirstNameList, 
		                            RArray <TPtrC> &aServiceField )  ;
		
		TInt RemoveAllVPbkContactsL() ;
		
	    const TDesC& GetRetrieveVPbkXSPIdL(TInt aIndex )  ;
		
		void DeleteDatabaseL()  ;
				
    public : // MVIMPSTStorageVPbkStoreHandler
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

        
    private:
		
		/**
         * Check if database exists         
         * @return	ETrue, if database exists.
         */
		TBool DbExists();
		
		/**
         * Creates new CPermanentFileStore stream and creates database on it.         
         */
		void CreateDbL();

		/**
         * Open existing database.         
         */
		
        void OpenDbL();
		
		
		/**
         * Close database.         
         */
		void CloseDb();		
		
		/*
         * Create tables to database         
         */	
		void CreateTablesL();
		
		
		/**
         * Add a column to colset         
         * @param	aName Name of column
         * @param	aType Type of column
         * @param	aColset Colset, where column is added.
         * @param   aMaxLength Maximum column length
         *          0 = use columns default value
         */
		void AddColumnL( const TDesC& aName, TDbColType aType, 
		                             CDbColSet* aColset );
		
		
		/*
         * Leaves if free diskspace is low.         
         * @param	aSize amount of free diskspace needed.
         */
		void DoFreespaceLevelCheckL( TInt aSize );


		/**
         * Opens database table         
         */
		void OpenTableL();
		
		/**
         * Closes database table         
         */
		void CloseTable();
		
		
		/** 
         * Seek a specific row with given contactlink in the database file.         
         * @param aColNo The column number (KContactLink)
         * @param aContactLink - Packed Contact Link
         * @return ETrue if seek successful, otherwise EFalse 
         */
        TBool SeekRowL( TDbColNo aColNo, TDesC8& aContactLink );
        
        
        /** 
         * Seek a specific row with given contact identifier in the database file.         
         * @param aColNo The column number (KContactId)
         * @param aContactId - Contact identifier
         * @return ETrue if seek successful, otherwise EFalse 
         */
        TBool SeekRowL( TDbColNo aColNo, TInt32& aContactId );
        
        
        /**
         * Write the given data to the database file         
         * @param aContactLink - Packed Contact Link
         * @param aIdentifier - Contact identifier
         */
        void  WriteToStoreDbL(  TInt32& aIdentifier );
        
        
        /**
         * Reads first contact from storage         
         * @param aContactLink - Packed Contact Link
         * @return	KErrNone,	if successful.
         */
        void  ReadCurrentL( TInt32& aContactLink);
        
        
        /**
         * Reads next contact from storage         
         * @param aContactLink - Packed Contact Link
         * @return	KErrNone,	if successful.
         */
        TInt ReadNextL( TInt32& aContactId );
        
        /**
         * Reads first contact from storage         
         * @param aContactLink - Packed Contact Link
         * @return	KErrNone,	if successful.
         */
        TInt ReadFirstL(  TInt32& aContactLink );
        
               
        /*
         * Deletes contact from storage         
         * @param aContactId - Contact Identifier of the Contact to Delete
         */
        void DeleteContactFromInternalStoreL( TInt32& aContactId );
        
        void DeleteAllContactFromInternalStoreL();
        
           
    private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CVIMPSTStorageVPbkLocalStore(MVIMPSTStorageServiceCacheWriter& aServiceCacheWriter);
        
        /**
         * Performs the 2nd phase of construction.
         *
         * @param aContactDb, contact database address
         * @param aServiceName, service name         
         */        
        void ConstructL(const TDesC& aContactDb, const TDesC& aServiceName );
        
        /*
        * helper method for deleting entry from iFetchContactsToBeAdded
        */ 
        void ResetAndDestroyLocalArrayL();
        
        /*
         * compare both string without colon part
         *  return ETrue if both contact are same
         */
        TBool IsSameContactIdL(const TDesC& aFirstId, const TDesC& aSecondId );
      
    private: // Data
        
        MVIMPSTStorageServiceCacheWriter& iServiceCacheWriter;
        // Own: pointer to a class which interact with Virtual phonebook   
        CVIMPSTStorageVPbkStoreHandler* iVPbkStoreHandler;
        
        // own , contact id converter to and from link
        CVPbkContactIdConverter* iIdConverter;
        
      	//Ownns - DB Store
    	CPermanentFileStore*	iFileStore;

		//File Stream 
        RFs		iFs;
        
        //DataBase Object for Creating/Opening/Closing DB
        RDbStoreDatabase		iDb;
        
        // current column set
        CDbColSet*			iColset;		// owned

        RDbTable			iTable;         // current table
        
        HBufC* iLocalDBName;
       
        RArray<TVIMPSTContactInfo> iFetchContactsToBeAdded;
        
        TVIMPSTFetchingSteps iFetchStep;
        
        RPointerArray<MVIMPSTStorageContact> iRetrivedContactArray;
        // owns : id of invitation geting accepted
        HBufC* iInviteId;
    };

#endif // C_CVIMPSTSTORAGELOCALSTORE_H

// End of file
