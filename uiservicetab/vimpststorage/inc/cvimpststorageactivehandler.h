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
* Description:  Active  class implementation to wait multiple refresh.
*
*/



#ifndef CVIMPSTSTORAGEACTIVEHANDLER_H
#define CVIMPSTSTORAGEACTIVEHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "tvimpstenums.h"
// FORWARD DECLARATIONS
class MVIMPSTStorageActiveObserver;
class MVIMPSTStorageContactList;
class MVIMPSTStorageContact;
// CLASS DECLARATION

/**
*  The CVIMPSTStorageRefreshTimer class handles the waiting functionality for the
*  storage manager to wait before calling observer in case of multiple
*  contacts adding.
*
*  @lib vimpststorage.dll
*  @since 5.0
*/
NONSHARABLE_CLASS ( CVIMPSTStorageActiveHandler ) : public CTimer
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aObserver The observer to which the notify about the 
        *                  timer events is
		* given.
        */
        static CVIMPSTStorageActiveHandler * NewL( MVIMPSTStorageActiveObserver* aObserver );

        /**
        * Destructor.
        */
        ~CVIMPSTStorageActiveHandler();

    protected:  // Functions from CActive

		/**
		* @see CActive
		*/
        void RunL();

		/**
		* @see CActive
		*/
		TInt RunError( TInt aError );
		
			/**
		* @see CActive
		*/
		void DoCancel();

	public: // new methods

		/**
		 * Starts timer. If timer is already running, stops timer and 
		 * restarts it.
		 */
		void IssueRequest(TVIMPSTEnums::TVIMPSTStorgaeEventType aType,
						  MVIMPSTStorageContactList *aList, 
                          MVIMPSTStorageContact* aContact,
                          TInt aContactIndex );

    private:

        /**
        * C++ default constructor.
        */
        CVIMPSTStorageActiveHandler( MVIMPSTStorageActiveObserver* aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // A pointer to the observer
        MVIMPSTStorageActiveObserver* iObserver;			/// Not owned
        // Event type 
        TVIMPSTEnums::TVIMPSTStorgaeEventType iEventType;
        // Not owns a contact List
		MVIMPSTStorageContactList* iList; 
		// Not owns contact
	    MVIMPSTStorageContact* iContact;
	    // index of contact
	    TInt iContactIndex;
    };

#endif      // CVIMPSTSTORAGEACTIVEHANDLER_H

// End of File
