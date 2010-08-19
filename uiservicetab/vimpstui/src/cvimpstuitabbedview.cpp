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
* Description:   Implementation for CVIMPSTUiTabbedView
 *
*/


// INCLUDE FILES
#include "cvimpstuitabbedview.h"

#include "cvimpstcmdhandler.h"
#include "mvimpstuitabbedviewcontrol.h"
#include "mvimpstuitabcontrolfactory.h"
#include "vimpstcmd.hrh"
#include "mvimpstcmd.h"
#include "mvimpstengine.h"
#include "mvimpstenginesearchmgrextention.h"

#include "vimpstui.hrh"
#include "vimpstextentionuiuid.h"
#include "tvimpstconsts.h"
#include "cvimpstuiextensionservice.h"
#include "cvimpstuibranddata.h"
#include "mvimpstprocessarray.h"
#include "vimpstutilsdialog.h"
#include "vimpstutilswaitnote.h"
#include "vimpstcmd.hrh"
#include "cvimpstuisearchfieldaray.h"
#include "mvimpstuisearchfield.h"
#include "cvimpstuisearchquerydialog.h"
#include "cvimpstuiavatarselectionhandler.h" // avatar selection
#include "cvimpstuistatuspanehandler.h"


#include "mvimpstengineextentionfeatures.h"
#include "mvimpststoragevpbkstorehandler.h"

// System includes
#include <akntoolbar.h>             //CAknToolbar
#include <eikcolib.h>               //CEikAppUiFactory

#include <coecntrl.h>
#include <eikmenup.h>
#include <AknGlobalNote.h>
#include <eikbtgpc.h>
#include <avkon.rsg>
#include <aknnotewrappers.h> 
#include <AknQueryDialog.h> 
#include <StringLoader.h>
#include <eikenv.h>
#include <e32property.h>
#include <apgtask.h>
#include <aknmessagequerydialog.h>

// Pbk2
#include <MPbk2AppUi.h>
#include <CPbk2UIExtensionView.h>
#include <MPbk2ViewActivationTransaction.h>
#include <MPbk2ViewExplorer.h>
#include <CPbk2ViewState.h>
#include <MPbk2StartupMonitor.h>
#include <Pbk2Commands.hrh>
#include <MVPbkStoreContact.h>

#include <MPbk2ApplicationServices.h>
#include <MPbk2CommandHandler.h>
// xSP extension manager
#include "MxSPViewMapper.h"
#include "MxSPContactManager.h"
#include "cvimpststoragemanagerfactory.h"
#include "mvimpststorageserviceview.h"
#include "cvimpstuimenuextension.h"

//resource
#include <extensionmanagerres.rsg>
#include <vimpstuires.rsg>

//CCA
#include <mccaparameter.h>
#include <ccafactory.h>
#include <mccaconnection.h>

#include "uiservicetabtracer.h"

//AIW - VOIP
#include <aiwdialdataext.h>
#include <aknPopup.h>
#include <AiwCommon.hrh>
#include <AiwServiceHandler.h>
#include <aiwdialdata.h>
#include <MVPbkStoreContact.h>

//browser launch
#include <browserlauncher.h>
#include "cvimpstuitermsofusedialoghandler.h"
#include <cvimpstsettingsstore.h>
#include <aknmessagequerydialog.h>
#include "tvimpstconsts.h"
// avatar
#include "mvimpstenginepresencesubservice.h"
#include "mvimpstenginesubservice.h"
#include "mvimpstengineimsubservice.h"
#include "cvimpstuicontactselection.h"

#include    <vimpstui.mbg>
// help launch
#include <hlplch.h>
#include <coehelp.h>
#include "meco.hlp.hrh"

#include    <AiwGenericParam.h>
#include    <AiwCommon.hrh>                //KAiwCmdCall
#include    <AiwContactAssignDataTypes.h>
#include 	<commonphoneparser.h>

using namespace AiwContactAssign;
using namespace Phonebook2;

const TInt KBufLen(512);


// ================= MEMBER FUNCTIONS =======================


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CVIMPSTUiTabbedView
// --------------------------------------------------------------------------
//
CVIMPSTUiTabbedView::CVIMPSTUiTabbedView( 
MxSPViewMapper& aMapper, 
MxSPContactManager& aContactManager, 
TUint aTabbedViewId,
TUint aServiceId,
CPbk2UIExtensionView& aView,
TUid /*aId*/,
CVIMPSTUIExtensionService& aServiceData,
MVIMPSTEngine& aEngine,
MVIMPSTCmdHandler& aCommandHandler,
CVIMPSTUIMenuExtension& aMenuExtension) :
CxSPBaseView( aMapper, aContactManager, aView ),
iViewId( aTabbedViewId ),
iServiceId(aServiceId),
iEngine(aEngine),
iExtensionService( aServiceData ),
iCommandHandler( aCommandHandler),
iMenuExtension(aMenuExtension),
iSelectedAction( KErrNotFound )

            {
            iFixedToolbar = NULL;
            iIsToolBarHidden = ETrue;
            }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ConstructL
// --------------------------------------------------------------------------
//
inline void CVIMPSTUiTabbedView::ConstructL()
    {
    ViewBaseConstructL( KIMCONTACTSEXTNIMPLEMENTATIONUID, iViewId );   
    
    iCommandHandler.RegisterEventObserverL(this);

    iPreviousState = TVIMPSTEnums::ESVCENotRegistered; 

    iServiceHandler = CAiwServiceHandler::NewL();    

    iServiceHandler->AttachL( R_IM_APP_INTEREST );
    iServiceHandler->AttachMenuL( R_SERVTAB_TABBED_VIEW_MENU, 
            R_IM_APP_INTEREST );   

    iServiceHandler->AttachL( R_AIWASSIGNMENT_INTEREST );    
   	
	iServiceHandler->AttachL( R_AIWSELECT_INTEREST );

    iStatusMessage = HBufC::NewL( KStatusTextLength ); 

    iStatusPaneHandler = CCVIMPSTUiStatusPaneHandler::NewL();

    //Get SearchMgr
    MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);
    if (feature)
        {
        MVIMPSTEngineSearchMgrExtention& searchMgr = 
        MVIMPSTEngineSearchMgrExtention::Cast
        (*feature);	

        searchMgr.RegisterObserver(this);   	
        }
    iSettingsStore = CVIMPSTSettingsStore::NewL();
    
	iBrandHandler = CVIMPSTUiBrandData::NewL( iEngine );
	
	iAppUi = CCoeEnv::Static()->AppUi();
	
	CFbsBitmap *bitmap, *mask;
	iBrandHandler->GetBrandBitmapsL( &bitmap, &mask);
	iStatusPaneHandler->SetPictureL( bitmap, mask );
	
	iIsServiceHandleReSet = EFalse;
	
	iContainerCreationStarted = EFalse;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::NewL
// --------------------------------------------------------------------------
//
CVIMPSTUiTabbedView* CVIMPSTUiTabbedView::NewL( MxSPViewMapper& aMapper, 
        MxSPContactManager& aContactManager, 
        TUint aTabbedViewId,
        TUint aServiceId,
        CPbk2UIExtensionView& aView,
        TUid aId,
        CVIMPSTUIExtensionService& aServiceData,
        MVIMPSTEngine& aEngine,
        MVIMPSTCmdHandler& aCommandHandler,
        CVIMPSTUIMenuExtension& aMenuExtension)
    {
    CVIMPSTUiTabbedView* self = new(ELeave) CVIMPSTUiTabbedView( aMapper, 
            aContactManager,
            aTabbedViewId,
            aServiceId, 
            aView, 
            aId,
            aServiceData,
            aEngine,
            aCommandHandler,
            aMenuExtension);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::~CVIMPSTUiTabbedView
// --------------------------------------------------------------------------
//
CVIMPSTUiTabbedView::~CVIMPSTUiTabbedView()
    {
    delete iContactSelection;
    delete iFriendRequestId;
    
    delete iSelectedData.iPackedLinks;
    iSelectedData.iXSPList.ResetAndDestroy();
  
  	delete iQueryResult;  
    //Get SearchMgr
    MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);
    if (feature)
        {
        MVIMPSTEngineSearchMgrExtention& searchMgr = 
        MVIMPSTEngineSearchMgrExtention::Cast
        (*feature);	

        searchMgr.UnRegisterObserver(this);   	
        }

    if(iParameter!=NULL)
        iParameter->Close();    
    if(iConnection!=NULL)
        {
        iConnection->Close();
        iConnection = NULL;
        }

    if (iContainer && iAppUi )
        {
        iAppUi->RemoveFromStack( iContainer->CoeControl() );
        delete iContainer;
        iContainerCreationStarted = EFalse;
        }
    delete iBrandHandler;
    iCommandHandler.UnRegisterEventObserver(this);	

    if ( iFixedToolbar )
        {
        iFixedToolbar->SetToolbarObserver( NULL );
        delete iFixedToolbar;
        iFixedToolbar = NULL;
        }

    if (iServiceHandler)
        {
        iServiceHandler->Reset();
        delete iServiceHandler;	
        iServiceHandler = NULL;
        }

    delete iContact;

    if(iFocussedIDHistory)
        {
        delete iFocussedIDHistory;
        iFocussedIDHistory = NULL;
        }
    delete iBrLauncher; 
    delete iUserId;
   
    if(iSettingsStore)
        {
        delete iSettingsStore;
        iSettingsStore = NULL;
        }
    delete iStatusPaneHandler;

    delete iAiwParamList;
    
    if(iStatusMessage)
	    {
	    delete iStatusMessage; 
	    }

    }



// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::DynInitMenuPaneL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {    
    
    if (iEngine.IsUnInstalled())
        {
        // if engine is uninstalled, do not initiate the menu pane.
        return;
        }
    // AIW knows its own submenu hooks, so we can return from 
    // here if AIW handled this.
    ReInitializeServiceHandlerL();
    
    iMenuExtension.ResetAIWCommandMap();
    
    if ( iServiceHandler->HandleSubmenuL( *aMenuPane ) )
        {
        return;
        }

    TBool imService     = iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM);
    TBool voipService   = iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EVoip);
    TBool presenceService = iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EPresence);
    TVIMPSTEnums::TVIMPSTRegistrationState serviceTabState = 
    							iEngine.ServiceState();

    TInt itemIndex = CurrentItemIndex();
    // fetch the item and process correct type    
    TVIMPSTEnums::TItem itemType = iCommandHandler.GetProcessInterface().GetType(itemIndex);        
    TVIMPSTEnums::TOnlineStatus onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(itemIndex);        

    switch( aResourceId)
        {
        case R_EXTENSION_MANAGER_MENU:
            { 
            aMenuPane->AddMenuItemsL(R_SERVTAB_TABBED_VIEW_MENU);
            aMenuPane->AddMenuItemsL(R_TABBED_VIEW_GENERAL_MENU);

			aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
            //based on sevice states decide what all submenu should be shown
            //and main menu's to be shown
            switch (serviceTabState)
                {
                case TVIMPSTEnums::ESVCENotRegistered:
                    {                   
                    aMenuPane->SetItemDimmed(ECmdCreateNewOption, ETrue);
                    aMenuPane->SetItemDimmed(ECmdMyAvatar, ETrue);
                    aMenuPane->SetItemDimmed(ECmdChangeStatus, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState, ETrue);
                    aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState, ETrue);
                    break;	
                    }
                case TVIMPSTEnums::ESVCERegistered:
                    { 
                    if (!presenceService)
                        {
                        aMenuPane->SetItemDimmed(ECmdMyAvatar, ETrue);	
                        aMenuPane->SetItemDimmed(ECmdChangeStatus, ETrue);	
                        }
                    else
                        {
                        //find whether avatar is supported  
                         	//Get Presence SubService  			
					   	MVIMPSTEngineSubService* subService =    	   
					   	   					(iEngine.SubService(TVIMPSTEnums::EPresence));
					    if(subService)
					        {
					        MVIMPSTEnginePresenceSubService& presence = 
					        		MVIMPSTEnginePresenceSubService::Cast (*subService);
					        
					        aMenuPane->SetItemDimmed(ECmdMyAvatar, !presence.IsAvatarSupported() ); 
					        }   
        
        	           }		           		
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
       		
		       		if (!ChangeConnectionSupportedL())
		           		{
		           		aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState, ETrue);
		           		}
		           		
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState, ETrue);
                    if(iEngine.IsBlockSupportedL())
                        {
						aMenuPane->SetItemDimmed(ECmdGetBlockedList, EFalse);
                        }
                    break;	
                    }
                case TVIMPSTEnums::ESVCENetworkConnecting:
                    {                  
                    aMenuPane->SetItemDimmed(ECmdChangeStatus, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCreateNewOption, ETrue);
                    aMenuPane->SetItemDimmed(ECmdMyAvatar, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
                    aMenuPane->SetItemDimmed(ECmdSettings, ETrue);	           		
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState, ETrue);   
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState, ETrue);        		
                    if( !CancelLoginSupportedL())
                        {
                        aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                        }
                    break;	
                    }
                case TVIMPSTEnums::ESVCEWaitingForNetwork:
                case TVIMPSTEnums::ESVCEUpdatingContacts:
                    {
                    aMenuPane->SetItemDimmed(ECmdChangeStatus, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCreateNewOption, ETrue);
                    aMenuPane->SetItemDimmed(ECmdMyAvatar, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState, ETrue);	           		
		       		if (!ChangeConnectionSupportedL())
		           		{
		           		aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState, ETrue);
		           		}
                    break;	
                    }
                case TVIMPSTEnums::ESVCENetworkDisConnecting:
                    {                   
                    aMenuPane->SetItemDimmed(ECmdChangeStatus, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCreateNewOption, ETrue);
                    aMenuPane->SetItemDimmed(ECmdMyAvatar, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdSettings, ETrue);	
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninWaitingState, ETrue);
		       		aMenuPane->SetItemDimmed(ECmdChangeConnectioninOnlineState, ETrue);	           		
                    break;	
                    }
                default:
                    {
                    break;	
                    }
                }
            // common to all 
            aMenuPane->SetItemDimmed(ECmdOpenConversation, ETrue);
            aMenuPane->SetItemDimmed(ECmdContactAddAsFriend, ETrue);
            aMenuPane->SetItemDimmed(KAiwCmdCall, ETrue);
            aMenuPane->SetItemDimmed(ECmdCloseConversation, ETrue);
            aMenuPane->SetItemDimmed(ECmdContactDetails, ETrue);
            aMenuPane->SetItemDimmed(ECmdOpenFriendRequest, ETrue);
            aMenuPane->SetItemDimmed(ECmdContactEdit, ETrue);
            aMenuPane->SetItemDimmed(ECmdAIWAddToContact, ETrue); 
            aMenuPane->SetItemDimmed(ECmdAIWLinkToContact, ETrue);
            aMenuPane->SetItemDimmed(ECmdBlock, ETrue);
            aMenuPane->SetItemDimmed(ECmdUnBlock, ETrue);
            aMenuPane->SetItemDimmed(ECmdContactDelete, ETrue);
            //based on items in focus decide what main menu items needs to be shown
            switch(itemType)
                {	            	
                case TVIMPSTEnums::EOwnStatusItem:
                case TVIMPSTEnums::EInvalid:     // this case comes when no matched found for findpane characters
                    {
                    break;	
                    }
                case TVIMPSTEnums::EContactItem:
                    {      
                    //conversation related menu   
                    MVIMPSTStorageServiceView* storage = 
                    CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
                    TBool localstore = EFalse;
                    if (storage)
                        {
                        localstore = storage->IsLocalStore();
                        }
                    if (TVIMPSTEnums::ESVCERegistered == serviceTabState)
                        {
                        TPtrC selectedItem = iCommandHandler.GetProcessInterface().
                        GetItemUserId(itemIndex);
                        if( selectedItem != KNullDesC )
                            {
                            if (imService)
                                {
                                if ( iCommandHandler.GetProcessInterface().IsConversationExist( itemIndex ) )
                                    {
                                    aMenuPane->SetItemDimmed(ECmdCloseConversation, EFalse);	
                                    aMenuPane->SetItemDimmed(ECmdOpenConversation, ETrue);
                                    }
                                else
                                	{
									aMenuPane->SetItemDimmed(ECmdOpenConversation, EFalse);
                                    }
                                }
                            }
                        if(voipService)
                            {
                            TInt index;
                            aMenuPane->MenuItemExists(KAiwCmdCall, index);
                            aMenuPane->SetItemDimmed(KAiwCmdCall, EFalse);

                            CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewL();
                            CleanupStack::PushL( dialDataExt );
                            dialDataExt->SetServiceId( iServiceId );
                            ReInitializeServiceHandlerL();
                            CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
                            dialDataExt->FillInParamListL( paramList );                          
                            TAiwVariant variant;
                            TAiwGenericParam param ( EGenericParamSIPAddress, variant );
                            paramList.AppendL( param );
                            iServiceHandler->InitializeMenuPaneL( *aMenuPane, 
                                    R_SERVTAB_TABBED_VIEW_MENU,
                                    // Must not overlap with the other menu ids!
                                    ECmdAIWRangeStart,
                                    paramList );

                            CleanupStack::PopAndDestroy( dialDataExt );	

                            CEikMenuPaneItem::SData& data = aMenuPane->ItemDataByIndexL(index);
                            //add to AIW command map of the menuextension manager
                            iMenuExtension.AddToAIWCommandMap(KAiwCmdCall,data.iCommandId);
                            }                            

                        aMenuPane->SetItemDimmed(ECmdContactDetails, EFalse);  
                        
                            if (localstore)
                                {
                                //ContactEdit is enabled only for localstore
                                aMenuPane->SetItemDimmed(ECmdContactEdit, EFalse);
                                }
                            if(iEngine.IsBlockSupportedL())
                                {
                                TVIMPSTEnums::TOnlineStatus status = iCommandHandler.GetProcessInterface().GetOnlineStatusL(itemIndex); 
                                if(TVIMPSTEnums::EBlocked == status )
                                    {
                                    aMenuPane->SetItemDimmed(ECmdUnBlock, EFalse);
                                    }
                                else if( selectedItem .Length() )
                                    {
                                    aMenuPane->SetItemDimmed(ECmdBlock, EFalse);
                                    }
                                }
                            aMenuPane->SetItemDimmed(ECmdContactDelete, EFalse);    
                            
                      
                        if (!localstore)
                            {
                            aMenuPane->SetItemDimmed(ECmdAIWAddToContact, EFalse);  
                            aMenuPane->SetItemDimmed(ECmdAIWLinkToContact, EFalse);
                            }
                        }
                    else if (TVIMPSTEnums::ESVCENotRegistered == serviceTabState)
                        {
                        if(!presenceService)
                            {
                            if (localstore)
                                {
                                //ContactEdit is enabled only for localstore
                                aMenuPane->SetItemDimmed(ECmdContactEdit, EFalse);
                                }                            
                            }
                        }
                    
					break;	
                    }
                case TVIMPSTEnums::EUnknonContactItem :
                    {
                    aMenuPane->SetItemDimmed(ECmdContactAddAsFriend, EFalse);
                    aMenuPane->SetItemDimmed(ECmdContactDetails, ETrue);
                    aMenuPane->SetItemDimmed(ECmdOpenFriendRequest, ETrue);
                    aMenuPane->SetItemDimmed(ECmdContactEdit, ETrue);
                    aMenuPane->SetItemDimmed(ECmdAIWAddToContact, ETrue); 
                    aMenuPane->SetItemDimmed(ECmdAIWLinkToContact, ETrue);
                    aMenuPane->SetItemDimmed(ECmdBlock, ETrue);
                    aMenuPane->SetItemDimmed(ECmdUnBlock, ETrue);
                    aMenuPane->SetItemDimmed(ECmdContactDelete, ETrue);
                    if( iEngine.IsBlockSupportedL() )
                        {
                        aMenuPane->SetItemDimmed(ECmdBlock, EFalse);
                        }
                    if (imService)
                        {                            
                        if (iCommandHandler.GetProcessInterface().IsConversationExist( itemIndex ))
                            {
                            aMenuPane->SetItemDimmed(ECmdCloseConversation, EFalse);
                            aMenuPane->SetItemDimmed(ECmdOpenConversation, ETrue);
                            }
                        else
                        	{
							aMenuPane->SetItemDimmed(ECmdOpenConversation, EFalse);
                            }
                        }
                    if(voipService)
                        {
						TInt index;
                        aMenuPane->MenuItemExists(KAiwCmdCall, index);
								
                        aMenuPane->SetItemDimmed(KAiwCmdCall, EFalse);

                        CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewL();
                        CleanupStack::PushL( dialDataExt );
                        dialDataExt->SetServiceId( iServiceId );
                        ReInitializeServiceHandlerL();
                        CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
                        dialDataExt->FillInParamListL( paramList );                          
                        TAiwVariant variant;
                        TAiwGenericParam param ( EGenericParamSIPAddress, variant );
                        paramList.AppendL( param );
                        iServiceHandler->InitializeMenuPaneL( *aMenuPane, 
                                R_SERVTAB_TABBED_VIEW_MENU,
                                // Must not overlap with the other menu ids!
                                ECmdAIWRangeStart,
                                paramList );
                        CleanupStack::PopAndDestroy( dialDataExt ); 
						
						CEikMenuPaneItem::SData& data = aMenuPane->ItemDataByIndexL(index);
					    //add to AIW command map of the menuextension manager
                        iMenuExtension.AddToAIWCommandMap(KAiwCmdCall,data.iCommandId);
 
                        }  
                    break;
                    }
                case TVIMPSTEnums::EFriendRequestItem:
                    {
                    aMenuPane->SetItemDimmed(ECmdContactDetails, ETrue);
                    aMenuPane->SetItemDimmed(ECmdContactEdit, ETrue);
                    aMenuPane->SetItemDimmed(ECmdAIWAddToContact, ETrue); 
                    aMenuPane->SetItemDimmed(ECmdAIWLinkToContact, ETrue);
                    aMenuPane->SetItemDimmed(ECmdBlock, ETrue);
                    aMenuPane->SetItemDimmed(ECmdUnBlock, ETrue);
                    aMenuPane->SetItemDimmed(ECmdContactDelete, ETrue);
                    aMenuPane->SetItemDimmed(ECmdOpenFriendRequest, EFalse);
                    if(iEngine.IsBlockSupportedL())
                        {
                        aMenuPane->SetItemDimmed(ECmdBlock, EFalse);
                        }
                    break;  
                    }
                default:
                    {
                    break;	
                    }
                }
            // append the servicetab options(R_TABBED_VIEW_OPTIONS_MENU) and tabbed 
            // viem(R_SERVTAB_TABBED_VIEW_MENU) menu items and the the service
            // specific options and then settings,help and exit.(R_TABBED_VIEW_GENERAL_MENU)
            iMenuExtension.OfferMenuPaneToPlugins(ECmdCancelLogin,*aMenuPane,iServiceId);

            break;	
            }

        case R_SERVTAB_CREATENEW_OPTION_SUBMENU:
            {

            aMenuPane->SetItemDimmed(ECmdAddContact, ETrue);
            aMenuPane->SetItemDimmed(ECmdAddFromPhoneBook, ETrue);
            aMenuPane->SetItemDimmed(ECmdSearch, ETrue);					           		

            if (TVIMPSTEnums::ESVCERegistered == serviceTabState)            	
                {
                aMenuPane->SetItemDimmed(ECmdAddContact, EFalse);
                aMenuPane->SetItemDimmed(ECmdAddFromPhoneBook, EFalse);

                //make search request using search manager in engine   	
                MVIMPSTEngineExtentionFeatures* feature = 
                iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);

                if (feature)
                    {
                    if ( TVIMPSTEnums::ESupported == feature->IsSupported() )
                        {
                        aMenuPane->SetItemDimmed(ECmdSearch, EFalse);//show if search is supported. 
                        }
                    }
                }
            break;	
            }

        case R_SERVTAB_CHANGEAVATAR_SUBMENU: 
            {            	
            if ((TVIMPSTEnums::ESVCERegistered != serviceTabState) ||
                    (!presenceService))
                {
                aMenuPane->SetItemDimmed(ECmdAvatarGallery, ETrue);	
                aMenuPane->SetItemDimmed(ECmdAvatarNewPhoto, ETrue);	
                aMenuPane->SetItemDimmed(ECmdDefaultAvatar, ETrue);	
                }            
            break;	
            }

        case R_TABBED_VIEW_GENERAL_MENU:
            {
            switch (serviceTabState)
                {
                case TVIMPSTEnums::ESVCENotRegistered:
                    {
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
					aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
                    break;	
                    }
                case TVIMPSTEnums::ESVCERegistered:
                    {
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                    break;	
                    }
                case TVIMPSTEnums::ESVCENetworkConnecting:
                    {
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
					aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
                    aMenuPane->SetItemDimmed(ECmdSettings, ETrue);	           		
                    if( !CancelLoginSupportedL())
                        {
                        aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                        }
                    break;	
                    }
                case TVIMPSTEnums::ESVCEWaitingForNetwork:
                case TVIMPSTEnums::ESVCEUpdatingContacts:
                    {
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
					aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
                    break;	
                    }
                case TVIMPSTEnums::ESVCENetworkDisConnecting:
                    {
                    aMenuPane->SetItemDimmed(ECmdLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdCancelLogin, ETrue);
                    aMenuPane->SetItemDimmed(ECmdSettings, ETrue);
					aMenuPane->SetItemDimmed(ECmdGetBlockedList, ETrue);
                    aMenuPane->SetItemDimmed(ECmdLogout, ETrue);
                    break;	
                    }
                default:
                    {
                    break;	
                    }


                }
            break;	
            }
        }                

    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleCommandL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleCommandL(TInt aCommand)
    {
    //offer to extensions first, then to aiw.
    if(iMenuExtension.OfferHandleCommandToPlugins(aCommand))
        {
        //command is executed, hence return.
        return;
        }
    ReInitializeServiceHandlerL();
    if ( iServiceHandler )
    		{
    		TInt serviceCommandId =iServiceHandler->ServiceCmdByMenuCmd(aCommand);
				
		    if ( iContainer && serviceCommandId == KAiwCmdCall )
		        {
		       	MakeVoipCallL();
		        return;
		        }	
    	  }
    if( iContainer )
        {
        iContainer->SetClrFindPaneFlag(ETrue);
        }
    
    switch (aCommand)
        {
        case ECmdLogin:
            {						
            LoginL();			
            break;
            }
        case ECmdSearch:
            {
            SearchContactsL();						
            break;
            }
        case ECmdAddFromPhoneBook:
            {
           
            LaunchFetchDialogL();						
            break;
            }
        case EAknCmdHelp:
            {
            TBool imSupported = iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM);
            TBool voipSupported = iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EVoip);
            if ( imSupported )
                { 
                if (  voipSupported  )
                    { // both Im and VOIP are enabled 
                    LaunchHelpL( KSERVTAB_HLP_MAIN_VIEW_VOIPIM );
                    }
                else
                    {// only IM is enabled
                    LaunchHelpL( KSERVTAB_HLP_MAIN_VIEW_IM );
                    }
                }
            else if ( voipSupported )
                { // only VOIP is enabled
                LaunchHelpL( KSERVTAB_HLP_MAIN_VIEW_VOIP );
                }   
            
            break;
            }
        case EAknCmdExit:
        case EAknSoftkeyExit:
            { 
            //exit from phone book.
            Pbk2AppUi()->HandleCommandL( EAknSoftkeyExit );

            break;
            }
        case ECmdSettings:
            {
             LaunchSettingsViewL();
             break;
            }		

        case ECmdLogout:
            {                        
            // this is for actual network logout          
            iCommandHandler.HandleCommandL(ELogoutService, this, &iServiceId);
            break;
            } 

        case ECmdContactDetails:        
            {
            TInt index = KErrNotFound;                        
            if (iContainer)
                {				
                index = iContainer->CurrentItemIndex(); 
                }
            if(KErrNotFound != index)
                {
                TLaunchCCAData data;
                data.iIndex = index;
                iConnection = TCCAFactory::NewConnectionL();
                data.iConnection = iConnection;
                iCommandHandler.HandleCommandL(ELaunchCCA, this, &data );
                }
            break;
            }
        case ECmdOpenConversation:
            {
            if( iContainer )
                {
                iContainer->SendMessageL();
                }

            break;
            }

        case ECmdAddContact:
            {
            AddContactL( R_IM_ADD_CONTACT_MANUAL_QUERY );
            break;
            }

        case ECmdContactEdit:
            { 
            EditContactL();           
            break;
            }
        case ECmdContactDelete:        
            {
            if( iContainer )
                {
                iContainer->SetClrFindPaneFlag(EFalse);
                }
            DeleteContactL();
            break;
            }
        case ECmdChangeStatus:
            {
            ChangeStatusProcessL();
            break;
            } 
        case ECmdCancelLogin:
            {
            iCommandHandler.HandleCommandL( ECancelLogin , this , NULL );
            break;
            }                     

        case ECmdOpenFriendRequest:
            {
            ProcessFriendRequestL();
            break;
            }

        case ECmdAvatarGallery:
        case ECmdAvatarNewPhoto:     
            {
            CAvatarSelectionHandler* avatarHandler = CAvatarSelectionHandler::NewLC();
            HBufC* avatarFile = avatarHandler->HandleAvatarSelectionL( aCommand );
            if( avatarFile ) // if user selected the image 
                {
                CleanupStack::PushL( avatarFile ); 
                if(iWaitNote)
	                {
	                delete iWaitNote;
	                iWaitNote = NULL;	
	                }
                iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_SERVTAB_CHANGE_AVATAR_CHANGING, EFalse , ETrue );
                ChangeOwnAvatarL( *avatarFile , avatarHandler->MimeTypeL() );
                CleanupStack::PopAndDestroy(); //avatarFile
                }
	        CleanupStack::PopAndDestroy(); // avatarHandler
            break;
            }
       case ECmdDefaultAvatar:
            {
            ChangeOwnAvatarL( KNullDesC , KNullDesC8 );
            break;
            }        

        case ECmdCloseConversation:
            {
            CloseConversationL();
            break;
            }
    	case ECmdChangeConnectioninWaitingState:
		case ECmdChangeConnectioninOnlineState:
			{
			iEngine.ChangeConnectionL();
			break;	
			}
		case ECmdBlock:        
            {
            if( iContainer )
                {
                iContainer->SetClrFindPaneFlag(EFalse);
                }
            BlockContactL(ETrue);
            break;
            }
		case ECmdUnBlock:        
            {
            if( iContainer )
                {
                iContainer->SetClrFindPaneFlag(EFalse);
                }
            BlockContactL();
            break;
            }
        case ECmdGetBlockedList:
	        {
			ShowBlockedContactListL();
			break;
	        }
        case ECmdAIWAddToContact:
       	case ECmdAIWLinkToContact:
        	{
            CreateOrLinkToPhoneBookL(aCommand);
           	break;	
        	}
       	case ECmdContactAddAsFriend:
       	    {
       	    TInt index = KErrNotFound;                        
       	    if (iContainer)
       	        {               
       	        index = iContainer->CurrentItemIndex(); 
       	        }
       	    MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
       	    TVIMPSTEnums::TItem itemType = arrayprocess.GetType( index );
       	    if( itemType ==  TVIMPSTEnums::EUnknonContactItem ) 
       	        {
       	        TPtrC userId = arrayprocess.GetItemUserId( index );
       	        AddContactL( R_IM_ADD_CONTACT_MANUAL_QUERY , userId );
       	        }
       	    break;
       	    }
			       	case EAknCmdHideInBackground:
       	    {
       	    //in case of EAknCmdHideInBackground phonebook be in 
       	    //background and all the child application should close

       	    if(iConnection)//  close connection to close cca
       	        {
       	        iConnection->Close();
       	        iConnection = NULL;
       	        }
       	    if (iServiceHandler)//reset selectionhandler so that it closes all the phonebook forms if open
       	        {
       	        iServiceHandler->Reset();
       	        iIsServiceHandleReSet = ETrue;
       	        }
       	    
            Pbk2AppUi()->HandleCommandL( aCommand );

       	    break;
       	    }
        default:
            {
            Pbk2AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }



// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::DoActivateViewL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::DoActivateViewL( const TVwsViewId& aPrevViewId,
        TUid /*aCustomMessageId*/,
        const TDesC8& aCustomMessage)
    {	
    // When ST is launched from widget or universal indicator plugin
    // user clicks twice continuously, then DoActivateL is called
    // twice, as iContainer is NULL since construction is not complete
    // then it creates two controls (containers), hence to avoid this
    // situation, we are checking if construction of iContainer has 
    // started, then we ignore DoActivateL second time.  
    MPbk2ViewActivationTransaction* viewActivationTransaction =
    Pbk2AppUi()->Pbk2ViewExplorer()->HandleViewActivationLC(
            iView.Id(),
            aPrevViewId,
            &iEngine.ServiceName(),
            NULL,
            EUpdateAll );

    // Notifies startup monitor of view activation
    if( Pbk2AppUi()->Pbk2StartupMonitor() )
        {
        Pbk2AppUi()->Pbk2StartupMonitor()->
        NotifyViewActivationL( iView.Id() );
        }
    
    if (iEngine.IsUnInstalled())
        {
    // engine is uninstalled, disable left softkey i.e options and provide only exit.
        iView.Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_EXIT);
        }
    else
        {
    // engine is constructed completely, provide tabbed view softkeys.
        iView.Cba()->SetCommandSetL(R_IM_CONTACTS_TABBED_VIEW_SOFTKEYS);
        }
    
	
	ReadAndSetOwnUserIdL();

    if (!iContainer&& !iContainerCreationStarted)
        {
        iContainerCreationStarted = ETrue;
        iContainer = MVIMPSTUiTabbedViewControlFactory::NewTabControlL(*this,
                &iView,iCommandHandler, iServiceId,
                *iBrandHandler, iEngine, iView.Cba());
        // take the control pointer
        CCoeControl* cCoeControl = iContainer->CoeControl();
        iAppUi->AddToStackL( iView, cCoeControl );
        cCoeControl->SetMopParent( &iView );        
        cCoeControl->SetRect( iView.ClientRect() ); 
        cCoeControl->ActivateL();
        } 
     iStatusPaneHandler->SetTextL( iEngine.ServiceName() );
    // Commit application-wide state changes
    viewActivationTransaction->Commit();
    CleanupStack::PopAndDestroy(); //viewActivationTransaction    
    
    //CreateToolBarL();
    
    if(TVIMPSTEnums::ESVCERegistered  == iEngine.ServiceState())
        {
        if(iIsToolBarHidden)
            {
            ShowHideFixedToolbarL(ETrue);
            }
        }
    else
        {
        if(!iIsToolBarHidden)
            {
            ShowHideFixedToolbarL(EFalse);
            }
        }
    
    if(iContainer && iFocussedIDHistory )
        {
        iContainer->SetFocussedId( iFocussedIDHistory->Des() );
        } 
    
    /*
     * aCustomMessage has following format if it is coming from Service Widget
     * when you click on widget 
     * we need to unpack data in similar format 
     * version number --> stream.WriteInt8L(1);
     * EEnd --> stream.WriteInt8L( 0 );
     * Service Id --> stream.WriteInt32L( iServiceId );
     * 
     * If function call is coming from other than service widget then 
     * aCustomMessage will not have data in above format
     */
    
    if(aCustomMessage.Length())
        {        
        RDesReadStream stream( aCustomMessage);
        stream.PushL();
        TInt serviceId = KErrNotFound;
        TRAP_IGNORE( const TUint versionNumber = stream.ReadUint8L(); //equal to 1
        TInt type = stream.ReadInt8L(); // equal to eend
        serviceId = stream.ReadInt32L(); );
        CleanupStack::PopAndDestroy();//stream
        if( serviceId ==  iServiceId  )
            {
            // this case will get excuted only when there is 
            // fresh login from service widget for first time only
            // uid has been taken for autologin 
            // if the service id is matches to aCustomMessage 
            // it is auto login ,called from service widget
            // start the login procedure            
            LoginL();            
            }       
        }
    UpdateToolbarL();	
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::DoDeactivate
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::DoDeactivate()
    {

    if(iFixedToolbar)
        {
        iIsToolBarHidden = ETrue;
        delete iFixedToolbar;
        iFixedToolbar = NULL;
        }   
    
    if (iContainer)
        {   
        delete iFocussedIDHistory;
    	iFocussedIDHistory = NULL;
        TRAP_IGNORE( iFocussedIDHistory = iContainer->GetFocussedId().AllocL() );
        iAppUi->RemoveFromStack( iContainer->CoeControl() );
        delete iContainer;
        iContainer = NULL;
        iContainerCreationStarted = EFalse;
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleForegroundEventL
// --------------------------------------------------------------------------
// 
TAny* CVIMPSTUiTabbedView::UIExtensionViewExtension(
                TUid aExtensionUid )
    {
    if (aExtensionUid == KMPbk2UIExtensionView2Uid)
        {
        return static_cast<MPbk2UIExtensionView2*>(this);
        }
    return NULL;
    }
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleForegroundEventL
// --------------------------------------------------------------------------
//
 
 void CVIMPSTUiTabbedView::HandleForegroundEventL(TBool aForeground)
	 {
	 if( aForeground && TVIMPSTEnums::ESVCENotRegistered == iEngine.ServiceState() )
		 {
		 // user might has changed the setting read the new and refresh the view
		 // user can change setting in offline only
		 ReadAndSetOwnUserIdL();	
		 }
	 }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleCommandKeyL
// --------------------------------------------------------------------------
//
TBool CVIMPSTUiTabbedView::HandleCommandKeyL(const TKeyEvent& aKeyEvent, TEventCode /*aType*/)
    {
    TBool result = EFalse;
    switch (aKeyEvent.iCode)
        {        
        default:
            {
            break;
            }
        }
    return result;
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleStatusPaneSizeChange
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleStatusPaneSizeChange()
    {
    // Resize the container to fill the client rectangle
    if (iContainer)
        {
        iContainer->CoeControl()->SetRect( iView.ClientRect() );         
        }
    }    

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CommandHandlerL
// --------------------------------------------------------------------------
//
MVIMPSTCmdHandler& CVIMPSTUiTabbedView::CommandHandlerL()
    {    
    return iCommandHandler;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CommandFinishedL
// --------------------------------------------------------------------------
//

void CVIMPSTUiTabbedView::CommandFinishedL(
        const MVIMPSTCmd& aCommand )
    {
      TRACER_AUTO;
    //handle the observer for commands issues thro HandleCommandL()

    switch (aCommand.CommandId())
        {
        case ELoginService:
            {
            //login command is complete
            //use aCommand.Result() to get the data     
            //Ownership is not given to the caller            
            break;
            }
        case ELogoutService:
            {
            //logout command is complete
            //use aCommand.Result() to get the data 
            //Ownership is not given to the caller               
            break;
            } 
        case ECmdProcessAcceptedContactNew: // accept create new 
        case ECmdProcessAcceptedContactExist: // update existing
        case ECmdProcessSelectedContactNew: 
        case ECmdProcessSelectedContactExist:// add from phonebook
        case EAddContact:
            {
            //addcontact command is complete
            //use aCommand.Result() to get the data 
            //Ownership is not given to the caller
            //some error occured.
            if (iWaitNote)
                {
                delete iWaitNote;
                iWaitNote = NULL;   
                }
            if(aCommand.Result() != KErrNone) 
                {
                ShowErrorNoteL( aCommand.Result() );
                }
            break;
            } 
        case EDeleteContact:
            {
            //addcontact command is complete
            //use aCommand.Result() to get the data 
            //Ownership is not given to the caller  
            //some error occured.
            if (iWaitNote)
                {
                delete iWaitNote;
                iWaitNote = NULL;   
                }
            if(aCommand.Result() != KErrNone) 
                {
                HBufC* prompt = NULL;
                prompt = StringLoader::LoadLC( R_IM_ERROR_SERVICE_ERR  );
                TInt ret( VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) );
                CleanupStack::PopAndDestroy(prompt);
                }
            break;
            } 
        case EChangeOwnStatus: 
            {
            TInt error = aCommand.Result();
            if( KErrNone == error )
                {
                // show confirmation note
                HBufC* text = CEikonEnv::Static()->AllocReadResourceLC( R_QTN_CHAT_CHANGE_STATUS_CHANGED );
                CAknConfirmationNote* dlg = new( ELeave )CAknConfirmationNote();
                dlg->ExecuteLD( *text );
                CleanupStack::PopAndDestroy( text );
                //get the status from the engine and update in the ui.
                if(iContainer)
                    {
                    iContainer->HandleItemAdditionL();
                    }

                }
            else if( KErrCancel == error )
                {
                // user cancelled, status was not updated
                HBufC* text = NULL;
                text = StringLoader::LoadLC( R_QTN_IM_CHANGE_STATUS_NOT_CHANGED );
                VIMPSTUtilsDialog::DisplayNoteDialogL( *text  );
                CleanupStack::PopAndDestroy(); //onlineText
                }
            else
                {
                // change status failed
                HBufC* failed = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_STATUS_FAILED );
                VIMPSTUtilsDialog::DisplayErrorNoteL( *failed );
                CleanupStack::PopAndDestroy( failed );
                }
            break;
            }
        case EChangeOwnMessage:
            {
            TInt error = aCommand.Result();
            if( KErrNone == error )
                {
                // show note qtn_chat_change_message_changing
                HBufC* prompt = NULL;
                prompt = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_MESSAGE_CHANGED );
                VIMPSTUtilsDialog::DisplayNoteDialogL( *prompt);
                CleanupStack::PopAndDestroy(prompt);
                }
            else if( KErrCancel == error)
                {
                HBufC* prompt = NULL;
                prompt = StringLoader::LoadLC( R_QTN_IM_CHANGE_STATUS_NOT_CHANGED );
                VIMPSTUtilsDialog::DisplayNoteDialogL( *prompt);
                CleanupStack::PopAndDestroy(prompt);
                }
            else
                {
                HBufC* prompt = NULL;
                prompt = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_MESSAGE_FAILED );
                VIMPSTUtilsDialog::DisplayNoteDialogL(*prompt );
                CleanupStack::PopAndDestroy(prompt);
                }

            break;
            }
        case ESearch:
            {
            // Search command is complete
            //use aCommand.Result() to get the data 
            if(iWaitNote)
                {
                delete iWaitNote;
                iWaitNote = NULL;	
                }
            if(aCommand.Result() != KErrNone) 
                {
                HBufC* prompt = NULL;
                prompt = StringLoader::LoadLC(R_QTN_CHAT_ERROR_NOT_SUP_FUNCTIONS );
                VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
                CleanupStack::PopAndDestroy(prompt);
                LaunchViewL(iExtensionService.TabbedViewId());
                } 
            }
        case EFriendRejected:
        case EFriendAccpeted:
            {
            if(iWaitNote)
          	     {
          	     delete iWaitNote;
          	     iWaitNote = NULL;   
          	     }
		    if( ( aCommand.Result() != KErrNone ) && ( aCommand.CommandId() == EFriendAccpeted  ) ) 
                {
                HBufC* prompt = StringLoader::LoadLC( R_QTN_SERVTAB_SAVING_CONTACT_FAILED );
                VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
                CleanupStack::PopAndDestroy(prompt);
                } 
            break;
            }
        case EChangeOwnAvtar:
            {
            if(aCommand.Result() == KErrNone)  // successfull show confirmation note
                {
                if(iWaitNote)
                    {
                    delete iWaitNote;
                    iWaitNote = NULL;   
                    }
                } 
            else  // show error note
                {
                HBufC* prompt  = StringLoader::LoadLC( R_QTN_SERVTAB_ERROR_UPDATE_AVATAR );
                VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) ;
                CleanupStack::PopAndDestroy(prompt);
                }
            break;
            }
        case ECmdBlockContact:
           {
		TRACE("block finished enter");
           //blockcontact command is complete
           //use aCommand.Result() to get the data 
           //Ownership is not given to the caller  
           //some error occured.
           if(iWaitNote)
               {
               delete iWaitNote;
               iWaitNote = NULL;   
               }
           if(aCommand.Result() != KErrNone) 
               {
		   TRACE( "aCommand result = %d ", aCommand.Result() );
               HBufC* prompt = NULL;
               prompt = StringLoader::LoadLC( R_QTN_CHAT_BLOCKING_FAILED  );
		   TRACE("blocking error prompt displaying");	
               TInt ret( VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) );
               CleanupStack::PopAndDestroy(prompt);
               }
           break;
           } 
        case ECmdUnBlockContact:
           {
		TRACE("unblock finished enter");

           if(iWaitNote)
               {
               delete iWaitNote;
               iWaitNote = NULL;   
               }
           //unblockcontact command is complete
           //use aCommand.Result() to get the data 
           //Ownership is not given to the caller  
           //some error occured.
           if(aCommand.Result() != KErrNone) 
               {
                TRACE( "aCommand result = %d ", aCommand.Result() );
               HBufC* prompt = NULL;
               prompt = StringLoader::LoadLC( R_QTN_CHAT_UNBLOCKING_FAILED  );
		   TRACE("unblocking error prompt displaying");	
               TInt ret( VIMPSTUtilsDialog::DisplayErrorNoteL( *prompt ) );
               CleanupStack::PopAndDestroy(prompt);
               }
           else
        	   {
			TRACE("unblocking successful");	

		         if(iContainer)
		          {
		            TInt index = CurrentItemIndex();
			        if( index > 0 )
                         {
	                      MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
	                      HBufC* contactId =  arrayprocess.GetItemUserId(index).AllocLC();
	              	      if( contactId->Length() )
	                          {									   
						TRACE("HandleAvatarRefreshl calling for unblock");		   
				    iContainer->HandleAvatarRefreshL(*contactId,EFalse);
		      
			          }					
	                      }
	               CleanupStack::PopAndDestroy();
                    }   
        	   }
           break;
           } 
        default:
            { 
            //should never be here.           
            break;
            }
        }
    }
    
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ShowErrorNoteL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::ShowErrorNoteL(TInt aError )
	{
	TRACER_AUTO;
	HBufC* textResource = NULL;
	switch( aError)
		{
		case EErrExistInBlockList:
			{
			textResource = StringLoader::LoadLC( R_QTN_SERVTAB_ADD_FRIEND_BLOCKED);
			break;	
			}
		case KErrAlreadyExists :
		case EErrExistInContactList:
			{
			textResource = StringLoader::LoadLC( R_QTN_SERTAB_CONTACT_ALREADY_EXISTS_ERROR_NOTE);
			break;	
			}
		default :
			{
			textResource = StringLoader::LoadLC( R_QTN_SERVTAB_SAVING_CONTACT_FAILED );
			break;	
			}
		}
	if( textResource )
		{
		CAknInformationNote* note = new ( ELeave ) CAknInformationNote;
		note->ExecuteLD( *textResource );
		CleanupStack::PopAndDestroy( textResource );
		}
	}
    
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleCommandEventL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleCommandEventL(TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt /*aServiceError*/) 
    {    
    switch (aState)	
        {
        case TVIMPSTEnums::ESVCERegistered:
            {
            // this is to make sure that it always has the user id
            // some services they do not have the user id in offline case
            // like skype ,get the user if service is logged in
            if( !iUserId || iUserId->Length() == 0 )
	            {
	            // read if user id is empty only
	            ReadAndSetOwnUserIdL();	
	            }
            TInt isSuccessfulLogin = 0;
            iSettingsStore->GetL(iServiceId, EServiceSuccessfullLogin, isSuccessfulLogin);
            if(!isSuccessfulLogin)
                {
                isSuccessfulLogin = 1;
                iSettingsStore->SetL(iServiceId, EServiceSuccessfullLogin, isSuccessfulLogin);
                }
            if(iIsToolBarHidden)
                {
                ShowHideFixedToolbarL(ETrue);
                }
            break;
            }
        case TVIMPSTEnums::ESVCENotRegistered:
            {
             // might user has choosen some diff user id while login
            // set to store and engine etc
            ReadAndSetOwnUserIdL();
        	
		 	// no need to hide the tool bar here as its already called while in the 
		 	// disconnecting state.
		 	// assume in some cornere scenario, where network is lost, and the ui does not 
		 	// get a callback for disconnecting state then we need to call hidetoolbar in this case.
		 	// hence the flag iIsToolBarHidden will set it when ever the tool bar is hidden 
		 	if(!iIsToolBarHidden)
		 	    {
		 	    ShowHideFixedToolbarL(EFalse);
		 	    }
            break;	
            }
        case TVIMPSTEnums::ESVCENetworkConnecting:
            {
            // might user has choosen some diff user id while login
            // set to store and engine etc
            ReadAndSetOwnUserIdL();
            break;
            }
        case TVIMPSTEnums::ESVCEWaitingForNetwork:
            {
            break;
            }
        case TVIMPSTEnums::ESVCENetworkDisConnecting:
            {
            if(!iIsToolBarHidden)
                {
                ShowHideFixedToolbarL(EFalse);
                }
            break;
            }
        case TVIMPSTEnums::ESVCEUpdatingContacts:
            {
            // might user has choosen some diff user id while login
            // set to store and engine etc 
            // this is to make sure that it always has the user id
            // some services they do not have the user id in offline case
            // like skype ,get the user if service is logged in
            if( !iUserId || iUserId->Length() == 0 )
	            {
	            // read if user id is empty only
	            ReadAndSetOwnUserIdL();	
	            }
            break;
            }	
        default:
            {
            //should never be here.
            break;
            }

        }
    if(iContainer)
        {
        iContainer->HandleItemAdditionL();        
        }

    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CurrentItemIndex
// --------------------------------------------------------------------------
//
TInt CVIMPSTUiTabbedView:: CurrentItemIndex()
    {
    TInt index = KErrNone;
    iContainer?(index = iContainer->CurrentItemIndex()):(index = KErrNotFound);
    return index;
    }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ReadAndSetOwnUserIdL()
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ReadAndSetOwnUserIdL()
    {
    if( iUserId )
		{
		delete iUserId;	
		iUserId = NULL;
		}
 	iUserId = iEngine.GetOwnUserIdFromCChOrStorageL();
 	iEngine.SetOwnUserIdIfChangedL( *iUserId );
    }
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::AddContactL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::AddContactL( const TInt aResouceId, const TDesC& aUserId )
    {
	TRACER_AUTO;
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);    
    if (storage)
        {
        if (!storage->IsLocalStore())
            {
            HBufC* userid = HBufC::NewL(KUsernameMaxLength);
    		CleanupStack::PushL( userid );
    		TPtr useridPtr( userid->Des() );
			
            useridPtr.Copy( aUserId.Left( KUsernameMaxLength ) );
            TInt result = 0;
            if( useridPtr == KNullDesC )
                {
                CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( useridPtr, CAknQueryDialog::ENoTone );
                dlg->SetPredictiveTextInputPermitted( ETrue ); // T9
                result = dlg->ExecuteLD( aResouceId );
                }
            if( ( result == EAknSoftkeyOk ) || ( result == EAknSoftkeyYes )  || ( useridPtr != KNullDesC ) )
                {
                TPtrC ownUserId = storage->OwnContactL().UserId();
                if(useridPtr.Compare( ownUserId ) == 0) //Do not add own data item to contact list
                    {
                    //show note "Own Data Item cannot be added"
                    HBufC* failed = StringLoader::LoadLC( R_QTN_SERVTAB_ERROR_ADDOWN );
                    VIMPSTUtilsDialog::DisplayErrorNoteL( *failed );
                    CleanupStack::PopAndDestroy( failed );
                    }
                else
                    {
                    CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_SERVTAB_SAVING_CONTACT ,ETrue);
                    iCommandHandler.HandleCommandL(EAddContact, this, userid ); 
                    CleanupStack::PopAndDestroy( note );
                    }
             	}
		    CleanupStack::PopAndDestroy(userid);
            }
    else // server store
               
        {
                ////////////////////////////////////////////
                ReInitializeServiceHandlerL();
                TUint assignFlags = 0;
                //for unknown item createnew/update existing querry is shown
                // check whether the selected contact is unknown
                if( iCommandHandler.GetProcessInterface().GetType(iContainer->CurrentItemIndex()) ==  TVIMPSTEnums::EUnknonContactItem ) 
                    {
                    iSelectedAction = ShowNewUpdateQueryL();
                    if(CVIMPSTUiTabbedView::EInvitAcceptCreateNew == iSelectedAction )//create new 
                        {
                        assignFlags |= ECreateNewContact;
                        iSelectedAction = EContactCreateNew;
                        }
                    else if(CVIMPSTUiTabbedView::EInvitAcceptUpdateExisting == iSelectedAction ) // update existing
                        {
                        iSelectedAction = EContactAddFromContacts;
                        }
                    else // selected cancle option
                        {
                        return;
                        }
                    }
                else // not unknown contact
                    {
                    iSelectedAction = EContactCreateNew;
                    assignFlags |= ECreateNewContact;
                    }
                //show the phonebook dialog for create new/update existing option
    if(iAiwParamList)
        {
    delete iAiwParamList;
    iAiwParamList = NULL;
                }        
            iAiwParamList = CAiwGenericParamList::NewL();
            iAiwParamList->AppendL(
                    TAiwGenericParam(
                            EGenericParamContactAssignData,
                            TAiwVariant(TAiwSingleContactAssignDataV1Pckg(
                                    TAiwSingleContactAssignDataV1().SetFlags( assignFlags )))));
            
            if( aUserId.Length() > 0 )
                {
                // add user id in impp field
                TBuf<KBufLen> xspId; //xsp max length
                xspId.Append(iEngine.ServiceName());
                xspId.Append(KColon);
                xspId.Append(aUserId);

                iAiwParamList->AppendL(
                        TAiwGenericParam(
                                EGenericParamXSpId,
                                TAiwVariant(xspId) ) );
                }
            iServiceHandler->ExecuteServiceCmdL(
                    KAiwCmdAssign,
                    *iAiwParamList,
                    iServiceHandler->OutParamListL(),
                    0,
                    this);
            }
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CreateOrLinkToPhoneBookL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::CreateOrLinkToPhoneBookL(TInt aCommand )
	{
	TRACER_AUTO;
	TInt index = CurrentItemIndex();
	if( index <= 0 )
		{
		return;
		}
	TUint assignFlags = 0;
    if(ECmdAIWAddToContact == aCommand )//create new option
        {
        assignFlags |= ECreateNewContact;
        }
	TPtrC itemUserId = iCommandHandler.GetProcessInterface().GetItemUserId(index) ;
	if(iAiwParamList)
		{
		delete iAiwParamList;
		iAiwParamList = NULL;
		}   
	iAiwParamList = CAiwGenericParamList::NewL();
	iAiwParamList->AppendL(
	TAiwGenericParam(
	EGenericParamContactAssignData,
	TAiwVariant(TAiwSingleContactAssignDataV1Pckg(
	TAiwSingleContactAssignDataV1().SetFlags( assignFlags )))));

	TBuf<KBufLen> xspid;
	xspid.Append(iEngine.ServiceName());
	xspid.Append(KColon);
	xspid.Append(itemUserId);
	ReInitializeServiceHandlerL();
	iAiwParamList->AppendL(
						TAiwGenericParam(
						EGenericParamXSpId,
						TAiwVariant(xspid))  );

	iServiceHandler->ExecuteServiceCmdL( KAiwCmdAssign,
					                     *iAiwParamList,
					                     iServiceHandler->OutParamListL(),
					                     0,
					                     this);
	}

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::EditContactL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::EditContactL()
    {
	TRACER_AUTO;
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);    
    if (storage)
        {        
        if( storage->IsLocalStore())
            {
            iSelectedAction = KErrNotFound;
            TInt index = CurrentItemIndex();
        	MVPbkContactLink* contactLink = iCommandHandler.GetProcessInterface().ContactLink(index);        	
        	//show the phonebook dialog for edit existing data
            if(iAiwParamList)
                {
                delete iAiwParamList;
                iAiwParamList = NULL;
                }        
            iAiwParamList = CAiwGenericParamList::NewL();
			iAiwParamList->AppendL(
                    TAiwGenericParam(
                            EGenericParamContactAssignData,
                            TAiwVariant(TAiwSingleContactAssignDataV1Pckg(
                                    TAiwSingleContactAssignDataV1().SetFlags( EskipInfoNotes )))));

			HBufC8* packedLinks = contactLink->PackLC();
		    iAiwParamList->AppendL(
                    TAiwGenericParam(
                            EGenericParamContactLinkArray,
                            TAiwVariant(*packedLinks) ) );
		    ReInitializeServiceHandlerL();

            iServiceHandler->ExecuteServiceCmdL(
                    KAiwCmdAssign,
                    *iAiwParamList,
                    iServiceHandler->OutParamListL(),
                    0,
                    this);
		    CleanupStack::PopAndDestroy(); // packedLinks
            }
       }
    }




// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::AddContactL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::AddContactL(RPointerArray<HBufC> aContacts)
    {
	TRACER_AUTO;
    TInt count = aContacts.Count();

    switch( count )
        {
        case 0://no xsp found.
            {
            //launch add manually dialog.
            AddContactL( R_IM_ADD_CONTACT_MANUAL_QUERY );//to do replace with some thing else.
            break;
            }
        case 1://1 xsp foud.
            {
            //add new contact.
            HBufC* xspId = aContacts[0];
            AddContactL( R_IM_ADD_CONTACT_MANUAL_QUERY, *xspId );
            break;
            }
        default :// > 1 xsp found.
            {
            //show single selection dialog
            TInt selectionIndex = 0;
            CDesCArray* xspIds = new( ELeave )CDesCArrayFlat( count );
            CleanupStack::PushL( xspIds );

            //add all xsp(s) from aContacts to xspIds.
            for( TInt i = 0 ; i < count ; i++ )
                {
                xspIds->AppendL( *aContacts[i] );
                }

            HBufC* titleText = StringLoader::LoadLC( R_QTN_SERVTAB_SELECT_USER );

            TInt ret = VIMPSTUtilsDialog::DisplayListQueryDialogL( &selectionIndex,
                    xspIds,
                    R_IM_LIST_SELECT_STATUS_MESSAGE_DLG,
                    *titleText,
                    KNullDesC,
                    ETrue );
            CleanupStack::PopAndDestroy( titleText );


            if ( ret == EAknSoftkeyOk || ret == EAknSoftkeyDone || ret == EAknSoftkeySelect )
                {
                TPtrC selectedXspid = (*xspIds)[selectionIndex];
                AddContactL( R_IM_ADD_CONTACT_MANUAL_QUERY, selectedXspid );
                }
            CleanupStack::PopAndDestroy(xspIds);
            break;
            }
        }    
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CopyToPhoneBookL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::CopyToPhoneBookL()
    {
	TRACER_AUTO;
    TInt index = CurrentItemIndex();

    TPtrC name( KNullDesC );
    if( index <= 0 )
        {
        // return if item not found
        return; 
        }

    TPtrC seletctedItem = iCommandHandler.GetProcessInterface().GetItemUserId(index) ;

    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId) ;
    if(storage)
        {
        MVIMPSTStorageContact* contact = storage->FindContactByUserId(seletctedItem);
        if(contact)
            {
            CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_SERVTAB_SAVING_CONTACT ,ETrue);
            iCommandHandler.HandleCommandL(EAddToPbk, this, contact );
            CleanupStack::PopAndDestroy(note);
            }
        }
    }

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::DeleteContactL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView:: DeleteContactL()
    {
	TRACER_AUTO;
    TInt index = CurrentItemIndex();
    HBufC* deletePrompt = NULL;
    TPtrC name( KNullDesC );

    if(iContainer)
        {
        iContainer->CheckAndSetLastItemFlag();
        }
    if( index <= 0 )
        {
        // return if item not found
        return;	
        }

    TPtrC seletctedItem = iCommandHandler.GetProcessInterface().GetItemUserId(index) ;
    name.Set( seletctedItem );

    deletePrompt = StringLoader::LoadLC( R_QTN_SERVTAB_DELETE_CONTACT, name, CCoeEnv::Static() );
    TInt ret( VIMPSTUtilsDialog::DisplayQueryDialogL(R_GENERIC_YES_NO_CONFIRMATION_QUERY, *deletePrompt ) );
    CleanupStack::PopAndDestroy( deletePrompt );

    MVPbkContactLink* contactLink = iCommandHandler.GetProcessInterface().ContactLink(index);

    if( ( ret == EAknSoftkeyOk ) || ( ret == EAknSoftkeyYes ) )
        {
        if(iWaitNote)
            {
            delete iWaitNote;
            iWaitNote = NULL;
            }
        iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_CHAT_DELETING_CONTACT ,ETrue);
        iCommandHandler.HandleCommandL(EDeleteContact, this, &contactLink );
        }
    }



// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ChangeStatusProcessL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ChangeStatusProcessL()
    {
	TRACER_AUTO;
    if( iEngine.ServiceState() )
        {
        // Check variation
        TInt showAppearOffline = 1;
        TInt showAway = 1 ;   
        TInt showBusy = 1;

        TInt awayIndex( 0 );
        TInt busyIndex( 0 );
        TInt appOfflineIndex( 0 );

        // first icon is online
        // calculate indexes for other icons
        if ( showAway)
            {
            awayIndex++;
            busyIndex++;
            appOfflineIndex++;
            }
        else
            {
            awayIndex = -1;
            }

        if ( showBusy )
            {
            busyIndex++;
            appOfflineIndex++;
            }
        else
            {
            busyIndex = -1;
            }

        if ( showAppearOffline )
            {
            appOfflineIndex++;
            }
        else
            {
            appOfflineIndex = -1;
            }

        TInt status( 0 );
        CAknListQueryDialog* dlg = new ( ELeave )
        CAknListQueryDialog( &status );
        dlg->PrepareLC( R_IM_STATUS_CHANGE_DLG );

        // Format the texts
        // Create a array for texts
        CDesCArray* listItemsArray =
        new ( ELeave ) CDesCArrayFlat( KAmountOfOwnStatusStates );
        CleanupStack::PushL( listItemsArray );

        // Online
        HBufC* onlineText = NULL;
        onlineText = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_OWN_ONLINE );

        HBufC* itemTxtOnline =
        HBufC::NewLC( KTabFormatLenght + onlineText->Length() );
        TPtr itemTxtPtrOnline( itemTxtOnline->Des() );

        itemTxtPtrOnline.Format( KTabFormat, KOnlineIconIndex );
        itemTxtPtrOnline.Append( *onlineText );
        listItemsArray->AppendL( itemTxtPtrOnline );
        CleanupStack::PopAndDestroy( itemTxtOnline );
        CleanupStack::PopAndDestroy( onlineText );

        // Away
        if ( showAway )
            {
            HBufC* awayText = NULL;
            awayText = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_OWN_AWAY );

            HBufC* itemTxtAway = HBufC::NewLC(
                    KTabFormatLenght + awayText->Length() );
            TPtr itemTxtPtrAway( itemTxtAway->Des() );

            itemTxtPtrAway.Format( KTabFormat, awayIndex );
            itemTxtPtrAway.Append( *awayText );
            listItemsArray->AppendL( itemTxtPtrAway );
            CleanupStack::PopAndDestroy( itemTxtAway );
            CleanupStack::PopAndDestroy( awayText );
            }

        // Busy
        if ( showBusy )
            {
            HBufC* busyText = NULL;
            busyText = StringLoader::LoadLC( R_QTN_SERVTAB_CHANGE_OWN_DND );

            HBufC* itemTextBusy = HBufC::NewLC(
                    KTabFormatLenght + busyText->Length() );
            TPtr itemTxtPtrBusy( itemTextBusy->Des() );

            itemTxtPtrBusy.Format( KTabFormat, busyIndex );
            itemTxtPtrBusy.Append( *busyText );
            listItemsArray->AppendL( itemTxtPtrBusy );
            CleanupStack::PopAndDestroy( itemTextBusy );
            CleanupStack::PopAndDestroy( busyText );
            }
        // Appear offline
        if ( showAppearOffline )
            {
            HBufC* offlineText = NULL;
            offlineText = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_OWN_APPEAR_OFFLINE );

            HBufC* itemTextOffline = HBufC::NewLC(
                    KTabFormatLenght + offlineText->Length() );
            TPtr itemTxtPtrOffline( itemTextOffline->Des() );

            itemTxtPtrOffline.Format( KTabFormat, appOfflineIndex );
            itemTxtPtrOffline.Append( *offlineText );
            listItemsArray->AppendL( itemTxtPtrOffline );
            CleanupStack::PopAndDestroy( itemTextOffline );
            CleanupStack::PopAndDestroy( offlineText );
            }

        // Set the array
        dlg->SetOwnershipType( ELbmOwnsItemArray );
        CleanupStack::Pop( listItemsArray );
        dlg->SetItemTextArray( listItemsArray );

        // Add icons
        CArrayPtr< CGulIcon >* icons = LoadOwnStatusIconsL();


        dlg->SetIconArrayL( icons ); // transfers ownership
        // TODO::get own status

        // get dialog's listbox
        CEikListBox* listBox = dlg->ListBox();

        //listBox->SetCurrentItemIndex( 0 );

        TInt result = dlg->RunLD();

        //structure to pack status and status text.
        TStatusAndStatusText statusAndSatatustext;

        TInt error( KErrNone );
        if ( ( result == EAknSoftkeyOk ) ||
                ( result == EAknSoftkeySelect ) )
            {
            if ( status == 0 )                    
                {
                TInt autoStatusMsgEnabledForOnline = 1; /*IMUtils::IntResourceValueL( RSC_CHAT_VARIATION_ONLINE_AUTO );*/
                if( autoStatusMsgEnabledForOnline )
                    {
                    error = AskStatusMessageL( TVIMPSTEnums::EOnline, EFalse );
                    } 
                if( error == KErrNone )
                    {
                    statusAndSatatustext.iStatus  = TVIMPSTEnums::EOnline;
                    statusAndSatatustext.iStatusText.Copy(iStatusMessage->Des());
                    CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_CHAT_CHANGE_STATUS_CHANGING );
                    error = iCommandHandler.HandleCommandL(EChangeOwnStatus, this, &statusAndSatatustext);
                    CleanupStack::PopAndDestroy( note );
                    }
                }
            else if ( status == awayIndex ) // Away 
                { 
                // can't get here unless Away is enabled  
                TInt autoStatusMsgEnabledForAway = 1;
                //IMUtils::IntResourceValueL( RSC_CHAT_VARIATION_AWAY_AUTO );
                if( autoStatusMsgEnabledForAway )
                    {                
                    error = AskStatusMessageL( TVIMPSTEnums::EAway, EFalse );
                    }
                if( error == KErrNone )
                    {
                    statusAndSatatustext.iStatus  = TVIMPSTEnums::EAway;
                    statusAndSatatustext.iStatusText.Copy(iStatusMessage->Des());
                    CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_CHAT_CHANGE_STATUS_CHANGING );
                    error = iCommandHandler.HandleCommandL(EChangeOwnStatus, this, &statusAndSatatustext);
                    CleanupStack::PopAndDestroy( note );
                    }
                }

            else if ( status == busyIndex ) // Busy 
                {
                // can't get here unless Busy is enabled
                TInt autoStatusMsgEnabledForBusy = 1;
                //IMUtils::IntResourceValueL( RSC_CHAT_VARIATION_BUSY_AUTO  );
                if( autoStatusMsgEnabledForBusy )
                    {
                    error = AskStatusMessageL( TVIMPSTEnums::EBusy, EFalse );
                    }
                if( error == KErrNone )
                    {
                    statusAndSatatustext.iStatus  = TVIMPSTEnums::EBusy;
                    statusAndSatatustext.iStatusText.Copy(iStatusMessage->Des());
                    CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_CHAT_CHANGE_STATUS_CHANGING );
                    error = iCommandHandler.HandleCommandL(EChangeOwnStatus, this, &statusAndSatatustext);
                    CleanupStack::PopAndDestroy( note );

                    }
                }
            else if ( status == appOfflineIndex ) //appearOffline
                {

                statusAndSatatustext.iStatus  = TVIMPSTEnums::EInvisible;
                statusAndSatatustext.iStatusText.Copy(iStatusMessage->Des());
                CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_CHAT_CHANGE_STATUS_CHANGING );
                error = iCommandHandler.HandleCommandL(EChangeOwnStatus, this, &statusAndSatatustext);
                CleanupStack::PopAndDestroy( note );
                }
            else
                {
                HBufC* text = NULL;
                text = StringLoader::LoadLC( R_QTN_IM_CHANGE_STATUS_NOT_CHANGED );
                VIMPSTUtilsDialog::DisplayNoteDialogL( *text );
                CleanupStack::PopAndDestroy( text );
                }
            //iUISessionManager->SetStatusPaneIconsL();
            }
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::LoadOwnStatusIconsL
// ---------------------------------------------------------
//
CAknIconArray* CVIMPSTUiTabbedView::LoadOwnStatusIconsL()
    {
	TRACER_AUTO;
    CAknIconArray* icons = iBrandHandler->LoadOwnStatusIconsLC();
    CleanupStack::Pop( icons );
    return icons;
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::AskStatusMessageL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CVIMPSTUiTabbedView::AskStatusMessageL( TVIMPSTEnums::TOnlineStatus aStatus,
        TBool aUpdateToNetwork )
    {
	TRACER_AUTO;
    CDesCArray* statusMessages = new( ELeave )CDesCArrayFlat(4);
    CleanupStack::PushL( statusMessages );

    HBufC* firstLine = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_NEW_MESSAGE );
    statusMessages->AppendL( *firstLine );
    CleanupStack::PopAndDestroy( firstLine );

    // Read status messages
    ReadStatusMessagesL( *statusMessages, aStatus );

    // show the query
    TInt selectionIndex( statusMessages->Count() > 1 ? 1 : 0 );

    // Force to continue if selection query is not shown
    TInt ret = EAknSoftkeyOk;

    if( selectionIndex == 1 )
        {
        // There were previous status messages, show selection query
        HBufC* titleText = StringLoader::LoadLC( R_QTN_CHAT_CHANGE_SELECT_MESSAGE );
        ret = VIMPSTUtilsDialog::DisplayListQueryDialogL( &selectionIndex, statusMessages,
                R_IM_LIST_SELECT_STATUS_MESSAGE_DLG,
                *titleText, KNullDesC, ETrue );
        CleanupStack::PopAndDestroy( titleText );
        }

    if ( !( ret == EAknSoftkeyOk || ret == EAknSoftkeyDone || ret == EAknSoftkeySelect ) )
        {
        CleanupStack::PopAndDestroy( statusMessages );
        return KErrCancel;
        }

    TPtr statusTxt( iStatusMessage->Des() );
    statusTxt.Zero();

    if( selectionIndex != 0 )
        {
        statusTxt.Copy( ( *statusMessages) [ selectionIndex ] );
        }

    TInt result = VIMPSTUtilsDialog::DisplayTextQueryDialogL(
            statusTxt ,
            R_QTN_CHAT_CHANGE_STATUS_MESSAGE,
            R_IM_ENTER_STATUS_MESSAGE_QUERY,
            ETrue, // T9
            ETrue // LSK always visible
    );

    if ( (result == EAknSoftkeyOk || result == EAknSoftkeyDone )&&(iEngine.ServiceState()))
        {

        // Delete first line.
        statusMessages->Delete( 0 );
        statusMessages->InsertL( 0, statusTxt );
        WriteStatusMessagesL( *statusMessages, aStatus );            
        if( aUpdateToNetwork ) 
            {
            CVIMPSTUtilsWaitNote* note = CVIMPSTUtilsWaitNote::ShowWaitNoteLC( R_QTN_CHAT_CHANGE_MESSAGE_CHANGING );
            //structure to pack status and status text.
            TStatusAndStatusText statusAndSatatustext;
            statusAndSatatustext.iStatus  = aStatus;
            statusAndSatatustext.iStatusText.Copy( statusTxt );
            iCommandHandler.HandleCommandL(EChangeOwnMessage, this, &statusAndSatatustext);
            CleanupStack::PopAndDestroy( note );
            }
        }
    else
        {
        CleanupStack::PopAndDestroy();//statusMessages
        return KErrCancel;
        }
    CleanupStack::PopAndDestroy(); //statusMessages
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ReadStatusMessagesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ReadStatusMessagesL( CDesCArray& aArray, TVIMPSTEnums::TOnlineStatus aStatus)
    {
	TRACER_AUTO;
    RBuf buffer;
    buffer.CreateL( RProperty::KMaxPropertySize );
    CleanupClosePushL( buffer );
            TInt err(KErrNone);
            switch ( aStatus )
                {
                case TVIMPSTEnums::EOnline:
                    {
                    err = iSettingsStore->GetL( iServiceId, EServiceOwnStatusMessagesOnline, buffer ); 
                    break;
                    }
                case TVIMPSTEnums::EBusy:
                    {
                    err = iSettingsStore->GetL( iServiceId, EServiceOwnStatusMessagesBusy, buffer ); 
                    break;
                    }
                case TVIMPSTEnums::EAway:
                    {
                    err = iSettingsStore->GetL( iServiceId, EServiceOwnStatusMessagesAway, buffer ); 
                    break;
                    }
                }
    TBuf< KStatusMessageHeaderLength > header;

    TInt countOfMessages( 0 );
    TInt offset( 1 ); // First character is for header length
    TInt headerLength( 0 );
    if( err == KErrNone && buffer.Length() )
        {
        TLex lexer( buffer.Left( 1 ) );
        err = lexer.Val( headerLength );
        }
    else
        {
        err = KErrEof;
        }
    while( err == KErrNone && countOfMessages < KMaxStatusMessageCount )
        {
        TPtrC ptr( buffer.Mid( offset ) );
        if( ptr.Length() > headerLength )
            {
            header.Copy( ptr.Left( headerLength ) );
            header.Trim();
            TLex lexer( header );
            offset += headerLength;
            TInt messageLength( 0 );
            err = lexer.Val( messageLength );
            if( err == KErrNone )
                {
                ptr.Set( buffer.Mid( offset ) );
                if( ptr.Length() >= messageLength )
                    {
                    if (messageLength)
                        {
                        aArray.AppendL( ptr.Left( messageLength ) );
                        ++countOfMessages; // this should be increased only for not null messages.
                        }
                    offset += messageLength;
                    }
                else
                    {
                    err = KErrEof;
                    }
                }
            }
        else
            {
            err = KErrEof;
            }
        }
    CleanupStack::PopAndDestroy(); //buffer
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::WriteStatusMessagesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CVIMPSTUiTabbedView::WriteStatusMessagesL( MDesCArray& aArray, TVIMPSTEnums::TOnlineStatus aStatus )
    {
	TRACER_AUTO;
    TPtrC firstLine = aArray.MdcaPoint( 0 );
    TInt countOfMessages( 0 );
    TInt arrayCount( aArray.MdcaCount() );
    TInt index( 0 );

    RBuf buffer;
    buffer.CreateL( RProperty::KMaxPropertySize );
    CleanupClosePushL( buffer );

    TBuf< KStatusMessageHeaderLength > header;
    // Read the array and append the buffer
    while( index < arrayCount && countOfMessages < KMaxStatusMessageCount )
        {
        if( index == 0 )
            {
            header.Format( KStatusMessageHeaderLengthFormat,
                    KStatusMessageHeaderLength );
            buffer.Append( header );
            }
        TPtrC message = aArray.MdcaPoint( index );
        if( index == 0 || message.CompareF( firstLine ) != 0 )
            {
            header.Format( KStatusMessageHeaderFormat, message.Length() );
            buffer.Append( header );
            buffer.Append( message );
            //skipping increase of countOfMessages is to get one more status message if it is null.
            if( message.Length() ) // null status message
                {
                ++countOfMessages;
                }
            }
        ++index;
                }
            switch ( aStatus )
                {
                case TVIMPSTEnums::EOnline:
                    {
                    iSettingsStore->SetL( iServiceId, EServiceOwnStatusMessagesOnline, buffer ); 
                    break;
                    }
                case TVIMPSTEnums::EBusy:
                    {
                    iSettingsStore->SetL( iServiceId, EServiceOwnStatusMessagesBusy, buffer ); 
                    break;
                    }
                case TVIMPSTEnums::EAway:
                    {
                    iSettingsStore->SetL( iServiceId, EServiceOwnStatusMessagesAway, buffer ); 
                    break;
                    }
                }
    CleanupStack::PopAndDestroy(); // buffer
    }


// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ChangeOwnStatusMessageL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ChangeOwnStatusMessageL()
    {
    TVIMPSTEnums::TOnlineStatus status = iCommandHandler.GetProcessInterface().GetOnlineStatusL( 0 ); //0 for own status item
    // ETrue meaning network change required
    AskStatusMessageL(status, ETrue);
    }
// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ChangeOwnAvatarL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ChangeOwnAvatarL(const TDesC& aFileName, const TDesC8& aMimeType )
	{
	TRACER_AUTO;
	MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
	TAvatarData avatarData;
	avatarData.iStatus = arrayprocess.GetOnlineStatusL(0);
	avatarData.iStatusText.Copy( arrayprocess.StatusTextL(0) );
	avatarData.iFileName.Set(aFileName);
	avatarData.iMimeType.Set(aMimeType);
	iCommandHandler.HandleCommandL(EChangeOwnAvtar, this, &avatarData);   
	}
// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::GetServiceState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTUiTabbedView::GetServiceState()
    {
    return iEngine.ServiceState();
    }


// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::SearchContactsL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView:: SearchContactsL()
	{
	TRACER_AUTO;
	CVIMPSTUiSearchFieldArray& fieldArray = iExtensionService.GetSearchFieldArray(); 

	if( !fieldArray.Count() )
		{
		return;	
		}
	
	if(!iIsToolBarHidden)
	    {
	    ShowHideFixedToolbarL(EFalse);
	    }

	if (iContainer)
		{
		iContainer->DeactivateFindPaneL();
		}

	fieldArray.ResetFieldData();
	fieldArray.SetSearchType( EIMBasicSearch );
	//Launch the search view with search dialog
	LaunchViewL( iExtensionService.SearchViewId() );
	            
	}

// ---------------------------------------------------------------------------
// CVIMPSTUiTabbedView::LaunchViewL()
// ---------------------------------------------------------------------------
// 
void CVIMPSTUiTabbedView::LaunchViewL( TInt aViewId )
    {
	TRACER_AUTO;

    CPbk2ViewState* state = CPbk2ViewState::NewLC( );

    TInt searchViewId;
    TInt err = iMapper.GetNewView( 
            KIMCONTACTSEXTNIMPLEMENTATIONUID, 
            aViewId, 
            searchViewId );
    User::LeaveIfError( err );                        
    Phonebook2::Pbk2AppUi()->Pbk2ViewExplorer()->ActivatePhonebook2ViewL
    (TUid::Uid(searchViewId), state);

    CleanupStack::PopAndDestroy( state );
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::CreateToolBarL
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::CreateToolBarL()
    {
	TRACER_AUTO;
    if ( AknLayoutUtils::PenEnabled() )
        {
        TInt resourceId = 0;
        if(iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM) && 
                !iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EVoip))
            {
            resourceId = R_IM_APP_TOOLBAR;
            }
        else if(iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EVoip) && 
                !iEngine.IsSubServiceSupportedL(TVIMPSTEnums::EIM))
            {
            resourceId = R_VOIP_APP_TOOLBAR;
            }
        else           
            {
            resourceId = R_IM_VOIP_APP_TOOLBAR;
            }
        if ( resourceId != 0 && !iFixedToolbar )
            {              
            CAknToolbar* temp = iFixedToolbar;

            iFixedToolbar = CAknToolbar::NewL( resourceId );
            iFixedToolbar->SetToolbarObserver( this ); 
            iFixedToolbar->SetCloseOnAllCommands( EFalse );
            CEikAppUiFactory* appUiFactory = 
            static_cast<CEikAppUiFactory*>(CEikonEnv::Static()->AppUiFactory() );                  
            appUiFactory->SetViewFixedToolbar( iFixedToolbar );

            iFixedToolbar->SetToolbarVisibility( EFalse, EFalse );
            delete temp;
            temp = NULL;
            }
        }
    else
        {
        delete iFixedToolbar;
        iFixedToolbar = NULL;
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::UpdateToolbarL
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::UpdateToolbarL()
    {
	TRACER_AUTO;
    if ( AknLayoutUtils::PenEnabled() && iFixedToolbar && iContainer )    
        {
        TInt currentItemIndex = iContainer->CurrentItemIndex();        
        TVIMPSTEnums::TItem itemType = iCommandHandler.GetProcessInterface().GetType(currentItemIndex);         
        TVIMPSTEnums::TOnlineStatus onlinestatus = iCommandHandler.GetProcessInterface().GetOnlineStatusL(currentItemIndex);        
        TBool imEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EIM);
        TBool voipEnabled = iEngine.IsSubServiceEnabled(TVIMPSTEnums::EVoip);
        TPtrC selectedItem = iCommandHandler.GetProcessInterface().GetItemUserId(currentItemIndex);
        
        if(voipEnabled && imEnabled)
            {                
            if( TVIMPSTEnums::EOwnStatusItem == itemType ||
                    TVIMPSTEnums::EFriendRequestItem == itemType ||
                    KErrNotFound == currentItemIndex )
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarVoipCall, ETrue, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarChat, ETrue, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, ETrue, EFalse );
                }
            else if( TVIMPSTEnums::EUnknonContactItem == itemType  )
                {  
                iFixedToolbar->SetItemDimmed(ECmdToolbarVoipCall, EFalse, EFalse); 
                iFixedToolbar->SetItemDimmed(ECmdToolbarChat, EFalse, EFalse); 
                iFixedToolbar->SetItemDimmed(ECmdToolbarDetails, ETrue, EFalse);                 
                }
            else if( TVIMPSTEnums::EContactItem == itemType )
                {  
                if( selectedItem != KNullDesC  &&  TVIMPSTEnums::EServiceOut != onlinestatus )
                    {
                  
                    iFixedToolbar->SetItemDimmed(ECmdToolbarChat, EFalse, EFalse);
                    }
                else
                    {
                    iFixedToolbar->SetItemDimmed(ECmdToolbarChat, ETrue, EFalse);
                    }
                iFixedToolbar->SetItemDimmed(ECmdToolbarDetails, EFalse, EFalse); 
                iFixedToolbar->SetItemDimmed(ECmdToolbarVoipCall, EFalse, EFalse); 
                }  
            else
                {  
                iFixedToolbar->SetItemDimmed(ECmdToolbarVoipCall, ETrue, EFalse); 
                iFixedToolbar->SetItemDimmed(ECmdToolbarChat, ETrue, EFalse);
                iFixedToolbar->SetItemDimmed(ECmdToolbarDetails, EFalse, EFalse);                 
                }  
            }
        else if(imEnabled && !voipEnabled)
            {                   
            if( TVIMPSTEnums::EOwnStatusItem == itemType ||
                    TVIMPSTEnums::EFriendRequestItem == itemType ||
                    KErrNotFound == currentItemIndex)
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarChat, ETrue, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, ETrue, EFalse );               
                }
            else if( TVIMPSTEnums::EUnknonContactItem == itemType  )
                {  
                iFixedToolbar->SetItemDimmed(ECmdToolbarChat, EFalse, EFalse); 
                iFixedToolbar->SetItemDimmed(ECmdToolbarDetails, ETrue, EFalse);                 
                }
            else if( TVIMPSTEnums::EContactItem == itemType )
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, EFalse, EFalse );
                if( selectedItem != KNullDesC && TVIMPSTEnums::EServiceOut != onlinestatus )
                    {
                    iFixedToolbar->SetItemDimmed(ECmdToolbarChat, EFalse, EFalse);
                    }
                else
                    {
                    iFixedToolbar->SetItemDimmed(ECmdToolbarChat, ETrue, EFalse);
                    }
                }   
            }
        else if( voipEnabled && !imEnabled )
            {               
            if( (TVIMPSTEnums::EOwnStatusItem == itemType ||
                    TVIMPSTEnums::EFriendRequestItem == itemType ||
                    KErrNotFound == currentItemIndex) && !iAddContact )
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarVoipCall, ETrue, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, ETrue, EFalse );               
                }   
            else if( TVIMPSTEnums::EUnknonContactItem == itemType  )
                {  
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed(ECmdToolbarVoipCall, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed(ECmdToolbarDetails, ETrue, EFalse);                 
                }
            else if( TVIMPSTEnums::EContactItem == itemType )
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed(ECmdToolbarVoipCall, EFalse, EFalse);  
                } 
            }
        else
            {               
            if( (TVIMPSTEnums::EOwnStatusItem == itemType ||
                    TVIMPSTEnums::EFriendRequestItem == itemType ||
                    KErrNotFound == currentItemIndex) && !iAddContact )
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse );
                } 
            else if( TVIMPSTEnums::EUnknonContactItem == itemType  )
                {  
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse);
                }
            else
                {
                iFixedToolbar->SetItemDimmed( ECmdToolbarNewContact, EFalse, EFalse );
                iFixedToolbar->SetItemDimmed( ECmdToolbarDetails, EFalse, EFalse );
                }
            }
        iFixedToolbar->DrawNow();
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::DynInitToolbarL
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::DynInitToolbarL( TInt /*aResourceId*/, CAknToolbar* /*aToolbar*/ )
    {
    if ( AknLayoutUtils::PenEnabled() )
        {
        UpdateToolbarL();
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::OfferToolbarEventL
// ---------------------------------------------------------
//        
void CVIMPSTUiTabbedView::OfferToolbarEventL( TInt aCommand )
    {
	TRACER_AUTO;
    if ( AknLayoutUtils::PenEnabled() )
        {
        switch( aCommand )
            {
            case ECmdToolbarVoipCall:
                {
                MakeVoipCallL();
                break;
                }
            case ECmdToolbarChat:
                {
                HandleCommandL( ECmdOpenConversation );
                break;
                }
            case ECmdToolbarDetails:
                {
                HandleCommandL( ECmdContactDetails );//EChatLaunchCCA

                break;
                }
            case ECmdToolbarNewContact:
                {
                HandleCommandL ( ECmdAddContact );
                break;
                }
            }  
        }
    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::ShowHideFixedToolbarL
// ---------------------------------------------------------
//     
void CVIMPSTUiTabbedView::ShowHideFixedToolbarL(TBool aShow)
    { 
    if ( iFixedToolbar && iContainer )
        {    	
        iFixedToolbar->HideItemsAndDrawOnlyBackground( !aShow );
        if(aShow)
            {
            iFixedToolbar->SetToolbarVisibility( aShow, !aShow);
            }
        iContainer->CoeControl()->SetRect( iView.ClientRect() );
        iContainer->CoeControl()->DrawDeferred();
        iIsToolBarHidden = !aShow;
        }

    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::ProcessFriendRequestL
// ---------------------------------------------------------
// 
void CVIMPSTUiTabbedView::ProcessFriendRequestL()
    {
	TRACER_AUTO;
    TInt index = CurrentItemIndex();    
    if( index <= 0 )
        {
        // return if item not found
        return; 
        }
    MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
    if( iFriendRequestId )
	    {
	    delete iFriendRequestId;
	    iFriendRequestId = NULL;	
	    }
    iFriendRequestId = arrayprocess.GetItemUserId(index).AllocL() ;

    if( !iFriendRequestId )
        {
        return;
        }

    HBufC* queryTxt = StringLoader::LoadLC( R_SERVTAB_FRIEND_REQUEST_CONF, *iFriendRequestId );

    //ask the query and send the response back to the server.
    TInt response = VIMPSTUtilsDialog::DisplayQueryDialogL(R_SERVTAB_GENERIC_ACCEPT_DECLINE_CONFIRMATION_QUERY,*queryTxt);
    CleanupStack::PopAndDestroy(queryTxt);

    if((EAknSoftkeyYes == response)||(EAknSoftkeyOk == response ) )
        {
        
        MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId() ) ;
        if( storage && storage->IsLocalStore() )//if local store popup create new/update existing dialog 
           {
            iSelectedAction = ShowNewUpdateQueryL();
            TUint assignFlags = 0;
            if(CVIMPSTUiTabbedView::EInvitAcceptCreateNew == iSelectedAction )//create new option
                {
                assignFlags |= ECreateNewContact;
                }
            else if(CVIMPSTUiTabbedView::EInvitAcceptUpdateExisting == iSelectedAction ) //if update existing is selected
                {
                assignFlags = 0;
                }
            else                                //if the selection dialog is cancelled then just do nothing
                {
                return;
                }
            //show the phonebook dialog for create new/update existing option
            if(iAiwParamList)
                {
                delete iAiwParamList;
                iAiwParamList = NULL;
                }        
            iAiwParamList = CAiwGenericParamList::NewL();

            iAiwParamList->AppendL(
                    TAiwGenericParam(
                            EGenericParamContactAssignData,
                            TAiwVariant(TAiwSingleContactAssignDataV1Pckg(
                                    TAiwSingleContactAssignDataV1().SetFlags( assignFlags )))));

            TBuf<KBufLen> xspId; //xsp max length
            xspId.Append(iEngine.ServiceName());
            xspId.Append(KColon);
            xspId.Append(*iFriendRequestId);

            iAiwParamList->AppendL(
                    TAiwGenericParam(
                            EGenericParamXSpId,
                            TAiwVariant(xspId))
            );
            ReInitializeServiceHandlerL();
            iServiceHandler->ExecuteServiceCmdL(
                    KAiwCmdAssign,
                    *iAiwParamList,
                    iServiceHandler->OutParamListL(),
                    0,
                    this);
            // remove of friend request is done in HnadleNotify aiw call back
            // to handle the cancel of selection dialog
            }
        else // in case of server store
            {
            if( iWaitNote )
		        {
		        delete iWaitNote;
		        iWaitNote = NULL;
		        }
        	iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_SERVTAB_SAVING_CONTACT, ETrue, EFalse );
            iCommandHandler.HandleCommandL(EFriendAccpeted, this, iFriendRequestId);
            }

        }
    //response value comes as zero if user rejects the request
    else if( EAknSoftkeyCancel == response || 0 == response) // rejected the request
        {
        if(iWaitNote)
            {
            delete iWaitNote;
            iWaitNote = NULL;	
            }
        iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_SERVTAB_DECLINING_INVITE, ETrue , EFalse );;
        iCommandHandler.HandleCommandL(EFriendRejected, this, iFriendRequestId );
        }
   

    }

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::HandleSearchResultL()
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleSearchResultL(RArray<TVIMPSTSearchData> /*aSearchData*/ )
    {
    // no implemetation
    }


// ---------------------------------------------------------
// CVIMPSTUiTabbedView::HandleSearchFinishedL()
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleSearchFinishedL(TInt /*aError*/)
    {
    // no implemetation
    }

// ---------------------------------------------------------
// CVIMPSTUiSearchView::HandleSearchKeysEventL()
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleSearchKeysEventL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray )
    {
    iExtensionService.SetSearchFieldsDataL(aEnumKeysArray, aLebelKeysArray );
    }

