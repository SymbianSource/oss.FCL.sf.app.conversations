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
* Description:  vimpst settings store definitions and repository transactions.
*
*/

#ifndef __CVIMPSTSETTINGSSTOREIMP_H
#define __CVIMPSTSETTINGSSTOREIMP_H

//  INCLUDES
#include <e32base.h>
#include "s_vimpstsettingsstore.h"

NONSHARABLE_CLASS( CVIMPSTSettingsStoreImp):public CBase,                                            
                                            public MVIMPSTSettingsStore
{

    public: // Constructors and destructor

        /**
         * Initialises the Library
         */
        static MVIMPSTSettingsStore* InitialiseLibraryL();

        /**
         * Release the resources allocated 
         */
        static void Release();
        
        /**
         * Destructors.
         */
        virtual ~CVIMPSTSettingsStoreImp();
        
        /**
         * Peforms Get/Set operation in the cenrep
         * @param aServiceId, id if the service for which settings are get/set      
         * @param aSettingItemName, setting item name 
         * @param aBuffer, variable to set/get the value
         * @return TInt, used as error passing value.  
         */  
       TInt SetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) ;
       
       TInt GetL(TUint32 aServiceId, TSettingItemName aSettingItemNAme, const TDesC8& aBuffer ) ;
            
        
    private:        
        /**
         * Two-phased constructor.
         */
        static MVIMPSTSettingsStore* NewL();
        
        /**
         * Performs the 2nd phase of construction.
         */
        void ConstructL();
        
    
    };

#endif //__CVIMPSTSETTINGSSTOREIMP_H
