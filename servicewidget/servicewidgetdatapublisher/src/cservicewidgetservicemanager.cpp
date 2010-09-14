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
* Description:  service manager for a specific service
 *                
 *
*/

 
#include "cservicewidgetservicemanager.h"

#include "cservicewidgetcontentpublisher.h"
#include "cservicewidgetbrandhandler.h"
#include "cservicewidgetmessagehandler.h"
#include "cservicewidgetcchhandler.h"
#include "cservicewidgetpresencehandler.h"
#include "cservicewidgettimer.h"
#include "servicewidgetcpglobals.h"
#include "servicewidgetutils.h"
#include "swpdebugtrace.h"
#include <eikenv.h>
#include <servicewidgetpluginres.rsg>
#include <cchtypes.h>

#include <cvimpstsettingsstore.h>
#include <imcvlauncher.h>
#include <xSPViewServices.h>
#include <eikappui.h>

#include <spsettings.h> 
#include <spentry.h>
#include <spproperty.h>

#include <s32mem.h> //For Stream writing
//---------------------------------------------------------------------------
// CServiceWidgetServiceManager::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetServiceManager* CServiceWidgetServiceManager::NewL(TUint32 aServiceId,
																 CServiceWidgetContentPublisher& aContentPublisher,
																 MVIMPSTSettingsStore& aSettingStore )
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::NewL() start") );
    CServiceWidgetServiceManager* self = new( ELeave )CServiceWidgetServiceManager( aServiceId, aContentPublisher , aSettingStore);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE_SWP(TXT("CServiceWidgetServiceManager::NewL() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::CServiceWidgetServiceManager
// ---------------------------------------------------------------------------
// 
CServiceWidgetServiceManager::CServiceWidgetServiceManager(TUint32 aServiceId,
														   CServiceWidgetContentPublisher& aContentPublisher,
														   MVIMPSTSettingsStore& aSettingStore )
	:iServiceId( aServiceId ),
	iContentPublisher( aContentPublisher ),
	iSettingStore( aSettingStore ),
	iExecuteOperation( ESWAUnknown )
   {
   TRACE_SWP(TXT("CServiceWidgetServiceManager::CServiceWidgetServiceManager() start") );
   TRACE_SWP(TXT("CServiceWidgetServiceManager::CServiceWidgetServiceManager() start") );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::ConstructL()
    {
	TRACE_SWP(TXT("CServiceWidgetServiceManager::ConstructL() start") );
	CSPSettings* settings = CSPSettings::NewLC(); 
	// read service name    
	CSPEntry* entry = CSPEntry::NewLC();
	settings->FindEntryL( iServiceId, *entry ) ;
	// set the service name to presence cache updater
    iServiceName = entry->GetServiceName().AllocL() ;
    
    //KThemeUid appended to iServiceName for making service name unique from serivces 
    //published by other applications
    iServiceName = iServiceName->ReAllocL(iServiceName->Des().Length() + KThemeUid().Length());
    iServiceName->Des().Append(KThemeUid);
	CleanupStack::PopAndDestroy(2); // entry,settings
	
	iBrandHandler = CServiceWidgetBrandHandler::NewL(iServiceId);
	TRACE_SWP(TXT("CServiceWidgetServiceManager::ConstructL() end") );
    }
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::ActivateWidgetL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::ActivateWidgetL(TDesC& aInstId)
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::ActivateWidget() start") );
    iEnv = CEikonEnv::Static();
    //CCH handler initialize
    iCchHandler =  CServiceWidgetCchHandler::NewL( iServiceId, *this );
    TRACE_SWP(TXT("CServiceWidgetServiceManager::ActivateWidget() iBrandHandler created") );
    iServiceState = iCchHandler->ParseGetServiceState();
    iServiceviewId = iCchHandler->GetServiceViewIdL( iServiceTabUid );
    CreateHandlersL();
    //Initialize instance id of corresponding widget
    iInstId = aInstId.AllocL();
    iExecuteOperation = ESWALaunchServiceTab;
    
    TRACE_SWP(TXT("CServiceWidgetServiceManager::ActivateWidget() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::DeactivateWidgetL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::DeactivateWidgetL(TDesC& aInstId)
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::DeactivateWidgetL() start") );
    
    iContentPublisher.RemoveWidgetDataL(*iServiceName,aInstId );
    
    delete iMessageHandler;
    iMessageHandler = NULL;
    
    if( iPresenceHandler)
        {
        iPresenceHandler->ReleaseConnectionL() ;  
        delete iPresenceHandler ;
        iPresenceHandler = NULL;
        }
    if( iWidgetTimer )
        {
        iWidgetTimer->StopListening();
        delete iWidgetTimer;
        iWidgetTimer = NULL;
        }
    
    delete iCchHandler;
    iCchHandler = NULL;
    
    delete iInstId;
    iInstId = NULL;
    
    iIsWidgetSuspended = EFalse;
    iExecuteOperation = ESWAUnknown;
    TRACE_SWP(TXT("CServiceWidgetServiceManager::DeactivateWidgetL() end") );
    }
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::WidgetSuspended
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::WidgetSuspended( TBool aIsWidgetSuspended )
    {
    iIsWidgetSuspended  = aIsWidgetSuspended;
    }
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::~CServiceWidgetServiceManager
// ---------------------------------------------------------------------------
// 
CServiceWidgetServiceManager::~CServiceWidgetServiceManager()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::~CServiceWidgetServiceManager() start") );
    delete iMessageHandler;
    if( iPresenceHandler)
		{
		TRAP_IGNORE( iPresenceHandler->ReleaseConnectionL() );	
		delete iPresenceHandler ;
		iPresenceHandler = NULL;
		}
	delete iCchHandler;
	
	delete iServiceName;
	
    if( iWidgetTimer )
	    {
	    iWidgetTimer->StopListening();
	    delete iWidgetTimer;
        }
    delete iBrandHandler;
    
    delete iInstId;
    
    TRACE_SWP(TXT("CServiceWidgetServiceManager::~CServiceWidgetServiceManager() end") );
   	}
  // ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::UpdateContentsL
// ----------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::UpdateContentsL()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::UpdateContentsL() start") );
    if( !iIsWidgetSuspended )
        {
        SetCurrentTextDataL(); // to set the data in box 2 and 4 : Own id and new message or invite or presence
        }
   	TRACE_SWP(TXT("CServiceWidgetServiceManager::UpdateContentsL() end") );
    }
 
 // ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::SetCurrentTextDataL
// ----------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::SetCurrentTextDataL()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() start") );
    iServiceState = iCchHandler->ParseGetServiceState();

    TWSPresenceStatus showIcon = EWSLast;
    iExecuteOperation = ESWALaunchServiceTab;

    TBuf<KIMWCP_FIRSTTEXT_SIZE> firstText( KNullDesC );
    TBuf<KIMWCP_SECONDTEXT_SIZE> SecondText( KNullDesC);
    
    TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() service is complete") );
    // service configured correctly
    TPtrC serviceNameWithUID = iServiceName->Left( KIMWCP_FIRSTTEXT_SIZE );
    
    //serviceName is retrieved by removing KThemeUID. This text is displayed in first line of 
    //home screen widget when iServiceState state is ESWSNotRegistered state. 
    TPtrC serviceName = serviceNameWithUID.Left( serviceNameWithUID.Length() - KThemeUid().Length() );
    
    TPtrC ownUser = ServiceWidgetUtils::DisplayId( iCchHandler->OwnUserIdL().Left( KIMWCP_FIRSTTEXT_SIZE  ) );
    switch( iServiceState )
        {
        case ESWSNetworkConnecting:
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() Connecting") );
            firstText.Append(ownUser);
            HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_CONNECTING,iEnv );
            if( str )
                {
                SecondText.Append( *str );
                CleanupStack::PopAndDestroy(str); //str	
                }
            break;	
            }
        case ESWSUpdatingContacts:
        case ESWSRegistered:
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() Enabled") );
            firstText.Append(ownUser );
            TWServiceOperationId operationId = EWSOperationUnknown;
            HBufC* tempValue = NULL;
            if( iPresenceHandler )
                {
                // presence service supported 
                // take the ongoing operation
                showIcon = iPresenceHandler->GetOwnStatus();
                operationId = iPresenceHandler->GetOngoingOperationId();	
                if( operationId != EWSOperationComplete )
                    {
                    // fetching is on going
                    tempValue = ServiceWidgetUtils::LoadResourceLC(R_QTN_SERVTAB_WIDGET_UPDATING ,iEnv );
                    }
                }
            if( !tempValue)
                {
                // second row data is empty ,read it
                tempValue = GetSecondRowDataLC();	
                }
            if( tempValue )
                {
                TPtrC tempValuePtr( tempValue->Des() ); 
                SecondText.Append( tempValuePtr );
                CleanupStack::PopAndDestroy(tempValue); // tempValue	
                }
            break;	
            }
        case ESWSNetworkDisConnecting:
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() Disconnecting") );
            firstText.Append(ownUser );
            HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_LOGGINGOUT ,iEnv);
            if( str )
                {
                SecondText.Append( *str );
                CleanupStack::PopAndDestroy(str); //str	
                }
            break;	
            }
        case ESWSWaitingForNetwork:
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() Waiting for connection") );
            // waiting for network connection mostly for service which has ALR support
            firstText.Append(ownUser );
            HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_WAITING ,iEnv);
            if( str )
                {
                SecondText.Append( *str );
                CleanupStack::PopAndDestroy(str); //str	
                }
            break;
            }
        case ESWSNotRegistered:
        default :
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() ECCHDisabled ||  default ") );
            firstText.Append(serviceName );
            HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_LOGIN ,iEnv );
            if( str )
                {
                SecondText.Append( *str );
                CleanupStack::PopAndDestroy(str); //str	
                }
            iExecuteOperation = ESWAStartLogin;
            TInt isSuccessfullLogin = KErrNone;
            iSettingStore.GetL(iServiceId, EServiceSuccessfullLogin, isSuccessfullLogin );
            if( isSuccessfullLogin )
                {
                iExecuteOperation = ESWAStartLogin;
                }
            break;	
            }
        }
