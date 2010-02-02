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
* Description:  Declaration for RProperty where conversation view login event handled.
*
*/

#ifndef CIMCVAPPLOGINEVENTLISTENER_H_
#define CIMCVAPPLOGINEVENTLISTENER_H_

#include <e32base.h>
#include <e32property.h>
#include <AknWaitDialog.h>

// forward declaration
class CIMCVEngine;
class CIMCVAppUi;
class CIMCVAppWaitNote;

class CIMCVAppLoginEventListener : public CActive
    {
public :
        // Constructor
        /**
         * Creates a new CIMCVAppLoginEventListener.
         * @param aTabbedView, reference to tabbedview
         * @return New instance of this class         
         */
        static CIMCVAppLoginEventListener* NewL(CIMCVEngine& aEngine, 
        			 							CIMCVAppUi& aAppUi,
        			 							const TDesC& aUserId);
        
          /**
           * Standard C++ destructor.
           */
         ~CIMCVAppLoginEventListener();
         
         /**
         * start listening
         */
         void StartListening();
         
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

public:

		TInt LoginL();
		
		void SetUserIdL( const TDesC& aUserId );
		
private:
		    
		void LoginCompletedL();	    
        
private: // Implementation
       /**
        * Standard C++ constructor
        * @param aTabbedView, reference to tabbedview
        */
        CIMCVAppLoginEventListener( CIMCVEngine& aEngine, 
        			 							CIMCVAppUi& aAppUi );

       /**
        * Performs the 2nd phase of construction.
        */
       void ConstructL(const TDesC& aUserId);
       
private :
    
    // owns , property to use
    RProperty iProperty;    
    
 	// processing note is visible
 	TBool iWaitVisible;
 	
 	// Doesn't own (destroys itself)
    CIMCVAppWaitNote* iWaitDialog;
    
    
    CActiveSchedulerWait iWait;    
    
    // not owned, reference to uiengine
    CIMCVEngine& iEngine ;
    
    // not owned., reference to appui	
    CIMCVAppUi& iAppUi;
    
    //user id - Owns
   	HBufC* iUserId;  
    
    //holds the error of the login operation
    TInt iErr;
        
       
    };

#endif /*CIMCVAPPLOGINEVENTLISTENER_H_*/
