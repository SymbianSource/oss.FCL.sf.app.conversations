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



#ifndef STUBPBK2APPUI_H
#define STUBPBK2APPUI_H

// INCLUDES
#include <MPbk2AppUi.h>
#include <e32base.h>
#include <MPbk2ApplicationServices.h>
#include <MPbk2CommandHandler.h>

// CLASS DECLARATION
class CPbk2ServiceManager;

/**
 * Phonebook 2 application UI interface.
 */
class CPbk2AppUiStub : public CBase, public MPbk2AppUi
    {
    public: // Interface

		CPbk2AppUiStub();
		
		~CPbk2AppUiStub();
		
        /**
         * Returns the currently active view.
         *
         * @return  Active view.
         */
        CPbk2AppViewBase* ActiveView() const ;
 
        /**
         * Returns the Phonebook 2 view explorer if it exists.
         *
         * @return  View explorer.
         */
        MPbk2ViewExplorer* Pbk2ViewExplorer() const ;

        /**
         * Returns the Phonebook 2 startup monitor.
         *
         * @return  Startup monitor.
         */
        MPbk2StartupMonitor* Pbk2StartupMonitor() const ;

        /**
         * Returns Phonebook 2 tab group container.
         *
         * @return  Tab group container.
         */
        CPbk2TabGroupContainer* TabGroups() const ;

        /**
         * Returns Phonebook 2 key event handler.
         *
         * @return  Key event handler.
         */
        MPbk2KeyEventHandler& KeyEventHandler() ;

        /**
         * Returns Phonebook 2 application services.
         *
         * @return  Application services.
         */
        MPbk2ApplicationServices& ApplicationServices() const ;

        /**
         * Handles a command. If Phonebook 2 command handler does not
         * handle a command it can then be passed to application UI
         * command handler which ultimately handles it.
         * For example, exit command is handled by
         * application UI not by command handler.
         *
         * @return  Id of the command to handle.
         */
        void HandleCommandL(
                TInt aCommand ) ;

        /**
         * Exits application.
         */
        void ExitApplication() ;
    };



// CLASS DECLARATION

/**
 * Phonebook 2 application services interface.
 * Shares commonly used objects.
 */
class CPbk2ApplicationServicesStub : public MPbk2ApplicationServices
    {
    public:  // Interface

		CPbk2ApplicationServicesStub();
		
		~CPbk2ApplicationServicesStub();
		
        /**
         * Returns Virtual Phonebook contact manager.
         *
         * @return  Virtual Phonebook contact manager.
         */
        CVPbkContactManager& ContactManager() const ;

        /**
         * Returns Phonebook 2 command handler, which handles
         * all the commands.
         *
         * @return  Phonebook 2 command handler.
         */
        MPbk2CommandHandler* CommandHandlerL() ;

        /**
         * Returns the name formatter object.
         *
         * @return  Phonebook 2 name formatter.
         */
        MPbk2ContactNameFormatter& NameFormatter() const ;

        /**
         * Returns the sort order manager.
         *
         * @return  Sort order manager.
         */
        CPbk2SortOrderManager& SortOrderManager() const ;

        /**
         * Returns Phonebook 2 global send UI.
         *
         * @return  SendUi instance.
         */
        CSendUi* SendUiL() ;

        /**
         * Returns store property array.
         *
         * @return  Store property array.
         */
        CPbk2StorePropertyArray& StoreProperties() const ;

        /**
         * Returns Phonebook 2 field properties.
         *
         * @return  Phonebook 2 field properties.
         */
        CPbk2FieldPropertyArray& FieldProperties() const ;

        /**
         * Returns Phonebook 2 store configuration.
         *
         * @return  Phonebook 2 store configuration.
         */
        CPbk2StoreConfiguration& StoreConfiguration() const ;

        /**
         * Returns Phonebook 2 contact store observation register.
         *
         * @return  Phonebook 2 store observation register.
         */
        MPbk2StoreObservationRegister&
            StoreObservationRegister() const ;

        /**
         * Returns Phonebook 2 contact store validity informer.
         *
         * @return  Phonebook 2 store validity informer.
         */
        MPbk2StoreValidityInformer&
            StoreValidityInformer() const ;

        /**
         * Returns Phonebook 2 view supplier.
         *
         * @return  Phonebook 2 view supplier.
         */
        MPbk2ContactViewSupplier& ViewSupplier() const ;

        /**
         * Returns global menu filtering flags.
         *
         * @return  Global menu filtering flags.
         */
        TInt GlobalMenuFilteringFlagsL() const ;

        /**
         * Returns service manager.
         *
         * @return  Service manager.
         */
        CPbk2ServiceManager& ServiceManager() const ;
        
        
    };