//Data to publish service Icon
    TInt sericonbitmapHandle = 0 ;
    TInt sericonmaskHandle  = 0 ;
    iBrandHandler->GetStatusBitmapL( EWSServiceIcon, sericonbitmapHandle, sericonmaskHandle ) ;
    //data to publish presence icon
    TInt bitmapHandle = 0;
    TInt maskHandle = 0 ;
    iBrandHandler->GetStatusBitmapL( showIcon, bitmapHandle, maskHandle ) ;
    // publish the data to widget
    iContentPublisher.PublishDataWithActionL( *iServiceName,
                                              *iInstId,
                                               firstText, 
                                               SecondText,
                                               bitmapHandle, 
                                               maskHandle,
                                               sericonbitmapHandle, 
                                               sericonmaskHandle
    );
    TRACE_SWP(TXT("CServiceWidgetServiceManager::SetCurrentTextDataL() end") );
    }
  
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::GetSecondRowDataLC
// ---------------------------------------------------------------------------
// 
HBufC* CServiceWidgetServiceManager::GetSecondRowDataLC()  
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() start ") );
	HBufC* text = HBufC::NewLC(KIMWCP_SECONDTEXT_SIZE);
	TPtr textPtr( text->Des() );
	TInt unreadCount = 0;
	TInt inviteCount = 0;
	HBufC* str = NULL;
	
	if( iMessageHandler)
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() iMessageHandler  ") );
		unreadCount = iMessageHandler->GetUnreadMessageCountL();
		}
	if( iPresenceHandler)
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() iPresenceHandler  ") );
		inviteCount = iPresenceHandler->GetInviteCounts();
		}
	if( ( static_cast<TIMWidgetContentPriority>( iPriority ) == EIMNewMessage) 
		&& unreadCount ) // match the priority
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() unreadCount %d  "), unreadCount );
		if( iMessageHandler->IsSingleMessageSender() )
			{
			// single sender launch conversation view 
			iExecuteOperation = ESWALaunchConverView ;	
			}
		if( unreadCount == 1 )
			{
			// exactly one message show singular form
			str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_ONENEWMSG , iEnv);	
			}
		else
			{
			//  more than one message show plural form
			str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_NEWMSGS, unreadCount, iEnv );		
			}
		if( str )
			{
			textPtr.Append( *str );
			CleanupStack::PopAndDestroy(str); //str	
			}
       	}
     else if( ( static_cast<TIMWidgetContentPriority>( ++iPriority ) == EIMFriendInvite ) 
     	&& inviteCount )
		 // match the priority
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() inviteCount %d  "), inviteCount );
		if( inviteCount == 1 )
			{
			// exactly one request show singular form
			str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_ONEPENDREQ ,iEnv );	
			}
		else
			{
			//  more than one request show plural form
			str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_PENDREQ, inviteCount, iEnv );		
			}
		if( str )
			{
			textPtr.Append( *str );
			CleanupStack::PopAndDestroy(str); //str	
			}
		} 
  	else if( iPresenceHandler )// this is next priority
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() iPresenceHandler ") );
		TInt totalCount = iPresenceHandler->AllBuddyCount();
		TInt onlineCount = iPresenceHandler->OnlineBuddyCount();
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() totalCount %d "), totalCount);
		TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() onlineCount %d "), onlineCount);	
		// array to append online / totlal counts
		CArrayFix<TInt>* intArray = new ( ELeave ) CArrayFixFlat<TInt>( 2 );
		CleanupStack::PushL( intArray );
		intArray->AppendL( onlineCount );
		intArray->AppendL( totalCount );
		str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_ONLINESTATUS, *intArray , iEnv );
		if( str )
			{
			textPtr.Append( *str );
			CleanupStack::PopAndDestroy(str); //str	
			}
		CleanupStack::PopAndDestroy(intArray); //intArray	
		}
	else
		{
		// nothing to show in online case ,show "You are online"
		str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_AVAILABLE ,iEnv );	
		if( str )
			{
			textPtr.Append( *str );
			CleanupStack::PopAndDestroy(str); //str	
			}
		}
	ResetPriority(); // reset to high again
	TRACE_SWP(TXT("CServiceWidgetServiceManager::GetSecondRowDataLC() end ") );
	return text; // ownership to caller
	}
   	
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleNewMessageL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleNewMessageL(const TDesC& aSenderId ,const TDesC& aMessageText)  
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleNewMessageL() start ") );
	if( !iIsWidgetSuspended )
	    {
	    TBuf<KIMWCP_FIRSTTEXT_SIZE> firstText;
	    TBuf<KIMWCP_SECONDTEXT_SIZE> secondText;
	    TBuf<KIMWCP_ADD_DATA_SIZE> additionalData( KNullDesC);
	    HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_FRIEND, 
	            ServiceWidgetUtils::DisplayId( aSenderId ),
	            iEnv  );
	    if( str )
	        {
	        TPtr strPtr( str->Des() );
	        firstText.Append( strPtr.Left( KIMWCP_FIRSTTEXT_SIZE ) );
	        CleanupStack::PopAndDestroy(str); //str	
	        }
	    secondText.Append( aMessageText.Left( KIMWCP_SECONDTEXT_SIZE ) );

	    additionalData.Append( aSenderId.Left( KIMWCP_ADD_DATA_SIZE ) ) ;
	    
	    //Data to be published for service icon
	    TInt sericonbitmapHandle = 0 ;
	    TInt sericonmaskHandle  = 0 ;
	    iBrandHandler->GetStatusBitmapL( EWSServiceIcon, sericonbitmapHandle, sericonmaskHandle ) ;
	    
	    TInt bitmapHandle = 0;
	    TInt maskHandle = 0 ;
	    iBrandHandler->GetStatusBitmapL( EWSNewMessage, bitmapHandle, maskHandle ) ;
	    // publish the new message icon with all other data  
	    iContentPublisher.PublishDataWithActionL( *iServiceName,
	                                               *iInstId,
                                                    firstText,
                                                    secondText ,
                                                    bitmapHandle,
                                                    maskHandle,
                                                    sericonbitmapHandle, 
                                                    sericonmaskHandle
                                                    );
	     iExecuteOperation = ESWALaunchConverView;
         if( iWidgetTimer )
	        {
	        TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleNewMessageL() timer started ") );
	        iWidgetTimer->StopListening();
	        iWidgetTimer->StartListening( KTimerInterval );	
	        }
	    }
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleNewMessageL() end ") );
	}
  
  // ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleAddRequestL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleAddRequestL(const TDesC& aRequesterId )     
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleAddRequestL() start ") );
	if( !iIsWidgetSuspended )
	    {
	    TBuf<KIMWCP_FIRSTTEXT_SIZE> firstText;
	    TBuf<KIMWCP_SECONDTEXT_SIZE> secondText;

	    HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_INVITE ,iEnv );  
	    if( str )
	        {
	        TPtr strPtr( str->Des() );
	        firstText.Append( strPtr.Left( KIMWCP_FIRSTTEXT_SIZE ) );
	        CleanupStack::PopAndDestroy(str); //str    
	        }
	    secondText.Append( ServiceWidgetUtils::DisplayId( aRequesterId ).Left( KIMWCP_SECONDTEXT_SIZE ) );
	    // publish the last icon which is empty 
	    TBuf<KIMWCP_ADD_DATA_SIZE> additionalData( KNullDesC);
	    //Data to be published for service icon
	    TInt sericonbitmapHandle = 0 ;
	    TInt sericonmaskHandle  = 0 ;
	    iBrandHandler->GetStatusBitmapL( EWSServiceIcon, sericonbitmapHandle, sericonmaskHandle ) ;
	    
	    TInt bitmapHandle = 0;
	    TInt maskHandle = 0 ;
	    iBrandHandler->GetStatusBitmapL( EWSLast, bitmapHandle, maskHandle ) ;
        //Publish all data
	    iContentPublisher.PublishDataWithActionL(*iServiceName,
	                                             *iInstId,
                                                firstText,
                                                secondText,
                                                bitmapHandle,
                                                maskHandle,
                                                sericonbitmapHandle, 
                                                sericonmaskHandle
                                            );
	    iExecuteOperation = ESWALaunchServiceTab;
	    if( iWidgetTimer )
	        {
	        TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleAddRequestL() timer started ") );
	        iWidgetTimer->StopListening();
	        iWidgetTimer->StartListening( KTimerInterval );	
	        }
	    }
    TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleAddRequestL() start ") );
	}
	
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleCountChangedL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleCountChangedL()  
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleCountChangedL() start ") );
	UpdateContentsL(); // set new data
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleCountChangedL() end ") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleChangeL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleChangeL()  
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleChangeL() start ") );
	UpdateContentsL(); // new data	
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleChangeL() end ") );  
	}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleBuddyPresenceChangeL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleBuddyPresenceChangeL(const TDesC& aBuddyId, 
                                                            TWSPresenceStatus aStatus,
                                                            const TDesC& aStatusText )  
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleBuddyPresenceChangeL() start ") );
    if( !iIsWidgetSuspended )
        {
        TBuf<KIMWCP_FIRSTTEXT_SIZE> firstText;
        TBuf<KIMWCP_SECONDTEXT_SIZE> secondText;
        HBufC* str = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_FRIEND, 
                                                       ServiceWidgetUtils::DisplayId( aBuddyId ),
                                                           iEnv  );
        if( str )
            {
            TPtr strPtr( str->Des() );
            firstText.Append( strPtr.Left( KIMWCP_FIRSTTEXT_SIZE ) );
            CleanupStack::PopAndDestroy(str); //str    
            }
        HBufC* statusText = NULL;
        HBufC* status = NULL;
        if( aStatusText.Length() )
            {
            statusText = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_STATUS, 
                    ServiceWidgetUtils::DisplayId( aStatusText ),
                    iEnv  );
            }
        else
            { 
            TInt statusResId = R_QTN_SERVTAB_WIDGET_OFFLINE;
            switch( aStatus )
                {
                case EWSOnline:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_ONLINE;
                    break;
                    }
                case EWSAway:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_AWAY;
                    break;
                    }
                case EWSbusy:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_BUSY;
                    break;
                    }

                case EWSDoNotDisturb:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_DND;
                    break;
                    }
                case EWSOnPhone:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_ONTHEPHONE;
                    break;
                    }
                case EWSOffline:
                default:
                    {
                    statusResId = R_QTN_SERVTAB_WIDGET_OFFLINE;
                    }
                }
            status = ServiceWidgetUtils::LoadResourceLC( statusResId, iEnv  ); 
            if( status )
                {
                statusText = ServiceWidgetUtils::LoadResourceLC( R_QTN_SERVTAB_WIDGET_STATUS,
                        *status,
                        iEnv  );
                }
            }
        if( statusText )
            {
            TPtr statusTextPtr( statusText->Des() );
            secondText.Append( statusTextPtr.Left( KIMWCP_SECONDTEXT_SIZE ) );
            CleanupStack::PopAndDestroy(); //statusText
            if( status )
                {
                CleanupStack::PopAndDestroy(status); //status 
                }
             
            }
        //Data to be published for service Icon
        TInt sericonbitmapHandle = 0 ;
        TInt sericonmaskHandle  = 0 ;
        iBrandHandler->GetStatusBitmapL( EWSServiceIcon, sericonbitmapHandle, sericonmaskHandle ) ;
        //////////////////////////////////////////
        TInt bitmapHandle = 0;
        TInt maskHandle = 0 ;
        iBrandHandler->GetStatusBitmapL( aStatus, bitmapHandle, maskHandle ) ;
        TBuf<KIMWCP_ADD_DATA_SIZE> additionalData( KNullDesC);
        iContentPublisher.PublishDataWithActionL(*iServiceName,
                                                 *iInstId,
                                                firstText, 
                                                secondText,
                                                bitmapHandle,
                                                maskHandle,
                                                sericonbitmapHandle, 
                                                sericonmaskHandle
                                                );
        iExecuteOperation = ESWALaunchServiceTab;
        if( iWidgetTimer )
            {
            TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleBuddyPresenceChangeL() timer started ") );
            iWidgetTimer->StopListening();
            iWidgetTimer->StartListening( KTimerInterval ); 
            }
        }
    TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleBuddyPresenceChangeL() end ") );  
  }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleOwnStatusChangeL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleOwnStatusChangeL( TWSPresenceStatus /*aStatus*/ )  
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleOwnStatusChangeL() start ") );
	UpdateContentsL();
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleOwnStatusChangeL() end ") );
	}
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleServiceStatusChangedL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleServiceStatusChangedL( TSWSTRegistrationState aState ) 
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleServiceStatusChangedL() start ") );
	iServiceState = aState ;
	CreateHandlersL();
	UpdateContentsL();	
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleServiceStatusChangedL() end ") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::HandleTimerExpiredL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::HandleTimerExpiredL() 
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleTimerExpiredL() start ") );
	UpdateContentsL(); // once timer expired show the current data based on availabity
	TRACE_SWP(TXT("CServiceWidgetServiceManager::HandleTimerExpiredL() end ") );	
	}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::CreateHandlersL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::CreateHandlersL() 
	{
	TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() start ") );
	if( iServiceState == ESWSRegistered  || iServiceState == ESWSUpdatingContacts )
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() ECCHEnabled ") );
		// once logged in create all interfaces
		// please note this is boot blugin so keep lesser memory use
		if( !iPresenceHandler && iCchHandler->IsSubServiceSupported( ECCHPresenceSub ) )
			{
			TPtrC ownId = iCchHandler->OwnUserIdL();			
			TPtrC str = iServiceName->Des().Left(iServiceName->Des().Length()-KThemeUid().Length());
			iPresenceHandler = CServiceWidgetPresenceHandler::NewL(*this,str,ownId );
			iPresenceHandler->GetConnectedSessionL( iCchHandler->XimpAdaptationUidL(), iServiceId );
			TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iPresenceHandler cretaed ") );	
			}
		if( !iMessageHandler && iCchHandler->IsSubServiceSupported( ECCHIMSub ) )
			{
			TPtrC ownId = iCchHandler->OwnUserIdL();
			iMessageHandler = CServiceWidgetMessageHandler::NewL( *this, iServiceId, ownId);
			TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iMessageHandler created ") );	
			}
		if( !iWidgetTimer && ( iPresenceHandler || iMessageHandler  ) )
			{
			// create the timer only if required
			iWidgetTimer =  CServiceWidgetTimer::NewL(*this) ;
		    TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iWidgetTimer created ") );
		 	}
		}
	else if( iServiceState == ESWSNotRegistered )
		{
		TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() ECCHDisabled ") );
		// once disconnecting off delete all interfaces
		if( iWidgetTimer )
			{
			delete iWidgetTimer;
			iWidgetTimer = NULL;
			TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iWidgetTimer deleted ") );	
			}
		if(iMessageHandler  )
			{
			delete iMessageHandler;
			iMessageHandler = NULL;	
			TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iMessageHandler deleted ") );
			}
		if( iPresenceHandler)
			{
			iPresenceHandler->ReleaseConnectionL();	
			delete iPresenceHandler ;
			iPresenceHandler = NULL;
			TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() iPresenceHandler deleted ") );
			}
	   	}
	TRACE_SWP(TXT("CServiceWidgetServiceManager::CreateHandlersL() end ") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::ServiceId
// ---------------------------------------------------------------------------
// 
TInt CServiceWidgetServiceManager::ServiceId()
{
TRACE_SWP(TXT("CServiceWidgetServiceManager::ServiceId() Manager") );
return iServiceId;
}

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::ServiceName
// ---------------------------------------------------------------------------
// 
const TDesC& CServiceWidgetServiceManager::ServiceName()
{
TRACE_SWP(TXT("CServiceWidgetServiceManager::ServiceName() Manager") );
return *iServiceName;
}
// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::UnregisterWidgetL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::UnregisterWidgetL()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::UnregisterWidgetL() Manager") );
    
    iContentPublisher.UnregisterWidgetL(*iServiceName);
    }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::RegisterWidgetL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::RegisterWidgetL()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::RegisterWidgetL() Manager") );
    TInt bitmapHandle = 0 ;
    TInt maskHandle  = 0 ;
    iBrandHandler->GetStatusBitmapL( EWSServiceIcon, bitmapHandle, maskHandle ) ;
    iContentPublisher.RegisterWidgetL(*iServiceName,bitmapHandle, maskHandle);
    }

