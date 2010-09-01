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
* Description: s_spentry.cpp
*
*/

#include <spentry.h>
#include <spproperty.h>


CSPEntry* CSPEntry::NewL()
    {
    CSPEntry* self = CSPEntry::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

CSPEntry* CSPEntry::NewLC()
    {
    CSPEntry* self = new ( ELeave) CSPEntry;
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }

CSPEntry::~CSPEntry()
    {
    iServiceName.Close();
    iPropertyArray.ResetAndDestroy();
    iPropertyArray.Close();
    }

TServiceId CSPEntry::GetServiceId() const
    {
    return iServiceId;
    }

void CSPEntry::SetServiceId( TServiceId aServiceId ) 
    {
    iServiceId = aServiceId;
    }

const TDesC16& CSPEntry::GetServiceName() const
    {
    return iServiceName;
    }

TInt CSPEntry::SetServiceName( const TDesC16& aServiceName )
    {
    
    iServiceName.Copy(aServiceName);
    return KErrNone;
    }

TInt CSPEntry::AddPropertyL( const CSPProperty& aProperty )
    {
    CSPProperty* property = CSPProperty::NewL();
    CleanupStack::PushL( property );

    property->CopyL( aProperty );

    TInt err = iPropertyArray.Append( property );
    if( err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    
    CleanupStack::Pop( property );

    return KErrNone;
    }


TInt CSPEntry::UpdateProperty( TServicePropertyName /*aPropertyName*/, TInt /*aValue*/ )
    {
    return KErrNone;
    }


TInt CSPEntry::UpdateProperty( TServicePropertyName /*aPropertyName*/, const TDesC& /*aValue*/ )
    {
    return KErrNone;
    }

TInt CSPEntry::UpdateProperty( TServicePropertyName /*aPropertyName*/, TOnOff /*aValue*/ )
    {
    return KErrNone;
    }

TInt CSPEntry::GetProperty( const CSPProperty*& aProperty, 
    					    TServicePropertyName aPropertyName ) const
    {
    TInt count = iPropertyArray.Count();
    for( TInt i=0; i<count; i++ )
        {        
        if( iPropertyArray[ i ]->GetName() == aPropertyName )
            {
            aProperty = iPropertyArray[ i ];
            return KErrNone;
            }
        }

    return KErrNotFound;
    }

TInt CSPEntry::GetProperty( const CSPProperty*& /*aProperty*/, 
    								TInt /*aIndex*/ ) const
    {
    return KErrNone;
    }

TInt CSPEntry::DeleteProperty( TInt /*aIndex*/ )
    {
    return KErrNone;
    }

TInt CSPEntry::DeleteProperty( TServicePropertyName /*aProperty*/ )
    {
    return KErrNone;
    }

const RPropertyArray& CSPEntry::GetAllProperties() const
    {
    return iPropertyArray;
    }

TInt CSPEntry::PropertyCount() const
    {
    return KErrNone;
    }

CSPEntry::CSPEntry()
    {
    }

void CSPEntry::ConstructL()
    {
    iServiceName.CreateL(500);
    }

TInt CSPEntry::SearchProperty( TServicePropertyName /*aProperty*/ ) const
    {
    return KErrNone;
    }


