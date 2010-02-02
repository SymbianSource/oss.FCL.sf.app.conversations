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

#include "s_mvimpststoragecontacts.h"

#include "s_vimpststoragemanagerfactory.h"
#include "s_vimpststorageviewid.h"
#include "s_vimpststoragecontact.h"

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
    return KErrNone;
    
    }


// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::InitialiseViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTStorageManagerFactory::InitialiseViewL(
												TUint32 /*aServiceId*/,
												const TDesC& /*aStoreName*/)
    {
    
    }    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::ContactListInterfaceL()
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageContacts* CVIMPSTStorageManagerFactory::ContactListInterfaceL(TUint32 /*aServiceId*/ )
    {
    MyMVIMPSTStorageViewIdStub* contactlistInterface = new ( ELeave ) MyMVIMPSTStorageViewIdStub();
    //cast to the base class and return the MVIMPSTStorageContacts base ptr
    return dynamic_cast<MVIMPSTStorageContacts*>(contactlistInterface);    
    }    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManagerFactory::ItemInterfaceL
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTStorageItemModel* CVIMPSTStorageManagerFactory::ItemInterfaceL( TUint32 /*aServiceId*/ )
	{	
    //cast to the base class and return the MVIMPSTStorageItemModel base ptr
    return NULL;
   
    }
        

//  End of File