// ---------------------------------------------------------------------------
// CServiceWidgetServiceManager::ExecuteActionL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetServiceManager::ExecuteActionL()
    {
    TRACE_SWP(TXT("CServiceWidgetServiceManager::ExecuteActionL() Manager") );
    switch( iExecuteOperation )
        {
        case ESWALaunchServiceTab:
            {
            RxSPViewServices viewServices;
            iServiceviewId = iCchHandler->GetServiceViewIdL( iServiceTabUid );
            TInt err = viewServices.Activate(  iServiceTabUid, 
                    iServiceviewId ) ; 
            TRACE_SWP(TXT("CServiceWidgetServiceManager::ExecuteActionL() Activate = %d"), err), 
            User::LeaveIfError( err );
            
            break;
            }
        case ESWALaunchConverView:
            {
            if( iMessageHandler && iEnv )
                {
                TRACE_SWP(TXT("CServiceWidgetServiceManager::ExecuteActionL() launch cv") );
                TVwsViewId activeViewId;
                CCoeAppUi* appui = iEnv->AppUi();
                appui->GetActiveViewId( activeViewId );
                IMCVLauncher::LaunchImConversationViewL(activeViewId,
                                                        iServiceId,
                                                        iMessageHandler->GetUnreadSenderId() ) ;
                }
            break;
            }
        case ESWALaunchSTAutoLogin :
            {
            /* 
             * First pack version number followed by service id.
             * PhoneBook first read till EEnd and then it goes to service tab
             * get the custom info in this call CVIMPSTUiTabbedView::DoActivateViewL
             * aCustomMessageId is CPbk2ViewState::Uid()
             * In service tab (tabbedview) we need to unpack dataPtr in similar reverse pattern
             *
            */
            HBufC8* data = HBufC8::NewLC( 20 );
                        TPtr8 dataPtr( data->Des() );
             RDesWriteStream stream( dataPtr );
             stream.PushL();
             stream.WriteInt8L(1);   // version number
             stream.WriteInt8L( 0 ); //EEnd
             stream.WriteInt32L( iServiceId ); //Service Id
             stream.CommitL();
                       
            RxSPViewServices viewServices;
            iServiceviewId = iCchHandler->GetServiceViewIdL( iServiceTabUid );
            TInt err = viewServices.Activate(  iServiceTabUid, 
                                            iServiceviewId , dataPtr );
            TRACE_SWP(TXT("CServiceWidgetServiceManager::ExecuteActionL() Activate = %d"), err), 
            User::LeaveIfError( err );
            CleanupStack::PopAndDestroy(); 
            CleanupStack::PopAndDestroy( data );// data
            break;
            }
        case ESWAStartLogin:
            {
            iCchHandler->EnableServiceL();
            break;
            }
        default :
            {
            // do nothing
            }
        } // end of switch
    
  }//End ExecuteActionL
// End of File
