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
* Description:  Implementation of Search Feature
*
*/

#ifndef __CVIMPSTENGINESEARCHMGREXTENTION_H
#define __CVIMPSTENGINESEARCHMGREXTENTION_H

//  INCLUDES
#include <e32base.h>

#include "mvimpstenginesearchmgrextention.h"
#include "mvimpstenginesessioncntxtobserver.h"
#include "tvimpstenums.h"
#include <searchelement.h>

// FORWARD DECLARATIONS           
class CVIMPSTEngineRequestMapper;
class MXIMPContext;
class MSearchFeature;
class MSearch;
class MSearchKeysEvent;

   
// CLASS DECLARATION
/**
 * search  implementation.
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineSearchMgrExtention ) : public CBase,
							   public MVIMPSTEngineSearchMgrExtention,
							   public MVIMPSTEngineSessionCntxtObserver
						 	
    {
    
    
	public:  // Two-phased constructors and destructor

		/**
		* Two-phased constructor.    
		* @param aPresContext: presence context     
		* @param aRequestMapper: Request Mapper for XIMP Events
		* @return CVIMPSTEngineSearchMgrExtention* a new instance of this class
		*/

		static CVIMPSTEngineSearchMgrExtention* NewL( MXIMPContext& aPresenceCtx,
											CVIMPSTEngineRequestMapper& aRequestMapper       										
											);
		/**
		* Two-phased constructor.
		* @param aPresContext: presence context
		* @param aRequestMapper: Request Mapper for XIMP Events
		* @return CVIMPSTEngineSearchMgrExtention* a new instance of this class
		*/										
		static CVIMPSTEngineSearchMgrExtention* NewLC( MXIMPContext& aPresenceCtx,
											CVIMPSTEngineRequestMapper& aRequestMapper);

		/**
		* C++ Destructor.
		*/
		virtual ~CVIMPSTEngineSearchMgrExtention();


	private:

		/**
		* default constructor    
		* @param aPresContext: presence context     
		* @param aRequestMapper: Request Mapper for XIMP Events
		*/
		CVIMPSTEngineSearchMgrExtention( MXIMPContext& aPresenceCtx,
								CVIMPSTEngineRequestMapper& aRequestMapper );

		/**
		* second phase ConstructL 
		*/
		void ConstructL();

		/**
		* convert the client search keys to ximpfw search keys
		* @parma aKey client's search key
		* @return the ximpfw search key 
		*/
		TSearchKey ConverttoTSearchKey(TVIMPSTEnums::TVIMPSTSearchKey aKey) ;	

		/**
		* ConverttoTVIMPSTSearchKey
		* @param aKey: ximpfw key to be conversted
		*/
		TVIMPSTEnums::TVIMPSTSearchKey ConverttoTVIMPSTSearchKey( TSearchKey aKey);

		/**
		* ProcessSearchKeysEventL
		* @param aSearchKeysEvent: ximpfw KeysEvent to be process
		*/
		void ProcessSearchKeysEventL(const MSearchKeysEvent* aSearchKeysEvent); 
	 	
	public: // New methods MVIMPSTEngineSearchMgrExtention   	
	 
		/**
		* RegisterSearchObserver registers the observer for search notifications
		* @param: aObserver to be registered.
		*/
		void RegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver);

		/**
		* UnRegisterSearchObserver registers the observer for search notifications
		* @param: aObserver to be registered.
		*/
		void  UnRegisterObserver(MVIMPSTEngineSearchExtentionEventObserver* aObserver);  

		/**
		* SearchContactsL - Used to search for contacts
		* @param aKeyDataArray ,array of all search key and entered data 
		* return TInt error if any
		*/
		TInt SearchContactsL(RArray<TVIMPSTSearchKeyData>& aKeyDataArray ) ;
		/**
		* SubscribeForSearchKeysL
		* subscribe for search keys
		* @return TInt error if any
		*/
		TInt SubscribeForSearchKeysL()  ;         


	public:

		/**
		* See MVIMPSTEngineExtentionFeatures
		*/
		TVIMPSTEnums::FeatureSupport IsSupported() const;


		/**
		* See MVIMPSTEngineExtentionFeatures
		*/
		TVIMPSTEnums::ExtentionType Type() const;    	
		
		/**
         * See MVIMPSTEngineExtentionFeatures
         */
        void  SetSupported(TVIMPSTEnums::FeatureSupport aSupported) ;


	public:
		/**
		 * See MVIMPSTEngineSessionCntxtObserver		 
		 */ 
		void HandleSessionContextEventL( const MXIMPContext& aContext, 
                                          const MXIMPBase& aEvent,
                                          TXimpOperation aXimpOperation = EVIMPSTXimpOperationNoOperation);	                       

	private:

		// not own 
		MXIMPContext& 		iPresenceCtx;    	

		//doesnt own
		CVIMPSTEngineRequestMapper& iRequestMapper; 
		
		//owns , search suppoted or not
		TVIMPSTEnums::FeatureSupport iSearchSupported;    

		//owned, search feature ximp interface
		MSearchFeature* iSearchFeature;

		//doesnt own
		MSearch* iSearch;  
		   
		// indicate that already called
		TBool iAlreadySubscibed;

		//not owned
		RPointerArray<MVIMPSTEngineSearchExtentionEventObserver> iSearchObservers;
		
		//Stores the XIMP request ErrCode
		TInt iReqResult;

    };
    
#endif      //__CVIMPSTENGINESEARCHMGREXTENTION_H

// End of File
