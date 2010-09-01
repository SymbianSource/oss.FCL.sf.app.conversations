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
* Description:  vimpst settings store definitions
*
*/
#ifndef __CVIMPSTSETTINGSSTORE_H
#define __CVIMPSTSETTINGSSTORE_H

#include <e32base.h>

/** types of settings provided in meco settings */
enum TSettingItemName
    {
    ETOUDlgOff = 0x00000000,                      // key value to enable or disable TOU Dialog to be shown 
                                                    //0-> show TOU(default),1->Disable TOU..
    EURLRegistrationOff = 0x00000001,             // key value to enable or disable URL registration query
                                                    //0->URL registration on,1-> URL registration off.
    EServiceSuccessfullLogin = 0x00000002,
    EServiceToneFileName = 0x00000003,              // key value to get the IM tone File name.complete path of the file name should be provided. example "C:\sounds\digital\imtone.wav"                        
    EServiceURLLink = 0x00000004,                    // key value to get the URL link for IM based on service     
    EServiceAvatar = 0x00000005,                  // key value to get the Own Avatar for IM based on service    
    EServiceAvatarMimeType = 0x00000006,          // key value to get the Own Avatar s 'mimetype' for IM based on service   
    EServiceOwnAvatarIconIndex = 0x00000007 ,         // key value to get the Own Icon index for avatar s  
    // key value to check whether the  "Cancel Login" is supported or not   
    // if setting "value = 1" then cancel login not supported else supported.
    // if cancel is supported , no need to write anything to cenrep.
    // By default its taken as supported.
    // if not supported then write explicitly this setting as '1'
    EServiceCancelLoginNotSupported = 0x00000008,
    EServiceOwnStatusMessagesOnline = 0x00000009,   // storing 4 previous messages for online
    EServiceOwnStatusMessagesBusy = 0x0000000A,   // storing 4 previous messages for busy
    EServiceOwnStatusMessagesAway = 0x0000000B,   // storing 4 previous messages for away
    // key value to check whether the  "ChangeConnection" is supported or not   
    // if setting "value = 1" then ChangeConnection not supported else supported.
    // if ChangeConnection is supported , no need to write anything to cenrep.
    // By default its taken as supported.
    // if not supported then write explicitly this setting as '1'
    EServiceChangeConnectionNotSupported = 0x0000000C,
    // Presence Info used to republish own presence status in case of network lost and recovered
    EServicePresenceAvailablilityValue = 0x0000000D,              //Presence Availablility Info
    EServicePresenceSessionIdentifier = 0x0000000E,               //Presence Session Identifier
    EServiceCustomStatusMessage = 0x0000000F,											//Presence Custom Status Message
    EServiceLastUserName = 0x00000010															// set username in cenrep.
    };                                                      



//CLASS  DECLARATION
/**  
 *  M-Class for meco settingstore API's 
 *  @lib vimpstsettingsstore.lib
 *  @since S60 5.0
 */
class MVIMPSTSettingsStore
    {        
    public:     //Interface
        
        /**
         * virtual Destructor.
         */
        virtual ~MVIMPSTSettingsStore()
            {
            }
        
        /**
        * Peforms Get/Set operation in the cenrep
        * @param aServiceId, id if the service for which settings are get/set      
        * @param aSettingItemName, setting item name 
        * @param aValue, variable to set/get the value
        * @return TInt, used as error passing value. 
        */      
        virtual TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemName, TInt& aValue ) const = 0;
        virtual TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TInt aValue ) = 0;
        
        /**
         * Peforms Get/Set operation in the cenrep
         * @param aServiceId, id if the service for which settings are get/set      
         * @param aSettingItemName, setting item name 
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
        virtual TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, RBuf& aBuffer ) const = 0;
        virtual TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC& aBuffer ) = 0;
        
        /**
         * Peforms Get/Set operation in the cenrep
         * @param aServiceId, id if the service for which settings are get/set      
         * @param aSettingItemName, setting item name 
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
        virtual TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, RBuf8& aBuffer ) const = 0;
        virtual TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) = 0;
        
    };

/**
 *  vimpst settings API class
 *
 *  Main class of managing vimpst settings
 *
 *  @code example code of how to get/set a value to the setting item in cenrep 
 *  MVIMPSTSettingsStore* settings = CVIMPSTUiSettingsStore::NewLC();
 *  TInt value = 1;
 *  TInt err = settings->SetL(serviceId, ETOUDlgOff, value );
 *  similarly for getting the value
 *  settings->GetL(serviceId, ETOUDlgOff, value);
 *  delete settings or use clean up stack to push and pop 
 *  @lib vimpstsettingsstore.lib
 *  @since S60 v5.0
 */
class CVIMPSTSettingsCenRep;
class CVIMPSTSettings;

class CVIMPSTSettingsStore   :           public CBase,                                            
                                         public MVIMPSTSettingsStore
    {
    
    public: // Constructors and destructor
        /**
         * Constructors
		* Do not use MVIMPSTSettingStore object as parameter while calling PopAndDestroy
         */
        IMPORT_C static MVIMPSTSettingsStore* NewLC();
        /**
         * Constructors , 
		* No not push MVIMPSSettingStore instance on cleanupstack,maily NewL can be used for member variable
         */
        IMPORT_C static MVIMPSTSettingsStore* NewL();

        /**
         * Destructors.
         */
        virtual ~CVIMPSTSettingsStore();
        
    private:        
        
        /**
         * Performs the 2nd phase of construction.
         */
         void ConstructL();
        
    private:        
        /**
        * Peforms Get/Set operation in the cenrep
        * @param aServiceId, id if the service for which settings are get/set      
        * @param aSettingItemName, setting item name 
        * @param aValue, variable to set/get the value
        * @return TInt, used as error passing value. 
        */      
        TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemName, TInt& aValue ) const;
        TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TInt aValue );
        
        /**
         * Peforms Get/Set operation in the cenrep
         * @param aServiceId, id if the service for which settings are get/set      
         * @param aSettingItemName, setting item name 
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
        TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, RBuf& aBuffer ) const;
        TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC& aBuffer ) ;
        
        /**
         * Peforms Get/Set operation in the cenrep
         * @param aServiceId, id if the service for which settings are get/set      
         * @param aSettingItemName, setting item name 
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
        TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, RBuf8& aBuffer ) const;
        TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) ;
        
        /**
         * Peforms comparision operation of userid in the cenrep
         * @param aServiceId, id off the service for which user name is to be compared with cenrep store user id's      
         * @param auserId, user name to be compared with cenrep store user id's
         * @return TBool, if TOU is already shown to user return ETrue else EFalse  
         */ 
        TBool IsTOUShownL(TUint32 aServiceId, const TDesC& auserId) ;
        
        /**
         * Peforms Set operation of userid in the cenrep
         * @param aServiceId, id of the service for which user name is to be stored in cenrep store      
         * @param auserId, user name to be stored in cenrep store
         * @return TBool, successful will return ETrue else EFalse  
         */  
        TBool SetTOUShownL(TUint32 aServiceId, const TDesC& auserId );
        
    private: // helper functions
        
        void GetSAPL( TUint32 aServiceId , CVIMPSTSettings* aSettings ); 
        
        
    private:
        // owns : pointer to cen rep
        CVIMPSTSettingsCenRep* iCenrepStore;
        // owns : pointer to settings
        CVIMPSTSettings* iSettings;
    };

#endif /*__CVIMPSTSETTINGSSTORE_H*/
