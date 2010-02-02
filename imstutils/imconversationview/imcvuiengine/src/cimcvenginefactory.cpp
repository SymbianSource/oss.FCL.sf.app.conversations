/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  service engine factory implementation
*
*/

// INCLUDE FILES
#include "cimcvenginefactory.h"

#include "mimcvenginefactory.h"
#include "cimcvengine.h"
#include "imcvuiliterals.h"
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spdefinitions.h>
#include <MVPbkContactLink.h>
#include "mimcvenginemessagecreator.h"
#include <cbsfactory.h>
#include "cimcvenginemessagecreator.h"
#include <spnotifychange.h>
#include "mimcvngineservicechangeobserver.h"
#include <apgtask.h>
#include <w32std.h>
#include "imcvuiparams.h"
#include "mimcvenginemessagehandler.h"
#include "mimcvenginechatinterface.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CIMCVEngineFactory::InstanceL
// Create an instance of the ximp manager
// behaving as singleton object already created then return same instance.
// -----------------------------------------------------------------------------
//
EXPORT_C MIMCVEngineFactory* CIMCVEngineFactory::InstanceL()
    {
    CIMCVEngineFactory* singleton;
    singleton = static_cast<CIMCVEngineFactory*> (Dll::Tls());
    if( !singleton )
        {
        singleton = CIMCVEngineFactory::NewLC();
        User::LeaveIfError( Dll::SetTls( static_cast<TAny*> (singleton ) ));
        CleanupStack::Pop(singleton);
        }
    return (MIMCVEngineFactory*)singleton;
    }
    
