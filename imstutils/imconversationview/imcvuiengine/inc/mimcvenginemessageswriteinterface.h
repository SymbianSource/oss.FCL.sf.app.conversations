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
* Description:  Interface for write access to messages.
*
*/

#ifndef __MIMCVENGINEMESSAGESWRITEINTERFACE_H__
#define __MIMCVENGINEMESSAGESWRITEINTERFACE_H__

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineMessage;

// CLASS DECLARATION

/**
 *  Interface for write access to messages.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class MIMCVEngineMessageWriteInterface
	{
	public: // Interface
		
		/**
		 * Append one message to container.
		 * @param aMessage. New message to container.
		 * @param aSharedOwnerShip EFalse if ownership is tranferred, ETrue if shared.
		 */
		virtual void AppendL( MIMCVEngineMessage* aMessage, TBool aSharedOwnership = EFalse ) = 0;
				
	
		
				
		/**
		 * Get the time and date of the last message written to this interface,
		 * Time is used to determine the need of datestamp.
		 * @return Date and time of the last message. if there are no 
		 *         previous messages, then returns Time::NullTTime()
		 */
		virtual TTime Time() = 0;
		
			
		/**
         * sets the display name
         * @param aDispalyName. updated display name
         */
        virtual void SetNameL( const TDesC& aDispalyName ) = 0;
 
        /**
         * sets the contactlink
         * @param aContactLink. updated contactlink
         */
       virtual  void  SetLinkL(const TDesC8& aContactLink) = 0;
        /**
		 * Virtual destructor
		 */
		virtual ~MIMCVEngineMessageWriteInterface(){}
	};

#endif      // __MIMCVENGINEMESSAGESWRITEINTERFACE_H__

// End of File
