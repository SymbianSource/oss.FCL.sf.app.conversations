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
* Description:  Class that capsulates single service data members
*
*/


//  INCLUDES
#include "cvimpstenginevoipsubservice.h"

#include "cvimpstenginecchhandler.h"
#include "tvimpstconsts.h"
#include "cvimpstengineservicetablefetcher.h"
#include "mvimpstengineserviceconnectioneventobserver.h"
//debug
#include "uiservicetabtracer.h"


// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::NewL
// 
// ---------------------------------------------------------
CVIMPSTEngineVOIPSubService* CVIMPSTEngineVOIPSubService::NewL( TUint32 aServiceId,
									CVIMPSTEngineCchHandler& aCchHandler,
									CVIMPSTEngineServiceTableFetcher& aTableFetcher,
									MVIMPSTEngineServiceConnectionEventObserver& aObserver
									)
    {
	TRACER_AUTO;
	TRACE( "aServiceId: %d", aServiceId);
    
    CVIMPSTEngineVOIPSubService* self = CVIMPSTEngineVOIPSubService::NewLC(aServiceId,aCchHandler,
    										aTableFetcher, aObserver);
    CleanupStack::Pop( self );
	
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::NewLC
// 
// ---------------------------------------------------------
    
CVIMPSTEngineVOIPSubService* CVIMPSTEngineVOIPSubService::NewLC( TUint32 aServiceId,
                                                   	CVIMPSTEngineCchHandler& aCchHandler,
                                                   	CVIMPSTEngineServiceTableFetcher& aTableFetcher,
                                                   	MVIMPSTEngineServiceConnectionEventObserver& aObserver )
	{
	TRACER_AUTO;
	TRACE( "aServiceId: %d", aServiceId);
	
    CVIMPSTEngineVOIPSubService* self = new (ELeave) CVIMPSTEngineVOIPSubService(
    												aServiceId,aCchHandler,aTableFetcher,
    												aObserver  );
    CleanupStack::PushL( self );
    self->ConstructL( );
    
    return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService
// 
// ---------------------------------------------------------

CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService()
	{
	TRACER_AUTO;
   	
   	iCchHandler.UnRegisterCchObserver(ECCHVoIPSub);	
   	
 	}


// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTEngineVOIPSubService::ConstructL(  )
	{
	TRACER_AUTO;
	
	TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCchHandler.GetServiceState( 
        			iServiceId, ECCHVoIPSub, serviceState );
	
	iServiceState = ResolveServiceState(serviceState, error);    	
	
	iCchHandler.RegisterCchObserverL(this,ECCHVoIPSub);       	
    
	TRACE( "ResolveServiceState returned ServiceState: %d", iServiceState );
	
   	
   	}


// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::CVIMPSTEngineVOIPSubService
// 
// ---------------------------------------------------------

CVIMPSTEngineVOIPSubService::CVIMPSTEngineVOIPSubService( TUint32 aServiceId, 
                                              CVIMPSTEngineCchHandler& aCchHandler,
                                              CVIMPSTEngineServiceTableFetcher& aTableFetcher,
                                              MVIMPSTEngineServiceConnectionEventObserver& aObserver) :
iServiceId(aServiceId),
iCchHandler(aCchHandler),
iTableFetcher(aTableFetcher),
iType (TVIMPSTEnums::EVoip),
iObserver(aObserver)
	{
		
	}


// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::CchEventOccuredL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineVOIPSubService::CchEventOccuredL( TUint /*aServiceId*/, 
    TCCHSubserviceState aState, TInt aServiceError )
	{	
	TRACER_AUTO;
	TRACE( " TCCHSubserviceState : %d, ServiceErr: %d",aState, aServiceError );
	
	if ( aServiceError && ECCHDisabled != aState )
        {
        //we might even end up in waiting for connection state for all
        //those service which are ALR enabled
        //So better check here if you get any CCH errors
        iServiceState = ResolveServiceState( aState, aServiceError );
        iObserver.HandleServceConnectionEventL();
        DoHandleCchErrorL( aServiceError );
        }
    else
        {
        iServiceState = ResolveServiceState( aState, aServiceError );    
        iObserver.HandleServceConnectionEventL();
        }	
	
	
    }    

	
// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::DoHandleCchErrorL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineVOIPSubService::DoHandleCchErrorL( 
    TInt aServiceError )
    {
	TRACER_AUTO;
	  TRACE(" ServiceErr: %d", aServiceError );
	
	/*								
	MCchUi& cchUi = iCchHandler.CchUiApi();
										
    switch ( aServiceError )
        {
        
        case KCCHErrorNetworkLost:  
        	{
        	cchUi.ShowDialogL( iServiceId, MCchUiObserver::ECchUiDialogTypeNoConnectionAvailable );
            break;
            }
            
        default:
            {
            break;
            }
            
        }
        */
	
        
    }	
    
    

// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::ResolveServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineVOIPSubService::ResolveServiceState(
										TCCHSubserviceState aState, 
            							TInt aServiceError )
    {
	TRACER_AUTO;
        
    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;       
    
    TRACE( " iServiceId: %d, ServiceState: %d", iServiceId, aState ); 

    TBool handleServiceStates = ETrue;
    if ( aServiceError && ECCHDisabled != aState )
        {    
		//Only if the Service supports ALR, the state can goto WaitingForNetwork
		//Still API from CCH is required to know whether ALR is supported or not
        if ( (KCCHErrorInvalidSettings != aServiceError) && (ECCHConnecting == aState) )        
            {   
            TRACE( "ESVCEWaitingForNetwork" );
            handleServiceStates = EFalse;  
            state = TVIMPSTEnums::ESVCEWaitingForNetwork;	           
            }
        }
    
    if ( handleServiceStates )
        {        
        switch ( aState )
            {
            case ECCHEnabled:
                {
                TRACE( " ESVCERegistered" );    
                state = TVIMPSTEnums::ESVCERegistered;
                }
                break;

            case ECCHDisconnecting:      
                {
                TRACE( " ESVCENetworkDisConnecting");   
                state = TVIMPSTEnums::ESVCENetworkDisConnecting;
                }
                break;

            case ECCHUninitialized:
            case ECCHDisabled:  
                {
                TRACE( " ESVCENotRegistered" );   
                state = TVIMPSTEnums::ESVCENotRegistered;
                }
                break;
                
            case ECCHConnecting:               
                {
                TRACE(" ESVCENetworkConnecting" );    
                state = TVIMPSTEnums::ESVCENetworkConnecting;
                }
                break;
            
            default:
                break;
            }
        }        
        
	        
    return state;  
	
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::SubServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineVOIPSubService::SubServiceState() const
	{
	TRACER_AUTO;
	TRACE( " ServiceId: %d ServiceState: %d",iServiceId, iServiceState );
	return iServiceState;
	}
 

// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::Type
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::SubServiceType CVIMPSTEngineVOIPSubService::Type() const	
	{
	TRACER_AUTO;
	TRACE( "Type: %d", iType );
	TRACE( " [0x%x]", this );
	return iType;	
	}


    
//  End of File

