/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares interface for recipient procedures in chat view
*
*/



#ifndef MIMCVRECIPIENTOBSERVER_H
#define MIMCVRECIPIENTOBSERVER_H

// CLASS DECLARATION

/**
*  Declares interface for recipient procedures in chat view
*  Interface between chat container and chat view
*
*  @lib chat.app
*  @since 1.2
*/
class MIMCVRecipientObserver
    {
    public:  

		/**
		 * Shows popup menu instead of application menu
		 */
		virtual void ShowPopUpMenuL() = 0;
	
	protected:

		virtual ~MIMCVRecipientObserver() {};

    };

#endif      // MIMCVRECIPIENTOBSERVER_H
            
// End of File