// -----------------------------------------------------------------------------
// CIMCVEngineFactory::Release
// Release the singelton
// -----------------------------------------------------------------------------
//
EXPORT_C void CIMCVEngineFactory::Release()
    {
    CIMCVEngineFactory* singleton;
    singleton = static_cast<CIMCVEngineFactory*>(Dll::Tls());
    if( singleton )
        {
        delete singleton;
        Dll::SetTls( NULL ) ;
        }
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory::NewLC
// Object creation using two phase construction
// -----------------------------------------------------------------------------
//
CIMCVEngineFactory* CIMCVEngineFactory::NewLC()
    {
    CIMCVEngineFactory* self = new (ELeave) CIMCVEngineFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CIMCVEngineFactory::ConstructL
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::ConstructL()
    {
    iMessageCreator = CIMCVEngineMessageCreator::NewL();
    iBrandingFactory = CBSFactory::NewL( KDefaultBrandIdForBrand, KAppIdForBrand );
    __ASSERT_ALWAYS((iBrandingFactory != NULL),User::Leave(KErrArgument));
    ReadAndCreateAllValidServiceL();
    //register for service table notifications
    RArray<TUint> serviceIds;
    CleanupClosePushL( serviceIds );
    iSpNotifyChange = CSPNotifyChange::NewL( *this );    
    iSpNotifyChange->NotifyChangeL( serviceIds );
    CleanupStack::PopAndDestroy( &serviceIds ) ; 
    }

// ---------------------------------------------------------
// CIMCVEngineFactory::CIMCVEngineFactory
// ---------------------------------------------------------  
CIMCVEngineFactory::CIMCVEngineFactory():
    iActiveChatIndex( KErrNotFound )
	{
		
	}

// ---------------------------------------------------------
// CIMCVEngineFactory::~CIMCVEngineFactory
// ---------------------------------------------------------   
CIMCVEngineFactory::~CIMCVEngineFactory()
    {
    delete iSpNotifyChange;
    //delete all the create engine instances
    //delete iBrandingFactory;
    iServiceEngines.ResetAndDestroy();
    iServiceEngines.Close();
    delete iMessageCreator;
    delete iBrandingFactory;
    }
// ---------------------------------------------------------
// CIMCVEngineFactory::ReadAndCreateAllValidServiceL
// ---------------------------------------------------------   
void CIMCVEngineFactory::ReadAndCreateAllValidServiceL()
    {
    CSPSettings* settings = CSPSettings::NewLC();
    
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    settings->FindServiceIdsL( services );
    const TInt serviceCount = services.Count();
    
    for ( TInt i( 0 ); i < serviceCount; i++ )
        {
        TInt serviceId = services[ i ];
        TUint32 refContactUid = ContactViewUidL(*settings , serviceId );
        if ( TUid::Uid( refContactUid ) == 
             TUid::Uid( 0x20012423 ) 
             || TUid::Uid( refContactUid ) == 
             TUid::Uid( 0x20007B6D )
             || TUid::Uid( refContactUid ) == 
             TUid::Uid( 0x2002B334 ) )
            {
            if( CheckValidIMServiceL(*settings , serviceId  ) )
                {
                CreateServiceEngineL( serviceId );
                }
            }
        }
    CleanupStack::PopAndDestroy( &services );
    CleanupStack::PopAndDestroy( settings );
    }

// ---------------------------------------------------------------------------
// CIMCVEngineFactory::ContactViewUidL()
// ---------------------------------------------------------------------------
//
TUint32 CIMCVEngineFactory::ContactViewUidL(CSPSettings& aSettings, TUint32 aServiceId )
    {
    TInt contactViewUid = KErrNotFound;
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = aSettings.FindPropertyL( aServiceId,
                                EPropertyContactViewPluginId,
                                *property );    
    if ( KErrNone == err )
        {
        property->GetValue( contactViewUid );
        }
    CleanupStack::PopAndDestroy( property );      
    return contactViewUid;
    }    
//-----------------------------------------------------------------------------
// CIMCVEngineFactory::CheckValidIMServiceL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
TBool CIMCVEngineFactory::CheckValidIMServiceL(CSPSettings& aSettings , TInt aServiceId) 
    {   
    //check whether it has IMSubservice    
    TBool ret = ETrue;
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = aSettings.FindPropertyL( aServiceId,
                                ESubPropertyIMEnabled,
                                *property );    
    if ( err )
        {
        ret = EFalse;
        }
    CleanupStack::PopAndDestroy( property );
    return ret;      
    }

// ---------------------------------------------------------------------------
// CIMCVEngineFactory::CreateServiceEngineL()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::CreateServiceEngineL( TInt aServiceId )
    {
    TInt count = iServiceEngines.Count();
    CIMCVEngine* engine = NULL;
    for(int i=0 ; i<count ; i++)
        {
        if(iServiceEngines[i]->GetServiceId() == aServiceId)
            {
            engine = iServiceEngines[i];
            break;
            }
        }
    if( !engine )
        {
        engine = CIMCVEngine::NewL( aServiceId , *iMessageCreator, *iBrandingFactory );
		CleanupStack::PushL(engine);
        iServiceEngines.AppendL(engine);
		CleanupStack::Pop(engine);
        }
    }           
// ---------------------------------------------------------------------------
// CIMCVEngineFactory::GetServiceEngine()
// ---------------------------------------------------------------------------
//
CIMCVEngine& CIMCVEngineFactory::GetServiceEngineL( TInt aServiceId )
    {
    TInt count = iServiceEngines.Count();
    CIMCVEngine* engine = NULL;
    for(int i=0 ; i<count ; i++)
        {
        if(iServiceEngines[i]->GetServiceId() == aServiceId)
            {
            engine = iServiceEngines[i];
            break;
            }
        }
    if( !engine )
        {
        engine = CIMCVEngine::NewL( aServiceId , *iMessageCreator, *iBrandingFactory );
		CleanupStack::PushL(engine);
        iServiceEngines.AppendL(engine);
        CleanupStack::Pop(engine);
        engine->OpenChats().RegisterObserver( this );
        engine->MessageHandler().SetObserver( this );
        engine->ChatInterface().RegisterChatListObserver( this );
	    }
    return *engine;
    }			
// ---------------------------------------------------------------------------
// CIMCVEngineFactory::ReleaseServiceEngine()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::ReleaseServiceEngine( TInt aServiceId )
    {
    TInt count = iServiceEngines.Count();
    for(int i=0 ; i<count ; i++)
        {
        if(iServiceEngines[i]->GetServiceId() == aServiceId )
            {
            CIMCVEngine* engine = iServiceEngines[i];
            iServiceEngines.Remove( i );
            delete engine;
            engine = NULL;
            iServiceEngines.Compress();
            break;
            }
        }
    }    
// -----------------------------------------------------------------------------
// CIMCVEngineFactory: GetDisplayNameL
// -----------------------------------------------------------------------------
//
TPtrC CIMCVEngineFactory::GetDisplayNameL(TInt aIndex) const
    {
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        if( aIndex < openChatCount )
            {
            return openChat.GetDisplayNameL( aIndex );
            }
        else
            {
            aIndex = aIndex - openChatCount;
            }
        }
    return KNullDesC();
    }
    

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: GetTargetIdL
// -----------------------------------------------------------------------------
//
const TDesC& CIMCVEngineFactory::GetTargetIdL(TInt aIndex) const
    {
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        if( aIndex < openChatCount )
            {
            return openChat.GetTargetIdL( aIndex );
            }
        else
            {
            aIndex = aIndex - openChatCount;
            }
        }
    return KNullDesC;   
    }



