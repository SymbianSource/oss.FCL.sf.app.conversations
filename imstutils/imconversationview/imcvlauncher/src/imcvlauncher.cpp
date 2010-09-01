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
* Description:  launcher class for launching the conversation view
*
*/


// INCLUDE FILES

#include "imcvlauncher.h"
#include "imcvuiliterals.h"

// logs file
#include "imcvlogger.h"
#include "imcvuiparams.h"

//system includes
#include <aknenv.h>

#include <e32property.h>
#include <imconnectionproviderconsts.h>
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spdefinitions.h>
#include    <MVPbkContactLink.h>
#include "xmppparams.h"
#include "xmppservicesettingsapi.h"

#include<apgtask.h> 

// Unique implementation Uid for this example
#define KIMCONTACTSEXTNIMPLEMENTATIONUID 0x20012423
#define KIMCVUIAPPEXEUID3           0x20012425
 
//-----------------------------------------------------------------------------
// IMCVLauncher::LaunchImConversationViewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
EXPORT_C TInt IMCVLauncher::LaunchImConversationViewL(TVwsViewId aActiveViewId,
														TInt aServiceId,
														const TDesC& aRecipientUserId, 
														const TDesC& aDisplayName , 
														MVPbkContactLink* aContactLink ) 
	{
	IM_CV_LOGS(TXT("IMCVLauncher::LaunchImConversationViewL start") );
	IM_CV_LOGS(TXT("IMCVLauncher:: aRecipientUserId = %s "), &aRecipientUserId );
	
	TInt err = RProperty::Define(KMeCoPropertyUid, KMeCoConvViewServiceIdKey, RProperty::EInt,
					  ECapabilityReadUserData ,ECapabilityWriteUserData);			
					  
	RProperty::Set( KMeCoPropertyUid, KMeCoConvViewServiceIdKey, aServiceId );

	//check whether its valid service and then only proceed
	if (!CheckValidServiceL(aServiceId))	
		{
		return 	KErrArgument;
		}
	// packing of data ,passed to conversation view
	TPckgBuf< TIMCVUiParams > params;
	
	params().iAppUid = aActiveViewId.iAppUid.iUid;
	params().iViewUid = aActiveViewId.iViewUid.iUid;
	params().iBuddyId = aRecipientUserId;
	params().iBuddyName = aDisplayName;
	 
	if ( aContactLink )
	    {
	    params().iContactLink = aContactLink->PackLC()->Des();
	    CleanupStack::PopAndDestroy(); //pop and destroy the hbufc8*
	    }
	       
	TApaTaskList taskList( CCoeEnv::Static()->WsSession() );
    TApaTask task = taskList.FindApp( KConversationViewAppUid );   
    if ( task.Exists() )    
		{			
		task.SendMessage(TUid::Uid( KUidApaMessageSwitchOpenFileValue ), params );
		}
	else
		{		
	// conversation view application uid and view uid
	// system call will activate the same view.
	TVwsViewId view( KConversationViewAppUid, KUidConversationsView );
	
	CCoeEnv::Static()->AppUi()->CreateActivateViewEventL(view, 
														 KUidIMCViewSoftNoteActivationId,
														 params ) ;
		}			        

	
														 
	IM_CV_LOGS(TXT("IMCVLauncher::LaunchImConversationViewL end") );   
	
	return KErrNone;
	
	}
	
	

//-----------------------------------------------------------------------------
// IMCVLauncher::CheckValidServiceL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
TBool IMCVLauncher::CheckValidServiceL(TInt aServiceId)	
	{	
	
	CSPSettings* settings = CSPSettings::NewLC();
	
	//check whether it has IMSubservice    
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = settings->FindPropertyL( aServiceId,
                                ESubPropertyIMEnabled,
                                *property );  	
	
	if ( err )
		{
		CleanupStack::PopAndDestroy( 2 );
		return EFalse;
		}
	
	//check whether it has IMSubservice valid for Meco ServiceTab
	TInt contactViewUid = KErrNotFound;
    err = settings->FindPropertyL( aServiceId,
                                EPropertyContactViewPluginId,
                                *property );  	
	if ( property )
        {
        property->GetValue( contactViewUid );
        }
	
	if ( err )
		{
		CleanupStack::PopAndDestroy( 2 );		
		return EFalse;
		}
			
	
	if ( TUid::Uid( contactViewUid ) == 
             TUid::Uid( KIMCONTACTSEXTNIMPLEMENTATIONUID ) 
             || TUid::Uid( contactViewUid ) == 
             TUid::Uid( 0x20007B6D ) 
             || TUid::Uid(contactViewUid) ==
             TUid::Uid(0x2002B334))
		{
		CleanupStack::PopAndDestroy( 2 );
		return ETrue;			
		}
	
	CleanupStack::PopAndDestroy( 2 );
	return EFalse;		
	}

// ---------------------------------------------------------------------------
// IMCVLauncher::PropertySettingsIdL()
// ---------------------------------------------------------------------------
//
TInt IMCVLauncher::PropertySettingsIdL(TUint32 aServiceId ) 
    {
    CSPSettings* settings = CSPSettings::NewLC();

    // default to english
    TInt settingsId = -1; 
    CSPProperty* property = CSPProperty::NewLC();
    settings->FindPropertyL( aServiceId,
                                ESubPropertyIMSettingsId,
                                *property );    
    if ( property )
        {
        property->GetValue( settingsId );
        }
    CleanupStack::PopAndDestroy( 2,settings );   

    return (settingsId);
    }
//  End of File
