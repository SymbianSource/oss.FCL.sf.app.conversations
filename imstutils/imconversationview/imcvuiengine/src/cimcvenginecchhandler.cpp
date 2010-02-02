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
* Description:  Class handling the use of Cch
*
*/


// INCLUDE FILES
#include <e32def.h>
#include <cchserver.h>
#include <cchclient.h>
#include <spsettings.h>
#include <spproperty.h>
#include "cimcvenginefactory.h"

#include "cimcvenginecchhandler.h"
#include "imcvuiliterals.h"

#include "imcvlogger.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::CIMCVEngineCchHandler
// ---------------------------------------------------------------------------
// 
CIMCVEngineCchHandler::CIMCVEngineCchHandler(TUint aServiceId, 
											 CIMCVEngine& aEngine)
: iServiceId(aServiceId),
iEngine(aEngine)
    {        
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CIMCVEngineCchHandler::ConstructL(  )
    {
    // Create service selection here
	iCchClient = CCch::NewL();
	
	if (iCchClient)
		{		
		CCchService* service = iCchClient->GetService( iServiceId );	
		
		if( service )
			{
			service->SetObserver( *this );		
			}
		}		
		
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::NewL
// ---------------------------------------------------------------------------
// 
CIMCVEngineCchHandler* 
    CIMCVEngineCchHandler::NewL( TUint aServiceId, CIMCVEngine& aEngine )
    {
    CIMCVEngineCchHandler* self = NewLC(aServiceId, aEngine);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::NewLC
// ---------------------------------------------------------------------------
// 
CIMCVEngineCchHandler* 
    CIMCVEngineCchHandler::NewLC(TUint aServiceId, CIMCVEngine& aEngine )
    {
    CIMCVEngineCchHandler* self =
        new (ELeave) CIMCVEngineCchHandler(aServiceId, aEngine);
    CleanupStack::PushL(self);
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::~CIMCVEngineCchHandler
// ---------------------------------------------------------------------------
// 
CIMCVEngineCchHandler::~CIMCVEngineCchHandler()
    {
	
	if(iCchClient)
    	{
    	CCchService* service = iCchClient->GetService( iServiceId );
    	if( service )
			{
			service->RemoveObserver();
			}    	
    	delete iCchClient; 	
    	}
   
    }



// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::GetServiceState
// ---------------------------------------------------------------------------
// 
TInt CIMCVEngineCchHandler::GetServiceState(
									TCCHSubserviceState& aCCHState
									)
    {
   	TInt err = KErrNone;
   	CCchService* service = iCchClient->GetService( iServiceId );
   	TCchServiceStatus status;
	status.SetState(ECCHUninitialized);

	if( service )
		{
		// Get status from the client
		err = service->GetStatus( ECCHIMSub, status );
		}		
    
    aCCHState = status.State();
    
	return err;   	
    }


// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::EnableService
// ---------------------------------------------------------------------------
// 
TInt CIMCVEngineCchHandler::EnableService()
    {
       
    // Retrieve service interface from the CCH client to be used to
	// enable aServiceType
	CCchService* service = iCchClient->GetService( iServiceId );
	//Allowing Connectivity Dialogs for cch
    iCchClient->SetConnectivityDialogsAllowed( ETrue );

	// By default set to fault value; if service pointer is valid then we
	// return the disable value, otherwise we return KErrNotFound
	TInt error = KErrNotFound;
	if( service )
		{		
	
		error = service->Enable( ECCHUnknown );
		
		}
    

	return error;    
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::GetAdapterUidL()
// ---------------------------------------------------------------------------
//
TInt CIMCVEngineCchHandler::GetAdapterUidL()
    {
    TInt ret = KErrNotFound;
    CSPProperty* property = CSPProperty::NewLC();
    CSPSettings* settings = CSPSettings::NewLC();
    TInt err = settings->FindPropertyL( iServiceId,
            EPropertyPCSPluginId,*property );   
    if (KErrNone == err)
        {
        property->GetValue( ret );
        }
    CleanupStack::PopAndDestroy( settings );
    CleanupStack::PopAndDestroy( property );
    return ret;
    }

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::GetUserIdL
// ---------------------------------------------------------------------------
// 
HBufC* CIMCVEngineCchHandler::GetUserIdL()
    {
    HBufC* temp = NULL;
    CCchService* service = iCchClient->GetService( iServiceId );
    // By default set to fault value; if service pointer is valid then we
    // return the disable value, otherwise we return KErrNotFound
    TInt error = KErrNotFound;
    if( service )
       {                  
       RBuf username;
       username.Create(KMAXUSERIDLENGTH);
       CleanupClosePushL( username ); 
       error = service->GetConnectionParameter(ECCHIMSub,ECchUsername,username);
       User::LeaveIfError( error);
       if( username.Length() > 0)
          {
          temp = username.AllocL(); // ownership transferred 
          }       
       CleanupStack::PopAndDestroy( &username );
      }
    return temp;        
    }
    


// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::DisableService
// ---------------------------------------------------------------------------
// 
TInt CIMCVEngineCchHandler::DisableService()
    {
       
    // Retrieve service interface from the CCH client to be used to
	// enable aServiceType
	CCchService* service = iCchClient->GetService( iServiceId );
	// By default set to fault value; if service pointer is valid then we
	// return the disable value, otherwise we return KErrNotFound
	TInt error = KErrNotFound;
	if( service )
		{
		error = service->Disable( ECCHUnknown );
		}
    

	return error;    
    }


// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::IsServiceLoggedIn
// ---------------------------------------------------------------------------
//        
TBool CIMCVEngineCchHandler::IsServiceLoggedIn()
	{	
	TCCHSubserviceState serviceState = ECCHUninitialized;

	TInt error = GetServiceState( serviceState ); 
	    			
	if ( !error && ECCHEnabled == serviceState )
	    {
	    return ETrue;	
	    }
	
	return EFalse;	    
	    
	}
         	  

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::ServiceStatusChanged
// ---------------------------------------------------------------------------
//
void CIMCVEngineCchHandler::ServiceStatusChanged(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus )
	{	
	TRAP_IGNORE(DoHandleServiceStatusChangedL(aServiceId, aType, aServiceStatus));
	} 


// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::DoHandleServiceStatusChanged
// ---------------------------------------------------------------------------
//
void CIMCVEngineCchHandler::DoHandleServiceStatusChangedL(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus )
	{
	
    IM_CV_LOGS(TXT("CVEngineCCHHnadler::DoHandleServiceStatusChangedL ") );   
	
	if (aType == ECCHIMSub)
		{
		MIMCVEngineCCHObserver::TServiceState notifyEvent = 
									MIMCVEngineCCHObserver::ENotLoggedIn;
		switch (aServiceStatus.State())
			{
			case ECCHUninitialized:
				{
				//Nothing to be done			
				break;	
				}
			case ECCHDisabled:
				{
				IM_CV_LOGS(TXT("CVEngineCCHHnadler::DoHandleServiceStatusChangedL DISABLED"));   
				iEngine.CloseAllOpenChatsL ();
				iEngine.ReleaseConnectionL();
				iEngine.DeleteContextL ();

				break;	
				}
			case ECCHConnecting:
				{
				notifyEvent = MIMCVEngineCCHObserver::EConnecting;
				break;	
				}
			case ECCHEnabled:
				{
				notifyEvent = MIMCVEngineCCHObserver::ELogin;
				iEngine.CreateContextL();
				break;	
				}
			case ECCHDisconnecting:
				{
				// If NetworkErrorLost error is received by CCH on this state, then do not close all chats
				// as user would loose all the on-going conversation when the network connection is
				// restored.				
				if (aServiceStatus.Error () != KCCHErrorNetworkLost )
				iEngine.CloseAllOpenChatsL();
				notifyEvent = MIMCVEngineCCHObserver::EDisconnecting;
				break;	
				}
			default:
				{
				break;	
				}
			}
			
		if (iObserver)
			{
			iObserver->ServiceStatusChanged( aServiceId,     								   
                                 notifyEvent );
					
			}
										
		}	        
	} 

// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::RegisterObserver
// ---------------------------------------------------------------------------
//	
void CIMCVEngineCchHandler::RegisterObserver(MIMCVEngineCCHObserver* aObserver)
	{
	if (aObserver)
		{
		iObserver = aObserver;
		}
	}



// ---------------------------------------------------------------------------
// CIMCVEngineCchHandler::UnRegisterObserver
// ---------------------------------------------------------------------------
//
void CIMCVEngineCchHandler::UnRegisterObserver()
	{
	iObserver = NULL;	
	}
	        	  

// End of file

