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

RArray< TServiceId > gServiceIds;
RPointerArray< CSPEntry > gEntries;


CSPSettings* myCSPSettings = NULL;


extern TServicePropertyName gPropertyName;
extern TInt gContactViewPluginId;
extern HBufC* gPropertyContactStoreId;
extern HBufC* gPropertyBrandId;
extern TBool gPropertyInvalid;
extern TInt gPropertyBrandLanguage;
extern TInt gPropertyBrandVersion;
extern TInt gPropertyXIMP;
extern TOnOff gPropertyPresenceRequestPreference;
extern TInt gContactViewId;

CSPSettings* CSPSettings::NewL()
    {
    if (!myCSPSettings)
	    {
	    myCSPSettings = new ( ELeave ) CSPSettings;
	    }
    return myCSPSettings;
    }

CSPSettings* CSPSettings::NewLC()
    {
    CSPSettings* settings = CSPSettings::NewL();
    CleanupStack::PushL( settings );
    return settings;
    }

CSPSettings::~CSPSettings()
    {
    gServiceIds.Close();
    
    gEntries.ResetAndDestroy();
    gEntries.Close();
    delete gPropertyContactStoreId;
    delete gPropertyBrandId;
   
    
    myCSPSettings = NULL;
    gPropertyName = EPropertyUnknown;
	gContactViewPluginId = 0;
	gPropertyContactStoreId = 0;
	gPropertyBrandId = 0;
	gPropertyInvalid = EFalse;
	gPropertyBrandLanguage = 0;
	gPropertyBrandVersion = 0;
	gPropertyXIMP = 0;
	gPropertyPresenceRequestPreference = EOff;
	gContactViewId = 0;
    }

void CopyEntry( const CSPEntry& aOriginal, CSPEntry& aTarget )
    {    
    aTarget.SetServiceId( aOriginal.GetServiceId() );
    aTarget.SetServiceName( aOriginal.GetServiceName() );
    RPropertyArray array = aOriginal.GetAllProperties();
    for( TInt i = 0; i < array.Count(); i++ )
        {
        TRAP_IGNORE( aTarget.AddPropertyL( *array[i] ) );
        }
    }

TInt CSPSettings::AddEntryL( CSPEntry& aEntry )
    {
    TInt err = gServiceIds.Append( aEntry.GetServiceId() );
    if( err )
        {
        User::Leave( err );
        }

    CSPEntry* entry = CSPEntry::NewLC();
    CopyEntry( aEntry, *entry );
    // If property counts difference from each other,
    // alloc failure has happened in CopyEntry
    if( aEntry.GetAllProperties().Count() != 
        entry->GetAllProperties().Count() )
    	{
    	User::Leave( KErrNoMemory );
    	}

    err = gEntries.Append( entry );
    if( err )
        {
        User::Leave( err );
        }
    CleanupStack::Pop( entry );
    return KErrNone;
    }

TInt CSPSettings::FindEntryL( TServiceId aServiceId, CSPEntry& aEntry )
    {
    for( TInt i=0; i<gServiceIds.Count(); i++ )
        {
        if( gServiceIds[ i ] == aServiceId )
            {
            CSPEntry* entry = gEntries[ i ];

            CopyEntry( *entry, aEntry );
            // If property counts difference from each other,
            // alloc failure has happened in CopyEntry
            if( aEntry.GetAllProperties().Count() != 
	            entry->GetAllProperties().Count() )
	        	{
	        	User::Leave( KErrNoMemory );
	        	}

            return KErrNone;
            }
        }

    return KErrNotFound;
    }


TInt CSPSettings::UpdateEntryL( const CSPEntry& aEntry )
    {
    for( TInt i=0; i<gServiceIds.Count(); i++ )
        {
        if( gServiceIds[ i ] == aEntry.GetServiceId() )
            {
            gEntries[i]->SetServiceName( aEntry.GetServiceName() );
            RPropertyArray propOrig = gEntries[i]->GetAllProperties();
            RPropertyArray propNew = aEntry.GetAllProperties();
            TBool found = EFalse;
            for( TInt j = 0; j < propNew.Count(); j++ )
                {
                for( TInt k = 0; k < propOrig.Count() && !found; k++ )
                    {
                    if( propNew[j] == propOrig[k] )
                        {
                        found = ETrue;
                        }
                    }
                if( !found )
                    {
                    gEntries[i]->AddPropertyL( *propNew[j] );
                    }
                found = EFalse;
                }
            return KErrNone;
            }
        }
    
    return KErrNotFound;
    }
    
TInt CSPSettings::FindPropertyL( TServiceId aServiceId,
                                TServicePropertyName aPropertyName,
                                CSPProperty& aProperty )
    {
    for( TInt i=0; i<gServiceIds.Count(); i++ )
        {
        if( gServiceIds[ i ] == aServiceId )
            {
            CSPEntry* entry = gEntries[ i ];
			
			const CSPProperty* property = NULL;
			
			TInt err = entry->GetProperty( property, 
    					    aPropertyName );
    		
    		if (KErrNone == err)
	    		{
	    		aProperty.CopyL(*property);			    
	    		return KErrNone;
	    		}
            }
        }

    return KErrNotFound;    
    }


TInt CSPSettings::FindServiceIdsL( RIdArray& aServiceIds )
    {
    if( gServiceIds.Count() == 0 )
        {
        return KErrNotFound;
        }

    for( TInt i=0; i<gServiceIds.Count(); i++ )
        {
        TInt err = aServiceIds.Append( gServiceIds[ i ] );
        if( err )
            {
            User::Leave( err );
            }
        }

    return KErrNone;
    }


CSPSettings::CSPSettings()
    {
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

	               


