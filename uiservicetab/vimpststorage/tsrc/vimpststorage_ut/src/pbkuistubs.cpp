/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/





// INCLUDES
#include "pbkuistubs.h"


/**
 * Returns Phonebook 2 application UI interface.
 * Usage: MPbk2AppUi* appUi = Phonebook2::Pbk2AppUi();
 *
 * @return  Phonebook 2 application UI interface.
 */
EXPORT_C MPbk2AppUi* Phonebook2::Pbk2AppUi()
{
return NULL;	
};

CPbk2AppUiStub::CPbk2AppUiStub(){ }

CPbk2AppUiStub::~CPbk2AppUiStub(){ }

/**
 * Returns the currently active view.
 *
 * @return  Active view.
 */
CPbk2AppViewBase* CPbk2AppUiStub::ActiveView() const { return NULL; }

/**
 * Returns the Phonebook 2 view explorer if it exists.
 *
 * @return  View explorer.
 */
MPbk2ViewExplorer* CPbk2AppUiStub::Pbk2ViewExplorer() const { return NULL; }

/**
 * Returns the Phonebook 2 startup monitor.
 *
 * @return  Startup monitor.
 */
MPbk2StartupMonitor* CPbk2AppUiStub::Pbk2StartupMonitor() const { return NULL; }

/**
 * Returns Phonebook 2 tab group container.
 *
 * @return  Tab group container.
 */
CPbk2TabGroupContainer* CPbk2AppUiStub::TabGroups() const { return NULL; }

/**
 * Returns Phonebook 2 key event handler.
 *
 * @return  Key event handler.
 */
MPbk2KeyEventHandler& CPbk2AppUiStub::KeyEventHandler() 
{ 
MPbk2KeyEventHandler* ret = NULL;
return *ret;
}

/**
 * Returns Phonebook 2 application services.
 *
 * @return  Application services.
 */
MPbk2ApplicationServices& CPbk2AppUiStub::ApplicationServices() const 
{ 
MPbk2ApplicationServices* ret = NULL;
return *ret;
}

/**
 * Handles a command. If Phonebook 2 command handler does not
 * handle a command it can then be passed to application UI
 * command handler which ultimately handles it.
 * For example, exit command is handled by
 * application UI not by command handler.
 *
 * @return  Id of the command to handle.
 */
void CPbk2AppUiStub::HandleCommandL(
        TInt /*aCommand*/ ) { }

/**
 * Exits application.
 */
void CPbk2AppUiStub::ExitApplication() { }

CPbk2ApplicationServicesStub::CPbk2ApplicationServicesStub(){ }

CPbk2ApplicationServicesStub::~CPbk2ApplicationServicesStub(){ }

/**
 * Returns Virtual Phonebook contact manager.
 *
 * @return  Virtual Phonebook contact manager.
 */
CVPbkContactManager& CPbk2ApplicationServicesStub::ContactManager() const 
{ 
CVPbkContactManager* ret = NULL;
return *ret;
}

/**
 * Returns Phonebook 2 command handler, which handles
 * all the commands.
 *
 * @return  Phonebook 2 command handler.
 */
MPbk2CommandHandler* CPbk2ApplicationServicesStub::CommandHandlerL() 
{ 
return NULL;
}

/**
 * Returns the name formatter object.
 *
 * @return  Phonebook 2 name formatter.
 */
MPbk2ContactNameFormatter& CPbk2ApplicationServicesStub::NameFormatter() const 
{ 
MPbk2ContactNameFormatter* ret = NULL;
return *ret;
}

/**
 * Returns the sort order manager.
 *
 * @return  Sort order manager.
 */
CPbk2SortOrderManager& CPbk2ApplicationServicesStub::SortOrderManager() const 
{ 
CPbk2SortOrderManager* ret = NULL;
return *ret;
}

/**
 * Returns Phonebook 2 global send UI.
 *
 * @return  SendUi instance.
 */
CSendUi* CPbk2ApplicationServicesStub::SendUiL() { return NULL; }

/**
 * Returns store property array.
 *
 * @return  Store property array.
 */
CPbk2StorePropertyArray& CPbk2ApplicationServicesStub::StoreProperties() const 
{ 
CPbk2StorePropertyArray* ret = NULL;
return *ret;
}

/**
 * Returns Phonebook 2 field properties.
 *
 * @return  Phonebook 2 field properties.
 */
CPbk2FieldPropertyArray& CPbk2ApplicationServicesStub::FieldProperties() const 
{ 
CPbk2FieldPropertyArray* ret = NULL;
return *ret;
}

/**
 * Returns Phonebook 2 store configuration.
 *
 * @return  Phonebook 2 store configuration.
 */
CPbk2StoreConfiguration& CPbk2ApplicationServicesStub::StoreConfiguration() const 
{ 
CPbk2StoreConfiguration* ret=NULL;
return *ret;
}

/**
 * Returns Phonebook 2 contact store observation register.
 *
 * @return  Phonebook 2 store observation register.
 */
MPbk2StoreObservationRegister& CPbk2ApplicationServicesStub::
    StoreObservationRegister() const 
    { 
    MPbk2StoreObservationRegister* ret = NULL;
    return *ret;
    }
    

/**
 * Returns Phonebook 2 contact store validity informer.
 *
 * @return  Phonebook 2 store validity informer.
 */
