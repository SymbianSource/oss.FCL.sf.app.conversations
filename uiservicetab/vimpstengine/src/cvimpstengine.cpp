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
* Description:  Class that capsulates single service data members
*
*/


//  INCLUDES
#include "cvimpstengine.h"

#include "cvimpstengineservicetablefetcher.h"
#include <e32std.h>
#include <barsc.h>
#include <gulutil.h>
#include <coemain.h>
#include <e32base.h>

#include "tvimpstconsts.h"
#include "cvimpstenginevoipsubservice.h"
#include "cvimpstengineimsubservice.h"
#include "cvimpstenginepresencesubservice.h"
#include "cvimpstenginecchhandler.h"
//storage
#include "cvimpststoragemanagerfactory.h"
#include "cvimpstenginesessioncntxtobserver.h"
#include "mvimpststorageserviceview.h"
#include "mvimpststoragevpbkstorehandler.h"

#include "tvimpstenums.h" 

#include "mvimpstengineextentionfeatures.h"
#include "mvimpstengineservicestateeventobserver.h"

//extention features
#include "cvimpstenginesearchmgrextention.h"
#include "cvimpstenginecontactmgmtextention.h"
#include "cvimpstblockedlistmanager.h"

#include "mvimpststoragecontactlist.h"
#include <cvimpstsettingsstore.h>

#include "uiservicetabtracer.h"

// Constants
_LIT( KListNameAllBuddy ,"buddylist" );

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTEngine::NewL
// 
// ---------------------------------------------------------
CVIMPSTEngine* CVIMPSTEngine::NewL(
								TUint32 aServiceId,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher )
    {
	TRACER_AUTO; 
	TRACE("ServiceId: %d", aServiceId);
    
    
    CVIMPSTEngine* self = CVIMPSTEngine::NewLC(	aServiceId,
    											aTableFetcher );
    CleanupStack::Pop( self );
 
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTEngine::NewLC
// 
// ---------------------------------------------------------
    
CVIMPSTEngine* CVIMPSTEngine::NewLC(
								TUint32 aServiceId,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher )
	{  
	TRACER_AUTO;
    TRACE( "ServiceId: %d", aServiceId );
	
    CVIMPSTEngine* self = new (ELeave) CVIMPSTEngine(
    												aServiceId, 
    												aTableFetcher );
    CleanupStack::PushL( self );
    self->ConstructL( aServiceId );    
  
    return self;
	}

// ---------------------------------------------------------
// CVIMPSTEngine::~CVIMPSTEngine
// 
// ---------------------------------------------------------

CVIMPSTEngine::~CVIMPSTEngine()
	{
	TRACER_AUTO;
	
	iObserverArray.Reset();
    iObserverArray.Close();
    
	iSubService.ResetAndDestroy();	
	iSubService.Close();
	// Delete this after sub services, those might use it in destructor!
	delete iSessionCntxtObserver;
    delete iServiceName;
	
	iExtentionFeatures.ResetAndDestroy();
	iExtentionFeatures.Close();
	
	delete iCchHandler;
    iCchHandler = NULL;       

 
 	}


// ---------------------------------------------------------
// CVIMPSTEngine::ConstructL
// 
// ---------------------------------------------------------

void CVIMPSTEngine::ConstructL( TUint32 aServiceId )
	{
	TRACER_AUTO;
    TRACE( "ServiceId: %d", aServiceId );

	iServiceName = HBufC::NewL( KVIMPSTUISPSMaxPropertyLength );
	
	// construct the service state by passing the array of subservices.
	iCchHandler = CVIMPSTEngineCchHandler::NewL(iServiceId,*this);
	
	//construct eventobserver only if there are any adaptations.
	TInt ximpAdapterUid = iTableFetcher.XimpAdapterUidL(aServiceId);
	
	
	TBool requireXimp = EFalse;		
	TBool presenceSupported = IsSubServiceSupportedInternal(TVIMPSTEnums::EPresence);
	TBool imSupported = IsSubServiceSupportedInternal(TVIMPSTEnums::EIM);
	
	requireXimp |= presenceSupported;
    requireXimp |= imSupported;

    TPtr serviceIdPtr(iServiceName->Des());
    iTableFetcher.GetServiceNameL(aServiceId, serviceIdPtr);

	//if either of Presence/IM Subservice supported then we need
	//to bind to XIMP context. 
	if ( KErrNotFound != ximpAdapterUid && requireXimp )
	    {
        TRAPD( err, (iSessionCntxtObserver = CVIMPSTEngineSessionCntxtObserver::NewL(aServiceId)));
        TRACE( "1st Creating session context observer: %d", err );	
        if (KErrNotFound == err)
            {
						TRACE( "Ximp impl not found. Calling Logout" );            	
            iUnInstall = ETrue;
            LogoutL();
            TRACE( "Logout Called on account of uninstall" );
            return;
            }
	    }
	
	// iterate the service array
    for ( TInt index = TVIMPSTEnums::EVoip; index < TVIMPSTEnums::ELast ; index++ )        
        {
	    //create the subservice
	    switch(index)
	        {
	        case TVIMPSTEnums::EVoip:
	            {
	            if(IsSubServiceSupportedInternal(TVIMPSTEnums::EVoip))
	                {
	                CVIMPSTEngineVOIPSubService* item = 
	                CVIMPSTEngineVOIPSubService::NewL(aServiceId, *iCchHandler,
	                        iTableFetcher, *this);
	                iSubService.Append(item);
	                }
	            break;				
	            }
	        case TVIMPSTEnums::EPresence:
	            {
	            if(presenceSupported)
	                {
	                if (iSessionCntxtObserver)
	                    {
	                    CVIMPSTEnginePresenceSubService* item = CVIMPSTEnginePresenceSubService::NewL(
	                            aServiceId,*iCchHandler,iTableFetcher,
	                            *iSessionCntxtObserver,*this
	                    );
	                    iSubService.Append(item);
	                    iSessionCntxtObserver->RegisterObserver(item);					
	                    }
	                }
	            break;
	            }
	        case TVIMPSTEnums::EIM:
	            {
	            if(imSupported)
	                {
	                CVIMPSTEngineIMSubService* item = 
	                CVIMPSTEngineIMSubService::NewL(aServiceId, *iCchHandler, 
	                        iTableFetcher,*this);
	                iSubService.Append(item);
	                }
	            break;
	            }
	        default:		
	            {
	            break;
	            }

	        }				
	    }
		
    //Create the required extention mgrs
    CreateExtentionFeaturesL(); 		
    iState = ParseGetServiceState();
    //if cch is already logged in, and the callback's servicestatuschanged() stopped
    RetriveContextIfCChEnabledL();
    }


// ---------------------------------------------------------
// CVIMPSTEngine::CVIMPSTEngine
// 
// ---------------------------------------------------------

CVIMPSTEngine::CVIMPSTEngine( TUint32 aServiceId,
								CVIMPSTEngineServiceTableFetcher& aTableFetcher ) :
iServiceId(aServiceId),
iTableFetcher(aTableFetcher)								
	{
    iUnInstall = EFalse;
	}

// ---------------------------------------------------------
// CVIMPSTEngine::RetriveContextIfCChEnabledL
// 
// ---------------------------------------------------------
void CVIMPSTEngine::RetriveContextIfCChEnabledL()
	{
	TRACER_AUTO; 
    if ( (iSessionCntxtObserver) && 
         (TVIMPSTEnums::ESVCEUpdatingContacts == iState || TVIMPSTEnums::ESVCERegistered == iState ) 
   		 && ( TVIMPSTEnums::EVIMPSTBindNotDone == iSessionCntxtObserver->ContextBindStatus()  ) )
		{
		TInt ximpAdapterUid = iTableFetcher.XimpAdapterUidL(iServiceId);
		TInt err = iSessionCntxtObserver->ServerBindL(TUid::Uid( ximpAdapterUid ));	      
        if(KErrNone != err)
            {
            //should never reache here;
            iCchHandler->DisableService();
            User::LeaveIfError(err);
            }
        SetExtentionFeaturesSupportedL();
		}

	}

// ---------------------------------------------------------
// CVIMPSTEngine::LoginL
// 
// ---------------------------------------------------------
TInt CVIMPSTEngine::Login()
	{
	TRACER_AUTO; 
    TRACE( "ServiceId: %d", iServiceId );
	
	TInt error = KErrNotFound;
	if(iCchHandler)
		{
	TRACE( "EnableService Called");
		error =  iCchHandler->EnableService(); 		
		}
	
	TRACE( "error: %d", error );
		
	return error;
	}


// ---------------------------------------------------------
// CVIMPSTEngine::Logout
// 
// ---------------------------------------------------------
void CVIMPSTEngine::LogoutL()
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	
	if(iCchHandler)
		{
		MVIMPSTEngineSubService* subService = SubService(TVIMPSTEnums::EPresence);
		if (subService)
		    {
		    //Get Presence SubService     
		    MVIMPSTEnginePresenceSubService& presenceSubService = 
		                    MVIMPSTEnginePresenceSubService::Cast(*subService);
		    if (TVIMPSTEnums::ESVCERegistered == subService->SubServiceState() ||
		            TVIMPSTEnums::ESVCEUpdatingContacts == subService->SubServiceState() ) 
		        {
		        // Unsubscribe all lists here to get whole list contents when doing a new bind.
                // Trap here to avoid unbind not being done if list operations should leave
                TRAP_IGNORE( presenceSubService.UnsubscribeListsL() ); 
		        }		    
		    }
 			//if either of Presence/IM Subservice supported then we need
      //to unbind to XIMP context. 
      if( iSessionCntxtObserver )
          {
          TRACE( "ServerUnBindL Called");
          iSessionCntxtObserver->ServerUnBindL(ETrue);    
          }		
		TRACE(" DisableService Called");
		iCchHandler->DisableService();  
		}	
	
	}


