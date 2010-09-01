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
* Description: this class is the implementation of the plugin
*
*/
// This file defines the API for imstatuspaneindicator.dll

#ifndef __INSTANTMSGINDICATORPLUGINIMPLEMENTATION_H__
#define __INSTNATMSGINDICATORPLUGINIMPLEMENTATION_H__

//  Include Files

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <cinstantmsgindicator.h>
#include "instantmsgindicatoruids.h"


//Forward Declarations

class CAknSmallIndicator;
class CIMAlertManager;
//  Class Definitions

class CInstantMsgIndicatorPluginImplementation : public CInstantMsgIndicator
    {
public:
  // new functions
    
       /**
		 * NewLC two phase constructor.
		 * @param aServiceId, service id
		 */
    static CInstantMsgIndicatorPluginImplementation* NewL( );
    
        /**
		 * NewLC two phase constructor.
		 * @param aServiceId, service id
		 */
    static CInstantMsgIndicatorPluginImplementation* NewLC( );
    
       /**
		 * c++ default destructor
		 */
    virtual ~CInstantMsgIndicatorPluginImplementation();

public: 


        /**
		 * sets/resets the status pane indicator
		 * @param aNewMessages, new message count
		 * @param aSeriveId, service id.
		 * @param aSenderId, message sender id.
		 * @param aMultipleSender, count of number of P-2_p chats.
		 * @param aCloseConversation, indicates that this even came from closing the 
		 * conversation and hence no need to paly the tone if there were already new messages.
		 */
     void MessageInfoL(TInt aNewMessages, TInt aSeriveId, const TDesC& aSenderId, 
                       TInt aMultipleSender,TBool aCloseConversation = EFalse);
 

    
private: // new functions


        /**
		 * C++ constructor.
		 */
        CInstantMsgIndicatorPluginImplementation();
    
        /**
		 * Second phase construction
		 * ConstructL
		 */
        void ConstructL();
        
        /*
         * Show IM indicator.
         */
        void ShowIMIndicatorL(TInt aNewMessages, TInt aSeriveId, const TDesC& aSenderId, TInt aMultipleSender);

        /*
         * play the msg tone.
         */
        void PlayMsgToneL(TInt aNewMessages,TInt aSeriveId);
private: // data
    
    // owned, object of status indicator.
    CAknSmallIndicator* iIndicator;

    CIMAlertManager* iIMAlertMgr;
    };

#endif  // __INSTANTMSGINDICATORPLUGINIMPLEMENTATION_H__

