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


#ifndef CSERVICEWIDGETBUDDYPRESENCE_H
#define CSERVICEWIDGETBUDDYPRESENCE_H

#include <e32base.h>
#include "servicewidgetenums.hrh"

class CServiceWidgetBuddyPresence : public CBase
	{
	public :
		
		/**
		* Two-phased constructor.
		* @param aBuddyId, friend buddy id
		* @param aStatus, buddy presence status
		* @return new instance of this class
		*/
		static CServiceWidgetBuddyPresence* NewL(const TDesC& aBuddyId, TWSPresenceStatus aStatus );
		/**
		* Standard C++ destructor
		*/    
		~CServiceWidgetBuddyPresence();
		
		/**
		* @return buddy Id
		*/    
		const TDesC& BuddyId();
		
		/**
		* @return buddy status
		*/ 
		TWSPresenceStatus PresenceStatus();
		
		/**
		* @param aStatus, status to sdet
		*/ 
		void SetPresenceStatus( TWSPresenceStatus aStatus );
	private :    
		/**
		* constructor
		*/    
		CServiceWidgetBuddyPresence(TWSPresenceStatus aStatus);
		/**
		* Performs the 2nd phase of construction.
		* @param aServiceName, service name
		*/        
		void ConstructL(const TDesC& aBuddyId );
	private :   // data
		// Owns : buddy id
		HBufC* iBuddyId;
		// owns : buddy presence status
		TWSPresenceStatus iStatus;     

	};
#endif // CSERVICEWIDGETBUDDYPRESENCE_H