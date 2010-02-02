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
* Description:  connection service manager for im
*
*/


#include "cicpservicemanager.h"	   	// service manager

#include <e32std.h>


#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximprequestcompleteevent.h>
#include <ximpcontextstateevent.h>
#include <ximpidentity.h>
#include <ximpstatus.h>
#include <ximpcontextstateevent.h>
#include <ximpcontextstate.h>
#include <ximperrors.hrh>

#include <imconversationevent.h> // recieve message
#include <imconversationinfo.h>

#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spdefinitions.h>

#include "icplogger.h"
#include "cicpservicerequest.h"

// imcache related includes
#include    <cimcachefactory.h>
#include	<mimcacheupdater.h>

//xmppsettings
#include "xmppparams.h"
#include "xmppservicesettingsapi.h"


/************** Consts ***************/
const TInt KPropertyMaxLength = 512;
const TInt KPasswordMaxLength = 50;
//Received message maximum length is 400 character if received message is longer than 400 character
//Truncate it to 400 character
//This must always be in sync with imcvuiapp KReceiveMsgMaxLength
const TInt KReceiveMsgMaxLength = 400;  
 _LIT(KAt, "@");
 _LIT(KPercentage, "%");
/***********End of Consts**********************/



// -----------------------------------------------------------------------------
// CIcpServiceManager::CIcpServiceManager()
// -----------------------------------------------------------------------------
//
CIcpServiceManager::CIcpServiceManager( MCchServiceObserver& aServiceObserver ) :
    iServiceObserver( aServiceObserver ),
    iOngoingState( ECCHUninitialized )
	{
    ICPLOGSTRING( "CIcpServiceManager::CIcpServiceManager");
	}

