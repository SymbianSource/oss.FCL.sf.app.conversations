/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  message creater class implementation
*
*/


#include "cimcvenginemessagecreator.h"
#include "cimcvenginetextmessage.h"
#include "imcvuiliterals.h"
#include "imcvlogger.h"

//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::CIMCVEngineMessageCreator
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageCreator::CIMCVEngineMessageCreator()
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CIMCVEngineMessageCreator() start") );
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CIMCVEngineMessageCreator() end") );
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::~CIMCVEngineMessageCreator
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageCreator::~CIMCVEngineMessageCreator()
	{
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageCreator* CIMCVEngineMessageCreator::NewL()
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::NewL() start") );
	CIMCVEngineMessageCreator* self = new ( ELeave ) CIMCVEngineMessageCreator();
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::NewL() end") );
	return self;
	}

	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::CreateMessageL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessage* CIMCVEngineMessageCreator::CreateMessageL( const SMessageData& aData ) const
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateMessageL() start") );
	if( aData.iContentType == MIMCVEngineMessage::EContentText ) // Text message
		{
		return CreateTextMessageL( aData );
		}
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateMessageL() end") );
	return NULL;
	}
	
 
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::CreateTextMessageL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessage* CIMCVEngineMessageCreator::CreateTextMessageL( 
                                        const SMessageData& aData ) const
	{
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateTextMessageL() start") );
	CIMCVEngineTextMessage* message = CIMCVEngineTextMessage::NewL( 
	                                                aData.iSenderId, 
                                                    aData.iTargetId,
	                                                aData.iText );
	message->SetMessagerType( aData.iMessager );
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateTextMessageL() end") );
	return message;
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageCreator::CreateFailedMessageL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
MIMCVEngineMessage* CIMCVEngineMessageCreator::CreateFailedMessageL( 
                                        const MIMCVEngineMessage* pSource ) const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateFailedMessageL() start") );
    CIMCVEngineTextMessage* message = CIMCVEngineTextMessage::NewLC(
	                                                 pSource->Sender(), 
	                                                 pSource->Recipient(),
	                                                 pSource->Text() );
    message->SetFailedL();
    CleanupStack::Pop( message );
	message->SetMessagerType( pSource->MessagerType() );
	IM_CV_LOGS(TXT("CIMCVEngineMessageCreator::CreateFailedMessageL() end") );
	
	return message;
    }
    	


// end of file
