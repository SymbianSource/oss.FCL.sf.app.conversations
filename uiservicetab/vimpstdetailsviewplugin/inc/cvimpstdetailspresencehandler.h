/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the presence handler
*
*/


#ifndef CVIMPSTDETAILSPRESENCEHANDLER_H
#define CVIMPSTDETAILSPRESENCEHANDLER_H

// INCLUDES
#include <e32base.h>

#include <presencecachereadhandler2.h> // handling the presence cache events 
#include <mpresencebuddyinfo2.h> // buddy info class
#include "tvimpstenums.h"
#include "mvimpstdetailspresencehandler.h"

// FORWARD DECLARATIONS
class MVIMPSTDetailsPresenceObserver;
class MPresenceBuddyInfo2;
class MPresenceCacheReader2;
class MXIMPClient;
class MXIMPContext;

// CLASS DECLARATION
/**
 * Class implementing the header part control of vimpstdetailsviewplugin
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v5.0
 */
 
class CVIMPSTDetailsPresenceHandler :public CBase, 
									 public MVIMPSTDetailsPresenceHandler,
									 public MPresenceCacheReadHandler2
    {
public:
    // Construction & destruction
    /**
     * Two-phased constructor
     * @param aServiceName, service data.
     * @param aObserver, presence observer 
     * @param aPresenceSupported, presence supported or not 
     * @return instance of this class
     * @since S60 v5.0
     */
    static CVIMPSTDetailsPresenceHandler* NewL( const TDesC& aServiceName, 
    											MVIMPSTDetailsPresenceObserver& aObserver,
    											TBool aPresenceSupported  );
    
    /**
     * Destructor.
     * @since S60 v5.0
     */
    ~CVIMPSTDetailsPresenceHandler();
 

public: //  from MVIMPSTDetailsPresenceHandler

     /**
     * from MVIMPSTDetailsPresenceHandler
     * fetch presence from cache
     * @since S60 v5.0
     */
    void  SubscribePresenceL(const TDesC& aUserId );
   
    /**
     * from MVIMPSTDetailsPresenceHandler
     * return the presence status
     * @since S60 v5.0
     */
    TVIMPSTEnums::TOnlineStatus GetPresenceStatus();
    
   
   	/**
     * from MVIMPSTDetailsPresenceHandler
     * @param aIndex , index of the control
     * @return  Label for control
     * @since S60 v5.0
     */
	const TDesC& GetHeaderLabelDataL( TInt aIndex );
	
	 /**
	 * from MVIMPSTDetailsPresenceHandler
     * @return the presence status message
     * @since S60 v5.0
     */
    const TDesC& GetStatusMessageL();

    /**
     * function to get supported or not 
     * @since S60 v5.0
     * @return ETrue if supported
     */
    TBool IsSupported() const ;
  

public: // prescachereadhandler
	
     /**
        * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
        *
        * @param aErrorCode 
        * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
        *         Ownership of elements to callee. Can be empty if not found.
        */   
       void HandlePresenceReadL(TInt aErrorCode,
            RPointerArray<MPresenceBuddyInfo2>& aPresenceBuddyInfoList) ;
                           
       /**
        * Handler for Buddy presence change notifications.
        *
        * @param aErrorCode 
        * @param aPresenceBuddyInfo presence info for buddy. 
        *         Ownership to callee. Can be NULL if aErrorCode exist.
        */    
      void HandlePresenceNotificationL(TInt aErrorCode,
           MPresenceBuddyInfo2* aPresenceBuddyInfo);
     
    
private:
	
    /**
     * Constructor for performing 1st stage construction
     * @param aObserver, presence observer 
     * @param aPresenceSupported, check for PresenceSupported or not
     * @since S60 v5.0
     */
    CVIMPSTDetailsPresenceHandler(MVIMPSTDetailsPresenceObserver& aObserver,
                                   TBool aPresenceSupported );

    /**
     * Constructor for performing 2nd stage construction
     * @param aServiceName, service data.
     * @since S60 v5.0
     */
    void ConstructL( const TDesC& aServiceName );
    
    /**
    
    /**
     * Converts the  presence cache status to client supported presence status.
     * 
     * @since 5.0
     * @param aAvailabilityEnum presence cache status value.
     * @param aAvabilityText avability text,to support multiple presence states.
     * @return TVIMPSTEnums::TOnlineStatus client  presence status.
     */
    void ConvertPresenceCacheEnums(MPresenceBuddyInfo2::TAvailabilityValues aAvailabilityEnum, TPtrC aAvabilityText);
	
	/**
     * Helper function for the setting the status text and status
     * @param aStatusMessage, status message .
	 * @since S60 v5.0
     */
	void ProcessStatusMesssageL(const TDesC& aStatusMessage  );
	
     			
	/**
     * function to get the label text
     * @since S60 v5.0
     * @return status text
     */
	const TDesC& GetStatusTextL();
	
	  /**
       * Converts the  presence cache extension keys 
       * user for block and pending states  to client supported presence status.
       * 
       * @since 5.0
       * @param aPresenceBuddyInfo presence buddy info class.
       */
     void GetKeyFieldsAndValuesL(MPresenceBuddyInfo2& aPresenceBuddyInfo);



private:  // data member
          
     // not owns: reference to presence observer 
    MVIMPSTDetailsPresenceObserver& iPresenceObserver;
  	// Owns : ximp client
	MXIMPClient* iXIMClient ;      
    //Own : ximp context 
    MXIMPContext* iPresenceContext;
   	//owns : pointer to presence read interface
	MPresenceCacheReader2* iPresenceCacheReader;
	//  owns, service name 
	HBufC* iServiceName;
	//  owns, user id
	HBufC* iUserId;
	
	//  owns, status message from server if any
	HBufC* iStatusMessage;
	
	//  owns, status text , online/offline etc
	HBufC* iStatusText;

	//  owns, status
	TVIMPSTEnums::TOnlineStatus iStatus;
	//  owns, presence supported or not  
	TBool iPresenceSupported;
    
    };

#endif // CVIMPSTDETAILSPRESENCEHANDLER_H
