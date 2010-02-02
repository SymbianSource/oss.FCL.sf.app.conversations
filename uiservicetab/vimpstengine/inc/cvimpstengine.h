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
* Description:  Class that capsulates single service data members
*
*/

#ifndef __CVIMPSTENGINE_H
#define __CVIMPSTENGINE_H

//  INCLUDES
#include <e32base.h>

#include "tvimpstenums.h"

//base
#include "mvimpstengine.h"
#include "mvimpststoragecontactsobserver.h"
#include "mvimpstenginecchuieventobserver.h"
#include "mvimpstengineserviceconnectioneventobserver.h"
// FORWARD DECLARATIONS
class CVIMPSTEngineServiceTableFetcher;
class MVIMPSTServiceListManager;
class MVIMPSTStorageServiceView;
class MVIMPSTEngineSubService;
class CVIMPSTEngineCchHandler;
class MVIMPSTEngineSearchExtentionEventObserver;
class CVIMPSTEngineSessionCntxtObserver;
class MVIMPSTStorageContactsObserver;
class MVIMPSTEngineCchUiEventObserver;

//CLASS DECLARATION
/**
 * engine class for a Single Service
 * @lib vimpstengine.dll
 * @since 5.0
 */


NONSHARABLE_CLASS( CVIMPSTEngine ) : public CBase, 
									public MVIMPSTEngine,
									public MVIMPSTEngineServiceConnectionEventObserver,
									public MVIMPSTStorageContactsObserver,
									public MVIMPSTEngineCchUiEventObserver
				                    
                                        
						
	{
	//for test cases.
    friend class T_VimpstEngine;

	public:  // Two-phased constructors and destructor

		/**
		* NewLC two phase constructor.
		* @param aServiceId - Service Id
		* @param aTableFetcher - ServiceTable Fetcher
		* return @ptr a new instance of this class
		*/
		static CVIMPSTEngine* NewLC(
					TUint32 aServiceId,
					CVIMPSTEngineServiceTableFetcher& aTableFetcher );

		
		/**
		* NewL two phase constructor.
		* @param aServiceId - Service Id
		* @param aTableFetcher - ServiceTable Fetcher
		* return @ptr a new instance of this class
		*/
		static CVIMPSTEngine* NewL(
					TUint32 aServiceId,
					CVIMPSTEngineServiceTableFetcher& aTableFetcher );
					
	   /**
		* C++ default destructor
		*/
		virtual ~CVIMPSTEngine();		


	public: //From MVIMPSTEngine	   

	  
		/**
		* See MVIMPSTEngine
		* 
		*/
		TUint32 ServiceId() const  ;		


		/**
		* See MVIMPSTEngine
		* 
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState ServiceState() const ;


		/**
		* See MVIMPSTEngine
		* 
		*/
		TInt GetBrandInfoL(TLanguage& aBrandLanguage, 
					TInt& aBrandVersion, TDes8& aBrandId) const ;



		/**
		* See MVIMPSTEngine
		* 
		*/			
		TBool IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType aType) const ;
					

		/**
		* See MVIMPSTEngine
		* 
		*/
		TBool IsSubServiceEnabled(TVIMPSTEnums::SubServiceType aType) const ;

		/**
		* See MVIMPSTEngine
		* 
		*/
		void ContactStoreIdL( 
					TDes& aContactStoreId ) const ;

		/**
		* See MVIMPSTEngine
		* 
		*/
		const TDesC& ServiceName() const ;
		
	
		/**
		* See MVIMPSTEngine
		* 
		*/
		 void RegisterServiceSessionObserverL
		 					(MVIMPSTEngineServiceStateEventObserver* aObserver) ;
		 
		/**
		* See MVIMPSTEngine
		* 
		*/
		void UnRegisterServiceSessionObserver
		 					(MVIMPSTEngineServiceStateEventObserver* aObserver) ;
		   

		/**
		* See MVIMPSTEngine
		* 
		*/	
		void IntializeStorageL() ;

		/**
		* See MVIMPSTEngine
		* 
		*/	
		void UnIntializeStorage() ;	   	    


		/**
		* See MVIMPSTEngine
		* 
		*/
		MVIMPSTEngineSubService* SubService(TVIMPSTEnums::SubServiceType aType) const;


		/**
		* See MVIMPSTEngine
		* 
		*/
		MVIMPSTEngineExtentionFeatures* ExtentionFeatures(TVIMPSTEnums::ExtentionType aType) const ;


	
		/**
		* See MVIMPSTEngine
		* 
		*/
		HBufC* GetOwnUserIdFromCChOrStorageL() const ;

	
		/**
		* See MVIMPSTEngine
		* 
		*/
		TInt Login();

		/**
		* See MVIMPSTEngine
		* 
		*/
		void LogoutL();
		
		/**
		* See MVIMPSTEngine
		* 
		*/
		void DefaultDomainNameL( TDes& aDefaultDomainName ) const;
		
		/**
		* See MVIMPSTEngine
		* 
		*/
		TInt ChangeConnectionL();
		/**
		 * See MVIMPSTEngine
		 * 
		 * @return True if password is present in the settings else returns false
         *
		 */
		TBool IsPasswordAvailableL();
		
		/**
         * See MVIMPSTEngine
         * 
        */
		TBool IsBlockSupportedL();
		
		/**
         * See MVIMPSTEngine
         * 
        */
		void DeleteDataBaseL();
		
		/**
         * See MVIMPSTEngine
         * 
        */
		MVIMPSTEnginePresenceSubService* GetPreseceSubService();

		/**
         * See MVIMPSTEngine
         * 
         */
		void FetchBlockedListL();
		
		/**
         * See MVIMPSTEngine
         * 
         */
		void RegisterBlockedListObserver(
						MVIMPSTEngineBlockedListFetchEventObserver* aOb);
						
						
		/**
		*  See MVIMPSTEngine
		* 
		*/
		void SetOwnUserIdIfChangedL(const TDesC& aUserId ) ;
		
		
		/**
		*  See MVIMPSTEngine
		* 
		*/
		void RetriveContextIfCChEnabledL() ;

	private: // From MVIMPSTEngineServiceConnectionEventObserver
		/**
		 * See MVIMPSTEngineServiceConnectionEventObserver
		 */ 
		void  HandleServceConnectionEventL();
	
	private:
		/**
		* Returns a SubService is Supported or not
		* Components intertested in knowing whether a SubService is Supported or not can use this API
		* @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
		* @return TBool, ETrue if the SubService is Supported in this Service, else returns EFalse
		*/
		TBool IsSubServiceSupportedInternal(TVIMPSTEnums::SubServiceType aType) const ;
			
	public:	    
		 /**
          * Called when all the contacts are fetched from the virtual store.
          * 
          * @since s60 5.0
          */
         void HandleContactFetchedL();
              
 	public: // From MVIMPSTStorageContactsObserver
       
       
        /**
        * HandleChange: To handle the Chnage event from the MCAStoredContactsObserver
        * @see MCAStoredContactsObserver
        * @param aList: reference to the list at which a change has occured
        * @param aContact:  reference to the contact at which a change has occured
        * @param aEventType: TVIMPSTEnums::TCAObserverEventType,type of change event that has occured 
        * @param aUserIdChanged, ETrue if userid has changed, else EFalse.
        * @return void
        */
         void HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
         							MVIMPSTStorageContactList* aList, 
	        						MVIMPSTStorageContact* aContact,
	        						TInt aContactIndex );


         //From MVIMPSTEngineCchUiEventObserver
         /**
          * Handles the change connection event from CCHUI
          *@ return void
          */
         void HandleChangeConnectionEventL();

                               

	private:		
	   	
	    /**
         * symbian second phase constructor
         * @param aServiceId service id
         */       
		void ConstructL( TUint32 aServiceId );

		/**
		* C++ constructor.
		*/
		CVIMPSTEngine( TUint32 aServiceId, 
					CVIMPSTEngineServiceTableFetcher& aTableFetcher );					
		
		
		/**
		* Parses and finds the Cumulative Service State of all Subservices
		* @return TVIMPSTRegistrationState
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState ParseGetServiceState();		
	
		/**
		* Adds Extention Feature
		* @param aFeature - Feature to be added
		*/
		void AddExtentionFeaturesL(MVIMPSTEngineExtentionFeatures* aFeature) ; 
	
		
		/**
		* Removes Extention Feature
		* @param aFeature - Feature to be Removed
		*/
		void RemoveExtentionFeatures(TVIMPSTEnums::ExtentionType aType) ;
		
		
		/**
		* Creates a list of known extention features
		* 
		*/
		void CreateExtentionFeaturesL();
		
		 /**
          * Sets the supported extension feature
          */
		void SetExtentionFeaturesSupportedL();
		
		/**
          * ReSets the Support for all extension feature
          */
		void ReSetExtentionFeaturesSupportedL();
		
	
	
							
	private :
		
		//Service Id
		TUint32 iServiceId;

		//Settings Id
		TUint32 iSettingsId;
			
		//doesnt own reference to servicetable entry
		CVIMPSTEngineServiceTableFetcher& iTableFetcher;

		//owns - service name
		HBufC*   iServiceName;
		
		//Owns, ximpfw session context observer.
		CVIMPSTEngineSessionCntxtObserver* iSessionCntxtObserver ;	
		
		//does not own reference to the contact list interface
		MVIMPSTStorageServiceView* iContactInterface;

		//owns pointer to subservice     
		RPointerArray <MVIMPSTEngineSubService> iSubService;

		// Own, cch handler for this service
		CVIMPSTEngineCchHandler*    iCchHandler;

		//owns pointer to subservice     
		RPointerArray <MVIMPSTEngineExtentionFeatures> iExtentionFeatures;

		// Doesnt Own, array of observers.
		RPointerArray<MVIMPSTEngineServiceStateEventObserver> iObserverArray;
		
		//Owns Pointer to the ServiceState Listener

		//Cumulative Service State of all SubServices Belonging to this Service
		TVIMPSTEnums::TVIMPSTRegistrationState iState;
	
		//block list observer to notify the blocked list has been fetched.
		MVIMPSTEngineBlockedListFetchEventObserver* iBlockListObs;
   
	};

#endif      //__CVIMPSTENGINE_H

//  End of File


