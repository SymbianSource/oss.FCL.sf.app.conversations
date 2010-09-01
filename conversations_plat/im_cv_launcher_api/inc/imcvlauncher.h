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
* Description:  launcher class for launching the conversation view
*
*/



#ifndef IMCVLAUNCHER_H
#define IMCVLAUNCHER_H

#include <e32base.h>
#include <vwsdef.h>
class    MVPbkContactLink;
//  INCLUDES
/**
*  IM conversationview launcher implementation class
*
*  @lib imcvlauncher
*  @since 5.0
*/
class IMCVLauncher
    {

    public:  
    
		/**
		* LaunchImConversationViewL, launches the imconversation view
        * @param aActiveViewId , active view details
        * @param aServiceId - ServiceId        
        * @param aRecipientUserId , recipient user id used to send the message
        * @Returns Error Code
	    */
	    
		IMPORT_C static TInt LaunchImConversationViewL(TVwsViewId aActiveViewId,
														TInt aServiceId,
														const TDesC& aRecipientUserId ,
														const TDesC& aDisplayName = KNullDesC, 
														MVPbkContactLink* aContactLink = NULL 
														) ;
														
		static TInt PropertySettingsIdL(TUint32 aServiceId ) ;
										
	private:
		/**
		* CheckValidServiceL, Checks whether the service is a Valid IM service         
        * @param aServiceId - ServiceId              
        * @Returns TBool - ETrue if Valid Service, else EFalse
	    */
		static TBool CheckValidServiceL(TInt aServiceId);
		
    };

#endif      // CIMCVLAUNCHER_H

// End of File
