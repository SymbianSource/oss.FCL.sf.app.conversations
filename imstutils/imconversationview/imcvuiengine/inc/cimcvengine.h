/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  owner class of this dll
*
*/

#ifndef CIMCVENGINE_H
#define CIMCVENGINE_H

//  INCLUDES
#include  <e32base.h>

#include <ximpclient.h>
#include <ximpcontext.h>
#include <ximpcontextobserver.h>
#include <ximpcontextstate.h>
#include "mimcvenginecchhandler.h"
#include "mimcvengineopenchats.h"
#include "mimcvenginecchobserver.h"

// FORWARD DECLARATIONS

class   MIMCVEngineChatInterface;
class   CIMCVEngineMessageHandler;
class   MIMCVEngineMessageCreator;
class 	CIMCVEngineContextObserver;
class 	MIMCVEngineMessageHandler;
class 	MIMCVEngineConnectionHandler;
class   CIMCVEngineCchHandler;
class   MBSAccess;
class   CBSFactory;
class   CGulIcon;
class   CFbsBitmap;
class  CIMCVEngineFetchSmiley;
/**
 *  CIMCVEngine provides one of the interfaces to the engine.
 *  This interface is used by UI to access network services i.e.
 *  sending messages and joining to groups e.g. This class is
 *  also used to get different interfaces to the services
 *  offered by the imcvengine.dll.
 *
 *  @lib imcvengine.lib
 *  @since 5.0
 */
