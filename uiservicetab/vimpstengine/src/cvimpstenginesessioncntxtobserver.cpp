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
* Description: Implementation for CVIMPSTEngineSessionCntxtObserver
*
*/


#include "cvimpstenginesessioncntxtobserver.h"



#include "tvimpstenums.h"
#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"
#include "mvimpstenginesearchextentioneventobserver.h"
#include "mvimpstenginesessioncntxtobserver.h"
//ximpfw
#include <presencegrantrequestinfo.h>
#include <presentitygroupcontentevent.h>
#include <ximpcontext.h>
#include <presenceinfofilter.h>
#include <presencefeatures.h>
#include <presenceobjectfactory.h>
#include <presenceinfo.h>
#include <presenceauthorization.h>
#include <ximpidentity.h>
#include <ximpobjectfactory.h>
#include <searchinfo.h>
#include <searchevent.h>
#include <ximpclient.h>
#include <ximpidentity.h>
#include <ximpstatus.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <presentitypresenceevent.h>
#include <presenceblocklistevent.h>
#include <presencegrantrequestlistevent.h>
#include <ximpcontextstate.h>
#include <searchelement.h>
#include <searchkeysevent.h>
#include <searchkeyinfo.h>
#include <ximpfeatureinfo.h>

#include "vimpstdebugtrace.h"

//xmppsettings
#include "xmppparams.h"
#include "xmppservicesettingsapi.h"


// ============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::NewL
// two phase construction
// ---------------------------------------------------------
CVIMPSTEngineSessionCntxtObserver* CVIMPSTEngineSessionCntxtObserver::NewL( TUint32 aServiceId )
    {
    CVIMPSTEngineSessionCntxtObserver* self = 
    					CVIMPSTEngineSessionCntxtObserver::NewLC( aServiceId);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::NewLC
// two phase construction
// ---------------------------------------------------------
CVIMPSTEngineSessionCntxtObserver* CVIMPSTEngineSessionCntxtObserver::NewLC(TUint32 aServiceId)
    {
    CVIMPSTEngineSessionCntxtObserver* self = new 
    					(ELeave) CVIMPSTEngineSessionCntxtObserver( aServiceId);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::ConstructL
// two phase construction
// ---------------------------------------------------------    
void CVIMPSTEngineSessionCntxtObserver::ConstructL()
	{
	iRequestMapper = CVIMPSTEngineRequestMapper::NewL();	
	iReqResult = KErrNone;
	iClient = MXIMPClient::NewClientL();               
    //Create new sink to receive ximp context events
	iPresenceCtx = iClient->NewPresenceContextLC();
    // ignore code scanner warning, it gives panic
    CleanupStack::Pop();// because of LC method    
	
	iFeatures = MPresenceFeatures::NewL(iPresenceCtx);
	
	//Stores the XIMPFw EventTypes Subscribed for      
	iAcceptedEventTypes.Reset();
    iAcceptedEventTypes.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MXIMPContextStateEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MPresentityGroupContentEvent::KInterfaceId );   
    iAcceptedEventTypes.AppendL( MPresenceGrantRequestListEvent::KInterfaceId );
	iAcceptedEventTypes.AppendL( MSearchEvent::KInterfaceId );   
    iAcceptedEventTypes.AppendL( MSearchKeysEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MPresenceBlockListEvent::KInterfaceId );
	    
	TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
    
	// register this to prsence context   
	iPresenceCtx->RegisterObserverL( *this, &eventFilterArray );
	 
	}

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::~CVIMPSTEngineSessionCntxtObserver
// destructor
// ---------------------------------------------------------
CVIMPSTEngineSessionCntxtObserver::~CVIMPSTEngineSessionCntxtObserver()
    {
    
    iObserverArray.Reset();
    iObserverArray.Close();
    
    iAcceptedEventTypes.Reset();
    iAcceptedEventTypes.Close();
    delete iFeatures;
    
    if(iPresenceCtx)
        {
        iPresenceCtx->UnregisterObserver( *this );
        delete  iPresenceCtx;   
        }
    
    delete iClient;           
    delete iRequestMapper;
    }

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::CVIMPSTEngineSessionCntxtObserver
// ---------------------------------------------------------
CVIMPSTEngineSessionCntxtObserver::CVIMPSTEngineSessionCntxtObserver(TUint32 aServiceId)
:iServiceId(aServiceId),
 iBindStatus( TVIMPSTEnums::EVIMPSTBindNotDone )
    {
    }

// ===========================================================================
// FROM MPRFWximpCONTEXTOBSERVER
// ===========================================================================
//
// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::HandlePresenceContextEvent
// ---------------------------------------------------------
void CVIMPSTEngineSessionCntxtObserver::HandlePresenceContextEvent( 
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("InsideCallback::HandlePresenceContextEvent start"));
    TRAP_IGNORE(DoHandlePresenceContextEventL( aContext, aEvent ));
   	TRACE( T_LIT("InsideCallback::HandlePresenceContextEvent end"));	
    }    

	
// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::GetCompletedReqResult
// 
// ---------------------------------------------------------
TInt CVIMPSTEngineSessionCntxtObserver::GetCompletedReqResult() const
	{
	return iReqResult;	
	}

// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::CVIMPSTEngineSessionCntxtObserver
// 
// ---------------------------------------------------------
CVIMPSTEngineRequestMapper* CVIMPSTEngineSessionCntxtObserver::
											GetRequestMapper() const
	{
	return iRequestMapper;	
	}


// ---------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::ContextBindStatus
// 
// ---------------------------------------------------------
TVIMPSTEnums::TVIMPSTBindStatus CVIMPSTEngineSessionCntxtObserver::ContextBindStatus()
	{
	return iBindStatus;	
	}
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::ServerBindL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineSessionCntxtObserver::ServerBindL(TUid aProtocolUid)
    {
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerBindL start"));
    TInt err = KErrNotFound;
    //Bind context to desired presence service
    if ( TVIMPSTEnums::EVIMPSTBindDone != iBindStatus )
        {  
        TXIMPRequestId operationId = TXIMPRequestId::Null();
        TRAP( err, operationId = iPresenceCtx->BindToL( 
                aProtocolUid, iServiceId ) );
        if ( err )
            {
            // XIMP queues all operations except bind operation. If there is
            // unbind operation pending in ximp side, bind call will leave
            // with KErrAlreadyExists. Client side is not ideal place to 
            // handle this type of logic so to work around this problem, this 
            // case is flagged here. When unbind completes, rebind is done. 
            // If unbind is called between these operations, flag is set 
            // ESVCEPresenceBindNotDone to avoid bind if service was disabled
            TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerBindL err: %d"), err);
            iBindStatus = TVIMPSTEnums::EVIMPSTBindFailureMode;
            }
        else
            {
            //create the requestmapper for the corresponidng reuqest id.
            iRequestMapper->CreateRequestL(operationId,ETrue,EVIMPSTXimpOperationBind);
            iBindStatus = TVIMPSTEnums::EVIMPSTBindDone; 
            iFeatureSupported = EVIMPSTFeatureUnknown;
            TRAP(err,IdentifySupportedFeaturesL());
            }
        }
    else
        {
        err = KErrNone;
        }
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerBindL end"));
    return err;
    }
// ---------------------------------------------------------------------------
// CVIMPSTEngineVoIPPrecenseHandler::ServerUnBindL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineSessionCntxtObserver::ServerUnBindL( 
    TBool aDoUnsubscribe )
    {
	TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerUnBind start"));
	TInt err = KErrNotFound;
	if ( TVIMPSTEnums::EVIMPSTBindDone == iBindStatus )
		{
		if ( aDoUnsubscribe )
			{
			iBindStatus = TVIMPSTEnums::EVIMPSTUnBinding;
			}
		
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerUnBind - do unbind"));
		TXIMPRequestId operationId = TXIMPRequestId::Null(); 
		TRAP( err, operationId = iPresenceCtx->UnbindL());
		iBindStatus = TVIMPSTEnums::EVIMPSTUnBindWaiting;
		iRequestMapper->CreateRequestL(operationId,EFalse,EVIMPSTXimpOperationUnBind) ;
		iBindStatus = TVIMPSTEnums::EVIMPSTBindNotDone;
		iFeatureSupported = EVIMPSTFeatureUnknown;
		}
	else if ( TVIMPSTEnums::EVIMPSTBindFailureMode == iBindStatus )
		{
		// Disable flag here. If service is really disabled, we must not
		// rebind when unbind completes.
		iBindStatus = TVIMPSTEnums::EVIMPSTBindNotDone;
		}
	TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::ServerUnBind end"));
	return err;
    } 


// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::UriFromXimpOperationLC
// ---------------------------------------------------------------------------
//
HBufC* CVIMPSTEngineSessionCntxtObserver::UriFromXimpOperationLC(const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::UriFromXimpOperationLC start"));
    const MPresentityPresenceEvent& event =
            *TXIMPGetInterface< const MPresentityPresenceEvent >::From( 
                aEvent, MXIMPBase::EPanicIfUnknown );  
    HBufC* retValue = NULL;
    retValue = event.PresentityId().Identity().AllocLC();
    if(!retValue)
        {
        retValue = KNullDesC().AllocLC();
        }
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::UriFromXimpOperationLC end"));
    return retValue;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::DoHandlePresenceContextEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineSessionCntxtObserver::DoHandlePresenceContextEventL(
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::DoHandlePresenceContextEventL start"));
    const TInt32 eventId = aEvent.GetInterfaceId();
        
        switch( aEvent.GetInterfaceId() )
            {
            case MXIMPRequestCompleteEvent::KInterfaceId:
                {
                
                TRACE( T_LIT("InsideCallbackswitch::MXIMPRequestCompleteEvent start"));
                const MXIMPRequestCompleteEvent* event =
                    TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
                        aEvent, MXIMPBase::EPanicIfUnknown );
                
                iReqResult = event->CompletionResult().ResultCode();
                const TXIMPRequestId& reqId = event->RequestId();
                // Note:FindRequestId does not pass the ownership. hanece req
                // should not be deleted.
                CVIMPSTEngineRequest *req = iRequestMapper->FindRequestId( reqId );                
                
                if ( req )
                    {                                        
                    TXimpOperation operation = req->RequestType(); 
                    
                    //contact mgmt & search will remove their own requests
                    //presence subservice should also do the same 
                    //untill then use the below if check 
                    //TBD
                    if ( (operation>EVIMPSTXimpOperationNoOperation)&&
                    		(operation<=EVIMPSTXimpOperationUnsubscribe) )
	                    {
	                    req->StopWait() ;   
	                    iRequestMapper->RemoveRequestId(reqId);              
	                    }

                    TInt count = iObserverArray.Count();
                    
                    for(TInt i = 0; i < count; i++)
	                    {
	                    iObserverArray[i]->HandleSessionContextEventL(aContext,
	                    						aEvent,
	                    						operation);
	                    }
                    }
                TRACE( T_LIT("InsideCallbackswitch::MXIMPRequestCompleteEvent end"));    
                break;
                }

            case MXIMPContextStateEvent::KInterfaceId:
                {
                TRACE( T_LIT("InsideCallbackswitch::MXIMPContextStateEvent"));                
                break;
                }
            case MPresentityPresenceEvent::KInterfaceId:
                {
                TRACE( T_LIT("InsideCallbackswitch::::MPresentityPresenceEvent start"));
                TInt count = iObserverArray.Count();
                for(TInt i = 0; i < count; i++)
                    {
                    iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                    }
                TRACE( T_LIT("InsideCallbackswitch::::MPresentityPresenceEvent end"));
                break;
                }
            case MPresentityGroupContentEvent::KInterfaceId:
               {
               TRACE( T_LIT("InsideCallbackswitch::::MPresentityGroupContentEvent start"));
               TInt count = iObserverArray.Count();
               for(TInt i = 0; i < count; i++)
                  {
                  iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                  }
               TRACE( T_LIT("InsideCallbackswitch::::MPresentityGroupContentEvent end"));
               break;  
               }
            case MPresenceGrantRequestListEvent::KInterfaceId:
               {
               TRACE( T_LIT("InsideCallbackswitch::::MPresentityGroupContentEvent start"));
               TInt count = iObserverArray.Count();
               for(TInt i = 0; i < count; i++)
                  {
                  iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                  }               
               TRACE( T_LIT("InsideCallbackswitch::::MPresentityGroupContentEvent end"));
               break;
               }
           
            case MSearchEvent::KInterfaceId:
                {
                TRACE( T_LIT("InsideCallbackswitch::::MSearchEvent start"));

                TInt count = iObserverArray.Count();
                for(TInt i = 0; i < count; i++)
                   {
                   iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                   } 
                TRACE( T_LIT("InsideCallbackswitch::::MSearchEvent end"));

                break;
                }
            case MSearchKeysEvent::KInterfaceId:
                {
                TRACE( T_LIT("InsideCallbackswitch::::MSearchKeysEvent start"));
                TInt count = iObserverArray.Count();
                for(TInt i = 0; i < count; i++)
                   {
                   iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                   } 
                TRACE( T_LIT("InsideCallbackswitch::::MSearchKeysEvent end"));

                break;
                }
            case MPresenceBlockListEvent::KInterfaceId:
               {
               TRACE( T_LIT("InsideCallbackswitch::::MPresenceBlockListEvent start"));
               TInt count = iObserverArray.Count();
               for(TInt i = 0; i < count; i++)
                  {
                  iObserverArray[i]->HandleSessionContextEventL(aContext,aEvent);
                  } 
               TRACE( T_LIT("InsideCallbackswitch::::MPresenceBlockListEvent end"));
               break;
               }
              
            default:
                {
                break;
                }
            }
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::DoHandlePresenceContextEventL end"));
    }
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::XimpAuthorizationL
// ---------------------------------------------------------------------------
// 
MPresenceAuthorization& 
CVIMPSTEngineSessionCntxtObserver::XimpAuthorizationL()
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ));
    __ASSERT_ALWAYS( &iFeatures->PresenceAuthorization(), 
        User::Leave( KErrNotSupported ));
    return iFeatures->PresenceAuthorization();
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::XimpPresenceWatchingL
// ---------------------------------------------------------------------------
//
MPresenceWatching& 
CVIMPSTEngineSessionCntxtObserver::XimpPresenceWatchingL()
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ));
    __ASSERT_ALWAYS( &iFeatures->PresenceWatching(), 
        User::Leave( KErrNotSupported ));
    return iFeatures->PresenceWatching();
    }    
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::XimpPresentityGroupsL
// ---------------------------------------------------------------------------
//
MPresentityGroups& 
CVIMPSTEngineSessionCntxtObserver::XimpPresentityGroupsL()
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ));
    __ASSERT_ALWAYS( &iFeatures->PresentityGroups(), 
        User::Leave( KErrNotSupported ));
    return iFeatures->PresentityGroups();
    }        
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::XimpPresencePublishingL
// ---------------------------------------------------------------------------
//
MPresencePublishing& 
CVIMPSTEngineSessionCntxtObserver::XimpPresencePublishingL()
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ));
    __ASSERT_ALWAYS( &iFeatures->PresencePublishing(), 
        User::Leave( KErrNotSupported ));
    return iFeatures->PresencePublishing();
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::XimpPresenceContext
// ---------------------------------------------------------------------------
//
MXIMPContext& 
CVIMPSTEngineSessionCntxtObserver::XimpPresenceContextL()
    {
    __ASSERT_ALWAYS( iPresenceCtx, User::Leave( KErrNotFound ));
     return *iPresenceCtx;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::PresenceObjectFactory
// ---------------------------------------------------------------------------
//
MPresenceObjectFactory& CVIMPSTEngineSessionCntxtObserver::PresenceObjectFactoryL() const
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotFound ));
    return iFeatures->PresenceObjectFactory();
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::PresenceFeaturesL
// ---------------------------------------------------------------------------
//
MPresenceFeatures& CVIMPSTEngineSessionCntxtObserver::PresenceFeaturesL() const
    {
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotFound ));
    return *iFeatures;
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::RegisterObserver
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineSessionCntxtObserver::RegisterObserver(MVIMPSTEngineSessionCntxtObserver* aObserver)
    {
    if (aObserver)
	    {    	
	    
	    TInt index = iObserverArray.Find(aObserver);
        if( index == KErrNotFound )
            {
            iObserverArray.Append( aObserver );   
            }
	    }
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::UnRegisterObserver
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineSessionCntxtObserver::UnRegisterObserver(MVIMPSTEngineSessionCntxtObserver* aObserver)
    {
    if (aObserver)
	    {    
	    TInt index = iObserverArray.Find(aObserver);
	        
	    if( index >=0 )
	        {
	        iObserverArray.Remove( index );
	        iObserverArray.Compress();
	        }
	    }    
   }

// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::IdentifySupportedFeaturesL
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineSessionCntxtObserver::IdentifySupportedFeaturesL()
    {
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::IdentifySupportedFeaturesL start"));
    using namespace NXIMPFeature::Presence;
    using namespace NXIMPFeature::InstantMessage;
    using namespace NXIMPFeature::Search;
    using namespace NXIMPFeature::PresentityGroups;
    
   	MXIMPFeatureInfo* ximpCtxFeats = iPresenceCtx->GetContextFeaturesLC();
    //get the supproted features from ximpfw adaptation.
    if( ximpCtxFeats )
        {        
        CleanupStack::Pop() ; //iXimpctxFeat 
        
        const MDesC8Array& supportedFeatures = ximpCtxFeats->FeatureIds();
        
        TInt count  = supportedFeatures.MdcaCount();
				
		for(TInt index = 0; index < count; index++)
		    {
		    TPtrC8 feature = supportedFeatures.MdcaPoint(index);
		    
		    if ( !(iFeatureSupported & EVIMPSTFeaturePublish) && !(KPublish().Compare(feature)) )
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeaturePublish;
			    
			    }
		    else if ( !(iFeatureSupported & EVIMPSTFeatureFetch) && !(KFetch().Compare(feature)) )
		        {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureFetch;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureSubscribe) && !(KSubscribe().Compare(feature)) )			    
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureSubscribe;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureUnsubscribe) && !(KUnsubscribe().Compare(feature)) )			    
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureUnsubscribe;
			    
			    }    
			else if ( !(iFeatureSupported & EVIMPSTFeatureAddContact) && !(KAddContact().Compare(feature)) )			    
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureAddContact;
			    
			    }	
			else if ( !(iFeatureSupported & EVIMPSTFeatureDeleteContact) && !(KDeleteContact().Compare(feature)) )			    
       		    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureDeleteContact;
			    
			    } 
			else if ( !(iFeatureSupported & EVIMPSTFeatureBlock) && !(KBlock().Compare(feature)) )			    
        	    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureBlock;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureUnBlock) && !(KUnBlock().Compare(feature)) )			    
        	    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureUnBlock;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureAvatar) && !(KAvatar().Compare(feature)) )			    
        	    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureAvatar;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureCreateGroup) && !(KCreate().Compare(feature)) )			    
        	    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureCreateGroup;
			    
			    }    
			else if ( !(iFeatureSupported & EVIMPSTFeatureDeleteGroup) && !(KDelete().Compare(feature)) )			    
    		    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureDeleteGroup;
			    
			    }
			else if ( !(iFeatureSupported & EVIMPSTFeatureInstantMessage) && !(KInstantMessage().Compare(feature)) )			    			
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureInstantMessage;
			    
			    }  
			else if ( !(iFeatureSupported & EVIMPSTFeatureSearch) && !(KSearch().Compare(feature)) )			    			
			    {
			    iFeatureSupported = iFeatureSupported | EVIMPSTFeatureSearch;
			    
			    }			      
    
		    }
    
    	delete ximpCtxFeats;
        }
    TRACE( T_LIT("CVIMPSTEngineSessionCntxtObserver::IdentifySupportedFeaturesL end"));
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineSessionCntxtObserver::GetSupportedFeatures
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineSessionCntxtObserver::GetSupportedFeatures()
	{
	return 	iFeatureSupported;
	}    

// end of file


