/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  For handling memory status.
*
*/


#ifndef __MIMCVEngineBUFFERMEMORYHANDLER_H__
#define __MIMCVEngineBUFFERMEMORYHANDLER_H__

#include <e32base.h>

//	FORWARD CLASS DECLERATIONS
class MIMCVEngineChatInterface;

// CLASS DECLARATION

/**
 *  For handling memory status.
 *
 *  @lib imcvuiengine.dll
 *  @since 5.0
 */
class MIMCVEngineBufferMemoryHandler
	{
	public:
		/**
		 * Free memory if needed.
		 * @param aSize Size needed.
		 * @return ETrue, if successfull, EFalse if not.
		 */
		virtual TBool FreeMemoryIfNeededL( TInt aSize ) = 0;
		
		
	};
#endif      // __MIMCVEngineBUFFERMEMORYHANDLER_H__

// End of File


