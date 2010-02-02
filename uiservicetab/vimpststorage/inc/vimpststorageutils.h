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
* Description:  Utils for vimpststorage
*
*/


#ifndef __VIMPSTSTORAGEUTILS_H__
#define __VIMPSTSTORAGEUTILS_H__

#include    <e32base.h>
#include    <s32strm.h>
#include    <badesca.h>

// FORWARD CLASS DECLERATIONS

// CLASS DECLARATION

/**
 *  Utils for IMUiServiceTab modules. 
 */
class VIMPSTStorageUtils   
	{
	public: // new methods

        /**
         * Case insensitive comparison of WVIDs, using protocol ("wv:") neutral
         * comparison.  The "wv:" part (actually everything up to the first
         * ":") is ignored in the comparison. If aDomainNeutral is ETrue, does
         * domain-neutral comparison with the following logic:
         *   - If only one of aId1 or aId2 (but not both) contains domain,
         *     domain-neutral comparison is done
         *   - If both or none of aId1 and aId2 contain domain, ordinary 
         *     wv-neutral comparison is done
         * If aDomainNeutral is EFalse, then falls back to the old way of 
         * comparing, i.e. wv-neutral only
         * @param aId1 The first id to compare (network-given)
         * @param aId2 The second id to compare
         * @param aDomainNeutral Do domain neutral comparison (ETrue, default)
         * @return -1, 0 or +1, like CompareC.
         * @since 2.5
         */
        static TInt NeutralCompare( const TDesC& aId1, const TDesC& aId2, TBool 
        	aDomainNeutral = ETrue );
        /**
         * Process userid/groupid/listid so that the "wv:" part is hidden,
         * depending on the branded setting coming from the UI.
         * @param aId The id to process
         * @return The id with the "wv:" (userid, groupid) or "wv:user" (listid) 
         * part hidden
         */
        static TPtrC DisplayId( const TDesC& aId, TBool aListHiding = EFalse );        
        
        
	};

#endif      // __VIMPSTSTORAGEUTILS_H__

// End of File
