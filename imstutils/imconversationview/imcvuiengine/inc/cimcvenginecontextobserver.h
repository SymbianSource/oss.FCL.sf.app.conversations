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
* Description:  single entry context observer implementation class
*
*/


#ifndef CIMCVENGINECONTEXTOBSERVER_H_
#define CIMCVENGINECONTEXTOBSERVER_H_


// INCLUDES
#include <e32base.h>
#include <ximpcontextobserver.h>
#include <ximpcontext.h>
#include <ximpcontextstate.h>
#include "mimcvenginenewmessagehandler.h"

// CLASS DESCRIPTION

class TIMCVStateEnums
	{
public:
	enum TIMCVOpersationState
			{
			EIMCVUnknown = -1,
			EIMCVLogging,
			EIMCVLoggingOut,
			EIMCVSendingMsg	
			};	
	};

/**
 * ximp context observer for usage scenario tests.
 */
class CIMCVEngineContextObserver : public CBase,
								  public MXIMPContextObserver,
								  public MIMCVEngineServiceSessionObserver
								  
								  
								  
    {
    
    enum TXIMPReqType
	    {
	    EReqUnknown = -1,
	    ELoginReq,
	    ELogoutReq
	    };
    
public:
		
	
   /**
	* Two-phased constructor.
	*/
	static CIMCVEngineContextObserver* NewL();
	/**
	* Two-phased constructor.
	*/
    static CIMCVEngineContextObserver* NewLC();
    
    /**
	* CIMCVEngineContextObserver
	*/
    CIMCVEngineContextObserver();
    
    /**
	* ~CIMCVEngineContextObserver
	*/
    ~CIMCVEngineContextObserver();
    
    /**
	* GetConnectedSessionL 
	* bind to the same session to the ximpfw
	*@param aContext, context to ximpfw
	*@param aProtocolUid, uid of protocol used 
	*/
      
    void GetConnectedSessionL(MXIMPContext& aContext, TInt aServiceId, TUid aProtocolUid );
     /**
	* ReleaseConnectionL 
	* releasse the  session to the ximpfw
	*@param aContext, context to ximpfw
	*/
    void ReleaseConnectionL(MXIMPContext& aContext);
    
    /**
	* IsLoggedIn 
	* return ETrue if binded
	*/
    TBool IsLoggedIn() const;
    
	/**
	* SetLoggedIn 
	* ETrue if binded
	*/
    void SetLoggedIn(TBool aState) ;
  
public : // MIMCVEngineServiceSessionObserver 

	/**
	* RegisterObserver 
	* register to listen the new message 
	*/
	void RegisterObserver(MIMCVEngineServiceSessionHandler* aObserver)  ;
   	/**
	* UnRegisterObserver 
	* unregister the message observer
	*/
	void UnRegisterObserver(MIMCVEngineServiceSessionHandler* aObserver)  ;
   
      	 
public:

    /**
     * HandleximpContextEvent method from context obsrver  
     * ximp framework
     * @param aContext
     * @param aEvent, event  
     */
    
    void HandlePresenceContextEvent( const MXIMPContext& aContext,
                                     const MXIMPBase& aEvent );
                                     
       
private: 
	
  
   	/**
     *constructL 
     */
    void ConstructL();

private:  //Data

    
    //OWN: Result code from waited request
    // owned 
    TBool iLoggedIn;
    // login state    		
	
	//owned , to handle the recieving ,messages
    MIMCVEngineServiceSessionHandler* iSessionObserver;
	
	// owned, wait for login\logout to complete
	CActiveSchedulerWait iWait;
	
	// ongoing reuest id
	TIMCVStateEnums::TIMCVOpersationState iOngoingOperation;
	
	TXIMPRequestId iCurrentXIMPReq;
	
	TXIMPReqType iCurrentReqType;
	
    };



#endif //CConversationContextObserver_H_


