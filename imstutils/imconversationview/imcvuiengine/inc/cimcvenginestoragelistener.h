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
* Description:  active object class for interacting with servicetab for link and Display Name
*
*/

#ifndef CIMCVENGINESTORAGELISTENER_H
#define CIMCVENGINESTORAGELISTENER_H

#include <e32base.h>
#include <e32property.h>
#include <AknWaitDialog.h>
#include "cimcvkeys.h"

_LIT(KDelimiter, ":");
/**
 * constant to define the size of the contact link
 */
#define KMaxDisplayNameContactLinkLength 512
/**
 * constant to define the size of the contact link
 */
#define KMaxServiceIdLength 3

//forword declaration.
class MIMCVEngineStorageObserver
        {
    public : 
        virtual  void HandleServiceBuddyDetailsFetchedL(TInt aServiceId, 
                const TDesC& aXspId,
                const TDesC8 & aContactLink,
                const TDesC & aDisplayName ) = 0 ;
   
        };

class CIMCVEngineStorageListener : public CActive
    {
public :
        // Constructor
        /**
         * Creates a new CIMCVEngineStorageListener.
         * @param aTabbedView, reference to tabbedview
         * @return New instance of this class         
         */
        static CIMCVEngineStorageListener* NewL(MIMCVEngineStorageObserver& aObserver);
        
          /**
           * Standard C++ destructor.
           */
         ~CIMCVEngineStorageListener();
         
         
         /**
          * Get the Display name and Contact link information.
          * @param aServiceId, service id for which info requested.
          * @param aXspID, xsp Id of contact.
          */
         void GetContactLinkInfoL(TInt aServiceId, const TDesC& aXspID);
         
private : // From CActive

        /**
        *  RunL
        */
        void RunL();  
        
        /**
        *  DoCancel
        */
        void DoCancel();
        
        /*
         * RunError
         */
        TInt RunError ( TInt aError );

private: // Implementation
       /**
        * Standard C++ constructor
        * @param aTabbedView, reference to tabbedview
        */
        CIMCVEngineStorageListener(MIMCVEngineStorageObserver& aObserver);

       /**
        * Performs the 2nd phase of construction.
        */
       void ConstructL();
       
       /**
        * start listening
        */
       void StartListening();

       /**
        * stop listening
        */
       void StopListening();
       
private :
    
    // owns , property to use
    RProperty iProperty;    
    // not owned
    MIMCVEngineStorageObserver& iObserver;
    
    // own active sheduler 
    CActiveSchedulerWait    iWait;
 	
    };

#endif /*CIMCVENGINESTORAGELISTENER_H*/
