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

#include "vimpstdebugtrace.h"

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
    TRACE( T_LIT("CVIMPSTEngineIMSubService::NewL start"));
    TRACE( T_LIT("NewL() ServiceId: %d"), aServiceId );
    
    CVIMPSTEngineIMSubService* self = CVIMPSTEngineIMSubService::NewLC(
    												aServiceId,
    												aCchHandler,
    												aTableFetcher,    												
    												aObserver );
    CleanupStack::Pop( self );
    
	TRACE( T_LIT("CVIMPSTEngineIMSubService::NewL end"));
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
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::NewLC start"));
	TRACE( T_LIT("NewLC() ServiceId: %d"), aServiceId );
	
    CVIMPSTEngineIMSubService* self = new (ELeave) CVIMPSTEngineIMSubService(aServiceId, 
    												aCchHandler, aTableFetcher,
    												aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    TRACE( T_LIT("CVIMPSTEngineIMSubService::NewLC end"));
    return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService
// 
// ---------------------------------------------------------

CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService()
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService start"));
	
	iChatObserver.Reset();
    iChatObserver.Close();
    
	iCchHandler.UnRegisterCchObserver(ECCHIMSub);
		
	ReleaseIMCacheAccessor();	
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::~CVIMPSTEngineIMSubService end"));
 	}


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTEngineIMSubService::ConstructL()
	{			
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::ConstructL start"));
	TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCchHandler.GetServiceState( 
        			iServiceId, ECCHIMSub, serviceState );
	
	iServiceState = ResolveServiceStateL(serviceState, error);    	
	
	iCchHandler.RegisterCchObserverL(this,ECCHIMSub);       
	
    TRACE( T_LIT("ConstructL() ResolveServiceStateL returned ServiceState: %d"), 
							iServiceState );
 
    TRACE( T_LIT("CVIMPSTEngineIMSubService::ConstructL end"));
    
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
	TRACE( T_LIT("CVIMPSTEngineIMSubService::RegisterChatObserver start"));	
	if (aObserver)
		{
		TInt index = iChatObserver.Find(aObserver);
		        if( index == KErrNotFound )
		            {
		            iChatObserver.Append( aObserver );   
		            }
		}    	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::RegisterChatObserver end"));
	
	}

// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::UnRegisterChatObserver
// 
// ---------------------------------------------------------	

void CVIMPSTEngineIMSubService::UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver)
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::UnRegisterChatObserver start"));
	
	if (aObserver)
		{
		
		TInt index = iChatObserver.Find(aObserver);
		        
		        if( index >=0 )
		            {
		            iChatObserver.Remove( index );
		            iChatObserver.Compress();
		            }
		        
	    
		}
	TRACE( T_LIT("CVIMPSTEngineIMSubService::UnRegisterChatObserver end"));
			
	}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::HandleIMCacheEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------  

void CVIMPSTEngineIMSubService::HandleIMCacheEventL(TIMCacheEventType aEventType, TAny* aChatMessage  )
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::HandleIMCacheEventL start"));
	TRACE( T_LIT("HandleIMCacheEventL() TIMCacheEventType: %d"), aEventType );
							
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
	            TRACE( T_LIT("HandleIMCacheEventL() EIMCacheUnreadMessage for %S"), 
	                    &buddyId );

	            NotifyObserversL(TVIMPSTEnums::EIMUnreadMessage,buddyId);               
	            }
	        break;	
	        }
	    case EIMCacheUnreadChange :
	        {
	        TRACE( T_LIT("HandleIMCacheEventL() EIMCacheUnreadChange") );
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
	        TRACE( T_LIT("HandleIMCacheEventL() EIMCacheChatClosed for %S"), 
	                &buddyId );
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
	        TRACE( T_LIT("HandleIMCacheEventL() EIMCacheChatClosed for %S"), 
	                &buddyId );
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
	        TRACE( T_LIT("HandleIMCacheEventL() EIMCacheChatClosed for %S"), 
	                &buddyId );
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
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::HandleIMCacheEventL end"));
	
	}

//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::GetUnreadCountL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TInt CVIMPSTEngineIMSubService::GetUnreadCountL(const TDesC& aBuddyId ) 
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::GetUnreadCountL start"));
	TRACE( T_LIT("GetUnreadCountL() ServiceId: %d BuddyId: %S"), 
							iServiceId, &aBuddyId );
							
	TInt count = 0;
	if( iIMCacheAccessor )
		{
		count = iIMCacheAccessor->GetUnreadMessageCountL(aBuddyId);
		TRACE( T_LIT("GetUnreadCountL() iIMCacheAccessor returns with %d"), count );
		}
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::GetUnreadCountL end"));
	return count;
	}


// ---------------------------------------------------------
// CVIMPSTEngineIMSubService::SubServiceState
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineIMSubService::SubServiceState() const
	{
	TRACE( T_LIT("SubServiceState() ServiceId: %d ServiceState: %d"), 
							iServiceId, iServiceState );
	return iServiceState;
	}


	
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::IsConversationExistL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TBool CVIMPSTEngineIMSubService::IsConversationExistL(const TDesC& aBuddyId) const
	{
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::IsConversationExistL start"));
	TRACE( T_LIT("IsConversationExistL() Buddy Id : %S"), &aBuddyId );
	
	TInt exist = EFalse;
	
	if( iIMCacheAccessor )
		{
		exist = iIMCacheAccessor->IsConversationExistL( aBuddyId );	
		TRACE( T_LIT("IsConversationExistL() iIMCacheAccessor returns with %d"), exist );
		}
	
	TRACE( T_LIT("IsConversationExistL() Exist: %d"), exist );	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::IsConversationExistL end"));		
	return exist; 
  	}

