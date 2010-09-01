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
* Description:  message creater class
*
*/


#ifndef CIMCVENGINEMESSAGECREATOR_H
#define CIMCVENGINEMESSAGECREATOR_H

#include "mimcvenginemessagecreator.h"


// CLASS DECLARATION

/**
 *  Creator class for messages.
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */

class CIMCVEngineMessageCreator : public CBase, public MIMCVEngineMessageCreator
    {
    public: // Construction
		
		/**
		 * Construction
		 * @return Pointer to new created instance of CIMCVEngineMessageCreator
		 */
		static CIMCVEngineMessageCreator* NewL();
		
	
		/**
		 * Destruction
		 */
		~CIMCVEngineMessageCreator();
		
	private: // Construction
	
		/**
		 * Constructor
		 */
		CIMCVEngineMessageCreator();

	private: // From MIMCVEngineMessageCreator

		/**
		 * @see MIMCVEngineMessageCreator
		 */
		MIMCVEngineMessage* CreateMessageL( const SMessageData& aData ) const;

	     
        /**
		 * Create failed text message.
		 * @param MIMCVEngineMessage, which has been failed to deliver
		 * @return new MIMCVEngineMessage. Ownership is passed to caller
		 */
		MIMCVEngineMessage* CreateFailedMessageL( 
                                     const MIMCVEngineMessage* pSource ) const ;
	                
	private: // New helper methods
		
		/**
		 * Create text message.
		 * @param aData Data for message.
		 * @return new MIMCVEngineMessage. Ownership is passed to caller
		 */
		MIMCVEngineMessage* CreateTextMessageL( const SMessageData& aData ) const;
		
	
	
    
    };
    
#endif      // CIMCVENGINEMESSAGECREATOR_H

// End of File
