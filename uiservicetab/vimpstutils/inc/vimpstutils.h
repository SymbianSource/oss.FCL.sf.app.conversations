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
* Description:  Utils for IMUiServiceTab modules.
*
*/


#ifndef __VIMPSTUTILS_H__
#define __VIMPSTUTILS_H__

//  INCLUDES
#include <e32def.h>
#include <e32cmn.h>

/**
 *  Utils for IMUiServiceTab modules. 
 *  @lib vimpstengine.lib
 *  @since 5.0
 */
class VIMPSTUtils   
	{
	public: // new methods
 
        /**
         * Used for removing the domain part from the user id
         * @param aId The id to process
         * @param aListHiding, is list hiding.
         * @return TPtrC The id without the domain part
         */
        IMPORT_C static TPtrC DisplayId( const TDesC& aId, TBool aListHiding = EFalse );        
        
        /**
         * used to load the resource
         * @param aResourceId The id to read
         * @return  HBufC* The string for given resource id 
         */
        IMPORT_C static HBufC* LoadResourceL( TInt aResourceId );      
        
        /**
         * used to load the resource
         * @param aResourceId The id to read
         * @param aPrompt %U string.
         * @return  HBufC* The string for given resource id 
         */
        IMPORT_C static HBufC* LoadResourceL( TInt aResourceId , const TDesC& aPrompt);
        
        /**
         * Combines two strings given resource ids.
         * The caller is responsible to delete the string
         *
         * @since 5.0
         * @param TInt aResourceId1
         * @param TInt aResourceId2
         * @return HBufC* the combined string
         */
        IMPORT_C static HBufC* CombineStringFromResourceLC( TInt aResourceId1, TInt aResourceId2 ); 
  
	};

#endif      // __VIMPSTUTILS_H__

// End of File
