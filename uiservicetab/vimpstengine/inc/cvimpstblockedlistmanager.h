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
* Description:  manages the locally maintained blocked list
*
*/
#ifndef __CVIMPSTBLOCKEDLISTMANAGER_H
#define __CVIMPSTBLOCKEDLISTMANAGER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION
/**
 * List & presence implementation.
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTBlockedListManager ) 						 	
    {
    //for testcases
    friend class T_VimpstEnginePresenceSubService;
    
    public:  // Two-phased constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CVIMPSTBlockedListManager* NewL();
        
        
        /**
         * Two-phased constructor.
         */										
        static CVIMPSTBlockedListManager* NewLC();

        /**
         * C++ Destructor.
         */
        virtual ~CVIMPSTBlockedListManager();
        
        /**
        *Add aContact to Blocked list.
        *@param aContact : contact to be added to blocked list.
        */
        void AddToBlockedListL(const TDesC& aContact);

       /**
        *Remove aContact to Blocked list.
        *@param aContact : contact to be Removed to blocked list.
        */
        void RemoveFromBlockListL(const TDesC& aContact);


        /**
        *Add aContact to Blocked list.
        *@return: list of blocked contacts.
        */
 		RPointerArray<HBufC>* BlockedList();
 		
 		/**
 		* Reset the blocked list to empty, and free memory of all contact in blocked list.
 		*/
 		void ResetL();

    private:
    	
    	//private constructor
    	CVIMPSTBlockedListManager();
    	
        void ConstructL();

    private:
    	RPointerArray<HBufC> iBlockedList;
    	
    };
    
#endif      //__CVIMPSTBLOCKEDLISTMANAGER_H

// End of File
