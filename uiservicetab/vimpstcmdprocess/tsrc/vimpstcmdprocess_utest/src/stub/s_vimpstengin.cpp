/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

#include "s_vimpstengin.h"
//#include "s_enginecontmgtextfeatures.h"
#include "s_enginesearchextention.h"
#include "s_vimpstenginepresencesubservice.h"
#include "s_imsubservice.h"
#include "tvimpstenums.h"
#include "s_engineextentionfeatures.h"

/*
*/
/*MVIMPSTEngine& vimpstengine_stub::NewL()
{
	return new vimpstengine_stub;
}*/

/**
* Gets the Brand Information for the current service
* @param aBrandLanguage - Holds the BrandLanguage
* @param aBrandVersion - Holds the Version to be used
* @param aBrandId - Holds the brandId 
* caller should allocate memory for all params and send the same to this API
* The result will be available in the InParams otself.
* @return TInt Error code
*/
TInt vimpstengine_stub::GetBrandInfoL(TLanguage& /*aBrandLanguage*/, 
		TInt& /*aBrandVersion*/, TDes8& /*aBrandId*/) const 
	{
	return 1;			
	}

/**
* Returns the Service State - Cumulative Service State of the SubServices
* @return TVIMPSTRegistrationState, current service state.
*/
TVIMPSTEnums::TVIMPSTRegistrationState vimpstengine_stub::ServiceState() const 
	{
	TVIMPSTEnums::TVIMPSTRegistrationState state = TVIMPSTEnums::ESVCERegistered;
	return state;
	}


/**
* Gets the Service Id
* @return TUint32 Service Id
*/
TUint32 vimpstengine_stub::ServiceId() const
{
return 1;	
	
}
	






/**
* Returns a SubService is Supported or not
* Components intertested in knowing whether a SubService is Supported or not can use this API
* @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
* @return TBool, ETrue if the SubService is Supported in this Service, else returns EFalse
*/			
TBool vimpstengine_stub::IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType /*aType*/) const 
	{
	return ETrue;
	}
		

/**
* Returns TBool whether a SubService is Enabled or not
* Components intertested in knowing whether a SubService is Enabled or not can use this API
* @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
* @return TBool, ETrue if the SubService is Enabled in this Service, else returns EFalse
*/
TBool vimpstengine_stub::IsSubServiceEnabled(TVIMPSTEnums::SubServiceType /*aType*/) const 
{
	return ETrue;
}

/**
* Gets the StoreName for given service
* @param aContactStoreId, Virtual store id. 
* The caller of the this function has to assign required memory for aContactStoreId
* aContactStoreId will be filled with the Virtual Store Id by this API
*/
void vimpstengine_stub::ContactStoreIdL( 
		TDes& /*aContactStoreId*/ ) const 
{
	
}

/**
* returns the Service Name	
* @return TDesC&, reference to service name. Ownership is not transferred.
*/
const TDesC& vimpstengine_stub::ServiceName() const 
{
	return KNullDesC;
}


/**
* Register Session Observer.
* @aObserver, observer ot be registered.
*/
void vimpstengine_stub::RegisterServiceSessionObserverL
				(MVIMPSTEngineServiceStateEventObserver* /*aObserver*/) 
{
	
}

/**
* UnRegisters Session Observer
* @aObserver, observer ot be Unregistered.   
*/
void vimpstengine_stub::UnRegisterServiceSessionObserver
				(MVIMPSTEngineServiceStateEventObserver* /*aObserver*/) 
{
	
}

  
/**
* intialize storage 
*/	
void vimpstengine_stub::IntializeStorageL() 
{
	
}

/**
* unintialize storage
*/	
void vimpstengine_stub::UnIntializeStorage() 
{
	
}


/**
* Get the Ptr to the SubService given the Type of the SubService
* This API returns a Generalised SubService Class MVIMPSTEngineSubService
* Ownership of the SubService is not Transferred to the Caller
* The Caller can use this (MVIMPSTEngineSubService*) ptr to typecast to appropriate SubService Interface
* Usage	 : 			MVIMPSTEngineIMSubService* imSubService = 
*			   	   			dynamic_cast<MVIMPSTEngineIMSubService*> 
*			   	   			(iEngine.SubService(TVIMPSTEnums::EIM));
*			    
*				    if(imSubService)
*				        {
*				        imSubService->CloseConversationL( iContactId );
*				        }
* @param aType - SubService Type (TVIMPSTEnums::SubServiceType) defined in tvimpstenums.h
* @return - Ptr to the SubService - Value can even be NULL
*/ 
MVIMPSTEngineSubService* vimpstengine_stub::SubService(TVIMPSTEnums::SubServiceType aType) const
{
MVIMPSTEngineSubService* subservice = NULL;
switch(aType)
        {
        case TVIMPSTEnums::EPresence:
            {
            TRAP_IGNORE(subservice = CVIMPSTEnginePresenceSubService_Stub::NewL());
            return subservice ;
            break;
            }
     /*   case TVIMPSTEnums::EIM:
                    {
                    TRAP_IGNORE(subservice = CVIMPSTEngineImSubService_Stub::NewL());
                    return subservice ;
                    break;
                    }*/
            
        default:
            break;
        }
	return subservice;
}


