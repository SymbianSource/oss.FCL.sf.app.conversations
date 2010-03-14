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
* Description:  engine class
*
*/

// INCLUDE FILES
#include "cimcvengine.h"

#include "cimcvenginechatcontainer.h"
#include "cimcvenginemessagehandler.h"
#include "cimcvenginemessagecreator.h"
#include <e32base.h>
#include <badesca.h>
#include <e32property.h>
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

// local includes
#include "cimcvenginecontextobserver.h"
#include "cimcvenginechatcontainer.h"
#include "imcvlogger.h"
#include "imcvuiliterals.h"
//settings
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>

#include "cimcvengineopenchats.h"
#include "cimcvenginecchhandler.h"
#include "conversations.h"

#include <imconnectionproviderconsts.h>
//branding related header files
#include <mbsaccess.h>
#include <mbselement.h>
#include <cbsfactory.h>
#include <utf.h>
#include <gulicon.h>

#define KMAX_SMILEY_COUNT 20

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
 CIMCVEngine* CIMCVEngine::NewL(TInt aServiceId, MIMCVEngineMessageCreator& aMessageCreater, 
        CBSFactory& aBrandingFactory )
    {
   	IM_CV_LOGS(TXT("CIMCVAppView::CIMCVEngine::NewL() start") );
    CIMCVEngine* self = new ( ELeave ) CIMCVEngine(aServiceId,aBrandingFactory);

    CleanupStack::PushL( self );
    self->ConstructL(aMessageCreater );
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppView::CIMCVEngine::NewL() end") );
    return self;
    }


// ---------------------------------------------------------
// CIMCVEngine::~CIMCVEngine()
// Note the destruction order of objects.
// ---------------------------------------------------------
//
 CIMCVEngine::~CIMCVEngine()
    {
	IM_CV_LOGS(TXT("CIMCVEngine destructor START") );
    if( iWait.IsStarted() )  
	    {
	    iWait.AsyncStop();  
	    }
    Cancel();
    delete iCCHHandler;
    iAcceptedEventTypes.Reset();
	iAcceptedEventTypes.Close();
	
	delete iOwnUserId;
	delete iServiceName;
	delete iBrandId;

	if(iPresenceContext)
		{
		iPresenceContext->UnregisterObserver( *iContextEventObserver );
		delete 	iPresenceContext;
		iPresenceContext = NULL;
		}
	if(iClient)
		{
		delete iClient;	
		iClient = NULL;
		}
	if(iContextEventObserver)
		{
		iContextEventObserver->UnRegisterObserver(iMessageHandler);
		delete iContextEventObserver;
		iContextEventObserver = NULL;	
		}
	if(iMessageHandler)
		{
		delete iMessageHandler;
		iMessageHandler = NULL;
		}
	delete iChatInterface;

	if( iBrandingAccess  )
	    {
	    iBrandingAccess->Close();
	    iBrandingAccess = NULL;
	    }
	IM_CV_LOGS(TXT("CIMCVEngine destructor END") );

#if 0
	if( iSmileyArrayOwned )
	    {
		IM_CV_LOGS(TXT("CIMCVEngine::deleting icon STRING array owned") );
	    iSmileyArray.ResetAndDestroy();
	    }
	IM_CV_LOGS(TXT("CIMCVEngine::RESETTING icon STRING array ") );
	iSmileyArray.Reset();
	
	if( iSmileyDlgIconArrayOwned )
	    {
		IM_CV_LOGS(TXT("CIMCVEngine::deleting icon array owned") );
	    iSmileyDlgIconArray.ResetAndDestroy();
	    }
	IM_CV_LOGS(TXT("CIMCVEngine::deleting icon array ") );	
	iSmileyDlgIconArray.Reset();
#endif	
	IM_CV_LOGS(TXT("CIMCVAppViewContainer::SendMessageL() start") );

	}

// ---------------------------------------------------------
// CIMCVEngine::CIMCVEngine()
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
CIMCVEngine::CIMCVEngine(TInt aServiceId,CBSFactory& aBrandingFactory) :
CActive( CActive::EPriorityStandard ),
iServiceId( aServiceId ),
iBrandingFactory( aBrandingFactory ),
iSmileyArrayOwned( ETrue ),
iSmileyDlgIconArrayOwned( ETrue )
    {
    iOwnUserId = NULL;
    CActiveScheduler::Add( this );
    }
