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
* Description:  Declaration of CVIMPSTStorageManagerFactory
 *
*/



#ifndef CVIMPSTSTORAGEMANAGERFACTORY_H
#define CVIMPSTSTORAGEMANAGERFACTORY_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "s_vimpststorageviewid.h"
// FORWARD DECLARATIONS
class MVIMPSTStorageItemModel;
class MVIMPSTStorageContacts;

// CLASS DECLARATION

/**
 * Factory to create contact and group list interfaces.
 * @lib vimpststorage.lib
 * @since S60 5.0 
 */
class CVIMPSTStorageManagerFactory : public CBase
    {
    public: // New functions

		CVIMPSTStorageManagerFactory()
			{
			}
		~CVIMPSTStorageManagerFactory()
			{
			}
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
         */         
         static void InitialiseViewL( TUint32 aServiceId,
										const TDesC& aStoreName) ;
         
        /**
         * Returns the pointer to MVIMPSTStorageContactsStub
         * this pointer can be used to perform numerous operations on
         * contacts
         * @param aServiceId service id 
         * @return MVIMPSTStorageContacts* instance of the contact list interface
         */
         static  MVIMPSTStorageContacts* ContactListInterfaceL(TUint32 aServiceId);
        
        /**
         * Returns the pointer to MVIMPSTStorageItemModelStub
         * this pointer can be used to perform numerous operations on
         * contactlist
         * @param aServiceId service id 
         * @return MVIMPSTStorageItemModel* interface to itemmodel
         */
         static MVIMPSTStorageItemModel* ItemInterfaceL( TUint32 aServiceId ) ;
   
    };

#endif      // CVIMPSTSTORAGEMANAGERFACTORY_H

// End of File
