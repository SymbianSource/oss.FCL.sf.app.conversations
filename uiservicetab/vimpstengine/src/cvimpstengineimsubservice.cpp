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
#include "cvimpstengineimsubservice.h"

#include "cvimpstenginecchhandler.h"
#include "cvimpstengineservicetablefetcher.h"
#include "mvimpstengineimsubserviceeventobserver.h"
#include "mvimpstengineserviceconnectioneventobserver.h"

#include <e32std.h>
#include <barsc.h>
#include <gulutil.h>
#include <coemain.h>
#include <e32base.h>

#include "tvimpstconsts.h"

// imcache  related headers
#include "cimcachefactory.h"
#include "mimcacheaccessor.h"

#include "uiservicetabtracer.h"

//system includes
#include <e32property.h>

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::NewL
// 
// ---------------------------------------------------------
CVIMPSTEngineIMSubService* CVIMPSTEngineIMSubService::NewL(
								TUint aServiceId,
								CVIMPSTEngineCchHandler& aCchHandler,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher,
								MVIMPSTEngineServiceConnectionEventObserver& aObserver
								 )
    {
	TRACER_AUTO;
    TRACE( "ServiceId: %d", aServiceId );
    
    CVIMPSTEngineIMSubService* self = CVIMPSTEngineIMSubService::NewLC(
    												aServiceId,
    												aCchHandler,
    												aTableFetcher,    												
    												aObserver );
    CleanupStack::Pop( self );
    
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::NewLC
// 
// ---------------------------------------------------------
    
CVIMPSTEngineIMSubService* CVIMPSTEngineIMSubService::NewLC(
								TUint aServiceId,
								CVIMPSTEngineCchHandler& aCchHandler,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher,
								MVIMPSTEngineServiceConnectionEventObserver& aObserver
								 )
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", aServiceId );
	
    CVIMPSTEngineIMSubService* self = new (ELeave) CVIMPSTEngineIMSubService(aServiceId, 
    												aCchHandler, aTableFetcher,
    												aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService
// 
// ---------------------------------------------------------

CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService()
	{
	TRACER_AUTO;
	
	iChatObserver.Reset();
    iChatObserver.Close();
    
	iCchHandler.UnRegisterCchObserver(ECCHIMSub);
		
	ReleaseIMCacheAccessor();	
	
 	}


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTEngineIMSubService::ConstructL()
	{			
	
	TRACER_AUTO;
	TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCchHandler.GetServiceState( 
        			iServiceId, ECCHIMSub, serviceState );
	
	iServiceState = ResolveServiceStateL(serviceState, error);    	
	
	iCchHandler.RegisterCchObserverL(this,ECCHIMSub);       
	
    TRACE( "ResolveServiceStateL returned ServiceState: %d", iServiceState );
 
    
    }


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::CVIMPSTEngine
// 
// ---------------------------------------------------------

CVIMPSTEngineIMSubService::CVIMPSTEngineIMSubService( TUint aServiceId,
								CVIMPSTEngineCchHandler& aCchHandler,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher,
								MVIMPSTEngineServiceConnectionEventObserver& aObserver) :
	iServiceId(aServiceId),
	iCchHandler(aCchHandler),
	iTableFetcher(aTableFetcher),	
	iObserver(aObserver),
	iType (TVIMPSTEnums::EIM)
	{
	}




		
// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::RegisterChatObserver
// 
// ---------------------------------------------------------	

void CVIMPSTEngineIMSubService::RegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver)
	{
	TRACER_AUTO;
	if (aObserver)
		{
		TInt index = iChatObserver.Find(aObserver);
		        if( index == KErrNotFound )
		            {
		            iChatObserver.Append( aObserver );   
		            }
		}    	
	
	}

// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::UnRegisterChatObserver
// 
// ---------------------------------------------------------	

void CVIMPSTEngineIMSubService::UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver)
	{
	TRACER_AUTO;
	
	if (aObserver)
		{
		
		TInt index = iChatObserver.Find(aObserver);
		        
		        if( index >=0 )
		            {
		            iChatObserver.Remove( index );
		            iChatObserver.Compress();
		            }
		        
	    
		}
			
	}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::HandleIMCacheEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------  

