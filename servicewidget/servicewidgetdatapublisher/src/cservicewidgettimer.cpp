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
* Description:  Event listener implementation presence
*
*/

// INCLUDE FILES
#include "cservicewidgettimer.h"
#include "mservicewidgetobservers.h"
//system includes
#include <e32base.h>
#include "swpdebugtrace.h"
// ================= MEMBER FUNCTIONS =======================
// --------------------------------------------------------------------------
// CServiceWidgetTimer::CServiceWidgetTimer
// --------------------------------------------------------------------------
//
CServiceWidgetTimer::CServiceWidgetTimer( MServiceWidgetTimerObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
    iObserver( aObserver )
   	{
    CActiveScheduler::Add( this );
    }
    
// --------------------------------------------------------------------------
// CServiceWidgetTimer::ConstructL
// --------------------------------------------------------------------------
//
void CServiceWidgetTimer::ConstructL()
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::ConstructL() start") );
    User::LeaveIfError( iTimer.CreateLocal() );
    TRACE_SWP(TXT("CServiceWidgetTimer::ConstructL() end") );
    }


// --------------------------------------------------------------------------
// CServiceWidgetTimer::NewL
// --------------------------------------------------------------------------
//
CServiceWidgetTimer* CServiceWidgetTimer::NewL(MServiceWidgetTimerObserver& aObserver )
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::NewL() start") );
    CServiceWidgetTimer* self = new(ELeave) CServiceWidgetTimer( aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetTimer::NewL() end") );
    return self;
    }
// ---------------------------------------------------------
// CServiceWidgetTimer::~CServiceWidgetTimer()
// C++ Destructor 
// ---------------------------------------------------------
//
CServiceWidgetTimer::~CServiceWidgetTimer()
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::~CServiceWidgetTimer() start") );
    Cancel(); // Cancel any request, if outstanding
  	iTimer.Close(); // Destroy the RTimer object
  	TRACE_SWP(TXT("CServiceWidgetTimer::~CServiceWidgetTimer() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetTimer::RunL()
// ---------------------------------------------------------
//
void CServiceWidgetTimer::RunL()
    {
	TRACE_SWP(TXT("CServiceWidgetTimer::RunL() start") );
    if( iStatus.Int() == KErrNone )
	    {
	    iObserver.HandleTimerExpiredL();	
	    }
	TRACE_SWP(TXT("CServiceWidgetTimer::RunL() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetTimer::RunL()
// ---------------------------------------------------------
//
void CServiceWidgetTimer::DoCancel()
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::DoCancel() start") );
   	iTimer.Cancel();
   	TRACE_SWP(TXT("CServiceWidgetTimer::DoCancel() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetTimer::StartListening()
// ---------------------------------------------------------
//
void CServiceWidgetTimer::StartListening(TTimeIntervalMicroSeconds32 anInterval)
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::StartListening() start") );
    if(!IsActive() )
        {
        iTimer.After ( iStatus, anInterval );
		SetActive (); // Tell scheduler a request is active
        }
    TRACE_SWP(TXT("CServiceWidgetTimer::StartListening() end") );
    }
// ---------------------------------------------------------
// CServiceWidgetTimer::StopListening()
// ---------------------------------------------------------
//
void CServiceWidgetTimer::StopListening()
    {
    TRACE_SWP(TXT("CServiceWidgetTimer::StopListening() start") );
    if(IsActive() )
        {
        Cancel();
        }
    TRACE_SWP(TXT("CServiceWidgetTimer::StopListening() end") );
    }
    
//  End of File  