// -----------------------------------------------------------------------------
// CIMCVEngineFactory: FindIndexL
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineFactory::FindIndexL(const TDesC& aItem )
    {
    TInt index = KErrNotFound;
    TInt validIndex = 0;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        index = openChat.FindIndexL( aItem ) ;
        if( index != KErrNotFound )
            {
            validIndex = validIndex + index;
            break;
            }
        else
            {
            validIndex = openChatCount;
            }
        }
    return validIndex; 
    }
    
// -----------------------------------------------------------------------------
// CIMCVEngineFactory: Count
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineFactory::Count() const
    {
    TInt chatCount = 0;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        chatCount = chatCount + openChat.Count();
        }
    return chatCount;
    }


// -----------------------------------------------------------------------------
// CIMCVEngineFactory: SetActiveItemL
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::SetActiveItemL( const TInt aIndex )
    {
    iActiveChatIndex = aIndex;
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: GetActiveItem
// -----------------------------------------------------------------------------
//
const TDesC& CIMCVEngineFactory::GetActiveItemL() const
    {
    TInt index = iActiveChatIndex ;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        if( index < openChatCount )
            {
            return openChat.GetTargetIdL( index ); 
            }
        else
            {
            index = index - openChatCount;
            }
        }
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: GetServiceEngines
// -----------------------------------------------------------------------------
//
RPointerArray<CIMCVEngine>&  CIMCVEngineFactory::GetServiceEngines()
    {
    return iServiceEngines;
    }
// -----------------------------------------------------------------------------
// CIMCVEngineFactory: GetActiveServiceId
// -----------------------------------------------------------------------------
//
TInt CIMCVEngineFactory::GetActiveServiceId() 
    {
    TInt index = iActiveChatIndex;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        if( index < openChatCount )
            {
            return iServiceEngines[i]->GetServiceId();
            }
        else
            {
            index = index - openChatCount;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: IsMsgPendingL
// -----------------------------------------------------------------------------
//
TBool CIMCVEngineFactory::IsMsgPendingL( const TInt aIndex ) const
    {
    TInt index = aIndex;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        MIMCVEngineOpenChats& openChat = iServiceEngines[i]->OpenChats();
        TInt openChatCount = openChat.Count();
        if( index < openChatCount )
            {
            openChat.IsMsgPendingL( index );
            }
        else
            {
            index = index - openChatCount;
            }
        }
    return EFalse;          
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: ResetL
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::ResetL()
    {
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        iServiceEngines[i]->OpenChats().ResetL();
        }
    iActiveServiceId = KErrNotFound;
    }   
// -----------------------------------------------------------------------------
// CIMCVEngineFactory: RegisterOpenChatObserver
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::RegisterOpenChatObserver(MIMCVEngineOpenChatsObserver* aChatObserver)
    {
    iChatObserver = aChatObserver;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        iServiceEngines[i]->OpenChats().RegisterObserver( this );
        }
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: UnRegisterOpenChatObserver
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::UnRegisterOpenChatObserver()
    {
    iChatObserver = NULL;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        iServiceEngines[i]->OpenChats().UnRegisterObserver();
        }
    }


// ---------------------------------------------------------    
// CIMCVEngineFactory::HandleChangeL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVEngineFactory::HandleChangeL(TInt aServiceId, TInt aIndex, TOpenChatsEventType aChangeType) 
    {
    if( iChatObserver )
        {
        TInt validIndex = KErrNotFound;
        MIMCVEngineOpenChats& openChat = GetServiceEngineL( aServiceId ).OpenChats();
        TInt chatCount = openChat.Count();
        if( aIndex < chatCount && aIndex>=0 )
            {
            validIndex = FindIndexL( openChat.GetTargetIdL( aIndex ) );
            }
        if(validIndex >=0 )
            {
            iChatObserver->HandleChangeL( aServiceId, validIndex, aChangeType );
            }
        }
    }       
    
// ---------------------------------------------------------
// CIMCVEngineFactory::SetStatusMsgL()
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVEngineFactory::SetStatusMsgL(TInt aServiceId, TInt aIndex,const TDesC & aMsg)
    {
    if( iChatObserver )
        {
        TInt validIndex = KErrNotFound;
        MIMCVEngineOpenChats& openChat = GetServiceEngineL( aServiceId ).OpenChats();
        TInt chatCount = openChat.Count();
        if( aIndex < chatCount && aIndex>=0 )
            {
            validIndex = FindIndexL( openChat.GetTargetIdL( aIndex ) );
            }
        if( validIndex >= 0 )
            {
            iChatObserver->SetStatusMsgL( aServiceId, validIndex, aMsg );
            }
        }
    }
// ---------------------------------------------------------------------------
// CIMCVEngineFactory::RegisterServiceObserver()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::RegisterServiceObserver(MIMCVEngineServiceChangeObserver* aServiceObserver)  
    {
    iServiceChangeObserver = aServiceObserver;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        iServiceEngines[i]->ChatInterface().RegisterChatListObserver( this );
        iServiceEngines[i]->MessageHandler().SetObserver( this );
        }
    }