// ---------------------------------------------------------
// CVIMPSTEngine::ServiceId
// 
// ---------------------------------------------------------
TUint32 CVIMPSTEngine::ServiceId() const
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	return iServiceId;	
	}


// ---------------------------------------------------------
// CVIMPSTEngine::ServiceName
// 
// ---------------------------------------------------------
const TDesC& CVIMPSTEngine::ServiceName() const
	{
	TRACER_AUTO;
	TPtr serviceNamePtr = iServiceName->Des();
	TRACE( "ServiceName: '%S'", &serviceNamePtr );
	return *iServiceName;	
	}

// ---------------------------------------------------------
// CVIMPSTEngine::ServiceState
// 
// ---------------------------------------------------------
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngine::ServiceState() const
	{
	TRACER_AUTO;
	TRACE( "ServiceState: %d", iState  );
	return iState;
	}


// ---------------------------------------------------------
// CVIMPSTEngine::GetBrandInfo
// 
// ---------------------------------------------------------
TInt CVIMPSTEngine::GetBrandInfoL(TLanguage& aBrandLanguage, 
				TInt &aBrandVersion, TDes8& aBrandId) const

	{
	TRACER_AUTO;	
	TRACE("ServiceId: %d", iServiceId );
	
	iTableFetcher.GetBrandIdL(iServiceId, aBrandId);
	aBrandLanguage = iTableFetcher.PropertyBrandLanguageL(iServiceId);
	aBrandVersion = iTableFetcher.PropertyBrandVersionL(iServiceId);	
	return KErrNone;
	}
	

// ---------------------------------------------------------
// CVIMPSTEngine::IsSubServiceSupportedL
// 
// ---------------------------------------------------------
TBool CVIMPSTEngine::IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType aType) const 
	{
	TRACER_AUTO;	
	TRACE("ServiceId: %d", iServiceId);
	TRACE("SubServiceType: %d", aType);
	
	TBool support = EFalse;
	
	MVIMPSTEngineSubService* subService = SubService(aType);
	if (subService)
		{
		support = ETrue;
		}
	
	TRACE( "support: %d", support );		
	return support;
	
	}
					

