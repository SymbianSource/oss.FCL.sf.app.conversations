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
* Description:  Contact data container observer interface
*
*/



#ifndef _MVIMPSTENGINESESSIONCNTXTOBSERVER_H
#define _MVIMPSTENGINESESSIONCNTXTOBSERVER_H

#include "tvimpstenums.h"

class MXIMPContext;
class MXIMPBase;

// CLASS DECLARATION

/**
 *  Interface for forwarding the ximpfw events.
 *
 *  @lib vimpstengine.dll
 *  @since 5.0
 */
	
class MVIMPSTEngineSessionCntxtObserver
{
	
	public:
	    
	    /**
	    * Observer Event for forwarding the ximp events.
	    * @param aContext - ximpfw context
	    * @param aEvent - ximpfw event
	    * @param aXimpOperation - XIMPOeration defined in tvimpstenums.h
	    */ 
	    virtual void HandleSessionContextEventL( const MXIMPContext& aContext, 
	    								const MXIMPBase& aEvent,
	    								TXimpOperation aXimpOperation = EVIMPSTXimpOperationNoOperation
	    								) = 0;
	    
	    
};

#endif //_MVIMPSTENGINESESSIONCNTXTOBSERVER_H
