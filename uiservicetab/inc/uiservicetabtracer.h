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
* Description: trace logs
 *
*/

#ifndef UISERVICETABTRACER_H
#define UISERVICETABTRACER_H

#include <e32base.h>
#include <flogger.h>
#include "vimpstbuilddefinitions.h"

#ifdef CHAT_ENABLE_DEBUG_PRINT

// --------------------------------------------------------------------------
// Define tracer logging method
// 0    = Logging off
// 1    = Log to RDebug
// 2    = Log to file (RFileLogger)
// --------------------------------------------------------------------------
//

#ifdef __WINS__
  #ifdef _DEBUG
    #define TRACER_LOG_METHOD 1 // for WINS UDEB
  #else
    #define TRACER_LOG_METHOD 0 // for WINS UREL
 #endif
#else
  #define TRACER_LOG_METHOD 2 // for device
#endif

#endif

// --------------------------------------------------------------------------
// Whether to use TRAP_INSTRUMENTATIONs to
// log trapped leaves
// --------------------------------------------------------------------------
//
#define TRACE_TRAPS
//============================================================================

// Logging off, define empty macros and skip all the rest
#if TRACER_LOG_METHOD == 0

#define TRACER(func)
#define TRACER_AUTO
#define TRACER_RET(func,format)
#define TRACER_AUTO_RET(format)
#define TRACER_LEAVE(func)
#define TRACER_AUTO_LEAVE
#define TRACE(args...)
#define TRACE8(args...)
#define TRACER_TO16(a,b)
#define TRACER_TO8(a,b)
#define TRACER_CLOSELOG

#else  // Logging on

// --------------------------------------------------------------------------
// Change these to modify line syntax
// --------------------------------------------------------------------------
//
_LIT( KLogBase, "UiServiceTab %S(%d): %S");
_LIT( KLogEnter, "UiServiceTab %S: ENTER" );
_LIT( KLogExit, "UiServiceTab %S: EXIT" );
_LIT( KLogLeave, "UiServiceTab %S: LEAVE!" );
_LIT( KLogTrappedLeave, "UiServiceTab %S(%d): TRAPPED LEAVE, code %d!" );
_LIT( KLogExitRet, "UiServiceTab %S: EXIT, Returning " );
_LIT( KLogDir,  "UiServiceTab" ); 
_LIT( KLogFile, "UiServiceTab.log" );

#define TRACER_PARAMS_LENGTH 200

// --------------------------------------------------------------------------
// Macro to print only function leaves (not entries or exits). 
// Example: TRACER_LEAVE( "CMyClass::MyFunction" );
// --------------------------------------------------------------------------
//
#define TRACER_LEAVE(func) TTracer function_tracer( _S(func), _S(""), EFalse )

// --------------------------------------------------------------------------
// Same as above, except that function name is generated automatically
// Example: TRACER_AUTO_LEAVE;
// --------------------------------------------------------------------------
//
#define TRACER_AUTO_LEAVE TTracer \
    function_tracer( ((const TText8*)__PRETTY_FUNCTION__), _S(""), EFalse )

// --------------------------------------------------------------------------
// Macro to print function entry, exit and leave. 
// Example: TRACER( "CMyClass::MyFunction" );
// --------------------------------------------------------------------------
//
#define TRACER(func) TTracer function_tracer( _S(func), _S(""), ETrue )

// --------------------------------------------------------------------------
// Same as above, except that function name is generated automatically
// Example: TRACER_AUTO;
// --------------------------------------------------------------------------
//
#define TRACER_AUTO \
    TTracer function_tracer \
        ( ((const TText8*)__PRETTY_FUNCTION__), _S(""), ETrue )

// --------------------------------------------------------------------------
// Macro to print function return value in addition to entry, exit 
// and leave conditions. Second parameter is a formatting string used 
// to print the return value Example to print an integer return value:
// TRACER_RET("CMyclass::MyFunction", "%d");
// --------------------------------------------------------------------------
//
#define TRACER_RET(func,format) \
    TTracer function_tracer( _S(func), _S(format), ETrue )

// --------------------------------------------------------------------------
// Same as above, except that function name is generated automatically
// Example: TRACER_AUTO_RET( "%d" );
// --------------------------------------------------------------------------
//
#define TRACER_AUTO_RET(format) \
    TTracer function_tracer \
        ( ((const TText8*)__PRETTY_FUNCTION__), _S(format), ETrue )

