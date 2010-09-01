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
* Description: Header for extension service
*
*/


#ifndef     C_CVIMPSTUIEXTENSIONSERVICE_H
#define     C_CVIMPSTUIEXTENSIONSERVICE_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "tvimpstenums.h"

// FORWARD DECLARATINS
class CVIMPSTUiTabbedView;
class CVIMPSTUiSearchView;
class CVIMPSTUIExtensionViewInfo;
class MxSPViewMapper;
class CPbk2UIExtensionView;
class MxSPContactManager;
class MVIMPSTEngine;
class CVIMPSTUiBrandData;
class MVIMPSTCmdHandler;
class CVIMPSTUiSearchFieldArray;
class CVIMPSTUiBlockView;
class CVIMPSTUIMenuExtension;

// CLASS DECLARATION

/**
 *  Extension Service
 *  This class represents one service
 *
 *  @lib vimpstui.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTUIExtensionService ) : public CBase
    {
    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @param aServiceId, service id
         * @param aTabbedViewId, tabbed view id for this service
         * @param aSearchViewId, Search view id for this service
         * @param aServiceName, 
         * @param aEngine, engine for logic handling
         */
        static CVIMPSTUIExtensionService* NewL(
            TUint aServiceId,
            TInt aTabbedViewId,
            TInt aSearchViewId, 
            TInt aBlockedViewId,
            const TDesC& aServiceName,
            CVIMPSTUIMenuExtension& aMenuExtension,
            MVIMPSTEngine& aEngine );

        /**
         * Two-phased constructor.
         *
         * @param aService, service id
         * @param aTabbedViewId, tabbed view id for this service
         * @param aSearchViewId, Search view id for this service
         * @param aServiceName, 
         * @param aEngine, engine for logic handling
         */
        static CVIMPSTUIExtensionService* NewLC(
            TUint aServiceId,
            TInt aTabbedViewId,
            TInt aSearchViewId, 
            TInt aBlockedViewId,
            const TDesC& aServiceName,
            CVIMPSTUIMenuExtension& aMenuExtension,
            MVIMPSTEngine& aEngine );

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTUIExtensionService();
        
            
        
    public:

        /**
         * Returns pointer to tabbed view of this service. 
         * Ownership is passed
         *
         * @param aMapper, view mapper
         * @param aContactManager, contact manager
         * @param aView, view
         * @param aTabbedViewId, tabbed view id
         * @param aId, id                 
         * @return CVIMPSTUiTabbedView*, pointer
         * to tabbed view. Ownership is passed.
         */
        CVIMPSTUiTabbedView* TabbedViewL(
            MxSPViewMapper& aMapper, 
            MxSPContactManager& aContactManager,
            CPbk2UIExtensionView& aView,
            TInt aTabbedViewId,
            TUid aId,
            MVIMPSTEngine& aEngine );
        
         /**
         * Returns pointer to Search view of this service. 
         * Ownership is passed
         *
         * @param aMapper, view mapper
         * @param aContactManager, contact manager
         * @param aView, view
         * @param aSearchViewId, search view id
         * @param aId, old view id for search view                
         * @return CVIMPSTUiSearchView*, pointer
         * to tabbed view. Ownership is passed.
         */    
		CVIMPSTUiSearchView*  SearchViewL( 
	        MxSPViewMapper& aMapper, 
	        MxSPContactManager& aContactManager, 
	        CPbk2UIExtensionView& aView,
	        TInt aSearchViewId,
	        TUid aId,
	        MVIMPSTEngine& aEngine );            

         /**
         * Returns pointer to Block view of this service. 
         * Ownership is passed
         *
         * @param aMapper, view mapper
         * @param aContactManager, contact manager
         * @param aView, view
         * @param aSearchViewId, search view id
         * @return CVIMPSTUiSearchView*, pointer
         * to tabbed view. Ownership is passed.
         */    
		CVIMPSTUiBlockView*  BlockViewL( 
	        MxSPViewMapper& aMapper, 
	        MxSPContactManager& aContactManager, 
	        CPbk2UIExtensionView& aView,
	        TInt aSearchViewId,
	        MVIMPSTEngine& aEngine );            
        
        /**
         * Returns tabbed view id assigned to this service
         *
         * @return TInt, tabbed view id of this service
         */
        TInt TabbedViewId();

        /**
         * Returns Search view id assigned to this service
         *
         * @return TInt, details view id of this service
         */
		TInt SearchViewId();
		
        /**
         * Returns Block view id assigned to this service
         *
         * @return TInt, details view id of this service
         */
		TInt BlockViewId();
        
        /**
         * Returns service id
         *
         * @return TInt, service id
         */        
        TUint32 ServiceId();
        
        
        /**
         * Returns pointer of tabbed view info. Ownership is passed.
         *
         * @return CVIMPSTUIExtensionViewInfo, view info data
         */                    
        CVIMPSTUIExtensionViewInfo* TabbedViewInfoL();
        
         /**
         * Returns pointer of Search view info. Ownership is passed.
         *
         * @return CVIMPSTUIExtensionViewInfo, view info data
         */ 
        CVIMPSTUIExtensionViewInfo* SearchViewInfo();

         /**
         * Returns pointer of Blocked view info. Ownership is passed.
         *
         * @return CVIMPSTUIExtensionViewInfo, view info data
         */ 
        CVIMPSTUIExtensionViewInfo* BlockedViewInfo();


        
        /**
         * Returns service name
         *
         * @return TDesC&, service name
         */                            
        const TDesC& ServiceName();
        
        
        /**
         * Returns search string
         *
         * @return TDesC&, search string
         */                            
        const TDesC& SearchString();
        
        /**
         * Sets search string
         *
         * @param aSearchString, search string used in search view for navi pane text
         */ 
        void SetSearchStringL( const TDesC& aSearchString );
            
        /**
         * Checks if this service has aViewId in use.
         *
         * @since S60 5.0
         * @param aViewId, view id to check
         * @return ETrue if aViewId belongs to this service
         */                                 
        TBool HasViewId( TInt aViewId );
        
        /**
         * Returns service Engine
         *
         * @return Ref to the Engine
         */  
        MVIMPSTEngine& ServiceEngine ();
        
        /**
         * set all search field found from server
         * @param aEnumKeysArray, array of enums key field
         * @param aLebelKeysArray, array of label key field
         */ 
        void SetSearchFieldsDataL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray ) ;
        
        /**
         * @returns search field array refernce
         */
        CVIMPSTUiSearchFieldArray& GetSearchFieldArray()  ; 
        
    private :
    	/**
         * get label of given search key
         * 
         * @returns HBufC* lable of the given search key.
         */
    	HBufC* GetSearchKeyLabelL(TVIMPSTEnums::TVIMPSTSearchKey aSearchKey ,TBool& aBasicField )  ;
    	
    	/**
         * sort the basic field in order of 
         * first name , last name and user
         */
    	void SortBasicFields()  ;
    	
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceId, service id
         * @param aTabbedViewId, tabbedview id
         * @param aSearchViewId, search view id
         * @param aEngine, reference to engine.
         */    
        CVIMPSTUIExtensionService( 
            TUint aServiceId,
            TInt aTabbedViewId,                             
            TInt aSearchViewId, 
            TInt aBlockedViewId,                           
            CVIMPSTUIMenuExtension& aMenuExtension,
            MVIMPSTEngine& aEngine );

        /**
         * Performs the 2nd phase of construction.
         * @param aServiceName, name of the installed service.
         */             
        void ConstructL(const TDesC& aServiceName);
        
    private: // Data
    
        // Service id of this service
        TUint32   iServiceId;
    
        // tabbed view id for this service
        TInt    iTabbedViewId;        
        
        // search view id for this service
        TInt    iSearchViewId;    
        
        // blocked view id for this service
        TInt    iBlockedViewId;    

        //does not own, reference to menu extension
        CVIMPSTUIMenuExtension& iMenuExtension;
        
        // Own Service name
        HBufC*  iServiceName;
        
        // Not own, tabbed view class for this service
        CVIMPSTUiTabbedView* iTabbedView;        
        
        //Not own,search view class for this subservice if supported      
        CVIMPSTUiSearchView* iSearchView;    
        
        CVIMPSTUiBlockView* iBlockedView;
                
        // Not own, tabbed view info of this service
        CVIMPSTUIExtensionViewInfo* iTabbedViewInfo;
        
         // Not own, search view info of this service
        CVIMPSTUIExtensionViewInfo* iSearchViewInfo;

         // Not own, search view info of this service
        CVIMPSTUIExtensionViewInfo* iBlockViewInfo;

		//not owned
        MVIMPSTEngine& iEngine;
        
        //Owns command Handler for this view
        MVIMPSTCmdHandler* iCommandHandler;  
        
        /** Owns Search string
         * used to show the string in search view's navi pane
         * remove if there any other way to do the same
         */
        HBufC* iSearchString;
        
         // owns , collection of all supported fields
        CVIMPSTUiSearchFieldArray* iUiFieldsArray;

    };

#endif // C_CVIMPSTUIEXTENSIONSERVICE_H

// End of file
