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
* Description:  Implementation of detailsview plugin control container
 *
*/
// settings

#include    <spsettings.h>
#include    <spentry.h>
#include    <spproperty.h>
#include <e32property.h>
#include <imconnectionproviderconsts.h>

#include "cvimpstdetailsviewpluginfactory.h"
#include "cvimpstdetailsviewplugin.h"
#include "vimpstdebugtrace.h"
// meco service uid
#define KMECOIMPLEMENTATIONUID 0x20012423
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::NewL
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPluginFactory* CVIMPSTDetailsViewPluginFactory::NewL()  
    {
    CVIMPSTDetailsViewPluginFactory* self = new ( ELeave ) CVIMPSTDetailsViewPluginFactory();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();//self, do not pass the parameter to pop since this is an M Class..
    return self;    
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::CVIMPSTDetailsViewPluginFactory
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPluginFactory::CVIMPSTDetailsViewPluginFactory():
    iServiceId( 0 )
    {
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::ConstructL
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsViewPluginFactory::ConstructL()
    {
    TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::ConstructL() start") );
    RProperty::Define(KMeCoPropertyUid, KMeCoXSPDetailsServiceIdKey, RProperty::EInt,
                             ECapabilityReadUserData ,ECapabilityWriteUserData); 
    
    ReadServiceIdL();
    if(iServiceIds.Count() == 0)
        {
        User::Leave(KErrArgument);
        }
    TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::ConstructL() end") );
    }
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::~CVIMPSTDetailsViewPluginFactory
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsViewPluginFactory::~CVIMPSTDetailsViewPluginFactory()
    {
    iServiceIds.Reset();
    iServiceIds.Close();
    RProperty::Set(KMeCoPropertyUid, KMeCoXSPDetailsServiceIdKey, 0 );  //reset to zero
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::TabViewCount
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsViewPluginFactory::TabViewCount()
    {
    // read the service id from RProperty
    RProperty::Get(KMeCoPropertyUid, KMeCoXSPDetailsServiceIdKey, iServiceId ); 
    TInt returnVal = 0;
    if( iServiceId <= 0 )
        {
        TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::TabViewCount() iservice <=0") );
        return iServiceIds.Count();
        }
    else
        {
        TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::TabViewCount() iservice > 0") );
        iServiceIds.Reset();
        iServiceIds.Append( iServiceId );
        returnVal = 1;
        }
    return returnVal;
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::CreateTabViewL
// ---------------------------------------------------------------------------
//
CCCAppViewPluginBase* CVIMPSTDetailsViewPluginFactory::CreateTabViewL( TInt aIndex )
    {
    TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::CreateTabViewL() start") );
    TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::CreateTabViewL() aIndex = %d"),aIndex );
    CCCAppViewPluginBase* viewPluginBase = NULL;    
    if( aIndex < iServiceIds.Count() && aIndex >= 0 )
        {
        TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::CreateTabViewL() creating view aIndex = %d"),aIndex );
        viewPluginBase = CVIMPSTDetailsViewPlugin::NewL( iServiceIds[aIndex]);
        TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::CreateTabViewL() view created") );
        }
    else
        {
        User::Leave(KErrArgument);
        }   
    TRACED( T_LIT("CVIMPSTDetailsViewPluginFactory::CreateTabViewL() end ") );
    return viewPluginBase;
    }
 
// ---------------------------------------------------------------------------
// CVIMPSTDetailsViewPluginFactory::ReadServiceIdL
// ---------------------------------------------------------------------------
// 
TInt CVIMPSTDetailsViewPluginFactory::ReadServiceIdL()
    {
    CSPSettings* settings = CSPSettings::NewLC();
    iServiceIds.Reset();
    ///////////////////////Get Service Id////////////////    
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    settings->FindServiceIdsL( services );
    const TInt serviceCount = services.Count();
    for ( TInt i( 0 ); i < serviceCount; i++ )
        {
        TInt refContactUid = KErrNotFound;
        CSPProperty* property = CSPProperty::NewLC();
        settings->FindPropertyL( services[i],EPropertyContactViewPluginId,*property );    
        if ( property )
            {
            property->GetValue( refContactUid );
            }
        CleanupStack::PopAndDestroy( property );
        if ( (TUid::Uid( refContactUid ) == TUid::Uid( KMECOIMPLEMENTATIONUID ))
                || (TUid::Uid( refContactUid ) ==  TUid::Uid( 0x20007B6D ) )
                || (TUid::Uid( refContactUid ) == TUid::Uid( 0x2002B334 )))
            {
            iServiceIds.Append( services[i] );
            }
        }
    CleanupStack::PopAndDestroy( &services );
    CleanupStack::PopAndDestroy( settings );
    return iServiceIds.Count();
    }