// ---------------------------------------------------------------------------
// CIMCVEngineFactory::UnRegisterServiceObserver()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::UnRegisterServiceObserver()
    {
    iServiceChangeObserver = NULL;
    TInt count = iServiceEngines.Count();
    for( TInt i=0; i<count; i++ )
        {
        iServiceEngines[i]->ChatInterface().UnregisterChatListObserver( this );
        iServiceEngines[i]->MessageHandler().RemoveObserver();
        }
    }
       
//////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
// CIMCVEngineFactory::HandleNotifyChange()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::HandleNotifyChange(
    TServiceId aServiceId )
    {
    TRAP_IGNORE( DoHandleServiceChangeL(aServiceId ) );
    }

// ---------------------------------------------------------------------------
// CIMCVEngineFactory::DoHandleServiceChangeL()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::DoHandleServiceChangeL( TInt aServiceId )
    {
    // check service Id exist intable
    CSPSettings* settings = CSPSettings::NewLC();
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    settings->FindServiceIdsL( services );
    const TInt serviceCount = services.Count();
    TBool serviceExist = EFalse;
    for ( TInt i( 0 ); i < serviceCount; i++ )
        {
        TInt serviceId = services[ i ];
        TUint32 refContactUid = ContactViewUidL(*settings , serviceId );
        if ( TUid::Uid( refContactUid ) == 
        TUid::Uid( 0x20012423 ) 
        || TUid::Uid( refContactUid ) == 
        TUid::Uid( 0x20007B6D )
        || TUid::Uid( refContactUid ) == 
            TUid::Uid( 0x2002B334 ) )
            {
            if( aServiceId == serviceId   )
                {
                serviceExist = ETrue;
                }
            }
        }
    CleanupStack::PopAndDestroy( &services );
   
    // if service does not exist delete the engine if created
    if( !serviceExist )
        {
        // delete if already created
        TInt count = iServiceEngines.Count();
        for( TInt i=0; i<count; i++ )
            {
            if( iServiceEngines[i]->GetServiceId() == aServiceId )
                {
                if( iServiceChangeObserver )
                    {
                    // this will delete the new branding icosn for smile etc.
                    iServiceChangeObserver->HandleServiceDeletedL( aServiceId );
                    }
                CIMCVEngine* engine = iServiceEngines[i];
                engine->OpenChats().UnRegisterObserver();
                engine->MessageHandler().RemoveObserver();
                engine->ChatInterface().UnregisterChatListObserver( this );
                
                iServiceEngines.Remove(i);
                delete engine;
                engine = NULL;
                iServiceEngines.Compress();
                break;
                }
            }
        if( !iServiceEngines.Count() )
            {
            ShutDownConversationviewL();
            }
        }
    else
        {
        // service exist check whether we have service engine or not 
        // create if we dont have
        TBool newService = ETrue;
        TInt count = iServiceEngines.Count();
        for( TInt i=0; i<count; i++ )
            {
            if( iServiceEngines[i]->GetServiceId() == aServiceId )
                {
                newService = EFalse;
                break;
                }
            }
        if ( newService && CheckValidIMServiceL( *settings, aServiceId ) )
            {
            // create if NULL and return
            GetServiceEngineL( aServiceId );
            if( iServiceChangeObserver )
                {
                // this will create the new branding icosn for smile etc.
                iServiceChangeObserver->HandleNewServiceL( aServiceId );
                }
            }
        }
    CleanupStack::PopAndDestroy( settings );
    }

