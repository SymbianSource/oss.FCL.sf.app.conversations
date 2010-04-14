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
#include "uiservicetabtracer.h"

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
	TRACER_AUTO;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::ConstructL
// ---------------------------------------------------------------------------
// 
void CVIMPSTUIMenuExtension::ConstructL()
    {   
	TRACER_AUTO;
    LoadPluginL( ); 
    

    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::NewL
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension* CVIMPSTUIMenuExtension::NewL( )
    { 
	TRACER_AUTO;
    CVIMPSTUIMenuExtension* self = NewLC(  );
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::NewLC
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension* CVIMPSTUIMenuExtension::NewLC()
    {
	TRACER_AUTO;
    CVIMPSTUIMenuExtension* self =
        new (ELeave) CVIMPSTUIMenuExtension(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension
// ---------------------------------------------------------------------------
// 
CVIMPSTUIMenuExtension::~CVIMPSTUIMenuExtension()
    {   
	TRACER_AUTO;
    iCommandIdMap.ResetAndDestroy();
    //iCommandIdMap.Close();
    // Reset and destroy the contents of the owned arrays,
    // this will delete the plugins.
    iPlugins.ResetAndDestroy();
    //iPlugins.Close();
    
    iAIWCommandMap.ResetAndDestroy();
    
    // Close the ECOM interface
    REComSession::FinalClose();
    
    }
 
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::LoadPluginL
// Loads all plugins
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::LoadPluginL( )
    {
	TRACER_AUTO;
    // plugininfo array, Owned
    RImplInfoPtrArray pluginInfo;
    iPlugins.ResetAndDestroy();
    REComSession::ListImplementationsL( KMenuCustomisationInterfaceUid,pluginInfo );
    TInt pluginCount = pluginInfo.Count();
    TRACE("plugincount: %d" ,pluginCount );      

    for ( TInt pluginIndex(0); pluginIndex < pluginCount; ++pluginIndex )
        {
        TRACE("Inside for loop"  );      
        TUid pluginUid = pluginInfo[pluginIndex]->ImplementationUid();
        TRACE("ImplementationUid: %u" ,pluginUid.iUid );
        // Creates the plugin and transfers ownership of the services
        // object to the plugin.
        CMenuExtension* plugin = NULL;
        TRAPD(error, plugin = CMenuExtension::CreateImplementationL( pluginUid ));
        
        if(KErrNone != error)
            {
            // handle the error here.
        TRACE("error loading the plugin error: %d" ,error ); 
            }
        else
            {
        TRACE("plugin loaded sucessfully"  ); 
            //if its here its sure that plugin is not null;
            CleanupStack::PushL( plugin);
            
            TRACE("before mapcommandl"  ); 
            //map the plugin commands to servicetab commands,
            //assings the command maintained in the commandpool,
            //for more details see 
            MapCommandL(*plugin,pluginUid.iUid);
            TRACE("after mapcommandl"  ); 
            
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
    

    }


// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::OfferMenuPaneToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::OfferMenuPaneToPlugins(TInt aPreviousId, CEikMenuPane& aMenuPane,TUint aServiceId)
    {
	TRACER_AUTO;
	TRACE(" aServiceId : %u" , aServiceId ); 

    iMenuPane = &aMenuPane;
    // Get the number of  plugins
    const TInt count = iPlugins.Count();
    TRACE(" count : %d" , count ); 
    // Loop though all the command handlers, If DynInitMenuPaneL leaves for one plugin, a plugin
    // error message will be displayed and the loop will continue with
    // the next command handler. If none of the plugins leave, there will
    // be only one TRAP used.
    for ( TInt index = 0; index < count; ++index )
        {
    TRACE("inside for loop"  ); 
        //trap is required if one plugin leaves then it should continue with other plugins.
        TRAPD(error,iPlugins[index]->Plugin().DynInitMenuPaneL( aPreviousId,
                                                             aMenuPane,aServiceId,*this ));
        if(KErrNone != error)
            {
            //display the appropriate error note for leaving;
        TRACE("error by one of plugind dyninitmenupanel"  ); 
            }
      
        }

    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::OfferHandleCommandToPluginsL
// Generates the list of active plugins
// ----------------------------------------------------------------------------
//
TBool CVIMPSTUIMenuExtension::OfferHandleCommandToPlugins(TInt aCommandId)
    {
	TRACER_AUTO;

     //Get the number of plugins loaded
     const TInt count = iPlugins.Count();
     TRACE("count : %d" ,count ); 

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
	TRACER_AUTO;

    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CPluginInfo::~CPluginInfo(
// C++ destructor
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CVIMPSTUIMenuExtension::CPluginInfo::~CPluginInfo() 
    {
	TRACER_AUTO;
     delete iPlugin;
     
    }
    
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::CPluginInfo::Plugin
// returns a reference to the plugin
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CMenuExtension& CVIMPSTUIMenuExtension::CPluginInfo::Plugin()
    {
	TRACER_AUTO;
     return *iPlugin;
    }


// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::MapCommandL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
// 
void CVIMPSTUIMenuExtension::MapCommandL( CMenuExtension& aMenuExtension, TInt32  aPluginId )
    { 
	TRACER_AUTO;

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
   
                            
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::GetNewCommand
// from MComandMapper
// ----------------------------------------------------------------------------
//

TInt CVIMPSTUIMenuExtension::GetOldCommand( TInt32 aPluginId, TInt aNewCommand,
                                    TInt& aOldCommand ) const
    {
	TRACER_AUTO;

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
	TRACER_AUTO;

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
   

    return err; 
    }
// ----------------------------------------------------------------------------
// CVIMPSTUIMenuExtension::AddToAIWCommandMap
// ----------------------------------------------------------------------------
//
void CVIMPSTUIMenuExtension::AddToAIWCommandMap(TInt aOriginalCommand, TInt aNewCommand)
    {
	TRACER_AUTO;
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
	TRACER_AUTO;
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
	TRACER_AUTO;
    TInt cmd = GetNewAIWCommand(aCommand);
    if(cmd != KErrNotFound)
        iMenuPane->SetItemDimmed(cmd,aHide);
    }

// End of file
