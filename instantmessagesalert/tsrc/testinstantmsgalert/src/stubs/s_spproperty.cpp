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

TServicePropertyName gPropertyName = EPropertyUnknown;
TInt gContactViewPluginId = 0;
HBufC* gPropertyContactStoreId = 0;
HBufC* gPropertyBrandId = 0;
TBool gPropertyInvalid = EFalse;
TInt gPropertyBrandLanguage = 0;
TInt gPropertyBrandVersion = 0;
TInt gPropertyXIMP = 0;
TOnOff gPropertyPresenceRequestPreference = EOff;
TInt gContactViewId = 0;

TOnOff gPropertyVoIPEnabled = EOff;
TInt gPropertyPresenceSettingsId = 0;
TInt gPropertyIMSettingsId = 0;
TInt gPropertyPCSPluginId = 0;

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
    if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == EPropertyContactViewPluginId )
        {
        aValue = gContactViewPluginId;
        } 
	else if( GetName() == EPropertyBrandLanguage )
        {
        aValue = gPropertyBrandLanguage;
        } 
	else if( GetName() == EPropertyBrandVersion )
        {
        aValue = gPropertyBrandVersion;
        }
	else if( GetName() == EPropertyPCSPluginId )
        {
        aValue = gPropertyXIMP;
        }  
	else if( GetName() == EPropertyContactViewId )
        {
        aValue = gContactViewId;
        }  
    else if( GetName() == ESubPropertyPresenceSettingsId )
        {
        aValue = gPropertyPresenceSettingsId;
        }  
    else if( GetName() == ESubPropertyIMSettingsId )
        {
        aValue = gPropertyIMSettingsId;
        }  
    else if( GetName() == EPropertyPCSPluginId )
        {
        aValue = gPropertyPCSPluginId;
        } 
    
    return KErrNone;
    }

TInt CSPProperty::SetValue( TInt aValue )
    {
    if( GetName() == EPropertyContactViewPluginId )
        {
        gContactViewPluginId = aValue;
        }    
	else if( GetName() == EPropertyBrandLanguage )
        {
        gPropertyBrandLanguage = aValue;
        } 
	else if( GetName() == EPropertyBrandVersion )
        {
        gPropertyBrandVersion = aValue;
        } 
	else if( GetName() == EPropertyPCSPluginId )
        {
        gPropertyXIMP = aValue;
        } 
	else if( GetName() == EPropertyContactViewId )
        {
        gContactViewId = aValue ;
        }         
    else if( GetName() == ESubPropertyPresenceSettingsId )
        {
        gPropertyPresenceSettingsId = aValue ;
        }          
    else if( GetName() == ESubPropertyIMSettingsId )
        {
        gPropertyIMSettingsId = aValue ;
        }     
    else if( GetName() == EPropertyPCSPluginId )
        {
        gPropertyPCSPluginId = aValue ;
        }   
    
    
    return KErrNone;
    }


TInt CSPProperty::GetValue( TDes& aValue ) const
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }
	if( GetName() == EPropertyContactStoreId )
        {
        if (gPropertyContactStoreId)
	        {
	        aValue.Copy(*gPropertyContactStoreId);
	        }
        }    
    else if( GetName() == EPropertyBrandId )
        {
        if (gPropertyBrandId)
	        {
	        aValue.Copy(*gPropertyBrandId);
	        }
        }   
	
	return KErrNone;        
	}

TInt CSPProperty::SetValue( const TDesC& aValue )
	{
	if( GetName() == EPropertyContactStoreId )
        {
		
        if (gPropertyContactStoreId)
	        {
	        delete gPropertyContactStoreId;
	        gPropertyContactStoreId = NULL;	
	        }
	    
	    gPropertyContactStoreId = aValue.Alloc();
        }    
    else if( GetName() == EPropertyBrandId )
        {
        if (gPropertyBrandId)
	        {
	        delete gPropertyBrandId;
	        gPropertyBrandId = NULL;	
	        }
        gPropertyBrandId = aValue.Alloc();        
        }   
	
	return KErrNone;
	}

TInt CSPProperty::GetValue( TOnOff& aValue ) const
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == ESubPropertyPresenceRequestPreference )
        {
        aValue = gPropertyPresenceRequestPreference;
        }	
    else if( GetName() == ESubPropertyVoIPEnabled )
        {
        aValue = gPropertyVoIPEnabled;
        }
	
	return KErrNone;
	}


TInt CSPProperty::SetValue( TOnOff aValue )
	{
	if( gPropertyInvalid )
        {
        return KErrNotFound;
        }

    if( GetName() == ESubPropertyPresenceRequestPreference )
        {
        gPropertyPresenceRequestPreference = aValue;
        }	 
    else if( GetName() == ESubPropertyVoIPEnabled )
        {
        gPropertyVoIPEnabled = aValue;
        } 
	
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
    iPropertyName = aSource.GetName();    
    
    if( GetName() == EPropertyContactStoreId )
        {
        HBufC* value = HBufC::NewLC(500);        
        TPtr valuePtr = value->Des();
        aSource.GetValue(valuePtr);
        SetValue( valuePtr );
        
        CleanupStack::PopAndDestroy(value);        
        }    
    else if( GetName() == EPropertyBrandId )
        {
        HBufC* value = HBufC::NewLC(500);        
        TPtr valuePtr = value->Des();
        aSource.GetValue(valuePtr);
        SetValue( valuePtr );
        
        CleanupStack::PopAndDestroy(value); 
        }   
    else  if( GetName() == EPropertyContactViewPluginId )
	    {
	    TInt value = 0;
	    aSource.GetValue( value );
	    SetValue( value );	
	    }   
    
    }


TBool CSPProperty::CSPProperty::operator==( 
            const CSPProperty& /*aProperty*/ ) const{ return ETrue; }

CSPProperty::CSPProperty(){}

void CSPProperty::ConstructL(){}


TInt SetDefaultValue( TServicePropertyName /*aProperty*/ ){ return KErrNone; }

