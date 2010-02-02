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
* Description:  factory class for Engine component
*
*/

// INCLUDE FILES
#include "cvimpstenginefactory.h"

#include <e32std.h>
#include <barsc.h>
#include <gulutil.h>
#include <coemain.h>

#include "cvimpstengineservicetablefetcher.h"
#include "cvimpstengine.h"
#include "cvimpststoragemanagerfactory.h"

#include <spnotifychange.h>
#include <spsettings.h>

#include "vimpstdebugtrace.h"
#include "cvimpstenginecvlistener.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::InstanceL
// Create an instance of the ximp manager
// behaving as singleton object already created then return same instance.
// -----------------------------------------------------------------------------
//
EXPORT_C MVIMPSTEngineFactory* CVIMPSTEngineFactory::InstanceL()
    {
    CVIMPSTEngineFactory* singleton;
    singleton = static_cast<CVIMPSTEngineFactory*> (Dll::Tls());
    if( !singleton )
        {
        singleton = CVIMPSTEngineFactory::NewLC();
        User::LeaveIfError( Dll::SetTls( static_cast<TAny*> (singleton ) ));
        CleanupStack::Pop(singleton);
        }
   	singleton->IncreamentRefereneCount();
   	
    return (MVIMPSTEngineFactory*)singleton;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::Release
// Release the singelton
// -----------------------------------------------------------------------------
//
EXPORT_C void CVIMPSTEngineFactory::Release()
    {
    CVIMPSTEngineFactory* singleton;
    singleton = static_cast<CVIMPSTEngineFactory*>(Dll::Tls());
    if( singleton && !singleton->DecreamentRefereneCount())
        {
        delete singleton;
        Dll::SetTls( NULL ) ;
        }
    }

// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::NewLC
// Object creation using two phase construction
// -----------------------------------------------------------------------------
//
CVIMPSTEngineFactory* CVIMPSTEngineFactory::NewLC()
    {
    CVIMPSTEngineFactory* self = new (ELeave) CVIMPSTEngineFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::ConstructL
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTEngineFactory::ConstructL()
    {
    
    CVIMPSTStorageManagerFactory::InitialiseLibraryL();
    
    //only 1 instance of iServiceTableFetcher shared between services
    iServiceTableFetcher = CVIMPSTEngineServiceTableFetcher::NewL();
    
    RArray<TUint32> serviceIdArray;
    CleanupClosePushL( serviceIdArray );
    //Get the available services
    iServiceTableFetcher->GetMasterServiceIdsL(serviceIdArray);
    
    // iterate the service array
    for ( TInt index = 0; index < serviceIdArray.Count() ; index++ )        
        {
        //create seperate engine instance for each service
        //and provide the same to the UI
        CVIMPSTEngine* item = 
        		CVIMPSTEngine::NewL(serviceIdArray[ index ], 
        		*iServiceTableFetcher);
        
        iServiceItems.Append(item);
        }
    
    CleanupStack::PopAndDestroy(); //serviceIdArray 
    
    //register for service table notifications
	RArray<TUint> serviceIds;
    CleanupClosePushL( serviceIds );
    
    iSpNotifyChange = CSPNotifyChange::NewL( *this );    
    iSpNotifyChange->NotifyChangeL( serviceIds );
    CleanupStack::PopAndDestroy( &serviceIds ) ;    
    iCVlistener = CVIMPSTEngineCVListener::NewL( *iServiceTableFetcher );
    }
    
// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::IncreamentRefereneCount
// Object creation using two phase construction
// -----------------------------------------------------------------------------
//
void CVIMPSTEngineFactory::IncreamentRefereneCount()
    {
    iReferenceCount++;
    }
// -----------------------------------------------------------------------------
// CVIMPSTEngineFactory::DecreamentRefereneCount
// Object creation using two phase construction
// -----------------------------------------------------------------------------
//
TInt CVIMPSTEngineFactory::DecreamentRefereneCount()
    {
    iReferenceCount--;
    return iReferenceCount;
    }
// ---------------------------------------------------------
// CVIMPSTEngineFactory::CVIMPSTEngineFactory
// ---------------------------------------------------------  
CVIMPSTEngineFactory::CVIMPSTEngineFactory()
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineFactory::~CVIMPSTEngineFactory
// ---------------------------------------------------------   
CVIMPSTEngineFactory::~CVIMPSTEngineFactory()
    {
    CVIMPSTStorageManagerFactory::Release();
    
    delete iSpNotifyChange;
    iSpNotifyChange = NULL;
    
    delete iServiceTableFetcher;
    
    //delete all the create engine instances
    iServiceItems.ResetAndDestroy();
    iServiceItems.Close();
    delete iCVlistener;
    iCVlistener = NULL;
    }

// ---------------------------------------------------------
// CVIMPSTEngineFactory::GetServiceEnginePtr
// ---------------------------------------------------------  
void CVIMPSTEngineFactory::GetServiceEnginePtr
					(RPointerArray<MVIMPSTEngine>& serviceIdArray) const
	{
	
	TInt count = iServiceItems.Count() ;
	
    // iterate the service array
    for ( TInt index = 0; index < count ; index++ )        
        {  
        //append the engines created for each service
        serviceIdArray.Append(iServiceItems[index]);
        }    
    
	}
	
// ---------------------------------------------------------
// CVIMPSTEngineFactory::GetServiceEnginePtr
// ---------------------------------------------------------  
	
TInt CVIMPSTEngineFactory::FindService(
				TUint aServiceId ) const
	{
	TInt count = iServiceItems.Count() ;
	TInt ret = KErrNotFound;
    // iterate the service array
    for ( TInt index = 0; index < count ; index++ )        
        {  
        //append the engines created for each service
        if(iServiceItems[index]->ServiceId() == aServiceId)
        	{
        	ret = index;
        	break;
        	}
        
        }  
    return ret;  
	}
	

// ---------------------------------------------------------------------------
// CVIMPSTEngineFactory::HandleNotifyChange()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineFactory::HandleNotifyChange(
    TServiceId aServiceId )
    {
    TRACE( T_LIT("CVIMPSTEngineFactory::HandleNotifyChange start"));        
    TRACE( T_LIT("HandleNotifyChange() serviceid: %d"), aServiceId );	
	
	TBool newService = EFalse;
	          
    if ( KErrNotFound == FindService(aServiceId) )
        {
        newService = ETrue;
        }
	        
    TRAP_IGNORE( iServiceTableFetcher->DoHandleNotifyChangeL( aServiceId, newService, GetEngine(aServiceId) ) );
    
    TRACE( T_LIT("CVIMPSTEngineFactory::HandleNotifyChange end"));        
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineFactory::HandleError()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineFactory::HandleError( TInt /*aError*/ )
    {
    TRACE( T_LIT("CVIMPSTEngineFactory::HandleError() start"));

    TRACE( T_LIT("CVIMPSTEngineFactory::HandleError() end"));
        
    }			
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineFactory::GetEngine()
// ---------------------------------------------------------------------------
//
MVIMPSTEngine* CVIMPSTEngineFactory::GetEngine( TServiceId aServiceId )
    {
    TInt count = iServiceItems.Count();
    MVIMPSTEngine* engine = NULL;
    for(int i=0 ; i<count ; i++)
	    {
	    if(iServiceItems[i]->ServiceId() == aServiceId)
		    {
		     engine = iServiceItems[i];
		    }
	    }
    return engine;
    }			
    
    

//  End of File
