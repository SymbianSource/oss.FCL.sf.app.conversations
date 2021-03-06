/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles the use of Precense Framework
 *
*/


// INCLUDE FILES
/*#include <ximpclient.h>*/
#include <ximpcontext.h>
#include <ximpobjectfactory.h>
#include <presenceobjectfactory.h>
#include <ximpidentity.h> //for MXIMPIdentity
#include <presentitygroups.h>
#include <presencewatching.h> //for MximpPresenceWatching
#include <ximpstatus.h> //Presence info test
#include <presentitygroupcontentevent.h>
#include <presentitygroupmemberinfo.h>
#include <presenceauthorization.h>
#include <presentitypresenceevent.h>
#include <presencegrantrequestlistevent.h> // Grant request list
#include <presencegrantrequestinfo.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <presenceblocklistevent.h>
#include <presenceblockinfo.h>


#include <presencefeatures.h>
#include <presenceinfofilter.h> //info filtter
#include <presenceinfofield.h> //MximpPresenceInfoField
#include <presenceinfofieldcollection.h> //MximpPresenceInfoFieldCollection
#include <presenceinfofieldvaluetext.h> //MximpPresenceInfoFieldValueText
#include <presenceinfofieldvalueenum.h>
#include <personpresenceinfo.h> // MximpPersonPresenceInfo
#include <presencepublishing.h>//MximpPresencePublishing
#include <ximperrors.hrh> //ximp errors
#include <presenceinfofieldvaluebinary.h>
//presence cache headers 
#include <presencecachereader2.h> // cache reader


#include "cvimpstenginepresencesubservice.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststoragecontact.h"
#include "mvimpststoragecontactlist.h"
#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginecchhandler.h"
#include "mvimpststorageserviceview.h"
#include "tvimpstconsts.h"
#include "cvimpstengineservicetablefetcher.h"
#include "cvimpstenginecchhandler.h"
#include "cvimpstenginesessioncntxtobserver.h"
#include "vimpstutilsnotemapper.h"
#include "vimpstallerrors.h"
#include "cvimpstenginerequest.h"
#include "cvimpstblockedlistmanager.h"
#include "mvimpstengineblockedlistfetcheventobserver.h"

#include   <ximpfeatureinfo.h>
#include <avabilitytext.h>

//Presence Observer
#include "mvimpstenginepresencesubserviceeventobserver.h"
#include "vimpstdebugtrace.h"
#include "vimpstcustomcleanupapi.h" //For customized cleanup function
#include "mvimpstengineserviceconnectioneventobserver.h"

// CONTANTS
const TInt KUriMaxLength = 255; 
_LIT( KListNameAllBuddy ,"buddylist" );
const TInt KCollationLevel = 1;

// Compares alphabetically using MVIMPSTStorageContact::Identification and 
// TDesC::CompareC
TInt CompareAlphabetically( const TPtrC& aFirst, 
                            const TPtrC& aSecond )
    {
    return aFirst.CompareC( aSecond, KCollationLevel, NULL );
    }


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      CVIMPSTEnginePresenceSubService()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService::CVIMPSTEnginePresenceSubService( TUint32 aServiceId, 
        CVIMPSTEngineCchHandler& aCchHandler,
        CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
        MVIMPSTEngineServiceConnectionEventObserver& aObserver ) 
:iServiceId( aServiceId ),
iCchHandler(aCchHandler),
iSettingsTableFetcher(aTableFetcher),
iXimpEventObserver(aXimpEventObserver),
    iObserver(aObserver),
    iChangeStatusSupported ( ETrue ),
    iChangeStatusMsgSupported( ETrue ),
    iAvatarSupported( EFalse ),
    iIsClearingAvatar(EFalse)
        {
        TRACE( T_LIT("CVIMPSTEnginePrecenseSubService::CVIMPSTEnginePrecenseSubService start")); 
        TRACE( T_LIT("CVIMPSTEnginePrecenseSubService::CVIMPSTEnginePrecenseSubService end"));
        }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::ConstructL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::ConstructL()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::ConstructL start")); 

    iCchHandler.RegisterCchObserverL(this,ECCHPresenceSub);

    TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCchHandler.GetServiceState( 
            iServiceId, ECCHPresenceSub, serviceState );

    iServiceState = ResolveServiceStateL(serviceState, error);   
    //initialize the presence cache.
    iPresenceCacheReader = MPresenceCacheReader2::CreateReaderL();                                                        

    iPresenceCacheReader->SetObserverForSubscribedNotifications(this);
    iServiceName = HBufC::NewL( KVIMPSTUISPSMaxPropertyLength );
    TPtr serviceNamePtr( iServiceName->Des() );    
    iSettingsTableFetcher.GetServiceNameL(iServiceId, serviceNamePtr);      
    iBlockedListMgr = CVIMPSTBlockedListManager::NewL();
    iBlockListFetchReqPending = EFalse; //req of fetching blocked list has been completed.
    
	iLogoutRequest = EFalse;
	iSubscribeToAuthList = EFalse;
	iAutoAccept = EFalse;
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::ConstructL end")); 

    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService* 
