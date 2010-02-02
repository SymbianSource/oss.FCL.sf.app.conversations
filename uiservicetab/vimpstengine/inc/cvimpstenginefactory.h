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
* Description: implements the factory for creating engine
*
*/

#ifndef __CVIMPSTENGINEFACTORY_H
#define __CVIMPSTENGINEFACTORY_H

//  INCLUDES
#include <e32base.h>
#include "mvimpstenginefactory.h"
#include <mspnotifychangeobserver.h>


// FORWARD DECLARATIONS
class CVIMPSTEngineServiceTableFetcher;
class MVIMPSTEngine;
class CSPNotifyChange;
class CVIMPSTEngineCVListener;

// CLASS DECLARATION
/**
 *  Factory to create engine.
 *  This class implements the factory for creating engine
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */

class CVIMPSTEngineFactory : public CBase,
							 public MVIMPSTEngineFactory,
							 public MSPNotifyChangeObserver
	{
	public:  // Two-phased constructors and destructor

	   /**
		* @returns a single instance of MVIMPSTEngineFactory
		*/
		IMPORT_C static MVIMPSTEngineFactory* InstanceL();
		
	   /**
		* Release 
		* dont call delete   
		*/
		IMPORT_C static void Release();
		
		/**
		 * c++ default destructor
		 * dont call delete 
		 */
		virtual ~CVIMPSTEngineFactory();
		
		/**
		 * return the engine instance for aServiceId.
		 */
		MVIMPSTEngine* GetEngine( TServiceId aServiceId );
		
	private:	
	
	   	/**
		 * NewLC two phase constructor.
		 * dont call delete call close 
		 */
		static CVIMPSTEngineFactory* NewLC();

		/**
		 * Second phase construction
		 * ConstructL
		 */
		void ConstructL();

		/**
		 * C++ constructor.
		 */
		CVIMPSTEngineFactory();
		
	
	public: //From MVIMPSTEngineFactory
		/*
		* @ See MVIMPSTEngineFactory
		*/
		void GetServiceEnginePtr(RPointerArray<MVIMPSTEngine>& serviceIdArray) const;
		
		/*
		* @ See MVIMPSTEngineFactory
		*/
		TInt FindService( TUint aServiceId ) const;
		
	
	private: // From MSPNotifyChangeObserver
    
        /**
         * From MSPNotifyChangeObserver 
         * Callback when service has changed
         *
         * @since S60 5.0
         * @param aServiceId changed service
         * @return None
         */
        void HandleNotifyChange( TServiceId aServiceId );

        /**
         * From MSPNotifyChangeObserver 
         * Callback when service error occures
         *
         * @since S60 5.0
         * @param aError error code
         * @return None
         */
        void HandleError( TInt aError );

        /**
         * increament the reference count of engine 
         *
         * @since S60 5.0
         */
        
        void IncreamentRefereneCount();
        /**
         * decreament the reference count of engine 
         * @return the number of ref count
         * @since S60 5.0
         */
        
        
        TInt DecreamentRefereneCount();
        
	private :	
		
		//owned pointer to service table entry
		CVIMPSTEngineServiceTableFetcher* iServiceTableFetcher;
		
		//owns array of services
		RPointerArray<MVIMPSTEngine> iServiceItems;	
		
		// Own: service observer
        CSPNotifyChange* iSpNotifyChange;   
        
        // owns : reference count
        TInt iReferenceCount;
        // owns, lister to rproperty defined by imcvengine.
        CVIMPSTEngineCVListener* iCVlistener;
		 
	};

#endif      //__CVIMPSTENGINEFACTORY_H

//  End of File


