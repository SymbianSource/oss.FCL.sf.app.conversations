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
* Description:  Class that capsulates im sub service data members
*
*/

#ifndef __CVIMPSTENGINEIMSUBSERVICE_H
#define __CVIMPSTENGINEIMSUBSERVICE_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h" 

//base
#include <mimcacheeventhandler.h>
#include "mvimpstenginecchhandlerobserver.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstengineimsubservice.h"


// FORWARD DECLARATIONS
class CVIMPSTEngineServiceTableFetcher;
class CIMCacheFactory;
class MIMCacheAccessor;
class MVIMPSTEngineIMSubServiceEventObserver;
class CVIMPSTEngineCchHandler;
class MVIMPSTEngineServiceConnectionEventObserver;



//CLASS DECLARATION

/**
 * class handling im sub service
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */

NONSHARABLE_CLASS( CVIMPSTEngineIMSubService ) : public CBase, 
					//	public MVIMPSTEngineSubService,
						public MVIMPSTEngineIMSubService,
						public MIMCacheEventHandler,
						public MVIMPSTEngineCchHandlerObserver
	{
	//for testcases
    friend class T_VimpstEngineImSubService;

	public:  // Two-phased constructors and destructor

		/**
		* NewLC two phase constructor.
		* @param aServiceId - Service Id
		* @param aCchHandler - reference to cchhandler
		* @param aTableFetcher - Handler to SPSettings
		* @param aServiceStateListener - Handler to the ServiceStateListener		 
		* return @ptr to CVIMPSTEngineIMSubService
		*/
		static CVIMPSTEngineIMSubService* NewLC( TUint aServiceId, 
									CVIMPSTEngineCchHandler& aCchHandler,
									CVIMPSTEngineServiceTableFetcher& aTableFetcher,
									MVIMPSTEngineServiceConnectionEventObserver& aObserver  );


		/**
		* NewL two phase constructor.
		* @param aServiceId - Service Id
		* @param aCchHandler - reference to cchhandler
		* @param aTableFetcher - Handler to SPSettings
		* @param aServiceStateListener - Handler to the ServiceStateListener		 
		* return @ptr to CVIMPSTEngineIMSubService
		*/
		static CVIMPSTEngineIMSubService* NewL( TUint aServiceId,
									CVIMPSTEngineCchHandler& aCchHandler,
									CVIMPSTEngineServiceTableFetcher& aTableFetcher,
									MVIMPSTEngineServiceConnectionEventObserver& aObserver);
					
		/**
		* C++ default destructor
		*/
		virtual ~CVIMPSTEngineIMSubService();


	public: //From MVIMPSTEngineSubService

		/**
		* @see MVIMPSTEngineSubService
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState SubServiceState() const;		

		/**
		* @see MVIMPSTEngineSubService
		*/
		TVIMPSTEnums::SubServiceType Type() const;	



	public: //From MVIMPSTIMSubService

		/**
		* @see MVIMPSTIMSubService
		*/
		void RegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver);

		/**
		* @see MVIMPSTIMSubService
		*/
		void UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver);

		/**
		* @see MVIMPSTIMSubService
		*/
		TInt GetUnreadCountL(const TDesC& aSender);


		/**
		* @see MVIMPSTIMSubService
		*/
		TBool IsConversationExistL(const TDesC& aRecipientId) const ;

		/**
		* @see MVIMPSTIMSubService
		*/
		void CloseConversationL( const TDesC& aContactId );  
		
		/**
		* @see MVIMPSTIMSubService
		*/
		
		RArray<SIMCacheChatItem> GetOpenChatListL();

	  

	public:     // From MIMCacheEventHandler

		/**
		* @see MIMCacheEventHandler
		*/
		void HandleIMCacheEventL(TIMCacheEventType aEventType, TAny* aChatMessage = NULL );

				                       
	private: //From MVIMPSTEngineCchHandlerObserver

		/**
		 * See MVIMPSTEngineCchHandlerObserver
		 */    
		void CchEventOccuredL( 
					    TUint aServiceId, 
					    TCCHSubserviceState aState, 
					    TInt aServiceError );

	        	
	private:		

		/**
		* Symbian second phase constructor	
		*/		
		void ConstructL();

		/**
		* C++ default constructor.
		* @param aServiceId - Service Id
		* @param aCchHandler - reference to cchhandler
		* @param aTableFetcher - Handler to SPSettings
		* @param aServiceStateListener - Handler to the ServiceStateListener	
		*/
		CVIMPSTEngineIMSubService( TUint aServiceId,
									CVIMPSTEngineCchHandler& aCchHandler,
									CVIMPSTEngineServiceTableFetcher& aTableFetcher,
									MVIMPSTEngineServiceConnectionEventObserver& aObserver  );
				
		
		/**
		* Resolves the CCH service state to Service Tab understandable state
		* @param aServiceId - Service Id
		* @param aCchErr - returns the last CCH Error
		* @return TVIMPSTRegistrationState  the Registration State		
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState ResolveServiceStateL( TCCHSubserviceState aState, 
		    							TInt aServiceError );
		
		/**
		* Handler for the CCH Errors
		* @param aServiceError
		*/	
		void DoHandleCchErrorL( TInt aServiceError );
		
		/**
		* Create IM Cache Accessor
		* 
		*/			
		void CreateIMCacheAccessorL();

		/**
		* Release IM Cache Accessor
		* 
		*/
		void ReleaseIMCacheAccessor();	    
		
		/**
    	* Notify the list of Observers registered for the IM Event
    	* @param aBuddyId, Buddy Id for which the Event is received
       	*/       	
		void NotifyObserversL(TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aBuddyId );
						

	private :
		
		// Service Id
		TUint iServiceId;

		//doesnot own.
		CVIMPSTEngineCchHandler& iCchHandler;                   
		
		//doesnt own - handler to the SP Settings
		CVIMPSTEngineServiceTableFetcher& iTableFetcher;

		//Doesnt Own - Handler to the ServiceStateListener
		MVIMPSTEngineServiceConnectionEventObserver& iObserver;
		
        // Stores the Type of SubService
        TVIMPSTEnums::SubServiceType iType; 

		// owned, access to accesss/update interface  
		// call realease of factory
		CIMCacheFactory* iIMCacheFactory;

		// owned, access/update to imcache
		// call realease of factory
		MIMCacheAccessor* iIMCacheAccessor;

		//Doesnt Own
		RPointerArray <MVIMPSTEngineIMSubServiceEventObserver> iChatObserver;   

		// Stores sub services current state
		TVIMPSTEnums::TVIMPSTRegistrationState  iServiceState;	

	};

#endif      //__CVIMPSTENGINEIMSUBSERVICE_H

//  End of File