// ---------------------------------------------------------
// CVIMPSTEngine::IsSubServiceEnabled
// 
// ---------------------------------------------------------		
TBool CVIMPSTEngine::IsSubServiceEnabled(TVIMPSTEnums::SubServiceType aType) const 
	{
	

	TRACER_AUTO;
    TRACE( "ServiceId: %d", iServiceId );
    TRACE( "SubServiceType: %d", aType );

	MVIMPSTEngineSubService* subService = SubService(aType);
	TBool enabled = EFalse;
	
	if (subService)
		{			
		if ( TVIMPSTEnums::ESVCERegistered == subService->SubServiceState() )
			{			
			enabled = ETrue;
			}
		}	
	TRACE( "enabled: %d", enabled );
	return enabled;	
	}
			
// ---------------------------------------------------------
// CVIMPSTEngine::GetContactStoreIdL
// 
// ---------------------------------------------------------	
void CVIMPSTEngine::ContactStoreIdL (TDes& aContactStoreId ) const
	{
	TRACER_AUTO;
	TRACE("ServiceId: %d", iServiceId );	
	iTableFetcher.GetContactStoreIdL(iServiceId, aContactStoreId);	

	}

// ---------------------------------------------------------------------------
// CVIMPSTEngine::RegisterServiceSessionObserver()
// ---------------------------------------------------------------------------
// 
void CVIMPSTEngine::RegisterServiceSessionObserverL
							(MVIMPSTEngineServiceStateEventObserver* aObserver)
	{
	TRACER_AUTO;
	__ASSERT_ALWAYS(aObserver,User::Leave(KErrArgument));	
	TInt index = iObserverArray.Find(aObserver);
	        if( index == KErrNotFound )
	            {
	            iObserverArray.Append( aObserver );   
	            } 

	}
	
// ---------------------------------------------------------------------------
// CVIMPSTEngine::UnRegisterServiceSessionObserver()
// ---------------------------------------------------------------------------
//     
void CVIMPSTEngine::UnRegisterServiceSessionObserver
							(MVIMPSTEngineServiceStateEventObserver* aObserver)
	{

	TRACER_AUTO;
    if(aObserver)
        {
        TInt index = iObserverArray.Find(aObserver);
        if( index >=0 )
            {
            iObserverArray.Remove( index );
            iObserverArray.Compress();
            }
        }

	}
// ---------------------------------------------------------
// CVIMPSTEngine::IntializeStorage
// 
// ---------------------------------------------------------	
void CVIMPSTEngine::IntializeStorageL()
	{
	TRACER_AUTO;
    TRACE( "ServiceId: %d", iServiceId );	
 	iContactInterface = CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
 	if(iContactInterface)
		{
		iContactInterface->AddObserverL( this );   
		MVIMPSTSettingsStore* store = CVIMPSTSettingsStore::NewLC(); 
		RBuf lastUserName;
		lastUserName.CreateL( KPropertyMaxLength );
		CleanupClosePushL( lastUserName );
		store->GetL( iServiceId, EServiceLastUserName, lastUserName );
		if( lastUserName.Length() == 0 )
			{
			HBufC* userId = GetOwnUserIdFromCChOrStorageL();
			CleanupStack::PushL( userId );
			store->SetL(iServiceId, EServiceLastUserName,*userId);
			iContactInterface->SetOwnUserIdL(*userId );  
			CleanupStack::PopAndDestroy(); //userId   
			}
		else
			{
			iContactInterface->SetOwnUserIdL(lastUserName );	
			}
		CleanupStack::PopAndDestroy(&lastUserName); //lastUserName  
		CleanupStack::PopAndDestroy(); //store     
		}
 
	}
// ---------------------------------------------------------
// CVIMPSTEngine::UnIntializeStorage
// 
// ---------------------------------------------------------	
void CVIMPSTEngine::UnIntializeStorage()
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	if(iContactInterface)
		{
		iContactInterface->RemoveObserver( this );
		} 

	}
	
//-----------------------------------------------------------------------------
// CVIMPSTEngine::SetOwnUserIdIfChangedL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
void CVIMPSTEngine::SetOwnUserIdIfChangedL(const TDesC& aUserId)
	{

	TRACER_AUTO;
	MVIMPSTSettingsStore* store = CVIMPSTSettingsStore::NewLC();
	RBuf lastUserName;
	lastUserName.CreateL( KPropertyMaxLength );
	CleanupClosePushL( lastUserName );
	store->GetL( iServiceId, EServiceLastUserName, lastUserName );
	//first time when cenrep is not set with any value. set current as lastusername
	if( lastUserName.Compare( aUserId ) != 0  )
		{ 
		store->SetL(iServiceId, EServiceLastUserName,aUserId);
		iContactInterface->SetOwnUserIdL(aUserId );  
		} 
	CleanupStack::PopAndDestroy(&lastUserName); //lastUserName         
	CleanupStack::PopAndDestroy(); //store 

	}

//-----------------------------------------------------------------------------
// CVIMPSTEngine::GetOwnUserIdFromCChOrStorageL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
HBufC* CVIMPSTEngine::GetOwnUserIdFromCChOrStorageL() const
	{

	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	HBufC* buffer = NULL;
	if(iCchHandler)
		{
		// Set userid
		buffer = iCchHandler->GetConParametersL(ECchUsername);      
		CleanupStack::PushL( buffer );
		}
	if (!buffer)
	    {
	    buffer = KNullDesC().AllocLC();
	    }
	CleanupStack::Pop();//buffer 
	return buffer;
	}

