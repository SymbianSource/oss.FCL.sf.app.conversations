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
*  Description : observer class for handling notifications of fetching blocked list.
*
*/

#ifndef _MVIMPSTENGINEBLOCKEDLISTFETCHEVENTOBSERVER_H
#define _MVIMPSTENGINEBLOCKEDLISTFETCHEVENTOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "tvimpstenums.h"

// CLASS DECLARATION
    
/**
 *  MVIMPSTEngineSearchExtentionEventObserver declaration.
 *  @since S60 5.0
 */
class MVIMPSTEngineBlockedListFetchEventObserver
    {

	public: // New functions

		/**
		* Observer Event for fetching blocked.
		* Notify that blocked list fetch has been completed.
		*/ 
		virtual void HandleBlockedListFetchCompleteL() = 0;

		/**
		* virtual destructor
		*/
		virtual ~MVIMPSTEngineBlockedListFetchEventObserver()
		{

		}

    };

#endif // _MVIMPSTENGINEBLOCKEDLISTFETCHEVENTOBSERVER_H

// End of file
