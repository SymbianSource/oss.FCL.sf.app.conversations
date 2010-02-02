/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  single context observer implementation
*
*/

#include "cimcvenginecontextobserver.h"

#include <ximpidentity.h>
#include <ximpstatus.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <presentitypresenceevent.h>
#include <presentitygrouplistevent.h>
#include <presentitygroupinfo.h>
#include <presentitygroupcontentevent.h>
#include <presentitygroupmemberinfo.h>
#include <ximpcontextstate.h>
#include "imcvuiliterals.h"
#include "mimcvenginenewmessagehandler.h"
#include "imcvlogger.h"
#include <imerrors.hrh>
// ============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------
// CIMCVEngineContextObserver::NewL
// two phase construction
// ---------------------------------------------------------
CIMCVEngineContextObserver* CIMCVEngineContextObserver::NewL()
    {
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::NewL() start") );
    CIMCVEngineContextObserver* self = 
    					CIMCVEngineContextObserver::NewLC();
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::NewL() end") );
    return self;
    }

// ---------------------------------------------------------
// CIMCVEngineContextObserver::NewLC
// two phase construction
// ---------------------------------------------------------
CIMCVEngineContextObserver* CIMCVEngineContextObserver::NewLC()
    {
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::NewLC() start") );
    CIMCVEngineContextObserver* self = new 
    					(ELeave) CIMCVEngineContextObserver();
    CleanupStack::PushL( self );
    self->ConstructL();
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::NewLC() end") );
    return self;
    }

// ---------------------------------------------------------
// CIMCVEngineContextObserver::ConstructL
// two phase construction
// ---------------------------------------------------------    
void CIMCVEngineContextObserver::ConstructL()
	{
	}

// ---------------------------------------------------------
// CIMCVEngineContextObserver::~CIMCVEngineContextObserver
// destructor
// ---------------------------------------------------------
CIMCVEngineContextObserver::~CIMCVEngineContextObserver()
    {
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::~CIMCVEngineContextObserver() start") );
     if(iWait.IsStarted() )
	    {
	    iWait.AsyncStop();	
	    }
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::~CIMCVEngineContextObserver() end") );
    }

// ---------------------------------------------------------
// CIMCVEngineContextObserver::CIMCVEngineContextObserver
// ---------------------------------------------------------
CIMCVEngineContextObserver::CIMCVEngineContextObserver() :
iCurrentReqType(EReqUnknown)
    {
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::CIMCVEngineContextObserver() start") );
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::CIMCVEngineContextObserver() end") );
    }

// ===========================================================================
// FROM MPRFWximpCONTEXTOBSERVER
// ===========================================================================
//
// ---------------------------------------------------------
// CIMCVEngineContextObserver::HandleximpContextEvent
// ---------------------------------------------------------
void CIMCVEngineContextObserver::HandlePresenceContextEvent( 
    const MXIMPContext& /*aContext*/,
    const MXIMPBase& aEvent )
    {
  	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::HandlePresenceContextEvent() start") );
    const TInt32 eventType = aEvent.GetInterfaceId();
    
    switch( eventType )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
			{
            const MXIMPRequestCompleteEvent* event =
                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( aEvent, MXIMPBase::EPanicIfUnknown );

            TXIMPRequestId reqId = event->RequestId();
            
            const MXIMPStatus& status = event->CompletionResult();
			
            TInt err = status.ResultCode();
            //Login request has been issued            
            if ((iCurrentXIMPReq == reqId) && (ELoginReq == iCurrentReqType))
	            {
				//Login is an synchronous call	            
	            if ( iWait.IsStarted() )
					{
					iWait.AsyncStop();	
					}
				if(KErrNone == err )
	                {	                
	                iLoggedIn = ETrue;	                		                
	                }
                else
	                {	                
	                iLoggedIn = EFalse;	                
	                } 	            
				
				iCurrentReqType = EReqUnknown;	
				if(iSessionObserver)
					{
					TRAP_IGNORE( iSessionObserver->HandleConnectionEventL( ELoggedIn, KErrNone ) );  				
					}				                	
	            }
	        //Logout request has been issued    
			else if ((iCurrentXIMPReq == reqId) && (ELogoutReq == iCurrentReqType))  
				{
				//Login is an synchronous call	            
	            if ( iWait.IsStarted() )
					{
					iWait.AsyncStop();	
					}
				iLoggedIn = EFalse;	
				iCurrentReqType = EReqUnknown;	
				if(iSessionObserver)
					{
					TRAP_IGNORE( iSessionObserver->HandleConnectionEventL(ELoggedOut, KErrNone ) );  				
					}			
				}
			//SendMessage request has been issued	
			else
				{
				// for message delivery report
			    if( iSessionObserver && iLoggedIn )
				    {
				    TRAP_IGNORE( iSessionObserver->HandleDeliveryReportL(reqId, status.ResultCode() ) );	
				    }	
				}
            break;
            }
            
        case MXIMPContextStateEvent::KInterfaceId:
        	{        	
        	// what do we do with context state event
        	// state of the context can be associated with the 
        	const MXIMPContextStateEvent* ctxtStateEvent = 
            TXIMPGetInterface< const MXIMPContextStateEvent >::From( aEvent, 
                                                                     MXIMPBase::EPanicIfUnknown );
            const MXIMPStatus* fwStatus = ctxtStateEvent->StateChangeReason();
            const MXIMPContextState& cState = ctxtStateEvent->ContextState();
            MXIMPContextState::TState state = cState.ContextState();          
            
            if (MXIMPContextState::EInactive == state  )
				{
				//No request of Login/Logout has been issued by CV
				//Hence this might be a server initiated disconnect
				//handle properly
			 	if( (iSessionObserver) && (EReqUnknown == iCurrentReqType) )
					{
					TRAP_IGNORE( iSessionObserver->HandleConnectionEventL(EForcedLoggedOut, KImErrServiceForceClosedSession ) );  
					}
			
				}
             break;
        	}
       default:
        	{
        		
        	break;
			}
        }
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::HandlePresenceContextEvent() end") );
    }
    