//-----------------------------------------------------------------------------
// CVIMPSTEngine::SubService
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
MVIMPSTEngineSubService* CVIMPSTEngine::SubService(TVIMPSTEnums::SubServiceType aType) const 
	{
	TRACER_AUTO;
	TRACE("ServiceId: %d", iServiceId );
	TRACE( "SubServiceType: %d", aType );
	TInt subServiceCount = iSubService.Count();
	MVIMPSTEngineSubService* subService = NULL;	
	
	// iterate the service array
    for ( TInt index = 0; index < subServiceCount ; index++ )        
        {
        
        if (aType == iSubService[index]->Type())
	        {
	        subService = iSubService[index];	  
	        TRACE("SubService Found");
	        break;
	        }       			
		}	
	

    
	return subService;	
	}
	

//-----------------------------------------------------------------------------
// CVIMPSTEngine::ExtentionFeatures
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
MVIMPSTEngineExtentionFeatures* CVIMPSTEngine::ExtentionFeatures(TVIMPSTEnums::ExtentionType aType) const 
	{
	TRACER_AUTO;
	TRACE("ServiceId: %d", iServiceId);
	TRACE("ExtentionType: %d", aType);
	TInt fetaureCount = iExtentionFeatures.Count();
	MVIMPSTEngineExtentionFeatures* feature = NULL;	
	
	// iterate the service array
    for ( TInt index = 0; index < fetaureCount ; index++ )        
        {
        
        if (aType == iExtentionFeatures[index]->Type())
	        {
	        feature = iExtentionFeatures[index];	   
	        TRACE("ExtentionFeatures Found");	
	        break;
	        }       			
		}	
	

	return feature;	
	}



//-----------------------------------------------------------------------------
// CVIMPSTEngine::AddExtentionFeaturesL
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
void CVIMPSTEngine::AddExtentionFeaturesL(MVIMPSTEngineExtentionFeatures* aFeature) 
	{
	__ASSERT_ALWAYS(aFeature,User::Leave(KErrArgument));	

	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	if (aFeature)
		{		
	    TRACE( "ExtentionType: %d", aFeature->Type() );
		iExtentionFeatures.Append(aFeature);		
		TRACE( "Append Done" );
		}	
	

	
	}
	
	
//-----------------------------------------------------------------------------
// CVIMPSTEngine::RemoveExtentionFeatures
// ( Other items commented in header )
//----------------------------------------------------------------------------- 
void CVIMPSTEngine::RemoveExtentionFeatures(TVIMPSTEnums::ExtentionType aType) 
	{
	TRACER_AUTO;	

	TRACE( "ServiceId: %d", iServiceId );
	TInt fetaureCount = iExtentionFeatures.Count();
	
	// iterate the service array
    for ( TInt index = 0; index < fetaureCount ; index++ )        
        {
        
        if (aType == iExtentionFeatures[index]->Type())
	        {
	        TRACE( "ExtentionType: %d", aType );
	        iExtentionFeatures.Remove(index);
	        iExtentionFeatures.Compress();
	        TRACE( "Remove Done" );
	        break;
	        }       			
		}	
	

		
	}	
	


//-----------------------------------------------------------------------------
// CVIMPSTEngine::HandleServceConnectionEventL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void  CVIMPSTEngine::HandleServceConnectionEventL()
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );	

	TVIMPSTEnums::TVIMPSTRegistrationState previousState = iState;
	iState = ParseGetServiceState();	

	if(TVIMPSTEnums::ESVCENotRegistered == iState)
		{
		CVIMPSTEnginePresenceSubService* presSubService = 
		dynamic_cast<CVIMPSTEnginePresenceSubService*> 
		(SubService(TVIMPSTEnums::EPresence));   
		if(presSubService)
			{
			presSubService->ResetBlockedListManagerL();
			}
		}
	TRACE( "PreviousState: %d, CurrentState:%d", previousState, iState );
	//Only send observer notifications if there is any change in the Service State
	//Otherwise not required
	if (previousState != iState)
		{
		//Presence Service needs to Register for the XIMP Events once when
		//the context gets binded
		//Get Presence SubService   
		CVIMPSTEnginePresenceSubService* presSubService = 
									dynamic_cast<CVIMPSTEnginePresenceSubService*>(SubService(TVIMPSTEnums::EPresence));     
		// in case of roaming(moving from one network to another)
		// servicetab goes from registered to connecting state, hence the unbind is done here.
    if (  presSubService && TVIMPSTEnums::ESVCERegistered == previousState &&
          TVIMPSTEnums::ESVCENetworkConnecting == iState )
          {
          TRACE( "UnsubscribeLists"  );
          
          TRAPD( err, presSubService->UnsubscribeListsL() );
	  
          TRACE( "UnsubscribeLists -err: %d" , err );
	  
          if ( iSessionCntxtObserver )
              {
              iSessionCntxtObserver->ServerUnBindL( ETrue );
              }
          }          
		TInt count = iObserverArray.Count();
		for (TInt index=0; index<count; index++)		
			{
			TRACE( "calling HandleServiceEventL" );
			iObserverArray[index]->HandleServiceEventL(iState, KErrNone);
			}		
		TInt ximpAdapterUid = iTableFetcher.XimpAdapterUidL(iServiceId);
		if ( (iSessionCntxtObserver) && (KErrNotFound != ximpAdapterUid ) )
			{

			if ( ( TVIMPSTEnums::ESVCEUpdatingContacts == iState || 
			     TVIMPSTEnums::ESVCERegistered == iState ) &&
			     TVIMPSTEnums::EVIMPSTBindNotDone == iSessionCntxtObserver->ContextBindStatus() )
				{				
				TInt error = iSessionCntxtObserver->ServerBindL(TUid::Uid( ximpAdapterUid ));
				if(KErrNone != error)
					{
					//should never reache here;
					iCchHandler->DisableService();
					User::LeaveIfError(error);
					}
				SetExtentionFeaturesSupportedL();
				}
			else if (TVIMPSTEnums::ESVCENotRegistered == iState )
				{
				MVIMPSTStorageServiceView* storage =  CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
				if( storage )
					{
					 MVIMPSTStorageContact& ownContact = storage->OwnContactL();
                     TPtrC ownUserId = ownContact.UserId();
                     if( ownUserId.Length() &&  presSubService )
                        {
                        presSubService->UnSubscribePresenceOfSingleContactL( ownUserId );
                        }
					 MVIMPSTStorageContactList* list = storage->FindContactList(KIMContactListId) ;      
					if ( list )                     
						{
						TInt count = list->Count();
						for(TInt i = 0;i < count; i++)
							{
							MVIMPSTStorageContact* contact = &(list->operator[](i));
							if(contact)
								{
								if(presSubService)
									{
									if( contact->UserId().Length() )
										{
										TRAP_IGNORE( presSubService->UnSubscribePresenceOfSingleContactL( contact->UserId() ) );	
										}
									if(!(contact->UserId().Length())) // if no XspId
									    {
									    contact->SetOnlineStatus(TVIMPSTEnums::EUnknown);
									    }
									else if(TVIMPSTEnums::EPending != contact->OnlineStatus())
										{
										contact->SetOnlineStatus(TVIMPSTEnums::EOffline);
										}// end of if
									}// end of for
								} // end of if(contact)
							}// end of for
						storage->Sort(KIMContactListId);						
						// Inform service state changes to UI service tab
						// and further to update the buddy list after sort. 
						TInt obsvrArrayCount = iObserverArray.Count();
						for (TInt index=0; index<obsvrArrayCount; index++)		
							{
							iObserverArray[index]->HandleServiceEventL(iState, KErrNone);
							}	
						}
					}
				ReSetExtentionFeaturesSupportedL();
				} //end of else if
			}// end of isessioncontextobserver and ximpadapteruid.
			}// end of if(previousstate != iState)	

	}
	