// ---------------------------------------------------------
// CIMCVEngine::ConstructL()
// Symbian OS default constructor can leave.
// ---------------------------------------------------------
//
void CIMCVEngine::ConstructL(MIMCVEngineMessageCreator& aMessageCreater)
    {
    IM_CV_LOGS(TXT("CIMCVAppView::CIMCVEngine::ConstructL()	start") );   
    iClient = MXIMPClient::NewClientL();
	//Create new sink to receive ximp context events
	iContextEventObserver = CIMCVEngineContextObserver::NewL();
	
	iChatInterface = CIMCVEngineChatContainer::NewL( iServiceId );
	//Read the logged in status from CCH True/False
    TBool isCchEnebled = EFalse;
	iCCHHandler = CIMCVEngineCchHandler::NewL(iServiceId, *this);
	isCchEnebled = iCCHHandler->IsServiceLoggedIn();
	
	// Read User Id from CCH
	iOwnUserId = HBufC::NewL(KMAXUSERIDLENGTH);
	TPtr owndataPtr =  iOwnUserId->Des();	
	GetUserIdL(owndataPtr ) ;
	
	//Read Servicename from CCH
    iServiceName = HBufC::NewL(KMAXUSERIDLENGTH);
	TPtr srvNamePtr( iServiceName->Des() );	
	GetServiceNameL( iServiceId, srvNamePtr );   
    
	//Read the service state from CCH
    TCCHSubserviceState serviceState = ECCHUninitialized;    
    TInt error = iCCHHandler->GetServiceState( 
        			serviceState ); 
  
	if ( ECCHEnabled == serviceState || isCchEnebled )
		{
		// register this to prsence context  
		LoginL();
		}
	// Read the BrandId from SPsettings
	iBrandId = HBufC8::NewL( KVIMPSTUISPSMaxPropertyLength );
	TPtr8 brandidPtr(iBrandId->Des());
	GetBrandIdL( iServiceId, brandidPtr );	
    
	//Read BrandLanguage id from SPsettings
    iLanguageId = PropertyBrandLanguageL(iServiceId);    
	
    ConstructBrandL();
    
	if ( ECCHEnabled == serviceState || isCchEnebled )
		{
		iMessageHandler = CIMCVEngineMessageHandler::NewL( *iChatInterface, 
                                                        aMessageCreater,
                                                        iPresenceContext, 
														iServiceId, 
														owndataPtr );
		}
	else
		{
		iMessageHandler = CIMCVEngineMessageHandler::NewL( *iChatInterface, 
                                                            aMessageCreater, 
                                                            NULL, 
                                                            iServiceId, 
                                                            owndataPtr );
			
		}		
														
	iContextEventObserver->RegisterObserver(iMessageHandler);	
   	IM_CV_LOGS(TXT("CIMCVAppView::CIMCVEngine::ConstructL()	end") );
    }

// ---------------------------------------------------------
// CIMCVEngine::ConstructBrandL()
// ---------------------------------------------------------
//
void CIMCVEngine::ConstructBrandL()
    {
    if(iBrandingAccess)
        {
        iBrandingAccess->Close();
        iBrandingAccess = NULL;
        }
    iBrandingAccess = iBrandingFactory.CreateAccessL( GetBrandId(), (TLanguage)GetLanguageId() );
    if(iBrandingAccess)
        {
        TInt totalCount = iBrandingAccess->GetIntL(KSmileycount);
        
        // Restricting the maximum number of smileys to KMAX_SMILEY_COUNT.
        if( totalCount > KMAX_SMILEY_COUNT )
            totalCount = KMAX_SMILEY_COUNT;
        HBufC* allSmileyString = iBrandingAccess->GetTextL(KSmileytext);
        CleanupStack::PushL(allSmileyString);   
        allSmileyString->Des().Delete(0, 1);
        allSmileyString->Des().Delete(((allSmileyString->Length())-1), 2);
        ParseAllSmileyStringL(totalCount, *allSmileyString);
        CleanupStack::PopAndDestroy(allSmileyString);

        TInt iconCount = iSmileyArray.Count();
        CGulIcon* icon = NULL;
        CFbsBitmap* bitmap = NULL;
        CFbsBitmap* mask = NULL; 
        HBufC* iconString = NULL;
        TInt err = KErrNotFound;
        for( TInt index = 0; index< iconCount; index++ )
            {
            iconString = iSmileyArray[index];
            TPtrC iconStringPtr = iconString->Des();          
            TBuf8<16> aUtf8; //            
            CnvUtfConverter::ConvertFromUnicodeToUtf8(aUtf8, iconStringPtr);            
            iBrandingAccess->GetBitmapL(aUtf8, bitmap, mask );
			CleanupStack::PushL( bitmap );
			CleanupStack::PushL( mask );
            icon = CGulIcon::NewL( bitmap, mask ); 
			CleanupStack::Pop( 2 ); // bitmap ,mask
		     
            err = iSmileyDlgIconArray.Append( icon );        
			   
            if( err < 0)
                {
                delete icon;
                icon = NULL;
                }
            }
        }
     }

