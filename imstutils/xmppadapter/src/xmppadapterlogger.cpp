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
* Description:  XMPP Logger implementation
*
*/

// INCLUDE FILES

#include <e32svr.h>
#include <e32std.h>
#include <flogger.h>

#include "xmppadapterlogger.h"


/**
 * Handler used by logger to truncate the string
 * rather than panic in case of buffer overflow.
*/

NONSHARABLE_CLASS ( TAdaptOverflowTruncate ) : public TDes16Overflow
	{

public:
	void Overflow ( TDes16& /*aDes*/ ) {}
	};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TLogger::WriteLog()
// -----------------------------------------------------------------------------
//
void TLogger::WriteLog ( TRefByValue<const TDesC> aFmt, ... )
	{
#ifdef ENABLE_DEBUG_LOGS
	( void ) aFmt;//Suppress unused formal parameter warning
	TBuf< KTAdaptMaxLogLineLength > buffer;
	buffer.Append ( _L ( "[" ) );           // CSI: 78 #
	buffer.Append ( RThread().Name() );
	buffer.Append ( _L ( "] " ) );          // CSI: 78 #
	TAdaptOverflowTruncate overflow;
	VA_LIST list;
	VA_START ( list, aFmt );
	buffer.AppendFormatList ( aFmt, list, &overflow );
	RFileLogger logger;

	if ( logger.Connect() == KErrNone )
		{
		logger.SetDateAndTime ( ETrue, ETrue );
		logger.CreateLog ( KTAdaptDebugOutputDir, KTAdaptDebugOutputFileName,
		                   EFileLoggingModeAppend );
		logger.Write ( buffer );
		logger.CloseLog();
		logger.Close();
		}

#endif

	}

// End of File


