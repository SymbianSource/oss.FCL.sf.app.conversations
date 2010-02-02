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
* Description: Header for Extension factory for VIMPSTUI
*
*/


#ifndef __CVIMPSTUIEXTENSIONFACTORY_H__
#define __CVIMPSTUIEXTENSIONFACTORY_H__

// INCLUDES
#include <e32base.h>
#include <CxSPFactory.h>

class MxSPViewMapper;
class MxSPContactManager;
class CxSPBaseView;
class CPbk2UIExtensionView;
class CCoeEnv;
class CxSPViewInfo;
class MVIMPSTEngineServiceTableFetcher;
class CVIMPSTUIViewManager;
class MVIMPSTEngineFactory;
class CVIMPSTUIMenuExtension;

// CLASS DECLARATION
/**
 * Implementation of the MxSPFactory.
 * Creates instances of the extension classes.
 */
// codescanner warnings  can be ignored as CxSPFactory derived from CBase

class CVIMPSTUiExtensionFactory : public CxSPFactory  // CSI: 51 #
	{
	public: // Construction and destruction
        /**
         * Static factory function that performs the 2-phased construction.
         * @return Newly created CVIMPSTUiExtensionFactory.
         */
		static CVIMPSTUiExtensionFactory* NewL();

        /**
         * Standard C++ destructor.
         */
        ~CVIMPSTUiExtensionFactory();

	private: // from MxSPFactory
        
	    /**
         * @see MxSPFactory
         */
        CxSPBaseView* CreateViewL( TUid aViewId, CPbk2UIExtensionView& aView );
        
        /**
         * @see MxSPFactory
         */
        void RegisterViewMapper( MxSPViewMapper& aMapper );
         
        /**
         * @see MxSPFactory
         */
        TInt ViewInfoResource() const;
        
        /**
         * @see MxSPFactory
         */
        TUint32 Id() const;
        
        /**
         * @see MxSPFactory
         */
        const TDesC& Name() const;
        
        /**
         * @see MxSPFactory
         */
        void RegisterContactManager( MxSPContactManager& aManager );
        
        /**
         * @see MxSPFactory
         */
        void SetVPbkContactManager( CVPbkContactManager& aVPbkContactManager );
        
        /**
         * @see MxSPFactory
         */
        void UpdateStorePropertiesL( CPbk2StorePropertyArray& aPropertyArray );
	private:     
       /**
         * Gets view count of this extension.
         * This is alternative B for providing view information.
         * Alternative A is to use ViewInfoResource method. If
         * this alternative B is used, ViewInfoResource method
         * must return KErrNotFound
         *
         * @return view count
         */  
        TInt ViewInfoCount() const ;
        
        /**
         * Gets view info at specified index.
         * This is alternative B for providing view information.
         * Alternative A is to use ViewInfoResource method. If
         * this alternative B is used, ViewInfoResource method
         * must return KErrNotFound
         *
         * @param aIndex index
         * @return view info at specified index
         */ 
        const MxSPViewInfo* ViewInfo( TInt aIndex ) const ;
        
	private:
		/**
		 * Finds the total number of services and creates different views for
		 * different services
		 * @param None
		 * @return None
		 */
		void CreateServicesL();
		/**
		 * Tells if it is search view's id
		 * @param aId
		 */

		TBool IsSearchId( TUint aId );
		
		/**
		 * Tells if it is block view's id
		 * @param aId
		 */
		TBool IsBlockeId( TUint aId );
   
		/**
		 * Tells if it is tabbed view's id
		 * @param aId, if its a tabbed view
		 */
		TBool IsTabbedId( TUint aId );
        
    private: // Implementation
        /**
         * Standard C++ constructor.
         */
		CVIMPSTUiExtensionFactory();

        /**
         * Performs the 2nd phase of the construction.
         */
		void ConstructL();

	private: // Data
		// environment
		CCoeEnv& iCoeEnv;
		
        // offset index for resource file
        TInt iResourceOffset;
        
        // Owns, view id mapper
        MxSPViewMapper* iMapper;
        
        //Owns, contact manager
        MxSPContactManager* iContactManager;
        
        // Owns, engine for all services
        CVIMPSTUIViewManager*    iEngine;
        
        //Doesnt Own. Need to call CVIMPSTEngineFactory::Release for Releasing this memory
        MVIMPSTEngineFactory* iEngineFactoryInstance;
        
        //own's. menuextension.
        CVIMPSTUIMenuExtension* iMenuExtension;
                 
	};

#endif // _CVIMPSTUiExtensionFactory_H

// End of File
