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
* Description:  presene status handler 
 *                
 *
*/

 
#include "cservicewidgetpresencehandler.h"

#include "cservicewidgetbuddypresence.h"
#include "servicewidgetcpglobals.h"
#include "mservicewidgetobservers.h"
#include "cservicewidgetimagedecoder.h"
#include "swpdebugtrace.h"
//ximp includes
#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpobjectfactory.h>
#include <ximpidentity.h>
#include <ximpstatus.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpcontextstate.h>
#include <ximpobjectfactory.h>
#include <ximpidentity.h>
#include <ximpstatus.h>
#include <presencegrantrequestlistevent.h>
#include <presentitygroupcontentevent.h>
#include <presentitygroupmemberinfo.h>
#include <presencegrantrequestinfo.h>
#include <presencefeatures.h>
#include <presentitygroups.h>
#include <ximpstatus.h> //Presence info test
#include <presentitygroupcontentevent.h>
#include <presentitygroupmemberinfo.h>
#include <presenceauthorization.h>

//ximpfw presence cache
#include <presencecachereader2.h> // cache reader
#include <presencebuddyinfolist.h>
#include <presencebuddyinfo.h>
#include <presenceinfofield.h>
#include <personpresenceinfo.h>
#include <presenceinfofieldvaluetext.h>
#include <presenceinfofieldcollection.h>
#include <ximpidentity.h>
//presencecache event handler.
#include <prescachereadhandler.h>
#include <presenceinfo.h>

#include <avabilitytext.h>

#include "vimpstcustomcleanupapi.h" //For customized cleanup function

_LIT( KListNameAllBuddy ,"buddylist" );
const TInt KUriMaxLength = 255; 
// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::CServiceWidgetPresenceHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetPresenceHandler::CServiceWidgetPresenceHandler(MServiceWidgetPresenceObserver& aPresenceObserver )
	:iPresenceObserver( aPresenceObserver ),
	 iTotalCount( 0 ),
	 iOperationId( EWSOperationUnknown ),
	 iOwnStatus( EWSOnline )
	   {
	   TRACE_SWP(TXT("CServiceWidgetPresenceHandler::CServiceWidgetPresenceHandler()") );
	   }

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetPresenceHandler* CServiceWidgetPresenceHandler::NewL(MServiceWidgetPresenceObserver& aPresenceObserver,
															const TDesC& aServiceName,
															const TDesC& aOwnId )
    {
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::NewL() start") );
    CServiceWidgetPresenceHandler* self = NewLC(aPresenceObserver, aServiceName, aOwnId );
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::NewL() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::NewLC
// ---------------------------------------------------------------------------
// 
CServiceWidgetPresenceHandler* CServiceWidgetPresenceHandler::NewLC(MServiceWidgetPresenceObserver& aPresenceObserver,
													const TDesC& aServiceName,
													const TDesC& aOwnId )
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::NewLC() start") );
    CServiceWidgetPresenceHandler* self =
        new (ELeave) CServiceWidgetPresenceHandler(aPresenceObserver);
    CleanupStack::PushL(self);
    self->ConstructL( aServiceName, aOwnId );
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::NewLC() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetPresenceHandler::ConstructL(const TDesC& aServiceName,const TDesC& aOwnId )
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConstructL() start") );
    iServiceName = aServiceName.AllocL();
    iOwnUserId =  aOwnId.AllocL();
    iClient = MXIMPClient::NewClientL();  
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConstructL() iClient created") );             
    //Create new sink to receive ximp context events
	iPresenceCtx = iClient->NewPresenceContextLC();
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConstructL() iPresenceCtx created") );    
	iFeatures = MPresenceFeatures::NewL(iPresenceCtx);
	iAcceptedEventTypes.Reset();
    iAcceptedEventTypes.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MXIMPContextStateEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MPresentityGroupContentEvent::KInterfaceId );   
    iAcceptedEventTypes.AppendL( MPresenceGrantRequestListEvent::KInterfaceId );
    TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
	// register this to prsence context   
	iPresenceCtx->RegisterObserverL( *this, &eventFilterArray );
    // ignore code scanner warning, it gives panic
    CleanupStack::Pop();// because of LC method    
    //initialize the presence cache.
    iPresenceCacheReader = MPresenceCacheReader2::CreateReaderL(); 	
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConstructL() iPresenceCacheReader created") );  													 
    iPresenceCacheReader->SetObserverForSubscribedNotifications(this);
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConstructL() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::~CServiceWidgetPresenceHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetPresenceHandler::~CServiceWidgetPresenceHandler()
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::~CServiceWidgetPresenceHandler() start") );
    iAcceptedEventTypes.Reset();
    iOnlineBuddyArray.ResetAndDestroy();
    delete iServiceName;
    delete iOwnUserId;
  	delete iPresenceCacheReader;
  	delete iFeatures;
    if(iPresenceCtx)
        {
        iPresenceCtx->UnregisterObserver( *this );
        delete  iPresenceCtx;   
        }
    delete iClient; 
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::~CServiceWidgetPresenceHandler() end") );
    }

// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::GetConnectedSessionL
// 
// ---------------------------------------------------------
void CServiceWidgetPresenceHandler::GetConnectedSessionL(TInt aProtocolUid,TInt aServiceId )
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::GetConnectedSessionL() start") );
	iOperationId = EWSOperationBind;
    iPresenceCtx->BindToL( TUid::Uid( aProtocolUid), aServiceId ) ;
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::GetConnectedSessionL() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::GetCompletedReqResult
// 
// ---------------------------------------------------------
void CServiceWidgetPresenceHandler::ReleaseConnectionL()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ReleaseConnectionL() start") );
	iPresenceCtx->UnbindL() ;
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ReleaseConnectionL() end") );
	} 
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::SubscribePresenceL
// 
// ---------------------------------------------------------
void CServiceWidgetPresenceHandler::SubscribePresenceL( const TDesC& aUserId )
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::SubscribePresenceL() start") );
	 __ASSERT_ALWAYS( aUserId.Length(), User::Leave( KErrArgument ) );	
	 //if anything is there with colon e.g. sip:user@presence1. strip the part before :
	 TInt len = aUserId.Find(_L(":"));
	 TPtrC buddyId = aUserId.Right( aUserId.Length() - len - KColon().Length() );

    HBufC* name = HBufC::NewLC( buddyId.Length() + iServiceName->Length() + KColon().Length() );
    TPtr namePtr( name->Des() );
    namePtr.Zero();
   // append the service name followed by user id ,seperated by colon
    namePtr.Append(*iServiceName);
    namePtr.Append(KColon);
    namePtr.Append(buddyId);
    iPresenceCacheReader->SubscribePresenceBuddyChangeL( *name );
    CleanupStack::PopAndDestroy(); //  name   
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::SubscribePresenceL() end") );                                          
	}


// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::RetrieveSubscribedListL
// ---------------------------------------------------------------------------
//
void CServiceWidgetPresenceHandler::RetrieveSubscribedListL()
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::RetrieveSubscribedListL() start") );
    iOperationId = EWSOperationFetching;
     //Create group id
    TBuf<KUriMaxLength> buffer( KListNameAllBuddy );
    MXIMPIdentity* groupList = iPresenceCtx->ObjectFactory().NewIdentityLC();
     __ASSERT_ALWAYS( groupList , User::Leave( KErrNoMemory ) );		
    groupList->SetIdentityL( buffer );
    //Subscribe buddy list
    TXIMPRequestId operationId = TXIMPRequestId::Null();
    __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ) ) ;
    MPresentityGroups& groups = iFeatures->PresentityGroups() ;
    groups.SubscribePresentityGroupContentL( *groupList );
    CleanupStack::PopAndDestroy(); // groupList
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::RetrieveSubscribedListL() end") );
    } 	
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::ReadPresenceFromCacheL
// 
// ---------------------------------------------------------	
void CServiceWidgetPresenceHandler::ReadPresenceFromCacheL()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ReadPresenceFromCache() start") );
	iOnlineBuddyArray.ResetAndDestroy();
	__ASSERT_ALWAYS( iServiceName, User::Leave( KErrArgument ) );
    // passed the service id to see the member count
    iPresenceCacheReader->BuddyCountInService(*iServiceName);
    // passed the service to register for notification
	iPresenceCacheReader->AllBuddiesPresenceInService(*iServiceName, this ); 
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ReadPresenceFromCache() end") );
	}
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::SubscribeForAuthorizationL
//  
// ---------------------------------------------------------    
void  CServiceWidgetPresenceHandler::SubscribeForAuthorizationL()
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::SubscribeForAuthorizationL() start") );
         __ASSERT_ALWAYS( iFeatures, User::Leave( KErrNotSupported ) ) ;
    MPresenceAuthorization& authorization = iFeatures->PresenceAuthorization();
     __ASSERT_ALWAYS( &authorization, User::Leave( KErrNotSupported ) ); 
    authorization.SubscribePresenceGrantRequestListL();  
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::SubscribeForAuthorizationL() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::OnlineBuddyCount
// 
// ---------------------------------------------------------	
TInt CServiceWidgetPresenceHandler::OnlineBuddyCount()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::OnlineBuddyCount()") );
    return iOnlineBuddyArray.Count();
	}
	
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::AllBuddyCount
// 
// ---------------------------------------------------------	
TInt CServiceWidgetPresenceHandler::AllBuddyCount()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::AllBuddyCount()") );
    return iTotalCount;
	}
// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::GetInviteCountsL
// 
// ---------------------------------------------------------	
TInt CServiceWidgetPresenceHandler::GetInviteCounts()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::GetInviteCountsL()") );
    return iInvitationCount;
	}

 // ---------------------------------------------------------
// CServiceWidgetPresenceHandler::GetOwnStatusL
// 
// ---------------------------------------------------------	
TWSPresenceStatus CServiceWidgetPresenceHandler::GetOwnStatus()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::GetOwnStatusL()") );
	// get the avatar image if not return the default image
    return iOwnStatus;
	}

// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::GetOwnStatusL
// 
// ---------------------------------------------------------	
TWServiceOperationId CServiceWidgetPresenceHandler::GetOngoingOperationId()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::GetOngoingOperationId()") );
	// get the state of ongoing operation
    return iOperationId;
	}

// ---------------------------------------------------------
// CServiceWidgetPresenceHandler::HandlePresenceContextEvent
// ---------------------------------------------------------
void CServiceWidgetPresenceHandler::HandlePresenceContextEvent( 
    const MXIMPContext& /*aContext*/,
    const MXIMPBase& aEvent )
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceContextEvent() start") );
	switch( aEvent.GetInterfaceId() )
		{
		case MXIMPRequestCompleteEvent::KInterfaceId:
			{
			TRACE_SWP(TXT("CServiceWidgetPresenceHandler:: MXIMPRequestCompleteEvent ") );
			const MXIMPRequestCompleteEvent* complEvent = 
			TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
							aEvent, MXIMPBase::EPanicIfUnknown );
			TXIMPRequestId reqId = complEvent->RequestId();
			const MXIMPStatus& status = complEvent->CompletionResult() ;
			TInt reqResult = complEvent->CompletionResult().ResultCode();
			if( iOperationId == EWSOperationBind && reqResult == KErrNone )
				{
				TRACE_SWP(TXT("CServiceWidgetPresenceHandler::login completed ") );
				iOwnStatus = EWSOnline;
				TRAP_IGNORE(DoHandleBindCompleteL()) ;	
				}
			break;
			}
		case MPresentityGroupContentEvent::KInterfaceId:
	       {
	       TRACE_SWP(TXT("CServiceWidgetPresenceHandler::MPresentityGroupContentEvent ") );
           TRAP_IGNORE(DoHandlePresentityGroupContentEventL( aEvent )) ;
	       break;  
	       }
		case MPresenceGrantRequestListEvent::KInterfaceId:
	       {
	       TRACE_SWP(TXT("CServiceWidgetPresenceHandler:: MPresenceGrantRequestListEvent ") );
	       TRAP_IGNORE(DoHandlePresenceGrantRequestListEventL(aEvent )) ;
	       break;
	       }
		default:
			{
			break;
			}
		}
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceContextEvent() end") );
	} 
// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::HandlePresenceNotificationL
// ---------------------------------------------------------------------------
//  
void CServiceWidgetPresenceHandler::HandlePresenceNotificationL(TInt /*aErrorCode*/,
	        									MPresenceBuddyInfo2* aPresenceBuddyInfo)
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceNotificationL() start") );
    if ( aPresenceBuddyInfo)
        {
        // we have the ownership of  aPresenceBuddyInfo ; Push into cleanup stack
        CleanupDeletePushL( aPresenceBuddyInfo );
        if (iServiceName )
            {
            // read the buddyID : returns in XSP format
            TPtrC buddyXSPId = aPresenceBuddyInfo->BuddyId();
            TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
            // read the availability /presence state enum value 
            MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = aPresenceBuddyInfo->Availability();
            TPtrC avablityText = aPresenceBuddyInfo->AvailabilityText();
            // convert the presence cache enum value to service tab enum 
            TWSPresenceStatus status = ConvertPresenceCacheEnums( availabilityEnum, avablityText );

            TPtrC statusMsg = aPresenceBuddyInfo->StatusMessage(); // read the  status message
            TBool isOwnId = buddyId.Compare(*iOwnUserId) ? EFalse : ETrue ;

            if( isOwnId )
                {
                TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceNotificationL() own status change") );
                iOwnStatus = status;
                iPresenceObserver.HandleOwnStatusChangeL( iOwnStatus );	
                }
            else
                {
                TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceNotificationL() buddy status change") );
                CheckAndCacheBuddyAvailabilityL( buddyId, status );
                iPresenceObserver.HandleBuddyPresenceChangeL(buddyId, status, statusMsg );
                }
            }
        CleanupStack::PopAndDestroy(); //  aPresenceBuddyInfo
        }
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceNotificationL() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::HandlePresenceReadL
// ---------------------------------------------------------------------------
//  
void CServiceWidgetPresenceHandler::HandlePresenceReadL(TInt /*aErrorCode*/,
	         									   RPointerArray<MPresenceBuddyInfo2>& aPresenceBuddyInfoList)
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceReadL() start") );
    iOnlineBuddyArray.ResetAndDestroy(); // rest and destroy old data
    // we have the ownership of  aPresenceBuddyInfoList : Push it to customize cleanupstack
    // aPresenceBuddyInfoList is collection of owned object and each object need to be deleted    
    CustomCleanupResetAndDestroyPushL(aPresenceBuddyInfoList);
   
    TInt buddyCount = aPresenceBuddyInfoList.Count();
    for ( TInt i =0 ; i < buddyCount && iServiceName; ++i )
        {
        MPresenceBuddyInfo2* buddyinfo = aPresenceBuddyInfoList[i];
        // read the buddyID : returns in XSP format
        TPtrC buddyXSPId = buddyinfo->BuddyId();
        TPtrC buddyId = buddyXSPId.Right( buddyXSPId.Length() - iServiceName->Length() - KColon().Length());
        // read the availability /presence state enum value 
        MPresenceBuddyInfo2::TAvailabilityValues availabilityEnum = buddyinfo->Availability();
        TPtrC avablityText = buddyinfo->AvailabilityText();
         // convert the presence cache enum value to service tab enum 
        TWSPresenceStatus status = ConvertPresenceCacheEnums( availabilityEnum,avablityText );
        //compare if its for own user id
        TBool isOwnId = buddyId.Compare(*iOwnUserId) ? EFalse : ETrue ;
        if( isOwnId )
	        {
	        TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceReadL() own status change") );
	         iOwnStatus = status;
	         iPresenceObserver.HandleOwnStatusChangeL( iOwnStatus );
	         }
        else 
            {
            TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceReadL() buddy status change") );
            CheckAndCacheBuddyAvailabilityL( buddyId, status );
            }
         }
    CleanupStack::PopAndDestroy();  //  aPresenceBuddyInfoList, calls LocalResetAndDestroy 
    iPresenceObserver.HandleChangeL();
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::HandlePresenceReadL() end") );	
    }

 // ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::DoHandleBindCompleteL