// -----------------------------------------------------------------------------
// CIcpServiceManager::NewL
// -----------------------------------------------------------------------------
//
CIcpServiceManager* CIcpServiceManager::NewL( MCchServiceObserver& aServiceObserver )
	{
    ICPLOGSTRING( "CIcpServiceManager::NewL");

    CIcpServiceManager* self = new ( ELeave ) CIcpServiceManager( aServiceObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

// -----------------------------------------------------------------------------
// CIcpServiceManager::ConstructL
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::ConstructL()
    {
    ICPLOGSTRING( "CIcpServiceManager::ConstructL");
    iCSPSetting = CSPSettings::NewL();
    iXmppParameters = CXmppSettingsApi::NewL();
    
	iOwnUserId = HBufC::NewL(MAX_LENGTH);
    
    if ( iPresClient )
        {
        delete iPresClient;
        iPresClient = NULL;
        }
    
    //XIMP Context creation    
    iPresClient = MXIMPClient::NewClientL();
    MXIMPContext* tmp = iPresClient->NewPresenceContextLC();
    iPresenceCtx = tmp;
    CleanupStack::Pop( ); // iPresenceCtx
    
	iAcceptedEventTypes.Reset();
    iAcceptedEventTypes.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MXIMPContextStateEvent::KInterfaceId );       
    iAcceptedEventTypes.AppendL( MImConversationEvent::KInterfaceId );
    
    
	TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
   	// register this to prsence context   
    iPresenceCtx->RegisterObserverL( *this, &eventFilterArray );       
    
    tmp = NULL;                                                                      
    
    ICPLOGSTRING( "CIcpServiceManager::ConstructL Done");  
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::~CIcpServiceManager
// -----------------------------------------------------------------------------
//
CIcpServiceManager::~CIcpServiceManager()
	{	
    ICPLOGSTRING( "CIcpServiceManager::~CIcpServiceManager"); 
    
    delete iXmppParameters;
    delete iOwnUserId; 
    
    delete iCSPSetting;
    
    iAcceptedEventTypes.Reset();
	iAcceptedEventTypes.Close();
    
    iReqIDArray.ResetAndDestroy();
    
    delete iPresenceCtx;
    delete iPresClient;
    
    if(iIMCacheFactory)
	    {
	    CIMCacheFactory::Release();	
	    }
	
	if (iServiceName)
		{
		delete iServiceName;
		iServiceName = NULL;	
		}
	}


// -----------------------------------------------------------------------------
// CIcpServiceManager::EnableServiceL
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::EnableServiceL( TUint aServiceId,
                                         TCCHSubserviceType aSubServiceType )
    {
    ICPLOGSTRING3( "CIcpServiceManager::EnableServiceL service:%i Type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
	
	if ( (iOngoingState != ECCHEnabled) && 
				(iOngoingState != ECCHConnecting) )
		{
		//Get the service
		iServiceId = aServiceId;
		
		if (iServiceName)
			{
			delete iServiceName;
			iServiceName = NULL;	
			}

		iServiceName = HBufC::NewL(KPropertyMaxLength);
		TPtr serviceIdPtr( iServiceName->Des() );    
		
		GetServiceNameL(iServiceId, serviceIdPtr);
	    
	    TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);
	    
	    //reset owndata
	    delete iOwnUserId;    
	    iOwnUserId = NULL;
	    iOwnUserId = HBufC::NewL(MAX_LENGTH);
		TPtr owndataPtr = iOwnUserId->Des();
	   	
	    //get the owndata from settingsapi
	    CXmppSettingsApi* xmppParameters = CXmppSettingsApi::NewLC();    	
		xmppParameters->GetParamL(settingsId, XmppParams::KXmppParamUsername(), owndataPtr );	
		CleanupStack::PopAndDestroy(); // xmppParameters

		TInt passwordSet = KErrNotFound;
		GetConnectionParameter(aServiceId,aSubServiceType,ECchPasswordSet,passwordSet);
		if(0 ==  iOwnUserId->Length() || !passwordSet)
		    {
		    // check the user name if its empty or leave with the KCCHErrorAuthenticationFailed
            // for the cch ui to show the user name and password query dialog.
            User::Leave(KCCHErrorAuthenticationFailed);
		    }
		
		//bind to presence context
		BindL(iServiceId);	
		iOngoingState = ECCHConnecting; 
		}

	
    ICPLOGSTRING( "CIcpServiceManager::EnableServiceL out" );
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::EnableServiceL
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::EnableServiceL( TUint aServiceId,
                                         TCCHSubserviceType aSubServiceType,
                                         TUint /*aIapId*/ )
    {
    ICPLOGSTRING3( "CIcpServiceManager::EnableServiceL service:%i Type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
    EnableServiceL( aServiceId, aSubServiceType );
	
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::DisableServiceL
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::DisableServiceL( TUint aServiceId,
                                          TCCHSubserviceType aSubServiceType  )
    {
    ICPLOGSTRING3( "CIcpServiceManager::DisableService service:%i Type:%i", aServiceId, aSubServiceType );
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );

	
	// just unbind the context from the oss protocol adaptation plugin
	if (iOngoingState == ECCHEnabled)	
		{
		UnBindL();   
		iOngoingState = ECCHDisconnecting;  
		}
         
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::IsAvailableL
// -----------------------------------------------------------------------------
//
TBool CIcpServiceManager::IsAvailableL( TUint /*aServiceId*/,
                                        TCCHSubserviceType /*a*/,
                                        const RArray<TUint32>& /*aIapIdArray*/ ) const
    {
    ICPLOGSTRING( "CIcpServiceManager::IsAvailableL" );

    return ETrue;
    }
 
// -----------------------------------------------------------------------------
// CIcpServiceManager::GetServiceState
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::GetServiceState( TUint aServiceId,
                                          TCCHSubserviceType aSubServiceType , 
                                          TCCHSubserviceState& aState ) const
    {
    ICPLOGSTRING3( "CIcpServiceManager::GetServiceState service:%i type:%i", aServiceId, aSubServiceType );

	__ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
    TInt result( KErrNone );
    
    aState = iOngoingState;        
    result = iLastReportedError;
    return result;
    }
    
// -----------------------------------------------------------------------------
// CIcpServiceManager::GetServiceNetworkInfo
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::GetServiceNetworkInfoL( TUint aServiceId,
                                                TCCHSubserviceType aSubServiceType,
                                                TUint32& aSnapId, 
                                                TUint32& aIapId,
                                                TBool& aSnapLocked,
                                                TBool& aPasswordSet) const
    {
    ICPLOGSTRING3( "CIcpServiceManager::GetServiceNetworkInfo service:%i type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
	
	//Please dont remove the below line
    //otheriwse enable service will not be called
    //assuming service is down. Hack
    aSnapId = 0;
    aIapId = 5;
    aSnapLocked = ETrue;    
    // read from setting see, if it is availabe set 1 else 0
    TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);            
    //get the owndata from settingsapi
    HBufC* password = HBufC::NewLC( KPasswordMaxLength );
    TPtr  passwordPtr( password->Des() );
   
    iXmppParameters->GetParamL((TUint32)settingsId, XmppParams::KXmppParamPassword(), passwordPtr );
    
    if( passwordPtr.Length() > 0 )
        {
        aPasswordSet = ETrue;
        }
    else
        {
        aPasswordSet = EFalse;
        }  
    CleanupStack::PopAndDestroy();// password 
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CIcpServiceManager::SetSnapId
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::SetSnapId( TUint /*aServiceId*/,
                                    TCCHSubserviceType /*aSubServiceType*/,
                                    const TUint /*aSnapId*/ )
    {    
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::SetIapId
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::SetIapId( TUint /*aServiceId*/,
                                   TCCHSubserviceType /*aSubServiceType*/,
                                   TUint /*aIapId*/ )
    {
    return KErrNotSupported;
    }
 

// -----------------------------------------------------------------------------
// CIcpServiceManager::GetServiceInfoL
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::GetServiceInfoL( TUint /*aServiceId*/,
                                          TCCHSubserviceType /*aSubServiceType*/,
                                          RBuf& /*aBuffer*/ ) const
    {    
    }


// -----------------------------------------------------------------------------
// CIcpServiceManager::ReserveService
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::SetServiceReserved( TBool /*aReserved*/,
                                             TUint /*aServiceId*/,
                                             TCCHSubserviceType /*aSubServiceType*/ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// CIcpServiceManager::GetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::GetConnectionParameter(TUint aServiceId,
                         TCCHSubserviceType aSubServiceType, 
                         TCchConnectionParameter aParameter,
                         TInt& aValue ) const
    {
    ICPLOGSTRING3( "CIcpServiceManager::GetConnectionParameter service:%i type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
    TInt error = KErrBadHandle;
     
    switch( aParameter )
	    {
	    case ECchIapId:
		    {
		    aValue = iIapId;
		    error = KErrNone;
		    break;	
		    }
		case ECchSnapId:
		    {
		    aValue = iSnapId;
		    error = KErrNone;
		    break;	
		    }
		case ECchSnapLocked:
		    {
		    aValue = ETrue;
		    error = KErrNone;
		    break;	
		    } 
		case ECchPasswordSet:
		    {
		    // dont return ETrue simply, check if password length is greater than 0,
		    // only then return ETrue, else return EFalse.
		    TBuf<MAX_LENGTH> password;
		    TInt settingsId = KErrNotFound;
		    TRAPD(err,settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId));               
           //get the owndata from settingsapi
		   if(KErrNone == err)
		       {
	           TRAP_IGNORE(iXmppParameters->GetParamL((TUint32)settingsId, 
	                                               XmppParams::KXmppParamPassword(), password ));
		       }
           if( 0 == password.Length())
               {
               aValue = EFalse;
               }
           else 
               {
               aValue = ETrue;
               }
		    break;	
		    } 
		default :
			{
			break;	
			}  
		 
	    }
    
   	return error;
  
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::GetConnectionParameter
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::GetConnectionParameterL(TUint aServiceId,
                         				TCCHSubserviceType aSubServiceType,  
                                        TCchConnectionParameter aParameter,
                                        RBuf& aValue  ) const
    {
    
    ICPLOGSTRING3( "CIcpServiceManager::GetConnectionParameterL service:%i type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
    TInt error = KErrBadHandle;
     switch( aParameter )
	    {
	   	case ECchUsername:
		    {
    	    TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);
       		TRAP(error,iXmppParameters->GetParamL((TUint32)settingsId, 
       											XmppParams::KXmppParamUsername(), aValue ));
		    break;	
		    }
	   	case ECchPassword:
            {
            TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);       		
            //get the owndata from settingsapi
            TRAP(error,iXmppParameters->GetParamL((TUint32)settingsId, 
            									XmppParams::KXmppParamPassword(), aValue ));
            break;  
            }
		default :
			{
			break;	
			}  
		 
	    }
    
    }

// -----------------------------------------------------------------------------
// CIcpPlugin::SetConnectionParameter
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::SetConnectionParameter( TUint aServiceId,
                        					TCCHSubserviceType aSubServiceType,  
                                            TCchConnectionParameter aParameter,
                                            TInt aValue )
    {
    
    ICPLOGSTRING3( "CIcpServiceManager::SetConnectionParameter service:%i type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
    TInt error = KErrBadHandle;
	 switch( aParameter )
	    {
	  	case ECchIapId:
		    {
		    iIapId = aValue ;
		    error = KErrNone;
		    break;	
		    }
		case ECchSnapId:
		    {
		    iSnapId = aValue ;
		    error = KErrNone;
		    break;	
		    }
	   default :
			{
			break;	
			}  
		 
	    }
    return error;	
    }

// -----------------------------------------------------------------------------
// CIcpServiceManager::SetConnectionParameter
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::SetConnectionParameterL(TUint aServiceId,
                         					TCCHSubserviceType aSubServiceType,  
                                            TCchConnectionParameter aParameter,
                                            const TDesC& aValue )
    {
    
    ICPLOGSTRING3( "CIcpServiceManager::SetConnectionParameterL service:%i type:%i", aServiceId, aSubServiceType );
    
    __ASSERT_DEBUG( ValidateSubService(aSubServiceType), User::Panic( KNullDesC, KErrNotFound ) );    
    __ASSERT_DEBUG( aServiceId > 0, User::Panic( KNullDesC, KErrNotFound ) );
    
     switch( aParameter )
	    {
	   	case ECchUsername:
		    {
            HBufC* userid = NULL;
            TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);             
            if(KErrNotFound == aValue.Find( KAt() ) )
                {
                //ownUserIdPtr.Append(KAt());
                HBufC* domainName = HBufC::NewLC(50);
                TPtr domainNamePtr = domainName->Des();
                iXmppParameters->GetParamL((TUint32)settingsId, XmppParams::KXmppParamDomain(), domainNamePtr);                
                CleanupStack::Pop();//domainName
                userid = HBufC::NewLC( aValue.Length() + domainName->Length() + KAt().Length() );
                TPtr useridPtr = userid->Des();
                useridPtr.Append(aValue);
                useridPtr.Append(KAt);
                useridPtr.Append(domainNamePtr);
                delete domainName;
                }
            else
                {
                userid = aValue.AllocLC();
                }
            TChar at('@');
            TInt loc = userid->Locate( at );
            if ( loc<2 )
                {
                // There must be at least @ + something in domain part
                User::Leave( KErrArgument );
                }
            if(iOwnUserId)
                {
                delete iOwnUserId;
                iOwnUserId = NULL;
                }
            iOwnUserId = userid->AllocL();
            iXmppParameters->SetParamL((TUint32)settingsId, 
            								XmppParams::KXmppParamUsername(), *userid);
            CleanupStack::PopAndDestroy();//userid
		    break;	
		    }
	   	case ECchPassword:
            {
            TInt settingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId); 
            iXmppParameters->SetParamL((TUint32)settingsId, 
            								XmppParams::KXmppParamPassword(), aValue);
            break;  
            }
		default :
			{
			break;	
			}  
		 
	    }
    
   
    }
 
 
// -----------------------------------------------------------------------------
// CIcpServiceManager::IsReserved
// -----------------------------------------------------------------------------
//
TInt CIcpServiceManager::IsReserved( TUint aServiceId,
                                     TCCHSubserviceType aSubServiceType ) const
    {
    ICPLOGSTRING3( "CIcpServiceManager::IsReserved service: %d type: %d",
                   aServiceId, aSubServiceType );

    TBool result = EFalse;

    return result;
    }



// -----------------------------------------------------------------------------
// CIcpServiceManager::
// -----------------------------------------------------------------------------
//      
void  CIcpServiceManager::BindL(TInt aServiceSettingId)
	{
	ICPLOGSTRING2( "CIcpServiceManager[0x%x]::BindL", this );
	//TUid protocolUid = TUid::Uid( KProtocolUid );
	TInt propertyId  = GetSPSettingsIntPropertyL( aServiceSettingId, EPropertyPCSPluginId );

	if ( KErrNotFound != propertyId )
		{
	    TUid protocolUid = TUid::Uid( propertyId );
	    	
		TXIMPRequestId reqId = iPresenceCtx->BindToL( protocolUid, 
												aServiceSettingId );			
		CreateRequestL(reqId, TIMConnProviderEnums::ELoginRequest);
		
		}

	ICPLOGSTRING( "CIcpServiceManager::BindL end" );
	}
	   
// -----------------------------------------------------------------------------
// CIcpServiceManager::UnBindL
// -----------------------------------------------------------------------------
//      
void  CIcpServiceManager::UnBindL()
	{
	ICPLOGSTRING2( "CIcpServiceManager[0x%x]::UnBindL", this );

	TXIMPRequestId reqId = iPresenceCtx->UnbindL();
	
	CreateRequestL(reqId, TIMConnProviderEnums::ELogoutRequest);
	
	ICPLOGSTRING( "CIcpServiceManager::UnBindL end" );
	}  


// -----------------------------------------------------------------------------
// CIcpServiceManager::ProfileEventOccurred
// -----------------------------------------------------------------------------
//
void CIcpServiceManager::HandlePresenceContextEvent( const MXIMPContext& /* aContext */,
                                     const MXIMPBase& aEvent )
	{
	ICPLOGSTRING2( "CIcpServiceManager[0x%x]::HandlePresenceContextEvent", this );

    const TInt32 eventType = aEvent.GetInterfaceId();
    
    switch( eventType )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
            {
            ICPLOGSTRING( "::HandlePresenceContextEvent event MximpRequestCompleteEvent start" );
            const MXIMPRequestCompleteEvent* event =
                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( aEvent, MXIMPBase::EPanicIfUnknown );

            TXIMPRequestId reqId = event->RequestId();
            
            const MXIMPStatus& status = event->CompletionResult();
            
            TInt err = status.ResultCode();
            
			CICPServiceRequest *req = FindRequestId( reqId );
			
            if( req->RequestType() == TIMConnProviderEnums::ELoginRequest )
                {
                if(KErrNone == err )
	                {
		                //login successfull
	                ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent event bind complete" );
	                
	                iOngoingState = ECCHEnabled;
	                
	                iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHPresenceSub ),
	                								 ECCHEnabled, err ); 
	                								 
	                iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHIMSub ),
	                								 ECCHEnabled, err );                       
	               
	               	
	                // create the imcache session ,since now ready to recieve message
	               if(!iIMCacheFactory) 
						{
						
						TRAP_IGNORE(
									iIMCacheFactory = CIMCacheFactory::InstanceL() ;
									iIMCacheUpdater = iIMCacheFactory->CreateUpdaterL(iServiceId, iOwnUserId->Des() ,EFalse) ;
									);
						}
	                
	                }
                else
	                {
	                TRAP_IGNORE( SetIMDisabledL(iServiceId) );
	                
	                TInt cchErr = ConvertXIMPErrToCChErr(err);
	                iLastReportedError = cchErr;
	                //login unsuccessfull
	                ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent event bind complete" );	                
	                
					iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHPresenceSub ),
	                								 ECCHDisabled, cchErr );
	                								 
	                iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHIMSub ),
	                								 ECCHDisabled, cchErr );                       
	                
	                iOngoingState = ECCHDisabled;
	                ICPLOGSTRING2( "CIcpServiceManager::HandlePresenceContextEvent err = %d",err); 
	                
		
	                }               
               
                }
            else if( req->RequestType() == TIMConnProviderEnums::ELogoutRequest )
                {
                //Logout is successfull
                ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent status offline" ); 
                
                
                
               	// release the imcache, all the conversation will lost here 
     			if( iIMCacheFactory ) 
					{
					CIMCacheFactory::Release();
					iIMCacheFactory = NULL; // make sure factory is deleted
					iIMCacheUpdater = NULL;	
					}
			
				// set the status into cenrep
				
                ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent status offline end" );
                }
                
              ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent offline end 2" );
            
            req->StopWait() ;	
			RemoveRequestId(reqId);
			delete req;						
            
            break;
            }
            
        case MXIMPContextStateEvent::KInterfaceId:
        	{
        	ICPLOGSTRING( "::HandlePresenceContextEvent event MXIMPContextStateEvent start" );
        	// what do we do with context state event
        	// state of the context can be associated with the 
        	const MXIMPContextStateEvent* ctxtStateEvent = 
            TXIMPGetInterface< const MXIMPContextStateEvent >::From( aEvent, 
                                                                     MXIMPBase::EPanicIfUnknown );
            const MXIMPStatus* fwStatus = ctxtStateEvent->StateChangeReason();
            const MXIMPContextState& cState = ctxtStateEvent->ContextState();
            MXIMPContextState::TState state = cState.ContextState();          
            
            if (MXIMPContextState::EBinding == state)
	            {
	            
	            iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHPresenceSub ),
	                								 ECCHConnecting, KErrNone ); 
	                								 
	             iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHIMSub ),
	                								 ECCHConnecting, KErrNone ); 	
	            }
            else if (MXIMPContextState::EUnbinding == state)
	            {
	            
	            iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHPresenceSub ),
	                								 ECCHDisconnecting, KErrNone );                 								 
	            
	             iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHIMSub ),
	                								 ECCHDisconnecting, KErrNone ); 	
	            }
	        else if (MXIMPContextState::EInactive == state  )
				{
			 	// release the imcache, all the conversation will lost here  
				CIMCacheFactory::Release();
				iIMCacheFactory = NULL; // make sure factory is deleted
				iIMCacheUpdater = NULL;
				iOngoingState = ECCHDisabled;				
				
				iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHPresenceSub ),
                								 ECCHDisabled, KErrNone );
                								 
                iServiceObserver.ServiceStateChanged( TServiceSelection( iServiceId, ECCHIMSub ),
                								 ECCHDisabled, KErrNone ); 	
				
				}
            
            break;
        	}
                
        case MImConversationEvent::KInterfaceId:
            {
            ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent new message recieved star" );
            const MImConversationEvent* convEvent = 
            TXIMPGetInterface< const MImConversationEvent >::From
            (aEvent, MXIMPBase::EPanicIfUnknown);
            if( iIMCacheUpdater )
                {
                TInt count = convEvent->NewTextMessageCount();
                const MImConversationInfo& convInfo = convEvent->NewTextMessage(0);
                const MXIMPIdentity& identity = convInfo.MessageId();
                const TDesC16& msgText = convInfo.TextMessage();
                ICPLOGSTRING2( "msg txt: %S", &msgText );
                TInt len = msgText.Length();
                HBufC* bigmsg = NULL;
                TRAP_IGNORE( bigmsg = HBufC::NewL(KReceiveMsgMaxLength + 1) ; // 1 for  %
                if( bigmsg )
                    {
                    CleanupStack::PushL(bigmsg) ; // bigmsg
                    TPtr bigmsgPtr = bigmsg->Des();
                    //If message is more than 400 character take only first 400 character
                    //Rest of the message will be lost and 
                    bigmsgPtr.Append( msgText.Left( KReceiveMsgMaxLength ) ); 
                    if( len > KReceiveMsgMaxLength)
                        {
                        //append % as 401st character to identify on UI that it is a 
                        //long message which is truncated        
                        bigmsgPtr.Append(KPercentage); 
                        }
                    TPtrC buddyId = identity.Identity();
                    iIMCacheUpdater->AppendReceiveMessageL( buddyId, bigmsgPtr ) ;
                    ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent Cache updated" );
                    CleanupStack::PopAndDestroy(bigmsg); // bigmsg          
                    }   ); //End TRAP_IGNORE( bigmsg ...
                }
            ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent new message recieved end" );
            break;  
            }

        
        default:
            {
            break;
            }
        }
        ICPLOGSTRING( "CIcpServiceManager::HandlePresenceContextEvent end" );
	}
	
	