//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::CloseConversationL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CVIMPSTEngineIMSubService::CloseConversationL( const TDesC& aContactId )
    {
    TRACE( T_LIT("CVIMPSTEngineIMSubService::CloseConversationL start"));
    TRACE( T_LIT("CloseConversationL() Buddy Id : %S"), &aContactId  );
    
    if( IsConversationExistL(aContactId) )
        {
        TRACE( T_LIT("CloseConversationL() iIMCacheAccessor CloseConversationL to be called") );
        iIMCacheAccessor->CloseConversationL( aContactId );
       	TRACE( T_LIT("CloseConversationL() iIMCacheAccessor CloseConversationL Done") );
        }
    
    TRACE( T_LIT("CVIMPSTEngineIMSubService::CloseConversationL end"));
    }
//-----------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::GetOpenChatListL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
RArray<SIMCacheChatItem> CVIMPSTEngineIMSubService::GetOpenChatListL()
    {
    TRACE( T_LIT("CloseConversationL() iIMCacheAccessor GetOpenChatListL called") );
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
    
    TRACE( T_LIT("CVIMPSTEngineIMSubService::ResolveServiceStateL start"));    
        
    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;       
    
    TRACE( T_LIT("ResolveServiceStateL() iServiceId: %d, ServiceState: %d"), 
    							iServiceId, aState );    	

    TBool handleServiceStates = ETrue;
    if ( aServiceError && ECCHDisabled != aState )
        {    
		//Only if the Service supports ALR, the state can goto WaitingForNetwork
		//Still API from CCH is required to know whether ALR is supported or not
        if ( (KCCHErrorInvalidSettings != aServiceError) && (ECCHConnecting == aState) )        
            {   
            TRACE( T_LIT("ResolveServiceStateL() ESVCEWaitingForNetwork") );
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
                TRACE( T_LIT("ResolveServiceStateL() ESVCERegistered") );
                CreateIMCacheAccessorL();
                state = TVIMPSTEnums::ESVCERegistered;
                }
                break;

            case ECCHDisconnecting:      
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCENetworkDisConnecting") );                
                state = TVIMPSTEnums::ESVCENetworkDisConnecting;
                }
                break;

            case ECCHUninitialized:
            case ECCHDisabled:  
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCENotRegistered") );
                ReleaseIMCacheAccessor();
                state = TVIMPSTEnums::ESVCENotRegistered;
                }
                break;
                
            case ECCHConnecting:               
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCENetworkConnecting") );               
                state = TVIMPSTEnums::ESVCENetworkConnecting;
                }
                break;
            
            default:
                break;
            }
        }        
        
	TRACE( T_LIT("CVIMPSTEngineIMSubService::ResolveServiceStateL end"));
	        
    return state;  
	
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::Type
// ---------------------------------------------------------------------------
//
TVIMPSTEnums::SubServiceType CVIMPSTEngineIMSubService::Type() const	
	{
	TRACE( T_LIT("SubServiceType() Type : %d"), iType );
	TRACE( T_LIT("CVIMPSTEngineIMSubService: [0x%x]"), this );		    	
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
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::CchEventOccuredL start"));
	TRACE( T_LIT("CchEventOccuredL() TCCHSubserviceState : %d, ServiceErr: %d"), 
									aState, aServiceError );
	
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
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::CchEventOccuredL end"));
	
	}


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::DoHandleCchErrorL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineIMSubService::DoHandleCchErrorL( 
    TInt aServiceError )
    {
    
    TRACE( T_LIT("CVIMPSTEngineIMSubService::DoHandleCchErrorL start"));
    TRACE( T_LIT("DoHandleCchErrorL() ServiceErr: %d"), 
									aServiceError );
									
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
    
    TRACE( T_LIT("CVIMPSTEngineIMSubService::DoHandleCchErrorL end"));
    
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::CreateIMCacheAccessorL()
// ---------------------------------------------------------------------------
//     
void CVIMPSTEngineIMSubService::CreateIMCacheAccessorL()
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::CreateIMCacheAccessorL start"));
	
	if (!iIMCacheFactory)
	    {
	    HBufC* ownData = iCchHandler.GetConParametersL(ECchUsername);//
	    CleanupStack::PushL(ownData);
	    TPtr ownDataPtr = ownData->Des();
	    TRACE( T_LIT("DoHandleCchErrorL() ServiceErr: %S"), 
	            &ownDataPtr );
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
	TRACE( T_LIT("CVIMPSTEngineIMSubService::CreateIMCacheAccessorL end"));
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor()
// ---------------------------------------------------------------------------
//	
void CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor()	
	{
	TRACE( T_LIT("CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor start"));
	
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
	
	TRACE( T_LIT("CVIMPSTEngineIMSubService::ReleaseIMCacheAccessor end"));
	}


// ---------------------------------------------------------------------------
// CVIMPSTEngineIMSubService::NotifyObserversL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineIMSubService::NotifyObserversL( TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aBuddyId )
    {
    TRACE( T_LIT("CVIMPSTEngineIMSubService::NotifyObserversL start"));
    TInt count = iChatObserver.Count();
    for (TInt index=0; index<count; index++)		
        {
        iChatObserver[index]->HandleChatMessageEventL(aEventType ,aBuddyId );
        }	
    TRACE( T_LIT("CVIMPSTEngineIMSubService::NotifyObserversL end"));
    }

//  End of File