// ---------------------------------------------------------
// CVIMPSTUiSearchView::ShowURLQueryL()
// ---------------------------------------------------------
//
void CVIMPSTUiTabbedView::ShowURLQueryL( )
    {   
	TRACER_AUTO;
    // Open browser with the specified URL
    RBuf buffer;
    buffer.CreateL( RProperty::KMaxPropertySize );
    CleanupClosePushL( buffer );
    iSettingsStore->GetL(iServiceId, EServiceURLLink, buffer);      
    HBufC16* url = HBufC::NewL(buffer.Length());
    url = buffer.Alloc();
    OpenWebBrowserL( *url );           
    CleanupStack::PopAndDestroy( ); //buffer   
    }

// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::OpenWebBrowserL
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::OpenWebBrowserL( const TDesC& aUrl )
    {
    if( !iBrLauncher )
        {
        iBrLauncher = CBrowserLauncher::NewL();             
        }
    //Asynchronous operation to launch the browser with aUrl
    iBrLauncher->LaunchBrowserEmbeddedL( aUrl,this,this );    
    }


// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::LoginL
// -----------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::LoginL()
    {
	TRACER_AUTO;
    TBool IsPasswordSet = iEngine.IsPasswordAvailableL();

    if(iUserId)
        {
        delete iUserId;
        iUserId = NULL;
        }
   
    iUserId = iEngine.GetOwnUserIdFromCChOrStorageL();
    
    
    TBool urlNeedtoShow = IsURLQueryToBeShownL();
    //first login : userid is not initialized and password is not set.
    TBool doLogin = ETrue;
    if( urlNeedtoShow &&  (!IsPasswordSet) && (iUserId->Length() == 0))
        {       
        TInt menuChoise( KErrCancel );
        ShowUseExistingOrCreateNewMenuL( menuChoise );
        switch(menuChoise )
            {
            case CVIMPSTUiTabbedView::EXmppMenuValuesCreateNew:
                {
                doLogin = EFalse;
                ShowURLQueryL();
                break;
                }
            case CVIMPSTUiTabbedView::EXmppMenuValuesUseExisting:
                {
                // login must be called
                doLogin = ETrue;
                break;
                }
            default: // user has cancelled the createnew and i have acccount query
                doLogin = EFalse;
                break;             
			}
        }
     if( doLogin )
        {
        TInt error = ShowTermsofuseL();
        if(KErrNone == error)
            {
            if( urlNeedtoShow  )
                {
                // Url query has been shown and accep by user
                iSettingsStore->SetL(iServiceId, EURLRegistrationOff, ETrue);
                }
            iCommandHandler.HandleCommandL(ELoginService, this, &iServiceId); 
            }
        }
    }


