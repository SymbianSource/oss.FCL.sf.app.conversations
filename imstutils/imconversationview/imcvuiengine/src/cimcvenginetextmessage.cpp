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
* Description:  text message implementation 
*
*/

#include "cimcvenginetextmessage.h"
#include <StringLoader.h>
#include "imcvenginemessageutils.h"
#include <imcvuiapp.rsg>
// logs
#include "imcvlogger.h"

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::CIMCVEngineTextMessage
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineTextMessage::CIMCVEngineTextMessage()
	{
	}

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::~CIMCVEngineTextMessage
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineTextMessage::~CIMCVEngineTextMessage()
	{
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineTextMessage* CIMCVEngineTextMessage::NewL( 
                                        const TDesC& aSender, 
                                        const TDesC& aRecipient, 
                                        const TDesC& aData )
	{
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::NewL() start") );
	CIMCVEngineTextMessage* self = CIMCVEngineTextMessage::NewLC(
															 aSender, 
															 aRecipient, 
															 aData );
	CleanupStack::Pop( self );
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::NewL() end") );
	return self;
	}
	

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::NewLC
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineTextMessage* CIMCVEngineTextMessage::NewLC( 
                                        const TDesC& aSender, 
                                        const TDesC& aRecipient, 
                                        const TDesC& aData )
	{
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::NewLC() start") );
	CIMCVEngineTextMessage* self = new ( ELeave ) CIMCVEngineTextMessage();
	CleanupStack::PushL( self );
	self->ConstructL( aData );
	self->CIMCVEngineMessageBase::ConstructL( 
										aSender, 
										aRecipient
										 );
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::NewLC() end") );
	return self;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineTextMessage::ConstructL( const TDesC& aData )
	{
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::ConstructL() start") );
	
	iText = aData.AllocL();
	
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::ConstructL() end") );
	}	

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::SizeInBytes
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TInt CIMCVEngineTextMessage::SizeInBytes() const
	{
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::SizeInBytes() start") );
	IM_CV_LOGS(TXT("CIMCVEngineTextMessage::SizeInBytes() end") );
	return  CIMCVEngineMessageBase::SizeInBytes() + 
	        iText->Size() +
	        sizeof( *this );
	}

//-----------------------------------------------------------------------------
// CIMCVEngineTextMessage::ContentType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage::TContentType CIMCVEngineTextMessage::ContentType() const
	{
	return EContentText;
	}

//-----------------------------------------------------------------------------
// CCATextMessage::SetFailed
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineTextMessage::SetFailedL()
    {
    IM_CV_LOGS(TXT("CIMCVEngineTextMessage::SetFailedL() start") );
    SetFailed();
    HBufC* failed = NULL;
    if( iText )
        {
        failed = IMCVEngineMessageUtils::LoadResourceL( R_QTN_CONVIEW_FAILED, *iText);
        delete iText;
        iText = NULL;
        iText = failed;
        }
    IM_CV_LOGS(TXT("CIMCVEngineTextMessage::SetFailedL() end") );
    }
   
// end of file
