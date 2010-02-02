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
* Description:  factory class for Engine component
*
*/


#ifndef MVIMPSTENGINEFACTORY_H
#define MVIMPSTENGINEFACTORY_H
#include <e32def.h>

//FORWARD DECLARATION
class MVIMPSTEngine;

//CLASS  DECLARATION
/**
 *  
 *  This class list the M-Class for Engine factory
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
class MVIMPSTEngineFactory
	{
		
	public:	
	
		/*
		* Fills the RPointerArray passed as a Param
		* with engine items for each service
		* The Owenership of the ptr is not transferred to the caller
		* @param - RPointerArray which upon completion of this function
		*  will hold the pointers to each engine instance created for 
		*  different services
		*/
		virtual	void GetServiceEnginePtr(
				RPointerArray<MVIMPSTEngine>& serviceIdArray) const = 0;
	  	/**
		 * FindService finds if a given serviceid is installed.
		 * @param aServiceId service id
		 * @return TInt index of the service else retunr KErrNotFOund.
		 **/
	   	virtual	TInt FindService(
				TUint aServiceId ) const = 0;
				
				
	  	/**
		* ~CVIMPSTEngineFactory 
		* dont call delete 
		*/
		virtual ~MVIMPSTEngineFactory() 
			{
				
			}
		
	};

#endif //MVIMPSTENGINEFACTORY_H