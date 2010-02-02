/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface class for fadetext in navipane
*
*/



#ifndef MIMCVNewMsgFadeControlObserver_H
#define MIMCVNewMsgFadeControlObserver_H

// CLASS DECLARATION

/**
*  Interface class for fadetext in navipane
*
*  @imcvuiapp.exe
*  @since 5.0
*/
class MIMCVNewMsgFadeControlObserver
	{
	public:

		/**
		 * This method notifies when complete
		 */
		virtual void NotifyWhenCompleted() = 0;
		
		
		/**
		 * This method notifies when fadetext starts
		 */
		virtual void NotifyWhenStarted() = 0;

	protected:

		/**
		 * DEstructor
		 */
		virtual ~MIMCVNewMsgFadeControlObserver() {};
					
	};

#endif	//MIMCVNewMsgFadeControlObserver_H

//end of file

