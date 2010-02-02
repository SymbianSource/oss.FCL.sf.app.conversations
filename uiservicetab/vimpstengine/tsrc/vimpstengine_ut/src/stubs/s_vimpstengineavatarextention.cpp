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
* Description: s_vimpstengineavatarextention.cpp
*
*/
// INCLUDE FILES
#include "cvimpstengineavatarextention.h"

#include <ximprequestcompleteevent.h>
#include <PresenceObjectFactory.h>
#include <presencefeatures.h>

#include <presencepublishing.h>
#include <personpresenceinfo.h>
#include <presenceinfofield.h>
#include <presenceinfofieldcollection.h>
#include <presenceinfofieldvaluebinary.h>
#include <ximpstatus.h>
#include <presenceinfo.h>

#include "cvimpstenginerequestmapper.h"
#include "cvimpstenginerequest.h"
#include "vimpstdebugprint.h"
#include "tvimpstenums.h"


#include "vimpstallerrors.h"
#include "tvimpstconsts.h"

#include "cvimpstsettingsstore.h" // settings store in cenrep used for own avatar data
#include "imconnectionproviderconsts.h"

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::NewL
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineAvatarExtention* CVIMPSTEngineAvatarExtention::NewL(
							TInt aServiceId,
							MPresenceFeatures& aFeatures,
							CVIMPSTEngineRequestMapper& aRequestMapper)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineAvatarExtention::NewL");
    CVIMPSTEngineAvatarExtention* self = 
    			CVIMPSTEngineAvatarExtention::NewLC( aServiceId, aFeatures,
    											aRequestMapper
    										    );
    CleanupStack::Pop( self );
   	CHAT_DP_FUNC_DONE("CVIMPSTEngineAvatarExtention::NewL");
    return self;
    }

// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
CVIMPSTEngineAvatarExtention* CVIMPSTEngineAvatarExtention::NewLC(
							TInt aServiceId,
							MPresenceFeatures& aFeatures,
							CVIMPSTEngineRequestMapper& aRequestMapper)
	{
	CHAT_DP_FUNC_ENTER("CVIMPSTEngineAvatarExtention::NewLC");
	CVIMPSTEngineAvatarExtention* self = new (ELeave) 
					CVIMPSTEngineAvatarExtention(aServiceId, aFeatures,aRequestMapper);
	CleanupStack::PushL( self );
	
	CHAT_DP_FUNC_DONE("CVIMPSTEngineAvatarExtention::NewLC");
	return self;
	}
    

// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::CVIMPSTEngineAvatarExtention
// ---------------------------------------------------------	
CVIMPSTEngineAvatarExtention::CVIMPSTEngineAvatarExtention(
											TInt aServiceId,
											MPresenceFeatures& aFeatures,	
											CVIMPSTEngineRequestMapper& aRequestMapper)
	:
	iServiceId(aServiceId),
	iFeatures(aFeatures),
	iRequestMapper(aRequestMapper),
	iSupported(TVIMPSTEnums::ESupportUnKnown)
	{
		
	}

// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::~CVIMPSTEngineAvatarExtention
// ---------------------------------------------------------
CVIMPSTEngineAvatarExtention::~CVIMPSTEngineAvatarExtention()
    {
	CHAT_DP_FUNC_ENTER("CVIMPSTEngineAvatarExtention::~CVIMPSTEngineAvatarExtention");
	
	CHAT_DP_FUNC_DONE("CVIMPSTEngineAvatarExtention::~CVIMPSTEngineAvatarExtention");
    }


// -----------------------------------------------------------------------------
// CVIMPSTEngineAvatarExtention::IsSupported()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::FeatureSupport CVIMPSTEngineAvatarExtention::IsSupported() const 
	{
	return iSupported;
	}
      	

// -----------------------------------------------------------------------------
// CVIMPSTEngineAvatarExtention::Type()
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::ExtentionType CVIMPSTEngineAvatarExtention::Type() const
	{
	return TVIMPSTEnums::EAvatar;
	}
	
// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::UpdateAvtarToServerL
//  
// ---------------------------------------------------------    
TInt CVIMPSTEngineAvatarExtention::UpdateAvtarToServerL(const TDesC& aFilename, const TDesC8& aMimetype)
    {
    TInt error = KErrNone;
    HBufC8* avatarContent = NULL;
    if ( aFilename.Length())
        {
        CVIMPSTEngineImageHandler* imageHandler = CVIMPSTEngineImageHandler::NewL();
        CleanupStack::PushL(imageHandler);
        // get the avatar content from the image processor
        // returns image content if the  processing succesful 
        avatarContent = imageHandler->ProcessImageFromFileL( aFilename , aMimetype);
        if ( !avatarContent )
            {
            // some problem in the image processing , so set as  general error
            error = KErrGeneral;
            }
        CleanupStack::PopAndDestroy(imageHandler); // imageHandler
        }
    // if there is no error in processing or to set default avatar 
    if( ! error)
        {
        MPresencePublishing& publisher =  iFeatures.PresencePublishing();
        MPresenceInfo* myPresence = iFeatures.PresenceObjectFactory().NewPresenceInfoLC();//1
        MPersonPresenceInfo *PersonPresence = iFeatures.PresenceObjectFactory().NewPersonPresenceInfoLC();//2
        MPresenceInfoFieldCollection& attributeFields = PersonPresence->Fields();
        MPresenceInfoField* infoField = iFeatures.PresenceObjectFactory().NewInfoFieldLC();//3
        MPresenceInfoFieldValueBinary* avatarField = iFeatures.PresenceObjectFactory().NewBinaryInfoFieldLC();//4  
        // "avatar" attribute

        if(avatarContent)
            {
            avatarField->SetBinaryValueL(*avatarContent);
            }
        else
            {
            // this is to clear the avatar 
            avatarField->SetBinaryValueL(KNullDesC8);
            }
        // Take handles to object factory and publish interface

        CleanupStack::Pop(); // (4)avatarField, pop before passing the ownership to infofiled

        //Fill presence doc with presence components and attributes
        infoField->SetFieldTypeL( NPresenceInfo::NFieldType::KAvatar ); 
        // for clear avatar mimetye is KNUllDesc so set for all the cases
        avatarField->SetMimeTypeL(aMimetype);
        infoField->SetFieldValue( avatarField ); 
        CleanupStack::Pop(); // (3)infoField ownership is passed to attributeFields

        attributeFields.AddOrReplaceFieldL(infoField );//
        CleanupStack::Pop(); // (2)PersonPresence, as ownership is transfered to mypresence

        myPresence->SetPersonPresenceL(PersonPresence); 
        TXIMPRequestId reqId = publisher.PublishOwnPresenceL( *myPresence );
        // wait completion     
        iRequestMapper.CreateRequestL(reqId, ETrue, EVIMPSTXimpOperationSetAvatar );
        if( !iReqResult )
            {
            // singleton object owned by tabbedview : dont delete 
            MVIMPSTSettingsStore* store = CVIMPSTSettingsStoreFactory ::NewL();
            if(avatarContent)
                {
                error = store->SetL(iServiceId ,EServiceAvatar ,*avatarContent);
                }
            else
                {
                error = store->SetL(iServiceId ,EServiceAvatar ,KNullDesC8);
                }
            }
        else 
            {
            error = iReqResult;
            }

        CleanupStack::Pop(); // (1) myPresence

        delete avatarContent;
        avatarContent = NULL;
        }
    // return the error 
    return error;
    }


// ---------------------------------------------------------
// CVIMPSTEngineAvatarExtention::HandleSessionContextEventL
// ---------------------------------------------------------
void CVIMPSTEngineAvatarExtention::HandleSessionContextEventL(const MXIMPContext& /*aContext*/,
                                             const MXIMPBase& aEvent,
                                             TXimpOperation aXimpOperation /*= EVIMPSTXimpOperationNoOperation*/ )
    {    
    
    CHAT_DP_FUNC_ENTER("CVIMPSTEngineAvatarExtention::HandleSessionContextEventL");
    
    switch( aEvent.GetInterfaceId() )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
        	{
            CHAT_DP_FUNC_ENTER("InsideCallbackswitch::MXIMPRequestCompleteEvent");
            if (EVIMPSTXimpOperationSetAvatar == aXimpOperation) 
	            {            
	            const MXIMPRequestCompleteEvent* event =
	                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( 
	                    aEvent, MXIMPBase::EPanicIfUnknown );
	            
	            iReqResult = event->CompletionResult().ResultCode();
	            const TXIMPRequestId& reqId = event->RequestId();
	            CVIMPSTEngineRequest *req = iRequestMapper.FindRequestId( reqId );                
	            
	            if ( req )
	                { 	                           
	                req->StopWait() ;   
	                iRequestMapper.RemoveRequestId(reqId);
	                delete req;                     
	                }
	            }
            break;
            }
            
        default:
            {
            break;
            }
        }
   	CHAT_DP_FUNC_DONE("CVIMPSTEngineAvatarExtention::HandleSessionContextEventL");	
    }    

//----------------------------------------------------------------------------
// CVIMPSTEngineAvatarExtention::SetSupported()
// -----------------------------------------------------------------------------
//
void  CVIMPSTEngineAvatarExtention::SetSupported(TVIMPSTEnums::FeatureSupport aSupported) 
    {
    iSupported = aSupported;
    }

// end of file
