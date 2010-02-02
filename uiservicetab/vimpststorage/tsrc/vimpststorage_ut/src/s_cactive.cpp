/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/

#include <e32base.h>

CActiveScheduler::TLoopOwner gTLoop ;

TBool gSetActive = EFalse;
/*
	IMPORT_C ~CActive();
	IMPORT_C void Deque();
	IMPORT_C void SetPriority(TInt aPriority);
	inline TBool IsAdded() const;
	inline TInt Priority() const;
	IMPORT_C virtual TInt RunError(TInt aError);
	IMPORT_C virtual TInt Extension_(TUint aExtensionId, TAny*& a0, TAny* a1);
*/

void CActive::Cancel()
    {
    iStatus.iFlags = 0;
    }

CActive::CActive(TInt /*aPriority*/)
    {
    }

void CActive::SetActive()
    {
    iStatus.iFlags = TRequestStatus::EActive;
    }

CActive::~CActive()
    {
    }

void CActive::Deque()
    {
    }

void CActive::SetPriority(TInt /*aPriority*/)
    {
    }

TInt CActive::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

TInt CActive::Extension_(TUint /*aExtensionId*/, TAny*& /*a0*/, TAny* /*a1*/)
    {
    return KErrNone;
    }

void CActiveScheduler::Add(CActive* /*aActive*/)
    {
    }


void CPeriodic::Start(TTimeIntervalMicroSeconds32 /*aDelay*/,
                      TTimeIntervalMicroSeconds32 /*anInterval*/,
                      TCallBack /*aCallBack*/)
    {
    }


CActiveSchedulerWait::CActiveSchedulerWait() 
	{		
	}
CActiveSchedulerWait::~CActiveSchedulerWait()
	{		
	}

void CActiveSchedulerWait::Start()
	{
	CActiveScheduler::TLoopOwner temp = NULL;	
	iLoop = temp; //assign some local variable with junk value
	}

void CActiveSchedulerWait::AsyncStop()
	{	
	iLoop = NULL;	
	}

	

