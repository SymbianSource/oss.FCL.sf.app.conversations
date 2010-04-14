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
#include "uiservicetabtracer.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::InitialiseLibraryL
// Factory method that create the singleton instance of the storage manager
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageManager::InitialiseLibraryL()
    {
	TRACER_AUTO;
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
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::Release
// Factory method to release the singleton instance of the storage manager
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageManager::Release()
    {
	TRACER_AUTO;
    //if any instance in TLS get the same and delete it    
    CVIMPSTStorageManager *storage = static_cast<CVIMPSTStorageManager*>( Dll::Tls() );
    if ( storage )
        {
        delete storage;
        Dll::SetTls( NULL );
        }
    return KErrNone;
    }



// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::Instance()
// -----------------------------------------------------------------------------
//
CVIMPSTStorageManager& CVIMPSTStorageManager::Instance()
    {
	TRACER_AUTO;
    //get the instance frm TLS and return the same
    CVIMPSTStorageManager *storage = static_cast<CVIMPSTStorageManager*>( Dll::Tls() );
    
    __ASSERT_ALWAYS( storage,
        User::Panic( KPanicCategory, KLIBNOTINITIALIZED ) );
    return *storage;
    }    
    
    

// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::NewL
// -----------------------------------------------------------------------------
//
 CVIMPSTStorageManager* CVIMPSTStorageManager::NewL()
    {
		TRACER_AUTO;
    //creates the instance
    CVIMPSTStorageManager* self = new( ELeave ) CVIMPSTStorageManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageManager::ConstructL()
	{
	TRACER_AUTO;
    }
    
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::~CVIMPSTStorageManager
// -----------------------------------------------------------------------------
// 
 CVIMPSTStorageManager::~CVIMPSTStorageManager()
    {
		TRACER_AUTO;
    //release  all the existing views
	iServiceViewList.ResetAndDestroy();	
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::CreateServiceViewL
// -----------------------------------------------------------------------------
//
CVIMPSTStorageServiceView* CVIMPSTStorageManager::CreateServiceViewL( TUint32 aServiceId,
											const TDesC& aStoreName,
											const TDesC& aServiceName )
	{
	TRACER_AUTO;
	//creates a new view based on the service id.
	TInt index = KErrNotFound;
	CVIMPSTStorageServiceView* serviceView = NULL;
	
	//check if the view already exists in the viewlist
	index = FindServiceView(aServiceId);
	if( KErrNotFound == index )
		{
	TRACE( "service does not exit" );
		// service view is not found 
		// check if store name is valid create a service view for aServiceId
		if( aStoreName.Length() )
			{
		TRACE(" store name is valid" );
			serviceView = CVIMPSTStorageServiceView::NewL(aServiceId,
	    										aStoreName, aServiceName);	
			iServiceViewList.Append(serviceView); 
			TRACE( "new view created for serviceId = %d",aServiceId );
			}
		// if store name is not valid ,return NULL
		}
    else
	    {
    TRACE( " service already exist = %d",aServiceId );
	    serviceView = iServiceViewList[ FindServiceView(aServiceId)];
	    }
    //return the extisting view from the viewlist
	return serviceView;
	}
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::FindServiceView
// -----------------------------------------------------------------------------
//
TInt CVIMPSTStorageManager::FindServiceView( TUint32 aServiceId)
	{
	TRACER_AUTO;
	//search whether the view exists in the view list.
	TInt index = KErrNotFound;
    TInt count = iServiceViewList.Count();
    for(TInt i = 0; i < count; i++ )
	    {
    	if( aServiceId == iServiceViewList[ i ]->GetServiceId() )
			{
			TRACE( "aServiceId found = %d" ,aServiceId ); 
			index = i;
			break;
			}
		}
	return index;
	}
	
	
// -----------------------------------------------------------------------------
// CVIMPSTStorageManager::RemoveServiceView
// -----------------------------------------------------------------------------
//
void  CVIMPSTStorageManager::RemoveServiceView( TUint32 aServiceId)
	{
	TRACER_AUTO;
	//searches and removes the view from the view list.
    TInt index = FindServiceView(aServiceId);
	if( index >= 0 )
		{
		TRACE( "aServiceId found = %d" ,aServiceId ); 
		//view exists, so delete and remove the same
		CVIMPSTStorageServiceView* serviceView = iServiceViewList[ index ];
		iServiceViewList.Remove( index ); 
		delete serviceView;
		iServiceViewList.Compress();     
		}
	}

//  End of File
