/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implimentation class for the settings store
*
*
*/

//  INCLUDES
#include <e32std.h>
#include "cvimpstsettingsstore.h"
#include "cvimpstsettingscenrep.h"
#include "cvimpstsettings.h"
#include <centralrepository.h>

//dummy serviceId, assuming the service id not more than 256. for the first time
//and it is assumed that this service id is for initializing the cenrep keys only.
//not updated in the service table and hence will not get a TAB in phone book
const TInt KServiceIDOffset = 16;

//================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CVIMPSTSettingsStore::NewLC
// 
// ---------------------------------------------------------
EXPORT_C MVIMPSTSettingsStore* CVIMPSTSettingsStore::NewLC()
    {
    CVIMPSTSettingsStore* self = new (ELeave) CVIMPSTSettingsStore( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }


// ---------------------------------------------------------
// CVIMPSTSettingsStore::NewL
// 
// ---------------------------------------------------------
EXPORT_C MVIMPSTSettingsStore* CVIMPSTSettingsStore::NewL()
    {
    MVIMPSTSettingsStore* self = CVIMPSTSettingsStore::NewLC();
    CleanupStack::Pop( );//self
    return self;
    }
    
// ---------------------------------------------------------
// CVIMPSTSettingsStore::Destructor
// 
// ---------------------------------------------------------
CVIMPSTSettingsStore::~CVIMPSTSettingsStore()
    {
    if(iCenrepStore)
        {
        delete iCenrepStore;
        iCenrepStore = NULL;
        }
    if(iSettings)
        {
        delete iSettings;
        iSettings = NULL;
        }
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStore::ConstructL
// 
// ---------------------------------------------------------

 void CVIMPSTSettingsStore::ConstructL( )
    {
    iCenrepStore = CVIMPSTSettingsCenRep::NewL();
    // Get settings from vimpstsettings store to work with   
    iSettings = CVIMPSTSettings::NewL(); 
    }
 

// ---------------------------------------------------------
// CVIMPSTSettingsStore::Get
// 
// ---------------------------------------------------------
TInt CVIMPSTSettingsStore::GetL(TUint32 aServiceId, TSettingItemName aSettingItemName, TInt& aValue ) const
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Get( maskedId + aSettingItemName, aValue );
    if( retError == KErrNotFound)
        {
        //if key not found, create a new key with the defualt value 0
        aValue = 0;
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aValue ) ;
        }
    return retError; // return the value : error code  
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStore::Set
// 
// ---------------------------------------------------------
TInt CVIMPSTSettingsStore::SetL(TUint32 aServiceId, TSettingItemName aSettingItemName, const TInt aValue  )
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Set( maskedId + aSettingItemName, aValue );    
    if( retError == KErrNotFound )
        {       
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aValue );
        }
    return retError; // return the value : error code  
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStore::Get
// 
// ---------------------------------------------------------

TInt CVIMPSTSettingsStore::GetL(TUint32 aServiceId, TSettingItemName aSettingItemName, RBuf& aBuffer  ) const
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Get( maskedId + aSettingItemName, aBuffer );
    if( retError == KErrNotFound)
        {
        aBuffer = _L(" ");
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aBuffer ) ;
        }
    return retError;     // return the value : error code  
    }

// ---------------------------------------------------------
// CVIMPSTSettingsStore::Set
// 
// ---------------------------------------------------------

TInt CVIMPSTSettingsStore::SetL(TUint32 aServiceId, TSettingItemName aSettingItemName, const TDesC& aBuffer  )
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Set( maskedId + aSettingItemName, aBuffer );
    if( retError == KErrNotFound)
        {        
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aBuffer );
        }
    return retError;  // return the value : error code  
    }

// -----------------------------------------------------------------------------------------------------------
// CVIMPSTSettingsStore::GetL
// 
// ------------------------------------------------------------------------------------------------------------

TInt CVIMPSTSettingsStore::GetL(TUint32 aServiceId, TSettingItemName aSettingItemName, RBuf8& aBuffer  ) const
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Get( maskedId + aSettingItemName, aBuffer );
    if( retError == KErrNotFound)
        { 
        // if not found set return value to null descriptor and create in the cenrep
        aBuffer = _L8("");
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aBuffer  ) ;  
        }
    return retError; // return the value : error code  
    }

// --------------------------------------------------------------------------------------------------
// CVIMPSTSettingsStore::SetL
// 
// ---------------------------------------------------------------------------------------------------

TInt CVIMPSTSettingsStore::SetL(TUint32 aServiceId, TSettingItemName aSettingItemName, const TDesC8& aBuffer  )
    {
    TInt maskedId = aServiceId << KServiceIDOffset ;
    TInt retError = iCenrepStore->Repository()->Set( maskedId + aSettingItemName, aBuffer );
    if( retError == KErrNotFound)
        { // if not found create  
        retError = iCenrepStore->Repository()->Create( maskedId + aSettingItemName, aBuffer );
        }
    return retError; // return the value : Error code
    }

// ---------------------------------------------------------------------------------------------------
// CVIMPSTSettingsStore::GetSAPL
// 
// ----------------------------------------------------------------------------------------------------
void CVIMPSTSettingsStore::GetSAPL( TUint32 aServiceId, CVIMPSTSettings* aSettings )
    {
    iCenrepStore->GetSettingsL( aServiceId, aSettings );
    }

//End of file
