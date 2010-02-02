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
* Description:  observer class for handling notifications of Search
*
*/


#ifndef _MVIMPSTENGINESEARCHEXTENTIONEVENTOBSERVER_H
#define _MVIMPSTENGINESEARCHEXTENTIONEVENTOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "tvimpstenums.h"

// CLASS DECLARATION
    
/**
 *  MVIMPSTEngineSearchExtentionEventObserver declaration.
 *  @since S60 5.0
 */
class MVIMPSTEngineSearchExtentionEventObserver
    {

	public: // New functions

		/**
		* Observer Event for search results
		* @param aSerachData - search results for a particular contact
		*/ 
		virtual void HandleSearchResultL(RArray<TVIMPSTSearchData> aSerachData) = 0;

		/**
		* Observer Event for search complete.
		* @param aError error if any
		*/
		virtual void HandleSearchFinishedL(TInt aError) = 0 ;


		/**
		* @see MVIMPSTEngineSearchExtentionEventObserver
		* @param aEnumKeysArray , array of enums key
		* @param aLebelKeysArray, array of key labels
		*/
		virtual void HandleSearchKeysEventL( RArray<TInt>& aEnumKeysArray, RPointerArray<HBufC>& aLebelKeysArray ) = 0 ;


		/**
		* virtual destructor
		*/
		virtual ~MVIMPSTEngineSearchExtentionEventObserver()
		{

		}

    };

#endif // _MVIMPSTENGINESEARCHEXTENTIONEVENTOBSERVER_H

// End of file
