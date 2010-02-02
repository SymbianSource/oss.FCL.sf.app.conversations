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
* Description:  M-Class that capsulates single service API's
*
*/


#ifndef MVIMPSTENGINE_H
#define MVIMPSTENGINE_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"


//FORWARD DECLARATION
class MVIMPSTEngineSearchMgrExtention;
class MVIMPSTEngineServiceStateEventObserver;
class MVIMPSTEngineSubService;
class MVIMPSTEngineExtentionFeatures;
class MVIMPSTEnginePresenceSubService;
class MVIMPSTEngineBlockedListFetchEventObserver;

//CLASS  DECLARATION
/**
 *  
 *  This class list the M-Class for Engine API's
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
//Class declaration
class MVIMPSTEngine
	{
		
		public:		

			/**
			* Gets the Service Id
			* @return TUint32 Service Id
			*/
			virtual TUint32 ServiceId() const = 0 ;		


			/**
			* Returns the Service State - Cumulative Service State of the SubServices
			* @return TVIMPSTRegistrationState, current service state.
			*/
			virtual TVIMPSTEnums::TVIMPSTRegistrationState ServiceState() const = 0;


			/**
			* Gets the Brand Information for the current service
			* @param aBrandLanguage - Holds the BrandLanguage
			* @param aBrandVersion - Holds the Version to be used
			* @param aBrandId - Holds the brandId 
			* caller should allocate memory for all params and send the same to this API
			* The result will be available in the InParams otself.
			* @return TInt Error code
			*/
			virtual TInt GetBrandInfoL(TLanguage& aBrandLanguage, 
					TInt& aBrandVersion, TDes8& aBrandId) const = 0;



			/**
			* Returns a SubService is Supported or not
			* Components intertested in knowing whether a SubService is Supported or not can use this API
			* @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
			* @return TBool, ETrue if the SubService is Supported in this Service, else returns EFalse
			*/			
			virtual TBool IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType aType) const = 0;
					

			/**
			* Returns TBool whether a SubService is Enabled or not
			* Components intertested in knowing whether a SubService is Enabled or not can use this API
			* @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
			* @return TBool, ETrue if the SubService is Enabled in this Service, else returns EFalse
			*/
			virtual TBool IsSubServiceEnabled(TVIMPSTEnums::SubServiceType aType) const = 0;

			/**
			* Gets the StoreName for given service
			* @param aContactStoreId, Virtual store id. 
			* The caller of the this function has to assign required memory for aContactStoreId
			* aContactStoreId will be filled with the Virtual Store Id by this API
			*/
			virtual void ContactStoreIdL( 
					TDes& aContactStoreId ) const = 0;

			/**
			* returns the Service Name	
			* @return TDesC&, reference to service name. Ownership is not transferred.
			*/
			virtual const TDesC& ServiceName() const = 0;

					/**
			* Register Session Observer.
			* @aObserver, observer ot be registered.
			*/
			virtual void RegisterServiceSessionObserverL
							(MVIMPSTEngineServiceStateEventObserver* aObserver) = 0;

			/**
			* UnRegisters Session Observer
			* @aObserver, observer ot be Unregistered.   
			*/
			virtual void UnRegisterServiceSessionObserver
							(MVIMPSTEngineServiceStateEventObserver* aObserver) = 0;

			  
			/**
			* intialize storage 
			*/	
			virtual void IntializeStorageL() = 0;

			/**
			* unintialize storage
			*/	
			virtual void UnIntializeStorage() = 0;	   	    


			/**
			* Get the Ptr to the SubService given the Type of the SubService
			* This API returns a Generalised SubService Class MVIMPSTEngineSubService
			* Ownership of the SubService is not Transferred to the Caller
			* The Caller can use this (MVIMPSTEngineSubService*) ptr to typecast to appropriate SubService Interface
			* Usage	 : 			MVIMPSTEngineIMSubService* imSubService = 
			*			   	   			dynamic_cast<MVIMPSTEngineIMSubService*> 
			*			   	   			(iEngine.SubService(TVIMPSTEnums::EIM));
			*			    
			*				    if(imSubService)
			*				        {
			*				        imSubService->CloseConversationL( iContactId );
			*				        }
			* @param aType - SubService Type (TVIMPSTEnums::SubServiceType) defined in tvimpstenums.h
			* @return - Ptr to the SubService - Value can even be NULL
			*/ 
			virtual MVIMPSTEngineSubService* SubService(TVIMPSTEnums::SubServiceType aType) const= 0;


			/**
			* Get the Ptr to the ExtentionFeatures given the Type of the Extention Features
			* This API returns a Generalised ExtentionFeatures Class MVIMPSTEngineExtentionFeatures
			* Ownership of the ExtentionFeatures is not Transferred to the Caller
			* The Caller can use this (MVIMPSTEngineExtentionFeatures*) ptr to typecast to appropriate SubService Interface
			* Usage : 				MVIMPSTEngineSearchMgrExtention* searchMgr = 
			*					   	   	dynamic_cast<MVIMPSTEngineSearchMgrExtention*> 
			*					   	   	(iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch));
			*					   	
			*					   	
			*					   	if (searchMgr)
			*						   	{
			*						   	iError = searchMgr->SearchContactsL( iKeyDataArray );
			*						   	}
			* @param aType - Extention Feature Type (TVIMPSTEnums::ExtentionType) defined in tvimpstenums.h
			* @return - Ptr to the ExtentionFeatures - Value can even be NULL
			*/
			virtual MVIMPSTEngineExtentionFeatures* ExtentionFeatures(TVIMPSTEnums::ExtentionType aType) const = 0;

			/**
			* gets the UserName for the Service          
			* @return HBufC* - Ownership is transferred to the caller
			* This API allocates memory for the UserName and transfers the Ownership to the Caller
			*/
			virtual HBufC* GetOwnUserIdFromCChOrStorageL() const = 0;

		
			/**
			* API used to login to the Service                  
			*/
			virtual TInt Login() = 0;

			/**
			* API used to Logout to the Service                  
			*/
			virtual void LogoutL() = 0;
			/**
			 * 
			 * @return True if password is present in the settings else returns false
			 *
			 */
			virtual TBool IsPasswordAvailableL() = 0;
			
			/**
			* Gets the default domain name for given service
			* @param aDefaultDomainName, Domain Name. 
			* The caller of the this function has to assign required memory for aDefaultDomainName
			* aDefaultDomainName will be filled with the deafault domain name by this API
			*/			
			virtual void DefaultDomainNameL( TDes& aDefaultDomainName ) const = 0;
			
			
			/**
			 * Change Connection
			 * Shows CCHUI dialog for changing the Network Connection for this service
			 * ECchUiDialogTypeChangeConnection
			 * @return Error Code
			 */
			virtual TInt ChangeConnectionL() = 0;
			
			/**
	         * IsBlockSupportedL
	         * @returns ETrue if block/unblock is supported 
	         * else EFalse
	         */
	       virtual TBool IsBlockSupportedL() = 0;
	       
	       /**
	       * Inform storage that data base need to be deleted.
	       */
	       virtual void DeleteDataBaseL() = 0;
	       
		   /**
		   * used for returning MVIMPSTEnginePresenceSubService pointer.
		   * @return MVIMPSTEnginePresenceSubService istance pointer
		   */
		   virtual MVIMPSTEnginePresenceSubService* GetPreseceSubService() = 0;

		   /**
		   *Fetch the blocked list from server..
		   */
		   virtual void FetchBlockedListL() = 0;  
		   
		   
		   
			/**
			*  See MVIMPSTEngine
			* 
			*/
			virtual void RetriveContextIfCChEnabledL() = 0  ;
		   
		   
			/**
			*  set function
			* 
			*/
			virtual void SetOwnUserIdIfChangedL(const TDesC& aUserId ) = 0  ;
		   
		   /**
		   *@param aOb,pointer of MVIMPSTEngineBlockedListFetchEventObserver,
		   *		   Notify that fetch of blocked list has been completed..
		   */
		   virtual void RegisterBlockedListObserver(
		   			MVIMPSTEngineBlockedListFetchEventObserver* aOb) = 0;  
			/**
			* Destructor
			*/		
			virtual ~MVIMPSTEngine() 
			{}
	  

	};

#endif //MVIMPSTENGINE_H


