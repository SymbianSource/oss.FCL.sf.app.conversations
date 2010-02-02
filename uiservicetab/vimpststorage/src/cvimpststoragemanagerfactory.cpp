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
* Description:  Implementation of CVIMPSTStorageManagerFactory
*
*/

// INCLUDE FILES
#include <e32std.h>

#include "cvimpststoragemanagerfactory.h"
#include "cvimpststoragemanager.h"
#include "cvimpststorageserviceview.h"

//debug prints
#include "vimpstdebugtrace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::InitialiseLibraryL
// Factory method that initialises the vimpststorage library
// used by CVIMPSTViewIdFactory
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTStorageManagerFactory::InitialiseLibraryL()
    {
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::InitialiseLibraryL() begin") );
    CVIMPSTStorageManager::InitialiseLibraryL();
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::InitialiseLibraryL() end") );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::Release
// Factory method to release the resource owned by the vimpststorage library
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CVIMPSTStorageManagerFactory::Release()
    {
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::Release()") );
    return CVIMPSTStorageManager::Release();  
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::InitialiseViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTStorageManagerFactory::InitialiseViewL(
												TUint32 aServiceId,
												const TDesC& aStoreName,
												const TDesC& aServiceName)
    {
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::InitialiseViewL() begin") );
    //get the stroage manager instance
    CVIMPSTStorageManager& storagemanager = CVIMPSTStorageManager::Instance( );
    
    //create the storageview based on the service id
    CVIMPSTStorageServiceView* contactlistInterface = 
    				storagemanager.CreateServiceViewL( aServiceId, aStoreName, aServiceName );
    
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::InitialiseViewL() end") );
    }    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::ContactListInterfaceL()
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageServiceView* CVIMPSTStorageManagerFactory::ContactListInterfaceL(TUint32 aServiceId)
    {
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::ContactListInterfaceL() begin") );
    //get the stroage manager instance
    CVIMPSTStorageManager& storagemanager = CVIMPSTStorageManager::Instance( );
    
    //create the storageview based on the service id
    CVIMPSTStorageServiceView* contactlistInterface = 
    				storagemanager.CreateServiceViewL( aServiceId, KNullDesC, KNullDesC );
    
    //cast to the base class and return the MVIMPSTStorageServiceView base ptr
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::ContactListInterfaceL() begin") );
    return dynamic_cast<MVIMPSTStorageServiceView*>(contactlistInterface);    
    
    }    
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::ItemModelInterfaceL
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageItemModel* CVIMPSTStorageManagerFactory::ItemModelInterfaceL( TUint32 aServiceId )
	{	
	TRACE( T_LIT("CVIMPSTStorageManagerFactory::ItemModelInterfaceL() begin") );
    //get the stroage manager instance
    CVIMPSTStorageManager& storagemanager = CVIMPSTStorageManager::Instance( );    
    
    //create the storageview based on the service id
    CVIMPSTStorageServiceView* itemModel = 
    				storagemanager.CreateServiceViewL( aServiceId, KNullDesC, KNullDesC );
    
    //cast to the base class and return the MVIMPSTStorageItemModel base ptr
    TRACE( T_LIT("CVIMPSTStorageManagerFactory::ItemModelInterfaceL() end") );
    return dynamic_cast<MVIMPSTStorageItemModel*>(itemModel);
   
    }      

//  End of File
