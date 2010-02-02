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
* Description: Implements the Contact Management Extention Feature
*
*/

#ifndef __CVIMPSTENGINECONTACTMGMTEXTENTION_H
#define __CVIMPSTENGINECONTACTMGMTEXTENTION_H

//  INCLUDES
#include <e32base.h>

#include "mvimpstenginecontactmgmtextention.h"
#include "tvimpstenums.h"
#include "mvimpstenginesessioncntxtobserver.h"

// FORWARD DECLARATIONS           
class CVIMPSTEngineRequestMapper;
class MPresentityGroups;
class MXIMPContext;
class MXIMPIdentity;
   
// CLASS DECLARATION
/**
 * This Class implements the Contact Management Extention Feature
 * @lib vimpstengine.lib
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineContactMgmtExtention ) : public CBase,							   
							   public MVIMPSTEngineContactMgmtExtention,
							   public MVIMPSTEngineSessionCntxtObserver
						 	
    {
    //for test cases
    friend class T_VIMPSTEngineContactMgmtExtention;

    public:  // Two-phased constructors and destructor

        /**
         * Two-phased constructor.
         * @param aPresContext: presence context
         * @param aPresGroup - Presentity Group
         * @param aRequestMapper: Request Mapper for XIMP Events
         */
        
        static CVIMPSTEngineContactMgmtExtention* NewL(
        					MXIMPContext& aPresenceCtx,
        					MPresentityGroups& aPresGroup,
							CVIMPSTEngineRequestMapper& aRequestMapper);
        /**
         * Two-phased constructor.
         * @param aPresContext: presence context
         * @param aPresGroup - Presentity Group
         * @param aRequestMapper: Request Mapper for XIMP Events
         */										
        static CVIMPSTEngineContactMgmtExtention* NewLC(
        					MXIMPContext& aPresenceCtx,
        					MPresentityGroups& aPresGroup,
							CVIMPSTEngineRequestMapper& aRequestMapper);

        /**
         * C++ Destructor.
         */
        virtual ~CVIMPSTEngineContactMgmtExtention();
        
   	
    private:
		
        /**
         * default constructor
         * @param aPresContext: presence context
         * @param aPresGroup - Presentity Group
         * @param aRequestMapper: event observer to get the notifications of the events.
         */
        CVIMPSTEngineContactMgmtExtention(
        					MXIMPContext& aPresenceCtx,
        					MPresentityGroups& aPresGroup,
							CVIMPSTEngineRequestMapper& aRequestMapper);
        
        
	 	/**
	     * CreateIdentityLC 
	     * create the identity for given param
	     * @ aparam  aListId, to create
	     * @ return pointer to MPrFwIdentity
	     */
	 	MXIMPIdentity* CreateIdentityLC(const TDesC& aListId) ;
	 	
	 
    public: // See MVIMPSTEngineContactMgmtExtention   	
		
        /**
	     * See MVIMPSTEngineContactMgmtExtention	     
		 */
         TInt AddServerContactL(const TDesC& aContactList, 
         						const TDesC& aUserId,const TDesC& aNickname,
         						TBool aWaitToComplete = ETrue ) ;
        
        
         /**
	     * See MVIMPSTEngineContactMgmtExtention	     
		 */
         TInt DeleteServerContactL( const TDesC& aContactListId, const TDesC& aUserId ) ;
		
	
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
    	
    	// not own 
    	MXIMPContext& 		iPresenceCtx;
    	
    	// not deletable
    	MPresentityGroups& 		iPresGroup; 	
    	 
    	
       //doesnt own
        CVIMPSTEngineRequestMapper& iRequestMapper;         
       
    	//Whether the extention Feature Supported or not
    	TVIMPSTEnums::FeatureSupport 	iSupported; 
    	    	
    	//Result code from waited request
	    TInt    iReqResult;
	    
    };
    
#endif      //__CVIMPSTENGINECONTACTMGMTEXTENTION_H

// End of File
