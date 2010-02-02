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
* Description:  observer class for contact 
*
*/


#ifndef MVIMPSTDETAILSPRESENCEHANDLER_H
#define MVIMPSTDETAILSPRESENCEHANDLER_H

#include "tvimpstenums.h"

/**
 * presence  observer interface.
 * @lib vimpstdetailsviewplugin.dll
 * @since 5.0
 */
class MVIMPSTDetailsPresenceObserver
    {
    public: // Interface

        /** 
	     * notification call back from presence handler about presence change
	     * @since s60 v5.0
	     */
        virtual void HandlePresenceChangedL() = 0 ;

    protected: // Disabled functions
        /**
         * virtual destructor.
         * @since 5.0
         */  
        virtual ~MVIMPSTDetailsPresenceObserver()
        {}
    };


/**
 * presence  handlere interface.
 * @lib vimpstdetailsviewplugin.dll
 * @since 5.0
 */
class MVIMPSTDetailsPresenceHandler
	{
	public: // Interface
	
		/**
		* fetch presence from cache
		* @since S60 v5.0
		*/
		virtual void SubscribePresenceL(const TDesC& aUserId ) = 0 ;


		/**
		* return the presence status
		* @since S60 v5.0
		*/
		virtual TVIMPSTEnums::TOnlineStatus GetPresenceStatus() = 0 ;

		/**
	     * @param aIndex , index of the control
	     * @return  Label for control
	     * @since S60 v5.0
	     */
		virtual const TDesC& GetHeaderLabelDataL( TInt aIndex ) = 0 ;
		
		 /**
	     * @return the presence status message
	     * @since S60 v5.0
	     */
	    virtual const TDesC& GetStatusMessageL() = 0;
	    
        /**
        * function to get supported or not 
        * @since S60 v5.0
        * @return ETrue if supported
        */
	    virtual TBool IsSupported() const  = 0 ;
   
	public: // Disabled functions
		/**
		* virtual destructor.
		*/  
		virtual ~MVIMPSTDetailsPresenceHandler()
			{}
	};


#endif // MVIMPSTDETAILSPRESENCEHANDLER_H

// End of File

