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
* Description:  UI control context implementation.
*
*/

#ifndef __MVIMPSTENGINESEARCHMGREXTENTION_H
#define __MVIMPSTENGINESEARCHMGREXTENTION_H

#include "tvimpstenums.h"
#include "mvimpstengineextentionfeatures.h"
// CLASS DECLARATION
class MVIMPSTEngineSearchExtentionEventObserver;

/**
 *  Interface for search manager.
 *
 *  @lib vimpstengine.dll
 *  @since 5.0
 */
class MVIMPSTEngineSearchMgrExtention: public MVIMPSTEngineExtentionFeatures
    {
    public:  
		
		/**
         * Casts MVIMPSTEngineSearchMgrExtention to this interface.
         *
         * @precond aExtentionFeature.Type() == TVIMPSTEnums::ESearch
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aExtentionFeature    Base Class .
         * @return Casted inteface for the Search Feature
         */
        IMPORT_C static MVIMPSTEngineSearchMgrExtention& Cast(
                MVIMPSTEngineExtentionFeatures& aExtentionFeature);

        
        /**
         * Casts constMVIMPSTEngineSearchMgrExtention to this interface.
         *
         * @precond aExtentionFeature.Type() == TVIMPSTEnums::ESearch
         *          Panic(EPanicTypeMismatch)
         *          is raised if the precondition does not hold.
         * @param aExtentionFeature    Base Class .
         * @return Casted inteface for the Search Feature
         */        
        IMPORT_C static const MVIMPSTEngineSearchMgrExtention& Cast(
                const MVIMPSTEngineExtentionFeatures& aExtentionFeature);
		
		/**
	    * RegisterSearchObserver registers the observer for search notifications
		* @param: aObserver to be registered.
		*/
 		virtual void RegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver) = 0;

		/**
	    * UnRegisterSearchObserver registers the observer for search notifications
		* @param: aObserver to be registered.
		*/
		virtual void  UnRegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver ) = 0;
	
        /**
	     * SearchContactsL
	     * search a given string from server
		 * @param aKeyDataArray to be search
		 * @return TInt. error if any
		 */
         virtual TInt SearchContactsL( RArray<TVIMPSTSearchKeyData>& aKeyDataArray ) = 0;
         
         /**
	     * SubscribeForSearchKeysL
	     * subscribe for search keys
	     * @return TInt error if any
		 */
         virtual TInt SubscribeForSearchKeysL() = 0 ;         
         
             
	 public:

	    /**
	    * Destructor.
	    */
	    virtual ~MVIMPSTEngineSearchMgrExtention() {};
        
    };
    
#endif      //__MVIMPSTENGINESEARCHMGREXTENTION_H
//  End of File
