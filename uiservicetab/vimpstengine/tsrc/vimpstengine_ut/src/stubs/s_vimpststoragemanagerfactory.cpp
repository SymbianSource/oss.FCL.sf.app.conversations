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
#include "cvimpststoragemanagerfactory.h"
#include "s_vimpststoragecontact.h"
#include "s_vimpststorageserviceview.h"
#include "mvimpststoragecontact.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::InitialiseLibraryL
// Factory method that initialises the vimpststorage library
// used by CVIMPSTViewIdFactory
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTStorageManagerFactory::InitialiseLibraryL()
    {
   
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::Release
// Factory method to release the resource owned by the vimpststorage library
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CVIMPSTStorageManagerFactory::Release()
    {
    return 1;  
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
    
    }    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::ContactListInterfaceL()
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageServiceView* CVIMPSTStorageManagerFactory::ContactListInterfaceL(TUint32 aServiceId)
    {
    MVIMPSTStorageServiceView *contact = vimpstserviceview_stub::NewL ();
    return contact ;
    }    
 
// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::ItemModelInterfaceL
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageItemModel* CVIMPSTStorageManagerFactory::ItemModelInterfaceL( TUint32 aServiceId )
	{
	return NULL;
    }      

//  End of File
