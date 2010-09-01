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
* Description: trace logs
*
*/
 
#ifndef __VIMPSTDEBUGTRACE_H__
#define __VIMPSTDEBUGTRACE_H__

//  INCLUDES
#include  "e32std.h"
#include <flogger.h>
//  DEFINES
// enable logs printing
// into  c:\\logs\\vimpst\\vimpst.txt
//#define VIMPST_ENABLE_DEBUG_LOGS
#undef VIMPST_ENABLE_DEBUG_LOGS

/**   
* Usage of Log MACRO'S
* _LIT( KExample, "Example" );
*  TXT(s) _L(s)	
*  TRACE( T_LIT("Some text.") );
*  TRACE( T_LIT("Some text: %d"), 100 );
*  TRACE( T_LIT("Some text: %S"), &KExample );
*/

_LIT( KTAdaptDebugOutputDir, "vimpst" );
_LIT( KTAdaptDebugOutputFileName, "vimpst.txt" );
_LIT( KTAdaptDebugOutputDFileName, "dvimpst.txt" );
const TInt KTAdaptMaxLogLineLength = 250 ;
#define T_LIT(s) _L(s)
#define TRACE TVIMPSTLogger::WriteLog
#define TRACED TVIMPSTLogger::DWriteLog // TRACED must be used in details view plugin only
#define PLUGIN_UNUSED_PARAM(p) (void) p


/**
 * IM cache debug logger.
 */
class TVIMPSTLogger
    {
    public: //Logging functions
    	/**
        * WriteLog, write the message into c:\\logs\\vimpst\\vimpst.txt
        * need to create imcv folder into c:\\logs
        * @param aFmt, list of messges to print
        */
		static void WriteLog( TRefByValue<const TDesC> aFmt,... );
		
		/**
		 * WriteLog, write the message into c:\\logs\\vimpst\\vimpstd.txt
		 * need to create vimpst folder into c:\\logs
		 * only used for detailsview plugin
		 * @param aFmt, list of messges to print
		 */
		static void DWriteLog( TRefByValue<const TDesC> aFmt,... );

    private: //Prohibited
    	/**
        * construtor
        */
        TVIMPSTLogger();
        /**
        * destructor
        */
        ~TVIMPSTLogger();
    };

#endif // __VIMPSTDEBUGTRACE_H__

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
// TVIMPSTLogger::WriteLog()
// -----------------------------------------------------------------------------
//
inline void TVIMPSTLogger::WriteLog ( TRefByValue<const TDesC> aFmt, ... )
	{
	(void) aFmt;//Suppress unused formal parameter warning
	#ifdef VIMPST_ENABLE_DEBUG_LOGS
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

// -----------------------------------------------------------------------------
// TVIMPSTLogger::DWriteLog() 
// -----------------------------------------------------------------------------
//
inline void TVIMPSTLogger::DWriteLog ( TRefByValue<const TDesC> aFmt, ... )
    {
    (void) aFmt;//Suppress unused formal parameter warning
    #ifdef VIMPST_ENABLE_DEBUG_LOGS
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
        logger.CreateLog ( KTAdaptDebugOutputDir, KTAdaptDebugOutputDFileName,
                   EFileLoggingModeAppend );
        logger.Write ( buffer );
        logger.CloseLog();
        logger.Close();
        }
    #endif
    }
// End of File

 
