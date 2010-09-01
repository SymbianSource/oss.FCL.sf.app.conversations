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
* Description:  observer class for contact retrieval
*
*/


#ifndef MVIMPSTDETAILSVIEWCONTACTOBSERVER_H
#define MVIMPSTDETAILSVIEWCONTACTOBSERVER_H

class MVPbkStoreContact;

// CLASS DECLARATION

/**
 * contact  observer interface.
 * @lib vimpstdetailsviewplugin.dll
 * @since 5.0
 */
class MVIMPSTDetailsViewContactObserver
    {
    public: // Interface

        /**
         * Notifies the contact observer that the contact is retrieved
         * @param aContact  contact which is retrieved
         */
        virtual void HandleContactReadyL( MVPbkStoreContact& aContact ) = 0 ;
        
		/**
		* call back from contact manager
		* @param aContact, a contact fetched from store
		*/	    
		virtual void HandleContactUnavailableL()  = 0  ;
	

    protected: // Disabled functions
        /**
         * virtual destructor.
         */  
        virtual ~MVIMPSTDetailsViewContactObserver()
        {}
    };

#endif // MVIMPSTDETAILSVIEWCONTACTOBSERVER_H

// End of File
