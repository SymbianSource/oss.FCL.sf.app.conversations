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
* Description:  buddy ,presene status of a user 
 *
*/


// INCLUDE FILES
#include "cservicewidgetbuddypresence.h"

// ================= MEMBER FUNCTIONS =======================
// --------------------------------------------------------------------------
// CServiceWidgetTimer::NewL
// --------------------------------------------------------------------------
//
CServiceWidgetBuddyPresence* CServiceWidgetBuddyPresence::NewL(const TDesC& aBuddyId, TWSPresenceStatus aStatus )
	{
	CServiceWidgetBuddyPresence* self =
	new (ELeave) CServiceWidgetBuddyPresence(aStatus);
	CleanupStack::PushL(self);
	self->ConstructL( aBuddyId);
	CleanupStack::Pop(self);
	return self;	
	}
// --------------------------------------------------------------------------
// CServiceWidgetTimer::CServiceWidgetBuddyPresence
// --------------------------------------------------------------------------
//
CServiceWidgetBuddyPresence::CServiceWidgetBuddyPresence(TWSPresenceStatus aStatus):
	iStatus( aStatus )
	{
	}
// --------------------------------------------------------------------------
// CServiceWidgetTimer::ConstructL
// --------------------------------------------------------------------------
//
void CServiceWidgetBuddyPresence::ConstructL(const TDesC& aBuddyId )
	{
	iBuddyId = aBuddyId.AllocL();	
	}

// --------------------------------------------------------------------------
// CServiceWidgetTimer::~CServiceWidgetBuddyPresence
// --------------------------------------------------------------------------
//
CServiceWidgetBuddyPresence::~CServiceWidgetBuddyPresence()
	{
	delete iBuddyId;	
	}
// --------------------------------------------------------------------------
// CServiceWidgetTimer::BuddyId
// --------------------------------------------------------------------------
//
const TDesC& CServiceWidgetBuddyPresence::BuddyId()
	{
	if( iBuddyId )	
		{
		return *iBuddyId;	
		}
	return KNullDesC;	
	}
// --------------------------------------------------------------------------
// CServiceWidgetTimer::PresenceStatus
// --------------------------------------------------------------------------
//
TWSPresenceStatus CServiceWidgetBuddyPresence::PresenceStatus()
	{
	return iStatus;	
	}
// --------------------------------------------------------------------------
// CServiceWidgetTimer::SetPresenceStatus
// --------------------------------------------------------------------------
//
void CServiceWidgetBuddyPresence::SetPresenceStatus( TWSPresenceStatus aStatus )
	{
	iStatus = aStatus;	
	}
//  End of File  


