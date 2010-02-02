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

// INCLUDE FILES
#include "s_vimpstenginepresencesubservice.h"
//Presence Observer
//#include "mvimpstenginepresencesubserviceeventobserver.h"

// CONTANTS
const TInt KUriMaxLength = 255; 
_LIT( KListNameAllBuddy ,"buddylist" );

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      CVIMPSTEnginePresenceSubService_Stub()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService_Stub::CVIMPSTEnginePresenceSubService_Stub(  ) 
        {
        }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::ConstructL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService_Stub::ConstructL()
    {
   

    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService_Stub* 
CVIMPSTEnginePresenceSubService_Stub::NewL(  )
    {
    CVIMPSTEnginePresenceSubService_Stub* self = NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService_Stub* 
CVIMPSTEnginePresenceSubService_Stub::NewLC( )
    {
    CVIMPSTEnginePresenceSubService_Stub* self =
    new (ELeave) CVIMPSTEnginePresenceSubService_Stub( );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      ~CVIMPSTEnginePresenceSubService_Stub()
// ---------------------------------------------------------------------------
//
CVIMPSTEnginePresenceSubService_Stub::~CVIMPSTEnginePresenceSubService_Stub()
    {
   
    }


 void CVIMPSTEnginePresenceSubService_Stub::FetchPresenceOfSingleContactL(const TDesC& aContact)
    {
    
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      ServiceState()
// ---------------------------------------------------------------------------
//

TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEnginePresenceSubService_Stub::SubServiceState() const
	{
	return TVIMPSTEnums::ESVCERegistered;
	}

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      Type()
// ---------------------------------------------------------------------------
//

TVIMPSTEnums::SubServiceType CVIMPSTEnginePresenceSubService_Stub::Type() const
    {		    	
	return TVIMPSTEnums::EPresence;
	}

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      Enabled()
// ---------------------------------------------------------------------------
//

TBool CVIMPSTEnginePresenceSubService_Stub::Enabled()
    {
    return  ETrue;    
    }



// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::SubscribePresenceOfSingleContactL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService_Stub::SubscribePresenceOfSingleContactL( const TDesC& /*aUriOfTheContact*/)
    {
    
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      UnsubscribePrecenseOfSingleContactL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService_Stub::UnSubscribePresenceOfSingleContactL(
        const TDesC& /*aUriOfTheContact */)
    {
   
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::PublishOwnPresenceL
// 
// ---------------------------------------------------------    
TInt CVIMPSTEnginePresenceSubService_Stub::PublishOwnPresenceL(TVIMPSTEnums::TOnlineStatus /*aStatus*/, 
														  const TDesC& /*aValue*/,
						         						  const TDesC&  /*aFilename= KNullDesC*/, 
						         						  const TDesC8& /*aMimetype = KNullDesC8*/,
						         						  TBool /*aIsAvatar = EFalse*/ )
    {
	return KErrNone;
    }


/// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::FetchPresenceFormCache
// 
// ---------------------------------------------------------    
TInt CVIMPSTEnginePresenceSubService_Stub::FetchPresenceFromCache()
    {
   
	return KErrNone;
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::SubscribeToPresenceCacheL
// ---------------------------------------------------------------------------
//  
void CVIMPSTEnginePresenceSubService_Stub::SubscribeToPresenceCacheL(const TDesC& /*aUserId*/ )
    {
                                              
    }


// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::UnSubscribeToPresenceCacheL
// ---------------------------------------------------------------------------
//  
void CVIMPSTEnginePresenceSubService_Stub::UnSubscribeToPresenceCacheL(const TDesC& /*aWVID*/)
    {
                                           
    }





// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::UnSubscribeForAuthorizationL
//  
// ---------------------------------------------------------    
void  CVIMPSTEnginePresenceSubService_Stub::UnSubscribeForAuthorizationL()
    {
        }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::SubscribeForAuthorizationL
//  
// ---------------------------------------------------------    
void  CVIMPSTEnginePresenceSubService_Stub::SubscribeForAuthorizationL()
    {
       }
// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::SendPresenceGrantPresentityResponseL
//  
// ---------------------------------------------------------    
TInt CVIMPSTEnginePresenceSubService_Stub::SendPresenceGrantPresentityL( const TDesC& /*aContactId*/ , TBool /*aResponse*/ )
    {
    return KErrNone;
    }  

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::IsChangeOwnStatusSupported
// 
// ---------------------------------------------------------        

TBool CVIMPSTEnginePresenceSubService_Stub::IsChangeOwnStatusSupported() 
    {
    //TODO:: get the feature supported from ximp and return
    return ETrue;
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::IsStatusMsgSupported
// 
// ---------------------------------------------------------        
TBool CVIMPSTEnginePresenceSubService_Stub::IsStatusMsgSupported()
    {
    //TODO:: get the feature supported from ximp and return
    return ETrue;
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::IsAvatarSupported
// 
// ---------------------------------------------------------        
TBool CVIMPSTEnginePresenceSubService_Stub::IsAvatarSupported()
    {
    return EFalse;
    }
 
 // ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::SetAvatarSupported
// 
// ---------------------------------------------------------        
void CVIMPSTEnginePresenceSubService_Stub::SetAvatarSupported(TBool /*aSupported*/ )
    {
   
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::RegisterPresenceEventObserver
// 
// ---------------------------------------------------------	
//TODO::Should be named as RegisterPresenceEventObserverL
void CVIMPSTEnginePresenceSubService_Stub::RegisterPresenceEventObserverL(
        MVIMPSTEnginePresenceSubServiceEventObserver* /*aObserver*/)
    {
    
    }

// ---------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::UnRegisterPresenceEventObserver
// 
// ---------------------------------------------------------	

void CVIMPSTEnginePresenceSubService_Stub::UnRegisterPresenceEventObserver(
        MVIMPSTEnginePresenceSubServiceEventObserver* /*aObserver*/)
    {
    
    }   
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::AddToBlockListL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEnginePresenceSubService_Stub::AddToBlockListL( const TDesC& aUriOfTheContact )
    {
    TInt err = KErrNotFound;
   _LIT(KAt, "@");
   TInt loc = aUriOfTheContact.Find(KAt());
   if( 0 == aUriOfTheContact.Length())
       return KErrNotFound;
   
   else if(KErrNotFound == loc )
      return KErrNotFound;
   else
       err = KErrNone;
    return err;
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::GetBlockedList
// ---------------------------------------------------------------------------
//
RPointerArray<HBufC>* CVIMPSTEnginePresenceSubService_Stub::GetBlockedList()
    {
    RPointerArray<HBufC> *blockedlist =  new (ELeave) RPointerArray<HBufC>;
    
    _LIT(KText,"blocked@presence");
        HBufC* buddyId = HBufC::NewL(32);
        CleanupStack::PushL(buddyId);
        *buddyId = KText;
        blockedlist->Append(buddyId);

        CleanupStack::Pop();
      
    return blockedlist;
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::FetchBlockedListFromServerL
// ---------------------------------------------------------------------------
//
void CVIMPSTEnginePresenceSubService_Stub::FetchBlockedListFromServerL(MVIMPSTEngineBlockedListFetchEventObserver* aOb)
    {
    
    }
// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::RemoveFromBlockListL
// ---------------------------------------------------------------------------
//
TInt CVIMPSTEnginePresenceSubService_Stub::RemoveFromBlockListL( const TDesC& aUriOfTheContact )
    {
    TInt err = KErrNotFound;
    _LIT(KAt, "@");
    TInt loc = aUriOfTheContact.Find(KAt());
    if( 0 == aUriOfTheContact.Length())
      return KErrNotFound;
   else if(KErrNotFound == loc )
       return KErrNotFound;
   else
       err = KErrNone;
     return err;
    }
 void CVIMPSTEnginePresenceSubService_Stub::FetchPresenceOfSinleContactL(const TDesC& aUserId)
	 {
	 }
 
 void CVIMPSTEnginePresenceSubService_Stub::UpdatePresenceStateL()
	 {
	 	
	 }
 
 void CVIMPSTEnginePresenceSubService_Stub::UnsubscribeListsL()
      {
         
      }

// End of file