// --------------------------------------------------------------------------
// Macro to print custom msg with any number of parameters
// Either TRACER, TRACER_RET or TRACER_LEAVE 
// must be called before in same scope
// It's is assumed that descriptor (pointer) parameters are 16-bit
// NOTE: It is also assumed that parameter values
// add no more than TRACE_PARAMS_LENGTH to the traced string
// --------------------------------------------------------------------------
//
#define TRACE( args... ) function_tracer.trace( __LINE__, ## args )

// --------------------------------------------------------------------------
// Macro to print custom msg with any number of parameters
// Either TRACER, TRACER_RET or TRACER_LEAVE 
// must be called before in same scope
// It's is assumed that descriptor (pointer) parameters are 8-bit
// NOTE: It is also assumed that parameter values 
// add no more than TRACE_PARAMS_LENGTH to the traced string
// --------------------------------------------------------------------------
//
#define TRACE8( args... ) function_tracer.trace8(__LINE__, ## args )

// --------------------------------------------------------------------------
// Just to quickly convert descriptors if needed
// --------------------------------------------------------------------------
//
#define DESCRIPTOR_LENGTH 100
#define TRACER_TO16( desc8, desc16 ) \
    TBuf<DESCRIPTOR_LENGTH> desc16;desc16.Copy(desc8) 
#define TRACER_TO8( desc16, desc8 ) \
    TBuf8<DESCRIPTOR_LENGTH> desc8;desc8.Copy(desc16) 

#ifdef TRACE_TRAPS
#undef TRAP_INSTRUMENTATION_LEAVE
#define TRAP_INSTRUMENTATION_LEAVE(aResult) \
    TTracer::static_trace( ((const TText8*)__PRETTY_FUNCTION__), \
                           __LINE__, aResult )
#endif


#if TRACER_LOG_METHOD == 1      // Print to RDebug

#include <e32debug.h>

#define TRACER_PRINT(a)            RDebug::Print(a,&iFunc);
#define TRACER_PRINT_BASE(a,b,c)   RDebug::Print(a,&iFunc, b, c);
#define TRACER_PRINT_RET(a,b)      RDebug::Print(a,&iFunc,b);
#define TRACER_PRINT_STATIC(a, b, c,d ) \
    RDebug::Print(a, b, c, d);

#elif TRACER_LOG_METHOD == 2    // Print to file

 // needs flogger.lib as DEBUGLIBRARY

// --------------------------------------------------------------------------
// Log directory under C:\logs
// --------------------------------------------------------------------------
//
//_LIT( KLogDir,  "MeCo" );      

// --------------------------------------------------------------------------
// Log filename in log directory
// --------------------------------------------------------------------------
//
//_LIT( KLogFile, "MeCo.log" );
#define TRACER_PRINT(a)         RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a,&iFunc);
#define TRACER_PRINT_BASE(a,b,c)   RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a,&iFunc,b,c);
#define TRACER_PRINT_RET(a,b)   RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a,&iFunc,b);
#define TRACER_PRINT_STATIC(a,b,c,d) \
    RFileLogger::WriteFormat(KLogDir, \
        KLogFile,EFileLoggingModeAppend,a,b,c,d);

#endif


/**
 * Simple tracer class that logs function enter, exit or leave
 */
