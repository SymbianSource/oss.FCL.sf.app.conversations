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
* Description:  utils class
*
*/


#ifndef SERVICEWIDGETUTILS_H
#define SERVICEWIDGETUTILS_H

//  INCLUDES
#include <e32base.h>

class CCoeEnv;

/**
 *  Utils for imwidget modules. 
 *  @lib imwidgetdatapublisher.dll
 *  @since 5.0
 */
class ServiceWidgetUtils   
	{
	public: // new methods
 
        /**
         * Used for removing the domain part from the user id
         * @param aId The id to process
         * @param aDomainHiding, is list hiding.
         * @return TPtrC The id without the domain part
         */
        static TPtrC DisplayId( const TDesC& aId, 
        						TBool aDomainHiding = ETrue );        
        
        /**
         * used to load the resource and 
         * @param aResourceId The id to read
         * @return  push onto cleanup stack HBufC* The string for given resource id 
         */
        static HBufC* LoadResourceLC( TInt aResourceId, 
        							  CCoeEnv* aEnv = NULL );      
        
        /**
         * used to load the resource
         * @param aResourceId The id to read
         * @param aText %U string.
         * @return  , push onto cleanup stack HBufC* The string for given resource id 
         */
        static HBufC* LoadResourceLC( TInt aResourceId ,
        							 const TDesC& aText, 
        							 CCoeEnv* aEnv = NULL );
        
        /**
         * used to load the resource 
         * @param aResourceId The id to read
         * @param aInt %N integer value to append
         * @return  , push onto cleanup stack HBufC* The string for given resource id 
         */
        static HBufC* LoadResourceLC( TInt aResourceId ,
        							   TInt aInt, 
        							   CCoeEnv* aEnv = NULL ); 
        
         /**
         * used to load the resource
         * @param aResourceId The id to read
         * @param aInts array of integer, %0N,%1N integer array.
         * @param aEnv, a pointer to enviroment
         * @return  , push onto cleanup stack HBufC* The string for given resource id 
         */
        static HBufC* LoadResourceLC( TInt aResourceId ,
        							  const CArrayFix<TInt>& aInts, 
        							  CCoeEnv* aEnv = NULL );
  
	};

#endif      // SERVICEWIDGETUTILS_H

// End of File
