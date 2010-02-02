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
* Description:  Implementation of detailsview plugin menu handler
*
*/


#include "cvimpstdetailsviewmenuhandler.h"

#include "cvimpstdetailsviewlistboxmodel.h"
#include "cvimpstdetailsviewcontainer.h"
#include "cvimpstdetailsviewplugin.h"
#include "cvimpstdetailscontacthandler.h"
#include "vimpstdetailsview.hrh"
#include <vimpstdetailsviewpluginrsc.rsg>
#include <ccappdetailsviewpluginrsc.rsg>
#include <aknnotewrappers.h>
#include <AiwContactAssignDataTypes.h>
#include <CVPbkContactLinkArray.h>

using namespace AiwContactAssign;
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewMenuHandler* CVIMPSTDetailsViewMenuHandler::NewL(
									CVIMPSTDetailsViewPlugin& aPlugin, 
									TStoreType aStoreType )
	{
	CVIMPSTDetailsViewMenuHandler* self = new(ELeave) CVIMPSTDetailsViewMenuHandler( aPlugin,aStoreType );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::~CVIMPSTDetailsViewMenuHandler
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewMenuHandler::~CVIMPSTDetailsViewMenuHandler()
	{
	delete iAiwServiceHandler;
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::CVIMPSTDetailsViewMenuHandler()
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewMenuHandler::CVIMPSTDetailsViewMenuHandler(
	CVIMPSTDetailsViewPlugin& aPlugin,
	TStoreType aStoreType) :
	iPlugin(aPlugin),
	iStoreType( aStoreType )
	{
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::ConstructL()
	{
	PrepareAiwMenusL();
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::DynInitMenuPaneL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::DynInitMenuPaneL(TInt aResourceId,
												CEikMenuPane* aMenuPane )
	{
	if (iAiwServiceHandler && iAiwServiceHandler->HandleSubmenuL(*aMenuPane))
	    {
	    return;
	    }
	switch (aResourceId)
		{
		case R_VIMPSTDETAILSVIEW_MENUPANE:
			{
			TInt pos = 0;			
			if ( iAiwServiceHandler->IsAiwMenu(aResourceId))
			    {
			    iAiwServiceHandler->InitializeMenuPaneL( *aMenuPane,
			            aResourceId, ECCAppDetailsViewAiwBaseCmd,
			            iAiwServiceHandler->InParamListL() );			           
			    } 
			if ( aMenuPane->MenuItemExists(ECCAppDetailsViewSetTopContactCmd, pos)  )
				{
				aMenuPane->SetItemDimmed(ECCAppDetailsViewSetTopContactCmd, ETrue );	
				}
			if( aMenuPane->MenuItemExists(ECCAppDetailsViewRemoveTopContactCmd, pos) )
				{
				aMenuPane->SetItemDimmed(ECCAppDetailsViewRemoveTopContactCmd, ETrue );
				}
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewNumberCmd, pos) )
				{	
				 aMenuPane->SetItemDimmed(ECCAppDetailsViewNumberCmd,ETrue );
				}
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewAddressCmd, pos) )
				{
				 aMenuPane->SetItemDimmed(ECCAppDetailsViewAddressCmd,ETrue );
				}   
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewCopyNumberCmd, pos) )
				{
				 aMenuPane->SetItemDimmed(ECCAppDetailsViewCopyNumberCmd, ETrue );
				}
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewCopyAddressCmd, pos) )
				{
				aMenuPane->SetItemDimmed(ECCAppDetailsViewCopyAddressCmd, ETrue );
				}
	    	if (aMenuPane->MenuItemExists(ECCAppDetailsViewImageCmd, pos) )
				{
				aMenuPane->SetItemDimmed(ECCAppDetailsViewImageCmd, ETrue );
				}
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewSendBusinessCardCmd, pos) )
				{
				aMenuPane->SetItemDimmed(ECCAppDetailsViewSendBusinessCardCmd, ETrue );
				}
			if (aMenuPane->MenuItemExists(ECCAppDetailsViewCopyCmd, pos) )
			    {
			    aMenuPane->SetItemDimmed(ECCAppDetailsViewCopyCmd, ETrue );
			    }
			// Dims the edit option in service details view.
			if(aMenuPane->MenuItemExists(KAiwCmdAssign, pos))
			    {                                      
			    aMenuPane->SetItemDimmed(KAiwCmdAssign,ETrue );
			    }
			
            if (aMenuPane->MenuItemExists(EVIMPSTDetailsCopyCmd, pos))
                   {
                   aMenuPane->SetItemDimmed(EVIMPSTDetailsCopyCmd, ETrue );
                   }
            if (aMenuPane->MenuItemExists(EVIMPSTDetailsDeleteCmd, pos))
                {
                aMenuPane->SetItemDimmed(EVIMPSTDetailsDeleteCmd, ETrue );
                }


			const CVIMPSTDetailsViewContainer& container =
                    static_cast<const CVIMPSTDetailsViewContainer&>( iPlugin.GetContainer() );
                    
			TInt count = container.ListBoxModel().MdcaCount();
			if( !count )
				{
				// no items hide options
				if (aMenuPane->MenuItemExists(EVIMPSTDetailsEditCmd, pos))
					{
					aMenuPane->SetItemDimmed(EVIMPSTDetailsEditCmd, ETrue );
					}
				if (aMenuPane->MenuItemExists(ECCAppDetailsViewDeleteCmd, pos) )
					{
					aMenuPane->SetItemDimmed(ECCAppDetailsViewDeleteCmd, ETrue );
					}
				if (aMenuPane->MenuItemExists(ECCAppDetailsViewCopyDetailCmd, pos) )
					{
					aMenuPane->SetItemDimmed(ECCAppDetailsViewCopyDetailCmd,ETrue );
					}
				if (aMenuPane->MenuItemExists(ECCAppDetailsViewEditItemCmd, pos) )
					{
					aMenuPane->SetItemDimmed(ECCAppDetailsViewEditItemCmd,ETrue );
					}
				}
			break;
			}
	   default:
	        break;
	    }
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::HandleNotifyL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewMenuHandler::HandleNotifyL(
    TInt aCmdId,
    TInt aEventId,
    CAiwGenericParamList& aEventParamList,
    const CAiwGenericParamList& /*aInParamList*/)
    {
    TInt result = KErrNone;

    if (aCmdId == KAiwCmdAssign)
        {
        if (aEventId == KAiwEventCompleted)
            {
            TInt index = 0;
            const TAiwGenericParam* param = aEventParamList.FindFirst(index,
                    EGenericParamContactLinkArray);
            if (param)
                {               
                TPtrC8 contactLinks = param->Value().AsData();
                CVPbkContactLinkArray* links = CVPbkContactLinkArray::NewLC(
                        contactLinks, iPlugin.GetContactHandler()->ContactStoresL());
                if(links)
                    {
                    iPlugin.GetContactHandler()->SetLinks(links, EFalse);
                    }                                 
                CleanupStack::Pop(links);                     
                }

            const TAiwGenericParam* indexparam = aEventParamList.FindFirst(index,
                    EGenericParamContactItem);
            if (indexparam)
                {
                const CVIMPSTDetailsViewContainer& container =
                static_cast<const CVIMPSTDetailsViewContainer&>(iPlugin.GetContainer());
                TInt index = indexparam->Value().AsTInt32();
                container.ListBoxModel().SetFocusedListIndex(index);
                }
            result = ETrue;
            }
        else if (aEventId == KAiwEventCanceled)
            {
            //if cancelled
            result = ETrue;    
            }
        else if (aEventId == KAiwEventQueryExit)
            {
            //issue command to cancel the previous request.
            iAiwServiceHandler->ExecuteServiceCmdL(
                    KAiwCmdAssign,
                    iAiwServiceHandler->InParamListL(),
                    iAiwServiceHandler->OutParamListL(),
                    KAiwOptCancel,
                    this);
            result = ETrue;
            }
        }

    return result;
    }
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::HandleCommandL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::HandleCommandL(TInt aCommand)
	{
	// TODO : dependency on cca , they are not deleting the menu items Then check if command is normal application command
	switch (aCommand)
		{
	    case KAiwCmdAssign:// AIW service cmd for Editing
	        {
	        if(iAiwServiceHandler && (iStoreType == EStoreLocal))
	            {
	            DoEditCmdL();
	            }	        
	        break;
	        }
		case EVIMPSTDetailsEditCmd:
		case ECCAppDetailsViewEditItemCmd:
			{
			//TODO: in case of server store nick names has to be supported
			break;	
			}
		case EVIMPSTDetailsDeleteCmd:
		case ECCAppDetailsViewDeleteCmd:
			{
			DoDeleteCmdL();
			break;
			}
	   	case EVIMPSTDetailsCopyCmd:
		case ECCAppDetailsViewCopyDetailCmd:
			{
			DoCopyCmdL();
			break;
			}
		default:
			break;
		}
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::DoEditCmdL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::DoEditCmdL()
	{
	const CVIMPSTDetailsViewContainer& container =
	static_cast<const CVIMPSTDetailsViewContainer&>(iPlugin.GetContainer());


	CAiwGenericParamList& inParamList = iAiwServiceHandler->InParamListL();

	inParamList.AppendL(TAiwGenericParam(
	        EGenericParamContactAssignData,
	        TAiwVariant(TAiwSingleContactAssignDataV1Pckg(
	                TAiwSingleContactAssignDataV1().SetFlags(EskipInfoNotes)))));

	HBufC8* packedLinks = iPlugin.GetContactHandler()->Links()->PackLC();

	inParamList.AppendL(TAiwGenericParam(EGenericParamContactLinkArray,
	        TAiwVariant( *packedLinks)));

	TInt focusIndex = container.ListBoxModel().FocusedFieldIndex();

	// TODO: use int instead of buf.
	TBuf<8> focusIndexBuf;
	focusIndexBuf.Num(focusIndex);
	const TAiwVariant focusIndexVariant(focusIndexBuf);

	inParamList.AppendL(TAiwGenericParam(EGenericParamContactItem, focusIndexVariant));

	iAiwServiceHandler->ExecuteServiceCmdL(
	        KAiwCmdAssign,
	        inParamList,
	        iAiwServiceHandler->OutParamListL(),
	        NULL,
	        this);

	CleanupStack::PopAndDestroy(packedLinks);
	    
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::DoDeleteCmdL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::DoDeleteCmdL()
	{
	CAknInformationNote* informationNote = new(ELeave) CAknInformationNote;
	informationNote->ExecuteLD(_L("Delete not supported yet"));
	}

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::DoCopyCmdL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::DoCopyCmdL()
	{
	CAknInformationNote* informationNote = new(ELeave) CAknInformationNote;
	informationNote->ExecuteLD(_L("Copy not supported yet"));
	}
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewMenuHandler::PrepareAiwMenusL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewMenuHandler::PrepareAiwMenusL()
    {
    // Initialisation of AIW Servicehandler cannot be in
    // ConstructL, since the menus of plugin are given
    // later via ProvideApplicationResourcesL(..)
    iAiwServiceHandler = CAiwServiceHandler::NewL();
    iAiwServiceHandler->AttachL(
            R_VIMPSTDETAILSVIEW_AIW_SERVICE_INTERESTS);
    
    }
// End of File
