/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  this class is the implementaiton of this plugin
*
*/
//  Include Files  
					
#include "instantmsgindicatorpluginimplementation.h"
#include <AknSmallIndicator.h> //Akn indicator
#include <avkon.hrh>
#include <e32property.h>
#include "cimalertmanager.h"

//  Member Functions

// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::CInstantMsgIndicatorPluginImplementation()
// ---------------------------------------------------------------------------
//
CInstantMsgIndicatorPluginImplementation* CInstantMsgIndicatorPluginImplementation::NewLC()
    {
    CInstantMsgIndicatorPluginImplementation* self = new (ELeave) CInstantMsgIndicatorPluginImplementation();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::CInstantMsgIndicatorPluginImplementation()
// ---------------------------------------------------------------------------
//
 CInstantMsgIndicatorPluginImplementation* CInstantMsgIndicatorPluginImplementation::NewL()
    {
    CInstantMsgIndicatorPluginImplementation* self = CInstantMsgIndicatorPluginImplementation::NewLC(  );
    CleanupStack::Pop(self);
    return self;
    }
 // ---------------------------------------------------------------------------
 // CInstantMsgIndicatorPluginImplementation::CInstantMsgIndicatorPluginImplementation()
 // ---------------------------------------------------------------------------
 //
CInstantMsgIndicatorPluginImplementation::CInstantMsgIndicatorPluginImplementation( )
	{
    }
// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::ConstructL()
// ---------------------------------------------------------------------------
//
void CInstantMsgIndicatorPluginImplementation::ConstructL()
    {
    iIndicator = CAknSmallIndicator::NewL( TUid::Uid( EAknIndicatorMecoServiceTab ) );
    //define the properties for publishing 
    // msg count, serive id, sender id, receiver id
    
    // define first property to be integer type
    TInt result = RProperty::Define(KIMStatusProperty,EIMStatusPropertyMessageCount,RProperty::EInt);
    if (result != KErrAlreadyExists)
        User::LeaveIfError(result);
    
    // define first property to be integer type
    result = RProperty::Define(KIMStatusProperty,EIMStatusPropertyServiceId,RProperty::EInt);
    if (result != KErrAlreadyExists)
        User::LeaveIfError(result);
    
    result = RProperty::Define(KIMStatusProperty,EIMMultipleSender,RProperty::EInt);
       if (result != KErrAlreadyExists)
           User::LeaveIfError(result);
    
    // define first property to be integer type
    result = RProperty::Define(KIMStatusProperty,EIMStatusPropertySenderId,RProperty::EText);
    if (result != KErrAlreadyExists)
        User::LeaveIfError(result);
    
    iIMAlertMgr = CIMAlertManager::NewL();
    }

// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::~CInstantMsgIndicatorPluginImplementation
// ---------------------------------------------------------------------------
//
CInstantMsgIndicatorPluginImplementation::~CInstantMsgIndicatorPluginImplementation()
    {
    //Make the IM Indicator invisible here
    if(iIndicator)
       {
       // using TRAP_IGNORE as we cannot call leaving methods in the destructor.
        TRAP_IGNORE(iIndicator->SetIndicatorStateL(EAknIndicatorStateOff));
        delete iIndicator;
        iIndicator = NULL;
        }
    // delete the earlier defined property
    RProperty::Delete(KIMStatusProperty,EIMStatusPropertyMessageCount);
    
    // delete the earlier defined property
    RProperty::Delete(KIMStatusProperty,EIMStatusPropertyServiceId);
  
    // delete the earlier defined property
    RProperty::Delete(KIMStatusProperty,EIMMultipleSender);
      
    // delete the earlier defined property
    RProperty::Delete(KIMStatusProperty,EIMStatusPropertySenderId);
    }


// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::ShowIMIndicator
// ---------------------------------------------------------------------------
//
void CInstantMsgIndicatorPluginImplementation::ShowIMIndicatorL(TInt aNewMessages, TInt aSeriveId,
                                                                const TDesC& aSenderId ,
                                                                TInt aMultipleSender)
    {
    if(iIndicator)
        {
        if(aNewMessages>0)
            {
            TInt result = RProperty::Set(KIMStatusProperty,EIMStatusPropertyMessageCount,aNewMessages);
            User::LeaveIfError(result);

            result = RProperty::Set(KIMStatusProperty,EIMStatusPropertyServiceId,aSeriveId);
            User::LeaveIfError(result);
            
            result = RProperty::Set(KIMStatusProperty,EIMMultipleSender,aMultipleSender);
            User::LeaveIfError(result);
            
            result = RProperty::Set(KIMStatusProperty,EIMStatusPropertySenderId,aSenderId);
            User::LeaveIfError(result);

            iIndicator->SetIndicatorStateL( EAknIndicatorStateOn );  
            }
        else
            {
            TInt result = RProperty::Set(KIMStatusProperty,EIMStatusPropertyMessageCount,aNewMessages);
            User::LeaveIfError(result);

            result = RProperty::Set(KIMStatusProperty,EIMStatusPropertyServiceId,aSeriveId);
            User::LeaveIfError(result);
           
            result = RProperty::Set(KIMStatusProperty,EIMMultipleSender,aMultipleSender);
                      User::LeaveIfError(result);
           
           result = RProperty::Set(KIMStatusProperty,EIMStatusPropertySenderId,aSenderId);
           User::LeaveIfError(result);
           iIndicator->SetIndicatorStateL( EAknIndicatorStateOff ); 
           }
       }

    }

// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::PlayMsgToneL
// ---------------------------------------------------------------------------
//
void CInstantMsgIndicatorPluginImplementation::PlayMsgToneL(TInt aNewMessages,TInt aSeriveId)
    {
    if(aNewMessages>0 && iIMAlertMgr)
        {
        iIMAlertMgr->PlayL(aSeriveId);
        }
    }

// ---------------------------------------------------------------------------
// CInstantMsgIndicatorPluginImplementation::MessageInfoL
// ---------------------------------------------------------------------------
//
 void CInstantMsgIndicatorPluginImplementation::MessageInfoL(TInt aNewMessages, TInt aSeriveId,
                                                         const TDesC& aSenderId ,
                                                         TInt aMultipleSender)
    {
    
    ShowIMIndicatorL(aNewMessages, aSeriveId,aSenderId, aMultipleSender);
    PlayMsgToneL(aNewMessages,aSeriveId);
  	}

//end of file