// ---------------------------------------------------------
// CIcpServiceManager::FindRequestId
// 
// ---------------------------------------------------------		
CICPServiceRequest* CIcpServiceManager::CreateRequestL(TXIMPRequestId& aRequestId,
							TIMConnProviderEnums::TRequestTypes aType )
	{	

	CICPServiceRequest* req = CICPServiceRequest::NewL(aRequestId, aType);
	CleanupStack::PushL(req);
	
	TInt status( iReqIDArray.Append( req ) );
	User::LeaveIfError( status );		
	
	CleanupStack::Pop(req);
	
	return req;
	}
	
	
// ---------------------------------------------------------
// CIcpServiceManager::FindRequestId
// 
// ---------------------------------------------------------		
CICPServiceRequest* CIcpServiceManager::FindRequestId(TXIMPRequestId& aRequestId )
	{
	TInt count = iReqIDArray.Count() ;
	TXIMPRequestId reqId ;
	CICPServiceRequest *req = NULL;
	for (TInt i=0; i < count; i++)
		{
		req = iReqIDArray[i] ;
		
		reqId = req->GetRequestId() ;
		
		if( aRequestId == reqId )			
			{
			break ;
			}
		}
	return req ;
	}

// ---------------------------------------------------------
// CIcpServiceManager::RemoveRequestId
// 
// ---------------------------------------------------------		
CICPServiceRequest* CIcpServiceManager::RemoveRequestId(TXIMPRequestId& aRequestId )
	{
	TInt count = iReqIDArray.Count() ;
	CICPServiceRequest *req = NULL;
	for (TInt i=0; i < count; i++)
		{
		req = iReqIDArray[i] ;
		if(req->GetRequestId() == aRequestId)			
			{
			iReqIDArray.Remove( i );
			iReqIDArray.Compress();
			break ;
			}
		}
		return req ;
	}