/**
*/
MVIMPSTEngineExtentionFeatures* vimpstengine_stub::ExtentionFeatures(TVIMPSTEnums::ExtentionType aType) const 
{
	MVIMPSTEngineExtentionFeatures* extFeature = NULL;
	switch(aType)
		{
		case TVIMPSTEnums::EContactManagement:
			{
			TRAP_IGNORE(extFeature = extentionfeatures_stub::NewL());
			return extFeature ;
			break;
			}
		case TVIMPSTEnums::ESearch:
			{
			TRAP_IGNORE(extFeature = searchextention_stub::NewL());
			return extFeature ;
			break;
			}
		}
		
		return extFeature;
}

/**
* Sets the User Name for the Service 
* Stores the username to CCH Plugin
*
void vimpstengine_stub::SetUserNameL(const TDesC& /*aUserid) 
{
	
}
*/
/**
* Sets the Password for the Service 
* Stores the Password to CCH Plugin
* @param Password, Password to be stored.
*
void vimpstengine_stub::SetPassword(const TDesC& /*aPassword) 
{
	
}


/**
* gets the UserName for the Service          
* @return HBufC* - Ownership is transferred to the caller
* This API allocates memory for the UserName and transfers the Ownership to the Caller
*
HBufC* vimpstengine_stub::UserNameL() const 
{
_LIT(KText,"Nutan@presence");
   HBufC* buf = HBufC::NewL(20);
  
   *buf = KText; 

	return buf;
}

/**
* gets the Password for the Service          
* @return HBufC* - Ownership is transferred to the caller
* This API allocates memory for the Password and transfers the Ownership to the Caller
*
HBufC* vimpstengine_stub::PasswordL() const 
{
	return NULL;
}
*/
/**
* API used to login to the Service                  
*/
TInt vimpstengine_stub::Login() 
{
	return 1;
}

/**
* API used to Logout to the Service                  
*/
void vimpstengine_stub::LogoutL() 
{
	
}


/**
* Gets the default domain name for given service
* @param aDefaultDomainName, Domain Name. 
* The caller of the this function has to assign required memory for aDefaultDomainName
* aDefaultDomainName will be filled with the deafault domain name by this API
*/			
void vimpstengine_stub::DefaultDomainNameL( TDes& /*aDefaultDomainName */) const 
{
	
}
/**
 * IsBlockSupportedL
 * @returns ETrue if block/unblock is supported 
 * else EFalse
 */
TBool vimpstengine_stub::IsBlockSupportedL()
{
	return ETrue;
}

TInt vimpstengine_stub::ChangeConnectionL()
{
	return 1;
}
/**
* See MVIMPSTEngine
* 
* @return True if password is present in the settings else returns false
*
*/
TBool vimpstengine_stub::IsPasswordAvailableL()
{
return ETrue;
}


/**
          * Inform storage that data base need to be deleted.
          */
 void  vimpstengine_stub::DeleteDataBaseL()
    {
    
    }
 /*----------------------------------------------------------
  * vimpstengine_stub::GetPreseceSubService
  *-----------------------------------------------------------*/
 MVIMPSTEnginePresenceSubService* vimpstengine_stub::GetPreseceSubService()
     {
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
 /*----------------------------------------------------------
  * vimpstengine_stub::FetchBlockedListL
  *-----------------------------------------------------------*/

 void vimpstengine_stub::FetchBlockedListL()
     {
     
     }
 /*----------------------------------------------------------
  * vimpstengine_stub::RegisterBlockedListObserver
  *-----------------------------------------------------------*/

 void vimpstengine_stub::RegisterBlockedListObserver(
                     MVIMPSTEngineBlockedListFetchEventObserver* aOb)
     {
     
     }
 
 HBufC* vimpstengine_stub::GetOwnUserIdFromCChOrStorageL() const
 {
 HBufC* temp = NULL;
 return temp;
 }
 
 void vimpstengine_stub::RetriveContextIfCChEnabledL() 
     {
     
     }
 void vimpstengine_stub::SetOwnUserIdIfChangedL(const TDesC& aUserId ) 
     {
     
     }
 
 TBool vimpstengine_stub::IsUnInstalled()
     {
     return EFalse;
     }
 
     
//end of file
