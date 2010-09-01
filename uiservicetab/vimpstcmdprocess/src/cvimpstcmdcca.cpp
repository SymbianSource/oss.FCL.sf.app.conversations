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
* Description:  implementation for CVIMPSTCmdCCA
*
*/


// INCLUDE FILES


#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "cvimpstcmdcca.h"
#include "mvimpstengine.h"
#include "mvimpstprocessarray.h"

#include <MVPbkContactLink.h> 

#include <e32def.h>
#include <e32property.h>
#include <imconnectionproviderconsts.h>
#include "tvimpstconsts.h"
#include "vimpstdetailsviewpluginuids.hrh"

#include "uiservicetabtracer.h"

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::CVIMPSTCmdCCA
// --------------------------------------------------------------------------
//
CVIMPSTCmdCCA::CVIMPSTCmdCCA( 
        const TInt aCommandId,
            TInt& aIndex, 
            MCCAConnection* aConnection,
            MVIMPSTProcessArray& mArrayProcess,
            MVIMPSTEngine& aEngine ) :            
    iCommandId( aCommandId ),
    iIndex(aIndex),
    iConnection(aConnection),
    iArrayProcess(mArrayProcess),
    iEngine( aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::~CVIMPSTCmdCCA
// --------------------------------------------------------------------------
//
CVIMPSTCmdCCA::~CVIMPSTCmdCCA()
    {   
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdCCA* CVIMPSTCmdCCA::NewL( 
        const TInt aCommandId,TInt& aIndex,
        MCCAConnection* aConnection,
        	MVIMPSTProcessArray& aArrayProcess,
        MVIMPSTEngine& aEngine )
    {
	 TRACER_AUTO;
    CVIMPSTCmdCCA* self = new (ELeave ) CVIMPSTCmdCCA( aCommandId,aIndex,aConnection,aArrayProcess, aEngine );
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCCA::ConstructL()
    {
    RProperty::Define(KMeCoPropertyUid, KMeCoXSPDetailsServiceIdKey, RProperty::EInt,
                          ECapabilityReadUserData ,ECapabilityWriteUserData); 
 
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCCA::ExecuteLD()
    {
    TRACER_AUTO;
   	
    //push to the cleanupstack
    CleanupStack::PushL( this );    
  	
	LaunchCcaL();
    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
		
	CleanupStack::PopAndDestroy();	
		
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCCA::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCCA::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdCCA::Result() const
	{
	//return valid data regd the command operation
	return KErrNone;
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdCCA::LaunchCcaL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdCCA::LaunchCcaL()
	{   
	TRACER_AUTO;
	if ( iParameter )
		{
		iParameter->Close();
		iParameter = NULL;
		}

	// The lifetime of this cmd object is too short to hold
	// iParameterRef, caller of this must own it    
	iParameter = TCCAFactory::NewParameterL();
	iParameter->SetConnectionFlag(MCCAParameter::ENormal);
	iParameter->SetContactDataFlag(MCCAParameter::EContactLink);
	
	TUid uid( TUid::Null() );
	uid.iUid = KVIMPSTDetailsViewPluginImplmentationUid + iEngine.ServiceId(); // service details view implementation uid
	iParameter->SetLaunchedViewUid( uid );
	
	RProperty::Set(KMeCoPropertyUid, KMeCoXSPDetailsServiceIdKey, iEngine.ServiceId() ); 
	 
	MVPbkContactLink* link = iArrayProcess.ContactLink( iIndex )->CloneLC();  
	if (link)
		{
		HBufC8* link8 = link->PackLC(); 
		HBufC16* link16 = HBufC16::NewLC( link8->Length() );
		link16->Des().Copy( *link8);
		iParameter->SetContactDataL(link16->Des());
		CleanupStack::PopAndDestroy(3); 
		}
	// Sync call
	iConnection->LaunchApplicationL(*iParameter);            
	          
	}
// End of File
