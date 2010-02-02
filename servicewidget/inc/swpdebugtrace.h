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
* Description:  logger 
*
*/



#ifndef __SWPDEBUGTRACE_H__
#define __SWPDEBUGTRACE_H__

//  INCLUDES
#include  "e32std.h"
#include <flogger.h>
//  DEFINES
// enable logs printing
// into  c:\\logs\\swp\\swp.txt
#define SWP_ENABLE_DEBUG_LOGS

/**   
* Usage of Log MACRO'S
* _LIT( KExample, "Example" );
*  TXT(s) _L(s)	
*  TRACE_SWP(TXT("Some text.") );
*  TRACE_SWP(TXT("Some text: %d"), 100 );
*  TRACE_SWP(TXT("Some text: %S"), &KExample );
*/

_LIT( KTAdaptDebugOutputDir, "swp" );
_LIT( KTAdaptDebugOutputFileName, "swp.txt" );
const TInt KTAdaptMaxLogLineLength = 250 ;
#define TXT(s) _L(s)
#define TRACE_SWP TSWPLogger::WriteLog
#define PLUGIN_UNUSED_PARAM(p) (void) p


/**
 * IM cache debug logger.
 */
class TSWPLogger
    {
    public: //Logging functions
    	/**
        * WriteLog, write the message into c:\\logs\\swp\\swp.txt
        * @param aFmt, list of messges to print
        */
		static void WriteLog( TRefByValue<const TDesC> aFmt,... );
		
    private: //Prohibited
    	/**
        * construtor
        */
        TSWPLogger();
        /**
        * destructor
        */
        ~TSWPLogger();
    };

#endif // __SWPDEBUGTRACE_H__



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
// TSWPLogger::WriteLog()
// -----------------------------------------------------------------------------
//
inline void TSWPLogger::WriteLog ( TRefByValue<const TDesC> aFmt, ... )
	{
	#ifdef SWP_ENABLE_DEBUG_LOGS
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

 
