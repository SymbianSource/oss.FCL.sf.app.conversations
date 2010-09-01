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
* Description:  Class that provides extensions for options menu
*
*/

#ifndef     CIMCVMENUEXTENSIONMANAGER_H
#define     CIMCVMENUEXTENSIONMANAGER_H

// INCLUDES
#include <e32base.h>
#include <e32const.h>
#include <mimcvcommandmapper.h>
#include <cimcvmenuextension.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CEikMenuPane;
class CIMCVCommandInfo;
class CIMCVMenuExtension;


// CLASS DECLARATION

/**
 *  
 *  This class provides extensions to options menu.
 *  
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CIMCVMenuExtensionManager ) : public CBase,
                                              public MIMCVCommandMapper
    {
    public: // Constructor and destructor

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CIMCVMenuExtensionManager* NewL(  );

        /**
         * Two-phased constructor.
         *
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory
         */
        static CIMCVMenuExtensionManager* NewLC();

        /**
         * Standard C++ destructor
         */    
        ~CIMCVMenuExtensionManager();
    public: // from MCommandMapper
        /**
         *  Gets old original command ID which corresponds
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
          *  Gets new mapped command ID which corresponds
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
           * offers the menu pane of the ConversationView connected to this
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
           * Handles ConversationView extension commands.
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
           * Map the plugin command to Conversation view command
           */
          void MapCommandL(CIMCVMenuExtension& aMenuExtension, TInt32 aPluginId);
        
    private: // Implementation

        /**
         * Standard C++ constructor
         * @param aServiceName, service name.
         * @param aExtensionFactory, extension factory 
         */    
        CIMCVMenuExtensionManager( );
        
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
                CPluginInfo( CIMCVMenuExtension* aPlugin, TUid aUid );
                ~CPluginInfo();

            public: // New functions
                CIMCVMenuExtension& Plugin();
            private: // Data
                CIMCVMenuExtension* iPlugin;
                TUid iPluginUid;
            };

    private: // Data
        
        // New commands pool
        TInt iNewCommands;

        // takes ownership of the plugins
        RPointerArray<CPluginInfo> iPlugins;  
        
         // Array of command id mappings,Owned
        RPointerArray<CIMCVCommandInfo> iCommandIdMap; 
      
    };

#endif // C_CVIMPSTUIVIEWMANAGER_H

// End of file
