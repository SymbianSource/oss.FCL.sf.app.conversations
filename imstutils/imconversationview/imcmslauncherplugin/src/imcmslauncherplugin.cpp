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
* Description:  This plugin is loaded by the Communication Launcher
*
*/


#include <ecom/implementationproxy.h>
#include <e32std.h>
#include "imcmslauncherplugin.h"
#include <imcvlauncher.h>

#include <eikappui.h>
#include <eikenv.h>

#include <apgcli.h>
#include <apacmdln.h>

#include <spsettings.h>


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Key value pair table to identify correct constructor
// function for the requested interface.
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x200255D0, CImCmsLauncherPlugin::NewL )
    };

// ---------------------------------------------------------------------------
// Exported function to return the implementation proxy table
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

// ---------------------------------------------------------------------------
// CImCmsLauncherPlugin::CImCmsLauncherPlugin()
// ---------------------------------------------------------------------------
//
CImCmsLauncherPlugin::CImCmsLauncherPlugin()
    {
    }

// ---------------------------------------------------------------------------
// CImCmsLauncherPlugin::NewL()
// ---------------------------------------------------------------------------
//
CImCmsLauncherPlugin* CImCmsLauncherPlugin::NewL()
    {
    return new( ELeave ) CImCmsLauncherPlugin();
    }

// ---------------------------------------------------------------------------
// CImCmsLauncherPlugin::~CImCmsLauncherPlugin()
// ---------------------------------------------------------------------------
//
CImCmsLauncherPlugin::~CImCmsLauncherPlugin()
    {

    }

// ---------------------------------------------------------------------------
// CImCmsLauncherPlugin::ExecuteL
// ---------------------------------------------------------------------------
//
void CImCmsLauncherPlugin::ExecuteL( const TDesC& aXspId )
    {
    
        
    // launch the UI
    TVwsViewId activeViewId;
    TInt serviceId = KErrNotFound;
    
    //Get the active view id. This used for getting the focus 
    //back to the application which launched the CV
	CCoeEnv::Static()->AppUi()->GetActiveViewId( activeViewId );	    

	// Call to launch the conversation view	
	TInt serviceIdStart = aXspId.Locate(':');
	
	//Get the User Id
	HBufC* userId = aXspId.Right(aXspId.Length() - serviceIdStart - 1).AllocLC();
	
	if (serviceIdStart)
		{
		//Check to see whether its a valid service id
		//then there is service id. set it in the RProperty
	    TInt err = KErrNone;	    	    	    
	    CDesCArrayFlat* nameArray = NULL;      	    
	    
	    RIdArray ids;
	    CleanupClosePushL( ids );
	    
	    HBufC* serviceName = aXspId.Left(serviceIdStart).AllocLC();				
				
	    CSPSettings* settings = CSPSettings::NewLC();
	    
	    nameArray = new (ELeave) CDesCArrayFlat(2);
	    CleanupStack::PushL( nameArray );    
	    
	    //Get the service table id's
	    err = settings->FindServiceIdsL( ids );  
	    User::LeaveIfError( err );  
	    
	    //get the service names   
	    err = settings->FindServiceNamesL( ids, *nameArray );  
	    User::LeaveIfError( err );     
	    
	    TInt count = nameArray->MdcaCount();
	    for ( TInt i(0); i < count; i++)
	        {
	        // search the mathching service name
	        TPtrC p = nameArray->MdcaPoint( i );
	        if (!p.CompareF( *serviceName ))
	            {
	            // We have found the service, 
	            serviceId = ids[i]; 	            
	            break;
	            }
	        }  
		CleanupStack::PopAndDestroy( 4 );	          
		
		}	
	
	if (KErrNotFound != serviceId)
		{
		//if valid service
		TInt err = IMCVLauncher::LaunchImConversationViewL(activeViewId, serviceId, userId->Des());	
		CleanupStack::PopAndDestroy();
		User::LeaveIfError(err);
		return;		
		}
	
	CleanupStack::PopAndDestroy();
	User::LeaveIfError(KErrArgument);
	
    }

// End of file

