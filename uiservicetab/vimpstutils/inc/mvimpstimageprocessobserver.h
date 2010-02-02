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
* Description:  Interface for observing content processors
*
*/


#ifndef MCACONTENTPROCESSOBSERVER_H
#define MCACONTENTPROCESSOBSERVER_H

//	FORWARD CLASS DECLERATIONS


// CLASS DECLARATION

/**
 *  Interface for observing content processors
 *
 *  @lib vimpstutils.dll
 */
class MCAImageProcessObserver
	{
	public: // Interface

        /**
         * Handle processing completion. Called by content processor when 
         * processing is ready.
         * @param aStatus, Status of processing. KErrNone -> OK.
         */
        virtual void HandleProcessingCompleteL( TInt aStatus ) = 0;

	protected: // For protection.

		/**
		 * Destructor
		 */
		virtual ~MCAImageProcessObserver(){}
	};

#endif      // MCACONTENTPROCESSOBSERVER_H

// End of File