// ---------------------------------------------------------
// CIMCVEngine::GetSmileStringArray()
// ---------------------------------------------------------
//
EXPORT_C RPointerArray<HBufC> CIMCVEngine::GetSmileStringArray()
    {   
    iSmileyArrayOwned = EFalse;
    return iSmileyArray;// ownership transfered
    }

// ---------------------------------------------------------
// CIMCVEngine::ParseAllSmileyStringL()
// ---------------------------------------------------------
//
EXPORT_C RPointerArray<CGulIcon> CIMCVEngine::GetSmileyIconArray( )
    {
    iSmileyDlgIconArrayOwned = EFalse;
    return iSmileyDlgIconArray; // ownership transfered
    }
// ---------------------------------------------------------
// CIMCVEngine::ParseAllSmileyStringL()
// ---------------------------------------------------------
//
void CIMCVEngine::ParseAllSmileyStringL(TInt aTotalCount , const TDesC& aAllSmileyString)
    {       
    TInt location = KErrNotFound;
    HBufC* string = aAllSmileyString.AllocLC();
    TPtr mainBufferPtr = string->Des();
    //make sure everything is fresh
    iSmileyArray.ResetAndDestroy();
    for(TInt i=0; i<aTotalCount; i++)
        {        
        location = mainBufferPtr.Locate(' ');
        if(location != KErrNotFound )
            {
            TPtrC smileStringPtr = mainBufferPtr.Left(location);
            HBufC* smileString = smileStringPtr.AllocL();
            iSmileyArray.Append(smileString);
            mainBufferPtr.Delete(0,location+1);
            }
        if( i == (aTotalCount -1 ))
            {
            //for last string
            HBufC* smileString = mainBufferPtr.AllocL();
            iSmileyArray.Append(smileString);
            }      
        } 
    CleanupStack::PopAndDestroy(string);   
    }

// ---------------------------------------------------------
// CIMCVEngine::ParseAllSmileyStringL()
// ---------------------------------------------------------
//
EXPORT_C CGulIcon* CIMCVEngine::GetPresenceIconL(const TDesC8& aIconid)
    {
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    CGulIcon* icon = NULL;
    iBrandingAccess->GetBitmapL(aIconid, bitmap, mask );
	CleanupStack::PushL( bitmap );
	CleanupStack::PushL( mask );
	icon = CGulIcon::NewL( bitmap, mask ); 
	CleanupStack::Pop( 2 ); // bitmap ,mask
    return icon;
    }
// ---------------------------------------------------------
// CIMCVEngine::CreateConnectionL()
// ---------------------------------------------------------
//
void CIMCVEngine::CreateConnectionL() 
	{
	iPresenceContext = iClient->NewPresenceContextLC();
	// ignore code scanner warning, it gives panic
	CleanupStack::Pop();// because of LC method
	// Read the ximpfw adaptation uid from SPsettings
	TInt protocolUidValue = iCCHHandler->GetAdapterUidL(); ;	
	// filter the event from ximpfw ,interested only login and request complete
	iAcceptedEventTypes.Reset();
	iAcceptedEventTypes.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
	iAcceptedEventTypes.AppendL( MXIMPContextStateEvent::KInterfaceId );	
		
	TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
	// register this to prsence context   
	iPresenceContext->RegisterObserverL( *iContextEventObserver, &eventFilterArray );
	TUid protocolUid =TUid::Uid( protocolUidValue );
	iContextEventObserver->GetConnectedSessionL(*iPresenceContext,iServiceId, protocolUid );
	}
