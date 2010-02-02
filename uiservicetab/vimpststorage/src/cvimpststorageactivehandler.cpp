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
* Description:  The CVIMPSTStorageActiveHandler class handles the waiting functionality 
*
*/


// INCLUDE FILES
#include "cvimpststorageactivehandler.h"
#include "mvimpststorageactiveobserver.h"

#include "vimpstdebugtrace.h"
#include <e32base.h>

// CONSTANTS
const TInt KTimeToWaitBeforeRefresh( 1000000 ); // 1 seconds in microseconds
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVIMPSTStorageActiveHandler::CVIMPSTStorageActiveHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageActiveHandler::CVIMPSTStorageActiveHandler( MVIMPSTStorageActiveObserver* aObserver ) : 
    CTimer( EPriorityIdle ),
    iObserver( aObserver )
    {
	CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageActiveHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageActiveHandler::ConstructL()
    {
	// the base class must be constructed explicitely
	CTimer::ConstructL();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageActiveHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVIMPSTStorageActiveHandler* CVIMPSTStorageActiveHandler::NewL( 
					MVIMPSTStorageActiveObserver* aObserver )
    {
    CVIMPSTStorageActiveHandler* self = new( ELeave ) CVIMPSTStorageActiveHandler( aObserver );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
	return self;
    }

// Destructor
CVIMPSTStorageActiveHandler::~CVIMPSTStorageActiveHandler()
    {
	Cancel();
    }

// -----------------------------------------------------------------------------
// CVIMPSTStorageActiveHandler::Start
// -----------------------------------------------------------------------------

void CVIMPSTStorageActiveHandler::IssueRequest(TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
											  MVIMPSTStorageContactList *aList,
					                          MVIMPSTStorageContact* aContact,
					                          TInt aContactIndex )
	{
	TRACE( T_LIT("CVIMPSTStorageActiveHandler::IssueRequest() begin") );
	
	 if( IsActive() )
        {
        Cancel();
        }
    TTimeIntervalMicroSeconds32 waittime( KTimeToWaitBeforeRefresh );
    CTimer::After( waittime );
  
	iEventType = aType ;
	iList = aList ; 
	iContact = aContact ;
	iContactIndex = aContactIndex;
	TRACE( T_LIT("CVIMPSTStorageActiveHandler::IssueRequest() end") );
	}

// -----------------------------------------------------------------------------
// CVIMPSTStorageActiveHandler::RunL
// Derived from CActive
// -----------------------------------------------------------------------------
//
void CVIMPSTStorageActiveHandler::RunL()
	{
	TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunL() begin") );
	if( iObserver )
		{
		TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunL() send notification") );
		TInt status( iStatus.Int() );
	    if( status != KErrCancel )
            {
		    iObserver->HandleDelayedNotificationL(iEventType,
											iList ,
											iContact,
											iContactIndex );
            }
            
        
		TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunL() notification sent") );
		}
    TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunL() end") );
	}

// ---------------------------------------------------------
// CVIMPSTStorageActiveHandler::RunError
// Derived from CActive
// ---------------------------------------------------------
//
TInt CVIMPSTStorageActiveHandler::RunError( TInt aError )
	{
	TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunError() begin") );
	if( iObserver )
		{
	    TRAP_IGNORE( iObserver->HandleDelayedNotificationL(iEventType,
											iList ,
											iContact,
											iContactIndex ) );
        }
    TRACE( T_LIT("CVIMPSTStorageActiveHandler::RunError() End ") );
	return aError;
	}
// ---------------------------------------------------------
// CVIMPSTStorageActiveHandler::DoCancel
// Derived from CActive
// ---------------------------------------------------------
//
void CVIMPSTStorageActiveHandler::DoCancel()
	{
	CTimer::DoCancel();// not required
	}

//  End of File
