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
#include "vimpstdebugtrace.h"

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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::ConstructL"));
    
     // Create service selection here
	 iCchClient = CCch::NewL();
	
	 iSettingsStore = CVIMPSTSettingsStore::NewL();
	
	if (iCchClient)
		{		
		CCchService* service = iCchClient->GetService( iServiceId );	
		
		if( service )
			{
			service->SetObserver( *this );		
			}
		}
		
	//Configure CCHUI notes
	MCchUi& cchUiApi = iCchClient->CchUiApi();
	
	// Set observer to listen cchui events for change connection.
    cchUiApi.AddObserverL( *this );  // parameter is MCchUiObserver

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

	allowedSubServices.AppendL( ECCHUnknown );
	allowedSubServices.AppendL( ECCHVoIPSub );
	allowedSubServices.AppendL( ECCHIMSub );
	allowedSubServices.AppendL( ECCHPresenceSub );

	cchUiApi.ConfigureVisualizationL( 
				allowedNotes, allowedSubServices );

	CleanupStack::PopAndDestroy( &allowedSubServices );
	CleanupStack::PopAndDestroy( &allowedNotes );
	
	TRACE( T_LIT("CVIMPSTEngineCchHandler::ConstructL"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler* CVIMPSTEngineCchHandler::NewL(TUint aServiceId,
        MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver )
        {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::NewL start"));    
    TRACE( T_LIT("ServiceId: %d"), aServiceId );
    								 
    CVIMPSTEngineCchHandler* self = NewLC(aServiceId, aCchUiEventObserver );
    CleanupStack::Pop(self);
    TRACE( T_LIT("CVIMPSTEngineCchHandler::NewL end"));
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler* CVIMPSTEngineCchHandler::NewLC(TUint aServiceId ,MVIMPSTEngineCchUiEventObserver& aCchUiEventObserver)
    {
    CVIMPSTEngineCchHandler* self =
        new (ELeave) CVIMPSTEngineCchHandler(aServiceId, aCchUiEventObserver);
    
    TRACE( T_LIT("CVIMPSTEngineCchHandler::NewLC start"));
    TRACE( T_LIT("ServiceId: %d"), aServiceId );
    
    CleanupStack::PushL(self);
    self->ConstructL( );
    TRACE( T_LIT("CVIMPSTEngineCchHandler::NewLC end"));
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler
// ---------------------------------------------------------------------------
// 
CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler()
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler start") );
    
    TRACE( T_LIT("ServiceId: %d"), iServiceId );
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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::~CVIMPSTEngineCchHandler end"));
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ShutdownCch
// ---------------------------------------------------------------------------
//
//TODO: not used any where.. need to be removed.
void CVIMPSTEngineCchHandler::ShutdownCch()
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::ShutdownCch start"));
	TRACE( T_LIT("ServiceId: %d"), iServiceId );
	
    if ( iCchClient )
        {        
        delete iCchClient;
        iCchClient = NULL;
        }
    
    TRACE( T_LIT("CVIMPSTEngineCchHandler::ShutdownCch end"));        
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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::GetServiceState"));   
    
    TRACE( T_LIT("GetServiceState ServiceId: %d, SubServiceType:%d"),
            aServiceId, aSubServiceType );
        
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
    TRACE( T_LIT("GetServiceState - aCCHState: (%d)"), aCCHState );
    
    TRACE( T_LIT("CVIMPSTEngineCchHandler::GetServiceState"));    

    return err;   	
    }


// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::EnableService
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTEngineCchHandler::EnableService()
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::EnableService start"));    
    TRACE( T_LIT("ServiceId: %d"), iServiceId );
    
    TInt error = KErrNotFound;
    if ( iCchClient )
	    {
	    iCchClient->SetConnectivityDialogsAllowed( ETrue );

		CCchService* service = iCchClient->GetService( iServiceId );
		if( service )
			{
			TRACE( T_LIT("EnableService - CCHClient->Enable") );			
			error = service->Enable( ECCHUnknown );		
			}
	    }
	
    TRACE( T_LIT("EnableService - stat: (%d)"), error );
    
    TRACE( T_LIT("CVIMPSTEngineCchHandler::EnableService"));    

	return error;
    
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::DisableService
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTEngineCchHandler::DisableService()
    {
    TRACE( T_LIT( 
       "CVIMPSTEngineCchHandler::DisableService start"));		
	TRACE( T_LIT("ServiceId: %d"), iServiceId );
	
	TInt error = KErrNotFound;
	
	if(iCchClient)
		{
		CCchService* service = iCchClient->GetService( iServiceId );
	    if( service )
			{
			TRACE( T_LIT("DisableService - CCHClient->Disable") );	
			error = service->Disable( ECCHUnknown );
			}		
		}
	
	TRACE( T_LIT("DisableService - stat: (%d)"), error );
			
    TRACE( T_LIT("CVIMPSTEngineCchHandler::DisableService end"));
    
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

    TRACE( T_LIT("CVIMPSTEngineCchHandler::ServiceStatusChanged start"));
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

    TRACE( T_LIT("CVIMPSTEngineCchHandler::ServiceStatusChanged end"));        
    }
	    
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::RegisterCchObserverL
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineCchHandler::RegisterCchObserverL(MVIMPSTEngineCchHandlerObserver* aObserver,
                                                   TCCHSubserviceType aSubServiceType )
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::RegisterCchObserverL start" ));   
    if(aObserver)
    	{
    	 TObserverStructure obsstruct;
	     obsstruct.iObserver = aObserver;
	     obsstruct.iSubserviceType = aSubServiceType;
	     iObserverStructure.Append(obsstruct);
    	}
   
    TRACE( T_LIT("CVIMPSTEngineCchHandler::RegisterCchObserverL end"));        
    }    
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::UnRegisterCchObserver
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngineCchHandler::UnRegisterCchObserver(TCCHSubserviceType aSubServiceType )
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::UnRegisterCchObserver start" ));
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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::UnRegisterCchObserver end"));        
    }    

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::GetConParametersL
// ---------------------------------------------------------------------------
// 
HBufC* CVIMPSTEngineCchHandler::GetConParametersL(			
		    TCchConnectionParameter aConnParam )
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::GetConParametersL start"));
    TRACE( T_LIT("ServiceId: %d"), iServiceId );
    TRACE( T_LIT("GetConParametersL - TCchConnectionParameter aConnParam: (%d)"), aConnParam );
    
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
		   
		   TRACE( T_LIT("GetConParametersL - CCHClient->GetConnectionParameter") );
		   error = service->GetConnectionParameter(ECCHUnknown,aConnParam,buffer);
		   User::LeaveIfError( error);

		   //extract only the user id in case
		   // Check for prefix and remove if found
		   TInt prefixLocation = buffer.Locate( ':' );
		   if ( KErrNotFound != prefixLocation  && ECchUsername == aConnParam )
		       {
		       TRACE( T_LIT("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  Prefix found -> remove"));
		       temp = buffer.Mid(prefixLocation+1 ).AllocL(); // ownership transferred  
		       }
		   else
		       {
		       TRACE( T_LIT("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  No Prefix found"));
		       temp = buffer.AllocL(); // ownership transferred  
		       }
		   TRACE( T_LIT("GetConParametersL - Return Value: Error: %d "), error );
		   CleanupStack::PopAndDestroy( &buffer );
		   }
	    }
    TRACE( T_LIT("CVIMPSTEngineCchHandler::GetConParametersL"));  
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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::SetConnectionParameter start"));
    TRACE( T_LIT("ServiceId: %d"), iServiceId );
    TRACE( T_LIT("SetConnectionParameter - TCchConnectionParameter aConnParam: (%d)"), aConnParam );
    
    TInt error = KErrNotFound;
    if(iCchClient)
        {
        CCchService* service = iCchClient->GetService( iServiceId );
        TRACE( T_LIT("SetConnectionParameter - CCHClient->SetConnectionParameter") );

        if (service)
	        {
	        error =  service->SetConnectionParameter( 
	                  ECCHUnknown, aConnParam, aConnParamValue );
	        }
        }    
    TRACE( T_LIT("SetConnectionParameter - error: %d"), error );
    TRACE( T_LIT("CVIMPSTEngineCchHandler::SetConnectionParameter end"));   
    return error;
    }
    


        
// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ChangeConnectionL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEngineCchHandler::ChangeConnectionL()
    {
    TRACE( T_LIT("CVIMPSTEngineCchHandler::ChangeConnection start") );
    TRACE( T_LIT("ServiceId: %d"), iServiceId );
   
    TInt err = KErrNone;
    if (iCchClient)
	    {
	    MCchUi& cchUiApi = iCchClient->CchUiApi();
		TRAP(err, cchUiApi.ShowDialogL(iServiceId, 
						MCchUiObserver::ECchUiDialogTypeChangeConnection););
	    }
	TRACE( T_LIT("CVIMPSTEngineCchHandler::ChangeConnection end ")); 
    
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
    TRACE( T_LIT("CVIMPSTEngineCchHandler::IsPasswordAvailableL start"));
    TRACE( T_LIT("IsPasswordAvailableL - TCchConnectionParameter aConnParam: (%d)"), aConnParam );
    TInt passwordSet = EFalse;
    if(iCchClient)  
        {
        CCchService* service = iCchClient->GetService( iServiceId );    
        if( service )
            {              
            TRACE( T_LIT("GetConParametersL - CCHClient->GetConnectionParameter") );
            service->GetConnectionParameter( ECCHUnknown,aConnParam,passwordSet );
            }
        }
    TRACE( T_LIT("CVIMPSTEngineCchHandler::IsPasswordAvailableL end"));
    return passwordSet;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEngineCchHandler::ConnectivityDialogsCompletedL
// ---------------------------------------------------------------------------
//

void CVIMPSTEngineCchHandler::ConnectivityDialogsCompletedL(
        TInt aServiceId, MCchUiObserver::TCchUiOperationResult aOperationResult )
    { 
    if((iServiceId == aServiceId)&&(aOperationResult == ECchUiClientOperationResultConnectionChanged))
        {
        iCchUiEventObserver.HandleChangeConnectionEventL();
        }
    }

// End of file

