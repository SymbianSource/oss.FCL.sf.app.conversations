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
* Description:  message container info interface
*
*/


#ifndef __MIMCVENGINEMESSAGECONTAINERINFO_H__
#define __MIMCVENGINEMESSAGECONTAINERINFO_H__

//	FORWARD CLASS DECLERATIONS

// CLASS DECLARATION

/**
 *  Message container information provider
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageContainerInfo
	{

	public: // Interface

		
		/**
		 * Part of Id of container
		 * @return Conversation Party id of container.
		 */		
        virtual const TDesC& Target() const = 0;
        
        /**
         * Returns target id to be used for
         * comparisons.
         * @since S60 v3.1
         * @return Target id.
         */
        virtual const TDesC& ComparableTarget() const = 0;
        
               
	protected: // For protection.

		/**
		 * Destructor
		 */
		virtual ~MIMCVEngineMessageContainerInfo(){}
	};

#endif      // __MIMCVENGINEMESSAGECONTAINERINFO_H__

// End of File
