/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Classes that want events from resource changes implement this interface.
*
*/



#ifndef MIMCVAPPRESOURCECHANGEOBSERVER_H
#define MIMCVAPPRESOURCECHANGEOBSERVER_H



// CLASS DECLARATION

/**
*  Interface class for observing resource changes.
*  @lib imcvuiapp.exe
*  @since S60 5.0
*/
class MIMCVAppResourceChangeObserver
    {
    
	public:  // New functions

		/*
		* Called when resource changes
		*/
		virtual void ResourceChangedL() = 0;

	protected:

		virtual ~MIMCVAppResourceChangeObserver() {};

    };

#endif      // MIMCVAPPRESOURCECHANGEOBSERVER_H

// End of File