// ---------------------------------------------------------------------------
// CIcpServiceManager::SetIMDisabledL
// ---------------------------------------------------------------------------
//
void CIcpServiceManager::SetIMDisabledL(
                                TUint32 aServiceId)
    {
    ICPLOGSTRING2( "CIcpServiceManager::SetIMDisabledL serviceId = %d",aServiceId); 
    
    
    {
    //Disable IM Subservice
    TSPItemType propertyType( EItemTypeIMSubProperty );
    TServicePropertyName propertyName( ESubPropertyIMEnabled );
    RPropertyArray subproperties;
    CleanupClosePushL( subproperties );        
     
    // Find correct subservice
    TInt err( iCSPSetting->FindSubServicePropertiesL( 
        aServiceId, propertyType, subproperties ) );
    
    for ( TInt i( 0 ); i < subproperties.Count(); i++ )
        {
        // Read subservice's On/Off information 
        if ( propertyName == subproperties[ i ]->GetName() )
            {
            TOnOff onOff( EOff );                
            ICPLOGSTRING2( "CIcpServiceManager::SetIMDisabledL subproperties[ i ]->SetValue err = %d",err);
            subproperties[ i ]->SetValue( onOff );            
            i = subproperties.Count();               
            }
        }    
    iCSPSetting->AddOrUpdatePropertiesL( aServiceId, subproperties ) ;
    subproperties.ResetAndDestroy( );
    CleanupStack::PopAndDestroy( &subproperties );
    }

	{
    TSPItemType propertyType( EItemTypePresenceSubProperty );
    TServicePropertyName propertyName( ESubPropertyPresenceEnabled );
    RPropertyArray subproperties;
    CleanupClosePushL( subproperties );        
     
    // Find correct subservice
    TInt err( iCSPSetting->FindSubServicePropertiesL( 
        aServiceId, propertyType, subproperties ) );
    
    for ( TInt i( 0 ); i < subproperties.Count(); i++ )
        {
        // Read subservice's On/Off information 
        if ( propertyName == subproperties[ i ]->GetName() )
            {
            TOnOff onOff( EOff );                
            ICPLOGSTRING2( "CIcpServiceManager::SetIMDisabledL subproperties[ i ]->SetValue err = %d",err);
            subproperties[ i ]->SetValue( onOff );            
            i = subproperties.Count();               
            }
        }    
    iCSPSetting->AddOrUpdatePropertiesL( aServiceId, subproperties ) ;
    subproperties.ResetAndDestroy( );
    CleanupStack::PopAndDestroy( &subproperties );
	}
	
	
	}
	

