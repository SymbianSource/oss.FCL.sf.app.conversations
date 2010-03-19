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
* Description: ximp context observer
*
*/


#ifndef CVIMPSTENGINESESSIONCNTXTOBSERVER_H_
#define CVIMPSTENGINESESSIONCNTXTOBSERVER_H_


// INCLUDES
#include <e32base.h>

#include "tvimpstenums.h"

#include <ximpcontextobserver.h>
#include <searchelement.h>

class MVIMPSTServiceListObserver;
class CVIMPSTEngineRequestMapper;
class MXIMPContext;
class MVIMPSTEngineSearchExtentionEventObserver;
class MPresenceGrantRequestListEvent;
class MSearchKeysEvent;
class MPresenceFeatures;
class MXIMPClient;
class MVIMPSTEngineSessionCntxtObserver;
class MPresenceAuthorization;
class MPresenceWatching;
class MPresentityGroups;
class MPresencePublishing;
class MPresenceObjectFactory;

// CLASS DESCRIPTION

/**
 * ximp context observer
 *  @lib vimpstengine.lib
 *  @since S60 5.0 
 */
NONSHARABLE_CLASS( CVIMPSTEngineSessionCntxtObserver ) : public CBase,
								  public MXIMPContextObserver
								  
								  
    {
    //for testcases.
    friend class T_VimpstEngineSessionCntxtObserver;
     friend class T_VimpstEnginePresenceSubService;

	public:
		
	    /**
	     * Two-phased constructor.
	     * @param aSettingsId: settings id.
         * @param aAdapterUid: Uid of the ximpfw adaptation.
		 * @return CVIMPSTEngineSessionCntxtObserver new instance of this class
	 	 */
		static CVIMPSTEngineSessionCntxtObserver* 
							NewL(TUint32 aServiceId);
        /**
	     * Two-phased constructor.
	     * @param aSettingsId: settings id.
         * @param aAdapterUid: Uid of the ximpfw adaptation.
		 * @return CVIMPSTEngineSessionCntxtObserver new instance of this class
	 	 */							
	    static CVIMPSTEngineSessionCntxtObserver* 
	    				NewLC(TUint32 aServiceId);
	    
	    
	   /**
	    * C++ default destructor.
		* ~CVIMPSTEngineSessionCntxtObserver
		*/
	    ~CVIMPSTEngineSessionCntxtObserver();
	    
	   /**
		* GetCompletedReqResult
		* @return TInt the result code 
		*/
	    TInt GetCompletedReqResult() const;
	    
	   /**
		* GetRequestMapper
		* @return CVIMPSTEngineRequestMapper pointer to RequestMapper 
		*/
	    CVIMPSTEngineRequestMapper* GetRequestMapper() const;   
       
	    /**
         * Presence bind
         * 
         * @since S60 5.0
         * @param aProtocolUid - uid of the ximpfw adaptation.
         * @return TInt, error if any.
         */         
         TInt ServerBindL(TUid aProtocolUid);

        /**
         * Presence unbind
         *
         * @since S60 5.0
         * @param aDoUnsubscribe, do unsubscribe before unbind.
         * Unsubscribe can not be done if unbind is triggered by
         * lost network.
         * @return TInt, error if any.
         */         
        TInt ServerUnBindL( TBool aDoUnsubscribe );  
        
        /**
         * Returns pointer to XIMP presence authorization interface.
         * This method must be used to get access.
         * If feature is not supported by current
         * presence adapter, leaves with KErrNotSupported.
         *
         * @since S60 3.2
         * @return MPresenceAuthorization reference
         */        
        MPresenceAuthorization& XimpAuthorizationL();

        /**
         * Returns pointer to XIMP presence watching interface.
         * This method must be used to get access.
         * If feature is not supported by current
         * presence adapter, leaves with KErrNotSupported.
         *
         * @since S60 3.2
         * @return MPresenceWatching reference
         */        
        MPresenceWatching& XimpPresenceWatchingL();

        /**
         * Returns pointer to XIMP presence presentity groups interface.
         * This method must be used to get access.
         * If feature is not supported by current
         * presence adapter, leaves with KErrNotSupported.
         *
         * @since S60 3.2
         * @return MPresentityGroups reference
         */        
        MPresentityGroups& XimpPresentityGroupsL();

        /**
         * Returns pointer to XIMP presence publishing interface.
         * This method must be used to get access.
         * If feature is not supported by current
         * presence adapter, leaves with KErrNotSupported.
         *
         * @since S60 3.2
         * @return MPresentityGroups reference
         */        
        MPresencePublishing& XimpPresencePublishingL();
        
        /**
         * Gets reference to object factory interface.
         *
         * Returned object factory interface is used
         * to instantiate presence objects.
         *
         * @return Object factory interface.
         *         No ownership is returned caller.
         */
        MPresenceObjectFactory& PresenceObjectFactoryL() const ;
        
        MPresenceFeatures& PresenceFeaturesL() const;

        /**
         * Gets reference to Presencecontext.
         *
         * Returned Presencecontext interface is used
         * to instantiate object factory.
         *
         * @since 5.0
         * @return Object factory interface.
         *  No ownership is returned caller.
         */
        MXIMPContext& XimpPresenceContextL();        
        
         /**
          * Register observer from getting notified of ximp events.
          *
          * @since 5.0
          * @param aObserver observer to be called when ximp events are received.
          * Unregister is done by session context when the presence context gets
		  *	unbinded. The Requester must register each time when XIMP Context gets Binded.
		  * Otherwise he will not get the event
          */
         void RegisterObserver(MVIMPSTEngineSessionCntxtObserver* aObserver);
         
         /**
          * get the supproted features form the ximp adaptation
          *
          * @since 5.0
          * @return MDesC8Array array of supported features.
          */
         TInt GetSupportedFeatures();
         
         
         /**
          * get the supproted features form the ximp adaptation
          *
          * @since 5.0
          * @return MDesC8Array array of supported features.
          */
         TVIMPSTEnums::TVIMPSTBindStatus ContextBindStatus();
      
	private:

		/**
         * Unregister observer from getting notified of ximp events.
         *
         * @since 5.0
         * @param aObserver observer to be called when ximp events are received.
         */
         void UnRegisterObserver(MVIMPSTEngineSessionCntxtObserver* aObserver);

	    /**
	     * HandleximpContextEvent method from context obsrver  
	     * ximp framework
	     * @param aContext
	     * @param aEvent, event  
	     */
	    
	    void HandlePresenceContextEvent( const MXIMPContext& aContext,
	                                     const MXIMPBase& aEvent );                                     
	    
	    /**
         * Handles presence callbacks
         *
         * @since S60 3.2
         * @param aContext, context
         * @param aEvent, event
         * @return void
         */ 
        void DoHandlePresenceContextEventL(
            const MXIMPContext& aContext,
            const MXIMPBase& aEvent );   
	               
       /**
        * Resolves and allocates presense uri from ximp operation.
        *
        * @since S60 5.0
        * @param aEvent, used to get the ximpidentity.
        * @return HBufC, presence uri from operation. Ownsership passed.
        */           
     	HBufC* UriFromXimpOperationLC(const MXIMPBase& aEvent );
	private: 	
		
	   	/**
	     *second phase constructor
	     */
	    void ConstructL();
	    
	   /**
		* CVIMPSTEngineSessionCntxtObserver
		* @param aSettingsId: settings id.
		* @param aAdapterUid: Uid of the ximpfw adaptation.
		*/
	    CVIMPSTEngineSessionCntxtObserver(TUint32 aServiceId);
	    
	    /**
	     * Identifies the Features supported by the Adaptation
	     * and stores the information in iFeatureSupported 
	     */
	    void IdentifySupportedFeaturesL();	    
	    

	private:  //Data
 	    // not own
        TUint32 iServiceId;
	   
  	    //Result code from waited request
	    TInt    iReqResult;  
	    
	    // Current presence bind status
	    TVIMPSTEnums::TVIMPSTBindStatus iBindStatus;
	    
	    //OWN: request mapper to mapp each request
	    CVIMPSTEngineRequestMapper*    iRequestMapper;
	    
	    // own , client for presence framework
        MXIMPClient*                      iClient;
        
        // own ,presence context to prsence framework
        MXIMPContext*             iPresenceCtx;
	    
	    // owned  pointer to presence plugin
	    MPresenceFeatures* iFeatures;
	    
	    //Stores the XIMPFw EventTypes Subscribed for      
	    RArray< TInt32 > iAcceptedEventTypes;
	    
	    // Own, array of observers.
	    RPointerArray<MVIMPSTEngineSessionCntxtObserver> iObserverArray;
	    
	    //Stores the XIMP Features Supported
	    TInt iFeatureSupported;

	    
    };



#endif //CVIMPSTENGINESESSIONCNTXTOBSERVER_H_


