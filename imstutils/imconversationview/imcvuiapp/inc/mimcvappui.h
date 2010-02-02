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
* Description:  Interface for AppUi class
*
*/



#ifndef MIMCVAPPUI_H
#define MIMCVAPPUI_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MIMCVAppResourceChangeObserver;
class MIMCVAppLayoutChangeObserver;

// CLASS DECLARATION

/**
*  Interface for AppUi class
*
*  @lib imcvuiapp.exe
*/

class MIMCVAppUi
    {
    public: // New functions

         virtual TDes& MbmFullPath()= 0;
		
		/**
		 * Adds layout change observer
		 * @param aObserver Pointer to observer
		 */
        virtual void AddLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver ) = 0;

		/**
		 * Removes layout observer
		 * @param aObserver Pointer to removed observer
		 */
        virtual void RemoveLayoutChangeObserver( MIMCVAppLayoutChangeObserver* aObserver ) = 0;
        
		/**
		 * Adds resource observer
		 * @param aObserver Pointer to observer
		 */
       virtual void AddResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver ) = 0;

		/**
		 * Removes resource observer
		 * @param aObserver Pointer to removed observer
		 */
        virtual void RemoveResourceChangeObserver( MIMCVAppResourceChangeObserver* aObserver ) = 0;
   };

#endif      // MIMCVAPPUI_H

// End of File
