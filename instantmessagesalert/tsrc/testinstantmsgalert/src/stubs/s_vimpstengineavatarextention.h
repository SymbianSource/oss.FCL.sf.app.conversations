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
* Description: s_vimpstengineavatarextention.h
*
*/

#ifndef __CVIMPSTENGINEAVATAREXTENTION_H
#define __CVIMPSTENGINEAVATAREXTENTION_H

//  INCLUDES
#include <e32base.h>

#include "mvimpstengineavatarextention.h"
#include "TVIMPSTEnums.h"
#include "mvimpstenginesessioncntxtobserver.h"
#include "cvimpstengineimagehandler.h"


// FORWARD DECLARATIONS           
class CVIMPSTEngineRequestMapper;
class MPresenceFeatures;
class MXIMPContext;
class MXIMPIdentity;


  
// CLASS DECLARATION
/**
 * This Class implements the Contact Management Extention Feature
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineAvatarExtention ) : public CBase,							   
							   public MVIMPSTEngineAvatarExtention,
							   public MVIMPSTEngineSessionCntxtObserver
						 	
    {
    
    
    public:  // Two-phased constructors and destructor

        /**
         * Two-phased constructor.
         * @param aFeatures - Presentity Group
         * @param aRequestMapper: Request Mapper for XIMP Events
         */
        
        static CVIMPSTEngineAvatarExtention* NewL(
        					TInt aServiceId,
        					MPresenceFeatures& aFeatures,
							CVIMPSTEngineRequestMapper& aRequestMapper);
        /**
         * Two-phased constructor.
         * @param aFeatures - Presentity Group
         * @param aRequestMapper: Request Mapper for XIMP Events
         */										
        static CVIMPSTEngineAvatarExtention* NewLC(
        					TInt aServiceId,
        					MPresenceFeatures& aFeatures,
							CVIMPSTEngineRequestMapper& aRequestMapper);

        /**
         * C++ Destructor.
         */
        virtual ~CVIMPSTEngineAvatarExtention();
        
   	
    private:
		
        /**
         * default constructor
         * @param aFeatures - Presentity Group
         * @param aRequestMapper: event observer to get the notifications of the events.
         */
        CVIMPSTEngineAvatarExtention(
        					TInt aServiceId,
        					MPresenceFeatures& aFeatures,
							CVIMPSTEngineRequestMapper& aRequestMapper);
        
        
	 	
    public: // See MVIMPSTEngineAvatarExtention   	
		
        /**
	     * See MVIMPSTEngineAvatarExtention	     
		 */
         TInt UpdateAvtarToServerL(const TDesC& aFilename ,
         							const  TDesC8& aMimetype);
        
        	
	private:
		/**
		 * See MVIMPSTEngineSessionCntxtObserver		 
		 */ 
		void HandleSessionContextEventL( const MXIMPContext& aContext, 
                                          const MXIMPBase& aEvent,
                                          TXimpOperation aXimpOperation = EVIMPSTXimpOperationNoOperation);	                       

	public: // From MVIMPSTEngineExtentionFeatures
	
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

    		
    private:   	
        // serivice id
        TInt iServiceId;
        
        // not own 
	    MPresenceFeatures& iFeatures;	    
    	
       //doesnt own
        CVIMPSTEngineRequestMapper& iRequestMapper;         
    	 	
    	//Whether the extention Feature Supported or not
    	TVIMPSTEnums::FeatureSupport    iSupported; 

    	//Result code from waited request
	    TInt    iReqResult;
	    
    };
    
#endif      //__CVIMPSTENGINEAVATAREXTENTION_H

// End of File