// ---------------------------------------------------------------------------
// CIcpServiceManager::ValidateServiceL
// ---------------------------------------------------------------------------
//
TBool CIcpServiceManager::ValidateServiceL( TUint32 aServiceId ) const
	{
	
	TInt presenceSettingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyPresenceSettingsId);
	TInt imSettingsId = GetSPSettingsIntPropertyL(aServiceId, ESubPropertyIMSettingsId);
	
	if ( (KErrNotFound != presenceSettingsId) && (presenceSettingsId == imSettingsId) )
		{
		return ETrue;
		}

	return EFalse;
	}
	
// ---------------------------------------------------------------------------
// CIcpServiceManager::GetSPSettingsIntPropertyL
// ---------------------------------------------------------------------------
//
TInt CIcpServiceManager::GetSPSettingsIntPropertyL( TUint32 aServiceId, 
											  TServicePropertyName aPropertyName ) const
	{
	TInt tIntProperty = KErrNotFound; 
	
    CSPProperty* property = CSPProperty::NewLC();
    
    iCSPSetting->FindPropertyL( aServiceId,
                                aPropertyName,
                                *property ); 	
	
	if ( property )
        {
        property->GetValue( tIntProperty );
        }
    CleanupStack::PopAndDestroy( property );    
    
    return tIntProperty;    
	}
	
