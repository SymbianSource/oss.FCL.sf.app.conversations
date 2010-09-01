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
* Description:  This class subscribes and listens to rproperty defined in cinstantmsgindicator.h
*
*/


#ifndef __CIMSTATUSPANEINDICATOR_H
#define __CIMSTATUSPANEINDICATOR_H

#include "imstatuspaneindicatorpluginuids.h"

//system includes
#include <e32base.h>
#include <e32property.h>
#include <cinstantmsgindicator.h>

//Forward Declarations

#define KMAXLENGTH 512
/**
*  IM Indicator Plug-in implementation
*
*  @lib ImIndicatorPlugin
*  @since 3.2
*/
class CIMStatusPaneIndicator : public CActive
                                        
    {
        
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CIMStatusPaneIndicator* NewL();

        /**
        * Destructor.
        */
        ~CIMStatusPaneIndicator();
    public: // New Functions
        
        /**
         * Returns the message count
         */
        TInt MsgCount();
        /**
         * Returns the service id
         */
        TInt ServiceId();
        /**
         * returns the message sender id
         */
        const TDesC& SenderId();
        
        /**
         * returns the number of p-2-p chats.
         */
        TInt IsMultipleSenders();
        
    private:

        /**
        * C++ default constructor.
        */
        CIMStatusPaneIndicator();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        
        /**
         * from CActive
         */
        void RunL();
        
        /**
         * from CActive
         */
        void DoCancel();

    private: // data
        
        // count of p-2-p chats
        TInt iMultipleSenders;

        // owned: object of the Rproperty
        RProperty   iProperty;
        
        // count of number of new messages
        TInt iMsgCount;
        
        // service id
        TInt iServiceId;
        
        // owned: message sender id.
        HBufC* iSenderId;
        
        
    };

#endif      // __CIMSTATUSPANEINDICATOR_H

// End of File
