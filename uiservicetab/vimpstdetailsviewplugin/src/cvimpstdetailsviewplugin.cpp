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
* Description:  Implementation of details view plugin
*
*/


#include "cvimpstdetailsviewplugin.h"

#include "cvimpstdetailsviewcontainer.h"
#include "cvimpstdetailsviewlistboxmodel.h"
#include "cvimpstdetailsviewmenuhandler.h"
#include "vimpstdetailsviewpluginuids.hrh"
#include "cvimpstdetailsviewbrandhandler.h"
#include "cvimpstdetailscontacthandler.h"

#include "vimpstdetailsview.hrh"
#include <vimpstui.mbg>
#include <conversations.mbg>
#include <vimpstdetailsviewpluginrsc.rsg>
#include "cvimpstdetailspresencehandler.h"

#include <imconnectionproviderconsts.h>

#include <bautils.h>
#include <AknIconUtils.h>
#include <AiwServiceHandler.h>
#include <AknsUtils.h>
#include <e32property.h>
#include <StringLoader.h>
// cca 
#include <ccappviewpluginakncontainer.h>
#include <mccapppluginparameter.h>
#include <mccaparameter.h>

// settings
#include 	<spsettings.h>
#include 	<spentry.h>
#include 	<spproperty.h>

// Virtual Phonebook
#include <CVPbkContactLinkArray.h>
#include "vimpstdebugtrace.h"
// meco service uid
#define KMECOIMPLEMENTATIONUID 0x20012423
_LIT(KVIMPSTDetailsViewDllResFileName, "\\resource\\vimpstdetailsviewpluginrsc.rsc");
_LIT8( KEMbmConversationsQgnDefaultImage,  "default_brand_image");

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPlugin* CVIMPSTDetailsViewPlugin::NewL(TInt aServiceId)
	{
	CVIMPSTDetailsViewPlugin* self = new(ELeave) CVIMPSTDetailsViewPlugin(aServiceId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin()
	{
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin start") );
	delete iMenuHandler;
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin iMenuHandler deleted") );
	delete iBrandHandler;
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin iBrandHandler deleted") );
	delete iPresenceHandler;	
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin iPresenceHandler deleted") );
	delete iContactHandler;
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::~CVIMPSTDetailsViewPlugin end") );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::CVIMPSTDetailsViewPlugin()
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPlugin::CVIMPSTDetailsViewPlugin(TInt aServiceId) : iFocusedListIndex(0),
iServiceId (aServiceId)
	{
	}
	
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::ConstructL()
	{	
	iBrandHandler = CVIMPSTDetailsViewBrandHandler::NewL( iServiceId );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::PreparePluginViewL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::PreparePluginViewL(
			MCCAppPluginParameter& aPluginParameter)
	{
	// check parameter version
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::PreparePluginViewL start") );
	if (aPluginParameter.Version() != 1)
		{
		User::Leave(KErrNotSupported);
		}
	
	if (aPluginParameter.CCAppLaunchParameter().ContactDataFlag() ==
		MCCAParameter::EContactLink)
		{
		iLaunchParameter = &aPluginParameter.CCAppLaunchParameter();
		}
	
	PrepareViewResourcesL();

	HBufC* serviceName = HBufC::NewLC( KVIMPST_MAX_LENGTH );
	TPtr serviceNamePtr = serviceName->Des() ;
	iBrandHandler->GetServiceNameL( serviceNamePtr );
	 //initialize the presence cache.
	iPresenceHandler = CVIMPSTDetailsPresenceHandler::NewL( serviceNamePtr,*this, 
	                                                        iBrandHandler->ServiceSupportedL() );	
	
	iContactHandler = CVIMPSTDetailsContactHandler::NewL( *iLaunchParameter , 
														  *this,
														  iBrandHandler->ServiceStoreUriL(),														  
														  serviceNamePtr,
														  iServiceId);
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::PreparePluginViewL iContactHandler created") );
	CleanupStack::PopAndDestroy(); // serviceName
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::PreparePluginViewL end") );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::ImplementationUid
// ---------------------------------------------------------------------------
//
TUid CVIMPSTDetailsViewPlugin::ImplementationUid()
	{
	return TUid::Uid(KVIMPSTDetailsViewPluginImplmentationUid );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::ProvideBitmapLC
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::ProvideBitmapLC(
					TCCAppIconType aIconType, CFbsBitmap*& aBitmap, CFbsBitmap*& aBitmapMask)
	{
	if (ECCAppTabIcon == aIconType)
		{
		CFbsBitmap* bmp = NULL;
		CFbsBitmap* bmpMask = NULL;
		
		// Create icon array
		iBrandHandler->LoadBitMapLC(
								bmp,
								bmpMask,
								EMbmConversationsQgn_prop_im_imsg,
								EMbmConversationsQgn_prop_im_imsg_mask
								);

		aBitmap = bmp;
		aBitmapMask = bmpMask;
		}
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::ProvideBitmapL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::ProvideBitmapL(
		TCCAppIconType aIconType, CAknIcon& aIcon)

    {
    if (ECCAppTabIcon == aIconType)
        {
        CFbsBitmap* bmp = NULL;
        CFbsBitmap* bmpMask = NULL;
        CGulIcon* tabIcon = iBrandHandler->LoadBrandBitmapL(KEMbmConversationsQgnDefaultImage);
        if(tabIcon)
            {
            bmp = tabIcon->Bitmap();
            bmpMask = tabIcon->Mask();
            aIcon.SetBitmap(bmp); // ownership transfer to aIcon
            aIcon.SetMask(bmpMask); // ownership transfer to aIcon
            // this is remove the memory leak
            tabIcon->SetBitmapsOwnedExternally( ETrue );
            delete tabIcon ;
            }
        }
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::NewContainerL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::NewContainerL()
	{
	if( iBrandHandler && iLaunchParameter )
		{
		iContainer = new (ELeave) CVIMPSTDetailsViewContainer( Id(), 
														   *iBrandHandler,
														   *iPresenceHandler,
														   iFocusedListIndex );	
		}

	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::DynInitMenuPaneL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::DynInitMenuPaneL(TInt aResourceId,
	CEikMenuPane* aMenuPane)
	{
	if (!iMenuHandler)
		{
		iMenuHandler = CVIMPSTDetailsViewMenuHandler::NewL(*this,
		                                        iContactHandler->StoreType() );
		}
	iMenuHandler->DynInitMenuPaneL(aResourceId, aMenuPane);
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::HandleCommandL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::HandleCommandL(TInt aCommand)
	{
	// CCApp handles the "Exit"- and "Back"-commands,
	// rest to menuhandler
	CCCAppViewPluginAknView::HandleCommandL(aCommand);

	iMenuHandler->HandleCommandL(aCommand);
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::Id
// ---------------------------------------------------------------------------
//
TUid CVIMPSTDetailsViewPlugin::Id() const
	{
	return TUid::Uid(KVIMPSTDetailsViewPluginImplmentationUid + iServiceId);
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::PrepareViewResourcesL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::PrepareViewResourcesL()
	{
	// Ignore codescanner warning generated here : " Leaving function called before BaseConstructL "
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::PrepareViewResourcesL start") );
	TFileName fileName(KVIMPSTDetailsViewDllResFileName);
    BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(), fileName);
   	iResourceLoader.OpenL(fileName);
    BaseConstructL(R_VIMPSTDETAILSVIEW_MAINVIEW);
    TRACED( T_LIT("CVIMPSTDetailsViewPlugin::PrepareViewResourcesL end") );
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::DoActivateL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::DoActivateL(
	const TVwsViewId& aPrevViewId,
	TUid aCustomMessageId,
	const TDesC8& aCustomMessage)
	{
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoActivateL start") );
	CCCAppViewPluginAknView::DoActivateL(aPrevViewId, aCustomMessageId,
	aCustomMessage);
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoActivateL calling GetContactData") );
	GetContactDataL();
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoActivateL end") );
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::DoDeactivate
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::DoDeactivate()
	{
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoDeactivate satrt") );
	if( iContactHandler )
		{
		iContactHandler->CancelOngoingRequest();	
		}

	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoDeactivate ongoing request canceled") );
	if (iContainer)
		{
		iFocusedListIndex = static_cast<CVIMPSTDetailsViewContainer*>
							(iContainer)->ListBoxModel().FocusedFieldIndex();
		}
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoDeactivate calling base class Deactivate") );
	CCCAppViewPluginAknView::DoDeactivate();
	// not woned by this class
	iContainer = NULL;
    TRACED( T_LIT("CVIMPSTDetailsViewPlugin::DoDeactivate end") );
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::GetContactDataL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::GetContactDataL()
	{
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL start") );
	if (iLaunchParameter && iContactHandler )
		{
		// todo: pass launchparameter to listbox model?
		HBufC& contactData = iLaunchParameter->ContactDataL();

		HBufC8* buf = HBufC8::NewLC(contactData.Length());
		buf->Des().Copy(contactData);
		TPtrC8 bufDes(buf->Des());
	
		CVPbkContactLinkArray* links = CVPbkContactLinkArray::NewLC( bufDes, iContactHandler->ContactStoresL() );
		CleanupStack::Pop(links);
		TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL links created") );
		CleanupStack::PopAndDestroy(buf);
		TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL buffer destroyed") );    
		TUid launchUid = iLaunchParameter->LaunchedViewUid();
		if(  launchUid.iUid != (KVIMPSTDetailsViewPluginImplmentationUid+iServiceId) && iContactHandler->StoreType() == EStoreServer )
			{
			TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL service store set link called from pbk") );
			// launch from other than service tab
			// ETrue, read xsp id from pbk store and search in service store and show 
			// incase of xsp store , need to find the details from xsp store
			iContactHandler->SetLinks( links , ETrue );
			}
		else
			{
			TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL set linkcalled from service tab/CV") );
			iContactHandler->SetLinks( links , EFalse );
			 //make sure this is set to null after view is changed
			// cannot be set it to null in dodeactivate call. as ilaunchparameter is not
			//owned by us. might get destryoed by cca
			iLaunchParameter->SetLaunchedViewUid(TUid::Null()); 
			}
		}
	TRACED( T_LIT("CVIMPSTDetailsViewPlugin::GetContactDataL end") );
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::SetTitleL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::SetTitleL()
	{
	HBufC* title = NULL;
	
	if( iContainer && iContactHandler )
		{
		title = static_cast<CVIMPSTDetailsViewContainer*>
								(iContainer)->GetTitleTextLC( iContactHandler->StoreType() );	
		}
	if( !title )
		{
                if(iContactHandler) 	
                  {	
		title = iContactHandler->GetDisplayNameLC();
                  } 
   		}
	 // Forward the SetTitleL-call to base-class
	if( title )
		{
		CCCAppViewPluginAknView::SetTitleL( *title );
		CleanupStack::PopAndDestroy(); // title	
		}
	}
	
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::HandlePresenceChangedL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::HandlePresenceChangedL()
	{
	if( iContainer )
		{
		static_cast<CVIMPSTDetailsViewContainer*>
								(iContainer)->PresenceChangedL();	
		}

	}    
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::HandleContactReadyL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::HandleContactReadyL( MVPbkStoreContact& aContact )
	{
	if( iContainer )
		{
		// read the service name for servic eid - iServiceId
		// pass to CreatePresentation
		HBufC* serviceName = HBufC::NewLC( KVIMPST_MAX_LENGTH );
        TPtr serviceNamePtr = serviceName->Des() ;
        iBrandHandler->GetServiceNameL( serviceNamePtr );
        
		static_cast<CVIMPSTDetailsViewContainer*>
					(iContainer)->CreatePresentationL( aContact ,*serviceName, iContactHandler->StoreType() );
	    // delete service name
		CleanupStack::PopAndDestroy();//serviceName
		}
	SetTitleL(); // call only after presentation is created	
	}

// --------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::HandleContactUnavailableL
// --------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPlugin::HandleContactUnavailableL()
	{
	if( iContainer )
		{
		static_cast<CVIMPSTDetailsViewContainer*>
					(iContainer)->CreateEmptyPresentationL();
		}
	SetTitleL(); // call only after presentation is created
	}	
			
// --------------------------------------------------------------------------
// CVIMPSTDetailsViewPlugin::GetContactHandler
// --------------------------------------------------------------------------
//
CVIMPSTDetailsContactHandler* CVIMPSTDetailsViewPlugin::GetContactHandler()
    {
    return iContactHandler;
    }
// End of File