// ---------------------------------------------------------
// CIMCVEngine::ChatInterface()
// ---------------------------------------------------------
//
EXPORT_C MIMCVEngineChatInterface& CIMCVEngine::ChatInterface() const
    {
    return *iChatInterface;
    }

// ---------------------------------------------------------
// CIMCVEngine::ChatInterface()
// ---------------------------------------------------------
//
EXPORT_C MIMCVEngineMessageHandler& CIMCVEngine::MessageHandler() const
    {
    return *iMessageHandler;
    }

// ---------------------------------------------------------
// CIMCVEngine::ReadyForShutdown()
// ---------------------------------------------------------
//
EXPORT_C TBool CIMCVEngine::ReadyForShutdown()
    {
    return !iContextEventObserver->IsLoggedIn();
    }
   
// ---------------------------------------------------------
// CIMCVEngine::IsLoggedIn()
// ---------------------------------------------------------
//
EXPORT_C TBool CIMCVEngine::IsLoggedIn() const
	{
    return iContextEventObserver->IsLoggedIn();	
    }

// ---------------------------------------------------------
// CIMCVEngine::GetLoggedInUserId()
// ---------------------------------------------------------
//
EXPORT_C const TDesC& CIMCVEngine::GetLoggedInUserId() 
	{
	if(iOwnUserId)
		{
		return *iOwnUserId;
		}
	return KNullDesC;
	}	
// ---------------------------------------------------------
// CIMCVEngine::ReleaseConnectionL()
// ---------------------------------------------------------
//
EXPORT_C void CIMCVEngine::ReleaseConnectionL() 
	{
	if (iPresenceContext)
	    {
	    iContextEventObserver->ReleaseConnectionL(*iPresenceContext);
	    }
	}
	
// ---------------------------------------------------------
// CIMCVEngine::DeleteContextL()
// ---------------------------------------------------------
//
void CIMCVEngine::DeleteContextL() 
	{
	if(iPresenceContext)
		{
		iPresenceContext->UnregisterObserver( *iContextEventObserver );
		delete 	iPresenceContext;
		iPresenceContext = NULL;

		}

	}

// ---------------------------------------------------------
// CIMCVEngine::CreateContextL()
// ---------------------------------------------------------
//
void CIMCVEngine::CreateContextL() 
	{
	if(!iPresenceContext)
		{
		CreateConnectionL();
		iMessageHandler->ResetContextL(iPresenceContext); 
		}
	}
// ---------------------------------------------------------
// CIMCVEngine::GetLoggedInUserId()
// ---------------------------------------------------------
//
EXPORT_C const TDesC8& CIMCVEngine::GetBrandId() 
    {
    if(iBrandId)
        {
        return *iBrandId;
        }
    return KNullDesC8;
    }   
// ---------------------------------------------------------
// CIMCVEngine::GetLoggedInUserId()
// ---------------------------------------------------------
//
EXPORT_C TInt CIMCVEngine::GetLanguageId() 
    {
    return iLanguageId;
    }   


// ---------------------------------------------------------
// CIMCVEngine::CCHHandler()
// ---------------------------------------------------------
//
EXPORT_C MIMCVEngineCchHandler& CIMCVEngine::CCHHandler() 
    {
    return *iCCHHandler;     
    } 

// ---------------------------------------------------------
// CIMCVEngine::ServiceName()
// ---------------------------------------------------------
//
EXPORT_C const TDesC& CIMCVEngine::ServiceName() 
    {
    return *iServiceName;     
    } 
    

// ---------------------------------------------------------
// CIMCVEngine::GetLoggedInUserId()
// ---------------------------------------------------------
//

EXPORT_C MIMCVEngineOpenChats& CIMCVEngine::OpenChats() 
	{
	return  iMessageHandler->OpenChats();
	}


