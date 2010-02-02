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
* Description:  utils class for engine
*
*/

#include "imcvenginemessageutils.h"

#include <e32std.h>
#include <StringLoader.h>
#include "mimcvenginemessage.h"
#include "mimcvenginemessagecreator.h"
#include "mimcvenginemessageswriteinterface.h"
#include <avkon.rsg>
#include "imcvuiliterals.h"
#include "imcvlogger.h"
#include <StringLoader.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// IMCVEngineMessageUtils::AppendDateStampL
// ---------------------------------------------------------------------------
//
void IMCVEngineMessageUtils::AppendDateStampL( 
                          TTime /*aNewTime*/,
                          MIMCVEngineMessageWriteInterface& aWriteInterface,
                          MIMCVEngineMessageCreator& /*aMessageCreator */)
    {
    // DateStamp is NOT appended if:
    // 1. There are no previous messages
    // 2. Previous message is from the same day as aNewTime
    IM_CV_LOGS(TXT("IMCVEngineMessageUtils::AppendDateStampL() start") );
   
    TTime lastTime = aWriteInterface.Time();
    
    if( lastTime == Time::NullTTime() )
        {
        // no previous messages
        return;
        }
   else
       {
       	// implementation needed
       }
    IM_CV_LOGS(TXT("IMCVEngineMessageUtils::AppendDateStampL() end") );
    }


// ---------------------------------------------------------------------------
// CIMCVEngineMessageUtils::AppendMessageWithDateStampL
// ---------------------------------------------------------------------------
//
void IMCVEngineMessageUtils::AppendMessageWithDateStampL( 
                              MIMCVEngineMessage& aNewMessage,
                              MIMCVEngineMessageWriteInterface& aWriteInterface,
                              MIMCVEngineMessageCreator& aMessageCreator,
                              TBool aSharedOwnership )
    {
    // Make sure this does not leave before the new message is
    // appended to interface,
    IM_CV_LOGS(TXT("IMCVEngineMessageUtils::AppendMessageWithDateStampL() start") );
    TRAP_IGNORE( AppendDateStampL( aNewMessage.TimeStamp(), 
                                   aWriteInterface, 
                                   aMessageCreator ) );
    aWriteInterface.AppendL( &aNewMessage, aSharedOwnership );
    IM_CV_LOGS(TXT("IMCVEngineMessageUtils::AppendMessageWithDateStampL() end") );
    }

// -----------------------------------------------------------------------------
// IMCVEngineMessageUtils::LoadResourceL
// give ownership to caller
// -----------------------------------------------------------------------------
// 
HBufC* IMCVEngineMessageUtils::LoadResourceL( TInt aResourceId, const TDesC& aText )
	{
	CCoeEnv* env = CCoeEnv::Static();
	HBufC* ret = NULL;
	if(env)
		{
		ret = StringLoader::LoadL( aResourceId, aText  );
		}
	return ret;
	}
	
// End of file
