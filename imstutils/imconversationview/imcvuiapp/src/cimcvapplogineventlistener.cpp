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
* Description:   Implementation for RProperty where conversation view login event handled.
*
*/


// INCLUDE FILES

#include "cimcvapplogineventlistener.h"
#include "cimcvengine.h"
#include "cimcvappui.h"
#include "imconnectionproviderconsts.h"
#include <imcvuiapp.rsg>
#include "cimcvappwaitnote.h"
#include "mimcvenginemessagehandler.h"

//system includes
#include <avkon.hrh>
#include <aknnotewrappers.h> 

#include <StringLoader.h>

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CIMCVAppLoginEventListener::CIMCVAppLoginEventListener
// --------------------------------------------------------------------------
//
CIMCVAppLoginEventListener::CIMCVAppLoginEventListener( CIMCVEngine& aEngine, 
        			 							CIMCVAppUi& aAppUi )
    : CActive( CActive::EPriorityStandard ),
    iEngine( aEngine ),
    iAppUi( aAppUi )
    {
    CActiveScheduler::Add( this );
    }
    
// --------------------------------------------------------------------------
// CIMCVAppLoginEventListener::ConstructL
// --------------------------------------------------------------------------
//
void CIMCVAppLoginEventListener::ConstructL( const TDesC& aUserId )
    {  
    iUserId = aUserId.AllocL();
    
    RProperty::Set(KIMPropertyUid, KIMCCHLauncher, EMecoCV ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherId, *iUserId ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherErr, 0 ); //0 reset property
    User::LeaveIfError( iProperty.Attach(KIMPropertyUid, KIMCCHLauncherErr) );    
    }


// --------------------------------------------------------------------------
// CIMCVAppLoginEventListener::NewL
// --------------------------------------------------------------------------
//
CIMCVAppLoginEventListener* CIMCVAppLoginEventListener::NewL(CIMCVEngine& aEngine, 
        			 							CIMCVAppUi& aAppUi,
        			 							const TDesC& aUserId )
    {
    CIMCVAppLoginEventListener* self = new(ELeave) CIMCVAppLoginEventListener( aEngine, aAppUi );
    CleanupStack::PushL(self);
    self->ConstructL(aUserId);
    CleanupStack::Pop(self);
    return self;
    }
// ---------------------------------------------------------
// CIMCVAppLoginEventListener::~CIMCVAppLoginEventListener()
// C++ Destructor 
// ---------------------------------------------------------
//
CIMCVAppLoginEventListener::~CIMCVAppLoginEventListener()
    {
    
    //reset all properties
    RProperty::Set(KIMPropertyUid, KIMCCHLauncher, EMecoCV ); //0 reset property    
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherErr, 0 ); //0 reset property

	if (iWaitDialog)
		{
		delete iWaitDialog;  
		iWaitDialog = NULL;
		}

	
	if (iUserId)
		{
		RProperty::Set(KIMPropertyUid, KIMCCHLauncherId, *iUserId ); //0 reset property
		delete iUserId;  
		iUserId = NULL;
		}

    Cancel(); // Cancel any request, if outstanding
    iProperty.Close(); // Destroy the property object
    }
