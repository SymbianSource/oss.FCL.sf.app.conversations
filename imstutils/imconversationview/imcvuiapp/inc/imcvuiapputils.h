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
* Description:  utils helper class
*
*/


#ifndef __IMCVUIAPPUTILS_H__
#define __IMCVUIAPPUTILS_H__

#include    <e32base.h>

//	FORWARD CLASS DECLERATIONS

// CLASS DECLARATION

/**
 *  Utils class . 
 */
 
class IMCVUiAppUtils   
	{
	public: // new methods
     	
       /**
         * @param aUid Uid for cenrep
         * @param aKey Central repository key
         * @return Value for key
         */
     static TUint32 CRKeyL( TUid aUid, TUint32 aKey );
     
     
	   /**
	     * Variated version of AknLayoutUtils::PenEnabled()
	     * Use this to preserver 3.0 compatibility
	     * @return ETrue - Pen support on
	     *         EFalse - Pen support off
	     */
    static TBool PenEnabled();
  
           
	};

#endif      // __IMCVUIAPPUTILS_H__

// End of File
