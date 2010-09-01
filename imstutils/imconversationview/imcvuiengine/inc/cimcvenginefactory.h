/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  service engine factory class 
*
*/


#ifndef CIMCVENGINEFACTORY_H
#define CIMCVENGINEFACTORY_H

//  INCLUDES
#include <e32base.h>
#include "mimcvenginefactory.h"
#include "mimcvengineopenchats.h"
#include <mspnotifychangeobserver.h>
#include "mimcvenginemessagehandlerobserver.h"
#include "mimcvenginechatlistobserver.h"

// FORWARD DECLARATIONS
class CIMCVEngine;
class MIMCVEngineConnectionHandler;
class MIMCVEngineOpenChatsObserver;
class CSPSettings;
class MIMCVEngineMessageCreator;
class CBSFactory;
class CSPNotifyChange;
class MIMCVEngineChatListObserver;
class CIMCVEngineStorageListener;

// CLASS DECLARATION
/**
 *  Factory to create engine.
 *  This class implements the factory for creating engine
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */

class CIMCVEngineFactory : public CBase,
						   public MIMCVEngineFactory,
						   public MIMCVEngineOpenChatsObserver,
						   public MSPNotifyChangeObserver,
						   public MIMCVEngineMessageHandlerObserver,
						   public MIMCVEngineChatListObserver
	{
	public:  // Two-phased constructors and destructor

	   /**
		* @returns a single instance of MVIMPSTEngineFactory
		*/
		IMPORT_C static MIMCVEngineFactory* InstanceL();
		
	   /**
		* Release 
		* dont call delete   
		*/
		IMPORT_C static void Release();
		
		/**
		 * c++ default destructor
		 * dont call delete 
		 */
		virtual ~CIMCVEngineFactory();
		
	private:	
	
	   	/**
		 * NewLC two phase constructor.
		 * dont call delete call close 
		 */
		static CIMCVEngineFactory* NewLC();

		/**
		 * Second phase construction
		 * ConstructL
		 */
		void ConstructL();

		/**
		 * C++ constructor.
		 */
		CIMCVEngineFactory();
		
		
		void CreateServiceEngineL( TInt aServiceId );
		
		
		TBool CheckValidIMServiceL(CSPSettings& aSettings , TInt aServiceId) ;
		
		TUint32 ContactViewUidL(CSPSettings& aSettings, TUint32 aServiceId );
		
		void ReadAndCreateAllValidServiceL();
		
	
	public: //From MVIMPSTEngineFactory
			
	    /**
	     * return the engine instance for aServiceId.
	     */
	    CIMCVEngine& GetServiceEngineL( TInt aServiceId )  ;
	    
	    /**
	     * find and release the engine instance for aServiceId.
	     */
	    void ReleaseServiceEngine( TInt aServiceId );
	    
	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    TPtrC GetDisplayNameL(TInt aIndex) const;


	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    const TDesC& GetTargetIdL(TInt aIndex) const;


	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    TInt Count() const;

	    /**
	     * @see MVIMPSTEngineFactory
	     */     
	    void SetActiveItemL( const TInt aIndex );

	    /**
	     * @see MVIMPSTEngineFactory
	     */     
	    const TDesC& GetActiveItemL() const;

	    /**
	     * @see MVIMPSTEngineFactory
	     */     
	    TBool IsMsgPendingL( const TInt aIndex ) const;

	    /**
	     * @see MVIMPSTEngineFactory
	     */     
	    TInt FindIndexL(const TDesC& aItem);
	    
	    /**
	     * @see MVIMPSTEngineFactory
	     */  
	    void ResetL();
	    
	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    void RegisterOpenChatObserver(MIMCVEngineOpenChatsObserver* aChatObserver);

	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    void UnRegisterOpenChatObserver();
	 
	    
	    TInt GetActiveServiceId();
		/*
		* return all service engines array
		*/
	    RPointerArray<CIMCVEngine>&  GetServiceEngines();
	   
	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    void RegisterServiceObserver(MIMCVEngineServiceChangeObserver* aServiceObserver)  ;
	    /**
	     * @see MVIMPSTEngineFactory
	     */
	    void UnRegisterServiceObserver() ;
	    
	 
	private :
	    
	    /**
	     * @see handle the presence change notification
	     * to handle the connection lost
	     */
	    void HandlePresenceChangedL(TInt aServiceId, const TDesC& aBuddyId )  ;

	    /**
	     * Handle message send operation completion.
	     * @param aResult result  to handle.
	     */
	    void HandleOperationCompletdL(TInt aserviceId, MIMCVEngineMessage::TMessagerType aType , TInt aResult)  ;

	    /**
	     * @see HandleConnectionTerminatedL
	     * to handle the connection lost
	     * @param aReason, reason report
	     */
	    void HandleConnectionEventL(TInt aServiceId, TIMCVConnectionState aState, TInt aReason )    ;
	    
	    /**
	     * Handle events.
	     * @param aEvent Event to handle.
	     * @param aContainerInfo, Container
	     */
	    void HandleChatListEvent(TInt aServiceId, TChatListEventType aEvent, 
	            MIMCVEngineMessageReadInterface* aContainerInfo
	             ) ;


	private: // From MSPNotifyChangeObserver
	    
	    /**
	     * From MSPNotifyChangeObserver 
	     * Callback when service has changed
	     *
	     * @since S60 5.0
	     * @param aServiceId changed service
	     * @return None
	     */
	    void HandleNotifyChange( TServiceId aServiceId );

	    /**
	     * From MSPNotifyChangeObserver 
	     * Callback when service error occures
	     *
	     * @since S60 5.0
	     * @param aError error code
	     * @return None
	     */
	    void HandleError( TInt aError );
	    
	    /*
	     * end the cv process
	     */
	    void ShutDownConversationviewL();
	        
	private:
	    /** 
	     * @see MIMCVEngineOpenChatsObserver
	     */
	    void HandleChangeL(TInt aServiceId , TInt aIndex, TOpenChatsEventType aChangeType) ;

	    /**
	     * @see MIMCVEngineOpenChatsObserver
	     */
	    void SetStatusMsgL(TInt aServiceId, TInt aIndex,const TDesC & aMsg);
	 
	 private :
	     /*
	      * helper method to for service table change
	      */
	    void DoHandleServiceChangeL( TInt aServiceId );
	    
 	private :	
	    // Owns : message creater 
	    MIMCVEngineMessageCreator* iMessageCreator;
	    //owns array of services
		RPointerArray<CIMCVEngine> iServiceEngines;	
		// not owns
		MIMCVEngineOpenChatsObserver* iChatObserver;
		// active service Id
		TInt iActiveServiceId;
		// active chat index
		TInt iActiveChatIndex;
		//owns brandingfactory for service specific icons
		CBSFactory* iBrandingFactory; 
		// Own: service observer
		CSPNotifyChange* iSpNotifyChange;  
		// not own 
		MIMCVEngineServiceChangeObserver* iServiceChangeObserver;
	    };

#endif      //CIMCVENGINEFACTORY_H

//  End of File