// ---------------------------------------------------------------------------
//  
void CServiceWidgetPresenceHandler::DoHandleBindCompleteL()
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandleBindCompleteL() start") );
    __ASSERT_ALWAYS( iOwnUserId, User::Leave( KErrArgument ) );	
	SubscribePresenceL(*iOwnUserId);
	iInvitationCount = 0;
	SubscribeForAuthorizationL();
	// get the list.
	RetrieveSubscribedListL();
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandleBindCompleteL() end") );
	}
// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler:: DoHandlePresentityGroupContentEventL
// ---------------------------------------------------------------------------
//
void CServiceWidgetPresenceHandler::DoHandlePresentityGroupContentEventL( const MXIMPBase& aEvent )
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandlePresentityGroupContentEventL() start") );
    const MPresentityGroupContentEvent& event =	*TXIMPGetInterface< 
    const MPresentityGroupContentEvent >::From(aEvent,MXIMPBase::EPanicIfUnknown );
    TInt newCount = event.NewMembersCount();
    TInt disapCount = event.DisappearedMembersCount();
    
    iTotalCount = iTotalCount + newCount; 
    iTotalCount = iTotalCount - disapCount; 
    if( iTotalCount <0 )
	    {
	    // if total count becomes < 0 assign to 0
	    iTotalCount = 0;	
	    }
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler iTotalCount %d "), iTotalCount );
    HBufC* groupId = event.GroupId().Identity().AllocLC();   
    if ( groupId && !groupId->Compare( KListNameAllBuddy ) )
        {
        for( TInt i =0; i < newCount; i++ )
            {
            const MPresentityGroupMemberInfo& subscribedEntitys =  event.NewMember(i);
            const MXIMPIdentity& ident = subscribedEntitys.GroupMemberId();
            HBufC* buddy = ident.Identity().AllocLC();
            if( buddy )
				{
				SubscribePresenceL(*buddy);
				CleanupStack::PopAndDestroy( buddy );
				}
            }
        }
    if( groupId)
	    {
	    CleanupStack::PopAndDestroy( groupId );
        }
    iOperationId = EWSOperationComplete;
    iPresenceObserver.HandleChangeL();	
    ReadPresenceFromCacheL();
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandlePresentityGroupContentEventL() end") );
    }
// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler:: DoHandlePresenceGrantRequestListEventL
// ---------------------------------------------------------------------------
//
void CServiceWidgetPresenceHandler::DoHandlePresenceGrantRequestListEventL( const MXIMPBase& aEvent )
	{
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandlePresenceGrantRequestListEventL() start") );
	const MPresenceGrantRequestListEvent& event = *TXIMPGetInterface< const MPresenceGrantRequestListEvent >::From( 
	   					 aEvent, MXIMPBase::EPanicIfUnknown );
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler iInvitationCount %d"), iInvitationCount );
	TInt newcount    = event.NewRequestsCount();
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler newcount %d"), newcount );
	TInt disapearCount  = event.DisappearedRequestsCount();
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::disapearCount %d "), disapearCount);
    if(newcount)
		{
		iInvitationCount++; // one more inviation recieved
		TRACE_SWP(TXT("CServiceWidgetPresenceHandler new invitation ,iInvitationCount %d"), iInvitationCount );
		const MPresenceGrantRequestInfo& reqInfo = event.NewRequest ( 0 );// its always a new request
		HBufC* requesterId = reqInfo.RequestorId().Identity().AllocLC();
		if( requesterId )
		    {
		    iPresenceObserver.HandleAddRequestL(  *requesterId );
			CleanupStack::PopAndDestroy( requesterId );
		    }
		}
	else if( disapearCount )
		{
		iInvitationCount--;
		if( iInvitationCount <0 )
			{
			// if <0  assign to 0
			iInvitationCount = 0;	
			}
		TRACE_SWP(TXT("CServiceWidgetPresenceHandler accept/decline ,iInvitationCount %d"), iInvitationCount );
		// there is some count changes
		iPresenceObserver.HandleChangeL();	
		}
	TRACE_SWP(TXT("CServiceWidgetPresenceHandler::DoHandlePresenceGrantRequestListEventL() end") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::ConvertXimpToClientPresenceStatus
// ---------------------------------------------------------------------------
//  
TWSPresenceStatus CServiceWidgetPresenceHandler::ConvertPresenceCacheEnums( MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum,TPtrC aAvabilityText)
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConvertPresenceCacheEnums() start") );
    TWSPresenceStatus status = EWSUnknown ;
    switch( aAvailabilityEnum )
        {
        case MPresenceBuddyInfo2::EBusy:
            {
            status  = EWSbusy;
            if(0==aAvabilityText.Compare(KAwayState))
                {
                status  = EWSAway;
                }
            if(0==aAvabilityText.Compare(KOnPhoneState))
                {
                status  = EWSOnPhone;
                }
            if(0==aAvabilityText.Compare(KDndState))
                {
                status  = EWSDoNotDisturb;
                }
            break;
            }
        case MPresenceBuddyInfo2::EAvailable:
            {
            status  = EWSOnline;
            break;
            }
        case MPresenceBuddyInfo2::ENotAvailable:
            {
            status  = EWSOffline;
            break;
            }
        case MPresenceBuddyInfo2::EUnknownAvailability:
            {
            status  = EWSUnknown;
            if(0==aAvabilityText.Compare(KInvisibleState))
                {
                status  = EWSInvisible;
                }
            break;
            }
        default:
            {
            status  = EWSUnknown;
            break;
            }    
        }
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::ConvertPresenceCacheEnums() end") );
    return status;
    }
 // ---------------------------------------------------------------------------
// CServiceWidgetPresenceHandler::CheckAndCacheBuddyAvailabilityL
// ---------------------------------------------------------------------------
//  
void CServiceWidgetPresenceHandler::CheckAndCacheBuddyAvailabilityL( const TDesC& aUserId, TWSPresenceStatus aStatus )
    {
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::CheckAndCacheBuddyAvailabilityL() start") );
    TBool needToAdd = ETrue;
    TInt count = iOnlineBuddyArray.Count();
    for( TInt i=0; i<count; i++ )
	    {
	    CServiceWidgetBuddyPresence* store = iOnlineBuddyArray[ i ];
	    if( store && aUserId.Compare( store->BuddyId() ) == 0 )
		    {
		    if( aStatus == EWSUnknown || aStatus == EWSOffline )
			    {
			    // went offline remove from array
			    iOnlineBuddyArray.Remove( i ); // do not use agian the loop ,may cuase to crash 
			    delete store;
			    store = NULL;
			    iOnlineBuddyArray.Compress();
			    }
			else
				{
				store->SetPresenceStatus( aStatus );
		   		}
		  	needToAdd = EFalse;	
		    break;	// break the loop 
		    }
	    }
	if( needToAdd && ( aStatus != EWSUnknown ) && ( aStatus != EWSOffline ) )
		{
		// add only availabe buddies to list
		CServiceWidgetBuddyPresence* store = CServiceWidgetBuddyPresence::NewL( aUserId, aStatus );
        iOnlineBuddyArray.Append(store); // ownership given to array	
		}
    TRACE_SWP(TXT("CServiceWidgetPresenceHandler::CheckAndCacheBuddyAvailabilityL() end") );
    }

// end of file
