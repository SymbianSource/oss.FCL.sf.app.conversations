/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_spsettings.cpp
*
*/

#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include "s_cch.h"
CSPSettings* mycspsettings = NULL;
extern CSPEntry* mycspentry ;
CSPSettings::CSPSettings() 
    {
    }
CSPSettings* CSPSettings::NewL()
    {
    CSPSettings* self = CSPSettings::NewLC( );
    CleanupStack::Pop( self );
    return self;
    }
CSPSettings* CSPSettings::NewLC()
    {
    CSPSettings* self = new (ELeave) CSPSettings( );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CSPSettings::~CSPSettings()
    {
    }


TInt CSPSettings::AddEntryL( CSPEntry& aEntry )
    {
    return KErrNone;
    }
void CopyEntry( const CSPEntry& aOriginal, CSPEntry& aTarget )
    {    
    aTarget.SetServiceId( aOriginal.GetServiceId() );
    aTarget.SetServiceName( aOriginal.GetServiceName() );
    }

TInt CSPSettings::FindEntryL( TServiceId aServiceId, CSPEntry& aEntry )
    {
    aEntry.SetServiceName(KTestServiceName);
    aEntry.SetServiceId(KTestServiceId);
    return KErrNone;
    }


TInt CSPSettings::UpdateEntryL( const CSPEntry& aEntry )
    {
    return KErrNotFound;
    }

TInt CSPSettings::FindPropertyL( TServiceId aServiceId,
        TServicePropertyName aPropertyName,
        CSPProperty& aProperty )
    {
    if(aPropertyName == EPropertyPCSPluginId)
        {
        TInt protocolUidValue = 0x20007B6D;
        aProperty.SetValue(protocolUidValue );       
        }
    if(aPropertyName == EPropertyBrandId)
        {
        _LIT(brand,"MyBrand");
        aProperty.SetValue(brand);       
        }
    return KErrNone;

    }
TInt CSPSettings::FindServiceIdsL( RIdArray& aServiceIds )
    {
    return KErrNone;
    }
void CSPSettings::ConstructL()
    {
    }
TInt CSPSettings::SettingsCountL()
    {
    return 0;
    }
TInt CSPSettings::AddOrUpdatePropertyL( TServiceId /*aServiceId*/, 
        const CSPProperty& /*aProperty*/ )
    {
    return KErrNone;	
    }