MPbk2StoreValidityInformer& CPbk2ApplicationServicesStub::
    StoreValidityInformer() const 
    { 
    MPbk2StoreValidityInformer* ret = NULL;
    return *ret;
    }

/**
 * Returns Phonebook 2 view supplier.
 *
 * @return  Phonebook 2 view supplier.
 */
MPbk2ContactViewSupplier& CPbk2ApplicationServicesStub::ViewSupplier() const 
{ 
MPbk2ContactViewSupplier* ret = NULL;
return *ret;
}

/**
 * Returns global menu filtering flags.
 *
 * @return  Global menu filtering flags.
 */
TInt CPbk2ApplicationServicesStub::GlobalMenuFilteringFlagsL() const { return 0; }

/**
 * Returns service manager.
 *
 * @return  Service manager.
 */
CPbk2ServiceManager& CPbk2ApplicationServicesStub::ServiceManager() const 
{ 
CPbk2ServiceManager* ret = NULL;
return *ret;
}

CPbk2CommandHandlerStub::CPbk2CommandHandlerStub(){ }

CPbk2CommandHandlerStub::~CPbk2CommandHandlerStub(){ }
/**
 * Handles Phonebook 2 commands.
 *
 * @param aCommandId    The id of the command to handle.
 * @param aControl      The UI control of the calling view.
 * @param aAppView      Application view from where
 *                      the command is launched.
 * @return  ETrue if the command was handled, EFalse otherwise.
 */
TBool CPbk2CommandHandlerStub::HandleCommandL(
        const TInt /*aCommandId*/,
        MPbk2ContactUiControl& /*aControl*/,
        const CPbk2AppViewBase* /*aAppView*/ ) { return EFalse; }

/**
 * Performs Phonebook 2 menu filtering.
 *
 * @param aResourceId   Resource id of the filtered menu pane.
 * @param aMenuPane     Menu pane to be filtered.
 * @param aViewBase     Application view where the filtering
 *                      takes place.
 * @param aControl      The UI control of the calling view.
 */
void CPbk2CommandHandlerStub::DynInitMenuPaneL(
        TInt /*aResourceId*/,
        CEikMenuPane* /*aMenuPane*/,
        CPbk2AppViewBase& /*aViewBase*/,
        MPbk2ContactUiControl& /*aControl*/ ) { }

/**
 * Registers (attaches) to an AIW service.
 *
 * @param aInterestId           Id of the AIW interest.
 * @param aMenuResourceId       Resource id of the menu to
 *                              attach to the AIW interest.
 * @param aInterestResourceId   Resource id of the AIW interest.
 * @param aAttachBaseService    If ETrue, attaches the AIW
 *                              resource id to the AIW base
 *                              service also. This enables command
 *                              handling via AIW without menus.
 */
void CPbk2CommandHandlerStub::RegisterAiwInterestL(
        const TInt /*aInterestId*/,
        const TInt /*aMenuResourceId*/,
        const TInt /*aInterestResourceId*/,
        const TBool /*aAttachBaseService*/ ) { }

/**
 * Returns the AIW service command id associated to
 * given menu command. If found, it means that there is a
 * provider which can handle the menu command.
 *
 * @see CAiwServiceHandler::ServiceCmdByMenuCmd
 * @param aMenuCmdId    Menu command id to inspect.
 * @return  Service command id or KAiwCmdNone.
 */
TInt CPbk2CommandHandlerStub::ServiceCmdByMenuCmd(
        TInt /*aMenuCmdId*/ ) const { return 0; }

/**
 * Adds the command into the Phonebook 2 command store and executes it.
 * Takes the ownership of the command.
 *
 * @param aCommand  The command to be executed.
 */
void CPbk2CommandHandlerStub::AddAndExecuteCommandL(
        MPbk2Command* /*aCommand*/ ) { }

/**
 * Adds a menu command observer.
 *
 * @param aObserver     Menu command observer to add.
 */
void CPbk2CommandHandlerStub::AddMenuCommandObserver(
        MPbk2MenuCommandObserver& /*aObserver*/ ) { }

/**
 * Removes a menu command observer.
 *
 * @param aObserver     Menu command observer to remove.
 */
void CPbk2CommandHandlerStub::RemoveMenuCommandObserver(
        MPbk2MenuCommandObserver& /*aObserver*/ ) { }

/**
 * Initialises toolbar before it is drawn.
 *
 * @param aResourceId   The resource id of the toolbar.
 * @param aToolbar      The toolbar object.
 * @param aAppView      Application view from where
 *                      the command is launched.
 * @param aControl      The UI control of the calling view.
 */
void CPbk2CommandHandlerStub::DynInitToolbarL(
        TInt /*aResourceId*/,
        CAknToolbar* /*aToolbar*/,
        const CPbk2AppViewBase& /*aAppView*/,
        MPbk2ContactUiControl& /*aControl*/ ) { }

/**
 * Handles toolbar events for a certain toolbar item.
 *
 * @param aCommand      The command id of the toolbar item.
 * @param aControl      The UI control of the calling view.
 * @param aAppView      Application view from where
 *                      the command is launched.
 */
void CPbk2CommandHandlerStub::OfferToolbarEventL(
        TInt /*aCommand*/,
        MPbk2ContactUiControl& /*aControl*/,
        const CPbk2AppViewBase* /*aAppView*/ ) { }


// End of File
