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
* Description:  Class handling the use of Cch - All CCH Events go thro this class
*
*/



// INCLUDE FILES
#include <e32def.h>
#include <cchserver.h>
#include <cchclient.h>

#include "cvimpstenginecchhandler.h"
#include "tvimpstconsts.h"

#include "f32file.h"
#include "uiservicetabtracer.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::CVIMPSTEngineCchHandler
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler::CVIMPSTEngineCchHandler(TUint aServiceId, MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver)
: iServiceId(aServiceId),iCchUiEventObserver(aCchUiEventObserver)
    {        
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineCchHandler::ConstructL(  )
    {
	TRACER_AUTO;
    
     // Create service selection here
	 iCchClient = CCch::NewL();
	 TRACE("CCch::NewL");
	 iSettingsStore = CVIMPSTSettingsStore::NewL();
	 TRACE("CVIMPSTSettingsStore::NewL");
	
	if (iCchClient)
		{		
		CCchService* service = iCchClient->GetService( iServiceId );
		TRACE("CCch::GetService");
		
		if( service )
			{
			service->SetObserver( *this );
			TRACE("CCchService::SetObserver");
			}
		}
		
	//Configure CCHUI notes
	MCchUi& cchUiApi = iCchClient->CchUiApi();
	TRACE("CCch::CchUiApi");
	
	// Set observer to listen cchui events for change connection.
    cchUiApi.AddObserverL( *this );  // parameter is MCchUiObserver
    TRACE("MCchUi::CchUiApi");

	// Configure CCHUI API to show all notes/dialogs except connecting note.
	// Also configure that only VoIP and IM subservices are allowed (notes are
	// shown only for those).
	RArray<MCchUiObserver::TCchUiDialogType> allowedNotes;
	TRACE("MCchUiObserver::TCchUiDialogType");
	RArray<TCCHSubserviceType> allowedSubServices;
	TRACE("TCCHSubserviceType");
	CleanupClosePushL( allowedNotes );
	TRACE("allowedNotes");
	CleanupClosePushL( allowedSubServices );
	TRACE("allowedSubServices");

	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeUsernamePasswordFailed );
	TRACE("ECchUiDialogTypeUsernamePasswordFailed");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeAuthenticationFailed );
	TRACE("ECchUiDialogTypeAuthenticationFailed");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeNoConnectionDefined );
	TRACE("ECchUiDialogTypeNoConnectionDefined");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeNoConnectionAvailable );
	TRACE("ECchUiDialogTypeNoConnectionAvailable");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeConfirmChangeConnection );
	TRACE("ECchUiDialogTypeConfirmChangeConnection");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeChangeConnection );
	TRACE("ECchUiDialogTypeChangeConnection");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeDefectiveSettings );
	TRACE("ECchUiDialogTypeDefectiveSettings");
	allowedNotes.AppendL( MCchUiObserver::ECchUiDialogTypeErrorInConnection );
	TRACE("ECchUiDialogTypeErrorInConnection");

	allowedSubServices.AppendL( ECCHUnknown );
	TRACE("ECCHUnknown");
	allowedSubServices.AppendL( ECCHVoIPSub );
	TRACE("ECCHVoIPSub");
	allowedSubServices.AppendL( ECCHIMSub );
	TRACE("ECCHIMSub");
	allowedSubServices.AppendL( ECCHPresenceSub );
	TRACE("ECCHPresenceSub");

	cchUiApi.ConfigureVisualizationL( 
				allowedNotes, allowedSubServices );
	TRACE("ConfigureVisualizationL");

	CleanupStack::PopAndDestroy( &allowedSubServices );
	TRACE("allowedSubServices::pop");
	CleanupStack::PopAndDestroy( &allowedNotes );
	TRACE("allowedNotes::pop");
	
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler* CVIMPSTEngineCchHandler::NewL(TUint aServiceId,
        MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver )
        {
	TRACER_AUTO;
	TRACE("ServiceId: %d", aServiceId );
    								 
    CVIMPSTEngineCchHandler* self = NewLC(aServiceId, aCchUiEventObserver );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler* CVIMPSTEngineCchHandler::NewLC(TUint aServiceId ,MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver)
    {
	TRACER_AUTO;
    CVIMPSTEngineCchHandler* self =
        new (ELeave) CVIMPSTEngineCchHandler(aServiceId, aCchUiEventObserver);
    
    TRACE("ServiceId: %d", aServiceId );
    CleanupStack::PushL(self);
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler()
    {
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
    if(iCchClient)
    	{
    	CCchService* service = iCchClient->GetService( iServiceId );
    	if( service )
			{
			service->RemoveObserver();
			}
    	}
    delete iCchClient; 
    delete iSettingsStore;
    iObserverStructure.Reset();
    iObserverStructure.Close();
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ShutdownCch
// ---------------------------------------------------------------------------
//
//TODO: not used any where.. need to be removed.
void CVIMPSTEngineCchHandler::ShutdownCch()
    {
	TRACER_AUTO;
	TRACE("ServiceId: %d", iServiceId );
    if ( iCchClient )
        {        
        delete iCchClient;
        iCchClient = NULL;
        }
    
    }    


// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::GetServiceState
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTEngineCchHandler::GetServiceState(TUint aServiceId,
									TInt aSubServiceType, 
									TCCHSubserviceState& aCCHState
									)
    {
	TRACER_AUTO;
    
	TRACE( "ServiceId: %d, SubServiceType:%d", aServiceId, aSubServiceType );    
   	TInt err = KErrNotFound;
   	if ( iCchClient )
	   	{
	   	CCchService* service = iCchClient->GetService( aServiceId );
	   	TCchServiceStatus status;
		status.SetState(ECCHUninitialized);

		if( service )
			{
			// Get status from the client
			err = service->GetStatus( TCCHSubserviceType(aSubServiceType), status );
			if (KErrNone == err)
				{
				aCCHState = status.State();	
				}
			}		
	   	}
   	TRACE( "aCCHState: (%d)", aCCHState );

    return err;   	
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::EnableService
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTEngineCchHandler::EnableService()
    {
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
    TInt error = KErrNotFound;
    if ( iCchClient )
	    {
	    iCchClient->SetConnectivityDialogsAllowed( ETrue );

		CCchService* service = iCchClient->GetService( iServiceId );
		if( service )
			{
		TRACE( "CCHClient->Enable" );
			error = service->Enable( ECCHUnknown );		
			}
	    }
	
    TRACE( " stat: (%d)", error );
    

	return error;
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::DisableService
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTEngineCchHandler::DisableService()
    {
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	TInt error = KErrNotFound;
	
	if(iCchClient)
		{
		CCchService* service = iCchClient->GetService( iServiceId );
	    if( service )
			{
	    TRACE( "CCHClient->Disable" );	
			error = service->Disable( ECCHUnknown );
			}		
		}
	
	TRACE( "stat: (%d)", error );
			
    
    return error;
    
    }
 

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ServiceStatusChanged
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineCchHandler::ServiceStatusChanged(
				TInt aServiceId,
				const TCCHSubserviceType aType,
				const TCchServiceStatus& aServiceStatus )
    {
	TRACER_AUTO;

    // Disable the service only if the login to one of the subservices fails
    // 1.ECCHVoIPSub
    // 2.ECCHPresenceSub
    // 3.ECCHIMSub
    if( aServiceStatus.Error() == KCCHErrorLoginFailed &&
            (ECCHVoIPSub == aType ||
             ECCHPresenceSub == aType ||
             ECCHIMSub == aType))
        {
        DisableService();
        }

    if((aServiceStatus.Error() == KCCHErrorAuthenticationFailed )||(aServiceStatus.Error() == KCCHErrorLoginFailed))
        {
        TInt loginfailed = 0;
        TRAP_IGNORE( iSettingsStore->SetL(iServiceId ,EServiceSuccessfullLogin ,loginfailed) );  
        }
    TInt count  = iObserverStructure.Count();
    for(TInt i = 0; i< count; i++)
        {
        if(aType == iObserverStructure[i].iSubserviceType )
            {
            TRAP_IGNORE(iObserverStructure[i].iObserver->CchEventOccuredL( 
                    aServiceId, 
                    aServiceStatus.State(),
                    aServiceStatus.Error() ));
            }
        }

    }
	    
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::RegisterCchObserverL
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineCchHandler::RegisterCchObserverL(MVIMPSTEngineCchHandlerObserver* aObserver,
                                                   TCCHSubserviceType aSubServiceType )
    {
	TRACER_AUTO;
    if(aObserver)
    	{
    	 TObserverStructure obsstruct;
	     obsstruct.iObserver = aObserver;
	     obsstruct.iSubserviceType = aSubServiceType;
	     iObserverStructure.Append(obsstruct);
    	}
   
    }    
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::UnRegisterCchObserver
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineCchHandler::UnRegisterCchObserver(TCCHSubserviceType aSubServiceType )
    {
	TRACER_AUTO;
    TInt count  = iObserverStructure.Count();
    for(TInt i = 0; i < count; i++)
    	{
    	if(aSubServiceType == iObserverStructure[i].iSubserviceType)
    		{
    		iObserverStructure[i].iObserver = NULL;
    		iObserverStructure.Remove(i);
    		iObserverStructure.Compress();
    		break;
    		}
    	}
    }    

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::GetConParametersL
// ---------------------------------------------------------------------------
// 
HBufC* CVIMPSTEngineCchHandler::GetConParametersL(			
		    TCchConnectionParameter aConnParam )
    {
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	TRACE( "TCchConnectionParameter aConnParam: (%d)", aConnParam );
    HBufC* temp = NULL;
    TInt error = KErrNotFound;
    if(iCchClient)  
	    {
		CCchService* service = iCchClient->GetService( iServiceId );    
		if( service )
		   {              
		   RBuf buffer;
	       CleanupClosePushL( buffer );
		   buffer.CreateL(KVIMPSTUISPSMaxPropertyLength);
		   
		   TRACE( "CCHClient->GetConnectionParameter" );
		   error = service->GetConnectionParameter(ECCHUnknown,aConnParam,buffer);
		   User::LeaveIfError( error);

		   //extract only the user id in case
		   // Check for prefix and remove if found
		   TInt prefixLocation = buffer.Locate( ':' );
		   if ( KErrNotFound != prefixLocation  && ECchUsername == aConnParam )
		       {
		   TRACE( "Prefix found -> remove");
		       temp = buffer.Mid(prefixLocation+1 ).AllocL(); // ownership transferred  
		       }
		   else
		       {
		   TRACE("No Prefix found");
		       temp = buffer.AllocL(); // ownership transferred  
		       }
		   TRACE( "Return Value: Error: %d ", error );
		   CleanupStack::PopAndDestroy( &buffer );
		   }
	    }
    return temp;
    }


// ---------------------------------------------------------------------------
// Set cch connection parameter.
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineCchHandler::SetConnectionParameter(     
    TCchConnectionParameter aConnParam,
    const TDesC& aConnParamValue )
    {
	TRACER_AUTO;
    TRACE( "ServiceId: %d", iServiceId );
    TRACE( "TCchConnectionParameter aConnParam: (%d)", aConnParam );
    TInt error = KErrNotFound;
    if(iCchClient)
        {
        CCchService* service = iCchClient->GetService( iServiceId );
        TRACE( "CCHClient->SetConnectionParameter");
        if (service)
	        {
	        error =  service->SetConnectionParameter( 
	                  ECCHUnknown, aConnParam, aConnParamValue );
	        }
        }    
    TRACE( "error: %d", error );
    return error;
    }
    


        
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ChangeConnectionL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineCchHandler::ChangeConnectionL()
    {
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
   
    TInt err = KErrNone;
    if (iCchClient)
	    {
	    MCchUi& cchUiApi = iCchClient->CchUiApi();
		TRAP(err, cchUiApi.ShowDialogL(iServiceId, 
						MCchUiObserver::ECchUiDialogTypeChangeConnection););
	    }
    
    return err;    
    } 
    

// ---------------------------------------------------------------------------
// CchUiApi
// ---------------------------------------------------------------------------
//
MCchUi& CVIMPSTEngineCchHandler::CchUiApi() const
	{
	return iCchClient->CchUiApi();	
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::IsPasswordAvailableL
//  To check whether password available in settings or not
//  Returns true if password is present in the settings else returns false    
// ---------------------------------------------------------------------------
//
TBool CVIMPSTEngineCchHandler::IsPasswordAvailable(TCchConnectionParameter aConnParam )
    {
	TRACER_AUTO;
	TRACE( "TCchConnectionParameter aConnParam: (%d)", aConnParam );
    TInt passwordSet = EFalse;
    if(iCchClient)  
        {
        CCchService* service = iCchClient->GetService( iServiceId );    
        if( service )
            {              
        TRACE( "CCHClient->GetConnectionParameter");
            service->GetConnectionParameter( ECCHUnknown,aConnParam,passwordSet );
            }
        }
    return passwordSet;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ConnectivityDialogsCompletedL
// ---------------------------------------------------------------------------
//

void CVIMPSTEngineCchHandler::ConnectivityDialogsCompletedL(
        TInt aServiceId, MCchUiObserver::TCchUiOperationResult aOperationResult )
    { 
	TRACER_AUTO;
    if((iServiceId == aServiceId)&&(aOperationResult == ECchUiClientOperationResultConnectionChanged))
        {
        iCchUiEventObserver.HandleChangeConnectionEventL();
        }
    }

// End of file

