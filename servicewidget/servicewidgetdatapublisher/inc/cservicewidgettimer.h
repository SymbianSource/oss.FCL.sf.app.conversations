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
* Description:  Event listener implementation presence
*
*/


#ifndef CSERVICEWIDGETTIMER_H
#define CSERVICEWIDGETTIMER_H

#include <e32base.h>

// forward declaration
class MServiceWidgetTimerObserver;
// class declaration

/**
 * @class timer class
 * @since 5.0
 */
 
class CServiceWidgetTimer : public CActive
    {
public :
        // Constructor
        /**
         * Creates a new CServiceWidgetTimer.
         * @param aObserver, reference to timer 
         * @return New instance of this class         
         */
        static CServiceWidgetTimer* NewL(MServiceWidgetTimerObserver& aObserver );
        
          /**
           * Standard C++ destructor.
           */
         ~CServiceWidgetTimer();
         
         /**
         * start listening
         * @param a time interval
         */
         void StartListening(TTimeIntervalMicroSeconds32 anInterval);
         
          
         /**
         * stop listening
         */
         void StopListening();     
                 
private : // From CActive

        /**
        *  RunL
        */
        void RunL();  
        
        /**
        *  DoCancel
        */
        void DoCancel();

        
private: // Implementation
       
       /**
        * Standard C++ constructor
        * @param aObserver, reference to timer
       */
        CServiceWidgetTimer(MServiceWidgetTimerObserver& aObserver );

       /**
        * Performs the 2nd phase of construction.
        */
       void ConstructL();
       
private :
    	    
       	// not owns : reference to contact update observer
        MServiceWidgetTimerObserver& iObserver;
        
		/**
		 *  owns : timer to trigger message queue uptation checking
		 */
		RTimer iTimer;
	
	    };

#endif /*CSERVICEWIDGETTIMER_H*/

//end of file