void CVIMPSTEngineIMSubService::HandleIMCacheEventL(TIMCacheEventType aEventType, TAny* aChatMessage  )
	{
	TRACER_AUTO;
	TRACE( "TIMCacheEventType: %d", aEventType );
							
	switch( aEventType )
	    {
	    case EIMCacheUnreadMessage :
	        {
	        if (!aChatMessage)
	            {
	            return;	
	            }
	        SIMCacheMessageData* chatData = static_cast<SIMCacheMessageData*>( aChatMessage ) ;
	        // check that buddy id exists
	        if ( chatData->iBuddyId )
	            {
	            TPtrC buddyId = chatData->iBuddyId->Des();
	            TRACE( "EIMCacheUnreadMessage for %S",  &buddyId );
	            NotifyObserversL(TVIMPSTEnums::EIMUnreadMessage,buddyId);               
	            }
	        break;	
	        }
	    case EIMCacheUnreadChange :
	        {
	        TRACE( "EIMCacheUnreadChange");
	        if (!aChatMessage)
	            {
	            return; 
	            }
	        SIMCacheChatItem* chatItem =  static_cast<SIMCacheChatItem*>( aChatMessage ) ;
	        if( iServiceId != chatItem->iServiceId )
	            {
	            return;
	            }
	        TPtrC buddyId = chatItem->iBuddyId->Des();
	        TRACE( "EIMCacheChatClosed for %S",  &buddyId );  
	        NotifyObserversL(TVIMPSTEnums::EIMUnreadChange, buddyId );  // ETrue for Closed conversation
	        break;
	        }
	    case EIMCacheChatStarted :
	        {
	        if (!aChatMessage)
	            {
	            return; 
	            }
	        SIMCacheChatItem* chatItem =  static_cast<SIMCacheChatItem*>( aChatMessage ) ;
	        if( iServiceId != chatItem->iServiceId )
	            {
	            return;
	            }
	        TPtrC buddyId = chatItem->iBuddyId->Des();
	        TRACE("EIMCacheChatClosed for %S", &buddyId );
	        NotifyObserversL(TVIMPSTEnums::EIMChatStarted, buddyId );  // ETrue for Closed conversation 
	        break;  
	        }
	    case EIMCacheChatClosed:
	        {
	        if (!aChatMessage)
	            {
	            return; 
	            }
	        SIMCacheChatItem* chatItem =  static_cast<SIMCacheChatItem*>( aChatMessage ) ;
	        if( iServiceId != chatItem->iServiceId )
	            {
	            return;
	            }
	        TPtrC buddyId = chatItem->iBuddyId->Des();
	        TRACE( "EIMCacheChatClosed for %S", &buddyId );
	        NotifyObserversL(TVIMPSTEnums::EIMChatClosed, buddyId );                     
	        break;  
	        }
	    case EIMCacheAllChatClosed:
	        {
	        NotifyObserversL(TVIMPSTEnums::EIMAllChatClosed, KNullDesC );                 
	        break;  
	        }
	    case EIMCacheRequestCompleted:
	        {
	        NotifyObserversL(TVIMPSTEnums::EIMRequestCompleted, KNullDesC );  
	        break;
	        }
	    default:
	        {
	        break;
	        }
	    }
	
	
	}

//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::GetUnreadCountL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TInt CVIMPSTEngineIMSubService::GetUnreadCountL(const TDesC& aBuddyId ) 
	{
	TRACER_AUTO;
	TRACE( " ServiceId: %d BuddyId: %S", iServiceId, &aBuddyId );
							
	TInt count = 0;
	if( iIMCacheAccessor )
		{
		count = iIMCacheAccessor->GetUnreadMessageCountL(aBuddyId);
		TRACE( "iIMCacheAccessor returns with %d", count );
		}
	
	return count;
	}


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::SubServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineIMSubService::SubServiceState() const
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d ServiceState: %d",iServiceId, iServiceState );
	return iServiceState;
	}


	
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::IsConversationExistL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TBool CVIMPSTEngineIMSubService::IsConversationExistL(const TDesC& aBuddyId) const
	{
	TRACER_AUTO;
	TRACE( "Buddy Id : %S", &aBuddyId );
	
	TInt exist = EFalse;
	
	if( iIMCacheAccessor )
		{
		exist = iIMCacheAccessor->IsConversationExistL( aBuddyId );	
		TRACE( "iIMCacheAccessor returns with %d", exist );
		}
	
	TRACE( "Exist: %d", exist );	
	return exist; 
  	}

