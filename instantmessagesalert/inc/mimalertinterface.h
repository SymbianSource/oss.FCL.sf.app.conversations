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
* Description:  Interface for controlling msg tone
*
*/

#ifndef MIMALERTINTERFACE_H
#define MIMALERTINTERFACE_H


#include <apgcli.h>
#include <mdaaudiosampleplayer.h>
#include <mdaaudiotoneplayer.h>
#include <bldvariant.hrh>

#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <barsc.h>
#include <Profile.hrh>

class MIMAlertManagerObserver;

class MIMAlertInterface

	{
		public:
		
		virtual void PlayL(TUint32 aServiceId) = 0;
		
		virtual void Stop() = 0;
		
		virtual void SetVolume( TInt aVolume ) = 0;
		
		
		virtual void SetRingingType( TProfileRingingType aRingingType ) = 0;
		
		
		virtual void SetVibra( TBool aVibra) = 0;

		virtual void AddObserverL( const MIMAlertManagerObserver* aObserver ) = 0;
		
		virtual void RemoveObserver( const MIMAlertManagerObserver* aObserver ) = 0;
		
		
			
		virtual ~MIMAlertInterface(){};
		
		
	};
	
	
#endif  //MIMALERTINTERFACE_H