// ---------------------------------------------------------
// CIMCVEngineContextObserver::GetCompletedReqResult
// 
// ---------------------------------------------------------
void CIMCVEngineContextObserver::GetConnectedSessionL(MXIMPContext& aContext, TInt aServiceId, TUid aProtocolUid)
	{
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::GetConnectedSessionL() start") );
	//TUid protocolUid = TUid::Uid( KProtocolUidTelepathy );
	if ( (!iLoggedIn) && (EReqUnknown == iCurrentReqType) )
	    {
	    //iOngoingOperation = TIMCVStateEnums::EIMCVLogging;
	    iCurrentXIMPReq = aContext.BindToL( aProtocolUid, aServiceId ) ;
	    iCurrentReqType = ELoginReq;

	    if( !iWait.IsStarted() )
	        {
	        // Code scanner warning "active object called without checking 
	        // whether it is active or cancelling it first" ignored because
	        // CActiveSchedulerWait is not an active object
	        iWait.Start(); // CSI: 10 # See above
	        }

	    //iOngoingOperation = TIMCVStateEnums::EIMCVUnknown;
	    }
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::GetConnectedSessionL() end") );
	
	}
// ---------------------------------------------------------
// CIMCVEngineContextObserver::GetCompletedReqResult
// 
// ---------------------------------------------------------
void CIMCVEngineContextObserver::ReleaseConnectionL(MXIMPContext& aContext)
	{
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::ReleaseConnectionL() start") );
	if ( (iLoggedIn) && (EReqUnknown == iCurrentReqType) )
	    {
	    //iOngoingOperation = TIMCVStateEnums::EIMCVLoggingOut;
	    iCurrentXIMPReq = aContext.UnbindL() ;
	    iCurrentReqType = ELogoutReq;
	    if( !iWait.IsStarted() )
	        {
	        // Code scanner warning "active object called without checking 
	        // whether it is active or cancelling it first" ignored because
	        // CActiveSchedulerWait is not an active object
	        iWait.Start(); // CSI: 10 # See above	
	        }
	    }
    IM_CV_LOGS(TXT("CIMCVEngineContextObserver::ReleaseConnectionL() end") );
   
	}
	
// ---------------------------------------------------------
// CIMCVEngineContextObserver::IsLoggedIn
// 
// ---------------------------------------------------------
TBool CIMCVEngineContextObserver::IsLoggedIn() const
	{
	return 	iLoggedIn;
	}
// ---------------------------------------------------------
// CIMCVEngineContextObserver::RegisterObserver
// 
// ---------------------------------------------------------	
void CIMCVEngineContextObserver::RegisterObserver(MIMCVEngineServiceSessionHandler* aObserver)  
	{
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::RegisterObserver() start") );
	iSessionObserver = 	aObserver;
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::RegisterObserver() end") );
	}
// ---------------------------------------------------------
// CIMCVEngineContextObserver::UnRegisterObserver
// 
// ---------------------------------------------------------		
void CIMCVEngineContextObserver::UnRegisterObserver(MIMCVEngineServiceSessionHandler* /*aObserver*/)  
	{
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::UnRegisterObserver() start") );
	// do not call delete 
	// not owned by this class
	iSessionObserver = NULL;
	IM_CV_LOGS(TXT("CIMCVEngineContextObserver::UnRegisterObserver() end") );
	}
// ---------------------------------------------------------
// CIMCVEngine::SetLoggedIn()
// ---------------------------------------------------------
//
void CIMCVEngineContextObserver::SetLoggedIn(TBool aState) 
	{
	iLoggedIn = aState;
	}	
		
// end of file

