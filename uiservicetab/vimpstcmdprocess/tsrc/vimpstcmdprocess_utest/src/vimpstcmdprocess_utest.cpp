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
* Description: This file contains STIFUnit module implementation.
*
*/

/******************************************************************************/
/*
 *
 * Test module configuration 
*/

// Version
#define TEST_MODULE_VERSION_MAJOR 0
#define TEST_MODULE_VERSION_MINOR 0
#define TEST_MODULE_VERSION_BUILD 0

// Uncomment following defines, if you want to provide implementation
// of enlisted virtual methods of test module.
// Implementation part is located at the end of this file.
//#define STIFUNIT_OOMTESTQUERYL
//#define STIFUNIT_OOMTESTINITIALIZEL
//#define STIFUNIT_OOMHANDLEWARNINGL
//#define STIFUNIT_OOMTESTFINALIZEL
/******************************************************************************/


/*
 * Test module internals. Please do not edit them. 
*/

// Includes
#include <e32base.h>
#include "cvimpstcmdfactory.h"
#include "cvimpstprocessarray.h"
#include "cvimpstprocessfriendrequestitem.h"
#include "s_vimpstengin.h"
#include "s_vimpststoragecontact.h"
#include "s_vimpststoragecontactlist.h"
#include "s_vimpstcmdobserver.h"

#include "s_vimpstprocessarrayItem.h"
#include "s_processarrayobserver.h" 
#include "s_vimpststorageserviceview.h"
#include "s_storageitemmodel.h"
#include "s_mccaconnection.h"
#include "mvimpstcmd.h"
#include "tvimpstconsts.h"

// Literals
_LIT( KUnitLogPath, "\\logs\\testframework\\vimpstcmdprocess_utest\\" ); 
_LIT( KUnitLogFile, "vimpstcmdprocess_utest.txt" ); 
_LIT( KLogStart, "vimpstcmdprocess_utest logging starts!" );

// Defines
#define STIF_UNIT_TEST_CASES "../src/vimpstcmdprocess_utestCases.cpp"
#define STIF_UNIT_MODULE_CLASS_NAME Cvimpstcmdprocess_utest
#define STIF_UNIT_MODULE_NAME _L("vimpstcmdprocess_utest.dll")

// Include STIF unit generic file
#include <StifUnitGeneric.h>

/*
 * User implementation of OOM virtual methods.
 * Providing own implementation requires uncommenting defines at the
 * beginnig of this file.   
*/
#ifdef STIFUNIT_OOMTESTQUERYL
TBool Cvimpstcmdprocess_utest::OOMTestQueryL( const TFileName& /*aTestCaseFile*/, 
                                   const TInt aCaseNumber, 
                                   TOOMFailureType& aFailureType, 
                                   TInt& aFirstMemFailure, 
                                   TInt& aLastMemFailure)
    {
    }
#undef STIFUNIT_OOMTESTQUERYL
#endif

#ifdef STIFUNIT_OOMTESTINITIALIZEL
void Cvimpstcmdprocess_utest::OOMTestInitializeL( const TFileName& /*aTestCaseFile*/, 
                                       const TInt /*aCaseNumber*/ )
    {
    }
#undef STIFUNIT_OOMTESTINITIALIZEL
#endif

#ifdef STIFUNIT_OOMHANDLEWARNINGL
void Cvimpstcmdprocess_utest::OOMHandleWarningL( const TFileName& /*aTestCaseFile*/,
                                      const TInt /*aCaseNumber*/, 
                                      TInt& /*aFailNextValue*/)
    {
    }
#undef STIFUNIT_OOMHANDLEWARNINGL
#endif
    
#ifdef STIFUNIT_OOMTESTFINALIZEL
void Cvimpstcmdprocess_utest::OOMTestFinalizeL( const TFileName& /*aTestCaseFile*/, 
                                     const TInt /*aCaseNumber*/ )
    {
    }
#undef STIFUNIT_OOMTESTFINALIZEL
#endif

//  End of File
