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
* Description:  Conversations observer interface
*
*/



#ifndef _MVIMPSTENGINEIMSUBSERVICEEVENTOBSERVER_H
#define _MVIMPSTENGINEIMSUBSERVICEEVENTOBSERVER_H

#include "tvimpstenums.h"

/**
 * Chat observer.
 * This class is responsible for handling received message events.
 * and reactive authorizations events and pass those to ui.
 * @lib vimpstengine.dll
 * @since 5.0
 */	
class MVIMPSTEngineIMSubServiceEventObserver
{
	
	public:
	    
	    /**
	    * Observer Event for new messages
	    * @param aSender sender of the message.
	    */ 
	    virtual void  HandleChatMessageEventL(TVIMPSTEnums::TIMEventType aEventType ,const TDesC& aSender) = 0;
	    	    
};

#endif //_MVIMPSTENGINEIMSUBSERVICEEVENTOBSERVER_H