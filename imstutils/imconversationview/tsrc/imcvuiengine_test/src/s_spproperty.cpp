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
* Description: s_spproperty.cpp
*
*/
#include <spproperty.h>

const TInt KMaxIntLen = 15;
CSPProperty* CSPProperty::NewL()
    {
    return new ( ELeave ) CSPProperty;
    }
CSPProperty* CSPProperty::NewLC()
    {
    CSPProperty* self = new ( ELeave ) CSPProperty;
    CleanupStack::PushL( self );
    return self;
    }
CSPProperty::~CSPProperty(){}
TServicePropertyName CSPProperty::GetName() const
    {
    return iPropertyName;
    }
TInt CSPProperty::SetName( TServicePropertyName aPropertyname )
    {
    iPropertyName = aPropertyname;
    return KErrNone;
    }
TSPItemType CSPProperty::GetPropertyType() const
    {
    TSPItemType type = EItemTypeNotDefined;
    return type;
    }
TInt CSPProperty::SetPropertyType( TSPItemType /*aPropertyType*/ ){ return KErrNone; }
TPropertyDataType CSPProperty::GetDataType() const
    {
    TPropertyDataType type = EDataTypeNotSet;
    return type;
    }
void CSPProperty::SetDataType( TPropertyDataType /*aDataType*/ ){}
TInt CSPProperty::GetValue( TInt& aValue ) const
    {
    TInt protocolUidValue = 0x20007B6D;  ////// dummy adaptation UID
    aValue = protocolUidValue;
    return KErrNone;
    }
TInt CSPProperty::SetValue( TInt aValue )
    { 
    return KErrNone;
    }

TInt CSPProperty::GetValue( TDes& aValue ) const
{
aValue.Zero();

if ( aValue.MaxLength() >= iPropertyValue.Length() )
    {
    aValue.Copy( iPropertyValue );
    return KErrNone;
    }

}

TInt CSPProperty::SetValue( const TDesC& aValue )
    {
    TInt ret( 0 );
    iPropertyValue.Close();
    ret = iPropertyValue.Create( aValue );
    return ret;
    }

TInt CSPProperty::GetValue( TOnOff& aValue ) const
	{
	return KErrNone;
	}
TInt CSPProperty::SetValue( TOnOff aValue )
	{
	return KErrNone;
	}
TInt CSPProperty::SetValueNoDataTypeCheck( const TDesC& /*aValue*/ ){ return KErrNone; }
TPropertyDataType CSPProperty::DataType( TServicePropertyName /*aProperty*/ )
    {
    TPropertyDataType type = EDataTypeNotSet;
    return type;
    }
TSPItemType CSPProperty::PropertyType( TServicePropertyName /*aProperty*/ )
    {
    TSPItemType type = EItemTypeNotDefined;
    return type;
    }
void CSPProperty::CopyL( const CSPProperty& aSource )
    {
    }
TBool CSPProperty::CSPProperty::operator==( 
            const CSPProperty& /*aProperty*/ ) const{ return ETrue; }
CSPProperty::CSPProperty(){}
void CSPProperty::ConstructL(){}
TInt SetDefaultValue( TServicePropertyName /*aProperty*/ ){ return KErrNone; }

