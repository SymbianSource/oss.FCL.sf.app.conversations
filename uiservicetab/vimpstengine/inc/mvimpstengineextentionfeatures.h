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
* Description:  M-Class that capsulates Extention Features Supported by Services
*
*/


#ifndef MVIMPSTENGINEEXTENTIONFEATURES_H
#define MVIMPSTENGINEEXTENTIONFEATURES_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"


//CLASS DECLARATION
/**
 *  
 *  This class list the M-Class for Extention Features required
 *  to be supported in any Services
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */

//Class declaration
class MVIMPSTEngineExtentionFeatures
	{
		
	public:			   
	   
		/**
		* IsSupported
		* @return TVIMPSTEnums::FeatureSupport defined in tvimpstenums.h
		*/
		virtual TVIMPSTEnums::FeatureSupport IsSupported() const = 0;
		
		/**
         * SetSupported
         * @param TVIMPSTEnums::FeatureSupport defined in tvimpstenums.h
         */
        virtual  void SetSupported(TVIMPSTEnums::FeatureSupport aSupported )= 0;


		/**
		* Type of the Extention Feature
		* @return TVIMPSTEnums::ExtentionType defined in tvimpstenums.h
		*/

		virtual TVIMPSTEnums::ExtentionType Type() const = 0;

		/**
		* Destructor
		*/		
		virtual ~MVIMPSTEngineExtentionFeatures() 
		{}
		
	};

#endif //MVIMPSTENGINEEXTENTIONFEATURES_H


