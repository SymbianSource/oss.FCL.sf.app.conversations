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
* Description:  command process for search
*
*/


// INCLUDE FILES
#include "cvimpstcmdsearch.h"

#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include "mvimpstengine.h"
#include "mvimpstenginesearchmgrextention.h"

#include "mvimpstengineextentionfeatures.h"

// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::CVIMPSTCmdSearch
// --------------------------------------------------------------------------
//
CVIMPSTCmdSearch::CVIMPSTCmdSearch( 
						        const TInt aCommandId ,
						        RArray<TVIMPSTSearchKeyData>& aKeyDataArray,
						        MVIMPSTEngine& aEngine) :            
            iCommandId( aCommandId ),
            iKeyDataArray( aKeyDataArray ),
            iEngine(aEngine)
    {
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::~CVIMPSTCmdSearch
// --------------------------------------------------------------------------
//
CVIMPSTCmdSearch::~CVIMPSTCmdSearch()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdSearch* CVIMPSTCmdSearch::NewL( 
							        const TInt aCommandId,
							        RArray<TVIMPSTSearchKeyData>& aKeyDataArray,
							        MVIMPSTEngine& aEngine )
    {
    CVIMPSTCmdSearch* self = new (ELeave ) CVIMPSTCmdSearch( aCommandId ,aKeyDataArray, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdSearch::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdSearch::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdSearch::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );   
	
	//make search request using search manager in engine 
	MVIMPSTEngineExtentionFeatures* feature = iEngine.ExtentionFeatures(TVIMPSTEnums::ESearch);
	if (feature)
		{
		MVIMPSTEngineSearchMgrExtention& searchMgr = 
	   	   	MVIMPSTEngineSearchMgrExtention::Cast
	   	   	(*feature);	
	   	
	   	iError = searchMgr.SearchContactsL( iKeyDataArray );   	
		}	  	
   	
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdSearch:: CommandFinished");

    if(iObserver)
	    {
	    iObserver->CommandFinishedL(*this);
	    }
	CHAT_DP_FUNC_ENTER("CVIMPSTCmdSearch:: CommandFinished");	
	CleanupStack::PopAndDestroy();	
	CHAT_DP_FUNC_DONE("CVIMPSTCmdSearch::ExecuteLD");	
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdSearch::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdSearch::CommandId() const
	{
	return iCommandId;	
	}

// --------------------------------------------------------------------------
// CVIMPSTCmdSearch::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdSearch::Result() const
	{
	//return valid data regd the command operation
	return iError;
	}

// End of File
