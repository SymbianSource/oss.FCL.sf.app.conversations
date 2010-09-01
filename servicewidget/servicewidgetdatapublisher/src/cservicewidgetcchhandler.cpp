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
* Description:  class for cch service handling
 *                
 *
*/

#include "cservicewidgetcchhandler.h"

#include "cservicewidgetcontentpublisher.h"
#include "mservicewidgetobservers.h"
#include "servicewidgetcpglobals.h"
#include "swpdebugtrace.h"

#include <spsettings.h> 
#include <spentry.h>
#include <spproperty.h>

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::CServiceWidgetCchHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetCchHandler::CServiceWidgetCchHandler(TUint32 aServiceId, 
												   MServiceWidgetServiceStateObserver& aStateObserver ):
   iServiceId(aServiceId),
   iStateObserver( aStateObserver ),
   iServiceState( ESWSNotRegistered )
   {
   TRACE_SWP(TXT("CServiceWidgetCchHandler::CServiceWidgetCchHandler iServiceId %d"),iServiceId );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetCchHandler* CServiceWidgetCchHandler::NewL(TUint32 aServiceId, 
														 MServiceWidgetServiceStateObserver& aStateObserver  )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::NewL() start") );
    CServiceWidgetCchHandler* self = NewLC(aServiceId, aStateObserver);
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetCchHandler::NewL() end") );
    return self;
    }

// ---------------------------------------------------------------------------aOwnStatusKey
// CServiceWidgetCchHandler::NewLC
// ---------------------------------------------------------------------------
// 
CServiceWidgetCchHandler* CServiceWidgetCchHandler::NewLC(TUint32 aServiceId,
														 MServiceWidgetServiceStateObserver& aStateObserver )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::NewLC() start") );
    CServiceWidgetCchHandler* self =
        new (ELeave) CServiceWidgetCchHandler(aServiceId, aStateObserver);
    CleanupStack::PushL(self);
    self->ConstructL( );
    TRACE_SWP(TXT("CServiceWidgetCchHandler::NewLC() end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetCchHandler::ConstructL()
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ConstructL() start") );
	iCchClient = CCch::NewL();
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ConstructL() iCchClient created") );
	CCchService* service = iCchClient->GetService( iServiceId );    
	if( service )
		{
		service->AddObserver( *this );      
		}
	//Configure CCHUI notes
	MCchUi& cchui = iCchClient->CchUiApi();
	// Configure CCHUI API to show all notes/dialogs except connecting note.
	// Also configure that only VoIP and IM subservices are allowed (notes are
	// shown only for those).
	RArray<MCchUiObserver::TCchUiDialogType> allowedNotes;
	RArray<TCCHSubserviceType> allowedSubServices;
	CleanupClosePushL( allowedNotes );
	CleanupClosePushL( allowedSubServices );

	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeUsernamePasswordFailed );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeAuthenticationFailed );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeNoConnectionDefined );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeNoConnectionAvailable );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeConfirmChangeConnection );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeChangeConnection );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeDefectiveSettings );
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeErrorInConnection );  

	allowedSubServices.AppendL( ECCHVoIPSub );
	allowedSubServices.AppendL( ECCHIMSub );
	allowedSubServices.AppendL( ECCHPresenceSub );

	cchui.ConfigureVisualizationL( 
			allowedNotes, allowedSubServices );
	CleanupStack::PopAndDestroy( &allowedSubServices );
	CleanupStack::PopAndDestroy( &allowedNotes );
	
	ReadSupportedSubServices();
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ConstructL() end") );
	}

 	
// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::~CServiceWidgetCchHandler
// ---------------------------------------------------------------------------
// 
CServiceWidgetCchHandler::~CServiceWidgetCchHandler()
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::~CServiceWidgetCchHandler() start") );
	if(iCchClient)
		{
		CCchService* service = iCchClient->GetService( iServiceId );
		if( service )
		    {
		    service->RemoveObserver(*this);
		    }
		delete iCchClient; 
		}
	iSupportedSubServices.Reset();
	
	delete iServiceName;
	delete iOwnData;
	TRACE_SWP(TXT("CServiceWidgetCchHandler::~CServiceWidgetCchHandler() end") );
	}

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::ReadSupportedSubServices
// ---------------------------------------------------------------------------
//
 void CServiceWidgetCchHandler::ReadSupportedSubServices()
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ReadSupportedSubServices() start") );
	iSupportedSubServices.Reset();
	TSubServiceState subService;
	TBool supported = IsServiceSupported( ECCHIMSub );
	if( supported )
		{
		// im sub service supported
		subService.iSubServiceType = ECCHIMSub;
		subService.iSubServiceState = GetServiceState( ECCHIMSub );
		iSupportedSubServices.Append( subService );	
		}
	supported = IsServiceSupported( ECCHPresenceSub );
	if( supported )
		{
		// presence sub service supported
		subService.iSubServiceType = ECCHPresenceSub;
		subService.iSubServiceState = GetServiceState( ECCHPresenceSub );
		iSupportedSubServices.Append( subService );	
		}
	supported = IsServiceSupported( ECCHVoIPSub );
	if( supported )
		{
		// voip sub service supported
		subService.iSubServiceType = ECCHVoIPSub;
		subService.iSubServiceState = GetServiceState( ECCHVoIPSub );
		iSupportedSubServices.Append( subService );	
		}
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ReadSupportedSubServices() end") );
	}


// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::ServiceStatusChanged
// ---------------------------------------------------------------------------
//
void CServiceWidgetCchHandler::ServiceStatusChanged(
                TInt  aServiceId,
                const TCCHSubserviceType aType,
                const TCchServiceStatus& aServiceStatus )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::ServiceStatusChanged() start") );
    if( aServiceId != iServiceId )
	    {
	    return;
	    }
	TCCHSubserviceState state = aServiceStatus.State();
    TInt subServiceCount = iSupportedSubServices.Count();
    TInt error = aServiceStatus.Error();
    
    for(TInt i=0; i< subServiceCount; i++)
	    {
	    TSubServiceState subService = iSupportedSubServices[ i ]; 
	    if( subService.iSubServiceType == aType )
			{
			// sub service is matched ,supported
			// check for ALR state if supported ,service may go to waiting state
			if( error && (error != KCCHErrorInvalidSettings )  && 
				( ECCHConnecting == state ) )
				{
				// waiting state
				iSupportedSubServices[ i ].iSubServiceState = ESWSWaitingForNetwork;	
				}
			else
				{
				TSWSTRegistrationState swState = GetLocalServiceState( state );
				if( swState == ESWSRegistered && aType == ECCHPresenceSub )
					{
					// in case of presence subservice ,need to fetch the contact ,so
					// move to updating state
					iSupportedSubServices[ i ].iSubServiceState = ESWSUpdatingContacts;	
					}
				else
					{
					// store the subservice state
					iSupportedSubServices[ i ].iSubServiceState = swState;		
					}
				}
			break; // break the loop if subservice matched	
			}
		}
	TSWSTRegistrationState previousState = iServiceState;
	// get best match state
	iServiceState  = ParseGetServiceState();
	
    if( iServiceState != previousState )
		{
		// inform the observer only if state changed
		TRACE_SWP(TXT("CServiceWidgetCchHandler::ServiceStatusChanged() state changed") );
		// some times give the same state call back more than one
		TRAP_IGNORE( iStateObserver.HandleServiceStatusChangedL( iServiceState  ) );	
		}
    TRACE_SWP(TXT("CServiceWidgetCchHandler::ServiceStatusChanged() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::ParseGetServiceState
// ---------------------------------------------------------------------------
//
TSWSTRegistrationState CServiceWidgetCchHandler::ParseGetServiceState()
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ParseGetServiceState() start") );
	
	TInt serviceState = 0;
	TInt subServiceCount = iSupportedSubServices.Count();
	//Find the cumulative of the ServiceStates of all subservices
	// iterate the service array
	for ( TInt index = 0; index < subServiceCount ; index++ )        
		{	
		serviceState |= iSupportedSubServices[index].iSubServiceState;
		}
	TSWSTRegistrationState swState = ESWSNotRegistered;
	//The logic below is that, if any one subservice not enabled for us the service is not enabled
	//So to do this we assign priority for each service state
	//Connecting gets the High Priority, when copmared to Waiting for Connection,
	//Disconnecting, Registered, UnRegistered.
	//By this way the Sevice Tab will be more usable and atleast we can perform operations
	//related to the the logged in subservice
	//its upto the connection handlers to do the job properly, whether to provide connectivity to all
	//sub services or not.. 

	//Start parsing and decide the state of the ServiceTab   
	if ( serviceState & ESWSNetworkConnecting )
		{
		//Connecting	
		swState = ESWSNetworkConnecting;
		}
	else if ( serviceState & ESWSWaitingForNetwork )
		{
		//Waiting for Connection	
		swState = ESWSWaitingForNetwork;
		}
	else if ( serviceState & ESWSUpdatingContacts )
		{
		//Waiting for Connection	
		swState = ESWSUpdatingContacts;
		}	    	    
	else if ( serviceState & ESWSNetworkDisConnecting )
		{
		//disconnecting	
		swState = ESWSNetworkDisConnecting;
		}   
	else if ( serviceState & ESWSRegistered ) 
		{
		//Enabled	
		swState = ESWSRegistered;
		}
	TRACE_SWP(TXT("CServiceWidgetCchHandler::ParseGetServiceState() end") );
	return swState;   	
	}
// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::GetServiceState
// ---------------------------------------------------------------------------
// 
TSWSTRegistrationState CServiceWidgetCchHandler::GetServiceState( TCCHSubserviceType aServiceType )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::GetServiceState() start") );
    CCchService* service = iCchClient->GetService( iServiceId );
   	TCchServiceStatus status;
	status.SetState(ECCHUninitialized);
	if( service )
		{
		// Get status from the client
	    service->GetStatus(  aServiceType , status );
		}		
	TRACE_SWP(TXT("CServiceWidgetCchHandler::GetServiceState() end") );
    return GetLocalServiceState( status.State() );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::GetLocalServiceState
// ---------------------------------------------------------------------------
// 
TSWSTRegistrationState CServiceWidgetCchHandler::GetLocalServiceState(TCCHSubserviceState aState )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::GetLocalServiceState() start") );
    TSWSTRegistrationState mappedState = ESWSNotRegistered;
	if( ECCHConnecting == aState )
		{
		mappedState = ESWSNetworkConnecting;	
		}
	else if( ECCHDisconnecting == aState )
		{
		mappedState = ESWSNetworkDisConnecting;	
		}
	else if( ECCHDisabled == aState )
		{
		mappedState = ESWSNotRegistered;	
		}
	else if( ECCHEnabled == aState )
		{
		mappedState = ESWSRegistered;	
		}
	TRACE_SWP(TXT("CServiceWidgetCchHandler::GetLocalServiceState() end") );
   	return mappedState;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::OwnUserIdL
// ---------------------------------------------------------------------------
// 
const TDesC& CServiceWidgetCchHandler::OwnUserIdL()
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::OwnUserIdL() start") );	
    if( iOwnData )
		{
		delete iOwnData;
		iOwnData = NULL;	
		}
	
   // call back can not be modified ignore the error
	iOwnData = GetConParametersL( ECchUsername );
	if( iOwnData )
	    {
	    TRACE_SWP(TXT("CServiceWidgetCchHandler::OwnUserIdL() valid") );
	    TRACE_SWP(TXT("CServiceWidgetCchHandler::OwnUserIdL() end") );	
	   	return *iOwnData;	
	    }
	TRACE_SWP(TXT("CServiceWidgetCchHandler::OwnUserIdL() end") );	
    return KNullDesC ;
    }  
 
 // ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::XimpAdaptationUidL()
// ---------------------------------------------------------------------------
//
TInt CServiceWidgetCchHandler::XimpAdaptationUidL()
    {
	TRACE_SWP(TXT("CServiceWidgetCchHandler::XimpAdaptationUidL() start") );
    TInt ret = KErrNotFound;
    CSPSettings* settings = CSPSettings::NewLC(); 
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = settings->FindPropertyL( iServiceId,
                                EPropertyPCSPluginId,
                                *property );    
    if (KErrNone == err)
	    {
	    TRACE_SWP(TXT("CServiceWidgetCchHandler::XimpAdaptationUidL() uid read") );
	    property->GetValue( ret );
	    }
    CleanupStack::PopAndDestroy(2);// property ,settings
    TRACE_SWP(TXT("CServiceWidgetCchHandler::XimpAdaptationUidL() end") );
    return ret;
    }        
    
 // ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::GetConParametersL
