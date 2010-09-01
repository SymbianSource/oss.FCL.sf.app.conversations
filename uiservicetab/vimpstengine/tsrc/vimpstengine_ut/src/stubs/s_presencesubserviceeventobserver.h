/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presencesubserviceeventobserver.h
*
*/
#include "mvimpstenginepresencesubserviceeventobserver.h"
#include <e32base.h>

class MyPresenceSubServiceEventObserverStub : public MVIMPSTEnginePresenceSubServiceEventObserver,
											  public CBase
	{
	public :
		
		static MyPresenceSubServiceEventObserverStub* MyPresenceSubServiceEventObserverStub::NewL()
		    {
		    MyPresenceSubServiceEventObserverStub* self = new (ELeave) MyPresenceSubServiceEventObserverStub();
		    return self;
		    }

		MyPresenceSubServiceEventObserverStub::~MyPresenceSubServiceEventObserverStub()
	    	{
	    	
	    	}
    	
   	public:	
		/**
		 * observer to handle own presence change notifications
		 *
		 * @since s60 5.0
		 * @param aStatus current status.
		 * @param aStatusText status text to be shown.
		 */
		 void HandleOwnPresenceChangeEventL(TVIMPSTEnums::TOnlineStatus /*aStatus*/, const TDesC& /*aStatusText*/ )
			 {
			 	
			 }

       
		 void  HandleAddRequestEventL(TVIMPSTEnums::TOperationType aType,const TDesC& /*aRequester*/ ,const TDesC& aRequestorDisplayName) 
		     {
		     
		     }
	};
	               