class TTracer : public CBase
    {
private:
    
    /**
     * Simple helper method to just return if NULL
     */
    template<class T> static void ReturnIfNull(T* aNy) 
        { if ( !aNy ) return; }

public:
    /**
     * inline constructor to write log of entering a function
     */
    TTracer( const TText* aFunc, 
             const TText* aRetFormat, 
             TBool aPrintEnter) :
        iFunc( aFunc), 
        iRetFormat( aRetFormat ),
        iPrintEnterAndExit( aPrintEnter)
        {
        iFuncBuf = NULL;
        //iLogger.Connect();
        //iLogger.CreateLog(KLogDir, KLogFile, mode);
        if ( iPrintEnterAndExit )
            {
            TRACER_PRINT( KLogEnter );
            }
        }

    /**
     * inline constructor to write log of entering a function (8-bit)
     */
    TTracer( const TText8* aFunc, 
             const TText* aRetFormat, 
             TBool aPrintEnter) :
        iRetFormat( aRetFormat ),
        iPrintEnterAndExit( aPrintEnter )
        {
        TInt len = User::StringLength ((const unsigned char*)aFunc );
        HBufC8* temp = HBufC8::New( len );
        ReturnIfNull( temp );
        *temp = (const TUint8*)aFunc;
        iFuncBuf = HBufC::NewL( len );
        //iLogger.Connect();
        //iLogger.CreateLog(KLogDir, KLogFile, EFileLoggingModeAppend);
        if ( iFuncBuf )
            {
            iFuncBuf->Des().Copy( *temp );
            iFunc.Set( *iFuncBuf );
            if ( iPrintEnterAndExit )
                {
                TRACER_PRINT( KLogEnter );
                }
            }
        delete temp;
        }


    /**
     * inline destructor to write log of exiting a function 
     * normally or with a leave
     */
    ~TTracer( )
        {
        if ( std::uncaught_exception ( ) ) // Leave is an exception
            {
            // The function exited with a leave
            TRACER_PRINT( KLogLeave );
            }
        else
            {
            // The function exited normally
            if ( iPrintEnterAndExit )
                {
                if ( iRetFormat.Length ( )== 0 )
                    {
                    TRACER_PRINT( KLogExit );
                    }
                else
                    {
                    // Log the return value
#ifdef __WINS__
                    TInt32 retVal = 0;

                    // The assembly bit. This needs to be reimplemented
                    // for every target. 
                    _asm( mov retVal, ebx );

                    TBuf<100> format( KLogExitRet );
                    format.Append( iRetFormat );
                    TRACER_PRINT_RET( format, retVal );
#else
                    // ARM assembler to get return value
                    // could be implemented here..
                    TRACER_PRINT( KLogExit );
#endif
                    }
                }
            }
        //iLogger.Close();
        delete iFuncBuf;
        }

public:

    /**
     * Trace method that assumes 16-bit descriptor parameters
     */
    void trace( TInt aLine, const char* aMsg, ... )
        {
        ReturnIfNull( iFunc.Ptr() );
        TInt len = User::StringLength ((const unsigned char*)aMsg );
        TPtrC8 msg( (const unsigned char*)aMsg, len );
        HBufC* format = HBufC::New( len );
        ReturnIfNull( format );
        format->Des().Copy( msg );
        HBufC* result = HBufC::New( len + TRACER_PARAMS_LENGTH );
        if ( result )
            {
            VA_LIST list;
            VA_START( list, aMsg );
            result->Des().FormatList( *format, list );
            VA_END(list);
            TRACER_PRINT_BASE(KLogBase, aLine, result );
            }
        delete format;
        delete result;
        }

    /**
     * Trace method that assumes 8-bit descriptor parameters
     */
    void trace8(TInt aLine, const char* aMsg, ... )
        {
        ReturnIfNull( iFunc.Ptr() );
        TInt len = User::StringLength ((const unsigned char*)aMsg );
        TPtrC8 msg( (const unsigned char*)aMsg, len );
        HBufC8* result8 = HBufC8::New( len + TRACER_PARAMS_LENGTH );
        ReturnIfNull( result8 );
        VA_LIST list;
        VA_START( list, aMsg );
        result8->Des().FormatList( msg, list );
        VA_END(list);
        HBufC* result16 = HBufC::New( result8->Length() );
        if ( result16 )
            {
            result16->Des().Copy( *result8 );
            TRACER_PRINT_BASE( KLogBase, aLine, result16 );
            }
        delete result8;
        delete result16;
        }
    
    static void static_trace( const TText8* aFunc, 
                              TInt aLine,
                              TInt aResult )
        {
        TInt len = User::StringLength ((const unsigned char*)aFunc );
        HBufC8* temp = HBufC8::New( len );
        ReturnIfNull( temp );
        *temp = (const TUint8*)aFunc;
        HBufC* funcBuf = HBufC::NewL( len );
        if ( funcBuf )
            {
            funcBuf->Des().Copy( *temp );
            TRACER_PRINT_STATIC( 
                KLogTrappedLeave, funcBuf, aLine, aResult );
            }
        delete temp;
        delete funcBuf;
        }
public:

    /** 
     * Pointer descriptor to function signature that is to be logged.
     */
    TPtrC iFunc;
    //RFileLogger iLogger;

private:

    
	/**
     * Formatting string used to print the function return value
     */
    TPtrC iRetFormat;
    
    /**
     * If true, method enters and exits are printed
     */
    TBool iPrintEnterAndExit;
    
    /**
     * Buffer to store function name when needed
     */
    HBufC* iFuncBuf;
    };

#endif // TRACER_LOG_METHOD != 0 && CHAT_ENABLE_DEBUG_PRINT

#endif // UISERVICETABTRACER_H

// End of file
