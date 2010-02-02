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
* Description:  Presence observer interface
*
*/



#ifndef _MVIMPSTENGINEPRESENCESUBSERVICEEVENTOBSERVER_H
#define _MVIMPSTENGINEPRESENCESUBSERVICEEVENTOBSERVER_H

#include "tvimpstenums.h"

/**
 * Chat observer.
 * This class is responsible for handling Presence events.
 * @lib vimpstengine.dll
 * @since 5.0
 */	
class MVIMPSTEnginePresenceSubServiceEventObserver
	{
	public:	

		/**
		* Observer Event for new add request
		* @parma aRequester requester for add request.
		*/ 
		virtual void  HandleAddRequestEventL(TVIMPSTEnums::TOperationType aType, const TDesC& aRequesterId, 
                                                const TDesC& aRequestorDisplayName  ) = 0;
		        
	    
	};

#endif //_MVIMPSTENGINEPRESENCESUBSERVICEEVENTOBSERVER_H
