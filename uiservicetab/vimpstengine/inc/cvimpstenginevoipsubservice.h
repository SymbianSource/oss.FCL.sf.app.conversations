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
* Description:  Class that capsulates the API's for a VOIP SubService
*
*/

#ifndef __CVIMPSTENGINEVOIPSUBSERVICE_H
#define __CVIMPSTENGINEVOIPSUBSERVICE_H

//  INCLUDES
#include <e32base.h>

//base
#include "mvimpstenginesubservice.h"
#include "mvimpstenginecchhandlerobserver.h"

//FORWARD DELARATION
class CVIMPSTEngineCchHandler;
class CVIMPSTEngineServiceTableFetcher;
class MVIMPSTEngineServiceConnectionEventObserver;

// CLASS DECLARATION

/**
 * class handling voip subservice.
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineVOIPSubService) : 
						public CBase, 
						public MVIMPSTEngineSubService,
                        public MVIMPSTEngineCchHandlerObserver

	{
	// for testcases.
    friend class T_VimpstEngineVoipSubService;

	public:  // Two-phased constructors and destructor

		/**
		* NewLC two phase constructor.
		* @param aServiceId - Service Id
		* @param aCchHandler - reference to cchhandler
		* @param aTableFetcher - Handler to SPSettings
		* @param aServiceStateListener - Handler to the ServiceStateListener		 
		* return @ptr to CVIMPSTVoipSUbService
		*/
		static CVIMPSTEngineVOIPSubService* NewLC(TUint32 aServiceId,
							CVIMPSTEngineCchHandler& aCchHandler,
							CVIMPSTEngineServiceTableFetcher& aTableFetcher,
							MVIMPSTEngineServiceConnectionEventObserver& aObserver);
					


		/**
		* NewL two phase constructor.
		* @param aServiceId - Service Id
		* @param aCchHandler - reference to cchhandler
		* @param aTableFetcher - Handler to SPSettings
		* @param aServiceStateListener - Handler to the ServiceStateListener		 
		* return @ptr to CVIMPSTVoipSUbService
		*/
		static CVIMPSTEngineVOIPSubService* NewL(TUint32 aServiceId,
							CVIMPSTEngineCchHandler& aCchHandler,
							CVIMPSTEngineServiceTableFetcher& aTableFetcher,
							MVIMPSTEngineServiceConnectionEventObserver& aObserver);
					
		/**
		* C++  destructor
		*/
		virtual ~CVIMPSTEngineVOIPSubService();

	public: //From MVIMPSTEngineSubService
       
	  	/**
		* @see MVIMPSTEngineSubService
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState SubServiceState() const;
		
		
		/**
		* @see MVIMPSTEngineSubService
		*/
		TVIMPSTEnums::SubServiceType Type() const;
		
		
    private: // From MVIMPSTEngineCchHandlerObserver
        
        /**
         * From MVIMPSTEngineCchHandlerObserver 
         * Observer callback for CCH event
         *
         * @param aServiceId, service id
         * @param aState, service state
         * @param aServiceError, error
         * @since S60 5.0
         */
        void CchEventOccuredL( 
            TUint aServiceId, 
            TCCHSubserviceState aState, 
            TInt aServiceError );
            

	private:		
	   	
		/**
		* Symbian second pahse constructor
		* dont call delete call close 
		*/		
		void ConstructL();

		/**
		 * C++ default constructor.
		 * @param aServiceId - Service Id
         * @param aCchHandler - reference to cchhandler
         * @param aTableFetcher - Handler to SPSettings
         * @param aServiceStateListener - Handler to the ServiceStateListener
		 */
		CVIMPSTEngineVOIPSubService( TUint32 aServiceId,
								CVIMPSTEngineCchHandler& aCchHandler,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher,
								MVIMPSTEngineServiceConnectionEventObserver& aObserver );
					
	private:
		
		/**
		* Handler for the CCH Errors
		* @param aServiceError
		*/
		void DoHandleCchErrorL( TInt aServiceError ) ;


		/**
		* Resolves the CCH service state to Service Tab understandable state
		* @param aServiceId - Service Id
		* @param aCchErr - returns the last CCH Error
		* @return TVIMPSTRegistrationState  the Registration State		
		*/
		TVIMPSTEnums::TVIMPSTRegistrationState ResolveServiceState(
									TCCHSubserviceState aState, 
            							TInt aServiceError) ;
		
	private :
		
		//Service Id
		TUint32 iServiceId;
		
		//Doesnt Own - Handler for CCH Activities
		CVIMPSTEngineCchHandler& iCchHandler;		
		
		//doesnt own - Handler to SPSettings Table
		CVIMPSTEngineServiceTableFetcher& iTableFetcher;
        
		// Stores voip sub services current state
        TVIMPSTEnums::TVIMPSTRegistrationState  iServiceState;        
	  
		//SubService Type    
	    TVIMPSTEnums::SubServiceType iType;
	    
	    //Doesnt Own - Handler to the Service State Listener 
	    MVIMPSTEngineServiceConnectionEventObserver& iObserver;

	};

#endif      //__CVIMPSTENGINEVOIPSUBSERVICE_H

//  End of File


