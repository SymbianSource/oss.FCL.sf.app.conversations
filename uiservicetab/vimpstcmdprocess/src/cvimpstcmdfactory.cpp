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
* Description:  implementation for CVIMPSTCmdFactory
*
*/


// INCLUDE FILES
#include "cvimpstcmdfactory.h"
#include "cvimpstprocessarray.h"
#include "cvimpstcmdcca.h"
#include "cvimpstcmdlogin.h"
#include "cvimpstcmdlogout.h"
#include "cvimpstcmdcancellogin.h"

#include "uiservicetabtracer.h"

#include "mvimpstcmdobserver.h"
#include "mvimpstengine.h"
#include "tvimpstconsts.h"
#include "cvimpststoragemanagerfactory.h"
#include "cvimpstcmdaddcontact.h"
#include "cvimpstcmddeletecontact.h"
#include "tvimpstconsts.h"
#include "cvimpstcmdchangeownstatus.h"
#include "cvimpstcmdchangeownmessage.h"
#include "cvimpstcmdsearch.h"
#include "cvimstcmdfriendrequest.h"
#include "cvimpstcmdchangeownavtar.h"
#include "cvimpstcmdcloseconversation.h"
#include "cvimpstcmdaddtopbk.h"
#include "cvimpstcmdprocessselectedcontact.h"
#include "cvimpstcmdblockcontact.h"
#include "cvimpstcmdunblockcontact.h"
#include "cvimpstcmdfetchblockedlist.h"