CVIMPSTEnginePresenceSubService::NewL( TUint32 aServiceId, 
        CVIMPSTEngineCchHandler& aCchHandler,
        CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
        MVIMPSTEngineServiceConnectionEventObserver& aObserver )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewL start")); 
    CVIMPSTEnginePresenceSubService* self = NewLC( aServiceId,aCchHandler, aTableFetcher, 
            aXimpEventObserver,aObserver );
    CleanupStack::Pop(self);
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewL end"));
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService* 
CVIMPSTEnginePresenceSubService::NewLC( TUint32 aServiceId, 
        CVIMPSTEngineCchHandler& aCchHandler,
        CVIMPSTEngineServiceTableFetcher& aTableFetcher,
        CVIMPSTEngineSessionCntxtObserver& aXimpEventObserver,
        MVIMPSTEngineServiceConnectionEventObserver& aObserver )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewLC start")); 
    CVIMPSTEnginePresenceSubService* self =
    new (ELeave) CVIMPSTEnginePresenceSubService( aServiceId,aCchHandler, aTableFetcher,  aXimpEventObserver,aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewLC end")); 
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      ~CVIMPSTEnginePresenceSubService()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService::~CVIMPSTEnginePresenceSubService()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewLC start")); 
	
	iCchHandler.UnRegisterCchObserver(ECCHPresenceSub);
    
    delete iServiceName;
    delete iPresenceCacheReader ; 
    
    delete iBlockedListMgr;
    iBlockedListMgr = NULL;
       
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::NewLC end")); 
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      ServiceState()
// ---------------------------------------------------------------------------
//

TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEnginePresenceSubService::SubServiceState() const
	{
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService::ServiceState start")); 
	return iServiceState;
	}

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      Type()
// ---------------------------------------------------------------------------
//
TVIMPSTEnums::SubServiceType CVIMPSTEnginePresenceSubService::Type() const
	{
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService::Type") ); 
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService: [0x%x]"), this );		    	
	return TVIMPSTEnums::EPresence;
	}

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      Enabled()
// ---------------------------------------------------------------------------
//

TBool CVIMPSTEnginePresenceSubService::Enabled()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::Enabled start")); 
    TBool ret = EFalse;
    if( TVIMPSTEnums::ESVCERegistered == iServiceState)
        {
        ret = ETrue;
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::Enabled end")); 
    return ret;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::RetrieveSubscribedListL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::RetrieveSubscribedListL()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RetrieveSubscribedListL start")); 

    //Create group id
    TBuf<KUriMaxLength> buffer( KListNameAllBuddy );
    MXIMPIdentity* groupList = iXimpEventObserver.XimpPresenceContextL().ObjectFactory().NewIdentityLC();
     __ASSERT_ALWAYS( groupList , User::Leave( KErrNoMemory ) );		
    groupList->SetIdentityL( buffer );
    iIsFetchingContact = ETrue;
    //Subscribe buddy list
    // do get subscribe list"));
    TXIMPRequestId operationId = TXIMPRequestId::Null();
    operationId = iXimpEventObserver.XimpPresentityGroupsL().SubscribePresentityGroupContentL( 
            *groupList );
    CVIMPSTEngineRequestMapper* requestMapper =iXimpEventObserver.GetRequestMapper();
    requestMapper->CreateRequestL(operationId,EFalse,EVIMPSTXimpOperationGetSubscribedList);
    CleanupStack::PopAndDestroy(); // groupList
    // list retrieving ok. Waiting for list.;
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RetrieveSubscribedListL end")); 
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::SubscribePresenceOfSingleContactL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::SubscribePresenceOfSingleContactL( const TDesC& aUriOfTheContact)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SubscribePrecenseOfSingleContactL start"));
     __ASSERT_ALWAYS( aUriOfTheContact.Length(), User::Leave( KErrArgument ) );		
     //if anything is there with colon eg sip:user@presence1. strip the part before :
    TInt len = aUriOfTheContact.Find(_L(":"));
    TPtrC buddyId = aUriOfTheContact.Right( aUriOfTheContact.Length() - len - KColon().Length());
    HBufC* name = HBufC::NewLC( KPropertyMaxLength );
    TPtr namePtr( name->Des() );
    namePtr.Zero();

    // append the service name followed by user id ,seperated by colon
    namePtr.Append(*iServiceName);
    namePtr.Append(KColon);
    namePtr.Append(buddyId);
    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SubscribeToPresenceCacheL: %S"), &namePtr );
    iPresenceCacheReader->SubscribePresenceBuddyChangeL(*name);
    CleanupStack::PopAndDestroy(name); //  name    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SubscribePrecenseOfSingleContactL end"));  
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      UnsubscribePrecenseOfSingleContactL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::UnSubscribePresenceOfSingleContactL(
        const TDesC& aUriOfTheContact )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribePrecenseOfSingleContactL start"));   
    HBufC* name = HBufC::NewLC( KPropertyMaxLength );
    TPtr namePtr( name->Des() );
    namePtr.Zero();
    // append the service name followed by user id ,seperated by colon
    namePtr.Append(*iServiceName);
    namePtr.Append(KColon);
    namePtr.Append(aUriOfTheContact);
    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnSubscribeToPresenceCacheL: %S"), &namePtr);
    iPresenceCacheReader->UnSubscribePresenceBuddyChangeL(*name);
    CleanupStack::PopAndDestroy(name); //  name                  
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribePrecenseOfSingleContactL end")) ;   
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      DoHandlePresentityGroupContentEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::
DoHandlePresentityGroupContentEventL(
        const MXIMPContext& /*aContext*/,
        const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresentityGroupContentEventL start"));
	  	      
    const MPresentityGroupContentEvent& event =
    	*TXIMPGetInterface< const MPresentityGroupContentEvent >::From( 
            aEvent,
            MXIMPBase::EPanicIfUnknown );
    //this needs to be checked if server contacts is supported only then update to 
    //store else don't.
	TInt supportedFeatures = iXimpEventObserver.GetSupportedFeatures();
  	if(EVIMPSTFeatureFetch & supportedFeatures)
		{   
	    TRACE( T_LIT(" -> storing into respective service store" ));
	    StoreToVirtualStoreL( event );
		}
	//this is to ensure the presence for the local sotre contacts is not lost.
	TRACE( T_LIT(" -> new member count: %d" ), event.NewMembersCount() );    
    TRACE( T_LIT(" -> current member count: %d" ), event.CurrentMembersCount() );    
    TRACE( T_LIT(" -> disappeared member count: %d" ), event.DisappearedMembersCount() );    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresentityGroupContentEventL end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      DoHandlePresenceGrantRequestListEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::
DoHandlePresenceGrantRequestListEventL(
        const MXIMPContext& /*aContext*/,
        const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceGrantRequestListEventL start"));
    TVIMPSTEnums::TVIMPSTPresenceRequestStatus autoAccept = iSettingsTableFetcher.PresenceRequestStatusL(iServiceId);
    if(autoAccept == TVIMPSTEnums::ESVCEPresenceRequestStatusAutoAccept)
        {
        iAutoAccept = ETrue;
        }
    else
        {
        iAutoAccept = EFalse;
        }
    
     const MPresenceGrantRequestListEvent& event = 
     							*TXIMPGetInterface<const MPresenceGrantRequestListEvent >::From( 
								aEvent, MXIMPBase::EPanicIfUnknown );

    TRACE( T_LIT(" -> new watcher count: %d" ), event.NewRequestsCount() );    
    TRACE( T_LIT(" -> current watcher count: %d" ), event.CurrentRequestsCount() );    
    HBufC* identbuf(NULL);
    HBufC* displayName(NULL);
    TInt newcount     = event.NewRequestsCount();
    TInt currentcount = event.CurrentRequestsCount();
    if(newcount)
        {
        TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceGrantRequestListEventL newcount =%d" ), newcount );    
        TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceGrantRequestListEventL iServiceState =%d" ), iServiceState );    

        for(TInt i=0; i<newcount; i++)
            {
            const MPresenceGrantRequestInfo& reqInfo = event.NewRequest ( i );// its always a new request
            identbuf = reqInfo.RequestorId().Identity().AllocLC();
            displayName = reqInfo.RequestorDisplayName().AllocLC();
            TPtr identbufPtr = identbuf->Des();
            TRACE( T_LIT(" -> identity: %S" ), &identbufPtr );   
            if(identbuf->Length())
                {
                TRACE( T_LIT(" -> pass to command process" ));   
                if( iAutoAccept &&
                        TVIMPSTEnums::ESVCERegistered == iServiceState)
                    {
                    TInt error = SendPresenceGrantPresentityL( identbufPtr, ETrue );                    
                    }
                 else if( iSubServiceObserver )
                    {
                    TRACE( T_LIT(" -> informed observer." )); 
                    iSubServiceObserver->HandleAddRequestEventL( TVIMPSTEnums::EAddItem , *identbuf, *displayName);
                    }                    
                TRACE( T_LIT(" -> pass to command process" )); 
                }
            CleanupStack::PopAndDestroy( displayName );
            CleanupStack::PopAndDestroy( identbuf );
            }
        }
    else if(currentcount)
        {
        const MPresenceGrantRequestInfo& reqInfo = event.CurrentRequest(0 );// its always a new request
        identbuf = reqInfo.RequestorId().Identity().AllocLC();
        displayName  = reqInfo.RequestorDisplayName().AllocLC();
        TPtr identbufPtr = identbuf->Des();
        TRACE( T_LIT(" -> identity: %S" ), &identbufPtr );   
        if(identbuf->Length())
            {
            TRACE( T_LIT(" -> pass to command process" ));   
            if( iAutoAccept)
                {
                TInt error = SendPresenceGrantPresentityL( identbufPtr, ETrue );                    
                }
             else if( iSubServiceObserver )
                {
                TRACE( T_LIT(" -> informed observer." )); 
                iSubServiceObserver->HandleAddRequestEventL(TVIMPSTEnums::EAddItem ,*identbuf, *displayName);
                }      
            TRACE( T_LIT(" -> pass to command process" )); 
            }
        CleanupStack::PopAndDestroy( displayName );
        CleanupStack::PopAndDestroy ( identbuf );
        }    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceGrantRequestListEventL end"));

    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::ResolveServiceStateL
// ---------------------------------------------------------------------------
// 
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEnginePresenceSubService::ResolveServiceStateL(
        TCCHSubserviceState aState, 
        TInt aServiceError )
    {

    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::ResolveServiceStateL start"));    

    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;       

    TRACE( T_LIT("ResolveServiceStateL() iServiceId: %d, ServiceState: %d"), 
            iServiceId, aState );       

    TBool handleServiceStates = ETrue;
    if ( aServiceError && ECCHDisabled != aState )
        {    
        //Only if the Service supports ALR, the state can goto WaitingForNetwork
        //Still API from CCH is required to know whether ALR is supported or not
        //Not sure whether the below is right - have mailed to Markus for MoreInfo on this state
        if ( (KCCHErrorInvalidSettings != aServiceError) && (ECCHConnecting == aState) )        
            {   
            TRACE( T_LIT("ResolveServiceStateL() ESVCEWaitingForNetwork") );
            handleServiceStates = EFalse;  
            //state = TVIMPSTEnums::ESVCEWaitingForNetwork;
            state = TVIMPSTEnums::ESVCENotRegistered; 
            }
        }

    if ( handleServiceStates )
        {        
        switch ( aState )
            {
            case ECCHEnabled:
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCERegistered") );
                state = TVIMPSTEnums::ESVCEUpdatingContacts;   
                break;
                }
            case ECCHDisconnecting:      
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCERegistered") );                
                state = TVIMPSTEnums::ESVCENetworkDisConnecting;
                }
                break;

            case ECCHUninitialized:
            case ECCHDisabled:  
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCENotRegistered") );
                state = TVIMPSTEnums::ESVCENotRegistered;                
                }
                break;

            case ECCHConnecting:               
                {
                TRACE( T_LIT("ResolveServiceStateL() ESVCENoNetworkConnecting") );                
                state = TVIMPSTEnums::ESVCENetworkConnecting;
                }
                break;

            default:
                break;
            }
        }        

    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::ResolveServiceStateL end"));

    return state;  

    }


// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::CchEventOccuredL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::CchEventOccuredL( 
        TUint /*aServiceId*/, 
        TCCHSubserviceState aState, 
        TInt aServiceError )
    {

    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::CchEventOccuredL start"));
    TRACE( T_LIT("CchEventOccuredL() TCCHSubserviceState : %d, ServiceErr: %d"), 
            aState, aServiceError );

    if ( aServiceError && ECCHDisabled != aState )
        {
        //we might even end up in waiting for connection state for all
        //those service which are ALR enabled
        //So better check here if you get any CCH errors=        
        iServiceState = ResolveServiceStateL( aState, aServiceError );
        iObserver.HandleServceConnectionEventL();
        DoHandleCchErrorL( aServiceError );
        }
    else
        {
        TVIMPSTEnums::TVIMPSTRegistrationState currentState = 
        							ResolveServiceStateL( aState, aServiceError );
        							
		if ( TVIMPSTEnums::ESVCERegistered == iServiceState 
				&& TVIMPSTEnums::ESVCEUpdatingContacts == currentState )
			{
			//dont do anything
			//updating contacts is a special case
			//Once updating contacts, we fetch the groups and once this is done
			//we end up changing the state to registered
			//but CCH might send the same callback twice	
			//to handle this situation we add a check here
			//because we might have already moved from Updatin to Registered state
			}
		else
			{
			iServiceState = ResolveServiceStateL( aState, aServiceError );    
		    iObserver.HandleServceConnectionEventL();			}
        }   

    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::CchEventOccuredL end"));

    }


// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::DoHandleCchErrorL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEnginePresenceSubService::DoHandleCchErrorL( 
        TInt aServiceError )
    {

    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandleCchErrorL start"));
    TRACE( T_LIT("DoHandleCchErrorL() ServiceErr: %d"), 
            aServiceError );

    if ( aServiceError )
        {
        //unsubscribe can only be done, when bind is already done
        if(TVIMPSTEnums::EVIMPSTBindDone ==iXimpEventObserver.ContextBindStatus())
            {
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandleCchErrorL unscribe and unbind"));
            TRAP_IGNORE( UnsubscribeListsL() ); 
            iXimpEventObserver.ServerUnBindL( ETrue );
            }
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandleCchErrorL end"));
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::PublishOwnPresenceL
// 
// ---------------------------------------------------------    
TInt CVIMPSTEnginePresenceSubService::PublishOwnPresenceL(TVIMPSTEnums::TOnlineStatus aStatus, 
														  const TDesC& aValue,
						         						  const TDesC& aFilename /*= KNullDesC*/, 
						         						  const TDesC8& aMimetype /*= KNullDesC8*/,
						         						  TBool aIsAvatar /*= EFalse*/ )
    {
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService::PublishOwnPresenceL start"));
    // Take handles to object factory and publish interface
    MPresencePublishing& publisher = iXimpEventObserver.XimpPresencePublishingL();

    //Fill presence doc with presence components and attributes
    MPresenceInfo* myPresence = iXimpEventObserver.PresenceObjectFactoryL().NewPresenceInfoLC();//1
    MPersonPresenceInfo *personPresence = iXimpEventObserver.PresenceObjectFactoryL().NewPersonPresenceInfoLC();//2
    MPresenceInfoFieldCollection& attributeFields = personPresence->Fields();
	
 	MPresenceInfoField* infoField = NULL;
	TRACE( T_LIT("PublishOwnPresenceL aIsAvatar: %d"),  aIsAvatar );
	// avatar field set || clear
	if ( aIsAvatar )
        {
        TRACE( T_LIT(" PublishOwnPresenceL adding avatar field"));
        
     	infoField = iXimpEventObserver.PresenceObjectFactoryL().NewInfoFieldLC();
        MPresenceInfoFieldValueBinary* avatarField = iXimpEventObserver.PresenceObjectFactoryL().NewBinaryInfoFieldLC(); 
        TRACE( T_LIT(" PublishOwnPresenceL processing image data"));
        CVIMPSTEngineImageHandler* imageHandler = CVIMPSTEngineImageHandler::NewL();
        CleanupStack::PushL(imageHandler);
        // get the avatar content from the image processor
        // returns image content if the  processing succesful 
        
        HBufC8* avatarContent = imageHandler->ProcessImageFromFileL( aFilename , aMimetype);
        TRACE( T_LIT(" PublishOwnPresenceL processing image data completed "));
        if ( avatarContent )
			{
			TRACE( T_LIT("PublishOwnPresenceL valid image data found "));
			CleanupStack::PushL(avatarContent);
			// set a new avatar
			avatarField->SetBinaryValueL(*avatarContent);
 			avatarField->SetMimeTypeL(aMimetype);
			CleanupStack::PopAndDestroy(); // avatarContent
			}
		else
			{
			TRACE( T_LIT("PublishOwnPresenceL NULL image data found "));
			//  clear the avatar 
			avatarField->SetBinaryValueL(KNullDesC8);
			iIsClearingAvatar = ETrue;  //set iIsClearingAvatar to ETrue
			}
        CleanupStack::PopAndDestroy(imageHandler); // imageHandler
        
 	    //Fill presence doc with presence components and attributes
        infoField->SetFieldTypeL( NPresenceInfo::NFieldType::KAvatar ); 
        // for clear avatar mimetye is KNUllDesc so set for all the cases
       	infoField->SetFieldValue( avatarField );  // avatarField ownership transfered
        CleanupStack::Pop(); // avatarField
		attributeFields.AddOrReplaceFieldL(infoField ); // infofield ownership transfered
        CleanupStack::Pop(); // infoField
		infoField = NULL;
		TRACE( T_LIT(" PublishOwnPresenceL adding avatar field completed "));
        }

    // availabilty field
    infoField = iXimpEventObserver.PresenceObjectFactoryL().NewInfoFieldLC();//3
    //based on the state conver it to  ximpfw status.
    NPresenceInfo::TAvailabilityValues availablity = ConvertPresenceStatus( aStatus );
    // "availability" attribute
    MPresenceInfoFieldValueEnum* availabilityField = iXimpEventObserver.PresenceObjectFactoryL().NewEnumInfoFieldLC();//4
    availabilityField->SetValueL( availablity );
    infoField->SetFieldTypeL( NPresenceInfo::NFieldType::KAvailabilityEnum ); 
    infoField->SetFieldValue( availabilityField );
    CleanupStack::Pop(); // availabilityField
    attributeFields.AddOrReplaceFieldL(infoField );
    CleanupStack::Pop(); // infoField

    //status text field
    infoField = iXimpEventObserver.PresenceObjectFactoryL().NewInfoFieldLC();//7
    MPresenceInfoFieldValueText* statustextField = iXimpEventObserver.PresenceObjectFactoryL().NewTextInfoFieldLC();//8
    //ownership is transfered to statustextField
    HBufC* statusText = aValue.AllocLC();
    //ownership is not transferred
    statustextField->SetTextValueL( *statusText); // some status text
    CleanupStack::PopAndDestroy();//statusText
    infoField->SetFieldTypeL( NPresenceInfo::NFieldType::KStatusMessage );
    infoField->SetFieldValue( statustextField );
    CleanupStack::Pop(); // statustextField
    attributeFields.AddOrReplaceFieldL( infoField );
    CleanupStack::Pop(); // infoField
	

	myPresence->SetPersonPresenceL(personPresence);
    CleanupStack::Pop(); // personPresence
  
    TXIMPRequestId reqId;
    reqId = publisher.PublishOwnPresenceL( *myPresence );
    // wait completion
    // not own
    CVIMPSTEngineRequestMapper* mapper = iXimpEventObserver.GetRequestMapper();
    mapper->CreateRequestL(reqId, ETrue,EVIMPSTXimpOperationPublisOwnPresence);// waite here
    TInt error = iXimpEventObserver.GetCompletedReqResult(); // get the result error
    CleanupStack::PopAndDestroy(1); // myPresence
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::PublishOwnPresenceL end "));
    return error;
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::UpdateStatusToServerL
// 
// ---------------------------------------------------------    
NPresenceInfo::TAvailabilityValues CVIMPSTEnginePresenceSubService::ConvertPresenceStatus(TVIMPSTEnums::TOnlineStatus aStatus)
    {
    NPresenceInfo::TAvailabilityValues availablity;
    switch(aStatus)
        {
        case TVIMPSTEnums::EOffline:
            {
            availablity = NPresenceInfo::ENotAvailable;
            break;
            }
        case TVIMPSTEnums::EOnline:
            {
            availablity = NPresenceInfo::EAvailable;
            break;
            }
        case TVIMPSTEnums::EInvisible:
            {
            availablity = NPresenceInfo::EHidden;
            break;
            }
        case TVIMPSTEnums::EAway:
            {
            availablity = NPresenceInfo::EAway;
            break;
            }
        case TVIMPSTEnums::EBusy:
            {
            availablity = NPresenceInfo::EBusy;
            break;
            }
        case TVIMPSTEnums::EOnPhone:
            {
            availablity = NPresenceInfo::EOnPhone;
            break;
            }
        case TVIMPSTEnums::EDoNotDisturb:
            {
            availablity = NPresenceInfo::EDoNotDisturb;
            break;
            }
        default: 
            {
            availablity = NPresenceInfo::ENotAvailable;
            break;
            }
        }
    return availablity;
    }
/// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::FetchPresenceFromCache
// 
// ---------------------------------------------------------    
TInt  CVIMPSTEnginePresenceSubService::FetchPresenceFromCache()
    {
    //TODO::Figure out how to get the service name.
    // passed the service id to see the member count
    TInt error( KErrArgument );
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceFormCache() start") );
	if( iServiceName )
		{
		TPtr serviceNamePtr = iServiceName->Des();
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceFormCache() - %S"), 
		        &serviceNamePtr );
		// passed the service to register for notification
		error = iPresenceCacheReader->AllBuddiesPresenceInService(*iServiceName, this );		
		}
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceFormCache() end") ); 
	return error;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::HandlePresenceReadL
// ---------------------------------------------------------------------------
//  
void CVIMPSTEnginePresenceSubService::HandlePresenceReadL(TInt /*aErrorCode*/,
        RPointerArray<MPresenceBuddyInfo2>& aPresenceBuddyInfoList)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL start"));
    // we have the ownership of  aPresenceBuddyInfoList : Push it to customize cleanupstack
    // aPresenceBuddyInfoList is collection of owned object and each object need to be deleted
    CustomCleanupResetAndDestroyPushL(aPresenceBuddyInfoList);
    MVIMPSTStorageServiceView* storage = 
    				CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
    TInt buddyCount = aPresenceBuddyInfoList.Count();
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - count: %d" ), buddyCount );
    for ( TInt i =0 ; i < buddyCount ; ++i)
		{
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - buddy index: %d" ), i );
		MPresenceBuddyInfo2* buddyinfo = aPresenceBuddyInfoList[i];
		// read the buddyID : returns in XSP format
		TPtrC buddyXSPId = buddyinfo->BuddyId();
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - Status Message: %s" ), &buddyXSPId );
		TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
		// read the availability /presence state enum value 
		MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = buddyinfo->Availability();
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - Availability ENUM value: %d" ), availabilityEnum );
		TPtrC avablityText = buddyinfo->AvailabilityText();
		// convert the presence cache enum value to service tab enum 
		TVIMPSTEnums::TOnlineStatus status = ConvertPresenceCacheEnums( availabilityEnum , avablityText);
		//  get the GetAnyFiled(which has only pending and blocked states.)
		if(TVIMPSTEnums::EUnKnown == status)
			{
			GetKeyFieldsAndValuesL(*buddyinfo,status);
			}
		// read the  status message 
		TPtrC statusMsg = buddyinfo->StatusMessage();
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - Status Message: %s" ), &statusMsg );
		TPtrC8 avatarContent = buddyinfo->Avatar();
		HBufC8* avatarScaledData = NULL;
		if ( avatarContent.Length() )
			{
			TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - avatarContent Content available" ) );
			CVIMPSTEngineImageHandler* imageHandler = CVIMPSTEngineImageHandler::NewL();
			CleanupStack::PushL(imageHandler);
			TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - imageHandler created " ) );
		    avatarScaledData = imageHandler->ProcessImageFromDataL( avatarContent , KNullDesC8() );
		    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL - ProcessImageFromDataL returned " ) );
			CleanupStack::PopAndDestroy();//imageHandler
			}
		if( avatarScaledData && avatarScaledData->Length() )
			{
			CleanupStack::PushL(avatarScaledData);
			storage->UpdatePresenceL(buddyId,status,statusMsg, *avatarScaledData );	
			CleanupStack::PopAndDestroy();//avatarScaledData
			}
		else  
			{
			storage->UpdatePresenceL(buddyId, status, statusMsg, KNullDesC8 );		
			}
		}
    aPresenceBuddyInfoList.ResetAndDestroy();
 
    CleanupStack::PopAndDestroy();  //  aPresenceBuddyInfoList
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceReadL end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::HandlePresenceNotificationL
// ---------------------------------------------------------------------------
//  
void CVIMPSTEnginePresenceSubService::HandlePresenceNotificationL(TInt /*aErrorCode*/,
        MPresenceBuddyInfo2* aPresenceBuddyInfo)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceNotificationL start"));    
       
    if ( aPresenceBuddyInfo )
        {
        CleanupDeletePushL( aPresenceBuddyInfo );
        DoHandlePresenceNotificationL(*aPresenceBuddyInfo);
        CleanupStack::PopAndDestroy(1); // aPresenceBuddyInfo
        
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandlePresenceNotificationL end"));
    }


// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::SubscribeForAuthorizationL
//  
// ---------------------------------------------------------    
void  CVIMPSTEnginePresenceSubService::SubscribeForAuthorizationL()
    {
    TXIMPRequestId req;
    MPresenceAuthorization& authorization = iXimpEventObserver.XimpAuthorizationL();
    req = authorization.SubscribePresenceGrantRequestListL();  
    // mapper is not own
    CVIMPSTEngineRequestMapper* mapper = iXimpEventObserver.GetRequestMapper(); 
    mapper->CreateRequestL(req, EFalse,EVIMPSTXimpOperationSubcribeGrantRequestList);
    
    iSubscribeToAuthList = ETrue;
    }
// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::SendPresenceGrantPresentityResponseL
//  
// ---------------------------------------------------------    
TInt CVIMPSTEnginePresenceSubService::SendPresenceGrantPresentityL( const TDesC& aContactId , TBool aResponse )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SendPresenceGrantPresentityL stat"));  
    __ASSERT_ALWAYS( aContactId.Length(), User::Leave( KErrArgument ) );
      
    // return the response to the server.    
    MPresenceAuthorization& authorization = iXimpEventObserver.XimpAuthorizationL();
    // mapper is not own
    CVIMPSTEngineRequestMapper* mapper = iXimpEventObserver.GetRequestMapper();

    MXIMPIdentity* contactIdentity = iXimpEventObserver.XimpPresenceContextL().ObjectFactory().NewIdentityLC();
     __ASSERT_ALWAYS( contactIdentity , User::Leave( KErrNoMemory ) );		
    contactIdentity->SetIdentityL( aContactId ) ;    
    TXIMPRequestId req;
    //accepted the request.
    if(aResponse)
        {
        MPresenceInfoFilter* infoFilt = iXimpEventObserver.PresenceObjectFactoryL().NewPresenceInfoFilterLC();
        if( infoFilt )
            {
            infoFilt->AcceptPersonFilterL( NPresenceInfo::NFieldType::KAcceptAll );
            req = authorization.GrantPresenceForPresentityL(*contactIdentity ,*infoFilt );
            CleanupStack::PopAndDestroy(); //infoFilt   	
            }
        }
    else// rejected the request.
        {        
        req = authorization.WithdrawPresenceGrantFromPresentityL(*contactIdentity);         
        }
    if( iSubServiceObserver && !iAutoAccept )
        {
        iSubServiceObserver->HandleAddRequestEventL(TVIMPSTEnums::ERemoveItem ,aContactId, KNullDesC() );
        }
    mapper->CreateRequestL(req, !iAutoAccept, EVIMPSTXimpOperationGrantPresenceForPresentity );

    TInt error = iXimpEventObserver.GetCompletedReqResult(); // get the result error    
    if( ( ( error == KPREQUESTERRSUCCESSFUL) || ( error == KErrNone )  || iAutoAccept)  )
        {        
        MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId) ;
        if(storage && ( !storage->IsLocalStore() || iAutoAccept  ) && aResponse )
            {   
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SendPresenceGrantPresentityL server store") );
            storage->CreateNewContactL( aContactId,KNullDesC, ETrue, iAutoAccept ); // ETrue is for invitation item 
            } 
        }
    CleanupStack::PopAndDestroy(); //contactIdentity
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::SendPresenceGrantPresentityL end"));
    return error;
    }  
// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::HandleSessionContextEventL
// ---------------------------------------------------------
void CVIMPSTEnginePresenceSubService::HandleSessionContextEventL(const MXIMPContext& aContext,
        const MXIMPBase& aEvent,
        TXimpOperation aXimpOperation /*= EVIMPSTXimpOperationNoOperation*/ )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleSessionContextEventL start"));
    TInt32 eventId = aEvent.GetInterfaceId();

    switch( aEvent.GetInterfaceId() )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
            {
            TRACE( T_LIT("InsideCallbackswitch::MXIMPRequestCompleteEvent"));
            //temp fix TBD
            //Only use the operations that u r intertest in
            if ( aXimpOperation <= EVIMPSTXimpOperationUnsubscribe )  
                {

                const MXIMPRequestCompleteEvent* event =
                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
                        aEvent, MXIMPBase::EPanicIfUnknown );
                TRAP_IGNORE( HandleXimpRequestCompleteL( 
                        aXimpOperation,
                        event->CompletionResult().ResultCode(),
                        aEvent ) );
                }
            TRACE( T_LIT("InsideCallback::HandlePresenceContextEvent"));    

            break;
            }
        case MXIMPContextStateEvent::KInterfaceId:
            {
            TRACE( T_LIT("InsideCallbackswitch::MXIMPContextStateEvent"));
            TRACE( T_LIT("InsideCallback::MXIMPContextStateEvent"));
            break;
            }
        case MPresentityGroupContentEvent::KInterfaceId:
            {
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresentityGroupContentEvent"));
            DoHandlePresentityGroupContentEventL( aContext, aEvent );
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresentityGroupContentEvent"));
            break;  
            }
        case MPresenceGrantRequestListEvent::KInterfaceId:
            {
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresenceGrantRequestListEvent"));
            DoHandlePresenceGrantRequestListEventL( aContext, aEvent );
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresenceGrantRequestListEvent"));
            break;
            }
        case MPresenceBlockListEvent::KInterfaceId:
           {
           TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresenceBlockListEvent"));
           DoHandlePresenceBlockListEventL( aContext, aEvent );
           TRACE( T_LIT("CVIMPSTEnginePresenceSubService::MPresenceBlockListEvent"));

           break;
           }
           
        default:
            {
            break;
            }
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleSessionContextEventL end"));   
    }    

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::HandleListEventCompleteL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::HandleListEventCompleteL(TXimpOperation aType,
        TInt aCompleteCode,
        const MXIMPBase& /*aEvent*/)
    { 
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleListEventCompleteL start"));

    switch ( aType )
        {
        case EVIMPSTXimpOperationBind:
            {
            if(iLogoutRequest)
                {
                //since logout is requested, no need to Subscribe and Retrieve list
                break;
                }
            if ( KErrNone == aCompleteCode )
                {         
			    SubscribeForAuthorizationL();
                // get the list.
                RetrieveSubscribedListL();
                
                }
            break;

            }
        case EVIMPSTXimpOperationGetSubscribedList:
            {
            if(iLogoutRequest)
                {
                //Since logout is requested need not to retrive block list
                break;
                }
            // check if blocking is supported then get the blocked list and 
            // subscribe those to persence cache.
            TInt supportedFeatures = iXimpEventObserver.GetSupportedFeatures();
            if ( (EVIMPSTFeatureBlock & supportedFeatures) && (EVIMPSTFeatureUnBlock & supportedFeatures) )
               {
               RetrieveBlockListL();
               }
            break;
            }
        case EVIMPSTXimpOperationUnBind:
            {
            //Logout request completed
            iLogoutRequest = EFalse;
            break;
            }            
        default:
            break;   
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleListEventCompleteL end"));    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::HandleXimpRequestCompleteL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::HandleXimpRequestCompleteL(TXimpOperation aType,
        TInt aCompleteCode,const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleXimpRequestCompleteL start"));
    switch ( aType )
        {
        case EVIMPSTXimpOperationBind:
        case EVIMPSTXimpOperationGetSubscribedList:
        case EVIMPSTXimpOperationGetBlockList:
            //case TVIMPSTEnums::ESVCEXimpOperationGetWatcherList:
        case EVIMPSTXimpOperationUnBind:        
            {
            HandleListEventCompleteL( aType, aCompleteCode,aEvent );
            break;
            }
        case EVIMPSTXimpOperationWithdrawPresenceGrant:
            {
            break;
            }
        case EVIMPSTXimpOperationBlockPresenceForPresentity:
        case EVIMPSTXimpOperationCancelPresenceBlockFromPresentity:
        default:
            {
            break;
            }
        }


    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::HandleXimpRequestCompleteL end"));
    }
// -----------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::StoreToVirtualStoreL
// -----------------------------------------------------------------------------
//  
void CVIMPSTEnginePresenceSubService::StoreToVirtualStoreL(
                            const MPresentityGroupContentEvent& aListEvent )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::StoreToVirtualStoreL start"));
    TRACE( T_LIT("CurrentMembersCount count = %d"),aListEvent.CurrentMembersCount() );
    TRACE( T_LIT("NewMembersCount count = %d"),aListEvent.NewMembersCount() );
    TRACE( T_LIT("UpdatedMembersCount count = %d"),aListEvent.UpdatedMembersCount() );
    MVIMPSTStorageServiceView* storage = 
    CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId) ;	
    TLinearOrder< TPtrC > linearOrder (*CompareAlphabetically );
    if (storage)
        {		
        //currMembrCount will be 0 when you login to the service
        if( iIsFetchingContact)  
            {
            // there are contacts process each
            TPtrC listName = aListEvent.GroupId().Identity() ;
            RArray <TPtrC> firstNameList;
            CleanupClosePushL( firstNameList );
            RArray <TPtrC> serviceField;
            CleanupClosePushL( serviceField );
            
            firstNameList.Reset();
            serviceField.Reset();
            // number of contact in this list
           TInt currentMembrcount = aListEvent.CurrentMembersCount();
           TRACE( T_LIT("currentMembrcount count = %d"),currentMembrcount );
            // Handle first current items
            for(TInt j = 0; j < currentMembrcount ;j++ )
                {
                const MPresentityGroupMemberInfo& memberInfo = 
                    aListEvent.CurrentMember( j ) ;
                const MXIMPIdentity& memberIdentity = memberInfo.GroupMemberId() ;
                TPtrC userId = memberIdentity.Identity();
                TPtrC displayeName = memberInfo.GroupMemberDisplayName();
                serviceField.AppendL(userId );    
                firstNameList.AppendL(displayeName );
                }

            // number of contact in this list
            TInt newMembrcount = aListEvent.NewMembersCount() ;
            TRACE( T_LIT("newMembrcount count = %d"),newMembrcount );
            for(TInt i = 0; i < newMembrcount ;i++ )
                {
                const MPresentityGroupMemberInfo& memberInfo = 
                aListEvent.NewMember( i ) ;
                const MXIMPIdentity& memberIdentity = memberInfo.GroupMemberId() ;
                TPtrC userId = memberIdentity.Identity();
                TPtrC displayeName = memberInfo.GroupMemberDisplayName();
                TInt error = serviceField.InsertInOrder(userId,linearOrder);
                if(KErrAlreadyExists != error)
                    {
                    firstNameList.Append(displayeName);
                    }
                }
            TRACE( T_LIT(" calling CreateNewFetchContactsL") );
            TRACE( T_LIT(" serviceField count %d"),serviceField.Count());
            TRACE( T_LIT(" firstNameList count %d"),firstNameList.Count());

            // If count in both arrays does not match, storage side can panic
            __ASSERT_ALWAYS( firstNameList.Count() == serviceField.Count(), User::Leave( KErrCorrupt));
            storage->CreateNewFetchContactsL(firstNameList, serviceField);
            iIsFetchingContact = EFalse;
            
            CleanupStack::PopAndDestroy( &serviceField );
            CleanupStack::PopAndDestroy( &firstNameList );
            }
        //in case of delayed fetch contact and add contact 
	 	else 
			{
			// number of contact in this list
			TInt currentMembrcount = aListEvent.CurrentMembersCount() ;
			for(TInt i = 0; i < currentMembrcount ; i++ )
                {
                const MPresentityGroupMemberInfo& memberInfo = 
                                                aListEvent.CurrentMember( i ) ;
                const MXIMPIdentity& memberIdentity = memberInfo.GroupMemberId() ;
                RDebug::Print( _L("CVIMPSTEnginePresenceSubService: newMember %S"), &memberIdentity.Identity() );   
                if ( !storage->FindContactByUserId( memberIdentity.Identity() ) )
                    {
                    storage->CreateNewContactL( memberIdentity.Identity(), 
                             memberInfo.GroupMemberDisplayName(), ETrue, ETrue );     
                    }
                }
		    // number of contact in this list
		    TInt newMembrcount = aListEvent.NewMembersCount() ;
		    // there are contacts process each
		    TPtrC listName = aListEvent.GroupId().Identity() ;
		    for(TInt i = 0; i < newMembrcount ;i++ )
		        {
		        const MPresentityGroupMemberInfo& memberInfo = 
		                                        aListEvent.NewMember( i ) ;
		        const MXIMPIdentity& memberIdentity = memberInfo.GroupMemberId() ;
		        TPtrC userId = memberIdentity.Identity();
		        TPtrC displayeName = memberInfo.GroupMemberDisplayName();
		        TRACE( T_LIT("CVIMPSTEnginePresenceSubService: newMember %S"), &userId );	
		        storage->CreateNewContactL(userId,displayeName,ETrue,ETrue); 	
		        }
		   TInt removedMembrcount = aListEvent.DisappearedMembersCount() ;    
		   for(TInt i = 0; i < removedMembrcount ;i++ )
		        {
		        const MPresentityGroupMemberInfo& memberInfo = 
		                                        aListEvent.DisappearedMember( i ) ;
		        const MXIMPIdentity& memberIdentity = memberInfo.GroupMemberId() ;
		        TPtrC userId = memberIdentity.Identity();
				TRACE( T_LIT("CVIMPSTEnginePresenceSubService: deleteMember %S"), &userId );
		    	MVIMPSTStorageContact* contactExist = storage->FindContactByUserId(userId);
		    	if(contactExist)
		    	    {
		    	    storage->RemoveContactL(contactExist);	
		    	    }
		        }
			}
         
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::StoreToVirtualStoreL end")); 
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::IsChangeOwnStatusSupported
// 
// ---------------------------------------------------------        

TBool CVIMPSTEnginePresenceSubService::IsChangeOwnStatusSupported() 
    {
    //TODO:: get the feature supported from ximp and return
    return iChangeStatusSupported;
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::IsStatusMsgSupported
// 
// ---------------------------------------------------------        
TBool CVIMPSTEnginePresenceSubService::IsStatusMsgSupported()
    {
    //TODO:: get the feature supported from ximp and return
    return iChangeStatusMsgSupported;
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::IsAvatarSupported
// 
// ---------------------------------------------------------        
TBool CVIMPSTEnginePresenceSubService::IsAvatarSupported()
    {
    return iAvatarSupported;
    }
 
 // ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::SetAvatarSupported
// 
// ---------------------------------------------------------        
void CVIMPSTEnginePresenceSubService::SetAvatarSupported(TBool aSupported )
    {
    iAvatarSupported = aSupported;
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::ConvertXimpToClientPresenceStatus
// ---------------------------------------------------------------------------
//  
TVIMPSTEnums::TOnlineStatus CVIMPSTEnginePresenceSubService::ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum,TPtrC aAvabilityText)
    {
    // convert the presence cache enums to UI enumvalues
    // by default if the enum doesnot match then its  TVIMPSTEnums::UnKnown
    TVIMPSTEnums::TOnlineStatus status;
    switch( aAvailabilityEnum )
        {
        case MPresenceBuddyInfo2::EBusy:
			{
			status  = TVIMPSTEnums::EBusy;
			if(0==aAvabilityText.Compare(KAwayState))
			    {
			    status  = TVIMPSTEnums::EAway;
			    }
			if(0==aAvabilityText.Compare(KOnPhoneState))
			    {
			    status  = TVIMPSTEnums::EOnPhone;
			    }
			if(0==aAvabilityText.Compare(KDndState))
			    {
			    status  = TVIMPSTEnums::EDoNotDisturb;
			    }
            break;
			}
        case MPresenceBuddyInfo2::EAvailable:
            {
            status  = TVIMPSTEnums::EOnline;
            break;
            }
        case MPresenceBuddyInfo2::ENotAvailable:
            {
            status  = TVIMPSTEnums::EOffline;
            break;
            }
        case MPresenceBuddyInfo2::EUnknownAvailability:
            {
            status  = TVIMPSTEnums::EUnknown;
			if(0==aAvabilityText.Compare(KInvisibleState))
			    {
			    status  = TVIMPSTEnums::EInvisible;
			    }
			
            break;
            }   
        default:
	        {
            status  = TVIMPSTEnums::EUnknown;
            break;
	        }
        }
    return status;
    }


// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::RegisterPresenceEventObserver
// 
// ---------------------------------------------------------	
//TODO::Should be named as RegisterPresenceEventObserverL
void CVIMPSTEnginePresenceSubService::RegisterPresenceEventObserverL(
        MVIMPSTEnginePresenceSubServiceEventObserver* aObserver)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RegisterPresenceEventObserver start"));	
    __ASSERT_ALWAYS( aObserver, User::Leave( KErrArgument ));		
    iSubServiceObserver = aObserver;
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RegisterPresenceEventObserver end"));
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService::UnRegisterPresenceEventObserver
// 
// ---------------------------------------------------------	

void CVIMPSTEnginePresenceSubService::UnRegisterPresenceEventObserver(
        MVIMPSTEnginePresenceSubServiceEventObserver* /*aObserver*/)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnRegisterPresenceEventObserver start"));
    iSubServiceObserver = NULL;
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnRegisterPresenceEventObserver end"));
    }   
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::RetrieveBlockListL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::RetrieveBlockListL()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RetrieveBlockListL start"));

    //Subscribe block list
    // do get block list"));
    TXIMPRequestId operationId = TXIMPRequestId::Null();
    operationId = iXimpEventObserver.XimpAuthorizationL().SubscribePresenceBlockListL();
    CVIMPSTEngineRequestMapper* requestMapper =iXimpEventObserver.GetRequestMapper();
    requestMapper->CreateRequestL(operationId,EFalse,EVIMPSTXimpOperationGetBlockList);
    // list retrieving ok. Waiting for list.;
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RetrieveBlockListL end")); 
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL(MPresenceBuddyInfo2& aPresenceBuddyInfo)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL start" ) );
    MVIMPSTStorageServiceView* storage = 
    						CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
    TPtrC ownUserId = storage->OwnContactL().UserId();
    // read the buddyID : returns in XSP format
    TPtrC buddyXSPId = aPresenceBuddyInfo.BuddyId();
    TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL buddyId : %s" ), &buddyId );
    // read the availability /presence state enum value 
    MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = aPresenceBuddyInfo.Availability();
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - Availability ENUM value: %d" ), availabilityEnum );
    TPtrC avablityText = aPresenceBuddyInfo.AvailabilityText();
    // convert the presence cache enum value to service tab enum 
    TVIMPSTEnums::TOnlineStatus status = ConvertPresenceCacheEnums( availabilityEnum, avablityText);
    //TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - status: %d" ), status );
    //check if its pending or blocked contact.
    if(TVIMPSTEnums::EUnKnown == status)
        {
        GetKeyFieldsAndValuesL(aPresenceBuddyInfo,status);
        //TRACE( T_LIT("DoHandlePresenceNotificationL after GetKeyFieldsAndValuesL- status: %d" ), status );
        }
    TRACE( T_LIT("DoHandlePresenceNotificationL - status: %d" ), status );
    // Read  the  status message 
    TPtrC statusMsg = aPresenceBuddyInfo.StatusMessage();
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - Status Message: %s" ), &statusMsg );
    TPtrC8 avatarContent = aPresenceBuddyInfo.Avatar();
    
    ////////////////////////////////////////////////////////////////
    HBufC8* avatarScaledData = NULL;
	if ( avatarContent.Length() )
		{
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - avatarContent Content available" ) );
		CVIMPSTEngineImageHandler* imageHandler = CVIMPSTEngineImageHandler::NewL();
		CleanupStack::PushL(imageHandler);
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - imageHandler created " ) );
		avatarScaledData = imageHandler->ProcessImageFromDataL( avatarContent , KNullDesC8() );
		TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL - ProcessImageFromDataL returned " ) );
		CleanupStack::PopAndDestroy();//imageHandler
		}
    if( avatarScaledData && avatarScaledData->Length())
        {
        CleanupStack::PushL(avatarScaledData);
        storage->UpdatePresenceL(buddyId,status,statusMsg, *avatarScaledData ); 
        CleanupStack::PopAndDestroy();//avatarScaledData
        }
    else if( iIsClearingAvatar ) //clear own avatar
        {
        storage->UpdatePresenceL(buddyId, status, statusMsg, KNullDesC8, ETrue );
        iIsClearingAvatar = EFalse;  
        }
    else
        {
        storage->UpdatePresenceL(buddyId, status, statusMsg, KNullDesC8 );      
        }
	TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceNotificationL end" ) );
    }
// ---------------	------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::
//      DoHandlePresentityGroupContentEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::DoHandlePresenceBlockListEventL(
                                                        const MXIMPContext& /*aContext*/,
                                                        const MXIMPBase& aEvent )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceBlockListEventL start"));
    const MPresenceBlockListEvent& event  =
        *TXIMPGetInterface< const MPresenceBlockListEvent >::From( 
            aEvent,
            MXIMPBase::EPanicIfUnknown );
    // if the feature is supported then we need to add this contact into virtual store.
    // that logic  needs to be implemented.
    // inform ui about the state change from updatingcontacts to registered.
    if(TVIMPSTEnums::ESVCEUpdatingContacts == iServiceState )
        {
        TRACE( T_LIT(" -> CVIMPSTEnginePresenceSubService:state is ESVCEUpdatingContacts" ));  
        }
        
    TRACE( T_LIT(" -> new member count: %d" ), event.NewBlocksCount());    

    TInt subscriptionCount  =  event.NewBlocksCount();

    TRACE( T_LIT(" -> subscriptionCount: %d"), subscriptionCount );

        TRACE( T_LIT(" -> handling buddy list" ) );    
        HBufC* subsbuf(NULL);        
        for( TInt i =0; i < subscriptionCount; i++ )
            {
            const MPresenceBlockInfo& blockedEntitys = event.NewBlock(i);
            const MXIMPIdentity& ident = blockedEntitys.BlockedEntityId();
            subsbuf = ident.Identity().AllocLC();
            TPtr subsbufPtr = subsbuf->Des();
            TRACE( T_LIT(" -> identity: %S"), &subsbufPtr );    

            TRACE( T_LIT(" -> subscribe to cache" ) );    
            SubscribePresenceOfSingleContactL(*subsbuf);
            iBlockedListMgr->AddToBlockedListL(*subsbuf);
            CleanupStack::PopAndDestroy( subsbuf );
            }
        TInt disappearedCount = event.DisappearedBlocksCount();  
                
            for( TInt j =0; j < disappearedCount; j++ )
                {
                const MPresenceBlockInfo& blockedEntitys = event.DisappearedBlock( j );
                const MXIMPIdentity& ident = blockedEntitys.BlockedEntityId();
                subsbuf = ident.Identity().AllocLC();
                TPtr subsbufPtr = subsbuf->Des();
                TRACE( T_LIT(" -> identity: %S"), &subsbufPtr );    
              
                iBlockedListMgr->RemoveFromBlockListL( *subsbuf );    
                
            CleanupStack::PopAndDestroy( subsbuf );
            }
        
        if(iBlockedListObserver)
	        {
	        iBlockedListObserver->HandleBlockedListFetchCompleteL();
	        iBlockListFetchReqPending = EFalse;
	        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::DoHandlePresenceBlockListEventL end"));

    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::AddToBlockListL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEnginePresenceSubService::AddToBlockListL( const TDesC& aContactId )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::AddToBlockListL start"));
    if(TVIMPSTEnums::ESVCERegistered != iServiceState)
       return KErrNotSupported;
    //if aContactId is zero.
    if( 0 == aContactId.Length())
       return KErrArgument;
    
    TRACE( T_LIT(" -> aContactId: %s" ), &aContactId);
    TRACE( T_LIT(" -> perform block operation" ) );      
    
    MXIMPIdentity* identity = iXimpEventObserver.XimpPresenceContextL().ObjectFactory().NewIdentityLC();
    
    identity->SetIdentityL( aContactId ); 

    TXIMPRequestId reqId = iXimpEventObserver.XimpAuthorizationL().BlockPresenceForPresentityL(*identity );
    CVIMPSTEngineRequestMapper* mapper = iXimpEventObserver.GetRequestMapper();
    mapper->CreateRequestL(reqId, ETrue,EVIMPSTXimpOperationBlockPresenceForPresentity);// waite here
    TInt error = iXimpEventObserver.GetCompletedReqResult(); // get the result error
    														 //Contact will be added to blocked list manager,
    //Delete avatar of contact
    if(KErrNone == error && IsAvatarSupported())
	    {
   		MVIMPSTStorageServiceView* storage = 
    		CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
		storage->UpdateAvatarL(aContactId,KNullDesC8);
	    }
    //when pres. cache call will come.
    CleanupStack::PopAndDestroy(); // identity    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::AddToBlockListL end"));
    return error;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::RemoveFromBlockListL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEnginePresenceSubService::RemoveFromBlockListL( const TDesC& aUriOfTheContact )
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RemoveFromBlockListL start"));
    if(TVIMPSTEnums::ESVCERegistered != iServiceState)
       return KErrNotSupported;
    //if aUriOfTheCOntact is zero.
    if( 0 == aUriOfTheContact.Length())
       return KErrNotFound;
    
    TRACE( T_LIT(" -> aUriOfTheContact: %s" ), &aUriOfTheContact);
    TRACE( T_LIT(" -> perform unblock operation" ) );      
    
    MXIMPIdentity* identity = iXimpEventObserver.XimpPresenceContextL().ObjectFactory().NewIdentityLC();
    identity->SetIdentityL( aUriOfTheContact ); 
   
    TXIMPRequestId reqId = iXimpEventObserver.XimpAuthorizationL().CancelPresenceBlockFromPresentityL(*identity );
    CVIMPSTEngineRequestMapper* mapper = iXimpEventObserver.GetRequestMapper();
    mapper->CreateRequestL(reqId, ETrue,EVIMPSTXimpOperationCancelPresenceBlockFromPresentity);// waite here
    TInt error = iXimpEventObserver.GetCompletedReqResult(); // get the result error
    //if blocked contact is unblocked, then remove it from
    //locally maintained blocked list..
    if(KErrNone == error)
	    {
	    iBlockedListMgr->RemoveFromBlockListL(aUriOfTheContact);
	    }
    CleanupStack::PopAndDestroy( 1 ); // identity    
    
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::RemoveFromBlockListL end"));
    return error;
    }
    
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::GetBlockedList
// ---------------------------------------------------------------------------
//
RPointerArray<HBufC>* CVIMPSTEnginePresenceSubService::GetBlockedList()
	{
	if(iBlockedListMgr)
		{
		return iBlockedListMgr->BlockedList();
		}
	return NULL;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::GetBlockedList
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::ResetBlockedListManagerL()
	{
	iBlockedListMgr->ResetL();
	}
	
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::FetchBlockedListFromServer
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::FetchBlockedListFromServerL(MVIMPSTEngineBlockedListFetchEventObserver* aOb)
	{
	if(EFalse == iBlockListFetchReqPending)
		{
		//set the observer to give call back; Fetch from server is completed.
		iBlockedListObserver = aOb;
	
		//unsubscribe blocked list.
	    TXIMPRequestId operationId = TXIMPRequestId::Null();
	    operationId = iXimpEventObserver.XimpAuthorizationL().UnsubscribePresenceBlockListL();
	    CVIMPSTEngineRequestMapper* requestMapper =iXimpEventObserver.GetRequestMapper();
	    requestMapper->CreateRequestL(operationId,ETrue,EVIMPSTXimpOperationGetBlockList);
		
		iBlockListFetchReqPending = ETrue; 
		//subscribe again to get fresh blocked list from server.
		RetrieveBlockListL();
		}
	}
	
	
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::GetKeyFieldsAndValuesL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::GetKeyFieldsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo,TVIMPSTEnums::TOnlineStatus &aStatus)
    {
    TPtrC8 value = aPresenceBuddyInfo.GetAnyField( KExtensionKey());
    // At any point of time fro remote and blocked contact only one of the keys
    // KPendingRequestExtensionValue/KBlockedExtensionValue will be assigned, and not both the keys.
    if(value.Compare( KPendingRequestExtensionValue ) == 0)
        {
        aStatus = TVIMPSTEnums::EPending;
        }
    else if(value.CompareF( KBlockedExtensionValue ) == 0)
        {
        aStatus = TVIMPSTEnums::EBlocked;
        }
    else if(value.CompareF( KServiceExtensionValue ) == 0)
        {
        aStatus = TVIMPSTEnums::EServiceOut;
        }
    else if(value.CompareF( KCallForwardExtensionValue ) == 0)
        {
        aStatus = TVIMPSTEnums::ECallForward;
        }
    else
		{
		aStatus  = TVIMPSTEnums::EOffline;
		}
    }
	
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::UpdatePresenceStateL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::UpdatePresenceStateL()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UpdatePresenceStateL start") );
    //inform ui about the state change from updatingcontacts to registered.
    iServiceState = TVIMPSTEnums::ESVCERegistered;    
    iObserver.HandleServceConnectionEventL();
    
    TRACE( T_LIT(" -> HandleContactFetchedL:state is ESVCERegistered" )); 
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UpdatePresenceStateL end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL(const TDesC& aContact)
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL Start"));  
    if(aContact.Length() && iServiceName->Length())
        {
        HBufC* name = HBufC::NewLC( iServiceName->Length() + KColon().Length() + aContact.Length()  ); // 1. on to cleanup stack
        TPtr namePtr( name->Des() );
        namePtr.Zero();
        // append the service name followed by user id ,seperated by colon
        namePtr.Append(*iServiceName);
        namePtr.Append(KColon);
        namePtr.Append(aContact);
        TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL namePtr = %S"), &namePtr);  
        MPresenceBuddyInfo2* presenceBuddyInfo = iPresenceCacheReader->PresenceInfoLC(namePtr); // 2. on to cleanupstack
        if ( presenceBuddyInfo )
            {
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL presenceBuddyInfo"));            
            DoHandlePresenceNotificationL(*presenceBuddyInfo);
            CleanupStack::PopAndDestroy();  // presenceBuddyInfo
            TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL presenceBuddyInfo end"));  
            }
        CleanupStack::PopAndDestroy(name);  // name
        }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::FetchPresenceOfSingleContactL End")); 
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::UnsubscribeListsL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService::UnsubscribeListsL()
    {
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL IN"));
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL - unsubscribe buddy list"));

    // It is assumed here that buddy list and authorization list has been always
    // subscribed if bind has been done. Caller of this function must check
    // that session has been bound before calling this.
    TBuf<KUriMaxLength> buddyGroupBuffer( KFriendList );
    MXIMPIdentity* buddyGroupList = iXimpEventObserver.XimpPresenceContextL().ObjectFactory().NewIdentityLC();
    __ASSERT_ALWAYS( buddyGroupList , User::Leave( KErrNoMemory ) );        
    buddyGroupList->SetIdentityL( buddyGroupBuffer );        
    iXimpEventObserver.XimpPresentityGroupsL().UnsubscribePresentityGroupContentL(
                   *buddyGroupList );
    CleanupStack::PopAndDestroy(); // buddyGroupList
           
   // Auth list
   if(iSubscribeToAuthList)
       {
       TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL - unsubscribe auth list"));
       iSubscribeToAuthList = EFalse;
      iXimpEventObserver.XimpAuthorizationL().UnsubscribePresenceGrantRequestListL();  
       }
 
   // block list
   TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL - check if block is supported"));
   TInt supportedFeatures = iXimpEventObserver.GetSupportedFeatures();
   if ( (EVIMPSTFeatureBlock & supportedFeatures) && (EVIMPSTFeatureUnBlock & supportedFeatures) )
       {
       TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL - unsubscribe block list"));
       iXimpEventObserver.XimpAuthorizationL().UnsubscribePresenceBlockListL();        
       }
    TRACE( T_LIT("CVIMPSTEnginePresenceSubService::UnsubscribeListsL OUT"));
    }

// End of file