// ---------------------------------------------------------------------------
// CIcpServiceManager::ValidateServiceL
// ---------------------------------------------------------------------------
//
TBool CIcpServiceManager::ValidateSubService( TCCHSubserviceType aSubServiceType ) const
	{
	if ( (ECCHUnknown == aSubServiceType) || 
					(ECCHPresenceSub == aSubServiceType) || 
						(ECCHIMSub == aSubServiceType) )
		{
		return ETrue;	
		}
	return EFalse;
	}	
	

// ---------------------------------------------------------------------------
// CIcpServiceManager::GetServiceNameL()
// ---------------------------------------------------------------------------
//
void CIcpServiceManager::GetServiceNameL( TUint32 aServiceId, TDes& aServiceName )
    {
    
    CSPEntry* entry = CSPEntry::NewLC();
    TRAPD( err, iCSPSetting->FindEntryL( aServiceId, *entry ) );
    if ( !err )
        {
        aServiceName = entry->GetServiceName();
        }
    
    CleanupStack::PopAndDestroy( entry );    
    
    }


// ---------------------------------------------------------------------------
// CIcpServiceManager::ConvertXIMPErrToCChErr()
// ---------------------------------------------------------------------------
//    
TInt CIcpServiceManager::ConvertXIMPErrToCChErr(const TInt aXimpErr)
	{

	TInt returnValue (KErrNone);
	
	switch (aXimpErr)
		{
		case KXIMPErrServiceGeneralError: 
		case KXIMPErrServiceShutdown:		
		case KXIMPErrServiceRequestTypeNotSupported:
			{
			returnValue = KCCHErrorLoginFailed;
			break;
			}

		case KXIMPErrServiceAuthenticationFailed:
			{
			returnValue = KCCHErrorAuthenticationFailed;
			break;         
			}		

		default:
			{
			break;	
			}
		}
	
	return 	returnValue;	
	}
	
            		
// End of file
