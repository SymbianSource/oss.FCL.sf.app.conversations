/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#ifndef S_VIMPSTSTORAGEMANAGERFACTORY_H
#define S_VIMPSTSTORAGEMANAGERFACTORY_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MVIMPSTStorageContacts;
class MVIMPSTStorageItemModel;

// CLASS DECLARATION

/**
 * Factory to create contact and group list interfaces.
 * @lib vimpststorage.lib
 * @since S60 5.0 
 */
class CVIMPSTStorageManagerFactory : public CBase
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
         * Inititalises the Store for the Service Id
         * Must be called before using  ContactListInterfaceL,
         * ItemInterfaceL API's.
         * @param aServiceId service id
         * @param aStoreName storename for this service
         * @param aServiceName ServiceName
         */         
         static void InitialiseViewL( TUint32 aServiceId,
										const TDesC& aStoreName,
										const TDesC& aServiceName) ;
         
        /**
         * Returns the pointer to MVIMPSTStorageContacts
         * this pointer can be used to perform numerous operations on
         * contacts
         * @param aServiceId service id 
         * @return MVIMPSTStorageServiceView* instance of the contact list interface
         */
         static  MVIMPSTStorageServiceView* ContactListInterfaceL(TUint32 aServiceId);
        
        /**
         * Returns the pointer to MVIMPSTStorageItemModel
         * this pointer can be used to perform numerous operations on
         * contactlist
         * @param aServiceId service id 
         * @return MVIMPSTStorageItemModel* interface to itemmodel
         */
         static MVIMPSTStorageItemModel* ItemModelInterfaceL( TUint32 aServiceId ) ;
   
    };

#endif      // CVIMPSTSTORAGEMANAGERFACTORY_H

// End of File
