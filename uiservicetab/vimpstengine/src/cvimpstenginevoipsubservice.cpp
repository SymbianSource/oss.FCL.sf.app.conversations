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
#include "vimpstdebugtrace.h"


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
    TRACE( T_LIT("CVIMPSTEngineVOIPSubService::NewL start"));
    TRACE( T_LIT("NewL() aServiceId: %d"), aServiceId);
    
    CVIMPSTEngineVOIPSubService* self = CVIMPSTEngineVOIPSubService::NewLC(aServiceId,aCchHandler,
    										aTableFetcher, aObserver);
    CleanupStack::Pop( self );
	
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::NewL end"));
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
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::NewLC start"));
	TRACE( T_LIT("NewL() aServiceId: %d"), aServiceId);
	
    CVIMPSTEngineVOIPSubService* self = new (ELeave) CVIMPSTEngineVOIPSubService(
    												aServiceId,aCchHandler,aTableFetcher,
    												aObserver  );
    CleanupStack::PushL( self );
    self->ConstructL( );
    
    TRACE( T_LIT("CVIMPSTEngineVOIPSubService::NewLC end"));
    return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService
// 
// ---------------------------------------------------------

CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService()
	{
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService start"));
   	
   	iCchHandler.UnRegisterCchObserver(ECCHVoIPSub);	
   	
   	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::~CVIMPSTEngineVOIPSubService end"));
 	}


// ---------------------------------------------------------
// CVIMPSTEngineVOIPSubService::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTEngineVOIPSubService::ConstructL(  )
	{
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::ConstructL start"));
	
	TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCchHandler.GetServiceState( 
        			iServiceId, ECCHVoIPSub, serviceState );
	
	iServiceState = ResolveServiceState(serviceState, error);    	
	
	iCchHandler.RegisterCchObserverL(this,ECCHVoIPSub);       	
    
    TRACE( T_LIT("ConstructL() ResolveServiceState returned ServiceState: %d"), 
							iServiceState );
   	
   	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::ConstructL end"));
   	
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
	
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::CchEventOccuredL start"));
	TRACE( T_LIT("CchEventOccuredL() TCCHSubserviceState : %d, ServiceErr: %d"), 
									aState, aServiceError );
	
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
	
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::CchEventOccuredL end"));
	
    }    

	
// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::DoHandleCchErrorL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineVOIPSubService::DoHandleCchErrorL( 
    TInt aServiceError )
    {
    
    TRACE( T_LIT("CVIMPSTEngineVOIPSubService::DoHandleCchErrorL start"));
    TRACE( T_LIT("DoHandleCchErrorL() ServiceErr: %d"), 
									aServiceError );
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
	
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::DoHandleCchErrorL end"));
        
    }	
    
    

// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::ResolveServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineVOIPSubService::ResolveServiceState(
										TCCHSubserviceState aState, 
            							TInt aServiceError )
    {
    
    TRACE( T_LIT("CVIMPSTEngineVOIPSubService::ResolveServiceState start"));    
        
    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;       
    
    TRACE( T_LIT("ResolveServiceState() iServiceId: %d, ServiceState: %d"), 
    							iServiceId, aState );    	

    TBool handleServiceStates = ETrue;
    if ( aServiceError && ECCHDisabled != aState )
        {    
		//Only if the Service supports ALR, the state can goto WaitingForNetwork
		//Still API from CCH is required to know whether ALR is supported or not
        if ( (KCCHErrorInvalidSettings != aServiceError) && (ECCHConnecting == aState) )        
            {   
            TRACE( T_LIT("ResolveServiceState() ESVCEWaitingForNetwork") );
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
                TRACE( T_LIT("ResolveServiceState() ESVCERegistered") );                
                state = TVIMPSTEnums::ESVCERegistered;
                }
                break;

            case ECCHDisconnecting:      
                {
                TRACE( T_LIT("ResolveServiceState() ESVCENetworkDisConnecting") );                
                state = TVIMPSTEnums::ESVCENetworkDisConnecting;
                }
                break;

            case ECCHUninitialized:
            case ECCHDisabled:  
                {
                TRACE( T_LIT("ResolveServiceState() ESVCENotRegistered") );                
                state = TVIMPSTEnums::ESVCENotRegistered;
                }
                break;
                
            case ECCHConnecting:               
                {
                TRACE( T_LIT("ResolveServiceState() ESVCENetworkConnecting") );                
                state = TVIMPSTEnums::ESVCENetworkConnecting;
                }
                break;
            
            default:
                break;
            }
        }        
        
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::ResolveServiceState end"));
	        
    return state;  
	
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::SubServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineVOIPSubService::SubServiceState() const
	{
	TRACE( T_LIT("SubServiceState() ServiceId: %d ServiceState: %d"), 
							iServiceId, iServiceState );
	return iServiceState;
	}
 

// ---------------------------------------------------------------------------
// CVIMPSTEngineVOIPSubService::Type
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::SubServiceType CVIMPSTEngineVOIPSubService::Type() const	
	{
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService::Type: %d"), iType );
	TRACE( T_LIT("CVIMPSTEngineVOIPSubService: [0x%x]"), this );		    	
	return iType;	
	}


    
//  End of File

