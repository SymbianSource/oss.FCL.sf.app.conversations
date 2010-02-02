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
* Description:  Base class for message
*
*/

#include "cimcvenginemessagebase.h"
#include "mimcvenginemessagecontainerinfo.h"
#include "imcvuiliterals.h"
#include <StringLoader.h>
#include <avkon.rsg>
#include <txtetext.h>                    
#include "imcvlogger.h"                  

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::CIMCVEngineMessageBase
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageBase::CIMCVEngineMessageBase()
	{
	}
	
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageBase::ConstructL(const TDesC& aSender, 
                                 const TDesC& aRecipient )
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::ConstructL() start") );
       
    iSender = aSender.AllocL();
    iRecipient = aRecipient.AllocL();
    
           
    iMessagerType = EMessageOther;
    iSystemMessageType = ESystemMessageNone;
    iTime.HomeTime();
    iMessageVersion = KVersionNum;
    iMessageType = EMessagePTOP;
    
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::ConstructL() end") );
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::~CIMCVEngineMessageBase
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCVEngineMessageBase::~CIMCVEngineMessageBase()
	{
	//__CHAT_ASSERT_DEBUG( iOwnerCount == 0 );
	delete iSender;
	delete iRecipient;
	delete iText;	
	}
	
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::TimeStamp
// ( Other items commented in header )
//-----------------------------------------------------------------------------
const TTime& CIMCVEngineMessageBase::TimeStamp() const
	{
	return iTime;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SetTimeStamp
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCVEngineMessageBase::SetTimeStamp(TTime aTime ) 
	{
	 iTime = aTime ;
	}
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::MessageType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage::TMessageType CIMCVEngineMessageBase::MessageType() const
	{
	return iMessageType;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::MessagerType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
MIMCVEngineMessage::TMessagerType CIMCVEngineMessageBase::MessagerType() const
	{
	return iMessagerType;
	}

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::OperationCode
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TXIMPRequestId CIMCVEngineMessageBase::OperationCode() const
    {
    return iOperationCode;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::Recipient
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageBase::SetOperationCode(TXIMPRequestId aOperationId )
    {
    iOperationCode = aOperationId;
    }

//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::Recipient
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
const TDesC& CIMCVEngineMessageBase::Recipient() const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Recipient() start") );
    if( iRecipient )
        {
        return *iRecipient;
        }
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Recipient() end") );
    return KNullDesC;
    }

   
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SetMessageContainerInfo
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageBase::SetContainerInfo( MIMCVEngineMessageContainerInfo* aInfo )
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetContainerInfo() start") );
    iInfo = aInfo;
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetContainerInfo() end") );
    }
 
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::Text
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
const TDesC& CIMCVEngineMessageBase::Text() const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Text() start") );
    if( iText )
        {
        return *iText;
        }
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Text() end") );
    return KNullDesC;
    }
    
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::Sender
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
const TDesC& CIMCVEngineMessageBase::Sender() const
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Sender() start") );
    if( iSender )
        {
        return *iSender;
        }
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::Sender() end") );
    return KNullDesC;
    }    
        
//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SizeInBytes
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TInt CIMCVEngineMessageBase::SizeInBytes() const
    {
    return  iSender->Size() + 
            iRecipient->Size();
    }    
    
// ---------------------------------------------------------
// CIMCVEngineMessageBase::FailedMessage()
// ---------------------------------------------------------
//
TBool CIMCVEngineMessageBase::FailedMessage() const
    {
    return iFailed;
    }
    
// ---------------------------------------------------------
// CIMCVEngineMessageBase::FailedMessage()
// ---------------------------------------------------------
//
void CIMCVEngineMessageBase::SetFailed( TBool aFailed /* = ETrue */  ) 
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetFailed() start") );
    iFailed = aFailed;
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetFailed() end") );
    }
    

// -----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SetMessageType
// -----------------------------------------------------------------------------
//
void CIMCVEngineMessageBase::SetMessageType( TMessageType aNewType )
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetMessageType() start") );
    iMessageType = aNewType;
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetMessageType() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SetMessagerType
// -----------------------------------------------------------------------------
//
void CIMCVEngineMessageBase::SetMessagerType( TMessagerType aNewType )
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetMessagerType() start") );
    iMessagerType = aNewType;
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetMessagerType() end") );
    }
    

// -----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SetSystem
// -----------------------------------------------------------------------------
//
void CIMCVEngineMessageBase::SetSystem( TSystemMessage aType )
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetSystem() start") );
    iSystemMessageType = aType;
    iMessagerType = EMessageSent;
    iMessageType = EMessageSystem;
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SetSystem() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVEngineMessageBase::SystemMessageType
// -----------------------------------------------------------------------------
//
MIMCVEngineMessage::TSystemMessage CIMCVEngineMessageBase::SystemMessageType()
    {
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SystemMessageType() start") );
    if( iMessageType != EMessageSystem )
        {
        // not a system message
        return ESystemMessageNone;
        }
    IM_CV_LOGS(TXT("CIMCVEngineMessageBase::SystemMessageType() end") );
    return iSystemMessageType;
    }


//-----------------------------------------------------------------------------
// CIMCVEngineMessageBase::IncreaseOwnerCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCVEngineMessageBase::IncreaseOwnerCount()
    {
    ++iOwnerCount;
    }

//-----------------------------------------------------------------------------
// CCAMessageBase::DecreaseOwnerCount
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TBool CIMCVEngineMessageBase::DecreaseOwnerCount()
    {
    return ( --iOwnerCount <= 0 );
    }
    
    
// end of file
