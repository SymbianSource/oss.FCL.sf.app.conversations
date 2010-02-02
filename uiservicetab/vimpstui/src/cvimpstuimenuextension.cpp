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
* Description:  cvimpstuimenuextension.cpp
 *
*/

// INCLUDE FILES

#include <eikenv.h>
#include <coeaui.h>
#include <coemain.h>
#include <barsread.h>
#include <eikmenup.h>
#include    "vimpstdebugprint.h" 


#include "cvimpstuimenuextension.h"
#include "ccommandinfo.h"

#include "servicetabmenucommandinfo.hrh"


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CVIMPSTUIMenuExtension
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension::CVIMPSTUIMenuExtension( )
: iNewCommands( EServiceTabMenuExtensionFirstFreeCommand )
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::CVIMPSTUIMenuExtension");
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::CVIMPSTUIMenuExtension");
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIMenuExtension::ConstructL()
    {   
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::ConstructL");
    LoadPluginL( ); 
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::ConstructL");

    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension* CVIMPSTUIMenuExtension::NewL( )
    { 
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::NewL");
    CVIMPSTUIMenuExtension* self = NewLC(  );
    CleanupStack::Pop(self);
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::NewL");
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension* CVIMPSTUIMenuExtension::NewLC()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::NewL");
    CVIMPSTUIMenuExtension* self =
        new (ELeave) CVIMPSTUIMenuExtension(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::NewL");
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension()
    {   
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension");
    iCommandIdMap.ResetAndDestroy();
    //iCommandIdMap.Close();
    // Reset and destroy the contents of the owned arrays,
    // this will delete the plugins.
    iPlugins.ResetAndDestroy();
    //iPlugins.Close();
    
    iAIWCommandMap.ResetAndDestroy();
    
    // Close the ECOM interface
    REComSession::FinalClose();
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension");
    }
 
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::LoadPluginL
// Loads all plugins
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::LoadPluginL( )
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::LoadPluginL");
    // plugininfo array, Owned
    RImplInfoPtrArray pluginInfo;
    iPlugins.ResetAndDestroy();
    REComSession::ListImplementationsL( KMenuCustomisationInterfaceUid,pluginInfo );
    TInt pluginCount = pluginInfo.Count();
    CHAT_DP( D_CHAT_LIT("CVIMPSTUIMenuExtension::LoadPluginL -->plugincount: %d" ),pluginCount );      

    for ( TInt pluginIndex(0); pluginIndex < pluginCount; ++pluginIndex )
        {
        CHAT_DP( D_CHAT_LIT("Inside for loop" ) );      
        TUid pluginUid = pluginInfo[pluginIndex]->ImplementationUid();
        CHAT_DP( D_CHAT_LIT("CVIMPSTUIMenuExtension::LoadPluginL -->ImplementationUid: %u" ),pluginUid.iUid );
        // Creates the plugin and transfers ownership of the services
        // object to the plugin.
        CMenuExtension* plugin = NULL;
        TRAPD(error, plugin = CMenuExtension::CreateImplementationL( pluginUid ));
        
        if(KErrNone != error)
            {
            // handle the error here.
            CHAT_DP( D_CHAT_LIT("error loading the plugin error: %d" ),error ); 
            }
        else
            {
            CHAT_DP( D_CHAT_LIT("plugin loaded sucessfully" ) ); 
            //if its here its sure that plugin is not null;
            CleanupStack::PushL( plugin);
            
            CHAT_DP( D_CHAT_LIT("before mapcommandl" ) ); 
            //map the plugin commands to servicetab commands,
            //assings the command maintained in the commandpool,
            //for more details see 
            MapCommandL(*plugin,pluginUid.iUid);
            CHAT_DP( D_CHAT_LIT("after mapcommandl" ) ); 
            
            //add the plugin to the array
            CPluginInfo* newPlugin = new ( ELeave ) CVIMPSTUIMenuExtension::CPluginInfo( plugin, pluginUid);    
            CleanupStack::PushL( newPlugin );
            iPlugins.AppendL( newPlugin );
            CleanupStack::Pop( newPlugin );
           
            CleanupStack::Pop( plugin );
           }
        
        }
    pluginInfo.ResetAndDestroy();
    pluginInfo.Close();
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::LoadPluginL");

    }


// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::OfferMenuPaneToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins(TInt aPreviousId, CEikMenuPane& aMenuPane,TUint aServiceId)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins");
    CHAT_DP( D_CHAT_LIT("CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins --> aServiceId : %u" ), aServiceId ); 

    iMenuPane = &aMenuPane;
    // Get the number of  plugins
    const TInt count = iPlugins.Count();
    CHAT_DP( D_CHAT_LIT("CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins --> count : %d" ), count ); 
    // Loop though all the command handlers, If DynInitMenuPaneL leaves for one plugin, a plugin
    // error message will be displayed and the loop will continue with
    // the next command handler. If none of the plugins leave, there will
    // be only one TRAP used.
    for ( TInt index = 0; index < count; ++index )
        {
        CHAT_DP( D_CHAT_LIT("inside for loop" ) ); 
        //trap is required if one plugin leaves then it should continue with other plugins.
        TRAPD(error,iPlugins[index]->Plugin().DynInitMenuPaneL( aPreviousId,
                                                             aMenuPane,aServiceId,*this ));
        if(KErrNone != error)
            {
            //display the appropriate error note for leaving;
            CHAT_DP( D_CHAT_LIT("error by one of plugind dyninitmenupanel" ) ); 
            }
      
        }
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins");

    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::OfferHandleCommandToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
TBool CVIMPSTUIMenuExtension::OfferHandleCommandToPlugins(TInt aCommandId)
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::OfferHandleCommandToPlugins");

     //Get the number of plugins loaded
     const TInt count = iPlugins.Count();
     CHAT_DP( D_CHAT_LIT("CVIMPSTUIMenuExtension::OfferHandleCommandToPlugins -->count : %d" ),count ); 

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
     CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::OfferHandleCommandToPlugins");

     return retval;
    }

// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CPluginInfo::CPluginInfo(
// C++ constructor
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CVIMPSTUIMenuExtension::CPluginInfo::CPluginInfo( 
                                    CMenuExtension* aPlugin,
                                    TUid aPluginUid )
    : iPlugin( aPlugin ),
      iPluginUid( aPluginUid )
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::CPluginInfo::CPluginInfo");
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::CPluginInfo::CPluginInfo");

    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CPluginInfo::~CPluginInfo(
// C++ destructor
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CVIMPSTUIMenuExtension::CPluginInfo::~CPluginInfo() 
    {
     CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::LoadPluginL");
     delete iPlugin;
     CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::LoadPluginL");
    }
    
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CPluginInfo::Plugin
// returns a reference to the plugin
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CMenuExtension& CVIMPSTUIMenuExtension::CPluginInfo::Plugin()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::CPluginInfo::Plugin");
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::CPluginInfo::Plugin");
     return *iPlugin;
    }


// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::MapCommandL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
// 
void CVIMPSTUIMenuExtension::MapCommandL( CMenuExtension& aMenuExtension, TInt32  aPluginId )
    { 
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::MapCommandL");

    TInt res = aMenuExtension.CommandInfoResource();
    if( res != KErrNotFound )
        {
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, res );
        TInt resCount = reader.ReadInt16();
        for( TInt j = 0; j < resCount; j++ )
            {
            CCommandInfo* commandInfo = NULL;
            commandInfo = CCommandInfo::NewLC( reader,
                                                aPluginId,
                                                iNewCommands );
            User::LeaveIfError( iCommandIdMap.Append( commandInfo ) );
            CleanupStack::Pop(); // commandInfo
            }
        CleanupStack::PopAndDestroy(); // reader                        
        aMenuExtension.RegisterCommandMapper( *this );
       
        }
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::MapCommandL");
                            
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::GetNewCommand
// from MComandMapper
// ----------------------------------------------------------------------------
//

TInt CVIMPSTUIMenuExtension::GetOldCommand( TInt32 aPluginId, TInt aNewCommand,
                                    TInt& aOldCommand ) const
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::GetOldCommand");

    TInt mapCount = iCommandIdMap.Count();
    TInt err( KErrNotFound );
    //TODO:: optimize searching.
    for( TInt i = 0; i < mapCount && err; i++ )
        {
        CCommandInfo* info = iCommandIdMap[i];
        if( info->PliginId() == aPluginId && info->NewCommandId() == aNewCommand )
            {
            aOldCommand = info->OldCommandId();
            err = KErrNone;
            break;
            }
        }  
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::GetOldCommand");

    return err;
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::GetNewCommand
// from MComandMapper
// ----------------------------------------------------------------------------
//
    
TInt CVIMPSTUIMenuExtension::GetNewCommand( TInt32 aPluginId, TInt aOldCommand,
                                    TInt& aNewCommand ) const
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTUIMenuExtension::GetNewCommand");

    TInt mapCount = iCommandIdMap.Count();
    TInt err( KErrNotFound );
    //TODO:: optimize searching.
    for( TInt i = 0; i < mapCount && err; i++ )
        {
        CCommandInfo* info = iCommandIdMap[i];
        if( info->PliginId() == aPluginId && info->OldCommandId() == aOldCommand )
            {
            aNewCommand = info->NewCommandId();
            err = KErrNone;
            break;
            }
        }   
    CHAT_DP_FUNC_DONE("CVIMPSTUIMenuExtension::GetNewCommand");

    return err; 
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::AddToAIWCommandMap
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::AddToAIWCommandMap(TInt aOriginalCommand, TInt aNewCommand)
    {
    TAIWCommandMapTableItem *item = new TAIWCommandMapTableItem();
    item->iOriginalCmd = aOriginalCommand;
    item->iNewCmd = aNewCommand;
    iAIWCommandMap.Append(item);
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::GetNewAIWCommand
// ----------------------------------------------------------------------------
//
TInt CVIMPSTUIMenuExtension::GetNewAIWCommand(TInt aOriginalCommand)
    {
    TInt count = iAIWCommandMap.Count();
    for( TInt i=0 ; i<count ; i++ )
        {
        if(iAIWCommandMap[i]->iOriginalCmd == aOriginalCommand)
            {
            return iAIWCommandMap[i]->iNewCmd;
            }
        }
    return KErrNotFound;
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::ResetAIWCommandMap
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::ResetAIWCommandMap()
    {
    iAIWCommandMap.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::SetAIWItemDimmed
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::SetAIWItemDimmed(TInt aCommand, TBool aHide)
    {
    TInt cmd = GetNewAIWCommand(aCommand);
    if(cmd != KErrNotFound)
        iMenuPane->SetItemDimmed(cmd,aHide);
    }

// End of file
