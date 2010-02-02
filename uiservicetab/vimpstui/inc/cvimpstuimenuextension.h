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
* Description:  cvimpstuimenuextension.h
 *
*/

#ifndef     CVIMPSUIMENUEXTENSION_H
#define     CVIMPSUIMENUEXTENSION_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "mcommandmapper.h"
#include "cmenuextension.h"
#include <mimpstuiaiwitemcontrol.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CEikMenuPane;
class CCommandInfo;


// CLASS DECLARATION

/**
 *  
 *  This class provides extensions to options menu.
 *
 *  @lib vimpstui.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CVIMPSTUIMenuExtension ) : public CBase,
                                              public MCommandMapper,
                                              public MVIMPSTUiAIWControl
    {
    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CVIMPSTUIMenuExtension* NewL(  );

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CVIMPSTUIMenuExtension* NewLC();

        /**
         * Standard C++ destructor
         */    
        ~CVIMPSTUIMenuExtension();
    public: // fomr MCommandMapper
        /**
         *  Gets old original "Service Tab" command ID which corresponds
         *  to the given new one
         *
         *  @param aPluginId  implementation ID for identification
         *  @param aNewCommand  New mapped command ID
         *  @param aOldCommand  On return old/original command ID
         *  @return KErrNone if mapping exist, otherwise KErrNotFound
         */
         TInt GetOldCommand( TInt32 aPluginId, TInt aNewCommand,
                                            TInt& aOldCommand ) const; 
         
         /**
          *  Gets new mapped "ServiceTab" command ID which corresponds
          *  to the given old one
          *
          *  @param aPluginId  implementation ID for identification
          *  @param aOldCommand Old/original command ID
          *  @param aNewCommand On return new mapped command ID
          *  @return KErrNone if mapping exist, otherwise KErrNotFound
          */
         TInt GetNewCommand(  TInt32 aPluginId,TInt aOldCommand,
                              TInt& aNewCommand ) const;
    public:
          /**
           * offers the menu pane of the servicetab view connected to this
           * extension.
           *
           * @param aPreviousId Menu command id after which the plugins menu
           *  should come.
           * @param aMenuPane Menu pane which will be filtered.
           * @param aServiceid service id
           */
           void OfferMenuPaneToPlugins
              (TInt aPreviousId, CEikMenuPane& aMenuPane,TUint aServiceId);
           
          /**
           * Handles servicetab extension commands.
           *
           * @param aCommandId Command ID.
           *
           * @return ETrue if command was handled, otherwise EFalse.
           */
          TBool OfferHandleCommandToPlugins(TInt aCommandId);
          
          /**
           * Loads all the plugins
           */
          void LoadPluginL( );
          
          /**
           * Map the plugin command to servicetab command
           */
          void MapCommandL(CMenuExtension& aMenuExtension, TInt32 aPluginId);
          
          /**
           * Add the entry to the map table
           */
          void AddToAIWCommandMap(TInt aOriginalCommand, TInt aNewCommand);
          
          /**
           * Finds the entry from the map table
           */
          TInt GetNewAIWCommand(TInt aOriginalCommand);
         
          /**
           * Resets the AIW Command Map
           */
          void ResetAIWCommandMap();
          
    public: //from interface - MVIMPSTUiAIWControl
          
          /**
           * called by plugins to hide/unhide the AIW menu item
           */
          void SetAIWItemDimmed(TInt aCommand, TBool aHide);
        
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory 
         */    
        CVIMPSTUIMenuExtension( );
        
        /**
         * Performs the 2nd phase of construction.
         */             
        void ConstructL();        
    private:
        /**
         * Internal class used as a wrapper around a plugin
         * and its uid.
         */
        class CPluginInfo : public CBase
            {
            public: // Constructor & Destructor
                CPluginInfo( CMenuExtension* aPlugin, TUid aUid );
                ~CPluginInfo();

            public: // New functions
                CMenuExtension& Plugin();
            private: // Data
                CMenuExtension* iPlugin;
                TUid iPluginUid;
            };
        
        private:
            /**
             *Internal structure used as a item in the AIW command map table
             */
            struct TAIWCommandMapTableItem
                {
                TInt iOriginalCmd;      //is used as the primary key in the table
                TInt iNewCmd;           //Mapped Command
                };

    private: // Data
        
        // New commands pool
        TInt iNewCommands;

        // takes ownership of the plugins
        RPointerArray<CPluginInfo> iPlugins;  
        
         // Array of command id mappings,Owned
        RPointerArray<CCommandInfo> iCommandIdMap; 
        
        // AIW map table - keeps the ownership of the items
        RPointerArray<TAIWCommandMapTableItem> iAIWCommandMap;
        
        //doesnt owns
        CEikMenuPane* iMenuPane;
      
    };

#endif // C_CVIMPSTUIVIEWMANAGER_H

// End of file