//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::CloseConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVIMPSTEngineIMSubService::CloseConversationL( const TDesC& aContactId )
    {
	TRACER_AUTO;
    TRACE( "Buddy Id : %S", &aContactId  );
    
    if( IsConversationExistL(aContactId) )
        {
    TRACE( "iIMCacheAccessor CloseConversationL to be called" );
        iIMCacheAccessor->CloseConversationL( aContactId );
    	TRACE( "iIMCacheAccessor CloseConversationL Done" );
        }
    
    }
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::GetOpenChatListL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
RArray<SIMCacheChatItem> CVIMPSTEngineIMSubService::GetOpenChatListL()
    {
	TRACER_AUTO;
    RArray<SIMCacheChatItem> item;
    if(iIMCacheAccessor)
        {
        item =  iIMCacheAccessor->GetChatListL( iServiceId );
        }
    return item;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::ResolveServiceStateL
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineIMSubService::ResolveServiceStateL(
										TCCHSubserviceState aState, 
            							TInt aServiceError )
    {
	TRACER_AUTO;
        
    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;       
    
    
    TRACE( "iServiceId: %d, ServiceState: %d", iServiceId, aState );  

    TBool handleServiceStates = ETrue;
    if ( aServiceError && ECCHDisabled != aState )
        {    
		//Only if the Service supports ALR, the state can goto WaitingForNetwork
		//Still API from CCH is required to know whether ALR is supported or not
        if ( (KCCHErrorInvalidSettings != aServiceError) && (ECCHConnecting == aState) )        
            {   
        TRACE("ESVCEWaitingForNetwork" );
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
                TRACE( "ESVCERegistered" );
                CreateIMCacheAccessorL();
                state = TVIMPSTEnums::ESVCERegistered;
                }
                break;

            case ECCHDisconnecting:      
                {
                TRACE( "ESVCENetworkDisConnecting" );          
                state = TVIMPSTEnums::ESVCENetworkDisConnecting;
                }
                break;

            case ECCHUninitialized:
            case ECCHDisabled:  
                {
                TRACE( "ESVCENotRegistered" );
                ReleaseIMCacheAccessor();
                state = TVIMPSTEnums::ESVCENotRegistered;
                }
                break;
                
            case ECCHConnecting:               
                {
                TRACE( "ESVCENetworkConnecting" );   
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
// CVIMPSTEngineIMSubService::Type
// ---------------------------------------------------------------------------
//
TVIMPSTEnums::SubServiceType CVIMPSTEngineIMSubService::Type() const	
	{
	TRACER_AUTO;
	TRACE("SubServiceType() Type : %d", iType );
	TRACE("CVIMPSTEngineIMSubService: [0x%x]", this );
	return iType;	
	}



// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::CchEventOccuredL
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineIMSubService::CchEventOccuredL( 
            TUint /*aServiceId*/, 
            TCCHSubserviceState aState, 
            TInt aServiceError )
	{
	TRACER_AUTO;
	TRACE( "TCCHSubserviceState : %d, ServiceErr: %d", aState, aServiceError );
	
    if ( aServiceError && ECCHDisabled != aState )
        {
        //we might even end up in waiting for connection state for all
        //those service which are ALR enabled
        //So better check here if you get any CCH errors
        iServiceState = ResolveServiceStateL( aState, aServiceError );
        iObserver.HandleServceConnectionEventL();
        DoHandleCchErrorL( aServiceError );
        }
    else
        {
        iServiceState = ResolveServiceStateL( aState, aServiceError );    
        iObserver.HandleServceConnectionEventL();
        }	
	
	
	}


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::DoHandleCchErrorL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineIMSubService::DoHandleCchErrorL( 
    TInt aServiceError )
    {
	TRACER_AUTO;
	TRACE( "ServiceErr: %d", aServiceError );
									
    switch ( aServiceError )
        {
        case KCCHErrorNetworkLost:
        case KCCHErrorBandwidthInsufficient:
        case KCCHErrorInvalidIap: 
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;

        case KCCHErrorAuthenticationFailed:
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;
            
        case KCCHErrorLoginFailed:
        case KCCHErrorServiceNotResponding:            
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;                    
        
        case KCCHErrorInvalidSettings:
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;
            
        case KCCHErrorAccessPointNotDefined:
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;
            
        default:
            {
            //Should be fwded to CCHUI to show the appropriate Notes
            //TBD
            }
            break;
        }
    
    
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::CreateIMCacheAccessorL()
// ---------------------------------------------------------------------------
//     
void CVIMPSTEngineIMSubService::CreateIMCacheAccessorL()
	{
	TRACER_AUTO;
	if (!iIMCacheFactory)
	    {
	    HBufC* ownData = iCchHandler.GetConParametersL(ECchUsername);//
	    CleanupStack::PushL(ownData);
	    TPtr ownDataPtr = ownData->Des();
	    TRACE( "ServiceErr: %S",&ownDataPtr );
	    // im cache factory
	    iIMCacheFactory = CIMCacheFactory::InstanceL(); 

	    // im cache update interface
	    if (!iIMCacheAccessor)
	        {				
	        iIMCacheAccessor = iIMCacheFactory->CreateAccessorL(iServiceId, *ownData );

	        iIMCacheAccessor->RegisterObserverL(*this);	
	        }

	    CleanupStack::PopAndDestroy(ownData);
	    }
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor()
// ---------------------------------------------------------------------------
//	
void CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor()	
	{
	TRACER_AUTO;
	
	if( iIMCacheAccessor )
	  	{
	  	iIMCacheAccessor->UnRegisterObserver(*this);		  	
	  	}
	  	
	if( iIMCacheFactory )
		{
		CIMCacheFactory::Release();  	
		iIMCacheAccessor = NULL;
		iIMCacheFactory = NULL;
		}
	
	}


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::NotifyObserversL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineIMSubService::NotifyObserversL( TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aBuddyId )
    {
	TRACER_AUTO;
    TInt count = iChatObserver.Count();
    for (TInt index=0; index<count; index++)		
        {
        iChatObserver[index]->HandleChatMessageEventL(aEventType ,aBuddyId );
        }	
    }

//  End of File