// ---------------------------------------------------------
// CIMCVEngine::GetServiceId()
// ---------------------------------------------------------
//
EXPORT_C TInt CIMCVEngine::GetServiceId() 
	{	
	return iServiceId;
	}
		
// ---------------------------------------------------------------------------
// CIMCVEngine::GetServiceNameL()
// ---------------------------------------------------------------------------
//
void CIMCVEngine::GetServiceNameL( 
    TUint32 aServiceId, TDes& aServiceName )
    {
    
    CSPSettings* settings = CSPSettings::NewLC();
    CSPEntry* entry = CSPEntry::NewLC();
    TRAPD( err, settings->FindEntryL( aServiceId, *entry ) );
    if ( !err )
        {
        aServiceName = entry->GetServiceName();
        }
    
    CleanupStack::PopAndDestroy( 2 );    
    }
// ---------------------------------------------------------------------------
// CIMCVEngine::IsVoipServiceL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CIMCVEngine::IsVoipServiceL()
    {
    TBool isvoipservice = EFalse;
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL(iServiceId, ESubPropertyVoIPEnabled, *property);
    if (property)
        {
        isvoipservice = ETrue; 
        }
    CleanupStack::PopAndDestroy(2); //property,settings 
    return isvoipservice;
    }    
    


// ---------------------------------------------------------------------------
// CIMCVEngine::GetBrandIdL()
// ---------------------------------------------------------------------------
//
void CIMCVEngine::GetBrandIdL( 
    TUint32 aServiceId, TDes8& aBrandId )
    {
    HBufC* ret = HBufC::NewL( KVIMPSTUISPSMaxPropertyLength );
    CleanupStack::PushL( ret );
    TPtr retPtr( ret->Des() );
    
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL( aServiceId,
                                EPropertyBrandId,
                                *property );    
    if ( property )
        {
        property->GetValue( retPtr );
        aBrandId.Copy( retPtr );
        }
    CleanupStack::PopAndDestroy( 3 );  //property,settings,ret             
    }    



// ---------------------------------------------------------------------------
// CIMCVEngine::PropertyBrandLanguageL()
// ---------------------------------------------------------------------------
//
TLanguage CIMCVEngine::PropertyBrandLanguageL( 
    TUint32 aServiceId ) 
    {
    
    // default to english
    TInt brandLanguage = ELangInternationalEnglish; 
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL( aServiceId,
                                EPropertyBrandLanguage,
                                *property );    
    if ( property )
        {
        property->GetValue( brandLanguage );
        }
    CleanupStack::PopAndDestroy( 2 ); // property,settings 

    return ((TLanguage) (brandLanguage) );
    }



// ---------------------------------------------------------------------------
// CIMCVEngine::PropertySettingsIdL()
// ---------------------------------------------------------------------------
//
TInt CIMCVEngine::PropertySettingsIdL( 
    TUint32 aServiceId ) 
    {
    
    // default to english
    TInt settingsId = 0; 
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL( aServiceId,
                                ESubPropertyIMSettingsId,
                                *property );    
    if ( property )
        {
        property->GetValue( settingsId );
        }
    CleanupStack::PopAndDestroy( 2 );  // property,settings 

    return (settingsId);
    }


// ---------------------------------------------------------------------------
// CIMCVEngine::ResolveServiceStateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CIMCVEngine::ResolveServiceStateL()
	{
	
	if ((!IsLoggedIn()) && (iCCHHandler->IsServiceLoggedIn()))
		{
		IM_CV_LOGS(TXT("CIMCVEngine::ResolveServiceState() NOT LOGGED IN") );
		iChatInterface->CloseAllContainers();
		CreateContextL();	
		}
		
	}
// ---------------------------------------------------------------------------
// CIMCVEngine::GetUserIdL()
// ---------------------------------------------------------------------------
//
void CIMCVEngine::GetUserIdL(TPtr& aUserId ) 
    {
	
	HBufC* userId = iCCHHandler->GetUserIdL();
	
	if(userId)
	    {
	    TPtr userIdPtr( userId->Des() );

	    aUserId.Copy( userIdPtr );
	    }
	delete userId;
	
    }