//-----------------------------------------------------------------------------
// CVIMPSTEngine::ParseGetServiceState
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngine::ParseGetServiceState()
	{
	TRACER_AUTO;
	TInt subServiceCount = iSubService.Count();
	TInt serviceState = 0;
		
	//Find the cumulative of the ServiceStates of all subservices
	// iterate the service array
    for ( TInt index = 0; index < subServiceCount ; index++ )        
        {           
        serviceState |= iSubService[index]->SubServiceState();
		}
	
	//ServiceState
    TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCENotRegistered;
    
    //The logic below is that, if any one subservice not enabled for us the service is not enabled
    //So to do this we assign priority for each service state
    //Connecting gets the High Priority, when copmared to Waiting for Connection,
    //Disconnecting, Registered, UnRegistered.
    //By this way the Sevice Tab will be more usable and atleast we can perform operations
    //related to the the logged in subservice
    //its upto the connection handlers to do the job properly, whether to provide connectivity to all
    //sub services or not.. 
    
    //Start parsing and decide the state of the ServiceTab   
    if ( serviceState & TVIMPSTEnums::ESVCENetworkConnecting )
	    {
	    //Connecting	
	    state = TVIMPSTEnums::ESVCENetworkConnecting;
	    }
	else if ( serviceState & TVIMPSTEnums::ESVCEWaitingForNetwork )
	    {
	    //Waiting for Connection	
	    state = TVIMPSTEnums::ESVCEWaitingForNetwork;
	    }
	else if ( serviceState & TVIMPSTEnums::ESVCEUpdatingContacts )
	    {
	    //Waiting for Connection	
	    state = TVIMPSTEnums::ESVCEUpdatingContacts;
	    }	    	    
    else if ( serviceState & TVIMPSTEnums::ESVCENetworkDisConnecting )
	    {
	    //disconnecting	
	    state = TVIMPSTEnums::ESVCENetworkDisConnecting;
	    }   
	else if ( serviceState & TVIMPSTEnums::ESVCERegistered ) 
	    {
	    //Enabled	
	    state = TVIMPSTEnums::ESVCERegistered;
	    }	     	
	
	return state;	
	}	

//-----------------------------------------------------------------------------
// CVIMPSTEngine::HandleContactFetchedL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void  CVIMPSTEngine::HandleContactFetchedL()
    {
  
	TRACER_AUTO;
    
    if (TVIMPSTEnums::ESVCERegistered == iState ||
	    	TVIMPSTEnums::ESVCEUpdatingContacts == iState ) 
		{
		MVIMPSTEngineSubService* subService = SubService(TVIMPSTEnums::EPresence);
		if (subService)
			{
			//Get Presence SubService     
			MVIMPSTEnginePresenceSubService& presenceSubService = 
			        MVIMPSTEnginePresenceSubService::Cast(*subService);

			if (TVIMPSTEnums::ESVCERegistered == subService->SubServiceState() ||
				TVIMPSTEnums::ESVCEUpdatingContacts == subService->SubServiceState() ) 
				{
				presenceSubService.FetchPresenceFromCache();
				}	    
			presenceSubService.UpdatePresenceStateL();
			}
		// this is added here as for xmpp subscribeforsearchkeys needs to be done only 
		// after the contactts are fetched from the server,
		// SetExtensionFeaturesSupported is moved to immediately after login.
		// as for sip case it would never come to this line if there are not contacts in it.   
		MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(
		                    TVIMPSTEnums::ESearch);
		if (feature)
			{
			MVIMPSTEngineSearchMgrExtention& searchFeature = 
							MVIMPSTEngineSearchMgrExtention::Cast(*feature);                				
			//subscribe for search keys, since this is a server feature            				               				
			if(TVIMPSTEnums::ESupportUnKnown == searchFeature.IsSupported())
			    {
			    searchFeature.SubscribeForSearchKeysL();                
			    }
			}                
		}                
  
    }
    
