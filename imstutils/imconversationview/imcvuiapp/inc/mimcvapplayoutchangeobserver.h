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
* Description:  layout change observer
*
*/



#ifndef MIMCVAPPLAYOUTCHANGEOBSERVER_H
#define MIMCVAPPLAYOUTCHANGEOBSERVER_H

#include <e32base.h>

// CLASS DECLARATION

/**
*  Interface class for observing layout changes.
*  @since 5.0
*/
class MIMCVAppLayoutChangeObserver
    {
    
	public:  // New functions

		/*
		* Called when layout changes
		*/
		virtual void LayoutChangedL( TInt aType = 0 ) = 0;

	protected:

		virtual ~MIMCVAppLayoutChangeObserver() {};

    };

#endif      // MIMCVAPPLAYOUTCHANGEOBSERVER_H

// End of File