// ---------------------------------------------------------------------------
// CIMCVEngine::LoginL()
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CIMCVEngine::LoginL()
	{
	if (!IsLoggedIn())
	    {	
	    iState = ELogin;	
	    IssueRequest();	
	    // ignore codescanner warning ignored.
	    if( !iWait.IsStarted() )  
	        {
	        // Code scanner warning "active object called without checking 
	        // whether it is active or cancelling it first" ignored because
	        // CActiveSchedulerWait is not an active object
	        iWait.Start(); // CSI: 10 # See above
	        }

	    if ( EOperationInComplete == iState )	        
	        {
	        return KErrGeneral;				
	        }
	    }
	return KErrNone;
	}


// --------------------------------------------------------------------------
// CIMCVEngine::RunL
// --------------------------------------------------------------------------
//
void CIMCVEngine::RunL()
    {
    switch( iState )
        {
        case ELogin:
            {
            //cch is not logged in
            if (!iCCHHandler->IsServiceLoggedIn())
	            {
				TInt error = iCCHHandler->EnableService();
	            if (KErrNone != error)
		            {
					iState = EOperationInComplete; 	
					IssueRequest();	            	
		            }
				else
					{
					iCCHHandler->RegisterObserver(this);	
					}		            		            	
	            }
			else
				{
				//cch logged in but the ximp context is not logged in
				if (!IsLoggedIn())
					{
					TRAPD(err,CreateConnectionL());
					if(KErrNone == err )
					    {
					    iState = EOperationComplete;
					    }
					else
					    {
					    iState = EOperationInComplete;
					    }
					IssueRequest();	
					}
				}	            
            break;
            }
        
        case EOperationInComplete:
        	{
        	iCCHHandler->UnRegisterObserver();
        	if( iWait.IsStarted() )  
		        {
		        iWait.AsyncStop();  
		        }
        	break;	
        	}
        	
        case EOperationComplete:
        	{
        	iCCHHandler->UnRegisterObserver();
        	if( iWait.IsStarted() )  
		        {
		        iWait.AsyncStop();  
		        }
        	break;	
        	}
        	        	
        default:
            {
            // Do nothing
            break;
            }
        }
    }

// --------------------------------------------------------------------------
// CIMCVEngine::DoCancel
// --------------------------------------------------------------------------
//
void CIMCVEngine::DoCancel()
    {
    }

// --------------------------------------------------------------------------
// CIMCVEngine::RunError
// --------------------------------------------------------------------------
//
TInt CIMCVEngine::RunError( TInt /*aError*/ )
    {    
    return KErrNone;
    }
	

// --------------------------------------------------------------------------
// CIMCVEngine::IssueRequest
// --------------------------------------------------------------------------
//
void CIMCVEngine::IssueRequest()
    {
    if (IsActive())
	    {
	    Cancel();	
	    }

    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }


// --------------------------------------------------------------------------
// CIMCVEngine::ServiceStatusChanged
// --------------------------------------------------------------------------
//
void CIMCVEngine::ServiceStatusChanged( TInt /*aServiceId*/,     								   
	                                 TServiceState aServiceStatus )
	{
	
	switch(aServiceStatus)
		{
		case MIMCVEngineCCHObserver::ENotLoggedIn:			
			{
			iState = EOperationInComplete;
			IssueRequest();
			break;	
			}
		case MIMCVEngineCCHObserver::EConnecting:
			{
			//Nothing to be done
			break;	
			}
		case MIMCVEngineCCHObserver::ELogin:
			{
			iState = EOperationComplete;
			IssueRequest();
			break;	
			}		
		case MIMCVEngineCCHObserver::EDisconnecting:
			{
			//nothing to be done
			break;	
			}
		
		default:
			{
			break;	
			}	
		}
	}
// --------------------------------------------------------------------------
// CIMCVEngine::CloseAllOpenChatsL
// --------------------------------------------------------------------------
//
void CIMCVEngine::CloseAllOpenChatsL()
    {
	IM_CV_LOGS(TXT("CIMCVEngine::CloseAllOpenChatsL() ***************************** START") );
    iMessageHandler->CloseAllConversationL();
	IM_CV_LOGS(TXT("CIMCVEngine::CloseAllOpenChatsL() ***************************** END") );
    }
	                                      
//  End of File
