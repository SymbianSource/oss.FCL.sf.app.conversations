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
* Description:  Class that provides centralized access for UI classes 
*                to logic handling
*
*/


#ifndef     C_CVIMPSTUIVIEWMANAGER_H
#define     C_CVIMPSTUIVIEWMANAGER_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "mvimpstengine.h"

// FORWARD DECLARATIONS
class MxSPViewInfo;
class CVIMPSTUIExtensionViewInfo;
class CEikonEnv;
class CVIMPSTUiExtensionFactory;
class CVIMPSTUIExtensionService;
class CVIMPSTUIMenuExtension;

// CLASS DECLARATION

/**
 *  
 *  This class provides access to logic handling behind UI.
 *
 *  @lib vimpstui.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTUIViewManager ) : public CBase
    {
    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CVIMPSTUIViewManager* NewL( 
            const TDesC& aServiceName,
            CVIMPSTUiExtensionFactory& aExtensionFactory );

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CVIMPSTUIViewManager* NewLC(
            const TDesC& aServiceName,
            CVIMPSTUiExtensionFactory& aExtensionFactory );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTUIViewManager();
        
    
    public:
    
        /**
         * Adds new client to use engine
         *
         * @param aServiceId, service id of the client
         * @param aEngine, reference to engine.
         * @param aTabbedViewId, tabbed view id for this service
         * @param aSearchViewId, details view id for this service
         */    
        void AddNewClientL( 
            TUint aServiceId,
            MVIMPSTEngine& aEngine,
            TInt aTabbedViewId,
            TInt aSearchViewId,
            TInt aBlockedViewId  ,
            CVIMPSTUIMenuExtension& aMenuExtension);
        
        
        
        /**
         * Returns reference to shared eik env instance.
         *
         * @since S60 5.0
         * @return CEikonEnv&, eik env reference
         */                         
        CEikonEnv& EikEnv();
        
            
        /**
         * Returns view info count.
         *
         * @since S60 5.0
         * @return Count of view info items
         */                                    
        TInt ViewInfoCount() const;
        
        /**
         * Returns specific view info object
         *
         * @since S60 5.0
         * @param aIndex, index of view info object.
         * @return View info object, ownerhsip is not passed.
         */                                            
        const MxSPViewInfo* ViewInfo( TInt aIndex ) const;
        
        /**
         * Returns service count
         *
         * @since S60 5.0
         * @return TInt, service count
         */                                                
        TInt ServiceCount() const;

        CVIMPSTUIExtensionService& Service( TInt aIndex ) const ;
        
        MVIMPSTEngine& Engine( TInt aIndex ) const;
    
    private:
    
            
        /**
         * Resolves service data object by service id. If service object
         * can not be found, leaves with KErrNotFound;
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @return CVIMPSTUIExtensionService, reference to service
         */                                
        CVIMPSTUIExtensionService& ResolveServiceL( 
            TUint aServiceId );
            
            
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory 
         */    
        CVIMPSTUIViewManager( 
            const TDesC& aServiceName,
            CVIMPSTUiExtensionFactory& aExtensionFactory );
        
        /**
         * Performs the 2nd phase of construction.
         */             
        void ConstructL();        
        
    private: // Data
    
        // Service provider settings handler
        const TDesC& iServiceName;        
        
        // Not own, shared eik env instance
        CEikonEnv*  iEikEnv;        
        
        // Own, services
        RPointerArray<CVIMPSTUIExtensionService> iServices;
        
        // Own, Array of view infos to be informed to xSP Extension Manager
        RPointerArray<CVIMPSTUIExtensionViewInfo> iViewInfos;
        
        // Extension factory
        CVIMPSTUiExtensionFactory&   iExtensionFactory;           
        
      
        
    };

#endif // C_CVIMPSTUIVIEWMANAGER_H

// End of file