/**
 * Phonebook 2 command handler interface.
 */
class CPbk2CommandHandlerStub : public MPbk2CommandHandler
    {
    public: // Interface

		CPbk2CommandHandlerStub();
		
		~CPbk2CommandHandlerStub();
        /**
         * Handles Phonebook 2 commands.
         *
         * @param aCommandId    The id of the command to handle.
         * @param aControl      The UI control of the calling view.
         * @param aAppView      Application view from where
         *                      the command is launched.
         * @return  ETrue if the command was handled, EFalse otherwise.
         */
        TBool HandleCommandL(
                const TInt aCommandId,
                MPbk2ContactUiControl& aControl,
                const CPbk2AppViewBase* aAppView ) ;

        /**
         * Performs Phonebook 2 menu filtering.
         *
         * @param aResourceId   Resource id of the filtered menu pane.
         * @param aMenuPane     Menu pane to be filtered.
         * @param aViewBase     Application view where the filtering
         *                      takes place.
         * @param aControl      The UI control of the calling view.
         */
        void DynInitMenuPaneL(
                TInt aResourceId,
                CEikMenuPane* aMenuPane,
                CPbk2AppViewBase& aViewBase,
                MPbk2ContactUiControl& aControl ) ;

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
        void RegisterAiwInterestL(
                const TInt aInterestId,
                const TInt aMenuResourceId,
                const TInt aInterestResourceId,
                const TBool aAttachBaseService ) ;

        /**
         * Returns the AIW service command id associated to
         * given menu command. If found, it means that there is a
         * provider which can handle the menu command.
         *
         * @see CAiwServiceHandler::ServiceCmdByMenuCmd
         * @param aMenuCmdId    Menu command id to inspect.
         * @return  Service command id or KAiwCmdNone.
         */
        TInt ServiceCmdByMenuCmd(
                TInt aMenuCmdId ) const ;

        /**
         * Adds the command into the Phonebook 2 command store and executes it.
         * Takes the ownership of the command.
         *
         * @param aCommand  The command to be executed.
         */
        void AddAndExecuteCommandL(
                MPbk2Command* aCommand ) ;

        /**
         * Adds a menu command observer.
         *
         * @param aObserver     Menu command observer to add.
         */
        void AddMenuCommandObserver(
                MPbk2MenuCommandObserver& aObserver ) ;

        /**
         * Removes a menu command observer.
         *
         * @param aObserver     Menu command observer to remove.
         */
        void RemoveMenuCommandObserver(
                MPbk2MenuCommandObserver& aObserver ) ;

        /**
         * Initialises toolbar before it is drawn.
         *
         * @param aResourceId   The resource id of the toolbar.
         * @param aToolbar      The toolbar object.
         * @param aAppView      Application view from where
         *                      the command is launched.
         * @param aControl      The UI control of the calling view.
         */
        void DynInitToolbarL(
                TInt aResourceId,
                CAknToolbar* aToolbar,
                const CPbk2AppViewBase& aAppView,
                MPbk2ContactUiControl& aControl ) ;

        /**
         * Handles toolbar events for a certain toolbar item.
         *
         * @param aCommand      The command id of the toolbar item.
         * @param aControl      The UI control of the calling view.
         * @param aAppView      Application view from where
         *                      the command is launched.
         */
        void OfferToolbarEventL(
                TInt aCommand,
                MPbk2ContactUiControl& aControl,
                const CPbk2AppViewBase* aAppView ) ;

        
    };

#endif // STUBPBK2APPUI_H


// End of File
