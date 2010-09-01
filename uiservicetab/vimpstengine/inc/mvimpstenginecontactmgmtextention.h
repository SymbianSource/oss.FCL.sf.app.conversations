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
* Description:  Handles the Contact Management Operations
*  Part of     : IMUiserviceTab / vimpstengine
*
*/

#ifndef __MVIMPSTENGINECONTACTMGMTEXTENTION_H
#define __MVIMPSTENGINECONTACTMGMTEXTENTION_H

//  INCLUDES
#include <e32base.h>
#include "mvimpstengineextentionfeatures.h"
   
// CLASS DECLARATION
/**
 * Contact Management Implementation
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
class MVIMPSTEngineContactMgmtExtention :public MVIMPSTEngineExtentionFeatures
						 	
    {
	 
    public: // New methods MVIMPSTEngineContactMgmtExtention   	
		
		/**
         * Casts MVIMPSTEngineContactMgmtExtention to this interface.
         *
         * @precond aExtentionFeature.Type() == TVIMPSTEnums::EServerContactManagement
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aExtentionFeature    Base Class .
         * @return Casted inteface for the Contact Mgmt Feature
         */
        IMPORT_C static MVIMPSTEngineContactMgmtExtention& Cast(
                MVIMPSTEngineExtentionFeatures& aExtentionFeature);

        
        /**
         * Casts constMVIMPSTEngineContactMgmtExtention to this interface.
         *
         * @precond aExtentionFeature.Type() == TVIMPSTEnums::EServerContactManagement
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aExtentionFeature    Base Class .
         * @return Casted inteface for the Contact Mgmt Feature
         */        
        IMPORT_C static const MVIMPSTEngineContactMgmtExtention& Cast(
                const MVIMPSTEngineExtentionFeatures& aExtentionFeature);
                
        /**
	     * AddServerContactL
	     * to add the contact to Server thro XIMP
		 * @param aContactListID ,in to be added
		 * @param aUserId , to add.
		 * @param aNickname to added.
		 * @ return TInt error if any
		 */
         virtual TInt AddServerContactL(const TDesC& aContactList, 
         						const TDesC& aUserId,const TDesC& aNickname,
         						TBool aWaitToComplete = ETrue ) = 0;
        
        
         /**
	     * DeleteServerContactL
	     * to delete the given contact from server thro XIMP
		 * @param aContactListID ,from to delete.
		 * @param aUserId , to delete.
		 * @return TInt error if any
		 */
         virtual TInt DeleteServerContactL( const TDesC& aContactListId, 
         									const TDesC& aUserId ) = 0;
	
        
    };
    
#endif      //__MVIMPSTENGINECONTACTMGMTEXTENTION_H

// End of File
