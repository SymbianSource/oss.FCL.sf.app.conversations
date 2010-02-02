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
* Description:  Implementation of storage manager
*
*/



// INCLUDE FILES
#include "cvimpststorageserviceview.h"

#include <e32std.h>

#include "cvimpststoragemanager.h"
#include "tvimpststoragepanics.h"
#include "vimpstdebugtrace.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::InitialiseLibraryL
// Factory method that create the singleton instance of the storage manager
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageManager::InitialiseLibraryL()
    {
    TRACE( T_LIT("CVIMPSTStorageManager::InitialiseLibraryL() begin") );
    //see whether there is an instance in the TLS
    //if not create the instance and set the same in TLS
    CVIMPSTStorageManager *sm = static_cast<CVIMPSTStorageManager*>( Dll::Tls() );
    if ( ! sm )
        {
        // no existing instance, create a new one
        CVIMPSTStorageManager *manager = CVIMPSTStorageManager::NewL();
        sm = manager;
        CleanupStack::PushL( manager );
        User::LeaveIfError( Dll::SetTls( static_cast<TAny*>( sm ) ) );
        CleanupStack::Pop( manager );
        }
    TRACE( T_LIT("CVIMPSTStorageManager::InitialiseLibraryL() end") );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::Release
// Factory method to release the singleton instance of the storage manager
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageManager::Release()
    {
    TRACE( T_LIT("CVIMPSTStorageManager::Release() begin") );
    //if any instance in TLS get the same and delete it    
    CVIMPSTStorageManager *storage = static_cast<CVIMPSTStorageManager*>( Dll::Tls() );
    if ( storage )
        {
        delete storage;
        Dll::SetTls( NULL );
        }
    TRACE( T_LIT("CVIMPSTStorageManager::Release() end") );
    return KErrNone;
    }



// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::Instance()
// -----------------------------------------------------------------------------
//
CVIMPSTStorageManager& CVIMPSTStorageManager::Instance()
    {
    //get the instance frm TLS and return the same
    TRACE( T_LIT("CVIMPSTStorageManager::Instance() begin") );
    CVIMPSTStorageManager *storage = static_cast<CVIMPSTStorageManager*>( Dll::Tls() );
    
    __ASSERT_ALWAYS( storage,
        User::Panic( KPanicCategory, KLIBNOTINITIALIZED ) );
	TRACE( T_LIT("CVIMPSTStorageManager::Instance() end") );
    return *storage;
    }    
    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::NewL
// -----------------------------------------------------------------------------
//
 CVIMPSTStorageManager* CVIMPSTStorageManager::NewL()
    {
    TRACE( T_LIT("CVIMPSTStorageManager::NewL() begin") );
    //creates the instance
    CVIMPSTStorageManager* self = new( ELeave ) CVIMPSTStorageManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE( T_LIT("CVIMPSTStorageManager::NewL() end") );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageManager::ConstructL()
	{
    TRACE( T_LIT("CVIMPSTStorageManager::ConstructL() begin") );
    TRACE( T_LIT("CVIMPSTStorageManager::ConstructL() end") );
    }
    
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::~CVIMPSTStorageManager
// -----------------------------------------------------------------------------
// 
 CVIMPSTStorageManager::~CVIMPSTStorageManager()
    {
    TRACE( T_LIT("CVIMPSTStorageManager::~CVIMPSTStorageManager() begin") );
    //release  all the existing views
	iServiceViewList.ResetAndDestroy();	
    TRACE( T_LIT("CVIMPSTStorageManager::~CVIMPSTStorageManager() end") );
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::CreateServiceViewL
// -----------------------------------------------------------------------------
//
CVIMPSTStorageServiceView* CVIMPSTStorageManager::CreateServiceViewL( TUint32 aServiceId,
											const TDesC& aStoreName,
											const TDesC& aServiceName )
	{
	TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() begin") );
	//creates a new view based on the service id.
	TInt index = KErrNotFound;
	CVIMPSTStorageServiceView* serviceView = NULL;
	
	//check if the view already exists in the viewlist
	index = FindServiceView(aServiceId);
	if( KErrNotFound == index )
		{
		TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() service does not exit") );
		// service view is not found 
		// check if store name is valid create a service view for aServiceId
		if( aStoreName.Length() )
			{
			TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() store name is valid") );
			serviceView = CVIMPSTStorageServiceView::NewL(aServiceId,
	    										aStoreName, aServiceName);	
			iServiceViewList.Append(serviceView); 
			TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() new view created for serviceId = %d"),aServiceId );
			}
		// if store name is not valid ,return NULL
		}
    else
	    {
	    TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() service already exist = %d"),aServiceId );
	    serviceView = iServiceViewList[ FindServiceView(aServiceId)];
	    }
    //return the extisting view from the viewlist
    TRACE( T_LIT("CVIMPSTStorageManager::CreateServiceViewL() end") );
	return serviceView;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::FindServiceView
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageManager::FindServiceView( TUint32 aServiceId)
	{
	TRACE( T_LIT("CVIMPSTStorageManager::FindServiceView() begin") );
	//search whether the view exists in the view list.
	TInt index = KErrNotFound;
    TInt count = iServiceViewList.Count();
    for(TInt i = 0; i < count; i++ )
	    {
    	if( aServiceId == iServiceViewList[ i ]->GetServiceId() )
			{
			TRACE( T_LIT("CVIMPSTStorageManager::FindServiceView() aServiceId found = %d") ,aServiceId ); 
			index = i;
			break;
			}
		}
	TRACE( T_LIT("CVIMPSTStorageManager::FindServiceView() end") );	
	return index;
	}
	
	
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::RemoveServiceView
// -----------------------------------------------------------------------------
//
void  CVIMPSTStorageManager::RemoveServiceView( TUint32 aServiceId)
	{
	TRACE( T_LIT("CVIMPSTStorageManager::RemoveServiceView() begin") );
	//searches and removes the view from the view list.
    TInt index = FindServiceView(aServiceId);
	if( index >= 0 )
		{
		TRACE( T_LIT("CVIMPSTStorageManager::RemoveServiceView() aServiceId found = %d") ,aServiceId ); 
		//view exists, so delete and remove the same
		CVIMPSTStorageServiceView* serviceView = iServiceViewList[ index ];
		iServiceViewList.Remove( index ); 
		delete serviceView;
		iServiceViewList.Compress();     
		}
	TRACE( T_LIT("CVIMPSTStorageManager::RemoveServiceView() end") );   
	}

//  End of File
