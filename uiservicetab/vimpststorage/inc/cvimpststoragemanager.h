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
* Description:  Declaration of storage manager class
 *
*/



#ifndef CVIMPSTSTORAGEMANAGER_H
#define CVIMPSTSTORAGEMANAGER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CVIMPSTStorageServiceView;

// CLASS DECLARATION

/**
 *  Factory to create contact and group list interfaces.
 */
class CVIMPSTStorageManager : public CBase
    {
    public: // New functions

		/**
         * Initialises the Library
         */
        static void InitialiseLibraryL();
         
        /**
         * Release the resources allocated 
         */
       	static TInt Release();
        
        /**
         * Returns the instance of Storagemanager
         * @return CVIMPSTStorageManager pointer
         */ 
       	static  CVIMPSTStorageManager& Instance();
         
       
        /**
         * Creates object of viewid 
         * @param aServiceId - Service Id
         * @param ContactStoreName
         * @param aServiceName ServiceName
         * @return CVIMPSTStorageViewId pointer
         */
        CVIMPSTStorageServiceView* CreateServiceViewL(TUint32 aServiceId,
        							const TDesC& aStoreName,
        							const TDesC& aServiceName);
        
        /**
         * Find the storage views index
         * @param aServiceId - Service Id
         * @return index of the view
         */
         TInt FindServiceView(TUint32 aServiceId);
         
         
        /**
         * Romoves the storage view from the array.
         * @param aServiceId - Service Id
         */
         void RemoveServiceView(TUint32 aServiceId);
         
        /**
         * Destructor
         */
          ~CVIMPSTStorageManager();

    private:
    
        /**
         * Two-phased constructor.
         */
        static CVIMPSTStorageManager* NewL();
        
        // ConstructL
        void CVIMPSTStorageManager::ConstructL();
  
       
    private:
    
    	// list of storage viewsn created for diff services
    	// owns
        RPointerArray< CVIMPSTStorageServiceView > iServiceViewList; 
       
    };

#endif      // CVIMPSTSTORAGEMANAGER_H

// End of File