// ================= MEMBER FUNCTIONS =======================


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::CVIMPSTCmdFactory
// --------------------------------------------------------------------------
//
CVIMPSTCmdFactory::CVIMPSTCmdFactory(MVIMPSTEngine& aEngine) :
iEngine(aEngine)
    {
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFactory::ConstructL()
    {
    TRACER_AUTO;
    TUint32 serviceId = iEngine.ServiceId();
    
    //Make sure we initialise the Storage view for this service
    HBufC* storeName = HBufC::NewLC( KVIMPSTUISPSMaxPropertyLength );
	
	TPtr storeNamePtr( storeName->Des() );  
	
	iEngine.ContactStoreIdL(storeNamePtr);
	
    CVIMPSTStorageManagerFactory::InitialiseViewL(serviceId, *storeName, iEngine.ServiceName());
    
    CleanupStack::PopAndDestroy(); //storeName	
    
    iEngine.IntializeStorageL();
    	
    
    //create the array process
    iArrayProcess =  CVIMPSTProcessArray::NewL( iEngine );

	iEngine.RegisterServiceSessionObserverL(this);
	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdFactory* CVIMPSTCmdFactory::NewL(MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdFactory* self = new (ELeave) CVIMPSTCmdFactory(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::~CVIMPSTCmdFactory
// --------------------------------------------------------------------------
//
CVIMPSTCmdFactory::~CVIMPSTCmdFactory()
    {
    TRACER_AUTO;
    iEngine.UnIntializeStorage();
    iEngine.UnRegisterServiceSessionObserver(this);
    
    iObservers.Reset();
    iObservers.Close();
    
    delete iArrayProcess;
    iArrayProcess = NULL;   

    }


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::CreateCommandForIdL
// --------------------------------------------------------------------------
//
MVIMPSTCmd* CVIMPSTCmdFactory::CreateCommandForIdL(
        TImCommandId aCommandId,TAny* aData /*NULL*/) 
    {
    TRACER_AUTO;
    MVIMPSTCmd* commd = NULL;
	
	//Instantiate appropriate command    
    switch ( aCommandId )
	    {
       	case ELoginService:    
    		{     		
    		commd = CVIMPSTCmdLogin::NewL(aCommandId,*(TUint32*)aData, iEngine);
    		break;	
    		}
    		
	    case ELogoutService:
	    	{	    	
    		commd = CVIMPSTCmdLogout::NewL(aCommandId,*(TUint32*)aData, iEngine);
    		break;		
	    	}
	    case ELaunchCCA:
	    	{
	    	commd = CVIMPSTCmdCCA::NewL(aCommandId,     									
    									(*(TLaunchCCAData*)aData).iIndex,
    									(*(TLaunchCCAData*)aData).iConnection,
    									*iArrayProcess, 
    									iEngine );
	    	break;
		    		
	    	}
	    case EAddContact:
	    	{
	    	commd = CVIMPSTCmdAddContact::NewL(aCommandId,*(TDesC*) aData, iEngine);
	    	break;
		    		
	    	}
	    case EDeleteContact:
	    	{
	    	commd = CVIMPSTCmdDeleteContact::NewL(aCommandId,*(MVPbkContactLink**) aData, iEngine);
	    	break;
		    		
	    	}
	    case EChangeOwnStatus:
	    	{
	    	commd = CVIMPSTCmdChangeOwnStatus::NewL(aCommandId,*(TStatusAndStatusText*) aData, iEngine);
	    	break;
		    		
	    	}
	    case EChangeOwnMessage:
	    	{
	    	commd = CVIMPSTCmdChangeOwnMessage::NewL(aCommandId,*(TStatusAndStatusText*) aData, iEngine);
	    	break;
		    		
	    	}	    	    	
	     case ESearch:
	    	{
	    	commd = CVIMPSTCmdSearch::NewL(aCommandId,*(RArray<TVIMPSTSearchKeyData>*) aData, iEngine);
	    	break;
		    		
	    	}	 
	     case EFriendAccpeted:
	     case EFriendRejected:
	         {
	         commd = CVIMPSTCmdFriendRequest::NewL(aCommandId,*(TDesC*) aData, iEngine);
	         break;
	         }    
	     case EChangeOwnAvtar:
	         {
	         commd = CVIMPSTCmdChangeOwnAvtar::NewL(aCommandId,*(TAvatarData*) aData, iEngine);
	         break;
	         }
	     case ECloseConversation:
	         {
	         commd = CVIMPSTCmdCloseConversation::NewL(aCommandId,*(TDesC*) aData, iEngine);
	         break;
	         }
	         
	     case EAddToPbk:
	         {
	         commd = CVIMPSTCmdAddToPbk::NewL(aCommandId,*(MVIMPSTStorageContact*) aData, iEngine);
	         break;
	         }
	     case ECancelLogin:
	         {
	         commd = CVIMPSTCmdCancelLogin::NewL(aCommandId ,iEngine);
	         break;
	         }
	     case ECmdProcessAcceptedContactNew:    
	     case ECmdProcessSelectedContactNew:
	     case ECmdProcessAcceptedContactExist:
	     case ECmdProcessSelectedContactExist:
	         {
	         commd = CVIMPSTCmdProcessSelectedContact::NewL( aCommandId, *(TVPbkSelectedData*)aData, iEngine );	                 
	         break;
	         }
	     case ECmdBlockContact:
             {
             commd = CVIMPSTCmdBlockContact::NewL(aCommandId,*(TDesC*) aData, *iArrayProcess , iEngine);
             break;
                     
             }
	     case ECmdUnBlockContact:
             {
             commd = CVIMPSTCmdUnBlockContact::NewL(aCommandId,*(TDesC*) aData, iEngine);
             break;
                     
             }
		case EFetchBlockedList:
			{
             commd = CVIMPSTCmdFetchBlockedContactList::NewL(aCommandId, iEngine);
			break;
			}
    	default:
    		{
    		break;	
    		}
	    		
	    }   
	    
    return commd;
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::HandleServiceEventL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFactory::HandleServiceEventL
	    			( TVIMPSTEnums::TVIMPSTRegistrationState aState,TInt aServiceError )
	{
	TRACER_AUTO;
	// need not to store update complete state
	iArrayProcess->SetLoginStateL( aState );
	for( TInt index( 0 ); index < iObservers.Count(); ++index )
		{
		iObservers[index]->HandleCommandEventL(aState,aServiceError);
		}

	}
// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFactory::AddObserverL(MVIMPSTCmdObserver* aObserver)
	{
	if( iObservers.Find( aObserver ) == KErrNotFound )
        {
        User::LeaveIfError( iObservers.Append( aObserver ) );
        }

	}
// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::RemoveObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdFactory::RemoveObserver(MVIMPSTCmdObserver* aObserver)
	{
	
	TInt index( iObservers.Find( aObserver) );
    
    if( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    
	}


// --------------------------------------------------------------------------
// CVIMPSTCmdFactory::GetProcessInterface
// --------------------------------------------------------------------------
//
MVIMPSTProcessArray& CVIMPSTCmdFactory::GetProcessInterface() 
	{
	return *iArrayProcess; 	
	}	
// End of File

