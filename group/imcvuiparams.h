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
* Description:  common params class shared between servicetab and conversationview
*
*/


#ifndef TIMCVUIPARAMS_H
#define TIMCVUIPARAMS_H

#include <e32base.h>


/**
 * constant to define the size of the contact link
 */
#define KMaxPackedContactLinkLength 300
#define KMaxLength 100
/**
 * Using TXmppMessages and TXmppErrors enum's
 */
const TUid KConversationViewAppUid = {0x20012425};
    
/**
 *  Class for passing parameters to cv application.
 *  
 *  The objects of this class are wrapped into package buffer which is sent
 *  from external process and handled in cv applications ProcessMessage()
 *
 *  @since S60 v5.0
 */
class TIMCVUiParams
    {
	public: // Data
	
	inline TIMCVUiParams()
			{
			iAppUid = 0;
			iViewUid = 0;
			iServiceId =0;	
			iUpdate = EFalse;
			}
	
	/**
	 * Buddy ID
	 */
	TBuf<KMaxLength> iBuddyId;
	
		/**
	 * Buddy ID
	 */
	TBuf<KMaxLength> iBuddyName;

	/**
	 * Using TXmppMessages and TXmppErrors enum's
	 */
	TInt iCommand;
	
	/**
	 * Using system wide error codes
	 */
	TInt iErrorCode;
	
	/**
	 * Calling application Uid
	 */
	TInt iAppUid;
	
	/**
	 * Calling application's ViewUid
	 */
	TInt iViewUid;
	
	
	//service id;
	TUint32 iServiceId;
	
	//contact link 
	TBuf8<KMaxPackedContactLinkLength> iContactLink;
	
	// bool varaible to differentiate if its update
    // of the display name or user id.
    TBool iUpdate;
	
	};


#endif // TIMCVUIPARAMS_H

// End of File
