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
    EURLRegistrationOff = 0x00000001,             // key value to enable or disable URL registration query
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
    EServiceCancelLoginNotSupported = 0x00000008        
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
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
        virtual TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) = 0;
        
        virtual TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) = 0;
        
    };

/**
 *  vimpst settings API class
 *
 *  Main class of managing vimpst settings
 *
 *  @code example code of how to get/set a value to the setting item in cenrep 
 *  MVIMPSTSettingsStore* obj = CVIMPSTUiSettingsStoreFactory::NewL();
 *  TInt value = 1;
 *  TInt err = obj->Set(serviceId, ETOUDlgOff, value );
 *  similarly for getting the value
 *  obj->Get(serviceId, ETOUDlgOff, value);
 *  @lib vimpstsettingsstore.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CVIMPSTSettingsStore ): public CBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static MVIMPSTSettingsStore* NewL();
        
        /**
         * Two-phased constructor.
         */
        IMPORT_C static MVIMPSTSettingsStore* NewLC();
        
        /**
        * Destructors.
        */
        virtual ~CVIMPSTSettingsStore();  
    };

#endif /*__CVIMPSTSETTINGSSTORE_H*/
