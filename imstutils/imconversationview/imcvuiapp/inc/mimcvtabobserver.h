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
* Description:  Interface for Tab observer
*
*/


#ifndef MIMCVTabObserver_H
#define MIMCVTabObserver_H


/**
 *  Tab observer interface to the UI. 
 *  @lib imcvuiapp.exe
 *  @since 5.0
 */
 
class MIMCVTabObserver
	{
	

	public:
		
		/** Helper Function
		 * This function is to be called when switching between tabs
		 * @param aChatId - Switch to this chat item
		 * @param aTabIndex - Index of the tab
		 */	
		virtual void SwitchTabL(TInt aActiveServiceId , const TDesC& aChatId, const TInt aTabIndex) = 0;

		/**
		 * Destructor
		 */
		virtual ~MIMCVTabObserver(){}

	};

#endif //MIMCVTabObserver_H
