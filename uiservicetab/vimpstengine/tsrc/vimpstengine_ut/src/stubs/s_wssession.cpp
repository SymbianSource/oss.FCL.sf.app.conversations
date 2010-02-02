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
* Description: s_wssession.cpp
*
*/
#include <apgtask.h>
#include <w32std.h>


RWsSession::RWsSession() {}

TInt RWsSession::Connect() {return KErrNone;}

void RWsSession::Close()  {}


TApaTask::TApaTask(RWsSession& aWsSession) 
:iWsSession(aWsSession)
{}

TBool TApaTask::Exists() const { return ETrue; }
void TApaTask::KillTask() {}
void TApaTask::EndTask() {}

TApaTaskList::TApaTaskList(RWsSession& aWsSession) 
:iWsSession(aWsSession)
{}

TApaTask TApaTaskList::FindApp(TUid /*aAppUid*/)
    {
    
    }
	

MWsClientClass::MWsClientClass() {}
