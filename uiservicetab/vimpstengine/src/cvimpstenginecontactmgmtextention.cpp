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
* Description: Implementation for CVIMPSTEngineContactMgmtExtention
*
*/

// INCLUDE FILES
#include "cvimpstenginecontactmgmtextention.h"

#include <ximpcontext.h>
#include <ximprequestcompleteevent.h>
#include <presentitygroups.h>
#include <presentitygroupinfo.h>
#include <presentitygroupmemberinfo.h>
#include <ximpidentity.h>
#include <ximpobjectfactory.h>
#include <ximpstatus.h>

#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"
#include "tvimpstenums.h"


#include "vimpstallerrors.h"
#include "tvimpstconsts.h"
#include "vimpstdebugtrace.h"

// Constants
_LIT( KListNameAllBuddy ,"buddylist" );


// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::NewL
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineContactMgmtExtention* CVIMPSTEngineContactMgmtExtention::NewL(
							MXIMPContext& aPresenceCtx,
							MPresentityGroups& aPresGroup,
							CVIMPSTEngineRequestMapper& aRequestMapper)
    {
    TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::NewL start"));
    CVIMPSTEngineContactMgmtExtention* self = 
    			CVIMPSTEngineContactMgmtExtention::NewLC( aPresenceCtx,
    											aPresGroup,
    											aRequestMapper
    										    );
    CleanupStack::Pop( self );
   	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::NewL end"));
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineContactMgmtExtention* CVIMPSTEngineContactMgmtExtention::NewLC(
							MXIMPContext& aPresenceCtx,
							MPresentityGroups& aPresGroup,
							CVIMPSTEngineRequestMapper& aRequestMapper)
	{
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::NewLC start"));
	CVIMPSTEngineContactMgmtExtention* self = new (ELeave) 
					CVIMPSTEngineContactMgmtExtention(aPresenceCtx, aPresGroup,aRequestMapper);
	CleanupStack::PushL( self );
	
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::NewLC end") );
	return self;
	}
    

// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::CVIMPSTEngineContactMgmtExtention
// ---------------------------------------------------------	
CVIMPSTEngineContactMgmtExtention::CVIMPSTEngineContactMgmtExtention(
											MXIMPContext& aPresenceCtx,
											MPresentityGroups& aPresGroup,	
											CVIMPSTEngineRequestMapper& aRequestMapper)
	:
	iPresenceCtx(aPresenceCtx),
	iPresGroup(aPresGroup),
	iRequestMapper(aRequestMapper),
	iSupported(TVIMPSTEnums::ESupportUnKnown)
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::~CVIMPSTEngineContactMgmtExtention
// ---------------------------------------------------------
CVIMPSTEngineContactMgmtExtention::~CVIMPSTEngineContactMgmtExtention()
    {
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::~CVIMPSTEngineContactMgmtExtention start"));
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::~CVIMPSTEngineContactMgmtExtention end"));
    }


// -----------------------------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::IsSupported()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::FeatureSupport CVIMPSTEngineContactMgmtExtention::IsSupported() const 
	{
	return iSupported;
	}
      	

// -----------------------------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::Type()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::ExtentionType CVIMPSTEngineContactMgmtExtention::Type() const
	{
	return TVIMPSTEnums::EContactManagement;
	}
	

// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::DeleteServerContactL
// ---------------------------------------------------------
TInt CVIMPSTEngineContactMgmtExtention::DeleteServerContactL( const TDesC& aContactListId,
										   		   const TDesC& aUserId )
	{
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::DeleteServerContactL start"));
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::DeleteServerContactL aContactListId = %s"),&aContactListId );
	
	// err is initialized to KErrNone, b'coz if adaptation or presence is not supported then,
	// the request should not be sent to the server, this is a hack as the voip
	// writes the adaptation uid into settings even if presence is not supported.
	// bug has to be raised to voip.
	
	TInt err = KErrNone;
	
	if (iSupported == TVIMPSTEnums::ESupported)
		{
		//if there is no contact list id, contact is added to default contact list
        // here its "buddylist"
        HBufC* listId = HBufC::NewLC(KPropertyMaxLength); //  onto cleanup stack
        TPtr listIdPtr = listId->Des();
        if(0 == aContactListId.Length())
            {
            listIdPtr.Copy(KListNameAllBuddy);
            }
        else 
            {
            listIdPtr.Copy(aContactListId) ;  
		    }
		MXIMPIdentity* listIdentity = CreateIdentityLC( listIdPtr ) ;
		MXIMPIdentity* userIdentity = CreateIdentityLC( aUserId ) ;
	     
	    //not sure whether we can pass NULL for listIdentity / userIdentity
		//lets leave it to the adaptation.	
		//so ignoring the NULL  for listIdentity, userIdentity
	    TXIMPRequestId reqId = iPresGroup.RemovePresentityGroupMemberL( 
	    									*listIdentity, *userIdentity );
	       
		iRequestMapper.CreateRequestL(reqId, ETrue, EVIMPSTXimpOperationDeleteContact);// waite here	
			
		// codescanner warning can be ignored as MXIMPIdentity doesn't allow to pass pointer
		// to PopAnddestroy function. It gives panic
		CleanupStack::PopAndDestroy(); //userIdentity
		CleanupStack::PopAndDestroy(); //listIdentity
		
		CleanupStack::PopAndDestroy(); // listId;
	    
	    err = iReqResult;
		}
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::DeleteServerContactL end"));
	return err;
	}
	

// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::AddServerContactL
// ---------------------------------------------------------
TInt CVIMPSTEngineContactMgmtExtention::AddServerContactL(const TDesC& aContactList,
        									   const TDesC& aUserId,
        									   const TDesC& aNickname ,
        									   TBool aWaitToComplete /*= ETrue*/ )
	{
	
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::AddServerContactL start"));
	
	// err is initialized to KErrNone, b'coz if adaptation or presence is not supported then,
	// the request should not be sent to the server, this is a hack as the voip
	// writes the adaptation uid into settings even if presence is not supported.
	// bug has to be raised to voip.
	TInt err = KErrNone;
	
	if (iSupported == TVIMPSTEnums::ESupported)
		{
		//if there is no contact list id, contact is added to default contact list
		// here its "buddylist"
		HBufC* listId = HBufC::NewLC(KPropertyMaxLength); //  onto cleanup stack
		TPtr listIdPtr = listId->Des();
		if(0 == aContactList.Length())
		    {
		    listIdPtr.Copy(KListNameAllBuddy);
		    }
		else 
		    {
		    listIdPtr.Copy(aContactList) ;  
		    }
		
		HBufC* userId = HBufC::NewLC(KPropertyMaxLength); //  onto cleanup stack
		TPtr userIdPtr = userId->Des();
		
		//domain name is checked on the ui and command process side.
	    userIdPtr.Zero();
	    userIdPtr.Append( aUserId );
		   
		// create identity
		MXIMPIdentity* listIdentity = CreateIdentityLC( listIdPtr ) ;
		MXIMPIdentity* userIdentity = CreateIdentityLC( userIdPtr ) ; //  onto cleanup stack

		//not sure whether we can pass NULL for listIdentity / userIdentity
		//lets leave it to the adaptation.
		//so ignoring the NULL  for listIdentity, userIdentity
		TXIMPRequestId reqId = iPresGroup.AddPresentityGroupMemberL(*listIdentity,
														*userIdentity,
														aNickname) ;
	 
		iRequestMapper.CreateRequestL(reqId, aWaitToComplete, EVIMPSTXimpOperationAddContact );
		
		// codescanner warning can be ignored as MXIMPIdentity doesn't allow to pass pointer
		// to PopAnddestroy function. It gives panic
		CleanupStack::PopAndDestroy(); //userIdentity
		CleanupStack::PopAndDestroy(); //listIdentity
		
		CleanupStack::PopAndDestroy(); //userId
		CleanupStack::PopAndDestroy(); // listId
		
		TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::AddServerContactL end"));
		err = iReqResult;
		}

	return err;
	}


// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::CreateIdentityLC
// ---------------------------------------------------------	 	
MXIMPIdentity* CVIMPSTEngineContactMgmtExtention::CreateIdentityLC(const TDesC& aListId) 
	{
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::CreateIdentityLC start"));
	
	MXIMPObjectFactory& objFactory = iPresenceCtx.ObjectFactory();
	MXIMPIdentity* newIdentity = objFactory.NewIdentityLC();
	//no need to check for null as NewIdentityLC does not return null,
	// if failed ot allocate memory it will leave with KErrNoMemory
	newIdentity->SetIdentityL( aListId) ;
	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::CreateIdentityLC end"));
	return newIdentity;
	}	         	


// ---------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::HandleSessionContextEventL
// ---------------------------------------------------------
void CVIMPSTEngineContactMgmtExtention::HandleSessionContextEventL(const MXIMPContext& /*aContext*/,
                                             const MXIMPBase& aEvent,
                                             TXimpOperation aXimpOperation /*= EVIMPSTXimpOperationNoOperation*/ )
    {    
    
    TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::HandleSessionContextEventL start"));
    
    switch( aEvent.GetInterfaceId() )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
        	{
            TRACE( T_LIT("InsideCallbackswitch::MXIMPRequestCompleteEvent"));
            if ( (EVIMPSTXimpOperationAddContact == aXimpOperation) 
             		|| (EVIMPSTXimpOperationDeleteContact == aXimpOperation) )
	            {            
	            const MXIMPRequestCompleteEvent* event =
	                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
	                    aEvent, MXIMPBase::EPanicIfUnknown );
	            
	            iReqResult = event->CompletionResult().ResultCode();
	            const TXIMPRequestId& reqId = event->RequestId();
	            // Note:FindRequestId does not pass the ownership. hanece req
	            // should not be deleted.
	            CVIMPSTEngineRequest *req = iRequestMapper.FindRequestId( reqId );                
	            
	            if ( req )
	                { 	                           
	                req->StopWait() ;   
	                iRequestMapper.RemoveRequestId(reqId);
	                }
	            }
            break;
            }
            
        default:
            {
            break;
            }
        }
   	TRACE( T_LIT("CVIMPSTEngineContactMgmtExtention::HandleSessionContextEventL end"));	
    }    

//----------------------------------------------------------------------------
// CVIMPSTEngineContactMgmtExtention::SetSupported()
// -----------------------------------------------------------------------------
//
void  CVIMPSTEngineContactMgmtExtention::SetSupported(TVIMPSTEnums::FeatureSupport aSupported)
    {
    iSupported = aSupported;
    }

// end of file
