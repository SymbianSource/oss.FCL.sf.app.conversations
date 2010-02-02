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
* Description:  Publish and Subscribe implementation of notify services
*
*/


#ifndef CIMALERTNOTIFIERIMPL_H
#define CIMALERTNOTIFIERIMPL_H

//  INCLUDES
#include "mimalertnotifierinterface.h"
#include "mimalertpropertynotificationobserver.h"

// FORWARD DECLARATIONS
class MIMAlertNotifierKeyObserver;
class CIMAlertPropertyObserver;

// CLASS DECLARATION
/**
*  Class for notifying key changes, reading and changing the values of 
*  Pub&Sub keys.
*  
*  @since Series 60 Platform 3.0
*/
class CIMAlertNotifierImpl : public CBase,
						public MIMAlertNotifierInterface,
						public MIMAlertPropertyNotificationObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CIMAlertNotifierImpl* NewL( MIMAlertNotifierKeyObserver* aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CIMAlertNotifierImpl();

    public: // Functions from base classes

        TInt SetInt( TNotifyKey aKey, TInt aValue );
		TInt GetInt( TNotifyKey aKey, TInt& aValue );
		TInt ListenKeyChanges( TNotifyKey aKey );		

    private: // From MIMAlertPropertyNotificationObserver
    
        /**
        * @see MIMAlertPropertyNotificationObserver
        */        
        void HandlePropertyNotificationEventL( TUid aCategory, TUint aKey );
        
    private:

        /**
        * C++ default constructor.
        */
        CIMAlertNotifierImpl( MIMAlertNotifierKeyObserver* aObserver );

      
    private:    // Data
    
        MIMAlertNotifierKeyObserver* iObserver;
		CIMAlertPropertyObserver* iPropWatcher;
	};

#endif      // CIMALERTNOTIFIERIMPL_H 
            
// End of File