// --------------------------------------------------------------------------
// CVIMPSTEngine::HandleStorageChangeL
// --------------------------------------------------------------------------
//
void CVIMPSTEngine::HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
		         							MVIMPSTStorageContactList* /*aList*/, 
			        						MVIMPSTStorageContact* aContact,
			        						TInt /*aContactIndex*/ )
    {
  
	TRACER_AUTO;  
	TRACE(" iState = %d", iState);
    if (TVIMPSTEnums::ESVCERegistered == iState ||	TVIMPSTEnums::ESVCEUpdatingContacts == iState ) 
		{	
        TRACE( "Inside IF");	
        TRACE( "aEventType : %d", aEventType);
		switch( aEventType )
		    {
		    case TVIMPSTEnums::EStorageContactReadComplete:
		    case TVIMPSTEnums::EStorageContactFetchComplete:
		        {
		        TRACE( "aEventType EStorageContactFetchComplete");
		        MVIMPSTEngineSubService* subService =  SubService(TVIMPSTEnums::EPresence); 
		        if( subService )   
		            {
		       
		           TRACE( "Inside IF subService");
		            MVIMPSTEnginePresenceSubService& presenceSubService = MVIMPSTEnginePresenceSubService::Cast(*subService);
		            
		            MVIMPSTStorageServiceView* storage =  CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
		            if( storage )
		                {
		                MVIMPSTStorageContact& ownContact = storage->OwnContactL();
                        TPtrC ownUserId = ownContact.UserId();
                        if( ownUserId.Length() )
                            {
                            presenceSubService.SubscribePresenceOfSingleContactL( ownUserId );
                            }
		          
		                TRACE( "inside  if storage");
		                MVIMPSTStorageContactList* list = storage->FindContactList(KIMContactListId) ;      
		                if ( list )                     
		                    {
		               
		                     TRACE( "inside  if list");
		                    TInt count = list->Count();		                 
		                    TRACE( "inside  count = %d", count);
		                    for(TInt i = 0;i < count; i++)
		                        {
		                        MVIMPSTStorageContact* contact = &(list->operator[](i));
		                        if(contact)
		                            {
                                    if( contact->UserId().Length() )
                                        {
                                        TPtrC userId = contact->UserId();                                   
                                        TRACE("inside userId = %S", &userId);
                                        presenceSubService.SubscribePresenceOfSingleContactL( contact->UserId() );    
                                        }
		                            } // end of if(contact)
		                        }// end of for
		                    }
		                }

		            }
		        HandleContactFetchedL();

		        break;	
		        }
		    case TVIMPSTEnums::EStorageEventOwnUserChanged:
		    case TVIMPSTEnums::EStorageEventContactAddition:
		    case TVIMPSTEnums::EStorageContactSynchronizing:
		    case TVIMPSTEnums::EStorageContactReading:
		    case TVIMPSTEnums::EStorageContactFetchExistInStore:
		        {		    
		        TRACE( "aEventType : %d", aEventType);
		        MVIMPSTEngineSubService* subService =  SubService(TVIMPSTEnums::EPresence); 
		        if( subService && aContact && 
		                aContact->UserId().Length() && 
		                TVIMPSTEnums::ESVCERegistered == iState )   
		            {
		            MVIMPSTEnginePresenceSubService& presenceSubService = MVIMPSTEnginePresenceSubService::Cast(*subService);
		            presenceSubService.SubscribePresenceOfSingleContactL( aContact->UserId() );	
		            presenceSubService.FetchPresenceOfSingleContactL(aContact->UserId());
		            }
		        break;
		        }
		    case TVIMPSTEnums::EStorageEventUserIdPreChange:
		    case TVIMPSTEnums::EStorageEventDeleteFromPbk:
		        {
		     
		        TRACE("EStorageEventUserIdPreChange/EStorageEventDeleteFromPbk");
		        if( aContact && aContact->UserId().Length() ) 
		            {
		            TPtrC userId = aContact->UserId();
		            MVIMPSTEngineSubService* subService =  SubService(TVIMPSTEnums::EPresence);	
		            // incase of delete from phonebook delete from server and close conversation if exist
		            // mostly applicable for local store cases
		            if( subService )
		                {
		                MVIMPSTEnginePresenceSubService& presenceSubService = MVIMPSTEnginePresenceSubService::Cast(*subService);
		                presenceSubService.UnSubscribePresenceOfSingleContactL( userId ) ;	
		                }
		            MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(TVIMPSTEnums::EContactManagement);
		            if (feature )	
		                {
		                MVIMPSTEngineContactMgmtExtention& contactMgr = MVIMPSTEngineContactMgmtExtention::Cast(*feature);
		                contactMgr.DeleteServerContactL(KNullDesC,userId) ;
		                }
		            MVIMPSTEngineSubService* imService = SubService(TVIMPSTEnums::EIM) ;    
		            if(imService)
		                {
		                MVIMPSTEngineIMSubService& imSubService = MVIMPSTEngineIMSubService::Cast (*imService);
		                imSubService.CloseConversationL( userId );
		                }
		            }
		        break;
		        }
		    case TVIMPSTEnums::EStorageEventUserIdPostChange:
		        {
		     
		        TRACE("EStorageEventUserIdPostChange");
		        if( aContact && aContact->UserId().Length() ) 
		            {
		            TPtrC userId = aContact->UserId();
		            // incase of delete from phonebook delete from server and close conversation if exist
		            // mostly applicable for local store cases
		            MVIMPSTEngineSubService* subService =  SubService(TVIMPSTEnums::EPresence);
		            if( subService )
		                {
		                MVIMPSTEnginePresenceSubService& presenceSubService = MVIMPSTEnginePresenceSubService::Cast(*subService);
		                presenceSubService.SubscribePresenceOfSingleContactL( userId );	
		                }
		            MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(TVIMPSTEnums::EContactManagement);
		            if (feature )   
		                {
		                MVIMPSTEngineContactMgmtExtention& contactMgr = MVIMPSTEngineContactMgmtExtention::Cast(*feature);
		                contactMgr.AddServerContactL(KListNameAllBuddy,userId, aContact->Name(), EFalse );
		                }
		            }
		        break;
		        } 
		    default:
		        {
		        break;
		        }
		    }
		}
    }
    