// ---------------------------------------------------------------------------
// 
HBufC* CServiceWidgetCchHandler::GetConParametersL( TCchConnectionParameter aConnParam )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::GetConParametersL() start") );
    HBufC* temp = NULL;
    TInt error = KErrNotFound;
    if(iCchClient)  
	    {
		CCchService* service = iCchClient->GetService( iServiceId );    
		if( service )
		   {              
		   RBuf buffer;
	       CleanupClosePushL( buffer );
		   buffer.CreateL(KIMW_MAX_LENGTH);
		   error = service->GetConnectionParameter(ECCHUnknown,aConnParam,buffer);
		   User::LeaveIfError( error);
		   if( buffer.Length() > 0)
		      {
		      TRACE_SWP(TXT("CServiceWidgetCchHandler::GetConParametersL() buffer valid") );
              //extract only the user id in case
		      // Check for prefix and remove if found
              TInt prefixLocation = buffer.Locate( ':' );
              if ( KErrNotFound != prefixLocation  && ECchUsername == aConnParam )
                  {
                  temp = buffer.Mid(prefixLocation+1 ).AllocL(); // ownership transferred  
                  }
              else
                  {
                  temp = buffer.AllocL(); // ownership transferred  
                  }
		      }
		   CleanupStack::PopAndDestroy( &buffer );
		   }
	    }
	TRACE_SWP(TXT("CServiceWidgetCchHandler::GetConParametersL() end") );
    return temp;
    }

 // ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::IsServiceSupported
// ---------------------------------------------------------------------------
// 
TBool CServiceWidgetCchHandler::IsServiceSupported(TCCHSubserviceType aSubServiceType )
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::IsServiceSupported() start") );
	TBool ret = EFalse;
   	if ( iCchClient )
	   	{
	   	CCchService* service = iCchClient->GetService( iServiceId );
		if( service )
			{
			service->IsSupported(aSubServiceType, ret );
			}		
	   	}
	TRACE_SWP(TXT("CServiceWidgetCchHandler::IsServiceSupported() end") );
 	return ret;   	
 	}
    
 // ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::IsSubServiceSupported
// ---------------------------------------------------------------------------
// 
TBool CServiceWidgetCchHandler::IsSubServiceSupported(TCCHSubserviceType aSubServiceType )
	{
	TRACE_SWP(TXT("CServiceWidgetCchHandler::IsSubServiceSupported() start") );
	TBool ret = EFalse;
   	TInt subServiceCount = iSupportedSubServices.Count();
	//Find the cumulative of the ServiceStates of all subservices
	// iterate the service array
	for ( TInt index = 0; index < subServiceCount ; index++ )        
		{	
		if( iSupportedSubServices[index].iSubServiceType == aSubServiceType )
			{
			ret = ETrue;
			break;	
			}
		}
	TRACE_SWP(TXT("CServiceWidgetCchHandler::IsSubServiceSupported() end") );
 	return ret;   	
 	}   


// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::GetServiceViewIdL
// ---------------------------------------------------------------------------
// 
TInt CServiceWidgetCchHandler::GetServiceViewIdL( TInt& aTabUid )
    {
    TInt serviceviewId = 0 ;
    if( iServiceId != KErrNotFound)
        {
        CSPSettings* spSettings = CSPSettings::NewL();
        CleanupStack::PushL(spSettings);
        //get the tabview id from settings
        CSPProperty* property = CSPProperty::NewLC();
        spSettings->FindPropertyL( iServiceId,
                EPropertyContactViewId,
                *property );    
        property->GetValue(serviceviewId);
        
        //get the tab uid from settings
        spSettings->FindPropertyL( iServiceId,
                EPropertyContactViewPluginId,
                *property );    
        property->GetValue(aTabUid);
        
        CleanupStack::PopAndDestroy( property );  
        CleanupStack::PopAndDestroy( spSettings );
        }
    return serviceviewId;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetCchHandler::EnableServiceL
// 
// ---------------------------------------------------------------------------
//
TInt CServiceWidgetCchHandler::EnableServiceL( )
    {
    TRACE_SWP(TXT("CServiceWidgetCchHandler::EnableServiceL() start") );
    TInt error = KErrNotFound;

    if( iCchClient  )
        {
		iCchClient->SetConnectivityDialogsAllowed( ETrue );
        CCchService* service = iCchClient->GetService( iServiceId );
        if( service )
            {
            error = service->Enable( ECCHUnknown );
            }
        }
    return error;
    }
 // end of file
 
