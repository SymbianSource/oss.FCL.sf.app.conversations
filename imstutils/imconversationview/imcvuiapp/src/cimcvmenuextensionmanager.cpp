/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Class that provides centralized access for UI classes 
*                to logic handling
*
*/

// INCLUDE FILES

#include <eikenv.h>
#include <coeaui.h>
#include <coemain.h>
#include <barsread.h>
#include <eikmenup.h> 


#include "cimcvmenuextensionmanager.h"
#include "cimcvcommandinfo.h"

#include "cvmenucommandinfo.hrh"
#include "vimpstcustomcleanupapi.h"


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CIMCVMenuExtensionManager::CIMCVMenuExtensionManager
// ---------------------------------------------------------------------------
// 
CIMCVMenuExtensionManager::CIMCVMenuExtensionManager( )
: iNewCommands( ECVMenuExtensionFirstFreeCommand )
    {

    }

// ---------------------------------------------------------------------------
// CIMCVMenuExtensionManager::ConstructL
// ---------------------------------------------------------------------------
// 
void CIMCVMenuExtensionManager::ConstructL()
    {   
    LoadPluginL( ); 
    }

// ---------------------------------------------------------------------------
// CIMCVMenuExtensionManager::NewL
// ---------------------------------------------------------------------------
// 
CIMCVMenuExtensionManager* CIMCVMenuExtensionManager::NewL( )
    { 
    CIMCVMenuExtensionManager* self = NewLC(  );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CIMCVMenuExtensionManager::NewLC
// ---------------------------------------------------------------------------
// 
CIMCVMenuExtensionManager* CIMCVMenuExtensionManager::NewLC()
    {
    CIMCVMenuExtensionManager* self =
        new (ELeave) CIMCVMenuExtensionManager(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CIMCVMenuExtensionManager::~CIMCVMenuExtensionManager
// ---------------------------------------------------------------------------
// 
CIMCVMenuExtensionManager::~CIMCVMenuExtensionManager()
    {   
    iCommandIdMap.ResetAndDestroy();
    iCommandIdMap.Close();
    // Reset and destroy the contents of the owned arrays,
    // this will delete the plugins.
    iPlugins.ResetAndDestroy();
    iPlugins.Close();
    
    // Close the ECOM interface
    REComSession::FinalClose();
    }
 
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::LoadPluginL
// Loads all plugins
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CIMCVMenuExtensionManager::LoadPluginL( )
    {
    // plugininfo array, Owned
    RImplInfoPtrArray pluginInfo;
    CustomCleanupResetAndDestroyPushL(pluginInfo);
    iPlugins.ResetAndDestroy();
    REComSession::ListImplementationsL( KMenuCustomisationInterfaceUid,pluginInfo );
    TInt pluginCount = pluginInfo.Count();
    
    for ( TInt pluginIndex(0); pluginIndex < pluginCount; ++pluginIndex )
        {
        TUid pluginUid = pluginInfo[pluginIndex]->ImplementationUid();
        // Creates the plugin and transfers ownership of the services
        // object to the plugin.
        CIMCVMenuExtension* plugin = NULL;
        TRAPD(error, plugin = CIMCVMenuExtension::CreateImplementationL( pluginUid ));
        
        if(KErrNone != error)
            {
            // handle the error here.
            }
        else
            {
            //if its here its sure that plugin is not null;
            CleanupStack::PushL( plugin);
            
            //map the plugin commands to CV commands,
            //assings the command maintained in the commandpool,
            //for more details see 
            MapCommandL(*plugin,pluginUid.iUid);
            
            //add the plugin to the array
            CPluginInfo* newPlugin = new ( ELeave ) CIMCVMenuExtensionManager::CPluginInfo( plugin, pluginUid);    
            CleanupStack::PushL( newPlugin );
            iPlugins.AppendL( newPlugin );
            CleanupStack::Pop( newPlugin );
           
            CleanupStack::Pop( plugin );
           }
        
        }

     CleanupStack::PopAndDestroy(); 
     
    }


// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::OfferMenuPaneToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
void CIMCVMenuExtensionManager::OfferMenuPaneToPlugins(TInt aPreviousId, CEikMenuPane& aMenuPane,TUint aServiceId)
    {
    // Get the number of  plugins
    const TInt count = iPlugins.Count();
    // Loop though all the command handlers, If DynInitMenuPaneL leaves for one plugin, a plugin
    // error message will be displayed and the loop will continue with
    // the next command handler. If none of the plugins leave, there will
    // be only one TRAP used.
    for ( TInt index = 0; index < count; ++index )
        {
        //trap is required if one plugin leaves then it should continue with other plugins.
        TRAPD(error,iPlugins[index]->Plugin().DynInitMenuPaneL( aPreviousId,
                                                             aMenuPane,aServiceId ));
        if(KErrNone != error)
            {
            //display the appropriate error note for leaving;
            }
      
        }
    
    }
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::OfferHandleCommandToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
TBool CIMCVMenuExtensionManager::OfferHandleCommandToPlugins(TInt aCommandId)
    {
     //Get the number of plugins loaded
     const TInt count = iPlugins.Count();
     
     TBool retval = EFalse;
     // Loop though all the command handlers, 
     // If HandleCommandL leaves for one plugin, a plugin
     // error message will be displayed and the loop will continue with
     // the next plugin. If none of the plugins leave, there will
     // be only one TRAP used.
     for ( TInt index = 0; index < count; ++index )
         {
         TRAPD( error,retval = iPlugins[index]->Plugin().HandleCommandL( aCommandId ));
             
         if ( KErrNone != error )
             {
             // Report a problem with plugin.
             }
         }
     return retval;
    }

// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::CPluginInfo::CPluginInfo(
// C++ constructor
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CIMCVMenuExtensionManager::CPluginInfo::CPluginInfo( 
        CIMCVMenuExtension* aPlugin,
        TUid aPluginUid )
: iPlugin( aPlugin ),
iPluginUid( aPluginUid )
            {

            }
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::CPluginInfo::~CPluginInfo(
// C++ destructor
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//

CIMCVMenuExtensionManager::CPluginInfo::~CPluginInfo() 
    {
    delete iPlugin;
    }
    
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::CPluginInfo::Plugin
// returns a reference to the plugin
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CIMCVMenuExtension& CIMCVMenuExtensionManager::CPluginInfo::Plugin()
    {
     return *iPlugin;
    }


// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::MapCommandL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
// 
void CIMCVMenuExtensionManager::MapCommandL( CIMCVMenuExtension& aMenuExtension, TInt32  aPluginId )
    { 
    TInt res = aMenuExtension.CommandInfoResource();
    if( res != KErrNotFound )
        {
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, res );
        TInt resCount = reader.ReadInt16();
        for( TInt j = 0; j < resCount; j++ )
            {
            CIMCVCommandInfo* commandInfo = NULL;
            commandInfo = CIMCVCommandInfo::NewLC( reader,
                                                aPluginId,
                                                iNewCommands );
            User::LeaveIfError( iCommandIdMap.Append( commandInfo ) );
            CleanupStack::Pop(); // commandInfo
            }
        CleanupStack::PopAndDestroy(); // reader                        
        aMenuExtension.RegisterCommandMapper( *this );
       
        }
    }
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::GetNewCommand
// from MComandMapper
// ----------------------------------------------------------------------------
//

TInt CIMCVMenuExtensionManager::GetOldCommand( TInt32 aPluginId, TInt aNewCommand,
                                    TInt& aOldCommand ) const
    {
    TInt mapCount = iCommandIdMap.Count();
    TInt err( KErrNotFound );
    for( TInt i = 0; i < mapCount && err; i++ )
        {
        CIMCVCommandInfo* info = iCommandIdMap[i];
        if( info->PluginId() == aPluginId && info->NewCommandId() == aNewCommand )
            {
            aOldCommand = info->OldCommandId();
            err = KErrNone;
            break;
            }
        }  
        return err;
    }
// ----------------------------------------------------------------------------
// CIMCVMenuExtensionManager::GetNewCommand
// from MComandMapper
// ----------------------------------------------------------------------------
//
    
TInt CIMCVMenuExtensionManager::GetNewCommand( TInt32 aPluginId, TInt aOldCommand,
                                    TInt& aNewCommand ) const
    {
    TInt mapCount = iCommandIdMap.Count();
    TInt err( KErrNotFound );
    
    for( TInt i = 0; i < mapCount && err; i++ )
        {
        CIMCVCommandInfo* info = iCommandIdMap[i];
        if( info->PluginId() == aPluginId && info->OldCommandId() == aOldCommand )
            {
            aNewCommand = info->NewCommandId();
            err = KErrNone;
            break;
            }
        }   
    
    return err; 
    }


// End of file
