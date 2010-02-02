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
* Description:  Class handling Service Provider Settings
*
*/


#ifndef     C_VIMPUSTENGINESERVICETABLEFETCHER_H
#define     C_VIMPUSTENGINESERVICETABLEFETCHER_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "tvimpstenums.h"
#include <spdefinitions.h>
#include "cvimpstengine.h"


// FORWARD DECLARATIONS
class CSPSettings;
class CVIMPSTEngineServiceState;
class MVIMPSTEngine;
// CLASS DECLARATION

/**
 *  
 *  This class implements the use of Service Provider Settings
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTEngineServiceTableFetcher ) : public CBase
                                            
    {
    //for testcases.
    friend class T_VIMPSTEngineServiceTableFetcher;

    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @return Ptr to CVIMPSTEngineServiceTableFetcher
         */
        static CVIMPSTEngineServiceTableFetcher* NewL(); // from the service ui factory class implemented from xsp factory

        /**
         * Two-phased constructor.
         *
         * @return Ptr to CVIMPSTEngineServiceTableFetcher
         */
        static CVIMPSTEngineServiceTableFetcher* NewLC();
        
        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTEngineServiceTableFetcher();
        
        /**
         * Gets Service Ids to array
         *
         * @since S60 5.0
         * @param aServiceIds array for service ids
         * @return None
         */
        void GetMasterServiceIdsL( 
            RArray<TUint32>& aServiceIds );

        
        /**
         * Gets brand id
         *
         * @since S60 5.0
         * @param aServiceId service id
         * @param aBrandId brand id
         * @return None
         */
        void GetBrandIdL( TUint32 aServiceId, TDes8& aBrandId );

        
        /**
         * Returns property brand version
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @return TInt, brand version
         */        
        TInt PropertyBrandVersionL( 
            TUint32 aServiceId );
        
        /**
         * Returns property brand language.
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @return TLanguage, brand language
         */        
        TLanguage PropertyBrandLanguageL( 
            TUint32 aServiceId );

        
        
        /**
         * Gets contact store id by service id
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param aContactStoreId, service store id is copied to this parameter
         *  caller should allocate memory for this parameter
         * @return None
         */                            
        void GetContactStoreIdL(  
            TUint32 aServiceId, TDes& aContactStoreId );
            
        /**
         * Gets the name of the service
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param aServiceName, service name is copied to this parameter
         *	 caller should allocate memory for this parameter
         * @return None
         */                                             
        void GetServiceNameL( 
            TUint32 aServiceId, TDes& aServiceName ); 

		
		/**
         * Finds a given property and returns the 
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param TServicePropertyName, Property Name defined in SPSettings
         * @return error - KErrNone if Property is found, else appropriate error
         */ 
		TInt FindPropertyL( TUint32 aServiceId,
	                        TServicePropertyName aPropertyName );		
		
        /**
         * Returns ximp adapter uid.
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @return TInt, uid of XIMP Adaptation
         */                                                
        TInt XimpAdapterUidL( TUint32 aServiceId ); 
        
        /**
         * Returns presence authorization request status
         * ( auto accept or always ask) ie. how to handle
         * incoming presence authorization requests.
         *
         * @since S60 5.0
         * @param aServiceId service id
         * @return TVIMPSTPresenceRequestStatus, presence request status
         */        
       TVIMPSTEnums::TVIMPSTPresenceRequestStatus 
           PresenceRequestStatusL( TUint32 aServiceId );
           
	   
	   /**
         * Handles notify change from SPS
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param aNewService - New Service (Not shown in the Tab)
         * @param aEngine - engine instance for aServiceId.
         * @return None
         */                                    
        void DoHandleNotifyChangeL( TServiceId aServiceId, TBool aNewService ,MVIMPSTEngine* aEngine);
       
        
        /**
         * Gets deafault Domain Name by service id
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @param aDefaultDomainName, domain name is copied to this parameter
         *  caller should allocate memory for this parameter
         * @return None
         */
       	void GetDefaultDomainNameL( TUint32 aServiceId, TDes& aDefaultDomainName );
       

    private: // Implementation

        /**
         * Standard C++ constructor
         */    
        CVIMPSTEngineServiceTableFetcher();
        
        /**
         * Performs the 2nd phase of construction.
         */        
        void ConstructL();
        
        /**
         * Returns contact view uid for service identified by 
         * aServiceId.
         *
         * @since S60 5.0
         * @param aServiceId, service identifier
         * @return Tuint32, contact view uid as int
         */                                     
        TUint32 ContactViewUidL( TUint32 aServiceId );
        
        
        /**
         * Compresses view ids in sps. Gets all voip services
         * and writes view to each one of them starting from
         * base id.
         *
         * @since S60 5.0
         * @param aServiceId, service identifier
         * @return TBool, ETrue if aServiceId was new service
         */                             
        TBool CompressViewIdsL( TServiceId aServiceId = 0 );
        
        /**
         * Writes view id to service table
         *
         * @since S60 5.0
         * @param aServiceId service id
         * @param aViewId view id
         * @return None
         */
        void WriteTabViewIdToServiceTableL( 
            TUint32 aServiceId, TUint aViewId );

        /**
         * Kills phonebook
         *
         * @since S60 5.0
         * @return None
         */
        void ShutDownPhonebookL();
        

    private: // Data
    
        // Own: service provider settings
        CSPSettings* iSettings;
        
        //bool flag keep track of if data base need to be deleted.
        TBool iDeleteDataBase;
        

    };

#endif // C_VIMPUICONTACTSEXTENSIONSPSHANDLER_H

// End of file