// ---------------------------------------------------------
// CIMCVAppLoginEventListener::RunL()
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::RunL()
    {
        
    TInt eventValue = 0;
    
    TInt error = RProperty::Get(KIMPropertyUid,KIMCCHLauncherErr, eventValue  );
    
    if (error)
	    {
	    iErr = KErrGeneral;
	    User::Leave( error );
	    }

    switch (eventValue)
    	{
    	case ESuccess:
    		//login is successfull;
    		// hide wait note
			if ( iWaitVisible )
				{
				TRAPD(err,LoginCompletedL() );
				if ( err != KErrNone )
					{
					User::Leave( err );
					}
				iWaitVisible = EFalse;
			
				//iEngine.DeleteContextL();
				//iEngine.CreateContextL();
				iEngine.MessageHandler().SubscribePresenceL( *iUserId );
				iErr = KErrNone;
				}
    		break;
    	
    	case EInvalid:
    		//this user is not in the fetched contact list
    		//hence show the note and close the coversation view
    		// hide wait note
			if ( iWaitVisible )
				{
				TRAPD(err,LoginCompletedL() );
				if ( err != KErrNone )
					{
					User::Leave( err );
					}
				iWaitVisible = EFalse;
				}
			//iEngine.DeleteContextL();
			//iEngine.CreateContextL();
			iErr = KErrBadHandle;			
    		break;
    		
    	case EGeneral:
    		//some error in logging
    		// hide wait note
			if ( iWaitVisible )
				{
				TRAPD(err,LoginCompletedL() );
				if ( err != KErrNone )
					{
					User::Leave( err );
					}
				iWaitVisible = EFalse;
				}
			iErr = KErrGeneral;			
			
    		break;
    	
    	default:
    		iErr = KErrGeneral;
    		break;
    		
    	}
    
    error = RProperty::Set(KIMPropertyUid,KIMCCHLauncherErr, 0  );    
    
    }
// ---------------------------------------------------------
// CIMCVAppLoginEventListener::RunL()
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::DoCancel()
    {    
    iProperty.Cancel();
    }
// ---------------------------------------------------------
// CIMCVAppLoginEventListener::StartListening()
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::StartListening()
    {
    if(!IsActive() )
        {
        iProperty.Subscribe(iStatus);
        SetActive(); // Tell scheduler a request is active  
        }
    }


// ---------------------------------------------------------
// CIMCVAppLoginEventListener::StopListening()
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::StopListening()
    {
    if(IsActive() )
        {
        Cancel(); // Cancel any request, if outstanding
        }
    }
    

// ---------------------------------------------------------
// CIMCVAppLoginEventListener::LoginL
// (other items were commented in a header).
// ---------------------------------------------------------
	
TInt CIMCVAppLoginEventListener::LoginL()
	{
	
	if( !iWaitVisible )
		{
		// launch the wait note
		iWaitVisible = ETrue;
		
		//reset the error 
		iErr = KErrNone;

		MIMCVEngineCchHandler& cchHandler = iEngine.CCHHandler();		
		
		StartListening();
		
		cchHandler.EnableService();		
		
		// load the promp from lOC file + server name
		HBufC* prompt = StringLoader::LoadLC( R_SERVTAB_LOGIN_ONGOING, 
												iEngine.ServiceName() );
	    
		iWaitDialog = CIMCVAppWaitNote::ShowWaitNoteL( 
                        *prompt, 
                        ETrue );        
        
        CleanupStack::PopAndDestroy( prompt );
        
        // start capturing the events
    	iAppUi.CaptureEventsL();
    
        if ( !iWait.IsStarted() )
            {    
            iWait.Start();  // CSI: 10 # iWait is not an active object
            }
        
        delete iWaitDialog;
        
        iWaitDialog = NULL;       
		
		StopListening();
		
		}
	
	RProperty::Set(KIMPropertyUid, KIMCCHLauncher, 0 ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherId, KNullDesC ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherErr, 0 ); //0 reset property    
    
    return iErr;
    	
	}
	

// ---------------------------------------------------------
// CIMCVAppLoginEventListener::LoginCompletedL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::LoginCompletedL()
    {    
     if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }   
    // stop capturing the events
    iAppUi.ReleaseCapturingL();        
    
    }
    


// ---------------------------------------------------------
// CIMCVAppLoginEventListener::SetUserIdL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppLoginEventListener::SetUserIdL( const TDesC& aUserId )
    { 
    delete  iUserId;
    iUserId = NULL;
    iUserId = aUserId.AllocL();
    
    RProperty::Set(KIMPropertyUid, KIMCCHLauncher, EMecoCV ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherId, *iUserId ); //0 reset property
    RProperty::Set(KIMPropertyUid, KIMCCHLauncherErr, 0 ); //0 reset property    
    }
    		        
//  End of File  