// -----------------------------------------------------------------------------
// CVIMPSTUiTabbedView::IsURLQueryToBeShownL
// -----------------------------------------------------------------------------
//
TBool CVIMPSTUiTabbedView::IsURLQueryToBeShownL()
    {
	TRACER_AUTO;
    TBool urlQueryShown = EFalse;
    
    RBuf buffer;
    buffer.CreateL( RProperty::KMaxPropertySize );
    CleanupClosePushL( buffer );
    iSettingsStore->GetL(iServiceId, EServiceURLLink, buffer); 
    
    //get the url from the cenrep.
    TInt isURLQueryDisabled = EFalse;
    iSettingsStore->GetL(iServiceId, EURLRegistrationOff, isURLQueryDisabled);
    
    if(buffer.Length() && !isURLQueryDisabled)
        {
        urlQueryShown = ETrue;
        }
    CleanupStack::PopAndDestroy(); //buffer
    return urlQueryShown;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ShowUseExistingOrCreateNewMenuL
// return values <0 -> error
//               0 -> use existing
//               1 -> create new
// ---------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ShowUseExistingOrCreateNewMenuL( TInt& aMenuChoise )
    {
	TRACER_AUTO;
    TInt rvalue( KErrGeneral );

    CAknSinglePopupMenuStyleListBox* accountList =
    new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( accountList );

    CAknPopupList* popupList =
    CAknPopupList::NewL( accountList, R_AVKON_SOFTKEYS_OK_BACK );
    CleanupStack::PushL( popupList );

    accountList->ConstructL(  popupList, EAknListBoxMenuList);

    // construct listbox item array
    CDesCArray *itemList = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( itemList );
    TBuf<KMaxListItemLength> item;

    // First listbox item
    HBufC* text1 = StringLoader::LoadLC( R_QTN_USE_EXISTING );
    itemList->AppendL( *text1 );
    CleanupStack::PopAndDestroy( text1 );

    // Second listbox item
    HBufC* text2 = StringLoader::LoadLC( R_QTN_CREATE_NEW );
    itemList->AppendL( *text2 );
    CleanupStack::PopAndDestroy( text2 );

    // Set items and ownership
    accountList->Model()->SetItemTextArray( itemList );
    accountList->Model()->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop( itemList );

    HBufC* title = StringLoader::LoadLC( R_QTN_SELECT_ACTION );
    popupList->SetTitleL( *title );
    CleanupStack::PopAndDestroy( title );

    TInt popupOk = popupList->ExecuteLD();

    if ( popupOk )
        {
        rvalue = accountList->CurrentItemIndex();
        }
    else
        {
        rvalue = KErrCancel;
        }

    CleanupStack::Pop( popupList );
    CleanupStack::PopAndDestroy( accountList );

    aMenuChoise = rvalue;
    }
// ---------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ShowNewUpdateQueryL
// return values <0 -> error
//               0 ->create new
//               1 -> update existing
// ---------------------------------------------------------------------------
//
TInt CVIMPSTUiTabbedView::ShowNewUpdateQueryL()
    {
	TRACER_AUTO;
    TInt retValue( KErrCancel );

    CAknSinglePopupMenuStyleListBox* selectionList =
                new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( selectionList );

    CAknPopupList* popupList = CAknPopupList::NewL( 
                                             selectionList,
                                             R_AVKON_SOFTKEYS_SELECT_CANCEL );
    CleanupStack::PushL( popupList );

    selectionList->ConstructL(  popupList, EAknListBoxMenuList );

    // construct listbox item array
    CDesCArray *itemList = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( itemList );
    
    // First listbox item
    HBufC* newItem = StringLoader::LoadLC( R_QTN_CREATE_NEW_CONTACT  );
    itemList->AppendL( *newItem );
    CleanupStack::PopAndDestroy( newItem );
    
    // Second listbox item
    HBufC* updateItem = StringLoader::LoadLC( R_QTN_UPDATE_EXISTING  );
    itemList->AppendL( *updateItem );
    CleanupStack::PopAndDestroy( updateItem );

    // Set items and ownership
    selectionList->Model()->SetItemTextArray( itemList );
    selectionList->Model()->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop( itemList ); // ownership is transfered to selectionList

    HBufC* title = StringLoader::LoadLC( R_QTN_SELECT_ACTION );
    popupList->SetTitleL( *title );
    CleanupStack::PopAndDestroy( title );

    TInt response = popupList->ExecuteLD();

    if ( response )
        {
        retValue = selectionList->CurrentItemIndex();
        }
    CleanupStack::Pop( popupList ); // onership given to selectionList
    CleanupStack::PopAndDestroy( selectionList ); // selectionList

    return retValue;
    }
// CVIMPSTUiTabbedView::ShowLoginDialogL
// ---------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::ShowLoginDialogL( TDes& aUserName, TDes& aPassword )
    {
	TRACER_AUTO;
    HBufC* userName = HBufC::NewLC( KUsernameMaxLength );
    HBufC* passWord = HBufC::NewLC( KPasswordMaxLength );

    TPtr ptrUserName = userName->Des(); 
    TPtr ptrPassWord = passWord->Des();

    if( aUserName.Length()== 0 )
        {
        CAknMultiLineDataQueryDialog* dlg = CAknMultiLineDataQueryDialog::NewL( ptrUserName,
                ptrPassWord );
        if ( dlg->ExecuteLD( R_LOGIN_DIALOG ) )
            {
            aUserName.Copy( ptrUserName.LeftTPtr( aUserName.MaxLength() ) );
            aPassword.Copy( ptrPassWord.LeftTPtr( aPassword.MaxLength() ) );
            }
        else
            {
            User::Leave( KErrCancel );
            }
        }
    else
        {
        CAknMultiLineDataQueryDialog* dlg = CAknMultiLineDataQueryDialog::NewL( aUserName,
                aPassword );
        if ( dlg->ExecuteLD( R_LOGIN_DIALOG ) )
            {
            aUserName.Copy( aUserName.LeftTPtr( aUserName.MaxLength() ) );
            aPassword.Copy( aPassword.LeftTPtr( aPassword.MaxLength() ) );
            }
        else
            {
            User::Leave( KErrCancel );
            }
        }
    CleanupStack::PopAndDestroy( passWord );
    CleanupStack::PopAndDestroy( userName );
    }
// ----------------------------------------------------
// CVIMPSTUiTabbedView::HandleContentL
// Handles the content coming from the embedded browser.
// Returns EFalse: content will be passed on to framework
// ----------------------------------------------------
//
TBool CVIMPSTUiTabbedView::HandleContentL( 
        const TDesC& /*aFileName*/, 
        const CAiwGenericParamList& /*aParamList*/, 
        TBool& aContinue )
    {
    // Close browser after downloading content
    aContinue = EFalse;
    return EFalse;
    }

// ----------------------------------------------------
// CVIMPSTUiTabbedView::DownloadedContentHandlerReserved1
// ( Reserved for future use )
// ----------------------------------------------------
//
TAny* CVIMPSTUiTabbedView::DownloadedContentHandlerReserved1( TAny* /*aAnyParam*/ )
    {
    // Empty implementation.
    return NULL;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUiTabbedView::CloseConversation
// ---------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::CloseConversationL()
    {
	TRACER_AUTO;
    TInt index = CurrentItemIndex();    
    if( index <= 0 )
        {
        // return if item not found
        return; 
        }
    MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
    TPtrC itemId = arrayprocess.GetItemUserId(index);

    if(itemId.Length() == 0)
        {
        return;
        }

    HBufC* queryTxt = StringLoader::LoadLC( R_QTN_SERVTAB_CLOSE_CONV, itemId );

    //ask the query and send the response back to the server.
    TInt response = VIMPSTUtilsDialog::DisplayQueryDialogL(R_GENERIC_YES_NO_CONFIRMATION_QUERY,*queryTxt);
    CleanupStack::PopAndDestroy(queryTxt);

    if( (EAknSoftkeyYes == response)||(EAknSoftkeyOk == response ) )
        {
        iCommandHandler.HandleCommandL(ECloseConversation, this, &itemId ); 
        } 

    }

// ----------------------------------------------------
// CVIMPSTUiTabbedView::HandleServerAppExit
// ( Reserved for future use )
// ----------------------------------------------------
//
void CVIMPSTUiTabbedView::HandleServerAppExit( TInt /*aReason*/ )
    {
	TRACER_AUTO;
    TInt error = KErrNone;
    TRAPD(leaveErr,error = ShowTermsofuseL());
    if(KErrNone == error && KErrNone == leaveErr)
        {
        TRAP_IGNORE(iCommandHandler.HandleCommandL(ELoginService, this, &iServiceId)); 
        }
    }
    
// ----------------------------------------------------
// CVIMPSTUiTabbedView::ShowTermsofuseL
// ( Reserved for future use )
// ----------------------------------------------------
//
TInt CVIMPSTUiTabbedView::ShowTermsofuseL()
    {
	TRACER_AUTO;
    TInt retVal = KErrNone;
    // get the  ETOUDlgOff if its set don't show TOU dialog else show TOU 
    TInt termsOfUseDisabled = KErrNone;
    iSettingsStore->GetL(iServiceId, ETOUDlgOff, termsOfUseDisabled);
    if( !termsOfUseDisabled)
        {
        HBufC* termsOfUsetext = iBrandHandler->GetToUStringL();
        if( termsOfUsetext )
	        {
	        CleanupStack::PushL(termsOfUsetext);
	        retVal = CVIMPSTUiTermsOfUseDialogHandler::HandleTermsOfUseDialogLD(
                   												*iSettingsStore,
                   												 *iUserId, 
                   												 iServiceId, 
                   												 termsOfUsetext ); // termsOfUsetext ownership is transfered	
	        CleanupStack::Pop(); // termsOfUsetext ownership is transfered
	        }
        }
    return retVal;
    }
// ----------------------------------------------------
// CVIMPSTUiTabbedView::MakeVoipCallL
// ( Reserved for future use )
// ----------------------------------------------------
//
void CVIMPSTUiTabbedView::MakeVoipCallL()
    {
	TRACER_AUTO;
    // making voip call will take in three steps
    // pop up the selection dialog
    // select adress
    // create a call
    TInt index = CurrentItemIndex();  
    //Index should always be greater than zero
    //Negative index means item not found.
    if(index <= 0 )
        {
        return;
        }
    MVIMPSTProcessArray& processArray = iCommandHandler.GetProcessInterface();
    TVIMPSTEnums::TItem itemType = processArray.GetType( index ) ;
    
    if( itemType == TVIMPSTEnums::EContactItem )
        {
        iSelectedAction = CVIMPSTUiTabbedView::ECreateXSPCall;
        TUint fetchFlags = 0;
        TAiwSingleItemSelectionDataV3 data;
        data.SetFlags(fetchFlags);
        data.SetCommAddressSelectType(EAiwCommVOIPCall);
        ReInitializeServiceHandlerL();
        CAiwGenericParamList& inParamList = iServiceHandler->InParamListL();
        // Set service data
        inParamList.AppendL(TAiwGenericParam(EGenericParamContactSelectionData,
                TAiwVariant(TAiwSingleItemSelectionDataV3Pckg(data))));

        HBufC8* pack = processArray.ContactLink(index )->PackLC();
        // Set contactlink
        inParamList.AppendL(TAiwGenericParam(EGenericParamContactLinkArray,
                TAiwVariant(*pack)));

        iServiceHandler->ExecuteServiceCmdL(KAiwCmdSelect, inParamList,
                iServiceHandler->OutParamListL(), 0, this);
        CleanupStack::PopAndDestroy( pack );
        }
    else if( itemType == TVIMPSTEnums::EUnknonContactItem )
        {
        TPtrC unknownId = processArray.GetItemUserId( index );
        ExecuteAiwCallL( unknownId );
        }
    }
// ---------------------------------------------------------
// CVIMPSTUiTabbedView::ExecuteAiwCallL
// ---------------------------------------------------------
void CVIMPSTUiTabbedView::ExecuteAiwCallL( const TDesC& aSelectedAddress )
	{
	TRACER_AUTO;
	//aSelectedAdress will be an xsp id of format "ServiceName:UserId"
	//here we need to truncate the servicename and pass only the UserId
    //Find if aSelectedAdress of type "ServiceName:UserId"
	//if true, then remove the service part
    TInt phoneNumberStart = aSelectedAddress.Locate(':');
    HBufC* numBuf = NULL;
    
    if ( KErrNotFound != phoneNumberStart )
        {
        phoneNumberStart++; //To get rid of ':'
        numBuf = aSelectedAddress.Right( 
                                aSelectedAddress.Length() - phoneNumberStart 
                                ).AllocLC();
        }
    else
        {
        numBuf = aSelectedAddress.AllocLC();
        }
    
    TPtr numDesc( numBuf->Des ());
	CommonPhoneParser::ParsePhoneNumber ( numDesc,
			CommonPhoneParser::EPhoneClientNumber); 

	CAiwDialDataExt* dialDataExt = CAiwDialDataExt::NewLC();
	dialDataExt->SetPhoneNumberL ( numBuf->Des ());
    dialDataExt->SetServiceId(iServiceId);
    dialDataExt->SetCallType( CAiwDialData::EAIWVoiP);

	dialDataExt->SetWindowGroup ( CCoeEnv::Static()->RootWin().Identifier() );
	ReInitializeServiceHandlerL();
	CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
	dialDataExt->FillInParamListL ( paramList);
	iServiceHandler->ExecuteServiceCmdL ( KAiwCmdCall, 
										 paramList,
										 iServiceHandler->OutParamListL(), 
										 0, 
										 NULL);
	CleanupStack::PopAndDestroy(dialDataExt);
	CleanupStack::PopAndDestroy(numBuf);
	}

// ---------------------------------------------------------
// CVIMPSTUiTabbedView::LaunchHelpL()
// Function launches help application using the HlpLauncher.
// @params: Reference to a descriptor, serves as context.
// ---------------------------------------------------------
//

void CVIMPSTUiTabbedView::LaunchHelpL( const TDesC& aContext )
    {
	TRACER_AUTO;
    CArrayFix< TCoeHelpContext >* cntx = new( ELeave ) CArrayFixFlat< TCoeHelpContext >( KHelpContextSize );
    CleanupStack::PushL( cntx );

    TUid KHelpUid = { KHELPUID };
    cntx->AppendL( TCoeHelpContext( KHelpUid, aContext ) );
    CleanupStack::Pop( cntx );

    //and launch help - takes ownership of context array
    HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), cntx );    
    }

// ----------------------------------------------------
// CVIMPSTUiTabbedView::SettingsStore()
// ----------------------------------------------------
//
MVIMPSTSettingsStore* CVIMPSTUiTabbedView::SettingsStore()
    {
    return iSettingsStore;
    }

// ----------------------------------------------------
// CVIMPSTUiTabbedView::CancelLoginSupportedL()
// ----------------------------------------------------
//
TBool CVIMPSTUiTabbedView::CancelLoginSupportedL()
    {
	TRACER_AUTO;
    TInt cancelNotSupported(KErrNone);
    TBool returnValue = ETrue;
    // read from the cenrep settings 
    TInt err = iSettingsStore->GetL(iServiceId, EServiceCancelLoginNotSupported,cancelNotSupported);
    // if any error  OR  cancelNotSupported = 1 then 
    if ( err || cancelNotSupported )
        {
        returnValue =  EFalse; // SOME Error or cancel is not supported : return EFalse
        }
    return returnValue; // cancel is supported : return ETrue
    }