//-----------------------------------------------------------------------------
// CVIMPSTEngine::CreateExtentionFeaturesL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CVIMPSTEngine::CreateExtentionFeaturesL()
    {
	TRACER_AUTO;
    if (!iSessionCntxtObserver)
        {
        return;	
        }

    //create all the features
    //first find whether we have the extention already created
    //in the extention manager list 
    
    //first find whether we have the extention already created
    //in the extention manager list
    MVIMPSTEngineExtentionFeatures* contactMgtFeature = ExtentionFeatures(
                            TVIMPSTEnums::EContactManagement);
    
    if (!contactMgtFeature)
        {
        CVIMPSTEngineContactMgmtExtention* contactItem = 
                        CVIMPSTEngineContactMgmtExtention::NewL(
                        iSessionCntxtObserver->XimpPresenceContextL(),
                        iSessionCntxtObserver->XimpPresentityGroupsL(),
                        *iSessionCntxtObserver->GetRequestMapper());

        AddExtentionFeaturesL(contactItem);
        //Unregister is done by session context when the presence context gets
        //unbinded
        iSessionCntxtObserver->RegisterObserver(contactItem);   
        }
    
    
    //first find whether we have the extention already created
    //in the extention manager list
    MVIMPSTEngineExtentionFeatures* searchFeature = ExtentionFeatures(
                            TVIMPSTEnums::ESearch);
    if (!searchFeature)
        {                   
        CVIMPSTEngineSearchMgrExtention* searchitem = 
                CVIMPSTEngineSearchMgrExtention::NewL(
                            iSessionCntxtObserver->XimpPresenceContextL(),
                            *iSessionCntxtObserver->GetRequestMapper());
                            
        AddExtentionFeaturesL(searchitem);                  
        
        //Unregister is done by session context when the presence context gets
        //unbinded
        iSessionCntxtObserver->RegisterObserver(searchitem);
        }
    
    }

//-----------------------------------------------------------------------------
// CVIMPSTEngine::SetExtentionFeaturesSupportedL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CVIMPSTEngine::SetExtentionFeaturesSupportedL()
    {
	TRACER_AUTO;
    if (!iSessionCntxtObserver)
		{
		return;	
		}
		
    //We would have got the supported features 
    //iterate and create the required extentionmgr
    TInt supportedFeatures = iSessionCntxtObserver->GetSupportedFeatures();

    if ( !(EVIMPSTFeatureSearch & supportedFeatures) )
        {
        //first find whether we have the extention already created
        //in the extention manager list
        MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(
                                TVIMPSTEnums::ESearch);
        if (feature)
            {
            MVIMPSTEngineSearchMgrExtention& searchFeature = 
            				MVIMPSTEngineSearchMgrExtention::Cast(*feature);                				
            searchFeature.SetSupported(TVIMPSTEnums::ENotSupported); 
            //subscribe for search keys is done once the contacts are fetched from the server.
            //check HandleContactFetchCOmpletedL().
            }                
        }
        
    if (( EVIMPSTFeatureAddContact & supportedFeatures ) || 
    			( EVIMPSTFeatureDeleteContact & supportedFeatures ))
        { 
        //first find whether we have the extention already created
        //in the extention manager list
        MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(
                                TVIMPSTEnums::EContactManagement);
        if (feature)
            {
            MVIMPSTEngineContactMgmtExtention& contactMgmtFeature = 
            				CVIMPSTEngineContactMgmtExtention::Cast(*feature);                				
            contactMgmtFeature.SetSupported(TVIMPSTEnums::ESupported);                
            }

        }
    
    if ( EVIMPSTFeatureAvatar & supportedFeatures )
    	{
        //first find whether we have the extention already created
        //in the extention manager list
        MVIMPSTEngineSubService* subService = SubService(
                                TVIMPSTEnums::EPresence);
        if (subService)
            {
            MVIMPSTEnginePresenceSubService& avatarFeature = 
            				CVIMPSTEnginePresenceSubService::Cast(*subService);                				
            avatarFeature.SetAvatarSupported(ETrue);                
            }
        }
    }


//-----------------------------------------------------------------------------
// CVIMPSTEngine::ReSetExtentionFeaturesSupportedL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
void CVIMPSTEngine::ReSetExtentionFeaturesSupportedL()
    {
	TRACER_AUTO;
    //first find whether we have the extention already created
    //in the extention manager list
    MVIMPSTEngineExtentionFeatures* feature = ExtentionFeatures(
                            TVIMPSTEnums::ESearch);
    if (feature)
        {
        MVIMPSTEngineSearchMgrExtention& searchFeature = 
        				MVIMPSTEngineSearchMgrExtention::Cast(*feature);                				
        searchFeature.SetSupported(TVIMPSTEnums::ESupportUnKnown);                    
        }                

    //first find whether we have the extention already created
    //in the extention manager list
    MVIMPSTEngineExtentionFeatures* feature1 = ExtentionFeatures(
                            TVIMPSTEnums::EContactManagement);
    if (feature1)
        {
        MVIMPSTEngineContactMgmtExtention& contactMgmtFeature = 
        				CVIMPSTEngineContactMgmtExtention::Cast(*feature1);                				
        contactMgmtFeature.SetSupported(TVIMPSTEnums::ESupportUnKnown);                
        }

    //first find whether we have the extention already created
    //in the extention manager list
    MVIMPSTEngineSubService* subService = SubService(
                                TVIMPSTEnums::EPresence );
    if (subService)
        {
        MVIMPSTEnginePresenceSubService& avatarFeature = 
        				CVIMPSTEnginePresenceSubService::Cast(*subService);                				
        avatarFeature.SetAvatarSupported(EFalse);                
        }
    
    }


// ---------------------------------------------------------
// CVIMPSTEngine::DefaultDomainNameL
// 
// ---------------------------------------------------------
void CVIMPSTEngine::DefaultDomainNameL( TDes& aDefaultDomainName ) const
	{
	TRACER_AUTO;	
	TRACE( "ServiceId: %d", iServiceId );
	iTableFetcher.GetDefaultDomainNameL(iServiceId, aDefaultDomainName);	

	}
	

