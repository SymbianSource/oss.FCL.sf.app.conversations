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
* Description: implementation for CVIMPSTCmdHandler  
*
*/


// INCLUDE FILES
#include "cvimpstcmdhandler.h"
#include "cvimpstcmdfactory.h"

#include "mvimpstcmd.h"
#include "mvimpstengine.h"


// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::CVIMPSTCmdHandler
// --------------------------------------------------------------------------
//
CVIMPSTCmdHandler::CVIMPSTCmdHandler()
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::~CVIMPSTCmdHandler
// --------------------------------------------------------------------------
//
CVIMPSTCmdHandler::~CVIMPSTCmdHandler()
    {
    delete iCommandFactory;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CVIMPSTCmdHandler* CVIMPSTCmdHandler::NewL(MVIMPSTEngine& aEngine)
    {
    CVIMPSTCmdHandler* self = new(ELeave) CVIMPSTCmdHandler();
    CleanupStack::PushL(self);
    self->ConstructL(aEngine);
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::ConstructL
// --------------------------------------------------------------------------
//
inline void CVIMPSTCmdHandler::ConstructL(MVIMPSTEngine& aEngine)
    {
    iCommandFactory = CVIMPSTCmdFactory::NewL(aEngine);
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::HandleCommandL
// --------------------------------------------------------------------------
//
TBool CVIMPSTCmdHandler::HandleCommandL
        ( const TImCommandId aCommandId, MVIMPSTCmdObserver* aObserver, 
          TAny* aData )
    {
    TBool ret = EFalse;

	//use the variable aData to set the command related data
    MVIMPSTCmd* cmd = iCommandFactory->CreateCommandForIdL(
            aCommandId,aData);
    
    if (cmd)
	    {
	    cmd->AddObserver(*aObserver);
	    TRAPD (err, cmd->ExecuteLD());
	    User::LeaveIfError(err);
	    ret = ETrue;
	    }

    return ret;
    }
 // --------------------------------------------------------------------------
// CVIMPSTCmdHandler::RegisterEventObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdHandler::RegisterEventObserverL( MVIMPSTCmdObserver* aObserver ) 
	{
	iCommandFactory->AddObserverL(aObserver);
	
	}
// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::UnRegisterEventObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdHandler::UnRegisterEventObserver( MVIMPSTCmdObserver* aObserver ) 
	{
	iCommandFactory->RemoveObserver(aObserver);
	}
// --------------------------------------------------------------------------
// CVIMPSTCmdHandler::GetProcessInterface
// --------------------------------------------------------------------------
//
MVIMPSTProcessArray& CVIMPSTCmdHandler::GetProcessInterface() 
	{
	return iCommandFactory->GetProcessInterface(); 	
	}
	
	
// End of File
