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
#include "s_imsubservice.h"

//Presence Observer
//#include "mvimpstenginepresencesubserviceeventobserver.h"



// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      CVIMPSTEnginePresenceSubService_Stub()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineImSubService_Stub::CVIMPSTEngineImSubService_Stub(  ) 
        {
        }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::ConstructL()
// ---------------------------------------------------------------------------
//
void CVIMPSTEngineImSubService_Stub::ConstructL()
    {
   

    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::NewL()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineImSubService_Stub* 
CVIMPSTEngineImSubService_Stub::NewL(  )
    {
    CVIMPSTEngineImSubService_Stub* self = NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::NewLC()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineImSubService_Stub* 
CVIMPSTEngineImSubService_Stub::NewLC( )
    {
    CVIMPSTEngineImSubService_Stub* self =
    new (ELeave) CVIMPSTEngineImSubService_Stub( );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      ~CVIMPSTEnginePresenceSubService_Stub()
// ---------------------------------------------------------------------------
//
CVIMPSTEngineImSubService_Stub::~CVIMPSTEngineImSubService_Stub()
    {
   
    }


TVIMPSTEnums::TVIMPSTRegistrationState CVIMPSTEngineImSubService_Stub::SubServiceState() const
    {
    return TVIMPSTEnums::ESVCERegistered;
    }

// ---------------------------------------------------------------------------
// CVIMPSTEnginePresenceSubService_Stub::
//      Type()
// ---------------------------------------------------------------------------
//

TVIMPSTEnums::SubServiceType CVIMPSTEngineImSubService_Stub::Type() const
    {               
    return TVIMPSTEnums::EPresence;
    } 



   

        /**
        * @see MVIMPSTIMSubService
        */
        void CVIMPSTEngineImSubService_Stub::RegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver)
            {
            
            }

        /**
        * @see MVIMPSTIMSubService
        */
        void CVIMPSTEngineImSubService_Stub::UnRegisterChatObserver(MVIMPSTEngineIMSubServiceEventObserver* aObserver)
            {
            
            }

        /**
        * @see MVIMPSTIMSubService
        */
        TInt CVIMPSTEngineImSubService_Stub::GetUnreadCountL(const TDesC& aSender)
        {
        return 0;
        }
        


        /**
        * @see MVIMPSTIMSubService
        */
        TBool CVIMPSTEngineImSubService_Stub::IsConversationExistL(const TDesC& aRecipientId) const 
        {
        if(aRecipientId.Length())
            return ETrue;
        else
            return EFalse;
        }

        /**
        * @see MVIMPSTIMSubService
        */
        void CVIMPSTEngineImSubService_Stub::CloseConversationL( const TDesC& aContactId )
        {
        
        }
        
        /**
        * @see MVIMPSTIMSubService
        */
        
        RArray<SIMCacheChatItem>CVIMPSTEngineImSubService_Stub::GetOpenChatListL()
            {

            RArray<SIMCacheChatItem> item;
            return item;
            }
// End of file
