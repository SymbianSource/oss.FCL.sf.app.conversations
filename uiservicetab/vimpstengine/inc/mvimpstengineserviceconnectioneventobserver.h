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
* Description:  MVIMPSTEngineServiceConnectionEventObserver
*
*/



#ifndef _MVIMPSTENGINESERVICECONNECTIONEVENTOBSERVER_H
#define _MVIMPSTENGINESERVICECONNECTIONEVENTOBSERVER_H

class MVIMPSTEngineServiceConnectionEventObserver
	{
		
	public:
	 
		/**
		 * Observer interface for Listening to the Service Connectivity
		 * events		 
		 */ 
		virtual void  HandleServceConnectionEventL() = 0;

		/**
		 * virtual destructor.
		 */
		virtual ~MVIMPSTEngineServiceConnectionEventObserver()
		{}
		    
	}; 

#endif //_MVIMPSTENGINESERVICECONNECTIONEVENTOBSERVER_H  
