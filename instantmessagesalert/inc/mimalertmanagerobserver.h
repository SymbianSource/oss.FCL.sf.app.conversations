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
* Description:  Interface class for observe audiomanager playing
*
*/

#ifndef MIMALERTMANAGEROBSERVER_H
#define MIMALERTMANAGEROBSERVER_H

// CLASS DECLARATION

/**
*  Interface class for fadetext in navipane
*
*  @lib chat.app
*  @since 2.6
*/
class MIMAlertManagerObserver
	{
	public:

		/**
		 * This method notifies when playing is completed
		 */
		virtual void PlayCompleted() = 0;

	protected:

		/**
		 * Destructor
		 */
		virtual ~MIMAlertManagerObserver() {};
					
	};

#endif	// MIMALERTMANAGEROBSERVER_H

//end of file

