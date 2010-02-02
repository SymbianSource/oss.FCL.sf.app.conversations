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
* Description:  Logger implementation
*
*/




#ifndef IMCVLOGGER_H
#define IMCVLOGGER_H

#include <e32svr.h>
#include <flogger.h>
#include <e32std.h>

// enable logs printing
// into  c:\\logs\\imcv\\imcv.txt
#define ENABLE_DEBUG_LOGS

/**   
* Usage of Log MACRO'S
* _LIT( KExample, "Example" );
*  TXT(s) _L(s)	
*  IM_CV_LOGS(TXT("Some text.") );
*  IM_CV_LOGS(TXT("Some text: %d"), 100 );
*  IM_CV_LOGS(TXT("Some text: %S"), &KExample );
*/

_LIT( KTAdaptDebugOutputDir, "imcv" );
_LIT( KTAdaptDebugOutputFileName, "imcv.txt" );
const TInt KTAdaptMaxLogLineLength = 250 ;
#define TXT(s) _L(s)
#define IM_CV_LOGS TIMCVLogger::WriteLog
#define PLUGIN_UNUSED_PARAM(p) (void) p


/**
 * IM Conversation View logger.
 */
class TIMCVLogger
    {
    public: //Logging functions
    	/*
        * WriteLog, write the message into c:\\logs\\imcv\\imcv.txt
        * need to create imcv folder into c:\\logs
        * @param aFmt, list of messges to print
        */
		static void WriteLog( TRefByValue<const TDesC> aFmt,... );
		
    private: //Prohibited
    	/*
        * construtor
        */
        TIMCVLogger();
        /*
        * destructor
        */
        ~TIMCVLogger();
    };

#endif // IMCVLOGGER_H



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
// TIMCVLogger::WriteLog()
// -----------------------------------------------------------------------------
//
inline void TIMCVLogger::WriteLog ( TRefByValue<const TDesC> aFmt, ... )
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

		RDebug::Print( _L("%S"), &buffer );
	#endif

	}


// End of File

 

