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
* Description:  Provides macros for logging
*
*/



#ifndef __ICPLOGGER_H__
#define __ICPLOGGER_H__

#ifdef __WINS__
 #ifdef _DEBUG
    #define ICP_ENABLE_DEBUG_LOGS // for WINS UDEB
 #endif
#else
 #ifdef _DEBUG     
    #define ICP_ENABLE_DEBUG_LOGS // for device UDEB
 #endif
#endif
#define SCP_TP_PRINT(s) L##s
#define SCP_STRA_PRINT(s) SCP_TP_PRINT(s)
#define SCP_STR_PRINT(t) SCP_STRA_PRINT("[SCP]") L##t
#define SCP_LIT_PRINT(s) TPtrC((const TText *) SCP_STR_PRINT(s))
#define SCP_LIT_CONVERT(s) TPtrC((const TText *)(s))

//-----------------------------------------------------------------------------
//  LOGGING MACROs 
//-----------------------------------------------------------------------------

// Logging method is defined in the mmp file

#ifdef SCP_LOGGING_0

    #define ICPLOGSTRING(a)           
    #define ICPLOGSTRING2(a,b)     
    #define ICPLOGSTRING3(a,b,c)    
    #define ICPLOGSTRING4(a,b,c,d)
    #define ICPLOGSTRING5(a,b,c,d,e) 

#endif

#ifdef SCP_LOGGING_1
    #ifdef _DEBUG
        #include <e32def.h> 
        #include <e32svr.h>   
        #define ICPLOGSTRING(a)          { RDebug::Print(SCP_LIT_PRINT(a)); } 
        #define ICPLOGSTRING2(a,b)       { RDebug::Print(SCP_LIT_PRINT(a),b); } 
        #define ICPLOGSTRING3(a,b,c)     { RDebug::Print(SCP_LIT_PRINT(a),b,c); } 
        #define ICPLOGSTRING4(a,b,c,d)   { RDebug::Print(SCP_LIT_PRINT(a),b,c,d); }  
        #define ICPLOGSTRING5(a,b,c,d,e) { RDebug::Print(SCP_LIT_PRINT(a),b,c,d,e); }     
    #else
        #define ICPLOGSTRING(a)           
        #define ICPLOGSTRING2(a,b)     
        #define ICPLOGSTRING3(a,b,c)    
        #define ICPLOGSTRING4(a,b,c,d) 
        #define ICPLOGSTRING5(a,b,c,d,e) 
    #endif
#endif

#ifdef SCP_LOGGING_2
    //#ifdef _DEBUG
    	#define ICPLOGSTRING(a) { IcpDebugWriteFormat(SCP_LIT_PRINT(a)); } 
    	#define ICPLOGSTRING2(a,b) { IcpDebugWriteFormat(SCP_LIT_PRINT(a),b); } 
    	#define ICPLOGSTRING3(a,b,c) { IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c); }    
    	#define ICPLOGSTRING4(a,b,c,d) { IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c,d); };
        #define ICPLOGSTRING5(a,b,c,d,e) { IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c,d,e); };     
    /*#else
        #define ICPLOGSTRING(a)           
        #define ICPLOGSTRING2(a,b)     
        #define ICPLOGSTRING3(a,b,c)    
        #define ICPLOGSTRING4(a,b,c,d) 
        #define ICPLOGSTRING5(a,b,c,d,e) 
    #endif */
#endif

#ifdef SCP_LOGGING_3 
    #ifdef _DEBUG   
        #include <e32def.h> 
        #include <e32svr.h>   
        #define ICPLOGSTRING(a)          { RDebug::Print(SCP_LIT_PRINT(a));\
                                           IcpDebugWriteFormat(SCP_LIT_PRINT(a)); } 
        #define ICPLOGSTRING2(a,b)       { RDebug::Print(SCP_LIT_PRINT(a),b);\
                                           IcpDebugWriteFormat(SCP_LIT_PRINT(a),b); } 
        #define ICPLOGSTRING3(a,b,c)     { RDebug::Print(SCP_LIT_PRINT(a),b,c);\
                                           IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c); } 
        #define ICPLOGSTRING4(a,b,c,d)   { RDebug::Print(SCP_LIT_PRINT(a),b,c,d);\
                                           IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c,d); }  
        #define ICPLOGSTRING5(a,b,c,d,e) { RDebug::Print(SCP_LIT_PRINT(a),b,c,d,e);\
                                           IcpDebugWriteFormat(SCP_LIT_PRINT(a),b,c,d,e); }     
    #else
        #define ICPLOGSTRING(a)           
        #define ICPLOGSTRING2(a,b)     
        #define ICPLOGSTRING3(a,b,c)    
        #define ICPLOGSTRING4(a,b,c,d)
        #define ICPLOGSTRING5(a,b,c,d,e) 
    #endif     
#endif

#include <flogger.h>

inline void IcpDebugWriteFormat(TRefByValue<const TDesC> aFmt,...)
    {
#ifdef ICP_ENABLE_DEBUG_LOGS
    _LIT(KDir, "icp");
    _LIT(KName, "icp.log");

    // take the ellipsis parameters
    VA_LIST args;
    VA_START(args,aFmt);
    RFileLogger::WriteFormat(KDir, KName, EFileLoggingModeAppend, aFmt, args);
    VA_END(args);
#endif
    }

#endif  // __SCPLOGGER_H__