// ----------------------------------------------------
// CVIMPSTUiTabbedView::ChangeConnectionSupportedL()
// ----------------------------------------------------
//
TBool CVIMPSTUiTabbedView::ChangeConnectionSupportedL()
    {
	TRACER_AUTO;
    TInt changeConnectionSupported(KErrNone);
    TBool returnValue = ETrue;
    // read from the cenrep settings 
    TInt err = iSettingsStore->GetL(iServiceId, EServiceChangeConnectionNotSupported,changeConnectionSupported);
    // if any error  OR  cancelNotSupported = 1 then 
    if ( err || changeConnectionSupported )
        {
        returnValue =  EFalse; // SOME Error or cancel is not supported : return EFalse
        }
    return returnValue; // cancel is supported : return ETrue
    }

// ----------------------------------------------------
// CVIMPSTUiTabbedView::HandleNotifyL()
// ----------------------------------------------------
//
TInt CVIMPSTUiTabbedView::HandleNotifyL(
        TInt aCmdId,
        TInt aEventId,
        CAiwGenericParamList& aEventParamList,
        const CAiwGenericParamList& /* aInParamList */)
	{
	TRACER_AUTO;
	TInt result = 0;
		if( (aCmdId == KAiwCmdSelect) && (aEventId == KAiwEventCompleted && iSelectedAction == ECreateXSPCall ))
			{
			if( iQueryResult )
				{
				delete iQueryResult;
				iQueryResult = NULL;	
				}
			TInt index = 1; // Get field data from index 1
			const TAiwGenericParam* param =
			aEventParamList.FindFirst(index, EGenericParamContactFieldData);
			if (KErrNotFound != index)
				{
				TInt length = param->Value().AsDes().Length();
				iQueryResult = HBufC::NewL(length);
				iQueryResult->Des().Append( param->Value().AsDes() );
				// Data must be found!
				if (0 >= iQueryResult->Length())
					{
					User::Leave(KErrGeneral);
					}
				if( !iContactSelection )
					{
					iContactSelection = CVIMPSTUiContactSelection::NewL(*this);	//register for the contactselection
					}
				iContactSelection->IssueRequest(); 	
				}
			else
				{
				User::Leave(KErrNotFound);
				}
			result = ETrue;
			}
		
		else if (aCmdId == KAiwCmdAssign || aCmdId == KAiwCmdSelect )
		{
		if (aEventId == KAiwEventCompleted )
			{
			if( iSelectedAction == CVIMPSTUiTabbedView::EInvitAcceptUpdateExisting || 
				iSelectedAction == CVIMPSTUiTabbedView::EInvitAcceptCreateNew || 
				iSelectedAction == CVIMPSTUiTabbedView::EContactCreateNew ||
				iSelectedAction == CVIMPSTUiTabbedView::EContactAddFromContacts )
				{
				// action has been completed ,
				// this will be excuted only if invitation accepted using update existing contact
				TInt paramIndex = 0;
				const TAiwGenericParam* contactParam = aEventParamList.FindFirst(
				paramIndex, EGenericParamContactLinkArray );//get the contact link
				TPtrC8 packLink = contactParam->Value().AsData();
				delete iSelectedData.iPackedLinks;
				iSelectedData.iPackedLinks = NULL; 
				if( packLink.Length() )
						{
						iSelectedData.iPackedLinks = packLink.AllocL();
						if( !iContactSelection )
							{
							iContactSelection = CVIMPSTUiContactSelection::NewL(*this);	//register for the contactselection
							}
						iContactSelection->IssueRequest(); 	
						}
				}
			result = ETrue;
			}
		else if (aEventId == KAiwEventQueryExit)
		    {
		    // Send an exit command in order to close CCA and Phonebook2        
		    CAknAppUi* appUi = static_cast<CAknAppUi*>( CCoeEnv::Static()->AppUi() );

		    if ( appUi )
		        {
		        appUi->ProcessCommandL( EAknCmdExit );
		        }
		    result = ETrue;
		    }
		else if(aEventId == KAiwEventOutParamCheck)
		    {
		        result = ETrue;
		    }
		}
    if(aEventId == KAiwEventCanceled)
		{
		result = ETrue;
		}
	return result;
	}
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::BlockContactL
// --------------------------------------------------------------------------
//
void CVIMPSTUiTabbedView::BlockContactL(TBool aBlock /* = EFalse */)
    {
	TRACER_AUTO;
    TInt index = CurrentItemIndex();
    if(iContainer)
        {
        iContainer->CheckAndSetLastItemFlag();
        }
    if( index <= 0 )
        {
        // return if item not found
        // 0 for own item which can not be blocked
        return; 
        }   
         if(iWaitNote)
             {
             delete iWaitNote;
             iWaitNote = NULL;
             }

    MVIMPSTProcessArray& arrayprocess = iCommandHandler.GetProcessInterface();
    HBufC* contactId =  arrayprocess.GetItemUserId(index).AllocLC();

    if( contactId->Length() )
        {
        if(aBlock )
            {
            // load, format and show confirmation note
            HBufC* prompt = StringLoader::LoadLC( R_QTN_SERVTAB_BLOCKING, *contactId );
            iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( *prompt, ETrue, EFalse );
          //  iCommandHandler.HandleCommandL(ECmdBlockContact, this, &(contactId) );
            iCommandHandler.HandleCommandL(ECmdBlockContact, this, contactId );
            CleanupStack::PopAndDestroy();// prompt
            
        	if(iContainer)
                {
                     TRACE("HandleAvatarRefreshl calling for blocking");		   
                
		iContainer->HandleAvatarRefreshL(*contactId, ETrue);
                }
            }
        else 
            {
            HBufC* unBprompt = StringLoader::LoadLC( R_QTN_CHAT_UNBLOCKING, *contactId );
            // load, format and show confirmation note
            iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( *unBprompt, ETrue, EFalse );
            iCommandHandler.HandleCommandL(ECmdUnBlockContact, this, contactId );
            CleanupStack::PopAndDestroy();// unBprompt
            }
        }
    CleanupStack::PopAndDestroy(); //contactId
    } 
    
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ShowBlockedContactListL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::ShowBlockedContactListL()
	{
	TRACER_AUTO;
	if(!iIsToolBarHidden)
	    {
	    ShowHideFixedToolbarL(EFalse);
	    }

	if (iContainer)
		{
		iContainer->DeactivateFindPaneL();
		}

	//Launch the search view with search dialog
	LaunchViewL( iExtensionService.BlockViewId() );
	}
	
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::HandleContactSelectionCompleteL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::HandleContactSelectionCompleteL()
	{
	TRACER_AUTO;
	
	 if( iWaitNote )
	        {
	        delete iWaitNote;
	        iWaitNote = NULL;
	        }
     if( iSelectedAction == CVIMPSTUiTabbedView::EInvitAcceptUpdateExisting || iSelectedAction == CVIMPSTUiTabbedView::EInvitAcceptCreateNew)
        {
        iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_SERVTAB_SAVING_CONTACT, ETrue, EFalse );

        if ( iSelectedData.iPackedLinks )                     //add the selected contact to the service tab
            {
            TRACE("process contact");
            if( iSelectedAction == CVIMPSTUiTabbedView::EInvitAcceptCreateNew )
                   {
                 iCommandHandler.HandleCommandL( ECmdProcessAcceptedContactNew, this, (TAny*)&iSelectedData );
                   }
            else
                   {
                 iCommandHandler.HandleCommandL( ECmdProcessAcceptedContactExist, this, (TAny*)&iSelectedData );
                   }
            delete iSelectedData.iPackedLinks;  
            iSelectedData.iPackedLinks = NULL;           
            } 
        iSelectedData.iXSPList.ResetAndDestroy();
       
        //invitation has been accepted using update existing contact
        // updation is successful
        // send accepted request to command/engine/server   
        TRACE("frnrequest accepted");
        iCommandHandler.HandleCommandL(EFriendAccpeted, this, iFriendRequestId );
        }
     else if(  iSelectedAction == CVIMPSTUiTabbedView::EContactCreateNew ||  iSelectedAction == CVIMPSTUiTabbedView::EContactAddFromContacts)
         {
         MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId() ) ;
         TBool localStore = EFalse;
         if( storage )
             {
             localStore = storage->IsLocalStore();
             }
         if( localStore )//for server store wait note is shown by AddContactL function
             {
             if(iWaitNote)
                 {
                 delete iWaitNote;
                 iWaitNote = NULL;
                 }
             iWaitNote = CVIMPSTUtilsWaitNote::ShowWaitNoteL( R_QTN_SERVTAB_SAVING_CONTACT, ETrue, EFalse );
             }

         if ( iSelectedData.iPackedLinks )                     //add the selected contact to the service tab
             {
           TRACE("process contact");
             if( iSelectedAction == CVIMPSTUiTabbedView::EContactCreateNew )
                 {
                 iCommandHandler.HandleCommandL( ECmdProcessSelectedContactNew, this, (TAny*)&iSelectedData );
                 }
             else
                 {
                 iCommandHandler.HandleCommandL( ECmdProcessSelectedContactExist, this, (TAny*)&iSelectedData );
                 }
             delete iSelectedData.iPackedLinks;  
             iSelectedData.iPackedLinks = NULL;
             //server store xspid will be stored in iSelectedData.iXSPList and adding it to UI
             if( !localStore && iSelectedAction == CVIMPSTUiTabbedView::EContactAddFromContacts)
                 {
                 AddContactL( iSelectedData.iXSPList );
                 }       
             iSelectedData.iXSPList.ResetAndDestroy();

             } 
         // this is for adding a fresh contact for local store
	     // read the data from link and send it to server for updation	
         }
	 else if( iSelectedAction == CVIMPSTUiTabbedView::ECreateXSPCall && iQueryResult )
		 {
		 // make a call here
		 ExecuteAiwCallL(*iQueryResult );
		 delete iQueryResult;
		 iQueryResult = NULL;	
		 }
    iSelectedAction = KErrNotFound;  // This is to clear the previous selected action.
      
	}

// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::LaunchSettingsViewL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::LaunchSettingsViewL()
    {
    /*
    If settingsui is already opened, instead of starting with StartDocument just bring csc to foreground and send message
    (csc opens correct settings ui even when it´s already open)What comes to coming back to meco tab by pressing Back in 
    settingsui i made change that tab view is opened by using RxSPViewServices in csc side and csc is closed after that.
    I found only one problem while testing this solution, that is if phonebook is closed when settingsui is still open. 
    In that case settingsui stays in background, but even then when Back is pressed on settingsui correct tab is opened 
    (phonebook is reopened using RxSPViewServices)
    
    **************************************************************************************
    CSC team agreed to provide new generic CSC view launcher API, once CSC provide new API
    below code need to remove and new API will be called.
    ***************************************************************************************
    */
	TRACER_AUTO; 
    _LIT(KStartupParams, "ACTION=4 SERID=");
    _LIT8(KStartupParams8, "ACTION=4 SERID=");
    //Take CSC UID (In future if this UID change , we also need to change it otherwise we will not be able
    //to launch CSC settings.
    const TUid KCSCAppUid = { 0x10275458 };
    RWsSession ws;
    User::LeaveIfError( ws.Connect() );
    CleanupClosePushL( ws );
    TApaTaskList list( ws );

    TApaTask task( list.FindApp( KCSCAppUid ) );

    if ( task.Exists() )
        {
        TBuf8<255> params8;
        params8.Copy( KStartupParams8 );
        params8.AppendNum( iServiceId );
       //Bring CSC to foreground if already open
        task.BringToForeground();
        task.SendMessage( 
                TUid::Uid( KUidApaMessageSwitchOpenFileValue ), params8 );
        }
    else
        {
        RApaLsSession session;
        CleanupClosePushL( session );                       
        User::LeaveIfError(session.Connect());

        TUid appUid = KCSCAppUid;
        TThreadId threadId;
        TFileName fileName;

        fileName.Copy( KStartupParams ); 
        fileName.AppendNum( iServiceId );

        User::LeaveIfError(
                session.StartDocument( fileName, appUid, threadId ) );

        session.Close();                       
        CleanupStack::PopAndDestroy( &session );
        }

    CleanupStack::PopAndDestroy( &ws );

    }//End LaunchSettingsUiL() 
	// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ReInitializeServiceHandlerL()
// --------------------------------------------------------------------------

void CVIMPSTUiTabbedView::ReInitializeServiceHandlerL()
    {
	TRACER_AUTO;
    if(iIsServiceHandleReSet )
        {
        if(iServiceHandler)
            {
            iServiceHandler->AttachL( R_IM_APP_INTEREST );
            iServiceHandler->AttachMenuL( R_SERVTAB_TABBED_VIEW_MENU, 
                R_IM_APP_INTEREST );   
            
            iServiceHandler->AttachL( R_AIWASSIGNMENT_INTEREST );    
            
            iServiceHandler->AttachL( R_AIWSELECT_INTEREST );
            iIsServiceHandleReSet = EFalse;
            }
        }
    }
	// --------------------------------------------------------------------------
	// CVIMPSTUiTabbedView::LaunchFetchDialogL()
	// --------------------------------------------------------------------------

void CVIMPSTUiTabbedView::LaunchFetchDialogL()
    {
	TRACER_AUTO;
    iSelectedAction = EContactAddFromContacts;
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId() ) ;
    TBool localStore = EFalse;
    if( storage )
        {
        localStore = storage->IsLocalStore();
        }
    
    if( localStore )
        {
        //multiple item fetch.
        TUint fetchFlags = 0;
        fetchFlags |= ::EExcludeGroupsView;
        
        ExecuteMultipleItemFetchL( TAiwMultipleEntrySelectionDataV2().SetFlags( fetchFlags ) );
        }
    else
        {
        //single item fetch.
        TUint fetchFlags = 0;

        ExecuteSingleItemFetchL( TAiwSingleEntrySelectionDataV2().SetFlags( fetchFlags ) );
        }

    }
	// --------------------------------------------------------------------------
	// CVIMPSTUiTabbedView::ExecuteSingleItemFetchL
	// --------------------------------------------------------------------------

void CVIMPSTUiTabbedView::ExecuteSingleItemFetchL( TAiwSingleEntrySelectionDataV2 aData )
    {
	TRACER_AUTO;
    if(iAiwParamList)
           {
           delete iAiwParamList;
           iAiwParamList = NULL;
           }        
       iAiwParamList = CAiwGenericParamList::NewL();
       iAiwParamList->AppendL(
            TAiwGenericParam(
            EGenericParamContactSelectionData,
            TAiwVariant(TAiwSingleEntrySelectionDataV2Pckg(aData))));

       ReInitializeServiceHandlerL();
    iServiceHandler->ExecuteServiceCmdL(
                                         KAiwCmdSelect,
                                         *iAiwParamList,
                                         iServiceHandler->OutParamListL(),
                                         0,
                                         this
                                         );

    }
// --------------------------------------------------------------------------
// CVIMPSTUiTabbedView::ExecuteMultipleItemFetchL
// --------------------------------------------------------------------------
void CVIMPSTUiTabbedView::ExecuteMultipleItemFetchL( TAiwMultipleEntrySelectionDataV2 aData )
    {
	TRACER_AUTO;
    if(iAiwParamList)
        {
        delete iAiwParamList;
        iAiwParamList = NULL;
        }        
    iAiwParamList = CAiwGenericParamList::NewL();
    ReInitializeServiceHandlerL();
    iAiwParamList->AppendL(
            TAiwGenericParam(
                    EGenericParamContactSelectionData,
                    TAiwVariant(TAiwMultipleEntrySelectionDataV2Pckg(aData))));

  
    iServiceHandler->ExecuteServiceCmdL(
            KAiwCmdSelect,
            *iAiwParamList,
            iServiceHandler->OutParamListL(),
            0,
            this
    );
    }
//  End of File