// ---------------------------------------------------------
// CVIMPSTEngine::IsSubServiceSupportedInternal
// 
// ---------------------------------------------------------
TBool CVIMPSTEngine::IsSubServiceSupportedInternal(TVIMPSTEnums::SubServiceType aType) const 
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	TRACE( "SubServiceType: %d", aType );
	TInt err = KErrNotFound;
	TBool support = EFalse;
	TCCHSubserviceState aCCHState(ECCHUninitialized);	

	switch(aType)
		{
		case TVIMPSTEnums::EVoip:
			{
			err = iCchHandler->GetServiceState(iServiceId, ECCHVoIPSub, aCCHState);				
			break;				
			}
		case TVIMPSTEnums::EPresence:
			{
			err = iCchHandler->GetServiceState(iServiceId, ECCHPresenceSub, aCCHState);							
			break;
			}
		case TVIMPSTEnums::EIM:
			{
			err = iCchHandler->GetServiceState(iServiceId, ECCHIMSub, aCCHState);	
			break;
			}
		default:		
			{
			break;
			}
		
		}
	
	if (KErrNone == err)
		{
		support = ETrue;		
		}
		

	TRACE("support: %d", support );
	return support;
	}
	

// ---------------------------------------------------------
// CVIMPSTEngine::ChangeConnectionL
// 
// ---------------------------------------------------------
TInt CVIMPSTEngine::ChangeConnectionL()
	{
	TRACER_AUTO;
	TRACE( "ServiceId: %d", iServiceId );
	TInt error = KErrNotFound;
	if(iCchHandler)
		{
	
	   TRACE( "ChangeConnectionL() Called" );
		error =  iCchHandler->ChangeConnectionL(); 		
		}
	

	TRACE("error: %d", error );

	return error;
	}		

// ---------------------------------------------------------
// CVIMPSTEngine::IsBlockSupportedL
// 
// ---------------------------------------------------------

TBool CVIMPSTEngine::IsBlockSupportedL()
    {
	TRACER_AUTO;
    TBool isBlockSupported = EFalse;
    //We would have got the supported features 
    if(iSessionCntxtObserver && (TVIMPSTEnums::ESVCERegistered == ParseGetServiceState()))
        {
        TInt supportedFeatures = iSessionCntxtObserver->GetSupportedFeatures();
        if ( (EVIMPSTFeatureBlock & supportedFeatures) && (EVIMPSTFeatureUnBlock & supportedFeatures) )
            {
            isBlockSupported = ETrue;
            }
        }
    return isBlockSupported;
    }

// ---------------------------------------------------------
// CVIMPSTEngine::IsPasswordAvailable
// returns True if password is present in the settings else returns false
// ---------------------------------------------------------
TBool CVIMPSTEngine::IsPasswordAvailableL()
    {
	TRACER_AUTO;
 
    return(iCchHandler->IsPasswordAvailable(ECchPasswordSet));    
    }
    
// ---------------------------------------------------------
// CVIMPSTEngine::DeleteDataBase
// ---------------------------------------------------------
void CVIMPSTEngine::DeleteDataBaseL() 
	{
	TRACER_AUTO;
	MVIMPSTStorageServiceView* storage = 
	CVIMPSTStorageManagerFactory::ContactListInterfaceL(iServiceId);
	if( storage )
		{
		//delete cdb file.
		storage->DeleteDatabaseL();
		}
	}
	
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService::GetPreseceSubService
// ---------------------------------------------------------------------------
//
MVIMPSTEnginePresenceSubService* CVIMPSTEngine::GetPreseceSubService()
	{
	TRACER_AUTO;
	MVIMPSTEngineSubService* subService = SubService(TVIMPSTEnums::EPresence);
	if (subService)
		{
		//Get Presence SubService     
		MVIMPSTEnginePresenceSubService& presenceSubService = 
									MVIMPSTEnginePresenceSubService::Cast(*subService);
		return &presenceSubService ;
		}
	return NULL;	
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngine::FetchBlockedList
// ---------------------------------------------------------------------------
//
void CVIMPSTEngine::FetchBlockedListL()
	{
	MVIMPSTEnginePresenceSubService* subService = GetPreseceSubService();
	if( subService )
		{
		//fetch the blocked list from server
		subService->FetchBlockedListFromServerL(iBlockListObs);
		}
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngine::RegisterBlockedListObserver
// ---------------------------------------------------------------------------
//
void CVIMPSTEngine::RegisterBlockedListObserver(
				MVIMPSTEngineBlockedListFetchEventObserver* aOb)
	{
	iBlockListObs = aOb;
	}

// ---------------------------------------------------------------------------
// CVIMPSTEngine::HandleChangeConnectionEventL
// ---------------------------------------------------------------------------
//
void CVIMPSTEngine::HandleChangeConnectionEventL()
    {
	TRACER_AUTO;
    MVIMPSTEngineSubService* subService = SubService(TVIMPSTEnums::EPresence);
    if (subService)
        {
        //Get Presence SubService     
        MVIMPSTEnginePresenceSubService& presenceSubService = 
        MVIMPSTEnginePresenceSubService::Cast(*subService);
        if (TVIMPSTEnums::ESVCERegistered == subService->SubServiceState() ||
                TVIMPSTEnums::ESVCEUpdatingContacts == subService->SubServiceState() ) 
            {
            // Unsubscribe all lists here to get whole list contents when doing a new bind.
           // Trap here to avoid unbind not being done if list operations should leave
            TRAP_IGNORE( presenceSubService.UnsubscribeListsL() ); 
            }           
        }
        
    if( iSessionCntxtObserver )
        {        
        iSessionCntxtObserver->ServerUnBindL(ETrue); 
        }
    }

// -------------------------------------------------------
// CVIMPSTEngine :: IsUnInstalled
// For Description see MVIMPSTEngine
// -------------------------------------------------------
//
TBool CVIMPSTEngine::IsUnInstalled( )
    {
    return iUnInstall;
    }
//  End of File