// ---------------------------------------------------------------------------
// CIMCVEngineFactory::HandleError()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::HandleError( TInt /*aError*/ )
    {
    }  
 // ---------------------------------------------------------------------------
// CIMCVEngineFactory::ShutDownConversationviewL()
// ---------------------------------------------------------------------------
//
void CIMCVEngineFactory::ShutDownConversationviewL()
    {
    // Kills cv if running
    RWsSession ws;
    User::LeaveIfError( ws.Connect() );
    CleanupClosePushL( ws );
    TApaTaskList list( ws );
    TApaTask task( list.FindApp( KConversationViewAppUid ) );
    if ( task.Exists() )
        {
        task.EndTask();
        }
    CleanupStack::PopAndDestroy( &ws ); // ws
    }
// -----------------------------------------------------------------------------
// CIMCVEngineFactory::HandlePresenceChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId )
    {
    if( iServiceChangeObserver )
        {
        // this will create the new branding icosn for smile etc.
        iServiceChangeObserver->HandlePresenceChangedL( aServiceId , aBuddyId );
        }
    }
// ---------------------------------------------------------
// CIMCVEngineFactory::HandleOperationCompletdL
// Handles operation completion
// ---------------------------------------------------------
//
void CIMCVEngineFactory::HandleOperationCompletdL(TInt aServiceId , MIMCVEngineMessage::TMessagerType aType , TInt aResult )
    {
    if( iServiceChangeObserver )
        {
        // this will create the new branding icosn for smile etc.
        iServiceChangeObserver->HandleOperationCompletdL( aServiceId , aType, aResult );
        }
    }


// -----------------------------------------------------------------------------
// CIMCVEngineFactory::HandleConnectionTerminatedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::HandleConnectionEventL( TInt aServiceId, TIMCVConnectionState aState, TInt aReason )
    {
    if( iServiceChangeObserver )
          {
          // this will create the new branding icosn for smile etc.
          iServiceChangeObserver->HandleConnectionEventL( aServiceId , aState, aReason );
          }
    }

// -----------------------------------------------------------------------------
// CIMCVEngineFactory: HandleChatListEvent
// -----------------------------------------------------------------------------
//
void CIMCVEngineFactory::HandleChatListEvent(TInt aServiceId, TChatListEventType aEvent, 
                                    MIMCVEngineMessageReadInterface* aContainerInfo
                                     ) 
    {
    if( iServiceChangeObserver )
       {
       // this will create the new branding icosn for smile etc.
       iServiceChangeObserver->HandleChatListEvent( aServiceId , aEvent, aContainerInfo );
       }
    }
//  End of File