class CIMCVEngine : public CActive,
                    public MIMCVEngineCCHObserver
    {
    
    //from MIMCVEngineCCHObserver
    void ServiceStatusChanged( TInt aServiceId,     								   
	                          TServiceState aServiceStatus );	                                 
    
    
    private:
	/**
         * Issues Request
         */
    	void IssueRequest();
    	 		
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.         
         */
        static CIMCVEngine* NewL(TInt aServiceId, 
                MIMCVEngineMessageCreator& aMessageCreater, 
                CBSFactory& aBrandingFactory);

        /**
         * Destructor.
         */
         virtual ~CIMCVEngine();
 
    
        /**
         * Get interface for chat interface.
         */         
        IMPORT_C MIMCVEngineChatInterface& ChatInterface() const;
        
        
        /**
         * Get interface to message handler 
         */
		IMPORT_C MIMCVEngineMessageHandler& MessageHandler() const ;
   
	    /**
         * IsLoggedIn 
         * Return ETrue if loggedin else EFalse
         */
        IMPORT_C TBool IsLoggedIn() const;
        
        /**
         * GetLoggedInUserId 
         * Return logged in user id 
         */
        IMPORT_C const TDesC& GetLoggedInUserId()  ;
        
        /**
         * ReleaseConnectionL 
         * Release the connection from server
         */
       	IMPORT_C void ReleaseConnectionL() ;
       
          
        /**
         * Checks engine if its ready for shutdown.
         * @return ETrue if ready.
         */
        IMPORT_C TBool ReadyForShutdown();

    	
		/**
		* DeleteContextL 
		*/
 		void DeleteContextL() ;

		/**
		* CreateContextL 
		*/
 		void CreateContextL() ;
	
 		 /**
          * GetLoggedInUserId 
          * Return logged in user id 
          */
        IMPORT_C const TDesC8& GetBrandId()  ;
        
        /**
         * Get Service Specific Array of Smiley Strings 
         * Return array
         * ownership not transferred
         */
        IMPORT_C RPointerArray<HBufC> GetSmileStringArray();
        
        /**
         * Get Service Specific Icon Array of Smiley's 
         * Return array
         * ownership not transferred
         */        
        IMPORT_C RPointerArray<CGulIcon> GetSmileyIconArray();
        
        
        IMPORT_C CGulIcon* GetPresenceIconL(const TDesC8& aIconid);
 		        
       /**
        * GetLoggedInUserId 
        * Return logged in user id 
        */
        IMPORT_C TInt GetLanguageId()  ;
        
        
       /**
        * CCHHandler 
        * Returns handler to CCH 
        */
        IMPORT_C MIMCVEngineCchHandler& CCHHandler() ;
        
        
        /**
        * ServiceName 
        * Returns Name of the Service
        */                
		IMPORT_C const TDesC& ServiceName() ;
		
		
		/**
        * Returns the M-Interface class for list of Open Conversations 
        * Returns MIMCVEngineOpenChats
        */
		IMPORT_C MIMCVEngineOpenChats& OpenChats() ;
		
		
		/**
        * ServiceId 
        * Returns Service Id of the Engine
        */
		IMPORT_C TInt GetServiceId() ;
		
		
		/**
		* Resolves the CCH service state to and does a bindl to
		* ximpfw if required.
		*/
		IMPORT_C void ResolveServiceStateL();		
		
		/**
		* Login to the Service
		* 
		*/
		IMPORT_C TInt LoginL();
		
		/**
         * CreateConnectionL
        */
		void CreateConnectionL() ;
		
		/*
		 * close all open chats
		 */
		void CloseAllOpenChatsL();
		
	    /**
	         * Check whether the service is VOIP Enabled Service
	         *
	         * @since S60 5.0
	         * @return ETrue/EFalse
	         */
		IMPORT_C TBool IsVoipServiceL();
		
	private:

        /**
         * By default Symbian OS constructor is private.
         */
        void ConstructL(MIMCVEngineMessageCreator& aMessageCreater );

        /**
         * C++ default constructor.
         * @see CIMCVEngine::NewL.
         */
        CIMCVEngine(TInt aServiceId,CBSFactory& aBrandingFactory);
		
		
		/**
         * Returns property brand language.
         *
         * @since S60 5.0
         * @param aServiceId, service id
         * @return TLanguage, brand language
         */        
        TLanguage PropertyBrandLanguageL( 
            TUint32 aServiceId );


		/**
         * Gets brand id
         *
         * @since S60 5.0
         * @param aServiceId service id
         * @param aBrandId brand id
         * @return None
         */
        void GetBrandIdL( TUint32 aServiceId, TDes8& aBrandId );

		/**
         * Gets the Settings ID
         *
         * @since S60 5.0
         * @param aServiceId, service id         
         * @return settings id
         */						
		TInt PropertySettingsIdL( TUint32 aServiceId );	
		
		
		/**
         * Gets the User Id ID
         *
         * @since S60 5.0
         * @param aUserId holds the user id    
         * @return None
         */
		void GetUserIdL(TPtr& aUserId ) ;
		
		/**
         * Gets the ServiceName
         *
         * @since S60 5.0
         * @param aServiceId, service id 
         * @param aServiceName holds the ServiceName    
         * @return None
         */
		void GetServiceNameL( TUint32 aServiceId, TDes& aServiceName );
		/**
		 *consturct's the brandaccess specific to service engine
		 */
		void ConstructBrandL();
		
		/**
		 *utility function to parse the smiley string received from branding server
		 * for example if smiley string is  ":-) [8]"
		 * it forms an array of each smiley string 
		 */
		void ParseAllSmileyStringL(TInt aTotalCount, const TDesC& aAllSmileyString);
	
	private: // From CActive
        void RunL();
        void DoCancel();
        TInt RunError(
                TInt aError );
                	
    private:    // Data
    
    	/// Process states
        enum TState
            {
            ELogin,                        
            EOperationComplete,
            EOperationInComplete
            };
        
                
		// service id to use
		TInt iServiceId;
		
			// own user id
		HBufC* iOwnUserId;
		
		//  Owns,Settings id of the service..
        HBufC8* iBrandId;

        // Language id
        TInt iLanguageId; 
		
	    // Owns , reference to message handler
        CIMCVEngineMessageHandler*      iMessageHandler;
       
      
        //owned, Chat interface
        MIMCVEngineChatInterface* iChatInterface;
        
        // own , client for ximp framework
		MXIMPClient*               		  iClient;
		// own ,presence context to ximp framework
		MXIMPContext*     		  iPresenceContext;
		
		//owned, observre for ximp call back
		CIMCVEngineContextObserver* iContextEventObserver;
		
		//Stores the XIMPFw EventTypes Subscribed for		
		RArray< TInt32 > iAcceptedEventTypes;
	
		//owns the handler for CCH
		CIMCVEngineCchHandler* iCCHHandler;
		
		//Owns the ServiceName
		HBufC* iServiceName;
		
		/// Own: State of the command
        TState iState;  
        
        // own active sheduler 
        CActiveSchedulerWait    iWait;      
        //doesn't own
        CBSFactory& iBrandingFactory;
                  
        //owns: branding access specific to service
        MBSAccess* iBrandingAccess;
        // not owned array of smiley strings
        RPointerArray<HBufC> iSmileyArray; 
        // not owned ,array of smiley icons
        RPointerArray<CGulIcon> iSmileyDlgIconArray;
        // ETrue if array owned
        TBool iSmileyArrayOwned;
        // ETrue if array owned
        TBool iSmileyDlgIconArrayOwned;
    };

#endif      // CIMCVENGINE_H

// End of File
