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
* Description:  text message
*
*/


#ifndef CIMCVENGINETEXTMESSAGE_H
#define CIMCVENGINETEXTMESSAGE_H

#include <e32base.h>
#include "cimcvenginemessagebase.h"

// CLASS DECLARATION

/**
 *  Message containing text
 *
 *  @lib imcvengine.dll
 *  @since 5.0
 */
class CIMCVEngineTextMessage : public CIMCVEngineMessageBase
	{
	
    public: // Construction
		
		/**
		 * Construction
		 * @param aSender, sender user id
		 * @param aRecipient, recipient id
		 * @param aData, text message
		 * @return Pointer to new created instance of CIMCVEngineTextMessage
		 */
		static CIMCVEngineTextMessage* NewL( 
		                             const TDesC& aSender, 
		                             const TDesC& aRecipient, 
		                             const TDesC& aData );

		
		/**
		 * two phase construction
		 * @param aSender, sender user id
		 * @param aRecipient, recipient id
		 * @param aData, text message
		 * @return Pointer to new created instance of CIMCVEngineTextMessage
		 */
		static CIMCVEngineTextMessage* NewLC( 
		                              const TDesC& aSender, 
		                              const TDesC& aRecipient, 
		                              const TDesC& aData );

		/**
		 * Destruction
		 */
		~CIMCVEngineTextMessage();
		
	public :
	
		void SetFailedL();
		
	private: // Construction
	
		/**
		 * Constructor
		 */
		CIMCVEngineTextMessage();
		
		/**
		 * Constructor
		 * @param aData, text message
		 */
		void ConstructL( const TDesC& aData );

	private: // From MIMCVEngineMessage
		
		/**
		 * @see MIMCVEngineMessage
		 */
		TInt SizeInBytes() const;

		/**
		 * @see MIMCVEngineMessage
		 */
		TContentType ContentType() const;
		
    
	}; 

#endif      // CIMCVENGINETEXTMESSAGE_H

// End of File
