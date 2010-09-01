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
* Description:  implementation for  CIcpPlugin
*
*/


#include "cicpplugin.h"

#include <ecom.h>
#include <implementationproxy.h>
#include <e32debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <apgtask.h>
#include <w32std.h>

#include "cicpservicemanager.h"
#include "icplogger.h"
#include "imconnectionprovideruids.hrh"
#include "cicpservicerequest.h"


// -----------------------------------------------------------------------------
// CIcpPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CIcpPlugin::ConstructL()
    {
    ICPLOGSTRING( "CIcpPlugin::ConstructL" );

    iServiceManager = CIcpServiceManager::NewL( iServiceObserver );
    }


// -----------------------------------------------------------------------------
// CIcpPlugin::CIcpPlugin()
// -----------------------------------------------------------------------------
//
CIcpPlugin::CIcpPlugin( MCchServiceObserver& serviceObserver ) :
    iServiceObserver( serviceObserver )
    {
	}

	
// -----------------------------------------------------------------------------
// CIcpPlugin::NewL
// -----------------------------------------------------------------------------
//
CIcpPlugin* CIcpPlugin::NewL( TAny* aParam )
	{
    ICPLOGSTRING2( "CIcpPlugin::NewL aParam: 0x%x", aParam );
    __ASSERT_DEBUG( aParam, User::Panic( KNullDesC, KErrGeneral ) );

    MCchServiceObserver& serviceObserver = *((MCchServiceObserver*)(aParam)); 

    CIcpPlugin* self = new ( ELeave ) CIcpPlugin( serviceObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

// -----------------------------------------------------------------------------
// CIcpPlugin::~CIcpPlugin
// -----------------------------------------------------------------------------
//
CIcpPlugin::~CIcpPlugin()
	{
	delete iServiceManager;
	}
  
// -----------------------------------------------------------------------------
// CIcpPlugin::EnableServiceL
// -----------------------------------------------------------------------------
//
void CIcpPlugin::EnableServiceL( const TServiceSelection& aServiceSelection )
    {
    ICPLOGSTRING3( "CIcpPlugin::EnableServiceL service id: %d type: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType );
	
	iServiceManager->EnableServiceL( aServiceSelection.iServiceId, 
                                    aServiceSelection.iType );
    }


// -----------------------------------------------------------------------------
// CIcpPlugin::EnableServiceL
// -----------------------------------------------------------------------------
//
void CIcpPlugin::EnableServiceL( const TServiceSelection& aServiceSelection,
                                 TUint aIapId )
    {
    ICPLOGSTRING4( "CIcpPlugin::EnableServiceL service id: %d type: %d iap id: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType, aIapId );

	
	iServiceManager->EnableServiceL( aServiceSelection.iServiceId, 
	                                     aServiceSelection.iType, 
	                                     aIapId );

    }

// -----------------------------------------------------------------------------
// CIcpPlugin::DisableServiceL
// -----------------------------------------------------------------------------
//
void CIcpPlugin::DisableServiceL( const TServiceSelection& aServiceSelection )
    {
    ICPLOGSTRING3( "CIcpPlugin::DisableServiceL service id: %d type: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType );
	
	iServiceManager->DisableServiceL( aServiceSelection.iServiceId, 
                                      aServiceSelection.iType );
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::IsAvailableL
// -----------------------------------------------------------------------------
//
TBool CIcpPlugin::IsAvailableL( const TServiceSelection& aServiceSelection,
                                const RArray<TUint32>& aIapIdArray ) const
    {
    ICPLOGSTRING3( "CIcpPlugin::IsAvailableL service id: %d type: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType );

    return iServiceManager->IsAvailableL( aServiceSelection.iServiceId, 
                                        aServiceSelection.iType, aIapIdArray );
    }
 
// -----------------------------------------------------------------------------
// CIcpPlugin::GetServiceState
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::GetServiceState( const TServiceSelection& aServiceSelection, 
                                  TCCHSubserviceState& aState ) const
    {
    ICPLOGSTRING3( "CIcpPlugin::GetServiceState service id: %d type: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType );

    return iServiceManager->GetServiceState( aServiceSelection.iServiceId, 
                                            aServiceSelection.iType, aState );
    }
    
// -----------------------------------------------------------------------------
// CIcpPlugin::GetServiceNetworkInfo
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::GetServiceNetworkInfo( TServiceConnectionInfo& aConnectionInfo ) const
    {
    TServiceSelection& selection = aConnectionInfo.iServiceSelection;

    ICPLOGSTRING3( "CIcpPlugin::GetServiceNetworkInfo service id: %d type: %d",
                   selection.iServiceId, selection.iType );
    TInt returnval = 0;
    //TRAP_IGNORE to remove the codescanner high warning
    TRAP_IGNORE(returnval = iServiceManager->GetServiceNetworkInfoL( selection.iServiceId,
                                                   selection.iType,
                                                   aConnectionInfo.iSNAPId,
                                                   aConnectionInfo.iIapId,
                                                   aConnectionInfo.iSNAPLocked,
                                                   aConnectionInfo.iPasswordSet));
    return returnval;
    }
    
// -----------------------------------------------------------------------------
// CIcpPlugin::SetSnapId
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::SetSnapId( const TServiceSelection& aServiceSelection,
                            TUint aSnapId )
    {
    ICPLOGSTRING4( "CIcpPlugin::SetSnapId service id: %d type: %d Snap %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType, aSnapId );

    return iServiceManager->SetSnapId( aServiceSelection.iServiceId, 
                                       aServiceSelection.iType, 
                                       aSnapId );
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::SetIapId
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::SetIapId( const TServiceSelection& aServiceSelection,
                           TUint aIapId )
    {
    ICPLOGSTRING4( "CIcpPlugin::SetIapId service id: %d type: %d iap id %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType, aIapId );

    return iServiceManager->SetIapId( aServiceSelection.iServiceId, 
                                      aServiceSelection.iType, 
                                      aIapId );
    }
    
// -----------------------------------------------------------------------------
// CIcpPlugin::GetServiceInfo
// -----------------------------------------------------------------------------
//
void CIcpPlugin::GetServiceInfoL( const TServiceSelection& aServiceSelection,
                                  RBuf& aBuffer ) const
    {
    ICPLOGSTRING3( "CIcpPlugin::GetServiceInfoL service id: %d type: %d",
                   aServiceSelection.iServiceId, aServiceSelection.iType );

    iServiceManager->GetServiceInfoL( aServiceSelection.iServiceId, 
                                      aServiceSelection.iType, 
                                      aBuffer );
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::ReserveService
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::ReserveService( const TServiceSelection& aServiceSelection )
    {
    return iServiceManager->SetServiceReserved( ETrue,
                                                aServiceSelection.iServiceId, 
                                                aServiceSelection.iType ); 
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::FreeService
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::FreeService( const TServiceSelection& aServiceSelection )
    {
    return iServiceManager->SetServiceReserved( EFalse,
                                                aServiceSelection.iServiceId, 
                                                aServiceSelection.iType );
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::IsReserved
// -----------------------------------------------------------------------------
//
TBool CIcpPlugin::IsReserved( const TServiceSelection& aServiceSelection ) const
    {
    return iServiceManager->IsReserved( aServiceSelection.iServiceId, 
                                        aServiceSelection.iType );
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::ServiceStateChanged
// -----------------------------------------------------------------------------
//
void CIcpPlugin::ServiceStateChanged( 
    const TUint aServiceId,
    const TCCHSubserviceType aSubService,
    TCCHSubserviceState aState,
    TInt aError )
    {
    
    // State of the service changed. Notify observer.
    iServiceObserver.ServiceStateChanged( 
                    TServiceSelection( aServiceId, aSubService ), 
                    aState,
                    aError );
    
    }
    
// -----------------------------------------------------------------------------
// CIcpPlugin::GetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::GetConnectionParameter( 
                         const TServiceSelection& aServiceSelection,
                         TCchConnectionParameter aParameter,
                         TInt& aValue ) const
    {
    return iServiceManager->GetConnectionParameter(
   									  aServiceSelection.iServiceId, 
                                      aServiceSelection.iType,
                                      aParameter,
                                      aValue
                                       ) ;
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::GetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::GetConnectionParameter(const TServiceSelection& aServiceSelection, 
                                        TCchConnectionParameter aParameter,
                                        RBuf& aValue  ) const
    {
  	TRAPD(error, iServiceManager->GetConnectionParameterL(
   									  aServiceSelection.iServiceId, 
                                      aServiceSelection.iType,
                                      aParameter,
                                      aValue
                                       )) ;
	return error;                                       
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::SetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::SetConnectionParameter(  const TServiceSelection& aServiceSelection, 
                                            TCchConnectionParameter aParameter,
                                            TInt aValue )
    {
   	return iServiceManager->SetConnectionParameter(
   									  aServiceSelection.iServiceId, 
                                      aServiceSelection.iType,
                                      aParameter,
                                      aValue
                                       ) ;
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::SetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpPlugin::SetConnectionParameter( const TServiceSelection& aServiceSelection, 
                                            TCchConnectionParameter aParameter,
                                            const TDesC& aValue )
    {
   	TRAPD(error, iServiceManager->SetConnectionParameterL(
   									  aServiceSelection.iServiceId, 
                                      aServiceSelection.iType,
                                      aParameter,
                                      aValue
                                       )) ;
	return error;                                       
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//
// Rest of the file is for ECom initialization. 
//

// Map the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KIMConnectivityPluginImplUid, CIcpPlugin::NewL )
    };


// -----------------------------------------------------------------------------
// ImplementationGroupProxy implements for ECom
// Exported proxy for instantiation method resolution
// Returns: ImplementationTable
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

// End of file

